#include "fmt_internal.h"
#include "fmt_internal_ryu.h"
#include "dh/mem/common.h"
#include "dh/ascii.h"

/*========== Constants ======================================================*/

#define fmt__ryu_special_exponent 0x7fffffff

/*========== Structures =====================================================*/

typedef struct FltDecimal {
    u64 mantissa;
    i32 exponent;
    bool sign;
} FltDecimal;

/*========== Helper Functions - Bit Manipulation ===========================*/

/// Convert f32 to u32 bit pattern (no __builtin_bit_cast)
$attr($inline_always)
$static fn_((f32ToBits(f32 f))(u32)) {
    return *mem_bytesAs$(u32*, mem_asBytes(&f));
};

/// Convert f64 to u64 bit pattern (no __builtin_bit_cast)
$attr($inline_always)
$static fn_((f64ToBits(f64 f))(u64)) {
    return *mem_bytesAs$(u64*, mem_asBytes(&f));
};

/*========== Helper Functions - Decimal Operations =========================*/

/// Calculate decimal length of a 64-bit integer
$attr($inline_always)
$static fn_((decimalLen64(u64 v))(u32)) {
    if (v >= 10000000000000000ull) { return 17; }
    if (v >= 1000000000000000ull) { return 16; }
    if (v >= 100000000000000ull) { return 15; }
    if (v >= 10000000000000ull) { return 14; }
    if (v >= 1000000000000ull) { return 13; }
    if (v >= 100000000000ull) { return 12; }
    if (v >= 10000000000ull) { return 11; }
    if (v >= 1000000000ull) { return 10; }
    if (v >= 100000000ull) { return 9; }
    if (v >= 10000000ull) { return 8; }
    if (v >= 1000000ull) { return 7; }
    if (v >= 100000ull) { return 6; }
    if (v >= 10000ull) { return 5; }
    if (v >= 1000ull) { return 4; }
    if (v >= 100ull) { return 3; }
    if (v >= 10ull) { return 2; }
    return 1;
};

/// Write decimal digits using 2-digit optimization
$static fn_((writeDecimal(u8* buf, u64* value, usize count))(void)) {
    // Two-digit lookup table for 00-99
    $static let_(digits2, A$$(200, u8)) = A_init({ /* clang-format off */
        '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
        '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
        '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
        '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
        '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
        '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
        '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
        '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
        '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
        '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'
    }); /* clang-format on */
    var_(i, usize) = 0;
    while (i + 2 < count) {
        let c = as$(u8)(*value % 100);
        *value /= 100;
        buf[count - i - 1] = *A_at((digits2)[c * 2 + 1]);
        buf[count - i - 2] = *A_at((digits2)[c * 2]);
        i += 2;
    }
    while (i < count) {
        let c = as$(u8)(*value % 10);
        *value /= 10;
        buf[count - i - 1] = u8_c('0') + c;
        i++;
    }
};

/// Check if number is power of 10
$static fn_((isPow10(u64 n))(bool)) {
    while (n != 0) {
        if (n % 10 != 0) { return false; }
        n /= 10;
    }
    return true;
};

/*========== Helper Functions - Math =======================================*/

/// floor(log_10(2^e))
$attr($inline_always)
$static fn_((log10Pow2(u32 e))(u32)) {
    return as$(u32)((as$(u64)(e) * 169464822037455ull) >> 49);
};

/// floor(log_10(5^e))
$attr($inline_always)
$static fn_((log10Pow5(u32 e))(u32)) {
    return as$(u32)((as$(u64)(e) * 196742565691928ull) >> 48);
};

/// Count trailing powers of 5
$static fn_((pow5Factor64(u64 value))(u32)) {
    u32 count = 0;
    while (value > 0) {
        if (value % 5 != 0) { return count; }
        count++;
        value /= 5;
    }
    return 0;
};

/// Check if value is divisible by 5^p
$static fn_((mulPow5(u64 value, u32 p))(bool)) {
    return pow5Factor64(value) >= p;
};

/// Check if value is divisible by 2^p
$static fn_((mulPow2(u64 value, u32 p))(bool)) {
    return (value & ((1ull << p) - 1)) == 0;
};

/*========== Binary to Decimal Conversion ==================================*/

