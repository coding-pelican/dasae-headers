#include "floats.h"


bool f32_eq(f32 a, f32 b) { return fabsf(a - b) < f32_epsilon; }
bool f32_ne(f32 a, f32 b) { return fabsf(a - b) >= f32_epsilon; }
bool f32_lt(f32 a, f32 b) { return (b - a) > f32_epsilon; }
bool f32_le(f32 a, f32 b) { return (a - b) < f32_epsilon; }
bool f32_gt(f32 a, f32 b) { return (a - b) > f32_epsilon; }
bool f32_ge(f32 a, f32 b) { return (b - a) < f32_epsilon; }

f32 f32_min(f32 x, f32 y) { return f32_lt(x, y) ? x : y; }
f32 f32_max(f32 x, f32 y) { return f32_gt(x, y) ? x : y; }
f32 f32_clamp(f32 x, f32 low, f32 high) { return f32_min(f32_max(low, x), high); }
f32 f32_clamp01(f32 x) { return f32_clamp(x, 0.0f, 1.0f); }
f32 f32_wrap(f32 x, f32 low, f32 high) { return (f32_le(high - low, 0.0f)) ? x : fmodf(x - low, high - low) + low; }
f32 f32_wrap01(f32 x) { return f32_wrap(x, 0.0f, 1.0f); }

bool f64_eq(f64 a, f64 b) { return fabs(a - b) < f64_epsilon; }
bool f64_ne(f64 a, f64 b) { return fabs(a - b) >= f64_epsilon; }
bool f64_lt(f64 a, f64 b) { return (b - a) > f64_epsilon; }
bool f64_le(f64 a, f64 b) { return (a - b) < f64_epsilon; }
bool f64_gt(f64 a, f64 b) { return (a - b) > f64_epsilon; }
bool f64_ge(f64 a, f64 b) { return (b - a) < f64_epsilon; }

f64 f64_min(f64 x, f64 y) { return f64_lt(x, y) ? x : y; }
f64 f64_max(f64 x, f64 y) { return f64_gt(x, y) ? x : y; }
f64 f64_clamp(f64 x, f64 low, f64 high) { return f64_min(f64_max(low, x), high); }
f64 f64_clamp01(f64 x) { return f64_clamp(x, 0.0, 1.0); }
f64 f64_wrap(f64 x, f64 low, f64 high) { return (f64_le(high - low, 0.0)) ? x : fmod(x - low, high - low) + low; }
f64 f64_wrap01(f64 x) { return f64_wrap(x, 0.0, 1.0); }
