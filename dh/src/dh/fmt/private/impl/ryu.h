/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ryu.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/fmt/private/impl
 * @prefix  fmt__ryu
 *
 * @brief   Ryu algorithm lookup tables for float formatting
 * @details Tables for computing powers of 5 efficiently.
 *          Supports both SMALL (~816 bytes) and FULL (~10.7 KB) versions.
 *
 * @par Version Selection
 * Define `fmt_flt_ryu_table_type` to `fmt_flt_ryu_table_type_small` to use small version.
 * Otherwise, full version is used (default, recommended).
 *
 * @see [Ryu Paper: Ulf Adams (2018), "Ryū: fast float-to-string conversion"](https://dl.acm.org/doi/pdf/10.1145/3360595)
 * @see [Zig Implementation](https://github.com/ziglang/zig/blob/master/lib/std/fmt/float.zig)
 */
#pragma once
#ifndef fmt_private_impl_ryu__included
#define fmt_private_impl_ryu__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/fmt/cfg.h"

/*========== Macros and Definitions =========================================*/

/* --- Constants --- */

#define fmt__ryu_bit_count (125)
#define fmt__ryu_special_exponent (0x7fffffff)

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full \
    || fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
/// Direct lookup table size for pow5
#define fmt__ryu_table_pow5_size (326)
/// Direct lookup table size for pow5_inv
#define fmt__ryu_table_pow5_inv_size (342)

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small
/// Base table size (powers 5^0 through 5^25)
#define fmt__ryu_table_pow5_base_size (26)
/// Split table size for pow5 (powers of 5^(i*26))
#define fmt__ryu_table_pow5_split_size (13)
/// Split table size for pow5_inv
#define fmt__ryu_table_pow5_inv_split_size (15)
/// Offset table size for pow5 corrections
#define fmt__ryu_table_pow5_offsets_size (21)
/// Offset table size for pow5_inv corrections
#define fmt__ryu_table_pow5_inv_offsets_size (19)

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_unknown
#endif /* fmt_flt_ryu_table_type */

/* --- Types --- */

/// 128-bit value represented as [low_64bits, high_64bits]
typedef A$$(2, u64) fmt__ryu_TableEntry;

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
typedef A$$(fmt__ryu_table_pow5_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5;
typedef A$$(fmt__ryu_table_pow5_inv_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5Inv;

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small
typedef A$$(fmt__ryu_table_pow5_base_size, u64) fmt__ryu_TablePow5Base;
typedef A$$(fmt__ryu_table_pow5_split_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5Split;
typedef A$$(fmt__ryu_table_pow5_inv_split_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5InvSplit;
typedef A$$(fmt__ryu_table_pow5_offsets_size, u32) fmt__ryu_TablePow5Offsets;
typedef A$$(fmt__ryu_table_pow5_inv_offsets_size, u32) fmt__ryu_TablePow5InvOffsets;

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_unknown
#endif /* fmt_flt_ryu_table_type */

/* --- Tables --- */

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
/// @brief Power of 5 lookup table
/// @details Direct lookup for 5^i, i ∈ [0, 325]
///          Size: 326 entries × 16 bytes = 5,216 bytes
$extern let_(fmt__ryu_table_pow5, fmt__ryu_TablePow5);
/// @brief Inverse power of 5 lookup table
/// @details Direct lookup for 5^(-i), i ∈ [0, 341]
///          Size: 342 entries × 16 bytes = 5,472 bytes
$extern let_(fmt__ryu_table_pow5_inv, fmt__ryu_TablePow5Inv);

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small
/// @brief Base power of 5 table
/// @details Contains 5^i for i ∈ [0, 25]
///          Size: 26 entries × 8 bytes = 208 bytes
$extern let_(fmt__ryu_table_pow5_base, fmt__ryu_TablePow5Base);
/// @brief Power of 5 split table
/// @details Contains 5^(i*26) approximations
///          Size: 13 entries × 16 bytes = 208 bytes
$extern let_(fmt__ryu_table_pow5_split, fmt__ryu_TablePow5Split);
/// @brief Power of 5 correction offsets
/// @details Each entry contains 16 packed 2-bit correction values
///          Size: 21 entries × 4 bytes = 84 bytes
$extern let_(fmt__ryu_table_pow5_offsets, fmt__ryu_TablePow5Offsets);
/// @brief Inverse power of 5 split table
/// @details Contains 5^(-i*26) approximations
///          Size: 15 entries × 16 bytes = 240 bytes
$extern let_(fmt__ryu_table_pow5_inv_split, fmt__ryu_TablePow5InvSplit);
/// @brief Inverse power of 5 correction offsets
/// @details Each entry contains 16 packed 2-bit correction values
///          Size: 19 entries × 4 bytes = 76 bytes
$extern let_(fmt__ryu_table_pow5_inv_offsets, fmt__ryu_TablePow5InvOffsets);

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_unknown
#endif /* fmt_flt_ryu_table_type */

/* --- Helper Functions --- */

/// if (e == 0) 1 else ceil(log_2(5^e))
$attr($inline_always)
$static fn_((fmt__ryu_pow5Bits(u32 e))(u32));

/* --- Table Access Functions --- */

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
/// Get power of 5 approximation (full version - direct lookup)
$attr($inline_always)
$static fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry));
/// Get inverse power of 5 approximation (full version - direct lookup)
$attr($inline_always)
$static fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry));

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small
/// Compute 5^i using split tables and offsets
$attr($inline_always)
$static fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry));
/// Compute 5^(-i) using split tables and offsets
$attr($inline_always)
$static fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry));

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
$attr($inline_always)
$static fn_((fmt__ryu_limbMul5Add(u64* limb, u64 carry))(u64));
#define fmt__ryu_bigint_limb_count (16)
typedef A$$(fmt__ryu_bigint_limb_count, u64) fmt__ryu_BigInt;
$attr($inline_always)
$static fn_((fmt__ryu_bigintPow5(u32 e))(fmt__ryu_BigInt));
$attr($inline_always)
$static fn_((fmt__ryu_bigintShr128(fmt__ryu_BigInt value, u32 shift))(fmt__ryu_TableEntry));
$attr($inline_always)
$static fn_((fmt__ryu_bigintShl128(fmt__ryu_BigInt value, u32 shift))(fmt__ryu_TableEntry));
$attr($inline_always)
$static fn_((fmt__ryu_bigintCompare(fmt__ryu_BigInt lhs, fmt__ryu_BigInt rhs))(i32));
$attr($inline_always)
$static fn_((fmt__ryu_bigintSub(fmt__ryu_BigInt* lhs, fmt__ryu_BigInt rhs))(void));
$attr($inline_always)
$static fn_((fmt__ryu_bigintShl1(fmt__ryu_BigInt* self))(void));
$attr($inline_always)
$static fn_((fmt__ryu_tableEntryAdd1(fmt__ryu_TableEntry value))(fmt__ryu_TableEntry));

