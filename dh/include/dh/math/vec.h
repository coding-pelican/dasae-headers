/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-01-18 (date of last update)
 * @version v0.1-alpha.3
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

/*========== Float32 (f32) Vector Functions =================================*/

/* Vec2f functions */
/* Constants */
#define math_Vec2f_zero      ((math_Vec2f){ .x = 0.0f, .y = 0.0f })
#define math_Vec2f_unit_x    ((math_Vec2f){ .x = 1.0f, .y = 0.0f })
#define math_Vec2f_unit_y    ((math_Vec2f){ .x = 0.0f, .y = 1.0f })
#define math_Vec2f_one       ((math_Vec2f){ .x = 1.0f, .y = 1.0f })
#define math_Vec2f_left      ((math_Vec2f){ .x = -1.0f, .y = 0.0f })
#define math_Vec2f_up        ((math_Vec2f){ .x = 0.0f, .y = 1.0f })
#define math_Vec2f_right     ((math_Vec2f){ .x = 1.0f, .y = 0.0f })
#define math_Vec2f_down      ((math_Vec2f){ .x = 0.0f, .y = -1.0f })
#define math_Vec2f_eps       ((math_Vec2f){ .x = f32_eps, .y = f32_eps })
#define math_Vec2f_nan       ((math_Vec2f){ .x = f32_nan, .y = f32_nan })
#define math_Vec2f_inf       ((math_Vec2f){ .x = f32_inf, .y = f32_inf })
#define math_Vec2f_limit_min ((math_Vec2f){ .x = f32_limit_min, .y = f32_limit_min })
#define math_Vec2f_limit_max ((math_Vec2f){ .x = f32_limit_max, .y = f32_limit_max })

/* Construction */
#define literal_math_Vec2f_from(f32_x, f32_y) ((math_Vec2f){ .x = (f32_x), .y = (f32_y) })
$inline_always math_Vec2f math_Vec2f_from(f32 x, f32 y);
$inline_always math_Vec2f math_Vec2f_from3(math_Vec3f v);
$inline_always math_Vec2f math_Vec2f_from4(math_Vec4f v);
$inline_always math_Vec2f math_Vec2f_fill(f32 scalar);
$inline_always math_Vec2f math_Vec2f_sincos(f32 radians);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2f);
cmp_fnEq_default(math_Vec2f);
cmp_fnNe_default(math_Vec2f);
cmp_fnLt_default(math_Vec2f);
cmp_fnGt_default(math_Vec2f);
cmp_fnLe_default(math_Vec2f);
cmp_fnGe_default(math_Vec2f);

$inline_always cmp_fnCmpApx(math_Vec2f);
cmp_fnEqApx_default(math_Vec2f);
cmp_fnNeApx_default(math_Vec2f);
cmp_fnLtApx_default(math_Vec2f);
cmp_fnGtApx_default(math_Vec2f);
cmp_fnLeApx_default(math_Vec2f);
cmp_fnGeApx_default(math_Vec2f);

