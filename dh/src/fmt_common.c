#include "fmt_internal.h"
/* FIXME: correct format specifier parsing logic `[fill][alignment][sign][#][width]` */

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((fmt__digitToInt(u8 c))(u8));
$attr($inline_always)
$static fn_((fmt__skipWhitespace(S_const$u8 str))(S_const$u8));

$attr($must_check)
$static fn_((fmt__parseU8(S_const$u8 str))(E$u8));
typedef struct fmt__ConsumedU8 {
    var_(value, u8);
    var_(remain, S_const$u8);
} fmt__ConsumedU8;
T_use_E$(fmt__ConsumedU8);
$attr($must_check)
$static fn_((fmt__consumeU8(S_const$u8 str))(E$fmt__ConsumedU8));
typedef struct fmt__ParsedFormat {
    var_(spec, fmt_Spec);
    var_(remain, S_const$u8);
} fmt__ParsedFormat;
T_use_E$(fmt__ParsedFormat);
$attr($must_check)
$static fn_((fmt__parseFormat(S_const$u8 fmt_str))(E$fmt__ParsedFormat));

typedef variant_((fmt__ArgValue $bits(8))(
    (fmt__ArgValue_void, Void),
    (fmt__ArgValue_bool, bool),
    (fmt__ArgValue_u8, u8),
    (fmt__ArgValue_u16, u16),
    (fmt__ArgValue_u32, u32),
    (fmt__ArgValue_u64, u64),
    (fmt__ArgValue_usize, usize),
    (fmt__ArgValue_i8, i8),
    (fmt__ArgValue_i16, i16),
    (fmt__ArgValue_i32, i32),
    (fmt__ArgValue_i64, i64),
    (fmt__ArgValue_isize, isize),
    (fmt__ArgValue_f32, f32),
    (fmt__ArgValue_f64, f64),
    (fmt__ArgValue_ptr, const void*),
    (fmt__ArgValue_error, Err),
    // (fmt__ArgValue_ascii_ch, u8),
    // (fmt__ArgValue_utf8_cp, u32),
    (fmt__ArgValue_sli_z_u8, const u8*),
    (fmt__ArgValue_sli_u8, S_const$u8)
)) fmt__ArgValue;
T_use$((fmt__ArgValue)(O, E));
typedef FieldType$(fmt__ArgValue, tag) fmt__ArgValue_Tag;
T_use$((fmt__ArgValue_Tag)(O, E));
$attr($must_check)
$static fn_((fmt__specToArgTag(fmt_Type type, fmt_Size size))(E$fmt__ArgValue_Tag));

typedef variant_((fmt__ArgType $bits(8))(
    (fmt__ArgType_value, fmt__ArgValue),
    (fmt__ArgType_optional, O$fmt__ArgValue),
    (fmt__ArgType_error_result, E$fmt__ArgValue)
)) fmt__ArgType;
T_use$((fmt__ArgType)(O, E));
$static fn_((fmt__collectArg(va_list* ap, O$fmt_TypePrefix wrapper, fmt__ArgValue_Tag tag))(fmt__ArgType));
$static fn_((fmt__collectArgValue(va_list* ap, fmt__ArgValue_Tag tag))(fmt__ArgValue));
$static fn_((fmt__collectArgOptional(va_list* ap, fmt__ArgValue_Tag tag))(O$fmt__ArgValue));
$static fn_((fmt__collectArgErrorResult(va_list* ap, fmt__ArgValue_Tag tag))(E$fmt__ArgValue));

$attr($must_check)
$static fn_((fmt__formatArg(io_Writer writer, fmt__ArgType arg, fmt_Spec spec))(E$void));
$attr($must_check)
$static fn_((fmt__formatArgValue(io_Writer writer, fmt__ArgValue value, fmt_Spec spec))(E$void));
$attr($must_check)
$static fn_((fmt__formatArgOptional(io_Writer writer, O$fmt__ArgValue optional, fmt_Spec spec))(E$void));
$attr($must_check)
$static fn_((fmt__formatArgErrorResult(io_Writer writer, E$fmt__ArgValue error_result, fmt_Spec spec))(E$void));
// Store each format spec occurrence with its position and argument binding
typedef struct fmt__FormatOccurrence {
    var_(spec, fmt_Spec);
    var_(arg_index, u8); // Which argument this uses
    var_(literal_start, usize); // Offset of literal text before this spec
    var_(literal_len, usize); // Length of literal text before this spec
} fmt__FormatOccurrence;
typedef struct fmt__ParsedFormatSpec {
    var_(occurrences, A$$(fmt_max_args, fmt__FormatOccurrence));
    var_(occurrence_count, usize);
    var_(max_arg_index, u8); // Highest argument index seen
    var_(trailing_literal_start, usize);
    var_(trailing_literal_len, usize);
} fmt__ParsedFormatSpec;
T_use$((fmt__ParsedFormatSpec)(E));
typedef struct fmt__ArgInfo {
    var_(index, u8);
    var_(wrapper, O$fmt_TypePrefix);
    var_(tag, fmt__ArgValue_Tag);
} fmt__ArgInfo;
T_use$((fmt__ArgInfo)(P, S, O));

// Single-pass parsing - NO REPARSING LATER
$attr($must_check)
$static fn_((fmt__parseFormatSpecOnce(S_const$u8 fmt))(E$fmt__ParsedFormatSpec));
// Write literal text handling escaped braces
$attr($must_check)
$static fn_((fmt__writeLiteralChunk(io_Writer writer, S_const$u8 fmt, usize start, usize len))(E$void));
$static fn_((fmt__ArgValue_Tag_isInt(fmt__ArgValue_Tag tag))(bool));
$static fn_((fmt__ArgValue_Tag_getIntSize(fmt__ArgValue_Tag tag))(u8));
// Helper: Get most general compatible type
$static fn_((fmt__ArgValue_Tag_getMostGeneralType(fmt__ArgValue_Tag lhs, fmt__ArgValue_Tag rhs))(fmt__ArgValue_Tag));

/*========== External Definitions ===========================================*/

