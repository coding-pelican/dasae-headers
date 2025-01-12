/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-01-12 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/math
 * @prefix  math
 *
 * @brief   Mathematical common vector operations
 * @details Provides a set of vector mathematical operations and functions
 */

#ifndef MATH_VEC_INCLUDED
#define MATH_VEC_INCLUDED (1)

#include "common.h"
#include "vec_types.h"

/*========== Float32 (f32) Vector Functions ================================*/

/* Vec2f functions */
/* Constants */
static const math_Vec2f math_Vec2f_zero  = { .x = 0.0f, .y = 0.0f };
static const math_Vec2f math_Vec2f_one   = { .x = 1.0f, .y = 1.0f };
static const math_Vec2f math_Vec2f_left  = { .x = -1.0f, .y = 0.0f };
static const math_Vec2f math_Vec2f_up    = { .x = 0.0f, .y = 1.0f };
static const math_Vec2f math_Vec2f_right = { .x = 1.0f, .y = 0.0f };
static const math_Vec2f math_Vec2f_down  = { .x = 0.0f, .y = -1.0f };
static const math_Vec2f math_Vec2f_eps   = { .x = f32_eps, .y = f32_eps };
static const math_Vec2f math_Vec2f_inf   = { .x = f32_inf, .y = f32_inf };
static const math_Vec2f math_Vec2f_nan   = { .x = f32_nan, .y = f32_nan };

/* Construction */
force_inline math_Vec2f math_Vec2f_create(f32 x, f32 y);
force_inline math_Vec2f math_Vec2f_fill(f32 scalar);
force_inline math_Vec2f math_Vec2f_from3(math_Vec3f v);
force_inline math_Vec2f math_Vec2f_from4(math_Vec4f v);
force_inline math_Vec2f math_Vec2f_sincos(f32 radians);

/* Comparison */
force_inline cmp_fnCmp(math_Vec2f);
cmp_fnEq_default(math_Vec2f);
cmp_fnNe_default(math_Vec2f);
cmp_fnLt_default(math_Vec2f);
cmp_fnGt_default(math_Vec2f);
cmp_fnLe_default(math_Vec2f);
cmp_fnGe_default(math_Vec2f);

