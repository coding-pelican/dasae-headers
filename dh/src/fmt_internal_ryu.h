/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    fmt_internal_ryu.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2025-12-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fmt/internal
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
#ifndef fmt_fmt__ryu__included
#define fmt_fmt__ryu__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/fmt/cfg.h"

/*========== Macros and Definitions =========================================*/

/* --- Constants --- */

#define fmt__ryu_bit_count (125)

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
/// Direct lookup table size for pow5
#define fmt__ryu_table_pow5_size (326)
/// Direct lookup table size for pow5_inv
#define fmt__ryu_table_pow5_inv_size (342)
#else /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small */

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
#endif /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full / fmt_flt_ryu_table_type_small */

/* --- Types --- */

/// 128-bit value represented as [low_64bits, high_64bits]
typedef A$$(2, u64) fmt__ryu_TableEntry;

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
typedef A$$(fmt__ryu_table_pow5_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5;
typedef A$$(fmt__ryu_table_pow5_inv_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5Inv;

#else /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small */
typedef A$$(fmt__ryu_table_pow5_base_size, u64) fmt__ryu_TablePow5Base;
typedef A$$(fmt__ryu_table_pow5_split_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5Split;
typedef A$$(fmt__ryu_table_pow5_inv_split_size, fmt__ryu_TableEntry) fmt__ryu_TablePow5InvSplit;
typedef A$$(fmt__ryu_table_pow5_offsets_size, u32) fmt__ryu_TablePow5Offsets;
typedef A$$(fmt__ryu_table_pow5_inv_offsets_size, u32) fmt__ryu_TablePow5InvOffsets;
#endif /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full / fmt_flt_ryu_table_type_small */

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

#else /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small */
/// @brief Base power of 5 table
/// @details Contains 5^i for i ∈ [0, 25]
///          Size: 26 entries × 8 bytes = 208 bytes
$extern let_(fmt__ryu_table_pow5_base, fmt__ryu_TablePow5Base);
/// @brief Power of 5 split table
/// @details Contains 5^(i*26) approximations
///          Size: 13 entries × 16 bytes = 208 bytes
$extern let_(fmt__ryu_table_pow5_split, fmt__ryu_TablePow5Split);
/// @brief Inverse power of 5 split table
/// @details Contains 5^(-i*26) approximations
///          Size: 15 entries × 16 bytes = 240 bytes
$extern let_(fmt__ryu_table_pow5_inv_split, fmt__ryu_TablePow5InvSplit);
/// @brief Power of 5 correction offsets
/// @details Each entry contains 16 packed 2-bit correction values
///          Size: 21 entries × 4 bytes = 84 bytes
$extern let_(fmt__ryu_table_pow5_offsets, fmt__ryu_TablePow5Offsets);
/// @brief Inverse power of 5 correction offsets
/// @details Each entry contains 16 packed 2-bit correction values
///          Size: 19 entries × 4 bytes = 76 bytes
$extern let_(fmt__ryu_table_pow5_inv_offsets, fmt__ryu_TablePow5InvOffsets);
#endif /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full / fmt_flt_ryu_table_type_small */

/* --- Helper Functions --- */

/// if (e == 0) 1 else ceil(log_2(5^e))
$attr($inline_always)
$static fn_((fmt__ryu_pow5Bits(u32 e))(u32)) {
    return as$(u32)(((as$(u64)(e) * 163391164108059ull) >> 46) + 1);
};

/* --- Table Access Functions --- */

#if fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full
/// Get power of 5 approximation (full version - direct lookup)
$attr($inline_always)
$static fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_size);
    return *A_at((fmt__ryu_table_pow5)[i]);
};

/// Get inverse power of 5 approximation (full version - direct lookup)
$attr($inline_always)
$static fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry)) {
    claim_assert(i < fmt__ryu_table_pow5_inv_size);
    return *A_at((fmt__ryu_table_pow5_inv)[i]);
};

#else /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_small */
/// Compute 5^i using split tables and offsets
$attr($inline_always)
$static fn_((fmt__ryu_pow5(u32 i))(fmt__ryu_TableEntry)) {
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

/// Compute 5^(-i) using split tables and offsets
$attr($inline_always)
$static fn_((fmt__ryu_pow5Inv(u32 i))(fmt__ryu_TableEntry)) {
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
#endif /* fmt_flt_ryu_table_type == fmt_flt_ryu_table_type_full / fmt_flt_ryu_table_type_small */

/* --- 64-bit Arithmetic --- */

/// Multiply two u64 values and get high 64 bits of result
/// Returns the high 64 bits of (a * b)
$attr($inline_always)
$static fn_((mul64_high(u64 lhs, u64 rhs))(u64)) {
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

/// Multiply and shift: (m * mul) >> shift
/// Note: shift is always in range (64, 128) for f64
/// Computes: ((m * mul_lo) >> 64 + m * mul_hi) >> (shift - 64)
$attr($inline_always)
$static fn_((fmt__ryu_mulShift64(u64 m, fmt__ryu_TableEntry mul, u32 shift))(u64)) {
    // mul is [low, high] representing a 128-bit value
    const u64 mul_lo = *A_at((mul)[0]);
    const u64 mul_hi = *A_at((mul)[1]);

    // b0 = m * mul_lo as 128-bit, we need high 64 bits
    const u64 b0_high = mul64_high(m, mul_lo);

    // b2 = m * mul_hi as 128-bit
    const u64 b2_lo = m * mul_hi;
    const u64 b2_hi = mul64_high(m, mul_hi);

    // sum = (b0_high as u128) + b2
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
#endif /* fmt_fmt__ryu__included */
