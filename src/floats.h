/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    floats.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef FLOATS_INCLUDED
#define FLOATS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"
#include <math.h>


bool f32_eq(f32 a, f32 b);
bool f32_ne(f32 a, f32 b);
bool f32_lt(f32 a, f32 b);
bool f32_le(f32 a, f32 b);
bool f32_gt(f32 a, f32 b);
bool f32_ge(f32 a, f32 b);

f32 f32_min(f32 x, f32 y);
f32 f32_max(f32 x, f32 y);
f32 f32_clamp(f32 x, f32 low, f32 high);
f32 f32_clamp01(f32 x);
f32 f32_wrap(f32 x, f32 low, f32 high);
f32 f32_wrap01(f32 x);

#define f32_epsilon (1e-6f)
#define f32_nan     (NAN)
#define f32_inf     (INFINITY)


bool f64_eq(f64 a, f64 b);
bool f64_ne(f64 a, f64 b);
bool f64_lt(f64 a, f64 b);
bool f64_le(f64 a, f64 b);
bool f64_gt(f64 a, f64 b);
bool f64_ge(f64 a, f64 b);

f64 f64_min(f64 x, f64 y);
f64 f64_max(f64 x, f64 y);
f64 f64_clamp(f64 x, f64 low, f64 high);
f64 f64_clamp01(f64 x);
f64 f64_wrap(f64 x, f64 low, f64 high);
f64 f64_wrap01(f64 x);

#define f64_epsilon (1e-15)
#define f64_nan     (NAN)
#define f64_inf     (INFINITY)


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* FLOATS_INCLUDED */