/* Arithmetic */
force_inline math_Vec2f math_Vec2f_neg(math_Vec2f v);
force_inline math_Vec2f math_Vec2f_add(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_sub(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_mul(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_div(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_mod(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_scale(math_Vec2f v, f32 scalar);

/* Range Operations */
force_inline math_Vec2f math_Vec2f_min(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_max(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_clamp(math_Vec2f v, math_Vec2f min, math_Vec2f max);
force_inline math_Vec2f math_Vec2f_clamp01(math_Vec2f v);
force_inline math_Vec2f math_Vec2f_wrap(math_Vec2f v, math_Vec2f min, math_Vec2f max);
force_inline math_Vec2f math_Vec2f_wrap01(math_Vec2f v);

/* Geometric Operations */
force_inline f32        math_Vec2f_lenSq(math_Vec2f v);
force_inline f32        math_Vec2f_len(math_Vec2f v);
force_inline f32        math_Vec2f_distSq(math_Vec2f lhs, math_Vec2f rhs);
force_inline f32        math_Vec2f_dist(math_Vec2f lhs, math_Vec2f rhs);
force_inline f32        math_Vec2f_dot(math_Vec2f lhs, math_Vec2f rhs);
force_inline math_Vec2f math_Vec2f_norm(math_Vec2f v);
force_inline math_Vec2f math_Vec2f_project(math_Vec2f v, math_Vec2f onto);
force_inline math_Vec2f math_Vec2f_reject(math_Vec2f v, math_Vec2f from);
force_inline math_Vec2f math_Vec2f_reflect(math_Vec2f v, math_Vec2f normal);
force_inline math_Vec2f math_Vec2f_rotate(math_Vec2f v, f32 angle);
force_inline math_Vec2f math_Vec2f_perp(math_Vec2f v);

/* Vec3f functions */
/* Constants */
static const math_Vec3f math_Vec3f_zero     = { .x = 0.0f, .y = 0.0f, .z = 0.0f };
static const math_Vec3f math_Vec3f_one      = { .x = 1.0f, .y = 1.0f, .z = 1.0f };
static const math_Vec3f math_Vec3f_left     = { .x = -1.0f, .y = 0.0f, .z = 0.0f };
static const math_Vec3f math_Vec3f_up       = { .x = 0.0f, .y = 1.0f, .z = 0.0f };
static const math_Vec3f math_Vec3f_right    = { .x = 1.0f, .y = 0.0f, .z = 0.0f };
static const math_Vec3f math_Vec3f_down     = { .x = 0.0f, .y = -1.0f, .z = 0.0f };
static const math_Vec3f math_Vec3f_forward  = { .x = 0.0f, .y = 0.0f, .z = 1.0f };
static const math_Vec3f math_Vec3f_backward = { .x = 0.0f, .y = 0.0f, .z = -1.0f };
static const math_Vec3f math_Vec3f_eps      = { .x = f32_eps, .y = f32_eps, .z = f32_eps };
static const math_Vec3f math_Vec3f_inf      = { .x = f32_inf, .y = f32_inf, .z = f32_inf };
static const math_Vec3f math_Vec3f_nan      = { .x = f32_nan, .y = f32_nan, .z = f32_nan };

/* Construction */
force_inline math_Vec3f math_Vec3f_create(f32 x, f32 y, f32 z);
force_inline math_Vec3f math_Vec3f_fill(f32 scalar);
force_inline math_Vec3f math_Vec3f_from2(math_Vec2f v);
force_inline math_Vec3f math_Vec3f_from4(math_Vec4f v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec3f);
cmp_fnEq_default(math_Vec3f);
cmp_fnNe_default(math_Vec3f);
cmp_fnLt_default(math_Vec3f);
cmp_fnGt_default(math_Vec3f);
cmp_fnLe_default(math_Vec3f);
cmp_fnGe_default(math_Vec3f);

/* Arithmetic */
force_inline math_Vec3f math_Vec3f_neg(math_Vec3f v);
force_inline math_Vec3f math_Vec3f_add(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_sub(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_mul(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_div(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_mod(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_scale(math_Vec3f v, f32 scalar);

/* Range Operations */
force_inline math_Vec3f math_Vec3f_min(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_max(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_clamp(math_Vec3f v, math_Vec3f min, math_Vec3f max);
force_inline math_Vec3f math_Vec3f_clamp01(math_Vec3f v);
force_inline math_Vec3f math_Vec3f_wrap(math_Vec3f v, math_Vec3f min, math_Vec3f max);
force_inline math_Vec3f math_Vec3f_wrap01(math_Vec3f v);

/* Geometric Operations */
force_inline f32        math_Vec3f_lenSq(math_Vec3f v);
force_inline f32        math_Vec3f_len(math_Vec3f v);
force_inline f32        math_Vec3f_distSq(math_Vec3f lhs, math_Vec3f rhs);
force_inline f32        math_Vec3f_dist(math_Vec3f lhs, math_Vec3f rhs);
force_inline f32        math_Vec3f_dot(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_cross(math_Vec3f lhs, math_Vec3f rhs);
force_inline math_Vec3f math_Vec3f_norm(math_Vec3f v);
force_inline math_Vec3f math_Vec3f_project(math_Vec3f v, math_Vec3f onto);
force_inline math_Vec3f math_Vec3f_reject(math_Vec3f v, math_Vec3f from);
force_inline math_Vec3f math_Vec3f_reflect(math_Vec3f v, math_Vec3f normal);
force_inline math_Vec3f math_Vec3f_rotate(math_Vec3f v, math_Vec3f axis, f32 angle);
force_inline math_Vec3f math_Vec3f_perp(math_Vec3f v);

/* Vec4f functions */
/* Constants */
static const math_Vec4f math_Vec4f_zero     = { .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_one      = { .x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f };
static const math_Vec4f math_Vec4f_left     = { .x = -1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_up       = { .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_right    = { .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_down     = { .x = 0.0f, .y = -1.0f, .z = 0.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_forward  = { .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_backward = { .x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 0.0f };
static const math_Vec4f math_Vec4f_eps      = { .x = f32_eps, .y = f32_eps, .z = f32_eps, .w = f32_eps };
static const math_Vec4f math_Vec4f_inf      = { .x = f32_inf, .y = f32_inf, .z = f32_inf, .w = f32_inf };
static const math_Vec4f math_Vec4f_nan      = { .x = f32_nan, .y = f32_nan, .z = f32_nan, .w = f32_nan };

/* Construction */
force_inline math_Vec4f math_Vec4f_create(f32 x, f32 y, f32 z, f32 w);
force_inline math_Vec4f math_Vec4f_fill(f32 scalar);
force_inline math_Vec4f math_Vec4f_from2(math_Vec2f v);
force_inline math_Vec4f math_Vec4f_from3(math_Vec3f v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec4f);
cmp_fnEq_default(math_Vec4f);
cmp_fnNe_default(math_Vec4f);
cmp_fnLt_default(math_Vec4f);
cmp_fnGt_default(math_Vec4f);
cmp_fnLe_default(math_Vec4f);
cmp_fnGe_default(math_Vec4f);

/* Arithmetic */
force_inline math_Vec4f math_Vec4f_neg(math_Vec4f v);
force_inline math_Vec4f math_Vec4f_add(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_sub(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_mul(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_div(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_mod(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_scale(math_Vec4f v, f32 scalar);

/* Range Operations */
force_inline math_Vec4f math_Vec4f_min(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_max(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_clamp(math_Vec4f v, math_Vec4f min, math_Vec4f max);
force_inline math_Vec4f math_Vec4f_clamp01(math_Vec4f v);
force_inline math_Vec4f math_Vec4f_wrap(math_Vec4f v, math_Vec4f min, math_Vec4f max);
force_inline math_Vec4f math_Vec4f_wrap01(math_Vec4f v);

/* Geometric Operations */
force_inline f32        math_Vec4f_lenSq(math_Vec4f v);
force_inline f32        math_Vec4f_len(math_Vec4f v);
force_inline f32        math_Vec4f_distSq(math_Vec4f lhs, math_Vec4f rhs);
force_inline f32        math_Vec4f_dist(math_Vec4f lhs, math_Vec4f rhs);
force_inline f32        math_Vec4f_dot(math_Vec4f lhs, math_Vec4f rhs);
force_inline math_Vec4f math_Vec4f_norm(math_Vec4f v);
force_inline math_Vec4f math_Vec4f_project(math_Vec4f v, math_Vec4f onto);
force_inline math_Vec4f math_Vec4f_reject(math_Vec4f v, math_Vec4f from);
force_inline math_Vec4f math_Vec4f_reflect(math_Vec4f v, math_Vec4f normal);

/*========== Float64 (f64) Vector Functions ================================*/

/* Vec2d functions */
/* Constants */
static const math_Vec2d math_Vec2d_zero  = { .x = 0.0, .y = 0.0 };
static const math_Vec2d math_Vec2d_one   = { .x = 1.0, .y = 1.0 };
static const math_Vec2d math_Vec2d_left  = { .x = -1.0, .y = 0.0 };
static const math_Vec2d math_Vec2d_up    = { .x = 0.0, .y = 1.0 };
static const math_Vec2d math_Vec2d_right = { .x = 1.0, .y = 0.0 };
static const math_Vec2d math_Vec2d_down  = { .x = 0.0, .y = -1.0 };
static const math_Vec2d math_Vec2d_eps   = { .x = f64_eps, .y = f64_eps };
static const math_Vec2d math_Vec2d_inf   = { .x = f64_inf, .y = f64_inf };
static const math_Vec2d math_Vec2d_nan   = { .x = f64_nan, .y = f64_nan };

/* Construction */
force_inline math_Vec2d math_Vec2d_create(f64 x, f64 y);
force_inline math_Vec2d math_Vec2d_fill(f64 scalar);
force_inline math_Vec2d math_Vec2d_from3(math_Vec3d v);
force_inline math_Vec2d math_Vec2d_from4(math_Vec4d v);
force_inline math_Vec2d math_Vec2d_sincos(f64 radians);

/* Comparison */
force_inline cmp_fnCmp(math_Vec2d);
cmp_fnEq_default(math_Vec2d);
cmp_fnNe_default(math_Vec2d);
cmp_fnLt_default(math_Vec2d);
cmp_fnGt_default(math_Vec2d);
cmp_fnLe_default(math_Vec2d);
cmp_fnGe_default(math_Vec2d);

/* Arithmetic */
force_inline math_Vec2d math_Vec2d_neg(math_Vec2d v);
force_inline math_Vec2d math_Vec2d_add(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_sub(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_mul(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_div(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_mod(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_scale(math_Vec2d v, f64 scalar);

/* Range Operations */
force_inline math_Vec2d math_Vec2d_min(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_max(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_clamp(math_Vec2d v, math_Vec2d min, math_Vec2d max);
force_inline math_Vec2d math_Vec2d_clamp01(math_Vec2d v);
force_inline math_Vec2d math_Vec2d_wrap(math_Vec2d v, math_Vec2d min, math_Vec2d max);
force_inline math_Vec2d math_Vec2d_wrap01(math_Vec2d v);

/* Geometric Operations */
force_inline f64        math_Vec2d_lenSq(math_Vec2d v);
force_inline f64        math_Vec2d_len(math_Vec2d v);
force_inline f64        math_Vec2d_distSq(math_Vec2d lhs, math_Vec2d rhs);
force_inline f64        math_Vec2d_dist(math_Vec2d lhs, math_Vec2d rhs);
force_inline f64        math_Vec2d_dot(math_Vec2d lhs, math_Vec2d rhs);
force_inline math_Vec2d math_Vec2d_norm(math_Vec2d v);
force_inline math_Vec2d math_Vec2d_project(math_Vec2d v, math_Vec2d onto);
force_inline math_Vec2d math_Vec2d_reject(math_Vec2d v, math_Vec2d from);
force_inline math_Vec2d math_Vec2d_reflect(math_Vec2d v, math_Vec2d normal);
force_inline math_Vec2d math_Vec2d_rotate(math_Vec2d v, f64 angle);
force_inline math_Vec2d math_Vec2d_perp(math_Vec2d v);

/* Vec3d functions */
/* Constants */
static const math_Vec3d math_Vec3d_zero     = { .x = 0.0, .y = 0.0, .z = 0.0 };
static const math_Vec3d math_Vec3d_one      = { .x = 1.0, .y = 1.0, .z = 1.0 };
static const math_Vec3d math_Vec3d_left     = { .x = -1.0, .y = 0.0, .z = 0.0 };
static const math_Vec3d math_Vec3d_up       = { .x = 0.0, .y = 1.0, .z = 0.0 };
static const math_Vec3d math_Vec3d_right    = { .x = 1.0, .y = 0.0, .z = 0.0 };
static const math_Vec3d math_Vec3d_down     = { .x = 0.0, .y = -1.0, .z = 0.0 };
static const math_Vec3d math_Vec3d_forward  = { .x = 0.0, .y = 0.0, .z = 1.0 };
static const math_Vec3d math_Vec3d_backward = { .x = 0.0, .y = 0.0, .z = -1.0 };
static const math_Vec3d math_Vec3d_eps      = { .x = f64_eps, .y = f64_eps, .z = f64_eps };
static const math_Vec3d math_Vec3d_inf      = { .x = f64_inf, .y = f64_inf, .z = f64_inf };
static const math_Vec3d math_Vec3d_nan      = { .x = f64_nan, .y = f64_nan, .z = f64_nan };

/* Construction */
force_inline math_Vec3d math_Vec3d_create(f64 x, f64 y, f64 z);
force_inline math_Vec3d math_Vec3d_fill(f64 scalar);
force_inline math_Vec3d math_Vec3d_from2(math_Vec2d v);
force_inline math_Vec3d math_Vec3d_from4(math_Vec4d v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec3d);
cmp_fnEq_default(math_Vec3d);
cmp_fnNe_default(math_Vec3d);
cmp_fnLt_default(math_Vec3d);
cmp_fnGt_default(math_Vec3d);
cmp_fnLe_default(math_Vec3d);
cmp_fnGe_default(math_Vec3d);

/* Arithmetic */
force_inline math_Vec3d math_Vec3d_neg(math_Vec3d v);
force_inline math_Vec3d math_Vec3d_add(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_sub(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_mul(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_div(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_mod(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_scale(math_Vec3d v, f64 scalar);

/* Range Operations */
force_inline math_Vec3d math_Vec3d_min(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_max(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_clamp(math_Vec3d v, math_Vec3d min, math_Vec3d max);
force_inline math_Vec3d math_Vec3d_clamp01(math_Vec3d v);
force_inline math_Vec3d math_Vec3d_wrap(math_Vec3d v, math_Vec3d min, math_Vec3d max);
force_inline math_Vec3d math_Vec3d_wrap01(math_Vec3d v);

/* Geometric Operations */
force_inline f64        math_Vec3d_lenSq(math_Vec3d v);
force_inline f64        math_Vec3d_len(math_Vec3d v);
force_inline f64        math_Vec3d_distSq(math_Vec3d lhs, math_Vec3d rhs);
force_inline f64        math_Vec3d_dist(math_Vec3d lhs, math_Vec3d rhs);
force_inline f64        math_Vec3d_dot(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_cross(math_Vec3d lhs, math_Vec3d rhs);
force_inline math_Vec3d math_Vec3d_norm(math_Vec3d v);
force_inline math_Vec3d math_Vec3d_project(math_Vec3d v, math_Vec3d onto);
force_inline math_Vec3d math_Vec3d_reject(math_Vec3d v, math_Vec3d from);
force_inline math_Vec3d math_Vec3d_reflect(math_Vec3d v, math_Vec3d normal);
force_inline math_Vec3d math_Vec3d_rotate(math_Vec3d v, math_Vec3d axis, f64 angle);
force_inline math_Vec3d math_Vec3d_perp(math_Vec3d v);

/* Vec4d functions */
/* Constants */
static const math_Vec4d math_Vec4d_zero     = { .x = 0.0, .y = 0.0, .z = 0.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_one      = { .x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0 };
static const math_Vec4d math_Vec4d_left     = { .x = -1.0, .y = 0.0, .z = 0.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_up       = { .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_right    = { .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_down     = { .x = 0.0, .y = -1.0, .z = 0.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_forward  = { .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_backward = { .x = 0.0, .y = 0.0, .z = -1.0, .w = 0.0 };
static const math_Vec4d math_Vec4d_eps      = { .x = f64_eps, .y = f64_eps, .z = f64_eps, .w = f64_eps };
static const math_Vec4d math_Vec4d_inf      = { .x = f64_inf, .y = f64_inf, .z = f64_inf, .w = f64_inf };
static const math_Vec4d math_Vec4d_nan      = { .x = f64_nan, .y = f64_nan, .z = f64_nan, .w = f64_nan };

/* Construction */
force_inline math_Vec4d math_Vec4d_create(f64 x, f64 y, f64 z, f64 w);
force_inline math_Vec4d math_Vec4d_fill(f64 scalar);
force_inline math_Vec4d math_Vec4d_from2(math_Vec2d v);
force_inline math_Vec4d math_Vec4d_from3(math_Vec3d v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec4d);
cmp_fnEq_default(math_Vec4d);
cmp_fnNe_default(math_Vec4d);
cmp_fnLt_default(math_Vec4d);
cmp_fnGt_default(math_Vec4d);
cmp_fnLe_default(math_Vec4d);
cmp_fnGe_default(math_Vec4d);

/* Arithmetic */
force_inline math_Vec4d math_Vec4d_neg(math_Vec4d v);
force_inline math_Vec4d math_Vec4d_add(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_sub(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_mul(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_div(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_mod(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_scale(math_Vec4d v, f64 scalar);

/* Range Operations */
force_inline math_Vec4d math_Vec4d_min(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_max(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_clamp(math_Vec4d v, math_Vec4d min, math_Vec4d max);
force_inline math_Vec4d math_Vec4d_clamp01(math_Vec4d v);
force_inline math_Vec4d math_Vec4d_wrap(math_Vec4d v, math_Vec4d min, math_Vec4d max);
force_inline math_Vec4d math_Vec4d_wrap01(math_Vec4d v);

/* Geometric Operations */
force_inline f64        math_Vec4d_lenSq(math_Vec4d v);
force_inline f64        math_Vec4d_len(math_Vec4d v);
force_inline f64        math_Vec4d_distSq(math_Vec4d lhs, math_Vec4d rhs);
force_inline f64        math_Vec4d_dist(math_Vec4d lhs, math_Vec4d rhs);
force_inline f64        math_Vec4d_dot(math_Vec4d lhs, math_Vec4d rhs);
force_inline math_Vec4d math_Vec4d_norm(math_Vec4d v);
force_inline math_Vec4d math_Vec4d_project(math_Vec4d v, math_Vec4d onto);
force_inline math_Vec4d math_Vec4d_reject(math_Vec4d v, math_Vec4d from);
force_inline math_Vec4d math_Vec4d_reflect(math_Vec4d v, math_Vec4d normal);

/*========== Int32 (i32) Vector Functions ================================*/

/* Vec2i functions */
/* Constants */
static const math_Vec2i math_Vec2i_zero      = { .x = 0, .y = 0 };
static const math_Vec2i math_Vec2i_one       = { .x = 1, .y = 1 };
static const math_Vec2i math_Vec2i_left      = { .x = -1, .y = 0 };
static const math_Vec2i math_Vec2i_up        = { .x = 0, .y = 1 };
static const math_Vec2i math_Vec2i_right     = { .x = 1, .y = 0 };
static const math_Vec2i math_Vec2i_down      = { .x = 0, .y = -1 };
static const math_Vec2i math_Vec2i_limit_min = { .x = i32_limit_min, .y = i32_limit_min };
static const math_Vec2i math_Vec2i_limit_max = { .x = i32_limit_max, .y = i32_limit_max };

/* Construction */
force_inline math_Vec2i math_Vec2i_create(i32 x, i32 y);
force_inline math_Vec2i math_Vec2i_fill(i32 scalar);
force_inline math_Vec2i math_Vec2i_from3(math_Vec3i v);
force_inline math_Vec2i math_Vec2i_from4(math_Vec4i v);
force_inline math_Vec2i math_Vec2i_sincos(i32 radians);

/* Comparison */
force_inline cmp_fnCmp(math_Vec2i);
cmp_fnEq_default(math_Vec2i);
cmp_fnNe_default(math_Vec2i);
cmp_fnLt_default(math_Vec2i);
cmp_fnGt_default(math_Vec2i);
cmp_fnLe_default(math_Vec2i);
cmp_fnGe_default(math_Vec2i);

/* Arithmetic */
force_inline math_Vec2i math_Vec2i_neg(math_Vec2i v);
force_inline math_Vec2i math_Vec2i_add(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_sub(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_mul(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_div(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_mod(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_scale(math_Vec2i v, i32 scalar);

/* Range Operations */
force_inline math_Vec2i math_Vec2i_min(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_max(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_clamp(math_Vec2i v, math_Vec2i min, math_Vec2i max);
force_inline math_Vec2i math_Vec2i_wrap(math_Vec2i v, math_Vec2i min, math_Vec2i max);

/* Geometric Operations */
force_inline i32        math_Vec2i_lenSq(math_Vec2i v);
force_inline i32        math_Vec2i_len(math_Vec2i v);
force_inline i32        math_Vec2i_distSq(math_Vec2i lhs, math_Vec2i rhs);
force_inline i32        math_Vec2i_dist(math_Vec2i lhs, math_Vec2i rhs);
force_inline i32        math_Vec2i_dot(math_Vec2i lhs, math_Vec2i rhs);
force_inline math_Vec2i math_Vec2i_norm(math_Vec2i v);
force_inline math_Vec2i math_Vec2i_project(math_Vec2i v, math_Vec2i onto);
force_inline math_Vec2i math_Vec2i_reject(math_Vec2i v, math_Vec2i from);
force_inline math_Vec2i math_Vec2i_reflect(math_Vec2i v, math_Vec2i normal);
force_inline math_Vec2i math_Vec2i_rotate(math_Vec2i v, i32 angle);
force_inline math_Vec2i math_Vec2i_perp(math_Vec2i v);

/* Vec3i functions */
/* Constants */
static const math_Vec3i math_Vec3i_zero      = { .x = 0, .y = 0, .z = 0 };
static const math_Vec3i math_Vec3i_one       = { .x = 1, .y = 1, .z = 1 };
static const math_Vec3i math_Vec3i_left      = { .x = -1, .y = 0, .z = 0 };
static const math_Vec3i math_Vec3i_up        = { .x = 0, .y = 1, .z = 0 };
static const math_Vec3i math_Vec3i_right     = { .x = 1, .y = 0, .z = 0 };
static const math_Vec3i math_Vec3i_down      = { .x = 0, .y = -1, .z = 0 };
static const math_Vec3i math_Vec3i_forward   = { .x = 0, .y = 0, .z = 1 };
static const math_Vec3i math_Vec3i_backward  = { .x = 0, .y = 0, .z = -1 };
static const math_Vec3i math_Vec3i_limit_min = { .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min };
static const math_Vec3i math_Vec3i_limit_max = { .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max };

/* Construction */
force_inline math_Vec3i math_Vec3i_create(i32 x, i32 y, i32 z);
force_inline math_Vec3i math_Vec3i_fill(i32 scalar);
force_inline math_Vec3i math_Vec3i_from2(math_Vec2i v);
force_inline math_Vec3i math_Vec3i_from4(math_Vec4i v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec3i);
cmp_fnEq_default(math_Vec3i);
cmp_fnNe_default(math_Vec3i);
cmp_fnLt_default(math_Vec3i);
cmp_fnGt_default(math_Vec3i);
cmp_fnLe_default(math_Vec3i);
cmp_fnGe_default(math_Vec3i);

/* Arithmetic */
force_inline math_Vec3i math_Vec3i_neg(math_Vec3i v);
force_inline math_Vec3i math_Vec3i_add(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_sub(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_mul(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_div(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_mod(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_scale(math_Vec3i v, i32 scalar);

/* Range Operations */
force_inline math_Vec3i math_Vec3i_min(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_max(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_clamp(math_Vec3i v, math_Vec3i min, math_Vec3i max);
force_inline math_Vec3i math_Vec3i_wrap(math_Vec3i v, math_Vec3i min, math_Vec3i max);

/* Geometric Operations */
force_inline i32        math_Vec3i_lenSq(math_Vec3i v);
force_inline i32        math_Vec3i_len(math_Vec3i v);
force_inline i32        math_Vec3i_distSq(math_Vec3i lhs, math_Vec3i rhs);
force_inline i32        math_Vec3i_dist(math_Vec3i lhs, math_Vec3i rhs);
force_inline i32        math_Vec3i_dot(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_cross(math_Vec3i lhs, math_Vec3i rhs);
force_inline math_Vec3i math_Vec3i_norm(math_Vec3i v);
force_inline math_Vec3i math_Vec3i_project(math_Vec3i v, math_Vec3i onto);
force_inline math_Vec3i math_Vec3i_reject(math_Vec3i v, math_Vec3i from);
force_inline math_Vec3i math_Vec3i_reflect(math_Vec3i v, math_Vec3i normal);
force_inline math_Vec3i math_Vec3i_rotate(math_Vec3i v, math_Vec3i axis, i32 angle);
force_inline math_Vec3i math_Vec3i_perp(math_Vec3i v);

/* Vec4i functions */
/* Constants */
static const math_Vec4i math_Vec4i_zero      = { .x = 0, .y = 0, .z = 0, .w = 0 };
static const math_Vec4i math_Vec4i_one       = { .x = 1, .y = 1, .z = 1, .w = 1 };
static const math_Vec4i math_Vec4i_left      = { .x = -1, .y = 0, .z = 0, .w = 0 };
static const math_Vec4i math_Vec4i_up        = { .x = 0, .y = 1, .z = 0, .w = 0 };
static const math_Vec4i math_Vec4i_right     = { .x = 1, .y = 0, .z = 0, .w = 0 };
static const math_Vec4i math_Vec4i_down      = { .x = 0, .y = -1, .z = 0, .w = 0 };
static const math_Vec4i math_Vec4i_forward   = { .x = 0, .y = 0, .z = 1, .w = 0 };
static const math_Vec4i math_Vec4i_backward  = { .x = 0, .y = 0, .z = -1, .w = 0 };
static const math_Vec4i math_Vec4i_limit_min = { .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min, .w = i32_limit_min };
static const math_Vec4i math_Vec4i_limit_max = { .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max, .w = i32_limit_max };

/* Construction */
force_inline math_Vec4i math_Vec4i_create(i32 x, i32 y, i32 z, i32 w);
force_inline math_Vec4i math_Vec4i_fill(i32 scalar);
force_inline math_Vec4i math_Vec4i_from2(math_Vec2i v);
force_inline math_Vec4i math_Vec4i_from3(math_Vec3i v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec4i);
cmp_fnEq_default(math_Vec4i);
cmp_fnNe_default(math_Vec4i);
cmp_fnLt_default(math_Vec4i);
cmp_fnGt_default(math_Vec4i);
cmp_fnLe_default(math_Vec4i);
cmp_fnGe_default(math_Vec4i);

/* Arithmetic */
force_inline math_Vec4i math_Vec4i_neg(math_Vec4i v);
force_inline math_Vec4i math_Vec4i_add(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_sub(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_mul(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_div(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_mod(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_scale(math_Vec4i v, i32 scalar);

/* Range Operations */
force_inline math_Vec4i math_Vec4i_min(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_max(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_clamp(math_Vec4i v, math_Vec4i min, math_Vec4i max);
force_inline math_Vec4i math_Vec4i_wrap(math_Vec4i v, math_Vec4i min, math_Vec4i max);

/* Geometric Operations */
force_inline i32        math_Vec4i_lenSq(math_Vec4i v);
force_inline i32        math_Vec4i_len(math_Vec4i v);
force_inline i32        math_Vec4i_distSq(math_Vec4i lhs, math_Vec4i rhs);
force_inline i32        math_Vec4i_dist(math_Vec4i lhs, math_Vec4i rhs);
force_inline i32        math_Vec4i_dot(math_Vec4i lhs, math_Vec4i rhs);
force_inline math_Vec4i math_Vec4i_norm(math_Vec4i v);
force_inline math_Vec4i math_Vec4i_project(math_Vec4i v, math_Vec4i onto);
force_inline math_Vec4i math_Vec4i_reject(math_Vec4i v, math_Vec4i from);
force_inline math_Vec4i math_Vec4i_reflect(math_Vec4i v, math_Vec4i normal);

/*========== Int64 (i64) Vector Functions ================================*/

/* Vec2l functions */
/* Constants */
static const math_Vec2l math_Vec2l_zero      = { .x = 0, .y = 0 };
static const math_Vec2l math_Vec2l_one       = { .x = 1, .y = 1 };
static const math_Vec2l math_Vec2l_left      = { .x = -1, .y = 0 };
static const math_Vec2l math_Vec2l_up        = { .x = 0, .y = 1 };
static const math_Vec2l math_Vec2l_right     = { .x = 1, .y = 0 };
static const math_Vec2l math_Vec2l_down      = { .x = 0, .y = -1 };
static const math_Vec2l math_Vec2l_limit_min = { .x = i64_limit_min, .y = i64_limit_min };
static const math_Vec2l math_Vec2l_limit_max = { .x = i64_limit_max, .y = i64_limit_max };

/* Construction */
force_inline math_Vec2l math_Vec2l_create(i64 x, i64 y);
force_inline math_Vec2l math_Vec2l_fill(i64 scalar);
force_inline math_Vec2l math_Vec2l_from3(math_Vec3l v);
force_inline math_Vec2l math_Vec2l_from4(math_Vec4l v);
force_inline math_Vec2l math_Vec2l_sincos(i64 radians);

/* Comparison */
force_inline cmp_fnCmp(math_Vec2l);
cmp_fnEq_default(math_Vec2l);
cmp_fnNe_default(math_Vec2l);
cmp_fnLt_default(math_Vec2l);
cmp_fnGt_default(math_Vec2l);
cmp_fnLe_default(math_Vec2l);
cmp_fnGe_default(math_Vec2l);

/* Arithmetic */
force_inline math_Vec2l math_Vec2l_neg(math_Vec2l v);
force_inline math_Vec2l math_Vec2l_add(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_sub(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_mul(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_div(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_mod(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_scale(math_Vec2l v, i64 scalar);

/* Range Operations */
force_inline math_Vec2l math_Vec2l_min(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_max(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_clamp(math_Vec2l v, math_Vec2l min, math_Vec2l max);
force_inline math_Vec2l math_Vec2l_wrap(math_Vec2l v, math_Vec2l min, math_Vec2l max);

/* Geometric Operations */
force_inline i64        math_Vec2l_lenSq(math_Vec2l v);
force_inline i64        math_Vec2l_len(math_Vec2l v);
force_inline i64        math_Vec2l_distSq(math_Vec2l lhs, math_Vec2l rhs);
force_inline i64        math_Vec2l_dist(math_Vec2l lhs, math_Vec2l rhs);
force_inline i64        math_Vec2l_dot(math_Vec2l lhs, math_Vec2l rhs);
force_inline math_Vec2l math_Vec2l_norm(math_Vec2l v);
force_inline math_Vec2l math_Vec2l_project(math_Vec2l v, math_Vec2l onto);
force_inline math_Vec2l math_Vec2l_reject(math_Vec2l v, math_Vec2l from);
force_inline math_Vec2l math_Vec2l_reflect(math_Vec2l v, math_Vec2l normal);
force_inline math_Vec2l math_Vec2l_rotate(math_Vec2l v, i64 angle);
force_inline math_Vec2l math_Vec2l_perp(math_Vec2l v);

/* Vec3l functions */
/* Constants */
static const math_Vec3l math_Vec3l_zero      = { .x = 0, .y = 0, .z = 0 };
static const math_Vec3l math_Vec3l_one       = { .x = 1, .y = 1, .z = 1 };
static const math_Vec3l math_Vec3l_left      = { .x = -1, .y = 0, .z = 0 };
static const math_Vec3l math_Vec3l_up        = { .x = 0, .y = 1, .z = 0 };
static const math_Vec3l math_Vec3l_right     = { .x = 1, .y = 0, .z = 0 };
static const math_Vec3l math_Vec3l_down      = { .x = 0, .y = -1, .z = 0 };
static const math_Vec3l math_Vec3l_forward   = { .x = 0, .y = 0, .z = 1 };
static const math_Vec3l math_Vec3l_backward  = { .x = 0, .y = 0, .z = -1 };
static const math_Vec3l math_Vec3l_limit_min = { .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min };
static const math_Vec3l math_Vec3l_limit_max = { .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max };

/* Construction */
force_inline math_Vec3l math_Vec3l_create(i64 x, i64 y, i64 z);
force_inline math_Vec3l math_Vec3l_fill(i64 scalar);
force_inline math_Vec3l math_Vec3l_from2(math_Vec2l v);
force_inline math_Vec3l math_Vec3l_from4(math_Vec4l v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec3l);
cmp_fnEq_default(math_Vec3l);
cmp_fnNe_default(math_Vec3l);
cmp_fnLt_default(math_Vec3l);
cmp_fnGt_default(math_Vec3l);
cmp_fnLe_default(math_Vec3l);
cmp_fnGe_default(math_Vec3l);

/* Arithmetic */
force_inline math_Vec3l math_Vec3l_neg(math_Vec3l v);
force_inline math_Vec3l math_Vec3l_add(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_sub(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_mul(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_div(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_mod(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_scale(math_Vec3l v, i64 scalar);

/* Range Operations */
force_inline math_Vec3l math_Vec3l_min(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_max(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_clamp(math_Vec3l v, math_Vec3l min, math_Vec3l max);
force_inline math_Vec3l math_Vec3l_wrap(math_Vec3l v, math_Vec3l min, math_Vec3l max);

/* Geometric Operations */
force_inline i64        math_Vec3l_lenSq(math_Vec3l v);
force_inline i64        math_Vec3l_len(math_Vec3l v);
force_inline i64        math_Vec3l_distSq(math_Vec3l lhs, math_Vec3l rhs);
force_inline i64        math_Vec3l_dist(math_Vec3l lhs, math_Vec3l rhs);
force_inline i64        math_Vec3l_dot(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_cross(math_Vec3l lhs, math_Vec3l rhs);
force_inline math_Vec3l math_Vec3l_norm(math_Vec3l v);
force_inline math_Vec3l math_Vec3l_project(math_Vec3l v, math_Vec3l onto);
force_inline math_Vec3l math_Vec3l_reject(math_Vec3l v, math_Vec3l from);
force_inline math_Vec3l math_Vec3l_reflect(math_Vec3l v, math_Vec3l normal);
force_inline math_Vec3l math_Vec3l_rotate(math_Vec3l v, math_Vec3l axis, i64 angle);
force_inline math_Vec3l math_Vec3l_perp(math_Vec3l v);

/* Vec4l functions */
/* Constants */
static const math_Vec4l math_Vec4l_zero      = { .x = 0, .y = 0, .z = 0, .w = 0 };
static const math_Vec4l math_Vec4l_one       = { .x = 1, .y = 1, .z = 1, .w = 1 };
static const math_Vec4l math_Vec4l_left      = { .x = -1, .y = 0, .z = 0, .w = 0 };
static const math_Vec4l math_Vec4l_up        = { .x = 0, .y = 1, .z = 0, .w = 0 };
static const math_Vec4l math_Vec4l_right     = { .x = 1, .y = 0, .z = 0, .w = 0 };
static const math_Vec4l math_Vec4l_down      = { .x = 0, .y = -1, .z = 0, .w = 0 };
static const math_Vec4l math_Vec4l_forward   = { .x = 0, .y = 0, .z = 1, .w = 0 };
static const math_Vec4l math_Vec4l_backward  = { .x = 0, .y = 0, .z = -1, .w = 0 };
static const math_Vec4l math_Vec4l_limit_min = { .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min, .w = i64_limit_min };
static const math_Vec4l math_Vec4l_limit_max = { .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max, .w = i64_limit_max };

/* Construction */
force_inline math_Vec4l math_Vec4l_create(i64 x, i64 y, i64 z, i64 w);
force_inline math_Vec4l math_Vec4l_fill(i64 scalar);
force_inline math_Vec4l math_Vec4l_from2(math_Vec2l v);
force_inline math_Vec4l math_Vec4l_from3(math_Vec3l v);

/* Comparison */
force_inline cmp_fnCmp(math_Vec4l);
cmp_fnEq_default(math_Vec4l);
cmp_fnNe_default(math_Vec4l);
cmp_fnLt_default(math_Vec4l);
cmp_fnGt_default(math_Vec4l);
cmp_fnLe_default(math_Vec4l);
cmp_fnGe_default(math_Vec4l);

/* Arithmetic */
force_inline math_Vec4l math_Vec4l_neg(math_Vec4l v);
force_inline math_Vec4l math_Vec4l_add(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_sub(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_mul(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_div(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_mod(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_scale(math_Vec4l v, i64 scalar);

/* Range Operations */
force_inline math_Vec4l math_Vec4l_min(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_max(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_clamp(math_Vec4l v, math_Vec4l min, math_Vec4l max);
force_inline math_Vec4l math_Vec4l_wrap(math_Vec4l v, math_Vec4l min, math_Vec4l max);

/* Geometric Operations */
force_inline i64        math_Vec4l_lenSq(math_Vec4l v);
force_inline i64        math_Vec4l_len(math_Vec4l v);
force_inline i64        math_Vec4l_distSq(math_Vec4l lhs, math_Vec4l rhs);
force_inline i64        math_Vec4l_dist(math_Vec4l lhs, math_Vec4l rhs);
force_inline i64        math_Vec4l_dot(math_Vec4l lhs, math_Vec4l rhs);
force_inline math_Vec4l math_Vec4l_norm(math_Vec4l v);
force_inline math_Vec4l math_Vec4l_project(math_Vec4l v, math_Vec4l onto);
force_inline math_Vec4l math_Vec4l_reject(math_Vec4l v, math_Vec4l from);
force_inline math_Vec4l math_Vec4l_reflect(math_Vec4l v, math_Vec4l normal);

#endif /* MATH_VEC_INCLUDED */