fn_((fmt_Align_parse(u8 ch))(O$fmt_Align) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Align_values))(i, value) {
        if (ch == *value) { $break_(some(as$(fmt_Align)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_TypePrefix_parse(u8 ch))(O$fmt_TypePrefix) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_TypePrefix_values))(i, value) {
        if (ch == *value) { $break_(some(as$(fmt_TypePrefix)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_Type_parse(S_const$u8 str))(O$fmt_Type) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Type_values))(i, value) {
        if (mem_eqlBytes(str, *value)) { $break_(some(as$(fmt_Type)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_Size_parse(S_const$u8 str))(O$fmt_Size) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Size_values))(i, value) {
        if (mem_eqlBytes(str, *value)) { $break_(some(as$(fmt_Size)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

/*========== Formatting (Output) ==========*/

fn_((fmt_format(io_Writer writer, S_const$u8 fmt, ...))(E$void) $guard) {
    // printf("--- debug print: fmt_format ---\n");
    va_list va_args = null;
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(fmt_formatVaArgs(writer, fmt, va_args)));
} $unguarded_(fn);

/* TODO: Refactor this */
fn_((fmt_formatVaArgs(io_Writer writer, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    // Parse format string ONCE
    let parsed = try_(fmt__parseFormatSpecOnce(fmt));

    // STEP 1: Determine the most general type needed for each argument
    // by examining ALL occurrences that use that argument
    var_(arg_general_types, A$$(fmt_max_args, O$fmt__ArgValue_Tag)) = A_zero();
    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = A_at((parsed.occurrences)[i]);
        let arg_idx = occurrence->arg_index;
        let tag = catch_((fmt__specToArgTag(occurrence->spec.type, occurrence->spec.size))($ignore, continue));
        if_none((*A_at((arg_general_types)[arg_idx]))) {
            // First occurrence of this arg - use its type
            asg_lit((A_at((arg_general_types)[arg_idx]))(some(tag)));
        } else_some((existing_tag)) {
            // Argument reused - pick most general compatible type
            let general_tag = fmt__ArgValue_Tag_getMostGeneralType(existing_tag, tag);
            asg_lit((A_at((arg_general_types)[arg_idx]))(some(general_tag)));
        }
    }

    // STEP 2: Build collection info using general types
    var_(arg_collection_info, A$$(fmt_max_args, O$fmt__ArgInfo)) = A_zero();
    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = A_at((parsed.occurrences)[i]);
        let arg_idx = occurrence->arg_index;
        // Skip if already have collection info for this arg
        if_some((*A_at((arg_collection_info)[arg_idx]))(info)) {
            let_ignore = info;
        } else if_some((*A_at((arg_general_types)[arg_idx]))(general_tag)) {
            asg_lit((A_at((arg_collection_info)[arg_idx]))(some({
                .index = arg_idx,
                .wrapper = occurrence->spec.type_prefix, // Use first occurrence's wrapper
                .tag = general_tag // Use general type
            })));
        }
    }

    // STEP 3: Collect all arguments from va_list in sequential order
    var_(collected_args, A$$(fmt_max_args, O$fmt__ArgType)) = A_zero();
    for (u8 i = 0; i <= parsed.max_arg_index && i < fmt_max_args; ++i) {
        if_some((*A_at((arg_collection_info)[i]))(info)) {
            let arg = fmt__collectArg(&va_args, info.wrapper, info.tag);
            asg_lit((A_at((collected_args)[i]))(some(arg)));
        } else_none {
            // Unused arg index - consume dummy
            let dummy = va_arg(va_args, Void);
            let_ignore = dummy;
        }
    }

    // STEP 4: Format using per-occurrence specs
    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = A_at((parsed.occurrences)[i]);
        try_(fmt__writeLiteralChunk(writer, fmt, occurrence->literal_start, occurrence->literal_len));
        let arg_idx = occurrence->arg_index;
        if_some((*A_at((collected_args)[arg_idx]))(arg)) {
            // Each occurrence has its own spec - no deduplication
            try_(fmt__formatArg(writer, arg, occurrence->spec));
        }
    }

    try_(fmt__writeLiteralChunk(writer, fmt, parsed.trailing_literal_start, parsed.trailing_literal_len));
    return_ok({});
} $unscoped_(fn);

/* --- Direct Type Formatting APIs --- */

fn_((fmt_formatBool(io_Writer writer, bool val, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatBool ---\n");
    $static let_(contents, A$$(2, S_const$u8)) = A_init({ [0] = u8_l("false"), [1] = u8_l("true") });
    return fmt__writePadded(writer, *A_at((contents)[val]), spec);
};
fn_((fmt_format$bool(io_Writer writer, bool val, fmt_Spec spec))(E$void)) {
    return fmt_formatBool(writer, val, spec);
};

/* TODO: Refactor this */
fn_((fmt_formatUInt(io_Writer writer, u64 val, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatUInt ---\n");
    // printf("formatUInt: val=%llu, spec.type=%d\n", val, spec.type);
    var_(buf, A$$(128, u8)) = A_zero();
    var_(pos, usize) = A_len(buf) - 1;

    // Determine base
    let base_w_digits = expr_(struct { u32 base; S_const$u8 digits; } $scope)({
        // printf("  switch on spec.type=%d\n", spec.type);
        switch (spec.type) {
        case fmt_Type_hex_lower:
            $break_({ .base = 16, .digits = u8_l("0123456789abcdef") });
        case fmt_Type_hex_upper:
            $break_({ .base = 16, .digits = u8_l("0123456789ABCDEF") });
        case fmt_Type_octal:
            $break_({ .base = 8, .digits = u8_l("01234567") });
        case fmt_Type_binary:
            $break_({ .base = 2, .digits = u8_l("01") });
        case fmt_Type_unsigned:
            $break_({ .base = 10, .digits = u8_l("0123456789") });
        default:
            claim_unreachable;
        }
    }) $unscoped_(expr);
    let base = base_w_digits.base;
    let digits = base_w_digits.digits;
    // printf("  base=%u\n", base);
    // Apply size mask
    switch (spec.size) {
    case fmt_Size_8:
        val &= 0xFF;
        break;
    case fmt_Size_16:
        val &= 0xFFFF;
        break;
    case fmt_Size_32:
        val &= 0xFFFFFFFF;
        break;
    case fmt_Size_64:
        $fallthrough;
    case fmt_Size_128:
        // Fallback to 64-bit until 128-bit is fully supported
        $fallthrough;
    case fmt_Size_ptr:
        // No masking needed for 64-bit and pointer size
        break;
    default:
        claim_unreachable;
    }
    // Convert to string (reverse order)
    if (prim_isZero(val)) {
        *A_at((buf)[pos--]) = u8_c('0');
    } else {
        while (val > 0) { /* NOLINT */
            *A_at((buf)[pos--]) = *S_at((digits)[val % base]);
            val /= base;
        }
    }
    // Add alternate form prefix if requested
    if (spec.alt_form) {
        switch (spec.type) {
        case fmt_Type_hex_lower:
        case fmt_Type_hex_upper:
            // Always use lowercase 0x for hex
            *A_at((buf)[pos--]) = u8_c('x');
            *A_at((buf)[pos--]) = u8_c('0');
            break;
        case fmt_Type_binary:
            *A_at((buf)[pos--]) = u8_c('b');
            *A_at((buf)[pos--]) = u8_c('0');
            break;
        case fmt_Type_octal:
            *A_at((buf)[pos--]) = u8_c('0');
            break;
        default:
            break;
        }
    }

    // pos now points one position BEFORE the first written character, so add 1
    return fmt__writePadded(writer, A_suffix$((S$u8)(buf)(pos + 1)).as_const, spec);
};
fn_((fmt_format$usize(io_Writer writer, usize val, fmt_Spec spec))(E$void)) {
    return fmt_formatUInt(writer, as$(u64)(val), spec);
};
fn_((fmt_format$u64(io_Writer writer, u64 val, fmt_Spec spec))(E$void)) {
    return fmt_formatUInt(writer, val, spec);
};
fn_((fmt_format$u32(io_Writer writer, u32 val, fmt_Spec spec))(E$void)) {
    return fmt_formatUInt(writer, as$(u64)(val), spec);
};
fn_((fmt_format$u16(io_Writer writer, u16 val, fmt_Spec spec))(E$void)) {
    return fmt_formatUInt(writer, as$(u64)(val), spec);
};
fn_((fmt_format$u8(io_Writer writer, u8 val, fmt_Spec spec))(E$void)) {
    return fmt_formatUInt(writer, as$(u64)(val), spec);
};

/* TODO: Refactor this */
fn_((fmt_formatIInt(io_Writer writer, i64 val, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatIInt ---\n");
    var_(buf, A$$(128, u8)) = A_zero();
    var_(pos, usize) = A_len(buf) - 1;

    // Apply size extension
    switch (spec.size) {
    case fmt_Size_8:
        val = (i8)(val);
        break;
    case fmt_Size_16:
        val = intCast$((i16)(val));
        break;
    case fmt_Size_32:
        val = intCast$((i32)(val));
        break;
    case fmt_Size_64:
        $fallthrough;
    case fmt_Size_128:
        // Fallback to 64-bit until 128-bit is fully supported
        $fallthrough;
    case fmt_Size_ptr:
        // No extension needed for 64-bit and pointer size
        break;
    default:
        claim_unreachable;
    }
    let negative = prim_lt(val, 0);
    var abs_val = negative ? as$(u64)(-val) : as$(u64)(val);
    // Convert to string (reverse order)
    if (abs_val == 0) {
        *A_at((buf)[pos--]) = u8_c('0');
    } else {
        while (abs_val > 0) {
            *A_at((buf)[pos--]) = u8_c('0') + (abs_val % 10);
            abs_val /= 10;
        }
    }
    // Apply sign based on sign flag
    if (negative) {
        *A_at((buf)[pos--]) = u8_c('-');
    } else {
        switch (spec.sign) {
        case fmt_Sign_always:
            *A_at((buf)[pos--]) = u8_c('+');
            break;
        case fmt_Sign_space:
            *A_at((buf)[pos--]) = u8_c(' ');
            break;
        case fmt_Sign_auto:
            break;
        }
    }

    // pos now points one position BEFORE the first written character, so add 1
    return fmt__writePadded(writer, A_suffix$((S$u8)(buf)(pos + 1)).as_const, spec);
};
fn_((fmt_format$isize(io_Writer writer, isize val, fmt_Spec spec))(E$void)) {
    return fmt_formatIInt(writer, as$(i64)(val), spec);
};
fn_((fmt_format$i64(io_Writer writer, i64 val, fmt_Spec spec))(E$void)) {
    return fmt_formatIInt(writer, val, spec);
};
fn_((fmt_format$i32(io_Writer writer, i32 val, fmt_Spec spec))(E$void)) {
    return fmt_formatIInt(writer, as$(i64)(val), spec);
};
fn_((fmt_format$i16(io_Writer writer, i16 val, fmt_Spec spec))(E$void)) {
    return fmt_formatIInt(writer, as$(i64)(val), spec);
};
fn_((fmt_format$i8(io_Writer writer, i8 val, fmt_Spec spec))(E$void)) {
    return fmt_formatIInt(writer, as$(i64)(val), spec);
};

fn_((fmt_formatFlt(io_Writer writer, f64 val, fmt_Spec spec))(E$void)) {
    return fmt__formatFltImpl(writer, val, spec);
};
fn_((fmt_format$f64(io_Writer writer, f64 val, fmt_Spec spec))(E$void)) {
    return fmt_formatFlt(writer, val, spec);
};
fn_((fmt_format$f32(io_Writer writer, f32 val, fmt_Spec spec))(E$void)) {
    return fmt_formatFlt(writer, val, spec);
};

/* TODO: Refactor this */
fn_((fmt_formatPtr(io_Writer writer, P_const$raw ptr, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: formatPtr ---\n");
    var_(buf, A$$(32, u8)) = A_zero();
    var_(pos, usize) = 0;
    // Select digit set based on case
    let digits = spec.type == fmt_Type_pointer_upper
                   ? u8_l("0123456789ABCDEF")
                   : u8_l("0123456789abcdef");
    // Convert pointer to usize and format as hex
    var ptr_val = ptrToInt(ptr);
    let is_zero = prim_isZero(ptr_val);
    if (is_zero) {
        *A_at((buf)[pos++]) = u8_c('0');
    } else {
        // Find number of hex digits needed
        var temp = ptr_val;
        var_(digit_count, usize) = 0;
        while (temp > 0) {
            digit_count++;
            temp >>= 4;
        }
        // Write hex digits in reverse to temp buffer
        var_(temp_buf, A$$(16, u8)) = A_zero();
        for (usize i = digit_count; i > 0; i--) {
            *A_at((temp_buf)[i - 1]) = *S_at((digits)[ptr_val & 0xF]);
            ptr_val >>= 4;
        }
        // Copy to main buffer
        for (usize i = 0; i < digit_count; ++i) {
            *A_at((buf)[pos++]) = *A_at((temp_buf)[i]);
        }
    }
    // Add "0x" prefix if alternate form is requested
    if (spec.alt_form) {
        // Shift content to make room for prefix
        for (usize i = pos; i > 0; i--) {
            *A_at((buf)[i + 1]) = *A_at((buf)[i - 1]);
        }
        *A_at((buf)[0]) = u8_c('0');
        *A_at((buf)[1]) = u8_c('x');
        pos += 2;
    }
    return fmt__writePadded(writer, A_suffix$((S$u8)(buf)(pos)).as_const, spec);
};
fn_((fmt_format$P$raw(io_Writer writer, P_const$raw ptr, fmt_Spec spec))(E$void)) {
    return fmt_formatPtr(writer, ptr, spec);
};
/* TODO: Refactor this */
fn_((fmt_formatErr(io_Writer writer, Err err, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatErr ---\n");
    var_(buf, A$$(128, u8)) = A_zero();
    var_(pos, usize) = 0;
    // Format: "[Domain] Code"
    *A_at((buf)[pos++]) = u8_c('[');
    // Get domain string
    let domain = Err_domainToStr(err);
    for_(($s(domain))(ch) {
        *A_at((buf)[pos++]) = *ch;
    });
    *A_at((buf)[pos++]) = u8_c(']');
    *A_at((buf)[pos++]) = u8_c(' ');
    // Get code string
    let code = Err_codeToStr(err);
    for_(($s(code))(ch) {
        *A_at((buf)[pos++]) = *ch;
    });
    return fmt__writePadded(writer, A_suffix$((S$u8)(buf)(pos)).as_const, spec);
};
fn_((fmt_format$Err(io_Writer writer, Err err, fmt_Spec spec))(E$void)) {
    return fmt_formatErr(writer, err, spec);
};

fn_((fmt_formatASCII(io_Writer writer, u8 code, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatASCII ---\n");
    let_(buf, A$$(1, u8)) = A_init({ [0] = code });
    return fmt_formatStr(writer, A_ref$((S_const$u8)buf), spec);
};
fn_((fmt_formatUTF8(io_Writer writer, u32 codepoint, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: fmt_formatUTF8 ---\n");
    var_(buf, A$$(4, u8)) = A_zero();
    return fmt_formatStr(writer, try_(utf8_encodeWithin(codepoint, A_ref$((S$u8)buf))).as_const, spec);
} $unscoped_(fn);

fn_((fmt_formatStrZ(io_Writer writer, P_const$u8 str, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatStrZ ---\n");
    return fmt_formatStr(writer, mem_spanZ0$u8(str), spec);
};
fn_((fmt_format$P$u8(io_Writer writer, P_const$u8 str, fmt_Spec spec))(E$void)) {
    return fmt_formatStrZ(writer, str, spec);
};
fn_((fmt_formatStr(io_Writer writer, S_const$u8 str, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt_formatStr ---\n");
    return fmt__writePadded(writer, str, spec);
};
fn_((fmt_format$S$u8(io_Writer writer, S_const$u8 str, fmt_Spec spec))(E$void)) {
    return fmt_formatStr(writer, str, spec);
};

/*========== Parsing (Input) - Slice based ==========*/

fn_((fmt_parseBool(S_const$u8 str))(E$bool) $scope) {
    str = fmt__skipWhitespace(str);
    if (mem_eqlBytes(str, mem_asBytes(&u8_c('1')).as_const) || mem_eqlBytes(str, u8_l("true"))) { return_ok(true); }
    if (mem_eqlBytes(str, mem_asBytes(&u8_c('0')).as_const) || mem_eqlBytes(str, u8_l("false"))) { return_ok(false); }
    return_err(fmt_Err_InvalidBoolFormat());
} $unscoped_(fn);
fn_((fmt_parse$bool(S_const$u8 str))(E$bool)) {
    return fmt_parseBool(str);
};

/* TODO: Refactor this */
fn_((fmt_parseUInt(S_const$u8 str, u8 base))(E$u64) $scope) {
    str = fmt__skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    if (base < 2 || 36 < base) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    u64 result = 0;
    for (usize i = 0; i < str.len; ++i) {
        u8 ch = str.ptr[i];
        if (ascii_isWhitespace(ch)) {
            break;
        }
        u8 digit_val = 0;
        if (ascii_isDigit(ch)) {
            digit_val = ch - u8_c('0');
        } else if (u8_c('a') <= ch && ch <= u8_c('z')) {
            digit_val = u8_add(10, ch - u8_c('a'));
        } else if (u8_c('A') <= ch && ch <= u8_c('Z')) {
            digit_val = u8_add(10, ch - u8_c('A'));
        } else {
            if (i == 0) {
                return_err(fmt_Err_InvalidUIntFormat());
            }
            break;
        }
        if (digit_val >= base) {
            if (i == 0) {
                return_err(fmt_Err_InvalidUIntFormat());
            }
            break;
        }
        // Check for overflow using checked arithmetic
        let mul_result = u64_mulChkd(result, base);
        if (isNone(mul_result)) {
            return_err(fmt_Err_InvalidUIntFormat());
        }
        let add_result = u64_addChkd(unwrap_(mul_result), digit_val);
        if (isNone(add_result)) {
            return_err(fmt_Err_InvalidUIntFormat());
        }
        result = unwrap_(add_result);
    }
    return_ok(result);
} $unscoped_(fn);
fn_((fmt_parse$usize(S_const$u8 str, u8 base))(E$usize) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > usize_limit_max) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    return_ok(as$(usize)(try_(fmt_parseUInt(str, base))));
} $unscoped_(fn);
fn_((fmt_parse$u64(S_const$u8 str, u8 base))(E$u64)) {
    return fmt_parseUInt(str, base);
};
fn_((fmt_parse$u32(S_const$u8 str, u8 base))(E$u32) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u32_limit_max) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    return_ok(as$(u32)(result));
} $unscoped_(fn);
fn_((fmt_parse$u16(S_const$u8 str, u8 base))(E$u16) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u16_limit_max) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    return_ok(as$(u16)(result));
} $unscoped_(fn);
fn_((fmt_parse$u8(S_const$u8 str, u8 base))(E$u8) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u8_limit_max) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    return_ok(as$(u8)(result));
} $unscoped_(fn);

/* TODO: Refactor this */
fn_((fmt_parseIInt(S_const$u8 str, u8 base))(E$i64) $scope) {
    str = fmt__skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidIIntFormat());
    }
    bool negative = false;
    if (str.ptr[0] == u8_c('-')) {
        negative = true;
        str.ptr++;
        str.len--;
    } else if (str.ptr[0] == u8_c('+')) {
        str.ptr++;
        str.len--;
    }
    let unsigned_result = try_(fmt_parseUInt(str, base));
    return_ok(expr_(i64 $scope)(if (negative) {
        let max_neg = as$(u64)(i64_limit_max) + 1;
        if (max_neg < unsigned_result) {
            return_err(fmt_Err_InvalidIIntFormat());
        }
        $break_(-as$(i64)(unsigned_result));
    } else {
        if (as$(u64)(i64_limit_max) < unsigned_result) {
            return_err(fmt_Err_InvalidIIntFormat());
        }
        $break_(as$(i64)(unsigned_result));
    }) $unscoped_(expr));
} $unscoped_(fn);
fn_((fmt_parse$isize(S_const$u8 str, u8 base))(E$isize) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < isize_limit_min || isize_limit_max < result) {
        return_err(fmt_Err_InvalidIIntFormat());
    }
    return_ok(as$(isize)(result));
} $unscoped_(fn);
fn_((fmt_parse$i64(S_const$u8 str, u8 base))(E$i64)) {
    return fmt_parseIInt(str, base);
};
fn_((fmt_parse$i32(S_const$u8 str, u8 base))(E$i32) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i32_limit_min || i32_limit_max < result) {
        return_err(fmt_Err_InvalidIIntFormat());
    }
    return_ok(as$(i32)(result));
} $unscoped_(fn);
fn_((fmt_parse$i16(S_const$u8 str, u8 base))(E$i16) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i16_limit_min || i16_limit_max < result) {
        return_err(fmt_Err_InvalidIIntFormat());
    }
    return_ok(as$(i16)(result));
} $unscoped_(fn);
fn_((fmt_parse$i8(S_const$u8 str, u8 base))(E$i8) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i8_limit_min || i8_limit_max < result) {
        return_err(fmt_Err_InvalidIIntFormat());
    }
    return_ok(as$(i8)(result));
} $unscoped_(fn);

/* TODO: Refactor this */
fn_((fmt_parseFlt(S_const$u8 str))(E$f64) $scope) {
    str = fmt__skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidFltFormat());
    }
    usize pos = 0;
    f64 sign = 1.0;
    f64 result = 0.0;
    bool has_int_part = false;
    // Check for sign
    if (pos < str.len && str.ptr[pos] == u8_c('+')) {
        pos++;
    } else if (pos < str.len && str.ptr[pos] == u8_c('-')) {
        sign = -1.0;
        pos++;
    }
    // Parse integer part
    while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
        result = (result * 10.0) + (str.ptr[pos] - u8_c('0'));
        has_int_part = true;
        pos++;
    }
    // Parse fractional part
    if (pos < str.len && str.ptr[pos] == u8_c('.')) {
        pos++;
        f64 power = 0.1;
        bool has_frac_part = false;
        while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
            result += (str.ptr[pos] - u8_c('0')) * power;
            power *= 0.1;
            has_frac_part = true;
            pos++;
        }
        if (!has_int_part && !has_frac_part) {
            return_err(fmt_Err_InvalidFltFormat());
        }
    } else {
        if (!has_int_part) {
            return_err(fmt_Err_InvalidFltFormat());
        }
    }
    // Parse exponent part
    if (pos < str.len && (str.ptr[pos] == u8_c('e') || str.ptr[pos] == u8_c('E'))) {
        pos++;
        f64 exp_sign = 1.0;
        i32 exp_val = 0;
        bool has_exp = false;

        if (pos < str.len && str.ptr[pos] == u8_c('+')) {
            pos++;
        } else if (pos < str.len && str.ptr[pos] == u8_c('-')) {
            exp_sign = -1.0;
            pos++;
        }
        while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
            exp_val = (exp_val * 10) + (str.ptr[pos] - u8_c('0'));
            has_exp = true;
            pos++;
        }
        if (!has_exp) {
            return_err(fmt_Err_InvalidFltFormat());
        }
        result *= __builtin_pow(10.0, exp_val * exp_sign);
    }
    // Check that we consumed all input
    if (pos != str.len) {
        return_err(fmt_Err_InvalidFltFormat());
    }
    return_ok(result * sign);
} $unscoped_(fn);
fn_((fmt_parse$f64(S_const$u8 str))(E$f64)) {
    return fmt_parseFlt(str);
};
fn_((fmt_parse$f32(S_const$u8 str))(E$f32) $scope) {
    let result = try_(fmt_parseFlt(str));
    if (result < f32_limit_min || f32_limit_max < result) {
        return_err(fmt_Err_InvalidFltFormat());
    }
    return_ok(as$(f32)(result));
} $unscoped_(fn);

/*========== Internal Definitions ===========================================*/

fn_((fmt__writePadded(io_Writer writer, S_const$u8 content, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: fmt__writePadded ---\n");
    let width = orelse_((spec.width)(return io_Writer_writeBytes(writer, content)));
    if (width <= content.len) { return io_Writer_writeBytes(writer, content); }
    let padding = width - content.len;
    // Use explicit fill or default to space
    let fill = orelse_((spec.fill)(u8_c(' ')));
    // Default to left alignment
    let align = orelse_((spec.align)(fmt_Align_left));
    switch (align) {
    case_((fmt_Align_left)) {
        try_(io_Writer_writeBytes(writer, content));
        try_(io_Writer_writeByteN(writer, fill, padding));
    } $end(case);
    case_((fmt_Align_center)) {
        let left_pad = padding / 2;
        let right_pad = padding - left_pad;
        try_(io_Writer_writeByteN(writer, fill, left_pad));
        try_(io_Writer_writeBytes(writer, content));
        try_(io_Writer_writeByteN(writer, fill, right_pad));
    } $end(case);
    case_((fmt_Align_right)){
        try_(io_Writer_writeByteN(writer, fill, padding));
        try_(io_Writer_writeBytes(writer, content));
    } $end(case);
    }
    return_ok({});
} $unscoped_(fn);

fn_((fmt__digitToInt(u8 c))(u8)) {
    claim_assert(ascii_isDigit(c));
    return c - u8_c('0');
};

fn_((fmt__skipWhitespace(S_const$u8 str))(S_const$u8)) {
    while (0 < str.len && ascii_isWhitespace(*S_at((str)[0]))) { str = S_suffix((str)(1)); }
    return str;
};

fn_((fmt__parseU8(S_const$u8 str))(E$u8) $scope) {
    // printf("--- debug print: fmt__parseU8 ---\n");
    if (str.len == 0 || !ascii_isDigit(*S_at((str)[0]))) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    var_(result, u8) = 0;
    for_(($s(str))(ch) {
        if (!ascii_isDigit(*ch)) { break; }
        let digit = fmt__digitToInt(*ch);
        let mul = orelse_((u8_mulChkd(result, 10))(
            return_err(fmt_Err_InvalidUIntFormat())
        ));
        let add = orelse_((u8_addChkd(mul, digit))(
            return_err(fmt_Err_InvalidUIntFormat())
        ));
        result = add;
    });
    return_ok(result);
} $unscoped_(fn);

/* TODO: Refactor this */
fn_((fmt__consumeU8(S_const$u8 str))(E$fmt__ConsumedU8) $scope) {
    // printf("--- debug print: fmt__consumeU8 ---\n");
    usize digit_count = 0;
    while (digit_count < str.len && ascii_isDigit(*S_at((str)[digit_count]))) { digit_count++; }
    if (digit_count == 0) {
        return_err(fmt_Err_InvalidUIntFormat());
    }
    let digits = S_prefix((str)(digit_count));
    let value = try_(fmt__parseU8(digits));
    return_ok({ .value = value, .remain = S_suffix((str)(digit_count)) });
} $unscoped_(fn);

/* TODO: Refactor this */
fn_((fmt__parseFormat(S_const$u8 fmt_str))(E$fmt__ParsedFormat) $scope) {
    // printf("--- debug print: fmt__parseFormat ---\n");
    // Skip opening '{'
    fmt_str = S_suffix((fmt_str)(1));
    if (fmt_str.len == 0) {
        return_err(fmt_Err_MissingClosingBrace());
    }

    // Parse optional index (must be followed by ':')
    let index = expr_(O$u8 $scope)({
        let ch = *S_at((fmt_str)[0]);
        if (ascii_isDigit(ch)) {
            usize colon_pos = 0;
            while (colon_pos < fmt_str.len) {
                let colon_pos_ch = *S_at((fmt_str)[colon_pos]);
                if (colon_pos_ch == u8_c(':') || colon_pos_ch == u8_c('}')) { break; }
                if (!ascii_isDigit(colon_pos_ch)) { break; /* Not an index, treat as width */ }
                colon_pos++;
            }
            if (colon_pos < fmt_str.len) {
                let colon_pos_ch = *S_at((fmt_str)[colon_pos]);
                if (colon_pos_ch == u8_c(':')) {
                    let index_slice = S_prefix((fmt_str)(colon_pos));
                    let index = try_(fmt__parseU8(index_slice));
                    if (index >= fmt_max_args) {
                        return_err(fmt_Err_IdxOutOfBounds());
                    }
                    fmt_str = S_suffix((fmt_str)(colon_pos + 1)); // Skip index and ':'
                    $break_(some(index));
                }
            }
        }
        $break_(none());
    }) $unscoped_(expr);

    // Skip optional ':' at format start
    if (0 < fmt_str.len && *S_at((fmt_str)[0]) == u8_c(':')) {
        fmt_str = suffix$S(fmt_str, 1);
    }

    // Parse fill and alignment (more efficient than function call)
    let fill_w_align = expr_(struct { O$u8 fill; O$fmt_Align align; } $scope)({
        if (fmt_str.len >= 2) {
            let maybe_fill = *S_at((fmt_str)[0]);
            let maybe_align = *S_at((fmt_str)[1]);

            // Check if second char is alignment
            if_some((fmt_Align_parse(maybe_align))(align)) {
                fmt_str = S_suffix((fmt_str)(2));
                $break_({ .fill = some(maybe_fill), .align = some(align) });
            } else_none {
                // Check if first char is alignment
                if_some((fmt_Align_parse(maybe_fill))(align)) {
                    fmt_str = S_suffix((fmt_str)(1));
                    $break_({ .fill = none(), .align = some(align) });
                }
            }
        } else if (fmt_str.len >= 1) {
            let maybe_align = *S_at((fmt_str)[0]);
            if_some((fmt_Align_parse(maybe_align))(align)) {
                fmt_str = S_suffix((fmt_str)(1));
                $break_({ .fill = none(), .align = some(align) });
            }
        }
        $break_({ .fill = none(), .align = none() });
    }) $unscoped_(expr);
    let fill = fill_w_align.fill;
    let align = fill_w_align.align;

    // Parse sign flag (+, -, or space)
    let sign = expr_(fmt_Sign $scope)(if (0 < fmt_str.len) {
        let ch = *S_at((fmt_str)[0]);
        if (ch == u8_c('+')) {
            fmt_str = S_suffix((fmt_str)(1));
            $break_(fmt_Sign_always);
        } else if (ch == u8_c(' ')) {
            fmt_str = S_suffix((fmt_str)(1));
            $break_(fmt_Sign_space);
        }
    }) expr_(else)($break_(fmt_Sign_auto)) $unscoped_(expr);

    // Parse alternate form flag (#)
    let alt_form = expr_(bool $scope)(if (0 < fmt_str.len && *S_at((fmt_str)[0]) == u8_c('#')) {
        fmt_str = S_suffix((fmt_str)(1));
        $break_(true);
    }) expr_(else)($break_(false)) $unscoped_(expr);

    // Parse width
    let width = expr_(O$u8 $scope)(if (0 < fmt_str.len && ascii_isDigit(*S_at((fmt_str)[0]))) {
        let consumed = try_(fmt__consumeU8(fmt_str));
        fmt_str = consumed.remain;
        $break_(some(consumed.value));
    }) expr_(else)($break_(none())) $unscoped_(expr);

    // Parse precision
    let precision = expr_(O$u8 $scope)(if (0 < fmt_str.len && *S_at((fmt_str)[0]) == u8_c('.')) {
        let consumed = try_(fmt__consumeU8(S_suffix((fmt_str)(1))));
        fmt_str = consumed.remain;
        $break_(some(consumed.value));
    }) expr_(else)($break_(none())) $unscoped_(expr);

    // Parse type prefix (? or !)
    let type_prefix = expr_(O$fmt_TypePrefix $scope)(if (0 < fmt_str.len) {
        if_some((fmt_TypePrefix_parse(*S_at((fmt_str)[0])))(prefix)) { /* NOLINT */
            fmt_str = S_suffix((fmt_str)(1));
            $break_(some(prefix));
        };
    }) expr_(else)($break_(none())) $unscoped_(expr);

    /* clang-format off */
    let type_w_size = expr_(struct { fmt_Type type; fmt_Size size; } $scope)({
        // Parse type character (required if we reach here)
        let type_ch = blk({
            if (fmt_str.len == 0) {
                return_err(fmt_Err_InvalidSpecFormat());
            }
            let ch = *S_at((fmt_str)[0]);
            fmt_str = S_suffix((fmt_str)(1));
            blk_return_(ch);
        });
        // Parse size modifier - use pre-built API for multi-char patterns
        let size = expr_(fmt_Size $scope)({
            if (2 <= fmt_str.len) if_some((fmt_Size_parse(S_prefix((fmt_str)(2))))(size)) { /* NOLINT */
                fmt_str = S_suffix((fmt_str)(2));
                $break_(size);
            }
            if (1 <= fmt_str.len) if_some((fmt_Size_parse(S_prefix((fmt_str)(1))))(size)) { /* NOLINT */
                fmt_str = S_suffix((fmt_str)(1));
                $break_(size);
            }
            $break_(fmt_Size_default);
        }) $unscoped_(expr);
        // Map type character to enum (switch is faster than fmt_Type_parse)
        switch (type_ch) {
            case u8_c('x'): $break_({ .type = fmt_Type_hex_lower, .size = size });
            case u8_c('X'): $break_({ .type = fmt_Type_hex_upper, .size = size });
            case u8_c('o'): $break_({ .type = fmt_Type_octal, .size = size });
            case u8_c('b'): $break_({ .type = fmt_Type_binary, .size = size });
            case u8_c('B'): $break_({ .type = fmt_Type_boolean, .size = size });
            case u8_c('u'): $break_({ .type = fmt_Type_unsigned, .size = size });
            case u8_c('d'):
            case u8_c('i'): $break_({ .type = fmt_Type_signed, .size = size });
            case u8_c('f'): $break_({ .type = fmt_Type_float_lower, .size = size });
            case u8_c('F'): $break_({ .type = fmt_Type_float_upper, .size = size });
            case u8_c('p'): $break_({ .type = fmt_Type_pointer_lower, .size = size });
            case u8_c('P'): $break_({ .type = fmt_Type_pointer_upper, .size = size });
            case u8_c('e'): $break_({ .type = fmt_Type_error, .size = size });
            case u8_c('0'): $break_({ .type = fmt_Type_void, .size = size });
            case u8_c('c'): $break_({ .type = fmt_Type_ascii_code, .size = size });
            case u8_c('C'): $break_({ .type = fmt_Type_utf8_codepoint, .size = size });
            case u8_c('z'): $break_({ .type = fmt_Type_string_z0, .size = size });
            case u8_c('s'): $break_({ .type = fmt_Type_string_s, .size = size });
            default: return_err(fmt_Err_InvalidSpecFormat());
        }
    }) $unscoped_(expr);
    /* clang-format on */
    let type = type_w_size.type;
    let size = type_w_size.size;

    // printf("parseFormat: type=%d, size=%d\n", type_w_size.type, type_w_size.size);
    // Expect closing '}'
    if (0 < fmt_str.len && *S_at((fmt_str)[0]) != u8_c('}')) {
        return_err(fmt_Err_MissingClosingBrace());
    }
    return_ok({
        .spec = {
            .index = index,
            .fill = fill,
            .align = align,
            .sign = sign,
            .alt_form = alt_form,
            .width = width,
            .precision = precision,
            .type_prefix = type_prefix,
            .type = type,
            .size = size,
        },
        .remain = suffix$S(fmt_str, 1), // Skip '}'
    });
} $unscoped_(fn);

fn_((fmt__specToArgTag(fmt_Type type, fmt_Size size))(E$fmt__ArgValue_Tag) $scope) {
    // printf("--- debug print: fmt__specToArgTag ---\n");
    /* clang-format off */
    switch (type) {
    case fmt_Type_void:
        return_ok(fmt__ArgValue_void);
    case fmt_Type_boolean:
        return_ok(fmt__ArgValue_bool);
    case fmt_Type_unsigned:
    case fmt_Type_hex_lower:
    case fmt_Type_hex_upper:
    case fmt_Type_octal:
    case fmt_Type_binary:
        switch (size) {
        case fmt_Size_8:   return_ok(fmt__ArgValue_u8);
        case fmt_Size_16:  return_ok(fmt__ArgValue_u16);
        case fmt_Size_32:  return_ok(fmt__ArgValue_u32);
        case fmt_Size_64:  return_ok(fmt__ArgValue_u64);
        case fmt_Size_ptr: return_ok(fmt__ArgValue_usize);
        default: return_err(fmt_Err_InvalidSpecFormat());
        }
    case fmt_Type_signed:
        switch (size) {
        case fmt_Size_8:   return_ok(fmt__ArgValue_i8);
        case fmt_Size_16:  return_ok(fmt__ArgValue_i16);
        case fmt_Size_32:  return_ok(fmt__ArgValue_i32);
        case fmt_Size_64:  return_ok(fmt__ArgValue_i64);
        case fmt_Size_ptr: return_ok(fmt__ArgValue_isize);
        default: return_err(fmt_Err_InvalidSpecFormat());
        }
    case fmt_Type_float_lower:
    case fmt_Type_float_upper:
        switch (size) {
        case fmt_Size_32: return_ok(fmt__ArgValue_f32);
        case fmt_Size_64: return_ok(fmt__ArgValue_f64);
        default: return_err(fmt_Err_InvalidSpecFormat());
        }
    case fmt_Type_pointer_lower:
    case fmt_Type_pointer_upper:
        return_ok(fmt__ArgValue_ptr);
    case fmt_Type_error:
        return_ok(fmt__ArgValue_error);
    case fmt_Type_ascii_code:
        return_ok(fmt__ArgValue_u8);
    case fmt_Type_utf8_codepoint:
        return_ok(fmt__ArgValue_u32);
    case fmt_Type_string_z0:
        return_ok(fmt__ArgValue_sli_z_u8);
    case fmt_Type_string_s:
        return_ok(fmt__ArgValue_sli_u8);
    default:
        return_err(fmt_Err_InvalidSpecFormat());
    }
    /* clang-format on */
} $unscoped_(fn);

fn_((fmt__collectArg(va_list* ap, O$fmt_TypePrefix wrapper, fmt__ArgValue_Tag tag))(fmt__ArgType) $scope) {
    // printf("--- debug print: collectArg ---\n");
    if_none((wrapper)) {
        return_(union_of((fmt__ArgType_value)(fmt__collectArgValue(ap, tag))));
    } else_some((prefix)) {
        switch (prefix) {
        case fmt_TypePrefix_optional:
            return_(union_of((fmt__ArgType_optional)(fmt__collectArgOptional(ap, tag))));
        case fmt_TypePrefix_error_result:
            return_(union_of((fmt__ArgType_error_result)(fmt__collectArgErrorResult(ap, tag))));
        }
    }
    claim_unreachable;
} $unscoped_(fn);

fn_((fmt__collectArgValue(va_list* ap, fmt__ArgValue_Tag tag))(fmt__ArgValue) $scope) {
    // printf("--- debug print: fmt__collectArgValue ---\n");
    switch (tag) {
    case fmt__ArgValue_void:
        return_(union_of((fmt__ArgValue_void)(va_arg(*ap, Void))));
    case fmt__ArgValue_bool:
        return_(union_of((fmt__ArgValue_bool)(as$(bool)(va_arg(*ap, u32)))));
    case fmt__ArgValue_u8:
        return_(union_of((fmt__ArgValue_u8)(as$(u8)(va_arg(*ap, u32)))));
    case fmt__ArgValue_u16:
        return_(union_of((fmt__ArgValue_u16)(as$(u16)(va_arg(*ap, u32)))));
    case fmt__ArgValue_u32:
        return_(union_of((fmt__ArgValue_u32)(va_arg(*ap, u32))));
    case fmt__ArgValue_u64:
        return_(union_of((fmt__ArgValue_u64)(va_arg(*ap, u64))));
    case fmt__ArgValue_usize:
        return_(union_of((fmt__ArgValue_usize)(va_arg(*ap, usize))));
    case fmt__ArgValue_i8:
        return_(union_of((fmt__ArgValue_i8)(as$(i8)(va_arg(*ap, i32)))));
    case fmt__ArgValue_i16:
        return_(union_of((fmt__ArgValue_i16)(as$(i16)(va_arg(*ap, i32)))));
    case fmt__ArgValue_i32:
        return_(union_of((fmt__ArgValue_i32)(va_arg(*ap, i32))));
    case fmt__ArgValue_i64:
        return_(union_of((fmt__ArgValue_i64)(va_arg(*ap, i64))));
    case fmt__ArgValue_isize:
        return_(union_of((fmt__ArgValue_isize)(va_arg(*ap, isize))));
    case fmt__ArgValue_f32:
        return_(union_of((fmt__ArgValue_f32)(as$(f32)(va_arg(*ap, f64)))));
    case fmt__ArgValue_f64:
        return_(union_of((fmt__ArgValue_f64)(va_arg(*ap, f64))));
    case fmt__ArgValue_ptr:
        return_(union_of((fmt__ArgValue_ptr)(va_arg(*ap, const void*))));
    case fmt__ArgValue_error:
        return_(union_of((fmt__ArgValue_error)(va_arg(*ap, Err))));
    case fmt__ArgValue_sli_z_u8:
        return_(union_of((fmt__ArgValue_sli_z_u8)(va_arg(*ap, const u8*))));
    case fmt__ArgValue_sli_u8:
        return_(union_of((fmt__ArgValue_sli_u8)(va_arg(*ap, S_const$u8))));
    }
    claim_unreachable;
} $unscoped_(fn);

fn_((fmt__collectArgOptional(va_list* ap, fmt__ArgValue_Tag tag))(O$fmt__ArgValue) $scope) {
    // printf("--- debug print: fmt__collectArgOptional ---\n");
    switch (tag) {
    case_((fmt__ArgValue_void)) {
        let arg = va_arg(*ap, O$$(Void));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_void)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_bool)) {
        let arg = va_arg(*ap, O$$(bool));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_bool)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_u8)) {
        let arg = va_arg(*ap, O$$(u8));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_u8)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_u16)) {
        let arg = va_arg(*ap, O$$(u16));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_u16)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_u32)) {
        let arg = va_arg(*ap, O$$(u32));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_u32)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_u64)) {
        let arg = va_arg(*ap, O$$(u64));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_u64)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_usize)) {
        let arg = va_arg(*ap, O$$(usize));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_usize)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_i8)) {
        let arg = va_arg(*ap, O$$(i8));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_i8)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_i16)) {
        let arg = va_arg(*ap, O$$(i16));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_i16)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_i32)) {
        let arg = va_arg(*ap, O$$(i32));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_i32)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_i64)) {
        let arg = va_arg(*ap, O$$(i64));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_i64)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_isize)) {
        let arg = va_arg(*ap, O$$(isize));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_isize)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_f32)) {
        let arg = va_arg(*ap, O$$(f32));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_f32)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_f64)) {
        let arg = va_arg(*ap, O$$(f64));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_f64)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_ptr)) {
        let arg = va_arg(*ap, O$$(const void*));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_ptr)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_error)) {
        let arg = va_arg(*ap, O$$(Err));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_error)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_sli_z_u8)) {
        let arg = va_arg(*ap, O$$(const u8*));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_sli_z_u8)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    case_((fmt__ArgValue_sli_u8)) {
        let arg = va_arg(*ap, O$$(S_const$u8));
        if_some((arg)(value)) {
            return_some(union_of((fmt__ArgValue_sli_u8)(value)));
        } else_none {
            return_none();
        }
    } $end(case);
    }
    claim_unreachable;
} $unscoped_(fn);