/// Convert f64 binary representation to decimal
$static fn_((bitsToDecimal64(u64 bits))(FltDecimal)) {
    const u32 bias = 1023;
    const u32 mantissa_bits = 52;
    const u32 exponent_bits = 11;

    const bool ieee_sign = ((bits >> 63) & 1) != 0;
    const u64 ieee_mantissa = bits & ((1ull << mantissa_bits) - 1);
    const u32 ieee_exponent = as$(u32)((bits >> mantissa_bits) & ((1u << exponent_bits) - 1));

    // Handle zero
    if (ieee_exponent == 0 && ieee_mantissa == 0) {
        return (FltDecimal){ .mantissa = 0, .exponent = 0, .sign = ieee_sign };
    }
    // Handle infinity and NaN
    if (ieee_exponent == ((1u << exponent_bits) - 1)) {
        return (FltDecimal){
            .mantissa = ieee_mantissa,
            .exponent = fmt__ryu_special_exponent,
            .sign = ieee_sign
        };
    }

    // Compute m2 and e2
    i32 e2 = 0;
    u64 m2 = 0;
    if (ieee_exponent == 0) {
        // Denormalized
        e2 = 1 - as$(i32)(bias) - as$(i32)(mantissa_bits)-2;
        m2 = ieee_mantissa;
    } else {
        // Normalized
        e2 = as$(i32)(ieee_exponent) - as$(i32)(bias) - as$(i32)(mantissa_bits)-2;
        m2 = (1ull << mantissa_bits) | ieee_mantissa;
    }

    const bool even = (m2 & 1) == 0;
    const bool accept_bounds = even;

    const u64 mv = 4 * m2;
    const u32 mm_shift = ((ieee_mantissa != 0) || (ieee_exponent == 0)) ? 1 : 0;

    // Convert to decimal
    u64 vr = 0;
    u64 vp = 0;
    u64 vm = 0;
    i32 e10 = 0;
    bool vm_is_trailing_zeros = false;
    bool vr_is_trailing_zeros = false;
    if (e2 >= 0) {
        u32 q = log10Pow2(as$(u32)(e2));
        if (e2 > 3) { q--; }
        e10 = as$(i32)(q);

        const i32 k = fmt__ryu_bit_count + as$(i32)(fmt__ryu_pow5Bits(q)) - 1;
        const u32 i = as$(u32)(-e2 + as$(i32)(q) + k);
        let pow5_inv = fmt__ryu_pow5Inv(q);
        vr = fmt__ryu_mulShift64(4 * m2, pow5_inv, i);
        vp = fmt__ryu_mulShift64(4 * m2 + 2, pow5_inv, i);
        vm = fmt__ryu_mulShift64(4 * m2 - 1 - mm_shift, pow5_inv, i);

        if (q <= 21) {
            if (mv % 5 == 0) {
                vr_is_trailing_zeros = mulPow5(mv, q);
            } else if (accept_bounds) {
                vm_is_trailing_zeros = mulPow5(mv - 1 - mm_shift, q);
            } else {
                vp -= mulPow5(mv + 2, q) ? 1 : 0;
            }
        }
    } else {
        u32 q = log10Pow5(as$(u32)(-e2));
        if (-e2 > 1) { q--; }
        e10 = as$(i32)(q) + e2;

        const i32 i = -e2 - as$(i32)(q);
        const i32 k = as$(i32)(fmt__ryu_pow5Bits(as$(u32)(i))) - fmt__ryu_bit_count;
        const u32 j = as$(u32)(as$(i32)(q)-k);
        let pow5 = fmt__ryu_pow5(as$(u32)(i));
        vr = fmt__ryu_mulShift64(4 * m2, pow5, j);
        vp = fmt__ryu_mulShift64(4 * m2 + 2, pow5, j);
        vm = fmt__ryu_mulShift64(4 * m2 - 1 - mm_shift, pow5, j);

        if (q <= 1) {
            vr_is_trailing_zeros = true;
            if (accept_bounds) {
                vm_is_trailing_zeros = (mm_shift == 1);
            } else {
                vp -= 1;
            }
        } else if (q < 63) {
            vr_is_trailing_zeros = mulPow2(mv, q);
        }
    }

    // Find shortest representation
    u32 removed = 0;
    u8 last_removed_digit = 0;
    while (vp / 10 > vm / 10) {
        vm_is_trailing_zeros = vm_is_trailing_zeros && (vm % 10 == 0);
        vr_is_trailing_zeros = vr_is_trailing_zeros && (last_removed_digit == 0);
        last_removed_digit = (u8)(vr % 10);
        vr /= 10;
        vp /= 10;
        vm /= 10;
        removed++;
    }
    if (vm_is_trailing_zeros) {
        while (vm % 10 == 0) {
            vr_is_trailing_zeros = vr_is_trailing_zeros && (last_removed_digit == 0);
            last_removed_digit = (u8)(vr % 10);
            vr /= 10;
            vp /= 10;
            vm /= 10;
            removed++;
        }
    }
    if (vr_is_trailing_zeros && (last_removed_digit == 5) && (vr % 2 == 0)) {
        last_removed_digit = 4;
    }
    let round_up = (vr == vm && (!accept_bounds || !vm_is_trailing_zeros)) || (last_removed_digit >= 5);
    return (FltDecimal){
        .mantissa = vr + (round_up ? 1 : 0),
        .exponent = e10 + as$(i32)(removed),
        .sign = ieee_sign
    };
};