$attr($inline_always)
$static fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry));
$attr($inline_always)
$static fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry));

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_unknown
#endif /* fmt_flt_ryu_table_type */

/* --- 64-bit Arithmetic --- */

/// Multiply two u64 values and get high 64 bits of result
/// Returns the high 64 bits of (a * b)
$attr($inline_always)
$static fn_((fmt__ryu_mul64High(u64 lhs, u64 rhs))(u64));
/// Multiply and shift: (m * mul) >> shift
/// Note: shift is always in range (64, 128) for f64
/// Computes: ((m * mul_lo) >> 64 + m * mul_hi) >> (shift - 64)
$attr($inline_always)
$static fn_((fmt__ryu_mulShift64(u64 m, fmt__ryu_TableEntry mul, u32 shift))(u64));

/*========== Macros and Definitions =========================================*/

/* --- Helper Functions --- */

fn_((fmt__ryu_pow5Bits(u32 e))(u32)) {
    return as$(u32)(((as$(u64)(e) * 163391164108059ull) >> 46) + 1);
};

/* --- Table Access Functions --- */

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_size);
    return *A_at((fmt__ryu_table_pow5)[i]);
};
fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_inv_size);
    return *A_at((fmt__ryu_table_pow5_inv)[i]);
};

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small
fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry)) {
    let base = i / fmt__ryu_table_pow5_base_size;
    let base2 = base * fmt__ryu_table_pow5_base_size;
    let mul = *A_at((fmt__ryu_table_pow5_split)[base]);

    if (i == base2) {
        return (fmt__ryu_TableEntry)A_init({ *A_at((mul)[0]), *A_at((mul)[1]) });
    }

    let offset = i - base2;
    let m = *A_at((fmt__ryu_table_pow5_base)[offset]);

    // 128-bit multiply: (mul[0:1]) * m
    let b0_lo = (*A_at((mul)[0]) & 0xFFFFFFFFull) * m;
    let b0_hi = (*A_at((mul)[0]) >> 32) * m;
    let b2_lo = (*A_at((mul)[1]) & 0xFFFFFFFFull) * m;
    let b2_hi = (*A_at((mul)[1]) >> 32) * m;

    // Combine partial products
    let mid1 = b0_hi + (b0_lo >> 32);
    let b0_result = (mid1 << 32) | (b0_lo & 0xFFFFFFFFull);
    let b2_result = b2_hi + (b2_lo >> 32) + (mid1 >> 32) + (b2_lo & 0xFFFFFFFFull);

    // Shift by delta
    let delta = fmt__ryu_pow5Bits(i) - fmt__ryu_pow5Bits(base2);
    var shifted_low = (b0_result >> delta) | (b2_result << (64 - delta));
    var shifted_high = b2_result >> delta;

    // Add correction offset
    let shift = (i % 16) << 1;
    let corr = (*A_at((fmt__ryu_table_pow5_offsets)[i / 16]) >> shift) & 3;

    shifted_low += 1 + corr;
    if (shifted_low < (1 + corr)) { shifted_high++; } // Carry

    return (fmt__ryu_TableEntry)A_init({ shifted_low, shifted_high });
};
fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry)) {
    let base = (i + fmt__ryu_table_pow5_base_size - 1) / fmt__ryu_table_pow5_base_size;
    let base2 = base * fmt__ryu_table_pow5_base_size;
    let mul = *A_at((fmt__ryu_table_pow5_inv_split)[base]);

    if (i == base2) {
        return (fmt__ryu_TableEntry)A_init({ *A_at((mul)[0]), *A_at((mul)[1]) });
    }

    let offset = base2 - i;
    let m = *A_at((fmt__ryu_table_pow5_base)[offset]);

    // 128-bit multiply: (mul[0]-1, mul[1]) * m
    let mul0_minus_1 = *A_at((mul)[0]) - 1;
    let b0_lo = (mul0_minus_1 & 0xFFFFFFFFull) * m;
    let b0_hi = (mul0_minus_1 >> 32) * m;
    let b2_lo = (*A_at((mul)[1]) & 0xFFFFFFFFull) * m;
    let b2_hi = (*A_at((mul)[1]) >> 32) * m;

    // Combine partial products
    let mid1 = b0_hi + (b0_lo >> 32);
    let b0_result = (mid1 << 32) | (b0_lo & 0xFFFFFFFFull);
    let b2_result = b2_hi + (b2_lo >> 32) + (mid1 >> 32) + (b2_lo & 0xFFFFFFFFull);

    // Shift by delta
    let delta = fmt__ryu_pow5Bits(base2) - fmt__ryu_pow5Bits(i);
    var shifted_low = (b0_result >> delta) | (b2_result << (64 - delta));
    var shifted_high = b2_result >> delta;

    // Add correction offset
    let shift = (i % 16) << 1;
    let corr = (*A_at((fmt__ryu_table_pow5_inv_offsets)[i / 16]) >> shift) & 3;

    shifted_low += 1 + corr;
    if (shifted_low < (1 + corr)) { shifted_high++; } // Carry

    return (fmt__ryu_TableEntry)A_init({ shifted_low, shifted_high });
};

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_less
fn_((fmt__ryu_limbMul5Add(u64* limb, u64 carry))(u64)) {
    let value = *limb;
    var lo = value << 2;
    var hi = value >> 62;

    let add_value = lo + value;
    hi += add_value < lo ? 1 : 0;
    lo = add_value;

    let add_carry = lo + carry;
    hi += add_carry < lo ? 1 : 0;
    *limb = add_carry;
    return hi;
};
fn_((fmt__ryu_bigintPow5(u32 e))(fmt__ryu_BigInt)) {
    var out = (fmt__ryu_BigInt)cleared();
    *A_at((out)[0]) = 1;
    for_(($r(0, e))(step)) {
        let_ignore = step;
        var carry = u64_(0);
        for_(($s(A_ref(out)))(limb)) {
            carry = fmt__ryu_limbMul5Add(limb, carry);
        } $end(for);
        claim_assert(carry == 0);
    } $end(for);
    return out;
};
fn_((fmt__ryu_bigintShr128(fmt__ryu_BigInt value, u32 shift))(fmt__ryu_TableEntry)) {
    let limb_idx = shift / 64;
    let bit_idx = shift % 64;
    let lo0 = limb_idx < fmt__ryu_bigint_limb_count ? *A_at((value)[limb_idx]) : 0;
    let lo1 = limb_idx + 1 < fmt__ryu_bigint_limb_count ? *A_at((value)[limb_idx + 1]) : 0;
    let lo2 = limb_idx + 2 < fmt__ryu_bigint_limb_count ? *A_at((value)[limb_idx + 2]) : 0;
    if (bit_idx == 0) {
        return (fmt__ryu_TableEntry)A_init({ lo0, lo1 });
    }
    return (fmt__ryu_TableEntry)A_init({
        (lo0 >> bit_idx) | (lo1 << (64 - bit_idx)),
        (lo1 >> bit_idx) | (lo2 << (64 - bit_idx)),
    });
};
fn_((fmt__ryu_bigintShl128(fmt__ryu_BigInt value, u32 shift))(fmt__ryu_TableEntry)) {
    claim_assert(shift < 128);
    let limb_shift = shift / 64;
    let bit_shift = shift % 64;
    var_(lo, u64) = 0;
    var_(hi, u64) = 0;
    if (limb_shift == 0) {
        lo = *A_at((value)[0]) << bit_shift;
        hi = *A_at((value)[1]) << bit_shift;
        if (bit_shift != 0) {
            hi |= *A_at((value)[0]) >> (64 - bit_shift);
        }
    } else if (limb_shift == 1) {
        hi = *A_at((value)[0]) << bit_shift;
    }
    return (fmt__ryu_TableEntry)A_init({ lo, hi });
};
fn_((fmt__ryu_bigintCompare(fmt__ryu_BigInt lhs, fmt__ryu_BigInt rhs))(i32)) {
    for_($rev($a(lhs), $a(rhs))(l, r)) {
        if (*l < *r) return -1;
        if (*l > *r) return 1;
    } $end(for);
    return 0;
};
fn_((fmt__ryu_bigintSub(fmt__ryu_BigInt* lhs, fmt__ryu_BigInt rhs))(void)) {
    var borrow = u64_(0);
    for_(($s(A_ref(*lhs)), $a(rhs))(l_limb, r_limb)) {
        let l = *l_limb;
        let r = *r_limb;
        let next = l - r - borrow;
        borrow = borrow ? (l <= r) : (l < r);
        *l_limb = next;
    } $end(for);
    claim_assert(borrow == 0);
};
fn_((fmt__ryu_bigintShl1(fmt__ryu_BigInt* self))(void)) {
    var carry = u64_(0);
    for_(($s(A_ref(*self)))(limb)) {
        let next_carry = *limb >> 63;
        *limb = (*limb << 1) | carry;
        carry = next_carry;
    } $end(for);
    claim_assert(carry == 0);
};
fn_((fmt__ryu_tableEntryAdd1(fmt__ryu_TableEntry value))(fmt__ryu_TableEntry)) {
    var lo = *A_at((value)[0]) + 1;
    var hi = *A_at((value)[1]);
    if (lo == 0) hi += 1;
    return (fmt__ryu_TableEntry)A_init({ lo, hi });
};

fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_size);
    let pow5 = fmt__ryu_bigintPow5(i);
    let bits = fmt__ryu_pow5Bits(i);
    if (bits <= fmt__ryu_bit_count) {
        return fmt__ryu_bigintShl128(pow5, fmt__ryu_bit_count - bits);
    }
    return fmt__ryu_bigintShr128(pow5, bits - fmt__ryu_bit_count);
};
fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_inv_size);
    let divisor = fmt__ryu_bigintPow5(i);
    let numerator_bit = fmt__ryu_pow5Bits(i) - 1 + fmt__ryu_bit_count;
    var remainder = (fmt__ryu_BigInt)cleared();
    var quotient = (fmt__ryu_TableEntry)cleared();

    for_(($r(0, numerator_bit + 1))(offset)) {
        let bit = numerator_bit - offset;
        fmt__ryu_bigintShl1(&remainder);
        if (bit == numerator_bit) {
            *A_at((remainder)[0]) = 1;
        }
        if (fmt__ryu_bigintCompare(remainder, divisor) >= 0) {
            fmt__ryu_bigintSub(&remainder, divisor);
            claim_assert(bit < 128);
            if (bit < 64) {
                *A_at((quotient)[0]) |= 1ull << bit;
            } else {
                *A_at((quotient)[1]) |= 1ull << (bit - 64);
            }
        }
    } $end(for);

    return fmt__ryu_tableEntryAdd1(quotient);
};