/* Arithmetic */
$inline_always math_Vec2f math_Vec2f_neg(math_Vec2f v);
$inline_always math_Vec2f math_Vec2f_add(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_sub(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_mul(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_div(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_mod(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_scale(math_Vec2f v, f32 scalar);
$inline_always math_Vec2f math_Vec2f_scaleInv(math_Vec2f v, f32 scalar);

$inline_always math_Vec2f math_Vec2f_addScalar(math_Vec2f lhs, f32 rhs);
$inline_always math_Vec2f math_Vec2f_subScalar(math_Vec2f lhs, f32 rhs);
$inline_always math_Vec2f math_Vec2f_mulScalar(math_Vec2f lhs, f32 rhs);
$inline_always math_Vec2f math_Vec2f_divScalar(math_Vec2f lhs, f32 rhs);
$inline_always math_Vec2f math_Vec2f_modScalar(math_Vec2f lhs, f32 rhs);

$inline_always math_Vec2f* math_Vec2f_addAsg(math_Vec2f* lhs, math_Vec2f rhs);
$inline_always math_Vec2f* math_Vec2f_subAsg(math_Vec2f* lhs, math_Vec2f rhs);
$inline_always math_Vec2f* math_Vec2f_mulAsg(math_Vec2f* lhs, math_Vec2f rhs);
$inline_always math_Vec2f* math_Vec2f_divAsg(math_Vec2f* lhs, math_Vec2f rhs);
$inline_always math_Vec2f* math_Vec2f_modAsg(math_Vec2f* lhs, math_Vec2f rhs);
$inline_always math_Vec2f* math_Vec2f_scaleAsg(math_Vec2f* v, f32 scalar);
$inline_always math_Vec2f* math_Vec2f_scaleInvAsg(math_Vec2f* v, f32 scalar);

$inline_always math_Vec2f* math_Vec2f_addScalarAsg(math_Vec2f* lhs, f32 rhs);
$inline_always math_Vec2f* math_Vec2f_subScalarAsg(math_Vec2f* lhs, f32 rhs);
$inline_always math_Vec2f* math_Vec2f_mulScalarAsg(math_Vec2f* lhs, f32 rhs);
$inline_always math_Vec2f* math_Vec2f_divScalarAsg(math_Vec2f* lhs, f32 rhs);
$inline_always math_Vec2f* math_Vec2f_modScalarAsg(math_Vec2f* lhs, f32 rhs);

/* Range Operations */
$inline_always math_Vec2f math_Vec2f_min(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_max(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_clamp(math_Vec2f v, math_Vec2f min, math_Vec2f max);
$inline_always math_Vec2f math_Vec2f_clamp01(math_Vec2f v);
$inline_always math_Vec2f math_Vec2f_wrap(math_Vec2f v, math_Vec2f min, math_Vec2f max);
$inline_always math_Vec2f math_Vec2f_wrap01(math_Vec2f v);

/* Geometric Operations */
$inline_always f32        math_Vec2f_lenSq(math_Vec2f v);
$inline_always f32        math_Vec2f_len(math_Vec2f v);
$inline_always f32        math_Vec2f_distSq(math_Vec2f lhs, math_Vec2f rhs);
$inline_always f32        math_Vec2f_dist(math_Vec2f lhs, math_Vec2f rhs);
$inline_always f32        math_Vec2f_dot(math_Vec2f lhs, math_Vec2f rhs);
$inline_always f32        math_Vec2f_cross(math_Vec2f lhs, math_Vec2f rhs);
$inline_always math_Vec2f math_Vec2f_norm(math_Vec2f v);
$inline_always math_Vec2f math_Vec2f_project(math_Vec2f v, math_Vec2f onto);
$inline_always math_Vec2f math_Vec2f_reject(math_Vec2f v, math_Vec2f from);
$inline_always math_Vec2f math_Vec2f_reflect(math_Vec2f v, math_Vec2f normal);
$inline_always math_Vec2f math_Vec2f_rotate(math_Vec2f v, f32 angle);
$inline_always math_Vec2f math_Vec2f_perp(math_Vec2f v);

/* Vec3f functions */
/* Constants */
#define math_Vec3f_zero      ((math_Vec3f){ .x = 0.0f, .y = 0.0f, .z = 0.0f })
#define math_Vec3f_unit_x    ((math_Vec3f){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define math_Vec3f_unit_y    ((math_Vec3f){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define math_Vec3f_unit_z    ((math_Vec3f){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define math_Vec3f_unit_xy   ((math_Vec3f){ .x = 1.0f, .y = 1.0f, .z = 0.0f })
#define math_Vec3f_unit_yz   ((math_Vec3f){ .x = 0.0f, .y = 1.0f, .z = 1.0f })
#define math_Vec3f_unit_zx   ((math_Vec3f){ .x = 1.0f, .y = 0.0f, .z = 1.0f })
#define math_Vec3f_one       ((math_Vec3f){ .x = 1.0f, .y = 1.0f, .z = 1.0f })
#define math_Vec3f_left      ((math_Vec3f){ .x = -1.0f, .y = 0.0f, .z = 0.0f })
#define math_Vec3f_up        ((math_Vec3f){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define math_Vec3f_right     ((math_Vec3f){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define math_Vec3f_down      ((math_Vec3f){ .x = 0.0f, .y = -1.0f, .z = 0.0f })
#define math_Vec3f_forward   ((math_Vec3f){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define math_Vec3f_backward  ((math_Vec3f){ .x = 0.0f, .y = 0.0f, .z = -1.0f })
#define math_Vec3f_eps       ((math_Vec3f){ .x = f32_eps, .y = f32_eps, .z = f32_eps })
#define math_Vec3f_nan       ((math_Vec3f){ .x = f32_nan, .y = f32_nan, .z = f32_nan })
#define math_Vec3f_inf       ((math_Vec3f){ .x = f32_inf, .y = f32_inf, .z = f32_inf })
#define math_Vec3f_limit_min ((math_Vec3f){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min })
#define math_Vec3f_limit_max ((math_Vec3f){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max })

/* Construction */
#define literal_math_Vec3f_from(f32_x, f32_y, f32_z) ((math_Vec3f){ .x = (f32_x), .y = (f32_y), .z = (f32_z) })
$inline_always math_Vec3f math_Vec3f_from(f32 x, f32 y, f32 z);
$inline_always math_Vec3f math_Vec3f_from2(math_Vec2f v);
$inline_always math_Vec3f math_Vec3f_from4(math_Vec4f v);
$inline_always math_Vec3f math_Vec3f_fill(f32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3f);
cmp_fnEq_default(math_Vec3f);
cmp_fnNe_default(math_Vec3f);
cmp_fnLt_default(math_Vec3f);
cmp_fnGt_default(math_Vec3f);
cmp_fnLe_default(math_Vec3f);
cmp_fnGe_default(math_Vec3f);

$inline_always cmp_fnCmpApx(math_Vec3f);
cmp_fnEqApx_default(math_Vec3f);
cmp_fnNeApx_default(math_Vec3f);
cmp_fnLtApx_default(math_Vec3f);
cmp_fnGtApx_default(math_Vec3f);
cmp_fnLeApx_default(math_Vec3f);
cmp_fnGeApx_default(math_Vec3f);

/* Arithmetic */
$inline_always math_Vec3f math_Vec3f_neg(math_Vec3f v);
$inline_always math_Vec3f math_Vec3f_add(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_sub(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_mul(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_div(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_mod(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_scale(math_Vec3f v, f32 scalar);

/* Range Operations */
$inline_always math_Vec3f math_Vec3f_min(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_max(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_clamp(math_Vec3f v, math_Vec3f min, math_Vec3f max);
$inline_always math_Vec3f math_Vec3f_clamp01(math_Vec3f v);
$inline_always math_Vec3f math_Vec3f_wrap(math_Vec3f v, math_Vec3f min, math_Vec3f max);
$inline_always math_Vec3f math_Vec3f_wrap01(math_Vec3f v);

/* Geometric Operations */
$inline_always f32        math_Vec3f_lenSq(math_Vec3f v);
$inline_always f32        math_Vec3f_len(math_Vec3f v);
$inline_always f32        math_Vec3f_distSq(math_Vec3f lhs, math_Vec3f rhs);
$inline_always f32        math_Vec3f_dist(math_Vec3f lhs, math_Vec3f rhs);
$inline_always f32        math_Vec3f_dot(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_cross(math_Vec3f lhs, math_Vec3f rhs);
$inline_always math_Vec3f math_Vec3f_norm(math_Vec3f v);
$inline_always math_Vec3f math_Vec3f_project(math_Vec3f v, math_Vec3f onto);
$inline_always math_Vec3f math_Vec3f_reject(math_Vec3f v, math_Vec3f from);
$inline_always math_Vec3f math_Vec3f_reflect(math_Vec3f v, math_Vec3f normal);
$inline_always math_Vec3f math_Vec3f_rotate(math_Vec3f v, math_Vec3f axis, f32 angle);
$inline_always math_Vec3f math_Vec3f_perp(math_Vec3f v);

/* Vec4f functions */
/* Constants */
#define math_Vec4f_zero      ((math_Vec4f){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_unit_x    ((math_Vec4f){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_unit_y    ((math_Vec4f){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_unit_z    ((math_Vec4f){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define math_Vec4f_unit_w    ((math_Vec4f){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f })
#define math_Vec4f_one       ((math_Vec4f){ .x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f })
#define math_Vec4f_left      ((math_Vec4f){ .x = -1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_up        ((math_Vec4f){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_right     ((math_Vec4f){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_down      ((math_Vec4f){ .x = 0.0f, .y = -1.0f, .z = 0.0f, .w = 0.0f })
#define math_Vec4f_forward   ((math_Vec4f){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define math_Vec4f_backward  ((math_Vec4f){ .x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 0.0f })
#define math_Vec4f_eps       ((math_Vec4f){ .x = f32_eps, .y = f32_eps, .z = f32_eps, .w = f32_eps })
#define math_Vec4f_nan       ((math_Vec4f){ .x = f32_nan, .y = f32_nan, .z = f32_nan, .w = f32_nan })
#define math_Vec4f_inf       ((math_Vec4f){ .x = f32_inf, .y = f32_inf, .z = f32_inf, .w = f32_inf })
#define math_Vec4f_limit_min ((math_Vec4f){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min, .w = f32_limit_min })
#define math_Vec4f_limit_max ((math_Vec4f){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max, .w = f32_limit_max })

/* Construction */
#define literal_math_Vec4f_from(f32_x, f32_y, f32_z, f32_w) ((math_Vec3f){ .x = (f32_x), .y = (f32_y), .z = (f32_z), .w = (f32_w) })
$inline_always math_Vec4f math_Vec4f_from(f32 x, f32 y, f32 z, f32 w);
$inline_always math_Vec4f math_Vec4f_from2(math_Vec2f v);
$inline_always math_Vec4f math_Vec4f_from3(math_Vec3f v);
$inline_always math_Vec4f math_Vec4f_fill(f32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4f);
cmp_fnEq_default(math_Vec4f);
cmp_fnNe_default(math_Vec4f);
cmp_fnLt_default(math_Vec4f);
cmp_fnGt_default(math_Vec4f);
cmp_fnLe_default(math_Vec4f);
cmp_fnGe_default(math_Vec4f);

$inline_always cmp_fnCmpApx(math_Vec4f);
cmp_fnEqApx_default(math_Vec4f);
cmp_fnNeApx_default(math_Vec4f);
cmp_fnLtApx_default(math_Vec4f);
cmp_fnGtApx_default(math_Vec4f);
cmp_fnLeApx_default(math_Vec4f);
cmp_fnGeApx_default(math_Vec4f);

/* Arithmetic */
$inline_always math_Vec4f math_Vec4f_neg(math_Vec4f v);
$inline_always math_Vec4f math_Vec4f_add(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_sub(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_mul(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_div(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_mod(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_scale(math_Vec4f v, f32 scalar);

/* Range Operations */
$inline_always math_Vec4f math_Vec4f_min(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_max(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_clamp(math_Vec4f v, math_Vec4f min, math_Vec4f max);
$inline_always math_Vec4f math_Vec4f_clamp01(math_Vec4f v);
$inline_always math_Vec4f math_Vec4f_wrap(math_Vec4f v, math_Vec4f min, math_Vec4f max);
$inline_always math_Vec4f math_Vec4f_wrap01(math_Vec4f v);

/* Geometric Operations */
$inline_always f32        math_Vec4f_lenSq(math_Vec4f v);
$inline_always f32        math_Vec4f_len(math_Vec4f v);
$inline_always f32        math_Vec4f_distSq(math_Vec4f lhs, math_Vec4f rhs);
$inline_always f32        math_Vec4f_dist(math_Vec4f lhs, math_Vec4f rhs);
$inline_always f32        math_Vec4f_dot(math_Vec4f lhs, math_Vec4f rhs);
$inline_always math_Vec4f math_Vec4f_norm(math_Vec4f v);
$inline_always math_Vec4f math_Vec4f_project(math_Vec4f v, math_Vec4f onto);
$inline_always math_Vec4f math_Vec4f_reject(math_Vec4f v, math_Vec4f from);
$inline_always math_Vec4f math_Vec4f_reflect(math_Vec4f v, math_Vec4f normal);

/*========== Float64 (f64) Vector Functions =================================*/

/* Vec2d functions */
/* Constants */
#define math_Vec2d_zero      ((math_Vec2d){ .x = 0.0, .y = 0.0 })
#define math_Vec2d_unit_x    ((math_Vec2d){ .x = 1.0, .y = 0.0 })
#define math_Vec2d_unit_y    ((math_Vec2d){ .x = 0.0, .y = 1.0 })
#define math_Vec2d_one       ((math_Vec2d){ .x = 1.0, .y = 1.0 })
#define math_Vec2d_left      ((math_Vec2d){ .x = -1.0, .y = 0.0 })
#define math_Vec2d_up        ((math_Vec2d){ .x = 0.0, .y = 1.0 })
#define math_Vec2d_right     ((math_Vec2d){ .x = 1.0, .y = 0.0 })
#define math_Vec2d_down      ((math_Vec2d){ .x = 0.0, .y = -1.0 })
#define math_Vec2d_eps       ((math_Vec2d){ .x = f64_eps, .y = f64_eps })
#define math_Vec2d_nan       ((math_Vec2d){ .x = f64_nan, .y = f64_nan })
#define math_Vec2d_inf       ((math_Vec2d){ .x = f64_inf, .y = f64_inf })
#define math_Vec2d_limit_min ((math_Vec2d){ .x = f64_limit_min, .y = f64_limit_min })
#define math_Vec2d_limit_max ((math_Vec2d){ .x = f64_limit_max, .y = f64_limit_max })

/* Construction */
#define literal_math_Vec2d_from(f64_x, f64_y) ((math_Vec2d){ .x = (f64_x), .y = (f64_y) })
$inline_always math_Vec2d math_Vec2d_from(f64 x, f64 y);
$inline_always math_Vec2d math_Vec2d_from3(math_Vec3d v);
$inline_always math_Vec2d math_Vec2d_from4(math_Vec4d v);
$inline_always math_Vec2d math_Vec2d_fill(f64 scalar);
$inline_always math_Vec2d math_Vec2d_sincos(f64 radians);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2d);
cmp_fnEq_default(math_Vec2d);
cmp_fnNe_default(math_Vec2d);
cmp_fnLt_default(math_Vec2d);
cmp_fnGt_default(math_Vec2d);
cmp_fnLe_default(math_Vec2d);
cmp_fnGe_default(math_Vec2d);

/* Arithmetic */
$inline_always math_Vec2d math_Vec2d_neg(math_Vec2d v);
$inline_always math_Vec2d math_Vec2d_add(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_sub(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_mul(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_div(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_mod(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_scale(math_Vec2d v, f64 scalar);
$inline_always math_Vec2d math_Vec2d_scaleInv(math_Vec2d v, f64 scaler);

$inline_always math_Vec2d math_Vec2d_addScalar(math_Vec2d lhs, f64 rhs);
$inline_always math_Vec2d math_Vec2d_subScalar(math_Vec2d lhs, f64 rhs);
$inline_always math_Vec2d math_Vec2d_mulScalar(math_Vec2d lhs, f64 rhs);
$inline_always math_Vec2d math_Vec2d_divScalar(math_Vec2d lhs, f64 rhs);
$inline_always math_Vec2d math_Vec2d_modScalar(math_Vec2d lhs, f64 rhs);

$inline_always math_Vec2d* math_Vec2d_addAsg(math_Vec2d* lhs, math_Vec2d rhs);
$inline_always math_Vec2d* math_Vec2d_subAsg(math_Vec2d* lhs, math_Vec2d rhs);
$inline_always math_Vec2d* math_Vec2d_mulAsg(math_Vec2d* lhs, math_Vec2d rhs);
$inline_always math_Vec2d* math_Vec2d_divAsg(math_Vec2d* lhs, math_Vec2d rhs);
$inline_always math_Vec2d* math_Vec2d_modAsg(math_Vec2d* lhs, math_Vec2d rhs);
$inline_always math_Vec2d* math_Vec2d_scaleAsg(math_Vec2d* v, f64 scalar);
$inline_always math_Vec2d* math_Vec2d_scaleInvAsg(math_Vec2d* v, f64 scalar);

$inline_always math_Vec2d* math_Vec2d_addScalarAsg(math_Vec2d* lhs, f64 rhs);
$inline_always math_Vec2d* math_Vec2d_subScalarAsg(math_Vec2d* lhs, f64 rhs);
$inline_always math_Vec2d* math_Vec2d_mulScalarAsg(math_Vec2d* lhs, f64 rhs);
$inline_always math_Vec2d* math_Vec2d_divScalarAsg(math_Vec2d* lhs, f64 rhs);
$inline_always math_Vec2d* math_Vec2d_modScalarAsg(math_Vec2d* lhs, f64 rhs);

/* Range Operations */
$inline_always math_Vec2d math_Vec2d_min(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_max(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_clamp(math_Vec2d v, math_Vec2d min, math_Vec2d max);
$inline_always math_Vec2d math_Vec2d_clamp01(math_Vec2d v);
$inline_always math_Vec2d math_Vec2d_wrap(math_Vec2d v, math_Vec2d min, math_Vec2d max);
$inline_always math_Vec2d math_Vec2d_wrap01(math_Vec2d v);

/* Geometric Operations */
$inline_always f64        math_Vec2d_lenSq(math_Vec2d v);
$inline_always f64        math_Vec2d_len(math_Vec2d v);
$inline_always f64        math_Vec2d_distSq(math_Vec2d lhs, math_Vec2d rhs);
$inline_always f64        math_Vec2d_dist(math_Vec2d lhs, math_Vec2d rhs);
$inline_always f64        math_Vec2d_dot(math_Vec2d lhs, math_Vec2d rhs);
$inline_always f64        math_Vec2d_cross(math_Vec2d lhs, math_Vec2d rhs);
$inline_always math_Vec2d math_Vec2d_norm(math_Vec2d v);
$inline_always math_Vec2d math_Vec2d_project(math_Vec2d v, math_Vec2d onto);
$inline_always math_Vec2d math_Vec2d_reject(math_Vec2d v, math_Vec2d from);
$inline_always math_Vec2d math_Vec2d_reflect(math_Vec2d v, math_Vec2d normal);
$inline_always math_Vec2d math_Vec2d_rotate(math_Vec2d v, f64 angle);
$inline_always math_Vec2d math_Vec2d_perp(math_Vec2d v);

/* Vec3d functions */
/* Constants */
#define math_Vec3d_zero     ((math_Vec3d){ .x = 0.0, .y = 0.0, .z = 0.0 })
#define math_Vec3d_unit_x   ((math_Vec3d){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define math_Vec3d_unit_y   ((math_Vec3d){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define math_Vec3d_unit_z   ((math_Vec3d){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define math_Vec3d_unit_xy  ((math_Vec3d){ .x = 1.0, .y = 1.0, .z = 0.0 })
#define math_Vec3d_unit_yz  ((math_Vec3d){ .x = 0.0, .y = 1.0, .z = 1.0 })
#define math_Vec3d_unit_zx  ((math_Vec3d){ .x = 1.0, .y = 0.0, .z = 1.0 })
#define math_Vec3d_one      ((math_Vec3d){ .x = 1.0, .y = 1.0, .z = 1.0 })
#define math_Vec3d_left     ((math_Vec3d){ .x = -1.0, .y = 0.0, .z = 0.0 })
#define math_Vec3d_up       ((math_Vec3d){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define math_Vec3d_right    ((math_Vec3d){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define math_Vec3d_down     ((math_Vec3d){ .x = 0.0, .y = -1.0, .z = 0.0 })
#define math_Vec3d_forward  ((math_Vec3d){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define math_Vec3d_backward ((math_Vec3d){ .x = 0.0, .y = 0.0, .z = -1.0 })
#define math_Vec3d_eps      ((math_Vec3d){ .x = f64_eps, .y = f64_eps, .z = f64_eps })
#define math_Vec3d_nan      ((math_Vec3d){ .x = f64_nan, .y = f64_nan, .z = f64_nan })
#define math_Vec3d_inf      ((math_Vec3d){ .x = f64_inf, .y = f64_inf, .z = f64_inf })

/* Construction */
#define literal_math_Vec3d_from(f64_x, f64_y, f64_z) ((math_Vec3d){ .x = (f64_x), .y = (f64_y), .z = (f64_z) })
$inline_always math_Vec3d math_Vec3d_from(f64 x, f64 y, f64 z);
$inline_always math_Vec3d math_Vec3d_from2(math_Vec2d v);
$inline_always math_Vec3d math_Vec3d_from4(math_Vec4d v);
$inline_always math_Vec3d math_Vec3d_fill(f64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3d);
cmp_fnEq_default(math_Vec3d);
cmp_fnNe_default(math_Vec3d);
cmp_fnLt_default(math_Vec3d);
cmp_fnGt_default(math_Vec3d);
cmp_fnLe_default(math_Vec3d);
cmp_fnGe_default(math_Vec3d);

/* Arithmetic */
$inline_always math_Vec3d math_Vec3d_neg(math_Vec3d v);
$inline_always math_Vec3d math_Vec3d_add(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_sub(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_mul(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_div(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_mod(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_scale(math_Vec3d v, f64 scalar);

/* Range Operations */
$inline_always math_Vec3d math_Vec3d_min(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_max(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_clamp(math_Vec3d v, math_Vec3d min, math_Vec3d max);
$inline_always math_Vec3d math_Vec3d_clamp01(math_Vec3d v);
$inline_always math_Vec3d math_Vec3d_wrap(math_Vec3d v, math_Vec3d min, math_Vec3d max);
$inline_always math_Vec3d math_Vec3d_wrap01(math_Vec3d v);

/* Geometric Operations */
$inline_always f64        math_Vec3d_lenSq(math_Vec3d v);
$inline_always f64        math_Vec3d_len(math_Vec3d v);
$inline_always f64        math_Vec3d_distSq(math_Vec3d lhs, math_Vec3d rhs);
$inline_always f64        math_Vec3d_dist(math_Vec3d lhs, math_Vec3d rhs);
$inline_always f64        math_Vec3d_dot(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_cross(math_Vec3d lhs, math_Vec3d rhs);
$inline_always math_Vec3d math_Vec3d_norm(math_Vec3d v);
$inline_always math_Vec3d math_Vec3d_project(math_Vec3d v, math_Vec3d onto);
$inline_always math_Vec3d math_Vec3d_reject(math_Vec3d v, math_Vec3d from);
$inline_always math_Vec3d math_Vec3d_reflect(math_Vec3d v, math_Vec3d normal);
$inline_always math_Vec3d math_Vec3d_rotate(math_Vec3d v, math_Vec3d axis, f64 angle);
$inline_always math_Vec3d math_Vec3d_perp(math_Vec3d v);

/* Vec4d functions */
/* Constants */
#define math_Vec4d_zero      ((math_Vec4d){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_unit_x    ((math_Vec4d){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_unit_y    ((math_Vec4d){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_unit_z    ((math_Vec4d){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define math_Vec4d_unit_w    ((math_Vec4d){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0 })
#define math_Vec4d_one       ((math_Vec4d){ .x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0 })
#define math_Vec4d_left      ((math_Vec4d){ .x = -1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_up        ((math_Vec4d){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_right     ((math_Vec4d){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_down      ((math_Vec4d){ .x = 0.0, .y = -1.0, .z = 0.0, .w = 0.0 })
#define math_Vec4d_forward   ((math_Vec4d){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define math_Vec4d_backward  ((math_Vec4d){ .x = 0.0, .y = 0.0, .z = -1.0, .w = 0.0 })
#define math_Vec4d_eps       ((math_Vec4d){ .x = f64_eps, .y = f64_eps, .z = f64_eps, .w = f64_eps })
#define math_Vec4d_nan       ((math_Vec4d){ .x = f64_nan, .y = f64_nan, .z = f64_nan, .w = f64_nan })
#define math_Vec4d_inf       ((math_Vec4d){ .x = f64_inf, .y = f64_inf, .z = f64_inf, .w = f64_inf })
#define math_Vec4d_limit_min ((math_Vec4d){ .x = f64_limit_min, .y = f64_limit_min, .z = f64_limit_min, .w = f64_limit_min })
#define math_Vec4d_limit_max ((math_Vec4d){ .x = f64_limit_max, .y = f64_limit_max, .z = f64_limit_max, .w = f64_limit_max })

/* Construction */
#define literal_math_Vec4d_from(f64_x, f64_y, f64_z, f64_w) ((math_Vec4d){ .x = (f64_x), .y = (f64_y), .z = (f64_z), .w = (f64_w) })
$inline_always math_Vec4d math_Vec4d_from(f64 x, f64 y, f64 z, f64 w);
$inline_always math_Vec4d math_Vec4d_from2(math_Vec2d v);
$inline_always math_Vec4d math_Vec4d_from3(math_Vec3d v);
$inline_always math_Vec4d math_Vec4d_fill(f64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4d);
cmp_fnEq_default(math_Vec4d);
cmp_fnNe_default(math_Vec4d);
cmp_fnLt_default(math_Vec4d);
cmp_fnGt_default(math_Vec4d);
cmp_fnLe_default(math_Vec4d);
cmp_fnGe_default(math_Vec4d);

/* Arithmetic */
$inline_always math_Vec4d math_Vec4d_neg(math_Vec4d v);
$inline_always math_Vec4d math_Vec4d_add(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_sub(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_mul(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_div(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_mod(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_scale(math_Vec4d v, f64 scalar);

/* Range Operations */
$inline_always math_Vec4d math_Vec4d_min(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_max(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_clamp(math_Vec4d v, math_Vec4d min, math_Vec4d max);
$inline_always math_Vec4d math_Vec4d_clamp01(math_Vec4d v);
$inline_always math_Vec4d math_Vec4d_wrap(math_Vec4d v, math_Vec4d min, math_Vec4d max);
$inline_always math_Vec4d math_Vec4d_wrap01(math_Vec4d v);

/* Geometric Operations */
$inline_always f64        math_Vec4d_lenSq(math_Vec4d v);
$inline_always f64        math_Vec4d_len(math_Vec4d v);
$inline_always f64        math_Vec4d_distSq(math_Vec4d lhs, math_Vec4d rhs);
$inline_always f64        math_Vec4d_dist(math_Vec4d lhs, math_Vec4d rhs);
$inline_always f64        math_Vec4d_dot(math_Vec4d lhs, math_Vec4d rhs);
$inline_always math_Vec4d math_Vec4d_norm(math_Vec4d v);
$inline_always math_Vec4d math_Vec4d_project(math_Vec4d v, math_Vec4d onto);
$inline_always math_Vec4d math_Vec4d_reject(math_Vec4d v, math_Vec4d from);
$inline_always math_Vec4d math_Vec4d_reflect(math_Vec4d v, math_Vec4d normal);

/*========== Int32 (i32) Vector Functions ===================================*/

/* Vec2i functions */
/* Constants */
#define math_Vec2i_zero      ((math_Vec2i){ .x = 0, .y = 0 })
#define math_Vec2i_unit_x    ((math_Vec2i){ .x = 1, .y = 0 })
#define math_Vec2i_unit_y    ((math_Vec2i){ .x = 0, .y = 1 })
#define math_Vec2i_one       ((math_Vec2i){ .x = 1, .y = 1 })
#define math_Vec2i_left      ((math_Vec2i){ .x = -1, .y = 0 })
#define math_Vec2i_up        ((math_Vec2i){ .x = 0, .y = 1 })
#define math_Vec2i_right     ((math_Vec2i){ .x = 1, .y = 0 })
#define math_Vec2i_down      ((math_Vec2i){ .x = 0, .y = -1 })
#define math_Vec2i_limit_min ((math_Vec2i){ .x = i32_limit_min, .y = i32_limit_min })
#define math_Vec2i_limit_max ((math_Vec2i){ .x = i32_limit_max, .y = i32_limit_max })

/* Construction */
#define literal_math_Vec2i_from(i32_x, i32_y) ((math_Vec2i){ .x = (i32_x), .y = (i32_y) })
$inline_always math_Vec2i math_Vec2i_from(i32 x, i32 y);
$inline_always math_Vec2i math_Vec2i_from3(math_Vec3i v);
$inline_always math_Vec2i math_Vec2i_from4(math_Vec4i v);
$inline_always math_Vec2i math_Vec2i_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2i);
cmp_fnEq_default(math_Vec2i);
cmp_fnNe_default(math_Vec2i);
cmp_fnLt_default(math_Vec2i);
cmp_fnGt_default(math_Vec2i);
cmp_fnLe_default(math_Vec2i);
cmp_fnGe_default(math_Vec2i);

/* Arithmetic */
$inline_always math_Vec2i math_Vec2i_neg(math_Vec2i v);
$inline_always math_Vec2i math_Vec2i_add(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_sub(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_mul(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_div(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_mod(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_scale(math_Vec2i v, i32 scalar);

/* Range Operations */
$inline_always math_Vec2i math_Vec2i_min(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_max(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_clamp(math_Vec2i v, math_Vec2i min, math_Vec2i max);
$inline_always math_Vec2i math_Vec2i_wrap(math_Vec2i v, math_Vec2i min, math_Vec2i max);

/* Geometric Operations */
$inline_always i32        math_Vec2i_lenSq(math_Vec2i v);
$inline_always i32        math_Vec2i_distSq(math_Vec2i lhs, math_Vec2i rhs);
$inline_always i32        math_Vec2i_dot(math_Vec2i lhs, math_Vec2i rhs);
$inline_always i32        math_Vec2i_cross(math_Vec2i lhs, math_Vec2i rhs);
$inline_always math_Vec2i math_Vec2i_perp(math_Vec2i v);

/* Vec3i functions */
/* Constants */
#define math_Vec3i_zero      ((math_Vec3i){ .x = 0, .y = 0, .z = 0 })
#define math_Vec3i_unit_x    ((math_Vec3i){ .x = 1, .y = 0, .z = 0 })
#define math_Vec3i_unit_y    ((math_Vec3i){ .x = 0, .y = 1, .z = 0 })
#define math_Vec3i_unit_z    ((math_Vec3i){ .x = 0, .y = 0, .z = 1 })
#define math_Vec3i_unit_xy   ((math_Vec3i){ .x = 1, .y = 1, .z = 0 })
#define math_Vec3i_unit_yz   ((math_Vec3i){ .x = 0, .y = 1, .z = 1 })
#define math_Vec3i_unit_zx   ((math_Vec3i){ .x = 1, .y = 0, .z = 1 })
#define math_Vec3i_one       ((math_Vec3i){ .x = 1, .y = 1, .z = 1 })
#define math_Vec3i_left      ((math_Vec3i){ .x = -1, .y = 0, .z = 0 })
#define math_Vec3i_up        ((math_Vec3i){ .x = 0, .y = 1, .z = 0 })
#define math_Vec3i_right     ((math_Vec3i){ .x = 1, .y = 0, .z = 0 })
#define math_Vec3i_down      ((math_Vec3i){ .x = 0, .y = -1, .z = 0 })
#define math_Vec3i_forward   ((math_Vec3i){ .x = 0, .y = 0, .z = 1 })
#define math_Vec3i_backward  ((math_Vec3i){ .x = 0, .y = 0, .z = -1 })
#define math_Vec3i_limit_min ((math_Vec3i){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min })
#define math_Vec3i_limit_max ((math_Vec3i){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max })

/* Construction */
#define literal_math_Vec3i_from(i32_x, i32_y, i32_z) ((math_Vec3i){ .x = (i32_x), .y = (i32_y), .z = (i32_z) })
$inline_always math_Vec3i math_Vec3i_from(i32 x, i32 y, i32 z);
$inline_always math_Vec3i math_Vec3i_from2(math_Vec2i v);
$inline_always math_Vec3i math_Vec3i_from4(math_Vec4i v);
$inline_always math_Vec3i math_Vec3i_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3i);
cmp_fnEq_default(math_Vec3i);
cmp_fnNe_default(math_Vec3i);
cmp_fnLt_default(math_Vec3i);
cmp_fnGt_default(math_Vec3i);
cmp_fnLe_default(math_Vec3i);
cmp_fnGe_default(math_Vec3i);

/* Arithmetic */
$inline_always math_Vec3i math_Vec3i_neg(math_Vec3i v);
$inline_always math_Vec3i math_Vec3i_add(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_sub(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_mul(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_div(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_mod(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_scale(math_Vec3i v, i32 scalar);

/* Range Operations */
$inline_always math_Vec3i math_Vec3i_min(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_max(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_clamp(math_Vec3i v, math_Vec3i min, math_Vec3i max);
$inline_always math_Vec3i math_Vec3i_wrap(math_Vec3i v, math_Vec3i min, math_Vec3i max);

/* Geometric Operations */
$inline_always i32        math_Vec3i_lenSq(math_Vec3i v);
$inline_always i32        math_Vec3i_distSq(math_Vec3i lhs, math_Vec3i rhs);
$inline_always i32        math_Vec3i_dot(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_cross(math_Vec3i lhs, math_Vec3i rhs);
$inline_always math_Vec3i math_Vec3i_perp(math_Vec3i v);

/* Vec4i functions */
/* Constants */
#define math_Vec4i_zero      ((math_Vec4i){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define math_Vec4i_unit_x    ((math_Vec4i){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4i_unit_y    ((math_Vec4i){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define math_Vec4i_unit_z    ((math_Vec4i){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define math_Vec4i_unit_w    ((math_Vec4i){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define math_Vec4i_one       ((math_Vec4i){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define math_Vec4i_left      ((math_Vec4i){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4i_up        ((math_Vec4i){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define math_Vec4i_right     ((math_Vec4i){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4i_down      ((math_Vec4i){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define math_Vec4i_forward   ((math_Vec4i){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define math_Vec4i_backward  ((math_Vec4i){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define math_Vec4i_limit_min ((math_Vec4i){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min, .w = i32_limit_min })
#define math_Vec4i_limit_max ((math_Vec4i){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max, .w = i32_limit_max })

/* Construction */
#define literal_math_Vec4i_from(i32_x, i32_y, i32_z, i32_w) ((math_Vec4i){ .x = (i32_x), .y = (i32_y), .z = (i32_z), .w = (i32_w) })
$inline_always math_Vec4i math_Vec4i_from(i32 x, i32 y, i32 z, i32 w);
$inline_always math_Vec4i math_Vec4i_from2(math_Vec2i v);
$inline_always math_Vec4i math_Vec4i_from3(math_Vec3i v);
$inline_always math_Vec4i math_Vec4i_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4i);
cmp_fnEq_default(math_Vec4i);
cmp_fnNe_default(math_Vec4i);
cmp_fnLt_default(math_Vec4i);
cmp_fnGt_default(math_Vec4i);
cmp_fnLe_default(math_Vec4i);
cmp_fnGe_default(math_Vec4i);

/* Arithmetic */
$inline_always math_Vec4i math_Vec4i_neg(math_Vec4i v);
$inline_always math_Vec4i math_Vec4i_add(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_sub(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_mul(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_div(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_mod(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_scale(math_Vec4i v, i32 scalar);

/* Range Operations */
$inline_always math_Vec4i math_Vec4i_min(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_max(math_Vec4i lhs, math_Vec4i rhs);
$inline_always math_Vec4i math_Vec4i_clamp(math_Vec4i v, math_Vec4i min, math_Vec4i max);
$inline_always math_Vec4i math_Vec4i_wrap(math_Vec4i v, math_Vec4i min, math_Vec4i max);

/* Geometric Operations */
$inline_always i32 math_Vec4i_lenSq(math_Vec4i v);
$inline_always i32 math_Vec4i_distSq(math_Vec4i lhs, math_Vec4i rhs);
$inline_always i32 math_Vec4i_dot(math_Vec4i lhs, math_Vec4i rhs);

/*========== Int64 (i64) Vector Functions ===================================*/

/* Vec2l functions */
/* Constants */
#define math_Vec2l_zero      ((math_Vec2l){ .x = 0, .y = 0 })
#define math_Vec2l_unit_x    ((math_Vec2l){ .x = 1, .y = 0 })
#define math_Vec2l_unit_y    ((math_Vec2l){ .x = 0, .y = 1 })
#define math_Vec2l_one       ((math_Vec2l){ .x = 1, .y = 1 })
#define math_Vec2l_left      ((math_Vec2l){ .x = -1, .y = 0 })
#define math_Vec2l_up        ((math_Vec2l){ .x = 0, .y = 1 })
#define math_Vec2l_right     ((math_Vec2l){ .x = 1, .y = 0 })
#define math_Vec2l_down      ((math_Vec2l){ .x = 0, .y = -1 })
#define math_Vec2l_limit_min ((math_Vec2l){ .x = i64_limit_min, .y = i64_limit_min })
#define math_Vec2l_limit_max ((math_Vec2l){ .x = i64_limit_max, .y = i64_limit_max })

/* Construction */
#define literal_math_Vec2l_from(i64_x, i64_y) ((math_Vec2l){ .x = (i64_x), .y = (i64_y) })
$inline_always math_Vec2l math_Vec2l_from(i64 x, i64 y);
$inline_always math_Vec2l math_Vec2l_from3(math_Vec3l v);
$inline_always math_Vec2l math_Vec2l_from4(math_Vec4l v);
$inline_always math_Vec2l math_Vec2l_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2l);
cmp_fnEq_default(math_Vec2l);
cmp_fnNe_default(math_Vec2l);
cmp_fnLt_default(math_Vec2l);
cmp_fnGt_default(math_Vec2l);
cmp_fnLe_default(math_Vec2l);
cmp_fnGe_default(math_Vec2l);

/* Arithmetic */
$inline_always math_Vec2l math_Vec2l_neg(math_Vec2l v);
$inline_always math_Vec2l math_Vec2l_add(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_sub(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_mul(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_div(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_mod(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_scale(math_Vec2l v, i64 scalar);

/* Range Operations */
$inline_always math_Vec2l math_Vec2l_min(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_max(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_clamp(math_Vec2l v, math_Vec2l min, math_Vec2l max);
$inline_always math_Vec2l math_Vec2l_wrap(math_Vec2l v, math_Vec2l min, math_Vec2l max);

/* Geometric Operations */
$inline_always i64        math_Vec2l_lenSq(math_Vec2l v);
$inline_always i64        math_Vec2l_distSq(math_Vec2l lhs, math_Vec2l rhs);
$inline_always i64        math_Vec2l_dot(math_Vec2l lhs, math_Vec2l rhs);
$inline_always i64        math_Vec2l_cross(math_Vec2l lhs, math_Vec2l rhs);
$inline_always math_Vec2l math_Vec2l_perp(math_Vec2l v);

/* Vec3l functions */
/* Constants */
#define math_Vec3l_zero      ((math_Vec3l){ .x = 0, .y = 0, .z = 0 })
#define math_Vec3l_unit_x    ((math_Vec3l){ .x = 1, .y = 0, .z = 0 })
#define math_Vec3l_unit_y    ((math_Vec3l){ .x = 0, .y = 1, .z = 0 })
#define math_Vec3l_unit_z    ((math_Vec3l){ .x = 0, .y = 0, .z = 1 })
#define math_Vec3l_unit_xy   ((math_Vec3l){ .x = 1, .y = 1, .z = 0 })
#define math_Vec3l_unit_yz   ((math_Vec3l){ .x = 0, .y = 1, .z = 1 })
#define math_Vec3l_unit_zx   ((math_Vec3l){ .x = 1, .y = 0, .z = 1 })
#define math_Vec3l_one       ((math_Vec3l){ .x = 1, .y = 1, .z = 1 })
#define math_Vec3l_left      ((math_Vec3l){ .x = -1, .y = 0, .z = 0 })
#define math_Vec3l_up        ((math_Vec3l){ .x = 0, .y = 1, .z = 0 })
#define math_Vec3l_right     ((math_Vec3l){ .x = 1, .y = 0, .z = 0 })
#define math_Vec3l_down      ((math_Vec3l){ .x = 0, .y = -1, .z = 0 })
#define math_Vec3l_forward   ((math_Vec3l){ .x = 0, .y = 0, .z = 1 })
#define math_Vec3l_backward  ((math_Vec3l){ .x = 0, .y = 0, .z = -1 })
#define math_Vec3l_limit_min ((math_Vec3l){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min })
#define math_Vec3l_limit_max ((math_Vec3l){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max })

/* Construction */
#define literal_math_Vec3l_from(i64_x, i64_y, i64_z) ((math_Vec3l){ .x = (i64_x), .y = (i64_y), .z = (i64_z) })
$inline_always math_Vec3l math_Vec3l_from(i64 x, i64 y, i64 z);
$inline_always math_Vec3l math_Vec3l_from2(math_Vec2l v);
$inline_always math_Vec3l math_Vec3l_from4(math_Vec4l v);
$inline_always math_Vec3l math_Vec3l_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3l);
cmp_fnEq_default(math_Vec3l);
cmp_fnNe_default(math_Vec3l);
cmp_fnLt_default(math_Vec3l);
cmp_fnGt_default(math_Vec3l);
cmp_fnLe_default(math_Vec3l);
cmp_fnGe_default(math_Vec3l);

/* Arithmetic */
$inline_always math_Vec3l math_Vec3l_neg(math_Vec3l v);
$inline_always math_Vec3l math_Vec3l_add(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_sub(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_mul(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_div(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_mod(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_scale(math_Vec3l v, i64 scalar);

/* Range Operations */
$inline_always math_Vec3l math_Vec3l_min(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_max(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_clamp(math_Vec3l v, math_Vec3l min, math_Vec3l max);
$inline_always math_Vec3l math_Vec3l_wrap(math_Vec3l v, math_Vec3l min, math_Vec3l max);

/* Geometric Operations */
$inline_always i64        math_Vec3l_lenSq(math_Vec3l v);
$inline_always i64        math_Vec3l_distSq(math_Vec3l lhs, math_Vec3l rhs);
$inline_always i64        math_Vec3l_dot(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_cross(math_Vec3l lhs, math_Vec3l rhs);
$inline_always math_Vec3l math_Vec3l_perp(math_Vec3l v);

/* Vec4l functions */
/* Constants */
#define math_Vec4l_zero      ((math_Vec4l){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define math_Vec4l_unit_x    ((math_Vec4l){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4l_unit_y    ((math_Vec4l){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define math_Vec4l_unit_z    ((math_Vec4l){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define math_Vec4l_unit_w    ((math_Vec4l){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define math_Vec4l_one       ((math_Vec4l){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define math_Vec4l_left      ((math_Vec4l){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4l_up        ((math_Vec4l){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define math_Vec4l_right     ((math_Vec4l){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define math_Vec4l_down      ((math_Vec4l){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define math_Vec4l_forward   ((math_Vec4l){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define math_Vec4l_backward  ((math_Vec4l){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define math_Vec4l_limit_min ((math_Vec4l){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min, .w = i64_limit_min })
#define math_Vec4l_limit_max ((math_Vec4l){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max, .w = i64_limit_max })

/* Construction */
#define literal_math_Vec4l_from(i64_x, i64_y, i64_z, i64_w) ((math_Vec4l){ .x = (i64_x), .y = (i64_y), .z = (i64_z), .w = (i64_w) })
$inline_always math_Vec4l math_Vec4l_from(i64 x, i64 y, i64 z, i64 w);
$inline_always math_Vec4l math_Vec4l_from2(math_Vec2l v);
$inline_always math_Vec4l math_Vec4l_from3(math_Vec3l v);
$inline_always math_Vec4l math_Vec4l_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4l);
cmp_fnEq_default(math_Vec4l);
cmp_fnNe_default(math_Vec4l);
cmp_fnLt_default(math_Vec4l);
cmp_fnGt_default(math_Vec4l);
cmp_fnLe_default(math_Vec4l);
cmp_fnGe_default(math_Vec4l);

/* Arithmetic */
$inline_always math_Vec4l math_Vec4l_neg(math_Vec4l v);
$inline_always math_Vec4l math_Vec4l_add(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_sub(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_mul(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_div(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_mod(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_scale(math_Vec4l v, i64 scalar);

/* Range Operations */
$inline_always math_Vec4l math_Vec4l_min(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_max(math_Vec4l lhs, math_Vec4l rhs);
$inline_always math_Vec4l math_Vec4l_clamp(math_Vec4l v, math_Vec4l min, math_Vec4l max);
$inline_always math_Vec4l math_Vec4l_wrap(math_Vec4l v, math_Vec4l min, math_Vec4l max);

/* Geometric Operations */
$inline_always i64 math_Vec4l_lenSq(math_Vec4l v);
$inline_always i64 math_Vec4l_distSq(math_Vec4l lhs, math_Vec4l rhs);
$inline_always i64 math_Vec4l_dot(math_Vec4l lhs, math_Vec4l rhs);

/*========== Implementations ================================================*/

/* Vec2f functions */
/* Construction */
$inline_always math_Vec2f math_Vec2f_from(f32 x, f32 y) {
    return (math_Vec2f){ .x = x, .y = y };
}
$inline_always math_Vec2f math_Vec2f_from3(math_Vec3f v) {
    return math_Vec2f_from(v.x, v.y);
}
$inline_always math_Vec2f math_Vec2f_from4(math_Vec4f v) {
    return math_Vec2f_from(v.x, v.y);
}
$inline_always math_Vec2f math_Vec2f_fill(f32 scalar) {
    return math_Vec2f_from(scalar, scalar);
}
$inline_always math_Vec2f math_Vec2f_sincos(f32 radians) {
    return math_Vec2f_from(math_cos(radians), math_sin(radians));
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2f) {
    for (usize i = 0; i < 2; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(math_Vec2f) {
    for (usize i = 0; i < 2; ++i) {
        // Calculate absolute difference
        let diff = fabsf(self.s[i] - other.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec2f math_Vec2f_neg(math_Vec2f v) {
    return math_Vec2f_from(-v.x, -v.y);
}
$inline_always math_Vec2f math_Vec2f_add(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always math_Vec2f math_Vec2f_sub(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always math_Vec2f math_Vec2f_mul(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always math_Vec2f math_Vec2f_div(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always math_Vec2f math_Vec2f_mod(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always math_Vec2f math_Vec2f_scale(math_Vec2f v, f32 scalar) {
    return math_Vec2f_from(v.x * scalar, v.y * scalar);
}
$inline_always math_Vec2f math_Vec2f_scaleInv(math_Vec2f v, f32 scalar) {
    return math_Vec2f_from(v.x / scalar, v.y / scalar);
}

$inline_always math_Vec2f math_Vec2f_addScalar(math_Vec2f lhs, f32 rhs) {
    return math_Vec2f_add(lhs, math_Vec2f_fill(rhs));
}
$inline_always math_Vec2f math_Vec2f_subScalar(math_Vec2f lhs, f32 rhs) {
    return math_Vec2f_sub(lhs, math_Vec2f_fill(rhs));
}
$inline_always math_Vec2f math_Vec2f_mulScalar(math_Vec2f lhs, f32 rhs) {
    return math_Vec2f_mul(lhs, math_Vec2f_fill(rhs));
}
$inline_always math_Vec2f math_Vec2f_divScalar(math_Vec2f lhs, f32 rhs) {
    return math_Vec2f_div(lhs, math_Vec2f_fill(rhs));
}
$inline_always math_Vec2f math_Vec2f_modScalar(math_Vec2f lhs, f32 rhs) {
    return math_Vec2f_mod(lhs, math_Vec2f_fill(rhs));
}

$inline_always math_Vec2f* math_Vec2f_addAsg(math_Vec2f* lhs, math_Vec2f rhs) {
    return *lhs = math_Vec2f_add(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_subAsg(math_Vec2f* lhs, math_Vec2f rhs) {
    return *lhs = math_Vec2f_sub(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_mulAsg(math_Vec2f* lhs, math_Vec2f rhs) {
    return *lhs = math_Vec2f_mul(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_divAsg(math_Vec2f* lhs, math_Vec2f rhs) {
    return *lhs = math_Vec2f_div(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_modAsg(math_Vec2f* lhs, math_Vec2f rhs) {
    return *lhs = math_Vec2f_mod(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_scaleAsg(math_Vec2f* v, f32 scalar) {
    return *v = math_Vec2f_scale(*v, scalar), v;
}
$inline_always math_Vec2f* math_Vec2f_scaleInvAsg(math_Vec2f* v, f32 scalar) {
    return *v = math_Vec2f_scaleInv(*v, scalar), v;
}

$inline_always math_Vec2f* math_Vec2f_addScalarAsg(math_Vec2f* lhs, f32 rhs) {
    return *lhs = math_Vec2f_addScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_subScalarAsg(math_Vec2f* lhs, f32 rhs) {
    return *lhs = math_Vec2f_subScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_mulScalarAsg(math_Vec2f* lhs, f32 rhs) {
    return *lhs = math_Vec2f_mulScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_divScalarAsg(math_Vec2f* lhs, f32 rhs) {
    return *lhs = math_Vec2f_divScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2f* math_Vec2f_modScalarAsg(math_Vec2f* lhs, f32 rhs) {
    return *lhs = math_Vec2f_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$inline_always math_Vec2f math_Vec2f_min(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always math_Vec2f math_Vec2f_max(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always math_Vec2f math_Vec2f_clamp(math_Vec2f v, math_Vec2f min, math_Vec2f max) {
    return math_Vec2f_min(math_Vec2f_max(v, min), max);
}
$inline_always math_Vec2f math_Vec2f_clamp01(math_Vec2f v) {
    return math_Vec2f_clamp(v, math_Vec2f_zero, math_Vec2f_one);
}
$inline_always math_Vec2f math_Vec2f_wrap(math_Vec2f v, math_Vec2f min, math_Vec2f max) {
    return math_Vec2f_from(math_wrap(v.x, min.x, max.x), math_wrap(v.y, min.y, max.y));
}
$inline_always math_Vec2f math_Vec2f_wrap01(math_Vec2f v) {
    return math_Vec2f_wrap(v, math_Vec2f_zero, math_Vec2f_one);
}

/* Geometric Operations */
$inline_always f32 math_Vec2f_lenSq(math_Vec2f v) {
    return math_Vec2f_dot(v, v);
}
$inline_always f32 math_Vec2f_len(math_Vec2f v) {
    return math_sqrt(math_Vec2f_lenSq(v));
}
$inline_always f32 math_Vec2f_distSq(math_Vec2f lhs, math_Vec2f rhs) {
    return math_Vec2f_lenSq(math_Vec2f_sub(lhs, rhs));
}
$inline_always f32 math_Vec2f_dist(math_Vec2f lhs, math_Vec2f rhs) {
    return math_sqrt(math_Vec2f_distSq(lhs, rhs));
}
$inline_always f32 math_Vec2f_dot(math_Vec2f lhs, math_Vec2f rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always f32 math_Vec2f_cross(math_Vec2f lhs, math_Vec2f rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always math_Vec2f math_Vec2f_norm(math_Vec2f v) {
    const f32 len = math_Vec2f_len(v);
    return len <= 0.0f ? math_Vec2f_zero : math_Vec2f_scale(v, 1.0f / len);
}
$inline_always math_Vec2f math_Vec2f_project(math_Vec2f v, math_Vec2f onto) {
    return math_Vec2f_scale(onto, math_Vec2f_dot(v, onto) / math_Vec2f_lenSq(onto));
}
$inline_always math_Vec2f math_Vec2f_reject(math_Vec2f v, math_Vec2f from) {
    return math_Vec2f_sub(v, math_Vec2f_project(v, from));
}
$inline_always math_Vec2f math_Vec2f_reflect(math_Vec2f v, math_Vec2f normal) {
    return math_Vec2f_sub(v, math_Vec2f_scale(normal, 2.0f * math_Vec2f_dot(v, normal)));
}
$inline_always math_Vec2f math_Vec2f_rotate(math_Vec2f v, f32 radians) {
    const math_Vec2f r = math_Vec2f_sincos(radians);
    return math_Vec2f_from(v.x * r.x - v.y * r.y, v.x * r.y + v.y * r.x);
}
$inline_always math_Vec2f math_Vec2f_perp(math_Vec2f v) {
    return math_Vec2f_from(-v.y, v.x);
}

/* Vec3f functions */
/* Construction */
$inline_always math_Vec3f math_Vec3f_from(f32 x, f32 y, f32 z) {
    return (math_Vec3f){ .x = x, .y = y, .z = z };
}
$inline_always math_Vec3f math_Vec3f_from2(math_Vec2f v) {
    return math_Vec3f_from(v.x, v.y, 0.0f);
}
$inline_always math_Vec3f math_Vec3f_from4(math_Vec4f v) {
    return math_Vec3f_from(v.x, v.y, v.z);
}
$inline_always math_Vec3f math_Vec3f_fill(f32 scalar) {
    return math_Vec3f_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3f) {
    for (usize i = 0; i < 3; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(math_Vec3f) {
    for (usize i = 0; i < 3; ++i) {
        // Calculate absolute difference
        let diff = fabsf(self.s[i] - other.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec3f math_Vec3f_neg(math_Vec3f v) {
    return math_Vec3f_from(-v.x, -v.y, -v.z);
}
$inline_always math_Vec3f math_Vec3f_add(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always math_Vec3f math_Vec3f_sub(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always math_Vec3f math_Vec3f_mul(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always math_Vec3f math_Vec3f_div(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always math_Vec3f math_Vec3f_mod(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y), math_mod(lhs.z, rhs.z));
}
$inline_always math_Vec3f math_Vec3f_scale(math_Vec3f v, f32 scalar) {
    return math_Vec3f_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always math_Vec3f math_Vec3f_min(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3f math_Vec3f_max(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3f math_Vec3f_clamp(math_Vec3f v, math_Vec3f min, math_Vec3f max) {
    return math_Vec3f_min(math_Vec3f_max(v, min), max);
}
$inline_always math_Vec3f math_Vec3f_clamp01(math_Vec3f v) {
    return math_Vec3f_clamp(v, math_Vec3f_zero, math_Vec3f_one);
}
$inline_always math_Vec3f math_Vec3f_wrap(math_Vec3f v, math_Vec3f min, math_Vec3f max) {
    return math_Vec3f_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$inline_always math_Vec3f math_Vec3f_wrap01(math_Vec3f v) {
    return math_Vec3f_wrap(v, math_Vec3f_zero, math_Vec3f_one);
}

/* Geometric Operations */
$inline_always f32 math_Vec3f_lenSq(math_Vec3f v) {
    return math_Vec3f_dot(v, v);
}
$inline_always f32 math_Vec3f_len(math_Vec3f v) {
    return math_sqrt(math_Vec3f_lenSq(v));
}
$inline_always f32 math_Vec3f_distSq(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_lenSq(math_Vec3f_sub(lhs, rhs));
}
$inline_always f32 math_Vec3f_dist(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_len(math_Vec3f_sub(lhs, rhs));
}
$inline_always f32 math_Vec3f_dot(math_Vec3f lhs, math_Vec3f rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always math_Vec3f math_Vec3f_cross(math_Vec3f lhs, math_Vec3f rhs) {
    return math_Vec3f_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always math_Vec3f math_Vec3f_norm(math_Vec3f v) {
    const f32 len = math_Vec3f_len(v);
    return len <= 0.0f ? math_Vec3f_zero : math_Vec3f_scale(v, 1.0f / len);
}
$inline_always math_Vec3f math_Vec3f_project(math_Vec3f v, math_Vec3f onto) {
    return math_Vec3f_scale(onto, math_Vec3f_dot(v, onto) / math_Vec3f_lenSq(onto));
}
$inline_always math_Vec3f math_Vec3f_reject(math_Vec3f v, math_Vec3f from) {
    return math_Vec3f_sub(v, math_Vec3f_project(v, from));
}
$inline_always math_Vec3f math_Vec3f_reflect(math_Vec3f v, math_Vec3f normal) {
    return math_Vec3f_sub(v, math_Vec3f_scale(normal, 2.0f * math_Vec3f_dot(v, normal)));
}
$inline_always math_Vec3f math_Vec3f_rotate(const math_Vec3f v, const math_Vec3f axis, const f32 angle) {
    const math_Vec3f normalized_axis = math_Vec3f_norm(axis);
    const math_Vec2f r               = math_Vec2f_sincos(angle);
    const f32        one_sub_cos     = 1.0f - r.x;
    return math_Vec3f_add(
        math_Vec3f_add(
            math_Vec3f_scale(v, r.x),
            math_Vec3f_scale(math_Vec3f_cross(normalized_axis, v), r.y)
        ),
        math_Vec3f_scale(
            normalized_axis,
            math_Vec3f_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$inline_always math_Vec3f math_Vec3f_perp(const math_Vec3f v) {
    return v.x != 0.0f || v.y != 0.0f
             ? math_Vec3f_from(-v.y, v.x, 0.0f)
             : math_Vec3f_from(0.0f, -v.z, v.y);
}

/* Vec4f functions */
/* Construction */
$inline_always math_Vec4f math_Vec4f_from(f32 x, f32 y, f32 z, f32 w) {
    return (math_Vec4f){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always math_Vec4f math_Vec4f_fill(f32 scalar) {
    return math_Vec4f_from(scalar, scalar, scalar, scalar);
}
$inline_always math_Vec4f math_Vec4f_from2(math_Vec2f v) {
    return math_Vec4f_from(v.x, v.y, 0.0f, 0.0f);
}
$inline_always math_Vec4f math_Vec4f_from3(math_Vec3f v) {
    return math_Vec4f_from(v.x, v.y, v.z, 0.0f);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4f) {
    for (usize i = 0; i < 4; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(math_Vec4f) {
    for (usize i = 0; i < 4; ++i) {
        // Calculate absolute difference
        let diff = fabsf(self.s[i] - other.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec4f math_Vec4f_neg(math_Vec4f v) {
    return math_Vec4f_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always math_Vec4f math_Vec4f_add(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always math_Vec4f math_Vec4f_sub(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always math_Vec4f math_Vec4f_mul(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always math_Vec4f math_Vec4f_div(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always math_Vec4f math_Vec4f_mod(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4f math_Vec4f_scale(math_Vec4f v, f32 scalar) {
    return math_Vec4f_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always math_Vec4f math_Vec4f_min(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4f math_Vec4f_max(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4f math_Vec4f_clamp(math_Vec4f v, math_Vec4f min, math_Vec4f max) {
    return math_Vec4f_min(math_Vec4f_max(v, min), max);
}
$inline_always math_Vec4f math_Vec4f_clamp01(math_Vec4f v) {
    return math_Vec4f_clamp(v, math_Vec4f_zero, math_Vec4f_one);
}
$inline_always math_Vec4f math_Vec4f_wrap(math_Vec4f v, math_Vec4f min, math_Vec4f max) {
    return math_Vec4f_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$inline_always math_Vec4f math_Vec4f_wrap01(math_Vec4f v) {
    return math_Vec4f_wrap(v, math_Vec4f_zero, math_Vec4f_one);
}

/* Geometric Operations */
$inline_always f32 math_Vec4f_lenSq(math_Vec4f v) {
    return math_Vec4f_dot(v, v);
}
$inline_always f32 math_Vec4f_len(math_Vec4f v) {
    return math_sqrt(math_Vec4f_lenSq(v));
}
$inline_always f32 math_Vec4f_distSq(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_lenSq(math_Vec4f_sub(lhs, rhs));
}
$inline_always f32 math_Vec4f_dist(math_Vec4f lhs, math_Vec4f rhs) {
    return math_Vec4f_len(math_Vec4f_sub(lhs, rhs));
}
$inline_always f32 math_Vec4f_dot(math_Vec4f lhs, math_Vec4f rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$inline_always math_Vec4f math_Vec4f_norm(math_Vec4f v) {
    const f32 len = math_Vec4f_len(v);
    return len <= 0.0f ? math_Vec4f_zero : math_Vec4f_scale(v, 1.0f / len);
}
$inline_always math_Vec4f math_Vec4f_project(math_Vec4f v, math_Vec4f onto) {
    return math_Vec4f_scale(onto, math_Vec4f_dot(v, onto) / math_Vec4f_lenSq(onto));
}
$inline_always math_Vec4f math_Vec4f_reject(math_Vec4f v, math_Vec4f from) {
    return math_Vec4f_sub(v, math_Vec4f_project(v, from));
}
$inline_always math_Vec4f math_Vec4f_reflect(math_Vec4f v, math_Vec4f normal) {
    return math_Vec4f_sub(v, math_Vec4f_scale(normal, 2.0f * math_Vec4f_dot(v, normal)));
}

/* Vec2d functions */
/* Construction */
$inline_always math_Vec2d math_Vec2d_from(f64 x, f64 y) {
    return (math_Vec2d){ .x = x, .y = y };
}
$inline_always math_Vec2d math_Vec2d_from3(math_Vec3d v) {
    return math_Vec2d_from(v.x, v.y);
}
$inline_always math_Vec2d math_Vec2d_from4(math_Vec4d v) {
    return math_Vec2d_from(v.x, v.y);
}
$inline_always math_Vec2d math_Vec2d_fill(f64 scalar) {
    return math_Vec2d_from(scalar, scalar);
}
$inline_always math_Vec2d math_Vec2d_sincos(f64 radians) {
    return math_Vec2d_from(math_cos(radians), math_sin(radians));
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2d) {
    for (usize i = 0; i < 2; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec2d math_Vec2d_neg(math_Vec2d v) {
    return math_Vec2d_from(-v.x, -v.y);
}
$inline_always math_Vec2d math_Vec2d_add(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always math_Vec2d math_Vec2d_sub(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always math_Vec2d math_Vec2d_mul(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always math_Vec2d math_Vec2d_div(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always math_Vec2d math_Vec2d_mod(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always math_Vec2d math_Vec2d_scale(math_Vec2d v, f64 scalar) {
    return math_Vec2d_from(v.x * scalar, v.y * scalar);
}
$inline_always math_Vec2d math_Vec2d_scaleInv(math_Vec2d v, f64 scaler) {
    return math_Vec2d_from(v.x / scaler, v.y / scaler);
}

$inline_always math_Vec2d math_Vec2d_addScalar(math_Vec2d lhs, f64 rhs) {
    return math_Vec2d_add(lhs, math_Vec2d_fill(rhs));
}
$inline_always math_Vec2d math_Vec2d_subScalar(math_Vec2d lhs, f64 rhs) {
    return math_Vec2d_sub(lhs, math_Vec2d_fill(rhs));
}
$inline_always math_Vec2d math_Vec2d_mulScalar(math_Vec2d lhs, f64 rhs) {
    return math_Vec2d_mul(lhs, math_Vec2d_fill(rhs));
}
$inline_always math_Vec2d math_Vec2d_divScalar(math_Vec2d lhs, f64 rhs) {
    return math_Vec2d_div(lhs, math_Vec2d_fill(rhs));
}
$inline_always math_Vec2d math_Vec2d_modScalar(math_Vec2d lhs, f64 rhs) {
    return math_Vec2d_mod(lhs, math_Vec2d_fill(rhs));
}

$inline_always math_Vec2d* math_Vec2d_addAsg(math_Vec2d* lhs, math_Vec2d rhs) {
    return *lhs = math_Vec2d_add(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_subAsg(math_Vec2d* lhs, math_Vec2d rhs) {
    return *lhs = math_Vec2d_sub(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_mulAsg(math_Vec2d* lhs, math_Vec2d rhs) {
    return *lhs = math_Vec2d_mul(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_divAsg(math_Vec2d* lhs, math_Vec2d rhs) {
    return *lhs = math_Vec2d_div(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_modAsg(math_Vec2d* lhs, math_Vec2d rhs) {
    return *lhs = math_Vec2d_mod(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_scaleAsg(math_Vec2d* v, f64 scalar) {
    return *v = math_Vec2d_scale(*v, scalar), v;
}
$inline_always math_Vec2d* math_Vec2d_scaleInvAsg(math_Vec2d* v, f64 scalar) {
    return *v = math_Vec2d_scaleInv(*v, scalar), v;
}

$inline_always math_Vec2d* math_Vec2d_addScalarAsg(math_Vec2d* lhs, f64 rhs) {
    return *lhs = math_Vec2d_addScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_subScalarAsg(math_Vec2d* lhs, f64 rhs) {
    return *lhs = math_Vec2d_subScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_mulScalarAsg(math_Vec2d* lhs, f64 rhs) {
    return *lhs = math_Vec2d_mulScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_divScalarAsg(math_Vec2d* lhs, f64 rhs) {
    return *lhs = math_Vec2d_divScalar(*lhs, rhs), lhs;
}
$inline_always math_Vec2d* math_Vec2d_modScalarAsg(math_Vec2d* lhs, f64 rhs) {
    return *lhs = math_Vec2d_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$inline_always math_Vec2d math_Vec2d_min(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always math_Vec2d math_Vec2d_max(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always math_Vec2d math_Vec2d_clamp(math_Vec2d v, math_Vec2d min, math_Vec2d max) {
    return math_Vec2d_min(math_Vec2d_max(v, min), max);
}
$inline_always math_Vec2d math_Vec2d_clamp01(math_Vec2d v) {
    return math_Vec2d_clamp(v, math_Vec2d_zero, math_Vec2d_one);
}
$inline_always math_Vec2d math_Vec2d_wrap(math_Vec2d v, math_Vec2d min, math_Vec2d max) {
    return math_Vec2d_from(math_wrap(v.x, min.x, max.x), math_wrap(v.y, min.y, max.y));
}
$inline_always math_Vec2d math_Vec2d_wrap01(math_Vec2d v) {
    return math_Vec2d_wrap(v, math_Vec2d_zero, math_Vec2d_one);
}

/* Geometric Operations */
$inline_always f64 math_Vec2d_lenSq(math_Vec2d v) {
    return math_Vec2d_dot(v, v);
}
$inline_always f64 math_Vec2d_len(math_Vec2d v) {
    return math_sqrt(math_Vec2d_lenSq(v));
}
$inline_always f64 math_Vec2d_distSq(math_Vec2d lhs, math_Vec2d rhs) {
    return math_Vec2d_lenSq(math_Vec2d_sub(lhs, rhs));
}
$inline_always f64 math_Vec2d_dist(math_Vec2d lhs, math_Vec2d rhs) {
    return math_sqrt(math_Vec2d_distSq(lhs, rhs));
}
$inline_always f64 math_Vec2d_dot(math_Vec2d lhs, math_Vec2d rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always f64 math_Vec2d_cross(math_Vec2d lhs, math_Vec2d rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always math_Vec2d math_Vec2d_norm(math_Vec2d v) {
    const f64 len = math_Vec2d_len(v);
    return len <= 0.0 ? math_Vec2d_zero : math_Vec2d_scale(v, 1.0 / len);
}
$inline_always math_Vec2d math_Vec2d_project(math_Vec2d v, math_Vec2d onto) {
    return math_Vec2d_scale(onto, math_Vec2d_dot(v, onto) / math_Vec2d_lenSq(onto));
}
$inline_always math_Vec2d math_Vec2d_reject(math_Vec2d v, math_Vec2d from) {
    return math_Vec2d_sub(v, math_Vec2d_project(v, from));
}
$inline_always math_Vec2d math_Vec2d_reflect(math_Vec2d v, math_Vec2d normal) {
    return math_Vec2d_sub(v, math_Vec2d_scale(normal, 2.0 * math_Vec2d_dot(v, normal)));
}
$inline_always math_Vec2d math_Vec2d_rotate(math_Vec2d v, f64 radians) {
    const math_Vec2d r = math_Vec2d_sincos(radians);
    return math_Vec2d_from(
        v.x * r.x - v.y * r.y,
        v.x * r.y + v.y * r.x
    );
}
$inline_always math_Vec2d math_Vec2d_perp(math_Vec2d v) {
    return math_Vec2d_from(-v.y, v.x);
}

/* Vec3d functions */
/* Construction */
$inline_always math_Vec3d math_Vec3d_from(f64 x, f64 y, f64 z) {
    return (math_Vec3d){ .x = x, .y = y, .z = z };
}
$inline_always math_Vec3d math_Vec3d_from2(math_Vec2d v) {
    return math_Vec3d_from(v.x, v.y, 0.0);
}
$inline_always math_Vec3d math_Vec3d_from4(math_Vec4d v) {
    return math_Vec3d_from(v.x, v.y, v.z);
}
$inline_always math_Vec3d math_Vec3d_fill(f64 scalar) {
    return math_Vec3d_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3d) {
    for (usize i = 0; i < 3; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec3d math_Vec3d_neg(math_Vec3d v) {
    return math_Vec3d_from(-v.x, -v.y, -v.z);
}
$inline_always math_Vec3d math_Vec3d_add(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always math_Vec3d math_Vec3d_sub(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always math_Vec3d math_Vec3d_mul(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always math_Vec3d math_Vec3d_div(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always math_Vec3d math_Vec3d_mod(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3d math_Vec3d_scale(math_Vec3d v, f64 scalar) {
    return math_Vec3d_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always math_Vec3d math_Vec3d_min(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3d math_Vec3d_max(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3d math_Vec3d_clamp(math_Vec3d v, math_Vec3d min, math_Vec3d max) {
    return math_Vec3d_min(math_Vec3d_max(v, min), max);
}
$inline_always math_Vec3d math_Vec3d_clamp01(math_Vec3d v) {
    return math_Vec3d_clamp(v, math_Vec3d_zero, math_Vec3d_one);
}
$inline_always math_Vec3d math_Vec3d_wrap(math_Vec3d v, math_Vec3d min, math_Vec3d max) {
    return math_Vec3d_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$inline_always math_Vec3d math_Vec3d_wrap01(math_Vec3d v) {
    return math_Vec3d_wrap(v, math_Vec3d_zero, math_Vec3d_one);
}

/* Geometric Operations */
$inline_always f64 math_Vec3d_lenSq(math_Vec3d v) {
    return math_Vec3d_dot(v, v);
}
$inline_always f64 math_Vec3d_len(math_Vec3d v) {
    return math_sqrt(math_Vec3d_lenSq(v));
}
$inline_always f64 math_Vec3d_distSq(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_lenSq(math_Vec3d_sub(lhs, rhs));
}
$inline_always f64 math_Vec3d_dist(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_len(math_Vec3d_sub(lhs, rhs));
}
$inline_always f64 math_Vec3d_dot(math_Vec3d lhs, math_Vec3d rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always math_Vec3d math_Vec3d_cross(math_Vec3d lhs, math_Vec3d rhs) {
    return math_Vec3d_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always math_Vec3d math_Vec3d_norm(math_Vec3d v) {
    const f64 len = math_Vec3d_len(v);
    return len <= 0.0 ? math_Vec3d_zero : math_Vec3d_scale(v, 1.0 / len);
}
$inline_always math_Vec3d math_Vec3d_project(math_Vec3d v, math_Vec3d onto) {
    return math_Vec3d_scale(onto, math_Vec3d_dot(v, onto) / math_Vec3d_lenSq(onto));
}
$inline_always math_Vec3d math_Vec3d_reject(math_Vec3d v, math_Vec3d from) {
    return math_Vec3d_sub(v, math_Vec3d_project(v, from));
}
$inline_always math_Vec3d math_Vec3d_reflect(math_Vec3d v, math_Vec3d normal) {
    return math_Vec3d_sub(v, math_Vec3d_scale(normal, 2.0 * math_Vec3d_dot(v, normal)));
}
$inline_always math_Vec3d math_Vec3d_rotate(math_Vec3d v, math_Vec3d axis, f64 angle) {
    const math_Vec3d normalized_axis = math_Vec3d_norm(axis);
    const math_Vec2d r               = math_Vec2d_sincos(angle);
    const f64        one_sub_cos     = 1.0 - r.x;
    return math_Vec3d_add(
        math_Vec3d_add(
            math_Vec3d_scale(v, r.x),
            math_Vec3d_scale(math_Vec3d_cross(normalized_axis, v), r.y)
        ),
        math_Vec3d_scale(
            normalized_axis,
            math_Vec3d_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$inline_always math_Vec3d math_Vec3d_perp(math_Vec3d v) {
    return v.x != 0.0 || v.y != 0.0
             ? math_Vec3d_from(-v.y, v.x, 0.0)
             : math_Vec3d_from(0.0, -v.z, v.y);
}

/* Vec4d functions */
/* Construction */
$inline_always math_Vec4d math_Vec4d_from(f64 x, f64 y, f64 z, f64 w) {
    return (math_Vec4d){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always math_Vec4d math_Vec4d_fill(f64 scalar) {
    return math_Vec4d_from(scalar, scalar, scalar, scalar);
}
$inline_always math_Vec4d math_Vec4d_from2(math_Vec2d v) {
    return math_Vec4d_from(v.x, v.y, 0.0, 0.0);
}
$inline_always math_Vec4d math_Vec4d_from3(math_Vec3d v) {
    return math_Vec4d_from(v.x, v.y, v.z, 0.0);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4d) {
    for (usize i = 0; i < 4; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec4d math_Vec4d_neg(math_Vec4d v) {
    return math_Vec4d_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always math_Vec4d math_Vec4d_add(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always math_Vec4d math_Vec4d_sub(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always math_Vec4d math_Vec4d_mul(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always math_Vec4d math_Vec4d_div(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always math_Vec4d math_Vec4d_mod(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4d math_Vec4d_scale(math_Vec4d v, f64 scalar) {
    return math_Vec4d_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always math_Vec4d math_Vec4d_min(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4d math_Vec4d_max(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4d math_Vec4d_clamp(math_Vec4d v, math_Vec4d min, math_Vec4d max) {
    return math_Vec4d_min(math_Vec4d_max(v, min), max);
}
$inline_always math_Vec4d math_Vec4d_clamp01(math_Vec4d v) {
    return math_Vec4d_clamp(v, math_Vec4d_zero, math_Vec4d_one);
}
$inline_always math_Vec4d math_Vec4d_wrap(math_Vec4d v, math_Vec4d min, math_Vec4d max) {
    return math_Vec4d_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$inline_always math_Vec4d math_Vec4d_wrap01(math_Vec4d v) {
    return math_Vec4d_wrap(v, math_Vec4d_zero, math_Vec4d_one);
}

/* Geometric Operations */
$inline_always f64 math_Vec4d_lenSq(math_Vec4d v) {
    return math_Vec4d_dot(v, v);
}
$inline_always f64 math_Vec4d_len(math_Vec4d v) {
    return math_sqrt(math_Vec4d_lenSq(v));
}
$inline_always f64 math_Vec4d_distSq(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_lenSq(math_Vec4d_sub(lhs, rhs));
}
$inline_always f64 math_Vec4d_dist(math_Vec4d lhs, math_Vec4d rhs) {
    return math_Vec4d_len(math_Vec4d_sub(lhs, rhs));
}
$inline_always f64 math_Vec4d_dot(math_Vec4d lhs, math_Vec4d rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$inline_always math_Vec4d math_Vec4d_norm(math_Vec4d v) {
    const f64 len = math_Vec4d_len(v);
    return len <= 0.0 ? math_Vec4d_zero : math_Vec4d_scale(v, 1.0 / len);
}
$inline_always math_Vec4d math_Vec4d_project(math_Vec4d v, math_Vec4d onto) {
    return math_Vec4d_scale(onto, math_Vec4d_dot(v, onto) / math_Vec4d_lenSq(onto));
}
$inline_always math_Vec4d math_Vec4d_reject(math_Vec4d v, math_Vec4d from) {
    return math_Vec4d_sub(v, math_Vec4d_project(v, from));
}
$inline_always math_Vec4d math_Vec4d_reflect(math_Vec4d v, math_Vec4d normal) {
    return math_Vec4d_sub(v, math_Vec4d_scale(normal, 2.0 * math_Vec4d_dot(v, normal)));
}

/* Vec2i functions */
/* Construction */
$inline_always math_Vec2i math_Vec2i_from(i32 x, i32 y) {
    return (math_Vec2i){ .x = x, .y = y };
}
$inline_always math_Vec2i math_Vec2i_from3(math_Vec3i v) {
    return math_Vec2i_from(v.x, v.y);
}
$inline_always math_Vec2i math_Vec2i_from4(math_Vec4i v) {
    return math_Vec2i_from(v.x, v.y);
}
$inline_always math_Vec2i math_Vec2i_fill(i32 scalar) {
    return math_Vec2i_from(scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2i) {
    for (usize i = 0; i < 2; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec2i math_Vec2i_neg(math_Vec2i v) {
    return math_Vec2i_from(-v.x, -v.y);
}
$inline_always math_Vec2i math_Vec2i_add(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always math_Vec2i math_Vec2i_sub(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always math_Vec2i math_Vec2i_mul(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always math_Vec2i math_Vec2i_div(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always math_Vec2i math_Vec2i_mod(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always math_Vec2i math_Vec2i_scale(math_Vec2i v, i32 scalar) {
    return math_Vec2i_from(v.x * scalar, v.y * scalar);
}

/* Range Operations */
$inline_always math_Vec2i math_Vec2i_min(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always math_Vec2i math_Vec2i_max(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always math_Vec2i math_Vec2i_clamp(math_Vec2i v, math_Vec2i min, math_Vec2i max) {
    return math_Vec2i_min(math_Vec2i_max(v, min), max);
}
$inline_always math_Vec2i math_Vec2i_wrap(math_Vec2i v, math_Vec2i min, math_Vec2i max) {
    return math_Vec2i_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$inline_always i32 math_Vec2i_lenSq(math_Vec2i v) {
    return math_Vec2i_dot(v, v);
}
$inline_always i32 math_Vec2i_distSq(math_Vec2i lhs, math_Vec2i rhs) {
    return math_Vec2i_lenSq(math_Vec2i_sub(lhs, rhs));
}
$inline_always i32 math_Vec2i_dot(math_Vec2i lhs, math_Vec2i rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always i32 math_Vec2i_cross(math_Vec2i lhs, math_Vec2i rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always math_Vec2i math_Vec2i_perp(math_Vec2i v) {
    return math_Vec2i_from(-v.y, v.x);
}

/* Vec3i functions */
/* Construction */
$inline_always math_Vec3i math_Vec3i_from(i32 x, i32 y, i32 z) {
    return (math_Vec3i){ .x = x, .y = y, .z = z };
}
$inline_always math_Vec3i math_Vec3i_from2(math_Vec2i v) {
    return math_Vec3i_from(v.x, v.y, 0);
}
$inline_always math_Vec3i math_Vec3i_from4(math_Vec4i v) {
    return math_Vec3i_from(v.x, v.y, v.z);
}
$inline_always math_Vec3i math_Vec3i_fill(i32 scalar) {
    return math_Vec3i_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3i) {
    for (usize i = 0; i < 3; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec3i math_Vec3i_neg(math_Vec3i v) {
    return math_Vec3i_from(-v.x, -v.y, -v.z);
}
$inline_always math_Vec3i math_Vec3i_add(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always math_Vec3i math_Vec3i_sub(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always math_Vec3i math_Vec3i_mul(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always math_Vec3i math_Vec3i_div(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always math_Vec3i math_Vec3i_mod(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3i math_Vec3i_scale(math_Vec3i v, i32 scalar) {
    return math_Vec3i_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always math_Vec3i math_Vec3i_min(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3i math_Vec3i_max(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3i math_Vec3i_clamp(math_Vec3i v, math_Vec3i min, math_Vec3i max) {
    return math_Vec3i_min(math_Vec3i_max(v, min), max);
}
$inline_always math_Vec3i math_Vec3i_wrap(math_Vec3i v, math_Vec3i min, math_Vec3i max) {
    return math_Vec3i_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$inline_always i32 math_Vec3i_lenSq(math_Vec3i v) {
    return math_Vec3i_dot(v, v);
}
$inline_always i32 math_Vec3i_distSq(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_lenSq(math_Vec3i_sub(lhs, rhs));
}
$inline_always i32 math_Vec3i_dot(math_Vec3i lhs, math_Vec3i rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always math_Vec3i math_Vec3i_cross(math_Vec3i lhs, math_Vec3i rhs) {
    return math_Vec3i_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always math_Vec3i math_Vec3i_perp(math_Vec3i v) {
    return v.x != 0 || v.y != 0
             ? math_Vec3i_from(-v.y, v.x, 0)
             : math_Vec3i_from(0, -v.z, v.y);
}

/* Vec4i functions */
/* Construction */
$inline_always math_Vec4i math_Vec4i_from(i32 x, i32 y, i32 z, i32 w) {
    return (math_Vec4i){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always math_Vec4i math_Vec4i_from2(math_Vec2i v) {
    return math_Vec4i_from(v.x, v.y, 0, 0);
}
$inline_always math_Vec4i math_Vec4i_from3(math_Vec3i v) {
    return math_Vec4i_from(v.x, v.y, v.z, 0);
}
$inline_always math_Vec4i math_Vec4i_fill(i32 scalar) {
    return math_Vec4i_from(scalar, scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4i) {
    for (usize i = 0; i < 4; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec4i math_Vec4i_neg(math_Vec4i v) {
    return math_Vec4i_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always math_Vec4i math_Vec4i_add(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always math_Vec4i math_Vec4i_sub(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always math_Vec4i math_Vec4i_mul(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always math_Vec4i math_Vec4i_div(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always math_Vec4i math_Vec4i_mod(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4i math_Vec4i_scale(math_Vec4i v, i32 scalar) {
    return math_Vec4i_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always math_Vec4i math_Vec4i_min(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4i math_Vec4i_max(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4i math_Vec4i_clamp(math_Vec4i v, math_Vec4i min, math_Vec4i max) {
    return math_Vec4i_min(math_Vec4i_max(v, min), max);
}
$inline_always math_Vec4i math_Vec4i_wrap(math_Vec4i v, math_Vec4i min, math_Vec4i max) {
    return math_Vec4i_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$inline_always i32 math_Vec4i_lenSq(math_Vec4i v) {
    return math_Vec4i_dot(v, v);
}
$inline_always i32 math_Vec4i_distSq(math_Vec4i lhs, math_Vec4i rhs) {
    return math_Vec4i_lenSq(math_Vec4i_sub(lhs, rhs));
}
$inline_always i32 math_Vec4i_dot(math_Vec4i lhs, math_Vec4i rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2l functions */
/* Construction */
$inline_always math_Vec2l math_Vec2l_from(i64 x, i64 y) {
    return (math_Vec2l){ .x = x, .y = y };
}
$inline_always math_Vec2l math_Vec2l_from3(math_Vec3l v) {
    return math_Vec2l_from(v.x, v.y);
}
$inline_always math_Vec2l math_Vec2l_from4(math_Vec4l v) {
    return math_Vec2l_from(v.x, v.y);
}
$inline_always math_Vec2l math_Vec2l_fill(i64 scalar) {
    return math_Vec2l_from(scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec2l) {
    for (usize i = 0; i < 2; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec2l math_Vec2l_neg(math_Vec2l v) {
    return math_Vec2l_from(-v.x, -v.y);
}
$inline_always math_Vec2l math_Vec2l_add(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always math_Vec2l math_Vec2l_sub(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always math_Vec2l math_Vec2l_mul(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always math_Vec2l math_Vec2l_div(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always math_Vec2l math_Vec2l_mod(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y)
    );
}
$inline_always math_Vec2l math_Vec2l_scale(math_Vec2l v, i64 scalar) {
    return math_Vec2l_from(v.x * scalar, v.y * scalar);
}

/* Range Operations */
$inline_always math_Vec2l math_Vec2l_min(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y)
    );
}
$inline_always math_Vec2l math_Vec2l_max(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y)
    );
}
$inline_always math_Vec2l math_Vec2l_clamp(math_Vec2l v, math_Vec2l min, math_Vec2l max) {
    return math_Vec2l_min(math_Vec2l_max(v, min), max);
}
$inline_always math_Vec2l math_Vec2l_wrap(math_Vec2l v, math_Vec2l min, math_Vec2l max) {
    return math_Vec2l_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$inline_always i64 math_Vec2l_lenSq(math_Vec2l v) {
    return math_Vec2l_dot(v, v);
}
$inline_always i64 math_Vec2l_distSq(math_Vec2l lhs, math_Vec2l rhs) {
    return math_Vec2l_lenSq(math_Vec2l_sub(lhs, rhs));
}
$inline_always i64 math_Vec2l_dot(math_Vec2l lhs, math_Vec2l rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always i64 math_Vec2l_cross(math_Vec2l lhs, math_Vec2l rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always math_Vec2l math_Vec2l_perp(math_Vec2l v) {
    return math_Vec2l_from(-v.y, v.x);
}

/* Vec3l functions */
/* Construction */
$inline_always math_Vec3l math_Vec3l_from(i64 x, i64 y, i64 z) {
    return (math_Vec3l){ .x = x, .y = y, .z = z };
}
$inline_always math_Vec3l math_Vec3l_from2(math_Vec2l v) {
    return math_Vec3l_from(v.x, v.y, 0);
}
$inline_always math_Vec3l math_Vec3l_from4(math_Vec4l v) {
    return math_Vec3l_from(v.x, v.y, v.z);
}
$inline_always math_Vec3l math_Vec3l_fill(i64 scalar) {
    return math_Vec3l_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec3l) {
    for (usize i = 0; i < 3; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec3l math_Vec3l_neg(math_Vec3l v) {
    return math_Vec3l_from(-v.x, -v.y, -v.z);
}
$inline_always math_Vec3l math_Vec3l_add(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always math_Vec3l math_Vec3l_sub(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always math_Vec3l math_Vec3l_mul(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always math_Vec3l math_Vec3l_div(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always math_Vec3l math_Vec3l_mod(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3l math_Vec3l_scale(math_Vec3l v, i64 scalar) {
    return math_Vec3l_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always math_Vec3l math_Vec3l_min(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3l math_Vec3l_max(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always math_Vec3l math_Vec3l_clamp(math_Vec3l v, math_Vec3l min, math_Vec3l max) {
    return math_Vec3l_min(math_Vec3l_max(v, min), max);
}
$inline_always math_Vec3l math_Vec3l_wrap(math_Vec3l v, math_Vec3l min, math_Vec3l max) {
    return math_Vec3l_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$inline_always i64 math_Vec3l_lenSq(math_Vec3l v) {
    return math_Vec3l_dot(v, v);
}
$inline_always i64 math_Vec3l_distSq(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_lenSq(math_Vec3l_sub(lhs, rhs));
}
$inline_always i64 math_Vec3l_dot(math_Vec3l lhs, math_Vec3l rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always math_Vec3l math_Vec3l_cross(math_Vec3l lhs, math_Vec3l rhs) {
    return math_Vec3l_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always math_Vec3l math_Vec3l_perp(math_Vec3l v) {
    return v.x != 0 || v.y != 0
             ? math_Vec3l_from(-v.y, v.x, 0)
             : math_Vec3l_from(0, -v.z, v.y);
}

/* Vec4l functions */
/* Construction */
$inline_always math_Vec4l math_Vec4l_from(i64 x, i64 y, i64 z, i64 w) {
    return (math_Vec4l){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always math_Vec4l math_Vec4l_from2(math_Vec2l v) {
    return math_Vec4l_from(v.x, v.y, 0, 0);
}
$inline_always math_Vec4l math_Vec4l_from3(math_Vec3l v) {
    return math_Vec4l_from(v.x, v.y, v.z, 0);
}
$inline_always math_Vec4l math_Vec4l_fill(i64 scalar) {
    return math_Vec4l_from(scalar, scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(math_Vec4l) {
    for (usize i = 0; i < 4; ++i) {
        if (self.s[i] < other.s[i]) {
            return cmp_Ord_lt;
        }
        if (self.s[i] > other.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$inline_always math_Vec4l math_Vec4l_neg(math_Vec4l v) {
    return math_Vec4l_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always math_Vec4l math_Vec4l_add(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always math_Vec4l math_Vec4l_sub(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always math_Vec4l math_Vec4l_mul(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always math_Vec4l math_Vec4l_div(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always math_Vec4l math_Vec4l_mod(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4l math_Vec4l_scale(math_Vec4l v, i64 scalar) {
    return math_Vec4l_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always math_Vec4l math_Vec4l_min(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4l math_Vec4l_max(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always math_Vec4l math_Vec4l_clamp(math_Vec4l v, math_Vec4l min, math_Vec4l max) {
    return math_Vec4l_min(math_Vec4l_max(v, min), max);
}
$inline_always math_Vec4l math_Vec4l_wrap(math_Vec4l v, math_Vec4l min, math_Vec4l max) {
    return math_Vec4l_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$inline_always i64 math_Vec4l_lenSq(math_Vec4l v) {
    return math_Vec4l_dot(v, v);
}
$inline_always i64 math_Vec4l_distSq(math_Vec4l lhs, math_Vec4l rhs) {
    return math_Vec4l_lenSq(math_Vec4l_sub(lhs, rhs));
}
$inline_always i64 math_Vec4l_dot(math_Vec4l lhs, math_Vec4l rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

#endif /* MATH_VEC_INCLUDED */
