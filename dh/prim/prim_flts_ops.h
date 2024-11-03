/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    flts.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#include "../compat/compat_inline.h"
#include "prim.h"

#include <math.h>


force_inline bool f32_eq(f32 x, f32 y) { return fabsf(x - y) < f32_eps; }
force_inline bool f32_ne(f32 x, f32 y) { return fabsf(x - y) >= f32_eps; }
force_inline bool f32_lt(f32 x, f32 y) { return (y - x) > f32_eps; }
force_inline bool f32_le(f32 x, f32 y) { return (x - y) < f32_eps; }
force_inline bool f32_gt(f32 x, f32 y) { return (x - y) > f32_eps; }
force_inline bool f32_ge(f32 x, f32 y) { return (y - x) < f32_eps; }

force_inline f32 f32_neg(f32 x) { return -x; }
force_inline f32 f32_abs(f32 x) { return fabsf(x); }

force_inline f32 f32_add(f32 x, f32 y) { return x + y; }
force_inline f32 f32_sub(f32 x, f32 y) { return x - y; }
force_inline f32 f32_mul(f32 x, f32 y) { return x * y; }
force_inline f32 f32_div(f32 x, f32 y) { return x / y; }
force_inline f32 f32_mod(f32 x, f32 y) { return fmodf(x, y); }

force_inline f32 f32_min(f32 x, f32 y) { return f32_lt(x, y) ? x : y; }
force_inline f32 f32_max(f32 x, f32 y) { return f32_gt(x, y) ? x : y; }
force_inline f32 f32_clamp(f32 x, f32 low, f32 high) { return f32_min(f32_max(low, x), high); }
force_inline f32 f32_clamp01(f32 x) { return f32_clamp(x, 0.0f, 1.0f); }
force_inline f32 f32_wrap(f32 x, f32 low, f32 high) { return (f32_le(high - low, 0.0f)) ? x : fmodf(x - low, high - low) + low; }
force_inline f32 f32_wrap01(f32 x) { return f32_wrap(x, 0.0f, 1.0f); }

force_inline bool f64_eq(f64 x, f64 y) { return fabs(x - y) < f64_eps; }
force_inline bool f64_ne(f64 x, f64 y) { return fabs(x - y) >= f64_eps; }
force_inline bool f64_lt(f64 x, f64 y) { return (y - x) > f64_eps; }
force_inline bool f64_le(f64 x, f64 y) { return (x - y) < f64_eps; }
force_inline bool f64_gt(f64 x, f64 y) { return (x - y) > f64_eps; }
force_inline bool f64_ge(f64 x, f64 y) { return (y - x) < f64_eps; }

force_inline f64 f64_neg(f64 x) { return -x; }
force_inline f64 f64_abs(f64 x) { return fabs(x); }

force_inline f64 f64_add(f64 x, f64 y) { return x + y; }
force_inline f64 f64_sub(f64 x, f64 y) { return x - y; }
force_inline f64 f64_mul(f64 x, f64 y) { return x * y; }
force_inline f64 f64_div(f64 x, f64 y) { return x / y; }
force_inline f64 f64_mod(f64 x, f64 y) { return fmod(x, y); }

force_inline f64 f64_min(f64 x, f64 y) { return f64_lt(x, y) ? x : y; }
force_inline f64 f64_max(f64 x, f64 y) { return f64_gt(x, y) ? x : y; }
force_inline f64 f64_clamp(f64 x, f64 low, f64 high) { return f64_min(f64_max(low, x), high); }
force_inline f64 f64_clamp01(f64 x) { return f64_clamp(x, 0.0, 1.0); }
force_inline f64 f64_wrap(f64 x, f64 low, f64 high) { return (f64_le(high - low, 0.0)) ? x : fmod(x - low, high - low) + low; }
force_inline f64 f64_wrap01(f64 x) { return f64_wrap(x, 0.0, 1.0); }
