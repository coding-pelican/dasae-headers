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
 * @prefix  NONE
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
#include <math.h>

/*========== Macros and Definitions =========================================*/

/* floating point */
// typedef _Float16 f16;
// #define f16_limit_min (FLT16_MIN)
// #define f16_limit_max (FLT16_MAX)
// #define f16_eps       (FLT16_EPSILON)
// #define f16_nan       (NAN)
// #define f16_inf       (INFINITY)

typedef float_t f32;
#define f32_limit_min      (FLT_MIN)
#define f32_limit_max      (FLT_MAX)
#define f32_limit_max_ngtv (-FLT_MAX)
#define f32_limit_min_ngtv (-FLT_MIN)
#define f32_limit_min_pstv (+FLT_MIN)
#define f32_limit_max_pstv (+FLT_MAX)
#define f32_eps            (FLT_EPSILON)
#define f32_nan            (NAN)
#define f32_inf            (INFINITY)
#define f32_inf_ngtv       (-INFINITY)
#define f32_inf_pstv       (+INFINITY)

typedef double_t f64;
#define f64_limit_min      (FLT_MIN)
#define f64_limit_max      (DBL_MAX)
#define f64_limit_max_ngtv (-DBL_MAX)
#define f64_limit_min_ngtv (-DBL_MIN)
#define f64_limit_min_pstv (+DBL_MIN)
#define f64_limit_max_pstv (+DBL_MAX)
#define f64_eps            (DBL_EPSILON)
#define f64_nan            (NAN)
#define f64_inf            (INFINITY)
#define f64_inf_ngtv       (-INFINITY)
#define f64_inf_pstv       (+INFINITY)

// typedef long double f80;
// #define f80_limit_min (LDBL_MIN)
// #define f80_limit_max (LDBL_MAX)
// #define f80_eps       (LDBL_EPSILON)
// #define f80_nan       (NAN)
// #define f80_inf       (INFINITY)

// typedef __float128 f128;
// #define f128_limit_min (FLT128_MIN)
// #define f128_limit_max (FLT128_MAX)
// #define f128_eps       (FLT128_EPSILON)
// #define f128_nan       (NAN)
// #define f128_inf       (INFINITY)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_flt__included */