/// Convert f32 to FltDecimal (convert to f64 first for simplicity)
$static fn_((bitsToDecimal32(u32 bits))(FltDecimal)) {
    // Convert f32 to f64 for processing
    let f32_val = *mem_bytesAs$(f32*, mem_asBytes(&bits));
    let f64_val = as$(f64)(f32_val);
    return bitsToDecimal64(f64ToBits(f64_val));
};

/*========== Rounding =======================================================*/

/// Round FltDecimal to specified precision
$static fn_((roundDecimal(FltDecimal f, bool is_scientific, u8 precision))(FltDecimal)) {
    let olen = decimalLen64(f.mantissa);
    u32 round_digit = 0;
    u64 mantissa = f.mantissa;
    i32 exponent = f.exponent;
    if (is_scientific) {
        round_digit = 1 + precision;
    } else {
        if (f.exponent > 0) {
            round_digit = (olen - 1) + precision + as$(u32)(f.exponent);
        } else {
            const u32 min_exp_required = as$(u32)(-f.exponent);
            if (precision + olen > min_exp_required) {
                round_digit = precision + olen - min_exp_required;
            }
        }
    }
    if (round_digit < olen) {
        for (u32 i = round_digit + 1; i < olen; ++i) {
            mantissa /= 10;
            exponent += 1;
        }
        if (mantissa % 10 >= 5) {
            mantissa /= 10;
            mantissa += 1;
            exponent += 1;
            if (isPow10(mantissa)) {
                mantissa /= 10;
                exponent += 1;
            }
        }
    }
    return (FltDecimal){ .mantissa = mantissa, .exponent = exponent, .sign = f.sign };
};

/*========== Formatting ====================================================*/