fn_((fmt__collectArgErrorResult(va_list* ap, fmt__ArgValue_Tag tag))(E$fmt__ArgValue) $scope) {
    // printf("--- debug print: fmt__collectArgErrorResult ---\n");
    switch (tag) {
    case_((fmt__ArgValue_void)) {
        let arg = va_arg(*ap, E$$(Void));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_void)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_bool)) {
        let arg = va_arg(*ap, E$$(bool));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_bool)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_u8)) {
        let arg = va_arg(*ap, E$$(u8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_u8)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_u16)) {
        let arg = va_arg(*ap, E$$(u16));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_u16)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_u32)) {
        let arg = va_arg(*ap, E$$(u32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_u32)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_u64)) {
        let arg = va_arg(*ap, E$$(u64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_u64)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_usize)) {
        let arg = va_arg(*ap, E$$(usize));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_usize)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_i8)) {
        let arg = va_arg(*ap, E$$(i8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_i8)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_i16)) {
        let arg = va_arg(*ap, E$$(i16));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_i16)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_i32)) {
        let arg = va_arg(*ap, E$$(i32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_i32)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_i64)) {
        let arg = va_arg(*ap, E$$(i64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_i64)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_isize)) {
        let arg = va_arg(*ap, E$$(isize));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_isize)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_f32)) {
        let arg = va_arg(*ap, E$$(f32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_f32)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_f64)) {
        let arg = va_arg(*ap, E$$(f64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_f64)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_ptr)) {
        let arg = va_arg(*ap, E$$(const void*));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_ptr)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_error)) {
        let arg = va_arg(*ap, E$$(Err));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_error)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_sli_z_u8)) {
        let arg = va_arg(*ap, E$$(const u8*));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_sli_z_u8)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    case_((fmt__ArgValue_sli_u8)) {
        let arg = va_arg(*ap, E$$(S_const$u8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt__ArgValue_sli_u8)(value)));
        } else_err(e) {
            return_(err(e));
        }
    } $end(case);
    };
    claim_unreachable;
} $unscoped_(fn);

