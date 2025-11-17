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

/// Unsigned integer

typedef uint8_t u8;
#define u8_limit     u8_limit_max
#define u8_limit_min (0)
#define u8_limit_max (UINT8_MAX)
#define u8_bits      (8)
#define u8_bytes     (1)

typedef uint16_t u16;
#define u16_limit     u16_limit_max
#define u16_limit_min (0)
#define u16_limit_max (UINT16_MAX)
#define u16_bits      (16)
#define u16_bytes     (2)

typedef uint32_t u32;
#define u32_limit     u32_limit_max
#define u32_limit_min (0)
#define u32_limit_max (UINT32_MAX)
#define u32_bits      (32)
#define u32_bytes     (4)

typedef uint64_t u64;
#define u64_limit     u64_limit_max
#define u64_limit_min (0)
#define u64_limit_max (UINT64_MAX)
#define u64_bits      (64)
#define u64_bytes     (8)

// typedef __uint128_t u128;
// #define u128_limit u128_limit_max
// #define u128_limit_min (0)
// #define u128_limit_max (UINT128_MAX)
// #define u128_bits      (128)
// #define u128_bytes     (16)

typedef uintptr_t usize;
#define usize_limit     usize_limit_max
#define usize_limit_min (0)
#define usize_limit_max (UINTPTR_MAX)
#define usize_bits      (sizeOf$(usize) * 8)
#define usize_bytes     (sizeOf$(usize))

/// Signed integer

typedef int8_t i8;
#define i8_limit_min      (INT8_MIN)
#define i8_limit_max      (INT8_MAX)
#define i8_limit_max_ngtv (INT8_MIN)
#define i8_limit_min_ngtv (-1)
#define i8_limit_min_pstv (+1)
#define i8_limit_max_pstv (INT8_MAX)
#define i8_bits           (8)
#define i8_bytes          (1)

typedef int16_t i16;
#define i16_limit_min      (INT16_MIN)
#define i16_limit_max      (INT16_MAX)
#define i16_limit_max_ngtv (INT16_MIN)
#define i16_limit_min_ngtv (-1)
#define i16_limit_min_pstv (+1)
#define i16_limit_max_pstv (INT16_MAX)
#define i16_bits           (16)
#define i16_bytes          (2)

typedef int32_t i32;
#define i32_limit_min      (INT32_MIN)
#define i32_limit_max      (INT32_MAX)
#define i32_limit_max_ngtv (INT32_MIN)
#define i32_limit_min_ngtv (-1)
#define i32_limit_min_pstv (+1)
#define i32_limit_max_pstv (INT32_MAX)
#define i32_bits           (32)
#define i32_bytes          (4)

typedef int64_t i64;
#define i64_limit_min      (INT64_MIN)
#define i64_limit_max      (INT64_MAX)
#define i64_limit_max_ngtv (INT64_MIN)
#define i64_limit_min_ngtv (-1)
#define i64_limit_min_pstv (+1)
#define i64_limit_max_pstv (INT64_MAX)
#define i64_bits           (64)
#define i64_bytes          (8)

// typedef __int128_t i128;
// #define i128_limit_min (INT128_MIN)
// #define i128_limit_max (INT128_MAX)
// #define i128_limit_max_ngtv (INT128_MIN)
// #define i128_limit_min_ngtv (-1)
// #define i128_limit_min_pstv (+1)
// #define i128_limit_max_pstv (INT128_MAX)
// #define i128_bits      (128)
// #define i128_bytes     (16)

typedef intptr_t isize;
#define isize_limit_min      (INTPTR_MIN)
#define isize_limit_max      (INTPTR_MAX)
#define isize_limit_max_ngtv (INTPTR_MIN)
#define isize_limit_min_ngtv (-1)
#define isize_limit_min_pstv (+1)
#define isize_limit_max_pstv (INTPTR_MAX)
#define isize_bits           (sizeOf$(isize) * 8)
#define isize_bytes          (sizeOf$(isize))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_int__included */