/// Format FltDecimal in decimal notation (123.456)
$static fn_((formatDecimal(S$u8 buf, FltDecimal f, O$u8 precision))(usize)) {
    if (f.exponent == fmt__ryu_special_exponent) {
        usize index = 0;
        if (f.sign) { *S_at((buf)[index++]) = u8_c('-'); }
        if (f.mantissa != 0) {
            *S_at((buf)[index++]) = u8_c('n');
            *S_at((buf)[index++]) = u8_c('a');
            *S_at((buf)[index++]) = u8_c('n');
        } else {
            *S_at((buf)[index++]) = u8_c('i');
            *S_at((buf)[index++]) = u8_c('n');
            *S_at((buf)[index++]) = u8_c('f');
        }
        return index;
    }

    let rounded = expr_(FltDecimal $scope)(if_some((precision)(prec)) {
        $break_(roundDecimal(f, false, prec));
    } else_none {
        $break_(f);
    }) $unscoped_(expr);
    u64 mantissa = rounded.mantissa;
    let olen = decimalLen64(mantissa);

    usize index = 0;
    if (rounded.sign) { *S_at((buf)[index++]) = u8_c('-'); }
    let dp_offset = rounded.exponent + as$(i32)(olen);
    if (dp_offset <= 0) {
        // 0.000001234
        *S_at((buf)[index++]) = u8_c('0');
        *S_at((buf)[index++]) = u8_c('.');
        let dp_index = index;
        let dp_poffset = as$(u32)(-dp_offset);
        for (u32 i = 0; i < dp_poffset; ++i) {
            *S_at((buf)[index++]) = u8_c('0');
        }
        writeDecimal(buf.ptr + index, &mantissa, olen);
        index += olen;
        if_some((precision)(prec)) {
            let dp_written = index - dp_index;
            if (dp_written < prec) {
                for (usize i = 0; i < prec - dp_written; ++i) {
                    *S_at((buf)[index + i]) = u8_c('0');
                }
            }
            index = dp_index + prec - ((prec == 0) ? 1 : 0);
        }
    } else {
        let dp_uoffset = as$(u32)(dp_offset);
        if (dp_uoffset >= olen) {
            // 123456000
            writeDecimal(buf.ptr + index, &mantissa, olen);
            index += olen;
            for (u32 i = 0; i < dp_uoffset - olen; ++i) {
                *S_at((buf)[index++]) = u8_c('0');
            }
            if_some((precision)(prec)) {
                if (prec != 0) {
                    *S_at((buf)[index++]) = u8_c('.');
                    for (u8 i = 0; i < prec; ++i) {
                        *S_at((buf)[index++]) = u8_c('0');
                    }
                }
            }
        } else {
            // 12345.6789
            writeDecimal(buf.ptr + index + dp_uoffset + 1, &mantissa, olen - dp_uoffset);
            *S_at((buf)[index + dp_uoffset]) = u8_c('.');
            let dp_index = index + dp_uoffset + 1;
            writeDecimal(buf.ptr + index, &mantissa, dp_uoffset);
            index += olen + 1;
            if_some((precision)(prec)) {
                let dp_written = olen - dp_uoffset;
                if (dp_written < prec) {
                    for (usize i = 0; i < prec - dp_written; ++i) {
                        *S_at((buf)[index + i]) = u8_c('0');
                    }
                }
                index = dp_index + prec - ((prec == 0) ? 1 : 0);
            }
        }
    }
    return index;
};

fn_((fmt__formatFlt(io_Writer writer, f64 val, fmt_Spec spec))(E$void)) {
    // Get formatting options
    let use_upper = (spec.type == fmt_Type_float_upper);
    // Default precision to 6 for C compatibility
    let precision = (O$u8)some(orelse_((spec.precision)(6)));
    // Convert to binary representation and then to decimal
    let d = expr_(FltDecimal $scope)(if (spec.size == fmt_Size_32) {
        $break_(bitsToDecimal32(f32ToBits(as$(f32)(val))));
    } else {
        $break_(bitsToDecimal64(f64ToBits(val)));
    }) $unscoped_(expr);
    // Handle special values
    if (d.exponent == fmt__ryu_special_exponent) {
        if (d.mantissa != 0) {
            return fmt__writePadded(writer, use_upper ? u8_l("NAN") : u8_l("nan"), spec);
        }
        if (d.sign) {
            return fmt__writePadded(writer, use_upper ? u8_l("-INF") : u8_l("-inf"), spec);
        }
        return fmt__writePadded(writer, use_upper ? u8_l("INF") : u8_l("inf"), spec);
    }
    // Format to buffer
    A$$(512, u8) buf = A_zero();
    let pos = formatDecimal(A_ref$((S$u8)buf), d, precision);
    // Handle sign formatting for positive numbers
    A$$(512, u8) final_buf = A_zero();
    usize final_pos = 0;
    if (!d.sign && *atA(buf, 0) != u8_c('-')) {
        switch (spec.sign) {
        case fmt_Sign_always:
            *atA(final_buf, final_pos++) = u8_c('+');
            break;
        case fmt_Sign_space:
            *atA(final_buf, final_pos++) = u8_c(' ');
            break;
        case fmt_Sign_auto:
            break;
        }
    }
    // Copy formatted number
    for (usize i = 0; i < pos; ++i) {
        *atA(final_buf, final_pos++) = *atA(buf, i);
    }
    // Apply case conversion
    if (use_upper) {
        for (usize i = 0; i < final_pos; ++i) {
            let ch = *atA(final_buf, i);
            if (ascii_isAlpha(ch)) {
                *atA(final_buf, i) = ch - u8_c('a') + u8_c('A');
            }
        }
    }
    S_const$u8 content = { .ptr = ptr$A(final_buf), .len = final_pos };
    return fmt__writePadded(writer, content, spec);
};