fn_((fmt__formatArg(io_Writer writer, fmt__ArgType arg, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: fmt__formatArg ---\n");
    match_(arg) {
    pattern_((fmt__ArgType_value)(value)) {
        return fmt__formatArgValue(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgType_optional)(optional)) {
        return fmt__formatArgOptional(writer, *optional, spec);
    } $end(pattern);
    pattern_((fmt__ArgType_error_result)(error_result)) {
        return fmt__formatArgErrorResult(writer, *error_result, spec);
    } $end(pattern);
    default_() return_err(fmt_Err_InvalidSpecFormat()) $end(default);
    } $end(match);
} $unscoped_(fn);

fn_((fmt__formatArgValue(io_Writer writer, fmt__ArgValue value, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: fmt__formatArgValue ---\n");
    match_(value) {
    pattern_((fmt__ArgValue_void)(value)) {
        return_ok(*value);
    } $end(pattern);
    pattern_((fmt__ArgValue_bool)(value)) {
        return fmt_formatBool(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_u8)(value)) {
        // printf("formatArgValue u8: value=%d, spec.type=%d\n", *value, spec.type);
        if (spec.type == fmt_Type_signed) {
            return fmt_formatIInt(writer, as$(i8)(*value), spec);
        }
        if (spec.type == fmt_Type_ascii_code) {
            return fmt_formatASCII(writer, *value, spec);
        }
        return fmt_formatUInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_u16)(value)) {
        if (spec.type == fmt_Type_signed) {
            return fmt_formatIInt(writer, as$(i16)(*value), spec);
        }
        return fmt_formatUInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_u32)(value)) {
        if (spec.type == fmt_Type_signed) {
            return fmt_formatIInt(writer, as$(i32)(*value), spec);
        }
        if (spec.type == fmt_Type_utf8_codepoint) {
            return fmt_formatUTF8(writer, *value, spec);
        }
        return fmt_formatUInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_u64)(value)) {
        if (spec.type == fmt_Type_signed) {
            return fmt_formatIInt(writer, as$(i64)(*value), spec);
        }
        return fmt_formatUInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_usize)(value)) {
        if (spec.type == fmt_Type_signed) {
            return fmt_formatIInt(writer, as$(isize)(*value), spec);
        }
        return fmt_formatUInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_i8)(value)) {
        if (spec.type == fmt_Type_unsigned
            || spec.type == fmt_Type_hex_lower
            || spec.type == fmt_Type_hex_upper
            || spec.type == fmt_Type_octal
            || spec.type == fmt_Type_binary) {
            return fmt_formatUInt(writer, as$(u8)(*value), spec);
        }
        return fmt_formatIInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_i16)(value)) {
        if (spec.type == fmt_Type_unsigned
            || spec.type == fmt_Type_hex_lower
            || spec.type == fmt_Type_hex_upper
            || spec.type == fmt_Type_octal
            || spec.type == fmt_Type_binary) {
            return fmt_formatUInt(writer, as$(u16)(*value), spec);
        }
        return fmt_formatIInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_i32)(value)) {
        if (spec.type == fmt_Type_unsigned
            || spec.type == fmt_Type_hex_lower
            || spec.type == fmt_Type_hex_upper
            || spec.type == fmt_Type_octal
            || spec.type == fmt_Type_binary) {
            return fmt_formatUInt(writer, as$(u32)(*value), spec);
        }
        return fmt_formatIInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_i64)(value)) {
        if (spec.type == fmt_Type_unsigned
            || spec.type == fmt_Type_hex_lower
            || spec.type == fmt_Type_hex_upper
            || spec.type == fmt_Type_octal
            || spec.type == fmt_Type_binary) {
            return fmt_formatUInt(writer, as$(u64)(*value), spec);
        }
        return fmt_formatIInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_isize)(value)) {
        if (spec.type == fmt_Type_unsigned
            || spec.type == fmt_Type_hex_lower
            || spec.type == fmt_Type_hex_upper
            || spec.type == fmt_Type_octal
            || spec.type == fmt_Type_binary) {
            return fmt_formatUInt(writer, as$(usize)(*value), spec);
        }
        return fmt_formatIInt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_f32)(value)) {
        return fmt_formatFlt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_f64)(value)) {
        return fmt_formatFlt(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_ptr)(value)) {
        return fmt_formatPtr(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_error)(value)) {
        return fmt_formatErr(writer, *value, spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_sli_z_u8)(value)) {
        let ptr = *value;
        let len = mem_lenZ0$u8(ptr);
        return fmt_formatStr(writer, lit$((S_const$u8){ .ptr = ptr, .len = len }), spec);
    } $end(pattern);
    pattern_((fmt__ArgValue_sli_u8)(value)) {
        return fmt_formatStr(writer, *value, spec);
    } $end(pattern);
    default_() return_err(fmt_Err_InvalidSpecFormat()) $end(default);
    } $end(match);
} $unscoped_(fn);

