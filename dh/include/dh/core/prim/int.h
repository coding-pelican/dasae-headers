/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    int.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-02 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Integer type utilities
 * @details Provides integer type utilities and functions for:
 *          - Integer type definition
 *          - Integer type usage
 *          - Integer type operations
 *          - Integer type utilities
 */
#ifndef core_prim_int__included
#define core_prim_int__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include <stdint.h>

/*========== Macros and Definitions =========================================*/

/* --- Unsigned integer --- */

typedef uint8_t u8;
#define u8_bits (8u)
#define u8_bytes (1u)
#define u8_limit u8_limit_max
#define u8_limit_min (as$(u8)(0))
#define u8_limit_max (as$(u8)(UINT8_MAX))
#define u8_limit_flt_min_bound_excl (-1.0)
#define u8_limit_flt_max_bound_excl (0x1p8)

typedef uint16_t u16;
#define u16_bits (16u)
#define u16_bytes (2u)
#define u16_limit u16_limit_max
#define u16_limit_min (as$(u16)(0))
#define u16_limit_max (as$(u16)(UINT16_MAX))
#define u16_limit_flt_min_bound_excl (-1.0)
#define u16_limit_flt_max_bound_excl (0x1p16)

typedef uint32_t u32;
#define u32_bits (32u)
#define u32_bytes (4u)
#define u32_limit u32_limit_max
#define u32_limit_min (as$(u32)(0))
#define u32_limit_max (as$(u32)(UINT32_MAX))
#define u32_limit_flt_min_bound_excl (-1.0)
#define u32_limit_flt_max_bound_excl (0x1p32)

typedef uint64_t u64;
#define u64_bits (64u)
#define u64_bytes (8u)
#define u64_limit u64_limit_max
#define u64_limit_min (as$(u64)(0))
#define u64_limit_max (as$(u64)(UINT64_MAX))
#define u64_limit_flt_min_bound_excl (-1.0)
#define u64_limit_flt_max_bound_excl (0x1p64)

// typedef __uint128_t u128;
// #define u128_bits (128u)
// #define u128_bytes (16u)
// #define u128_limit u128_limit_max
// #define u128_limit_min (as$(u128)(0))
// #define u128_limit_max (as$(u128)(UINT128_MAX))
// #define u128_limit_flt_min_bound_excl (-1.0)
// #define u128_limit_flt_max_bound_excl (0x1p127)

typedef uintptr_t usize;
#define usize_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_bits), \
    pp_else_(u32_bits) \
)
#define usize_bytes pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_bytes), \
    pp_else_(u32_bytes) \
)
#define usize_limit usize_limit_max
#define usize_limit_min pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_min), \
    pp_else_(u32_limit_min) \
)
#define usize_limit_max pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_max), \
    pp_else_(u32_limit_max) \
)
#define usize_limit_flt_min_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_flt_min_bound_excl), \
    pp_else_(u32_limit_flt_min_bound_excl) \
)
#define usize_limit_flt_max_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_flt_max_bound_excl), \
    pp_else_(u32_limit_flt_max_bound_excl) \
)

/* --- Signed integer --- */

typedef int8_t i8;
#define i8_bits (8u)
#define i8_bytes (1u)
#define i8_limit_min (as$(i8)(INT8_MIN))
#define i8_limit_max (as$(i8)(INT8_MAX))
#define i8_limit_max_ngtv (as$(i8)(INT8_MIN))
#define i8_limit_min_ngtv (as$(i8)(-1))
#define i8_limit_min_pstv (as$(i8)(+1))
#define i8_limit_max_pstv (as$(i8)(INT8_MAX))
#define i8_limit_flt_min_bound_excl (-0x1p7 - 1.0)
#define i8_limit_flt_max_bound_excl (0x1p7)

typedef int16_t i16;
#define i16_bits (16u)
#define i16_bytes (2u)
#define i16_limit_min (as$(i16)(INT16_MIN))
#define i16_limit_max (as$(i16)(INT16_MAX))
#define i16_limit_max_ngtv (as$(i16)(INT16_MIN))
#define i16_limit_min_ngtv (as$(i16)(-1))
#define i16_limit_min_pstv (as$(i16)(+1))
#define i16_limit_max_pstv (as$(i16)(INT16_MAX))
#define i16_limit_flt_min_bound_excl (-0x1p15 - 1.0)
#define i16_limit_flt_max_bound_excl (0x1p15)

typedef int32_t i32;
#define i32_bits (32u)
#define i32_bytes (4u)
#define i32_limit_min (as$(i32)(INT32_MIN))
#define i32_limit_max (as$(i32)(INT32_MAX))
#define i32_limit_max_ngtv (as$(i32)(INT32_MIN))
#define i32_limit_min_ngtv (as$(i32)(-1))
#define i32_limit_min_pstv (as$(i32)(+1))
#define i32_limit_max_pstv (as$(i32)(INT32_MAX))
#define i32_limit_flt_min_bound_excl (-0x1p31 - 1.0)
#define i32_limit_flt_max_bound_excl (0x1p31)

typedef int64_t i64;
#define i64_bits (64u)
#define i64_bytes (8u)
#define i64_limit_min (as$(i64)(INT64_MIN))
#define i64_limit_max (as$(i64)(INT64_MAX))
#define i64_limit_max_ngtv (as$(i64)(INT64_MIN))
#define i64_limit_min_ngtv (as$(i64)(-1))
#define i64_limit_min_pstv (as$(i64)(+1))
#define i64_limit_max_pstv (as$(i64)(INT64_MAX))
#define i64_limit_flt_min_bound_excl (-0x1p63 - 1.0)
#define i64_limit_flt_max_bound_excl (0x1p63)

// typedef __int128_t i128;
// #define i128_bits      (128u)
// #define i128_bytes     (16u)
// #define i128_limit_min (as$(i128)(INT128_MIN))
// #define i128_limit_max (as$(i128)(INT128_MAX))
// #define i128_limit_max_ngtv (as$(i128)(INT128_MIN))
// #define i128_limit_min_ngtv (as$(i128)(-1))
// #define i128_limit_min_pstv (as$(i128)(+1))
// #define i128_limit_max_pstv (as$(i128)(INT128_MAX))
// #define i128_limit_flt_min_bound_excl (-0x1p127 - 1.0)
// #define i128_limit_flt_max_bound_excl (0x1p127)

typedef intptr_t isize;
#define isize_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_bits), \
    pp_else_(i32_bits) \
)
#define isize_bytes pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_bytes), \
    pp_else_(i32_bytes) \
)
#define isize_limit_min pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min), \
    pp_else_(i32_limit_min) \
)
#define isize_limit_max pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max), \
    pp_else_(i32_limit_max) \
)
#define isize_limit_max_ngtv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max_ngtv), \
    pp_else_(i32_limit_max_ngtv) \
)
#define isize_limit_min_ngtv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min_ngtv), \
    pp_else_(i32_limit_min_ngtv) \
)
#define isize_limit_min_pstv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min_pstv), \
    pp_else_(i32_limit_min_pstv) \
)
#define isize_limit_max_pstv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max_pstv), \
    pp_else_(i32_limit_max_pstv) \
)
#define isize_limit_flt_min_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_flt_min_bound_excl), \
    pp_else_(i32_limit_flt_min_bound_excl) \
)
#define isize_limit_flt_max_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_flt_max_bound_excl), \
    pp_else_(i32_limit_flt_max_bound_excl) \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_int__included */
