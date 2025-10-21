/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    int.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-02 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
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

typedef uint16_t u16;
#define u16_limit     u16_limit_max
#define u16_limit_min (0)
#define u16_limit_max (UINT16_MAX)

typedef uint32_t u32;
#define u32_limit     u32_limit_max
#define u32_limit_min (0)
#define u32_limit_max (UINT32_MAX)

typedef uint64_t u64;
#define u64_limit     u64_limit_max
#define u64_limit_min (0)
#define u64_limit_max (UINT64_MAX)

// typedef __uint128_t u128;
// #define u128_limit u128_limit_max
// #define u128_limit_min (0)
// #define u128_limit_max (UINT128_MAX)

typedef uintptr_t usize;
#define usize_limit     usize_limit_max
#define usize_limit_min (0)
#define usize_limit_max (UINTPTR_MAX)

/// Signed integer

typedef int8_t i8;
#define i8_limit_min      (INT8_MIN)
#define i8_limit_max      (INT8_MAX)
#define i8_limit_max_ngtv (-INT8_MAX)
#define i8_limit_min_ngtv (-INT8_MIN)
#define i8_limit_min_pstv (+INT8_MIN)
#define i8_limit_max_pstv (+INT8_MAX)

typedef int16_t i16;
#define i16_limit_min      (INT16_MIN)
#define i16_limit_max      (INT16_MAX)
#define i16_limit_max_ngtv (-INT16_MAX)
#define i16_limit_min_ngtv (-INT16_MIN)
#define i16_limit_min_pstv (+INT16_MIN)
#define i16_limit_max_pstv (+INT16_MAX)

typedef int32_t i32;
#define i32_limit_min      (INT32_MIN)
#define i32_limit_max      (INT32_MAX)
#define i32_limit_max_ngtv (-INT32_MAX)
#define i32_limit_min_ngtv (-INT32_MIN)
#define i32_limit_min_pstv (+INT32_MIN)
#define i32_limit_max_pstv (+INT32_MAX)

typedef int64_t i64;
#define i64_limit_min      (INT64_MIN)
#define i64_limit_max      (INT64_MAX)
#define i64_limit_max_ngtv (-INT64_MAX)
#define i64_limit_min_ngtv (-INT64_MIN)
#define i64_limit_min_pstv (+INT64_MIN)
#define i64_limit_max_pstv (+INT64_MAX)

// typedef __int128_t i128;
// #define i128_limit_min (INT128_MIN)
// #define i128_limit_max (INT128_MAX)

typedef intptr_t isize;
#define isize_limit_min      (INTPTR_MIN)
#define isize_limit_max      (INTPTR_MAX)
#define isize_limit_max_ngtv (-INTPTR_MAX)
#define isize_limit_min_ngtv (-INTPTR_MIN)
#define isize_limit_min_pstv (+INTPTR_MIN)
#define isize_limit_max_pstv (+INTPTR_MAX)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_int__included */