fn_((fmt__formatArgOptional(io_Writer writer, O$fmt__ArgValue optional, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt__formatArgOptional ---\n");
    // Handle void optional - just print tag
    if (spec.type == fmt_Type_void) {
        return fmt__writePadded(writer, optional.is_some ? u8_l("some") : u8_l("none"), spec);
    }
    // Handle none case
    if_none((optional)) {
        return fmt__writePadded(writer, u8_l("none"), spec);
    } else_some((value)) { // Format the inner value
        return fmt__formatArgValue(writer, value, spec);
    }
    claim_unreachable;
};

fn_((fmt__formatArgErrorResult(io_Writer writer, E$fmt__ArgValue error_result, fmt_Spec spec))(E$void)) {
    // printf("--- debug print: fmt__formatArgErrorResult ---\n");
    // Handle void result - just print tag or error
    if (spec.type == fmt_Type_void) {
        if_ok((error_result)(value)) {
            let_ignore = value;
            return fmt__writePadded(writer, u8_l("ok"), spec);
        } else_err((e)) {
            return fmt_formatErr(writer, e, spec);
        }
    }
    // Handle error case
    if_err((error_result)(e)) {
        return fmt_formatErr(writer, e, spec);
    } else_ok((value)) { // Format the ok value
        return fmt__formatArgValue(writer, value, spec);
    }
    claim_unreachable;
};

/* TODO: Refactor this */
fn_((fmt__parseFormatSpecOnce(S_const$u8 fmt))(E$fmt__ParsedFormatSpec) $scope) {
    fmt__ParsedFormatSpec result = {};
    u8 next_positional = 0;
    S_const$u8 scan_fmt = fmt;
    isize current_literal_start = 0;

    while (0 < scan_fmt.len) {
        isize offset = scan_fmt.ptr - fmt.ptr;
        u8 ch = *atS(scan_fmt, 0);
        if (ch == u8_c('{')) {
            // Handle escaped '{{'
            if (1 < scan_fmt.len && *atS(scan_fmt, 1) == u8_c('{')) {
                scan_fmt = suffix$S(scan_fmt, 2);
                continue;
            }
            // Parse format spec
            let parsed = catch_((fmt__parseFormat(scan_fmt))($ignore, {
                // Malformed - skip to closing brace or next char
                usize close_pos = 1;
                while (close_pos < scan_fmt.len && *atS(scan_fmt, close_pos) != u8_c('}')) {
                    close_pos++;
                }
                scan_fmt = close_pos < scan_fmt.len ? suffix$S(scan_fmt, close_pos + 1) : suffix$S(scan_fmt, 1);
                continue;
            }));
            // printf("After parseFormat: spec.type=%d, spec.size=%d\n", parsed.spec.type, parsed.spec.size);
            // Store this occurrence with its literal prefix
            if (result.occurrence_count >= fmt_max_args) {
                return_err(fmt_Err_IdxOutOfBounds());
            }
            // printf("--- next_positional: %d\n", next_positional);
            let arg_index = expr_(u8 $scope)(if_some((parsed.spec.index)(idx)) {
                next_positional = prim_max(next_positional, idx + 1);
                $break_(idx);
            } else_none {
                $break_(next_positional++);
            }) $unscoped_(expr);
            if (arg_index >= fmt_max_args) {
                return_err(fmt_Err_IdxOutOfBounds());
            }
            asg_lit((atA(result.occurrences, result.occurrence_count))({
                .spec = parsed.spec,
                .arg_index = arg_index,
                .literal_start = intCast$((usize)(current_literal_start)),
                .literal_len = intCast$((usize)(offset - current_literal_start)),
            }));
            // printf("Stored in occurrence[%zu]: type=%d, size=%d\n", result.occurrence_count, atA(result.occurrences, result.occurrence_count)->spec.type, atA(result.occurrences, result.occurrence_count)->spec.size);
            result.occurrence_count++;
            result.max_arg_index = prim_max(result.max_arg_index, arg_index);
            scan_fmt = parsed.remain;
            current_literal_start = scan_fmt.ptr - fmt.ptr;
        } else if (ch == u8_c('}')) {
            // Handle escaped '}}'
            if (1 < scan_fmt.len && *atS(scan_fmt, 1) == u8_c('}')) {
                scan_fmt = suffix$S(scan_fmt, 2);
                continue;
            }
            scan_fmt = suffix$S(scan_fmt, 1);
        } else {
            scan_fmt = suffix$S(scan_fmt, 1);
        }
    }
    // Store trailing literal info
    result.trailing_literal_start = intCast$((usize)(current_literal_start));
    result.trailing_literal_len = intCast$((usize)((fmt.ptr + fmt.len) - (fmt.ptr + current_literal_start)));
    return_ok(result);
} $unscoped_(fn);

fn_((fmt__writeLiteralChunk(io_Writer writer, S_const$u8 fmt, usize start, usize len))(E$void) $scope) {
    if (len == 0) { return_ok({}); }
    let chunk = S_slice((fmt)$r(start, start + len));
    var_(pos, usize) = 0;
    while (pos < chunk.len) {
        let ch = *S_at((chunk)[pos]);
        // Check for escaped braces
        if (ch == u8_c('{') && pos + 1 < chunk.len && *S_at((chunk)[pos + 1]) == u8_c('{')) {
            try_(io_Writer_writeByte(writer, u8_c('{')));
            pos += 2;
        } else if (ch == u8_c('}') && pos + 1 < chunk.len && *S_at((chunk)[pos + 1]) == u8_c('}')) {
            try_(io_Writer_writeByte(writer, u8_c('}')));
            pos += 2;
        } else {
            try_(io_Writer_writeByte(writer, ch));
            pos++;
        }
    }
    return_ok({});
} $unscoped_(fn);

fn_((fmt__ArgValue_Tag_isInt(fmt__ArgValue_Tag tag))(bool)) {
    return (fmt__ArgValue_u8 <= tag && tag <= fmt__ArgValue_usize)
        || (fmt__ArgValue_i8 <= tag && tag <= fmt__ArgValue_isize);
};

fn_((fmt__ArgValue_Tag_getIntSize(fmt__ArgValue_Tag tag))(u8)) {
    switch (tag) {
    case fmt__ArgValue_u8:
    case fmt__ArgValue_i8:
        return 8;
    case fmt__ArgValue_u16:
    case fmt__ArgValue_i16:
        return 16;
    case fmt__ArgValue_u32:
    case fmt__ArgValue_i32:
        return 32;
    case fmt__ArgValue_u64:
    case fmt__ArgValue_i64:
        return 64;
    default:
        claim_unreachable;
    }
};

fn_((fmt__ArgValue_Tag_getMostGeneralType(fmt__ArgValue_Tag lhs, fmt__ArgValue_Tag rhs))(fmt__ArgValue_Tag)) {
    // If both same, return either
    if (lhs == rhs) { return lhs; }
    // Signed/unsigned of same size: prefer unsigned (can format both ways)
    if (lhs == fmt__ArgValue_i32 && rhs == fmt__ArgValue_u32) { return fmt__ArgValue_u32; }
    if (lhs == fmt__ArgValue_u32 && rhs == fmt__ArgValue_i32) { return fmt__ArgValue_u32; }
    if (lhs == fmt__ArgValue_i64 && rhs == fmt__ArgValue_u64) { return fmt__ArgValue_u64; }
    if (lhs == fmt__ArgValue_u64 && rhs == fmt__ArgValue_i64) { return fmt__ArgValue_u64; }
    // Different sizes: prefer larger
    if (fmt__ArgValue_Tag_isInt(lhs) && fmt__ArgValue_Tag_isInt(rhs)) {
        return fmt__ArgValue_Tag_getIntSize(lhs) > fmt__ArgValue_Tag_getIntSize(rhs) ? lhs : rhs;
    }
    // Incompatible types: keep first (best effort)
    return lhs;
};
