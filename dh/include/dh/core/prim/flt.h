/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    flt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PRIM_FLTS_INCLUDED
#define CORE_PRIM_FLTS_INCLUDED (1)
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
// #define f16_eps      (FLT16_EPSILON)

typedef float_t f32;
#define f32_limit_min (FLT_MIN)
#define f32_limit_max (FLT_MAX)
#define f32_eps       (FLT_EPSILON)
#define f32_nan       (NAN)
#define f32_inf       (INFINITY)

typedef double_t f64;
#define f64_limit_min (FLT_MIN)
#define f64_limit_max (DBL_MAX)
#define f64_eps       (DBL_EPSILON)
#define f64_nan       (NAN)
#define f64_inf       (INFINITY)

// typedef long double f80;
// #define f80_limit_min (LDBL_MIN)
// #define f80_limit_max (LDBL_MAX)
// #define f80_eps      (LDBL_EPSILON)

// typedef __float128 f128;
// #define f128_limit_min (FLT128_MIN)
// #define f128_limit_max (FLT128_MAX)
// #define f128_eps      (FLT128_EPSILON)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_FLTS_INCLUDED */