#elif fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_unknown
#endif /* fmt_flt_ryu_table_type */

/* --- 64-bit Arithmetic --- */

fn_((fmt__ryu_mul64High(u64 lhs, u64 rhs))(u64)) {
    // Split into 32-bit parts
    const u64 l_lo = lhs & 0xFFFFFFFFull;
    const u64 l_hi = lhs >> 32;
    const u64 r_lo = rhs & 0xFFFFFFFFull;
    const u64 r_hi = rhs >> 32;

    const u64 t0 = l_lo * r_lo;
    const u64 w1 = t0 >> 32;

    const u64 t1 = l_hi * r_lo + w1;
    const u64 w2 = t1 & 0xFFFFFFFFull;
    const u64 w3 = t1 >> 32;

    const u64 t2 = l_lo * r_hi + w2;
    const u64 k = t2 >> 32;

    return l_hi * r_hi + w3 + k;
};
fn_((fmt__ryu_mulShift64(u64 m, fmt__ryu_TableEntry mul, u32 shift))(u64)) {
    // mul is [low, high] representing a 128-bit value
    const u64 mul_lo = *A_at((mul)[0]);
    const u64 mul_hi = *A_at((mul)[1]);

    // b0 = m * mul_lo as 128-bit, we need high 64 bits
    const u64 b0_high = fmt__ryu_mul64High(m, mul_lo);

    // b2 = m * mul_hi as 128-bit
    const u64 b2_lo = m * mul_hi;
    const u64 b2_hi = fmt__ryu_mul64High(m, mul_hi);

    // sum = b0_high + b2 as a 128-bit pair
    // sum_lo = b0_high + b2_lo, with carry to sum_hi
    const u64 sum_lo = b0_high + b2_lo;
    const u64 carry = (sum_lo < b0_high) ? 1 : 0;
    const u64 sum_hi = b2_hi + carry;

    // result = sum >> (shift - 64)
    // shift is in (64, 128), so remaining_shift is in (0, 64)
    const u32 remaining_shift = shift - 64;
    return (sum_lo >> remaining_shift) | (sum_hi << (64 - remaining_shift));
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_private_impl_ryu__included */
