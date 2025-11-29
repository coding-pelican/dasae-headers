/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    flt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Floating point type utilities
 * @details Provides floating point type utilities and functions for:
 *          - Floating point type definition
 *          - Floating point type usage
 *          - Floating point type operations
 *          - Floating point type utilities
 */
#ifndef core_prim_flt__included
#define core_prim_flt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include <float.h>

/*========== Macros and Definitions =========================================*/

/* floating point */
// typedef _Float16 f16;
// #define f16_limit_min      (FLT16_MIN)
// #define f16_limit_max      (FLT16_MAX)
// #define f16_limit_max_ngtv (-FLT16_MAX)
// #define f16_limit_min_ngtv (-FLT16_MIN)
// #define f16_limit_min_pstv (+FLT16_MIN)
// #define f16_limit_max_pstv (+FLT16_MAX)
// #define f16_bits           (16)
// #define f16_bytes          (2)
// #define f16_eps            (FLT16_EPSILON)
// #define f16_nan            (as$(f16)(__builtin_nan("")))
// #define f16_inf            (as$(f16)(__builtin_inf()))
// #define f16_inf_ngtv       (-f16_inf)
// #define f16_inf_pstv       (+f16_inf)

typedef float f32;
#define f32_limit_min (FLT_MIN)
#define f32_limit_max (FLT_MAX)
#define f32_limit_max_ngtv (-FLT_MAX)
#define f32_limit_min_ngtv (-FLT_MIN)
#define f32_limit_min_pstv (+FLT_MIN)
#define f32_limit_max_pstv (+FLT_MAX)
#define f32_bits (32)
#define f32_bytes (4)
#define f32_eps (FLT_EPSILON)
#define f32_nan (as$(f32)(__builtin_nan("")))
#define f32_inf (as$(f32)(__builtin_inf()))
#define f32_inf_ngtv (-f32_inf)
#define f32_inf_pstv (+f32_inf)

typedef double f64;
#define f64_limit_min (FLT_MIN)
#define f64_limit_max (DBL_MAX)
#define f64_limit_max_ngtv (-DBL_MAX)
#define f64_limit_min_ngtv (-DBL_MIN)
#define f64_limit_min_pstv (+DBL_MIN)
#define f64_limit_max_pstv (+DBL_MAX)
#define f64_bits (64)
#define f64_bytes (8)
#define f64_eps (DBL_EPSILON)
#define f64_nan (as$(f64)(__builtin_nan("")))
#define f64_inf (as$(f64)(__builtin_inf()))
#define f64_inf_ngtv (-f64_inf)
#define f64_inf_pstv (+f64_inf)

// typedef long double f80;
// #define f80_limit_min      (LDBL_MIN)
// #define f80_limit_max      (LDBL_MAX)
// #define f80_limit_max_ngtv (-LDBL_MAX)
// #define f80_limit_min_ngtv (-LDBL_MIN)
// #define f80_limit_min_pstv (+LDBL_MIN)
// #define f80_limit_max_pstv (+LDBL_MAX)
// #define f80_bits           (80)
// #define f80_bytes          (10)
// #define f80_eps            (LDBL_EPSILON)
// #define f80_nan            (as$(f80)(__builtin_nan("")))
// #define f80_inf            (as$(f80)(__builtin_inf()))
// #define f80_inf_ngtv       (-f80_inf)
// #define f80_inf_pstv       (+f80_inf)

// typedef __float128 f128;
// #define f128_limit_min      (FLT128_MIN)
// #define f128_limit_max      (FLT128_MAX)
// #define f128_limit_max_ngtv (-FLT128_MAX)
// #define f128_limit_min_ngtv (-FLT128_MIN)
// #define f128_limit_min_pstv (+FLT128_MIN)
// #define f128_limit_max_pstv (+FLT128_MAX)
// #define f128_bits           (128)
// #define f128_bytes          (16)
// #define f128_eps            (FLT128_EPSILON)
// #define f128_nan            (as$(f128)(__builtin_nan("")))
// #define f128_inf            (as$(f128)(__builtin_inf()))
// #define f128_inf_ngtv       (-f128_inf)
// #define f128_inf_pstv       (+f128_inf)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_flt__included */
