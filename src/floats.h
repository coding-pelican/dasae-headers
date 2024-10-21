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


#ifndef FLOATS_H
#define FLOATS_H
#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)


#include "primitive_types.h"

bool f32_eq(f32 a, f32 b);
bool f32_neq(f32 a, f32 b);
bool f32_lt(f32 a, f32 b);
bool f32_gt(f32 a, f32 b);
bool f32_leq(f32 a, f32 b);
bool f32_geq(f32 a, f32 b);

f32 f32_min(f32 x, f32 y);
f32 f32_max(f32 x, f32 y);
f32 f32_clamp(f32 x, f32 low, f32 high);
f32 f32_clamp01(f32 x);
f32 f32_wrap(f32 x, f32 low, f32 high);
f32 f32_wrap01(f32 x);

#define f32_Epsilon (1e-6f)
#define f32_Nan     (NAN)
#define f32_Inf     (INFINITY)
#define f32_InfNeg  (-f32_Inf)


bool f64_eq(f64 a, f64 b);
bool f64_neq(f64 a, f64 b);
bool f64_lt(f64 a, f64 b);
bool f64_gt(f64 a, f64 b);
bool f64_leq(f64 a, f64 b);
bool f64_geq(f64 a, f64 b);

f64 f64_min(f64 x, f64 y);
f64 f64_max(f64 x, f64 y);
f64 f64_clamp(f64 x, f64 low, f64 high);
f64 f64_clamp01(f64 x);
f64 f64_wrap(f64 x, f64 low, f64 high);
f64 f64_wrap01(f64 x);

#define f64_Epsilon (1e-15)
#define f64_Nan     (NAN)
#define f64_Inf     (INFINITY)
#define f64_InfNeg  (-f64_Inf)


#ifndef FLOATS_IMPL
#  define FLOATS_IMPL

#  include <math.h>

bool f32_eq(f32 a, f32 b) { return fabsf(a - b) < f32_Epsilon; }
bool f32_neq(f32 a, f32 b) { return fabsf(a - b) >= f32_Epsilon; }
bool f32_lt(f32 a, f32 b) { return (b - a) > f32_Epsilon; }
bool f32_gt(f32 a, f32 b) { return (a - b) > f32_Epsilon; }
bool f32_leq(f32 a, f32 b) { return (a - b) < f32_Epsilon; }
bool f32_geq(f32 a, f32 b) { return (b - a) < f32_Epsilon; }

f32 f32_min(f32 x, f32 y) { return f32_lt(x, y) ? x : y; }
f32 f32_max(f32 x, f32 y) { return f32_gt(x, y) ? x : y; }
f32 f32_clamp(f32 x, f32 low, f32 high) { return f32_min(f32_max(low, x), high); }
f32 f32_clamp01(f32 x) { return f32_clamp(x, 0.0f, 1.0f); }
f32 f32_wrap(f32 x, f32 low, f32 high) { return (f32_leq(high - low, 0.0f)) ? x : fmodf(x - low, high - low) + low; }
f32 f32_wrap01(f32 x) { return f32_wrap(x, 0.0f, 1.0f); }

bool f64_eq(f64 a, f64 b) { return fabs(a - b) < f64_Epsilon; }
bool f64_neq(f64 a, f64 b) { return fabs(a - b) >= f64_Epsilon; }
bool f64_lt(f64 a, f64 b) { return (b - a) > f64_Epsilon; }
bool f64_gt(f64 a, f64 b) { return (a - b) > f64_Epsilon; }
bool f64_leq(f64 a, f64 b) { return (a - b) < f64_Epsilon; }
bool f64_geq(f64 a, f64 b) { return (b - a) < f64_Epsilon; }

f64 f64_min(f64 x, f64 y) { return f64_lt(x, y) ? x : y; }
f64 f64_max(f64 x, f64 y) { return f64_gt(x, y) ? x : y; }
f64 f64_clamp(f64 x, f64 low, f64 high) { return f64_min(f64_max(low, x), high); }
f64 f64_clamp01(f64 x) { return f64_clamp(x, 0.0, 1.0); }
f64 f64_wrap(f64 x, f64 low, f64 high) { return (f64_geq(high - low, 0.0)) ? x : fmod(x - low, high - low) + low; }
f64 f64_wrap01(f64 x) { return f64_wrap(x, 0.0, 1.0); }

#endif // FLOATS_IMPL


#if defined(__cplusplus)
}
#endif // defined(__cplusplus)
#endif // FLOATS_H
