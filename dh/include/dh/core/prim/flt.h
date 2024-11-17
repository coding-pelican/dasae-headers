/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    flts.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
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
// #define f16_LIMIT_MIN (FLT16_MIN)
// #define f16_LIMIT_MAX (FLT16_MAX)
// #define f16_EPS       (FLT16_EPSILON)

typedef float_t f32;
#define f32_LIMIT_MIN (FLT_MIN)
#define f32_LIMIT_MAX (FLT_MAX)
#define f32_EPS       (FLT_EPSILON)
#define f32_NAN       (NAN)
#define f32_INF       (INFINITY)

typedef double_t f64;
#define f64_LIMIT_MIN (FLT_MIN)
#define f64_LIMIT_MAX (DBL_MAX)
#define f64_EPS       (DBL_EPSILON)
#define f64_NAN       (NAN)
#define f64_INF       (INFINITY)

// typedef long double f80;
// #define f80_LIMIT_MIN (LDBL_MIN)
// #define f80_LIMIT_MAX (LDBL_MAX)
// #define f80_EPS       (LDBL_EPSILON)

// typedef __float128 f128;
// #define f128_LIMIT_MIN (FLT128_MIN)
// #define f128_LIMIT_MAX (FLT128_MAX)
// #define f128_EPS       (FLT128_EPSILON)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_FLTS_INCLUDED */
