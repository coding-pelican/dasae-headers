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
#define m_V2f32_zero      ((m_V2f32){ .x = 0.0f, .y = 0.0f })
#define m_V2f32_unit_x    ((m_V2f32){ .x = 1.0f, .y = 0.0f })
#define m_V2f32_unit_y    ((m_V2f32){ .x = 0.0f, .y = 1.0f })
#define m_V2f32_one       ((m_V2f32){ .x = 1.0f, .y = 1.0f })
#define m_V2f32_left      ((m_V2f32){ .x = -1.0f, .y = 0.0f })
#define m_V2f32_up        ((m_V2f32){ .x = 0.0f, .y = 1.0f })
#define m_V2f32_right     ((m_V2f32){ .x = 1.0f, .y = 0.0f })
#define m_V2f32_down      ((m_V2f32){ .x = 0.0f, .y = -1.0f })
#define m_V2f32_eps       ((m_V2f32){ .x = f32_eps, .y = f32_eps })
#define m_V2f32_nan       ((m_V2f32){ .x = f32_nan, .y = f32_nan })
#define m_V2f32_inf       ((m_V2f32){ .x = f32_inf, .y = f32_inf })
#define m_V2f32_limit_min ((m_V2f32){ .x = f32_limit_min, .y = f32_limit_min })
#define m_V2f32_limit_max ((m_V2f32){ .x = f32_limit_max, .y = f32_limit_max })

/* Construction */
#define literal_m_V2f32_from(f32_x, f32_y) ((m_V2f32){ .x = (f32_x), .y = (f32_y) })
$inline_always m_V2f32 m_V2f32_from(f32 x, f32 y);
$inline_always m_V2f32 m_V2f32_from3(m_V3f32 v);
$inline_always m_V2f32 m_V2f32_from4(m_V4f32 v);
$inline_always m_V2f32 m_V2f32_fill(f32 scalar);
$inline_always m_V2f32 m_V2f32_sincos(f32 radians);

/* Comparison */
$inline_always cmp_fnCmp(m_V2f32);
cmp_fnEq_default(m_V2f32);
cmp_fnNe_default(m_V2f32);
cmp_fnLt_default(m_V2f32);
cmp_fnGt_default(m_V2f32);
cmp_fnLe_default(m_V2f32);
cmp_fnGe_default(m_V2f32);

$inline_always cmp_fnCmpApx(m_V2f32);
cmp_fnEqApx_default(m_V2f32);
cmp_fnNeApx_default(m_V2f32);
cmp_fnLtApx_default(m_V2f32);
cmp_fnGtApx_default(m_V2f32);
cmp_fnLeApx_default(m_V2f32);
cmp_fnGeApx_default(m_V2f32);

/* Arithmetic */
$inline_always m_V2f32 m_V2f32_neg(m_V2f32 v);
$inline_always m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_mod(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_scale(m_V2f32 v, f32 scalar);
$inline_always m_V2f32 m_V2f32_scaleInv(m_V2f32 v, f32 scalar);

$inline_always m_V2f32 m_V2f32_addScalar(m_V2f32 lhs, f32 rhs);
$inline_always m_V2f32 m_V2f32_subScalar(m_V2f32 lhs, f32 rhs);
$inline_always m_V2f32 m_V2f32_mulScalar(m_V2f32 lhs, f32 rhs);
$inline_always m_V2f32 m_V2f32_divScalar(m_V2f32 lhs, f32 rhs);
$inline_always m_V2f32 m_V2f32_modScalar(m_V2f32 lhs, f32 rhs);

$inline_always m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs);
$inline_always m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs);
$inline_always m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs);
$inline_always m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs);
$inline_always m_V2f32* m_V2f32_modAsg(m_V2f32* lhs, m_V2f32 rhs);
$inline_always m_V2f32* m_V2f32_scaleAsg(m_V2f32* v, f32 scalar);
$inline_always m_V2f32* m_V2f32_scaleInvAsg(m_V2f32* v, f32 scalar);

$inline_always m_V2f32* m_V2f32_addScalarAsg(m_V2f32* lhs, f32 rhs);
$inline_always m_V2f32* m_V2f32_subScalarAsg(m_V2f32* lhs, f32 rhs);
$inline_always m_V2f32* m_V2f32_mulScalarAsg(m_V2f32* lhs, f32 rhs);
$inline_always m_V2f32* m_V2f32_divScalarAsg(m_V2f32* lhs, f32 rhs);
$inline_always m_V2f32* m_V2f32_modScalarAsg(m_V2f32* lhs, f32 rhs);

/* Range Operations */
$inline_always m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$inline_always m_V2f32 m_V2f32_clamp01(m_V2f32 v);
$inline_always m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$inline_always m_V2f32 m_V2f32_wrap01(m_V2f32 v);

/* Geometric Operations */
$inline_always f32     m_V2f32_lenSq(m_V2f32 v);
$inline_always f32     m_V2f32_len(m_V2f32 v);
$inline_always f32     m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs);
$inline_always f32     m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs);
$inline_always f32     m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs);
$inline_always f32     m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs);
$inline_always m_V2f32 m_V2f32_norm(m_V2f32 v);
$inline_always m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto);
$inline_always m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from);
$inline_always m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal);
$inline_always m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 angle);
$inline_always m_V2f32 m_V2f32_perp(m_V2f32 v);

/* Vec3f functions */
/* Constants */
#define m_V3f32_zero      ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_unit_x    ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_unit_y    ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_unit_z    ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_unit_xy   ((m_V3f32){ .x = 1.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_unit_yz   ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 1.0f })
#define m_V3f32_unit_zx   ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_one       ((m_V3f32){ .x = 1.0f, .y = 1.0f, .z = 1.0f })
#define m_V3f32_left      ((m_V3f32){ .x = -1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_up        ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_right     ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_down      ((m_V3f32){ .x = 0.0f, .y = -1.0f, .z = 0.0f })
#define m_V3f32_forward   ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_backward  ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = -1.0f })
#define m_V3f32_eps       ((m_V3f32){ .x = f32_eps, .y = f32_eps, .z = f32_eps })
#define m_V3f32_nan       ((m_V3f32){ .x = f32_nan, .y = f32_nan, .z = f32_nan })
#define m_V3f32_inf       ((m_V3f32){ .x = f32_inf, .y = f32_inf, .z = f32_inf })
#define m_V3f32_limit_min ((m_V3f32){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min })
#define m_V3f32_limit_max ((m_V3f32){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max })

/* Construction */
#define literal_m_V3f32_from(f32_x, f32_y, f32_z) ((m_V3f32){ .x = (f32_x), .y = (f32_y), .z = (f32_z) })
$inline_always m_V3f32 m_V3f32_from(f32 x, f32 y, f32 z);
$inline_always m_V3f32 m_V3f32_from2(m_V2f32 v);
$inline_always m_V3f32 m_V3f32_from4(m_V4f32 v);
$inline_always m_V3f32 m_V3f32_fill(f32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V3f32);
cmp_fnEq_default(m_V3f32);
cmp_fnNe_default(m_V3f32);
cmp_fnLt_default(m_V3f32);
cmp_fnGt_default(m_V3f32);
cmp_fnLe_default(m_V3f32);
cmp_fnGe_default(m_V3f32);

$inline_always cmp_fnCmpApx(m_V3f32);
cmp_fnEqApx_default(m_V3f32);
cmp_fnNeApx_default(m_V3f32);
cmp_fnLtApx_default(m_V3f32);
cmp_fnGtApx_default(m_V3f32);
cmp_fnLeApx_default(m_V3f32);
cmp_fnGeApx_default(m_V3f32);

/* Arithmetic */
$inline_always m_V3f32 m_V3f32_neg(m_V3f32 v);
$inline_always m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_mod(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_scale(m_V3f32 v, f32 scalar);

/* Range Operations */
$inline_always m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$inline_always m_V3f32 m_V3f32_clamp01(m_V3f32 v);
$inline_always m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$inline_always m_V3f32 m_V3f32_wrap01(m_V3f32 v);

/* Geometric Operations */
$inline_always f32     m_V3f32_lenSq(m_V3f32 v);
$inline_always f32     m_V3f32_len(m_V3f32 v);
$inline_always f32     m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs);
$inline_always f32     m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs);
$inline_always f32     m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs);
$inline_always m_V3f32 m_V3f32_norm(m_V3f32 v);
$inline_always m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto);
$inline_always m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from);
$inline_always m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal);
$inline_always m_V3f32 m_V3f32_rotate(m_V3f32 v, m_V3f32 axis, f32 angle);
$inline_always m_V3f32 m_V3f32_perp(m_V3f32 v);

/* Vec4f functions */
/* Constants */
#define m_V4f32_zero      ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_x    ((m_V4f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_y    ((m_V4f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_z    ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define m_V4f32_unit_w    ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f })
#define m_V4f32_one       ((m_V4f32){ .x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f })
#define m_V4f32_left      ((m_V4f32){ .x = -1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_up        ((m_V4f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_right     ((m_V4f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_down      ((m_V4f32){ .x = 0.0f, .y = -1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_forward   ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define m_V4f32_backward  ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 0.0f })
#define m_V4f32_eps       ((m_V4f32){ .x = f32_eps, .y = f32_eps, .z = f32_eps, .w = f32_eps })
#define m_V4f32_nan       ((m_V4f32){ .x = f32_nan, .y = f32_nan, .z = f32_nan, .w = f32_nan })
#define m_V4f32_inf       ((m_V4f32){ .x = f32_inf, .y = f32_inf, .z = f32_inf, .w = f32_inf })
#define m_V4f32_limit_min ((m_V4f32){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min, .w = f32_limit_min })
#define m_V4f32_limit_max ((m_V4f32){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max, .w = f32_limit_max })

/* Construction */
#define literal_m_V4f32_from(f32_x, f32_y, f32_z, f32_w) ((m_V3f32){ .x = (f32_x), .y = (f32_y), .z = (f32_z), .w = (f32_w) })
$inline_always m_V4f32 m_V4f32_from(f32 x, f32 y, f32 z, f32 w);
$inline_always m_V4f32 m_V4f32_from2(m_V2f32 v);
$inline_always m_V4f32 m_V4f32_from3(m_V3f32 v);
$inline_always m_V4f32 m_V4f32_fill(f32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V4f32);
cmp_fnEq_default(m_V4f32);
cmp_fnNe_default(m_V4f32);
cmp_fnLt_default(m_V4f32);
cmp_fnGt_default(m_V4f32);
cmp_fnLe_default(m_V4f32);
cmp_fnGe_default(m_V4f32);

$inline_always cmp_fnCmpApx(m_V4f32);
cmp_fnEqApx_default(m_V4f32);
cmp_fnNeApx_default(m_V4f32);
cmp_fnLtApx_default(m_V4f32);
cmp_fnGtApx_default(m_V4f32);
cmp_fnLeApx_default(m_V4f32);
cmp_fnGeApx_default(m_V4f32);

/* Arithmetic */
$inline_always m_V4f32 m_V4f32_neg(m_V4f32 v);
$inline_always m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_mod(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_scale(m_V4f32 v, f32 scalar);

/* Range Operations */
$inline_always m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$inline_always m_V4f32 m_V4f32_clamp01(m_V4f32 v);
$inline_always m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$inline_always m_V4f32 m_V4f32_wrap01(m_V4f32 v);

/* Geometric Operations */
$inline_always f32     m_V4f32_lenSq(m_V4f32 v);
$inline_always f32     m_V4f32_len(m_V4f32 v);
$inline_always f32     m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs);
$inline_always f32     m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs);
$inline_always f32     m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs);
$inline_always m_V4f32 m_V4f32_norm(m_V4f32 v);
$inline_always m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto);
$inline_always m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from);
$inline_always m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal);

/*========== Float64 (f64) Vector Functions =================================*/

/* Vec2d functions */
/* Constants */
#define m_V2f64_zero      ((m_V2f64){ .x = 0.0, .y = 0.0 })
#define m_V2f64_unit_x    ((m_V2f64){ .x = 1.0, .y = 0.0 })
#define m_V2f64_unit_y    ((m_V2f64){ .x = 0.0, .y = 1.0 })
#define m_V2f64_one       ((m_V2f64){ .x = 1.0, .y = 1.0 })
#define m_V2f64_left      ((m_V2f64){ .x = -1.0, .y = 0.0 })
#define m_V2f64_up        ((m_V2f64){ .x = 0.0, .y = 1.0 })
#define m_V2f64_right     ((m_V2f64){ .x = 1.0, .y = 0.0 })
#define m_V2f64_down      ((m_V2f64){ .x = 0.0, .y = -1.0 })
#define m_V2f64_eps       ((m_V2f64){ .x = f64_eps, .y = f64_eps })
#define m_V2f64_nan       ((m_V2f64){ .x = f64_nan, .y = f64_nan })
#define m_V2f64_inf       ((m_V2f64){ .x = f64_inf, .y = f64_inf })
#define m_V2f64_limit_min ((m_V2f64){ .x = f64_limit_min, .y = f64_limit_min })
#define m_V2f64_limit_max ((m_V2f64){ .x = f64_limit_max, .y = f64_limit_max })

/* Construction */
#define literal_m_V2f64_from(f64_x, f64_y) ((m_V2f64){ .x = (f64_x), .y = (f64_y) })
$inline_always m_V2f64 m_V2f64_from(f64 x, f64 y);
$inline_always m_V2f64 m_V2f64_from3(m_V3f64 v);
$inline_always m_V2f64 m_V2f64_from4(m_V4f64 v);
$inline_always m_V2f64 m_V2f64_fill(f64 scalar);
$inline_always m_V2f64 m_V2f64_sincos(f64 radians);

/* Comparison */
$inline_always cmp_fnCmp(m_V2f64);
cmp_fnEq_default(m_V2f64);
cmp_fnNe_default(m_V2f64);
cmp_fnLt_default(m_V2f64);
cmp_fnGt_default(m_V2f64);
cmp_fnLe_default(m_V2f64);
cmp_fnGe_default(m_V2f64);

/* Arithmetic */
$inline_always m_V2f64 m_V2f64_neg(m_V2f64 v);
$inline_always m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_mod(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_scale(m_V2f64 v, f64 scalar);
$inline_always m_V2f64 m_V2f64_scaleInv(m_V2f64 v, f64 scaler);

$inline_always m_V2f64 m_V2f64_addScalar(m_V2f64 lhs, f64 rhs);
$inline_always m_V2f64 m_V2f64_subScalar(m_V2f64 lhs, f64 rhs);
$inline_always m_V2f64 m_V2f64_mulScalar(m_V2f64 lhs, f64 rhs);
$inline_always m_V2f64 m_V2f64_divScalar(m_V2f64 lhs, f64 rhs);
$inline_always m_V2f64 m_V2f64_modScalar(m_V2f64 lhs, f64 rhs);

$inline_always m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs);
$inline_always m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs);
$inline_always m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs);
$inline_always m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs);
$inline_always m_V2f64* m_V2f64_modAsg(m_V2f64* lhs, m_V2f64 rhs);
$inline_always m_V2f64* m_V2f64_scaleAsg(m_V2f64* v, f64 scalar);
$inline_always m_V2f64* m_V2f64_scaleInvAsg(m_V2f64* v, f64 scalar);

$inline_always m_V2f64* m_V2f64_addScalarAsg(m_V2f64* lhs, f64 rhs);
$inline_always m_V2f64* m_V2f64_subScalarAsg(m_V2f64* lhs, f64 rhs);
$inline_always m_V2f64* m_V2f64_mulScalarAsg(m_V2f64* lhs, f64 rhs);
$inline_always m_V2f64* m_V2f64_divScalarAsg(m_V2f64* lhs, f64 rhs);
$inline_always m_V2f64* m_V2f64_modScalarAsg(m_V2f64* lhs, f64 rhs);

/* Range Operations */
$inline_always m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$inline_always m_V2f64 m_V2f64_clamp01(m_V2f64 v);
$inline_always m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$inline_always m_V2f64 m_V2f64_wrap01(m_V2f64 v);

/* Geometric Operations */
$inline_always f64     m_V2f64_lenSq(m_V2f64 v);
$inline_always f64     m_V2f64_len(m_V2f64 v);
$inline_always f64     m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs);
$inline_always f64     m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs);
$inline_always f64     m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs);
$inline_always f64     m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs);
$inline_always m_V2f64 m_V2f64_norm(m_V2f64 v);
$inline_always m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto);
$inline_always m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from);
$inline_always m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal);
$inline_always m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 angle);
$inline_always m_V2f64 m_V2f64_perp(m_V2f64 v);

/* Vec3d functions */
/* Constants */
#define m_V3f64_zero     ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_unit_x   ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_unit_y   ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_unit_z   ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_unit_xy  ((m_V3f64){ .x = 1.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_unit_yz  ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 1.0 })
#define m_V3f64_unit_zx  ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_one      ((m_V3f64){ .x = 1.0, .y = 1.0, .z = 1.0 })
#define m_V3f64_left     ((m_V3f64){ .x = -1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_up       ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_right    ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_down     ((m_V3f64){ .x = 0.0, .y = -1.0, .z = 0.0 })
#define m_V3f64_forward  ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_backward ((m_V3f64){ .x = 0.0, .y = 0.0, .z = -1.0 })
#define m_V3f64_eps      ((m_V3f64){ .x = f64_eps, .y = f64_eps, .z = f64_eps })
#define m_V3f64_nan      ((m_V3f64){ .x = f64_nan, .y = f64_nan, .z = f64_nan })
#define m_V3f64_inf      ((m_V3f64){ .x = f64_inf, .y = f64_inf, .z = f64_inf })

/* Construction */
#define literal_m_V3f64_from(f64_x, f64_y, f64_z) ((m_V3f64){ .x = (f64_x), .y = (f64_y), .z = (f64_z) })
$inline_always m_V3f64 m_V3f64_from(f64 x, f64 y, f64 z);
$inline_always m_V3f64 m_V3f64_from2(m_V2f64 v);
$inline_always m_V3f64 m_V3f64_from4(m_V4f64 v);
$inline_always m_V3f64 m_V3f64_fill(f64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V3f64);
cmp_fnEq_default(m_V3f64);
cmp_fnNe_default(m_V3f64);
cmp_fnLt_default(m_V3f64);
cmp_fnGt_default(m_V3f64);
cmp_fnLe_default(m_V3f64);
cmp_fnGe_default(m_V3f64);

/* Arithmetic */
$inline_always m_V3f64 m_V3f64_neg(m_V3f64 v);
$inline_always m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_mod(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_scale(m_V3f64 v, f64 scalar);

/* Range Operations */
$inline_always m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$inline_always m_V3f64 m_V3f64_clamp01(m_V3f64 v);
$inline_always m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$inline_always m_V3f64 m_V3f64_wrap01(m_V3f64 v);

/* Geometric Operations */
$inline_always f64     m_V3f64_lenSq(m_V3f64 v);
$inline_always f64     m_V3f64_len(m_V3f64 v);
$inline_always f64     m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs);
$inline_always f64     m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs);
$inline_always f64     m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs);
$inline_always m_V3f64 m_V3f64_norm(m_V3f64 v);
$inline_always m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto);
$inline_always m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from);
$inline_always m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal);
$inline_always m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle);
$inline_always m_V3f64 m_V3f64_perp(m_V3f64 v);

/* Vec4d functions */
/* Constants */
#define m_V4f64_zero      ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_x    ((m_V4f64){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_y    ((m_V4f64){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_z    ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define m_V4f64_unit_w    ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0 })
#define m_V4f64_one       ((m_V4f64){ .x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0 })
#define m_V4f64_left      ((m_V4f64){ .x = -1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_up        ((m_V4f64){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_right     ((m_V4f64){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_down      ((m_V4f64){ .x = 0.0, .y = -1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_forward   ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define m_V4f64_backward  ((m_V4f64){ .x = 0.0, .y = 0.0, .z = -1.0, .w = 0.0 })
#define m_V4f64_eps       ((m_V4f64){ .x = f64_eps, .y = f64_eps, .z = f64_eps, .w = f64_eps })
#define m_V4f64_nan       ((m_V4f64){ .x = f64_nan, .y = f64_nan, .z = f64_nan, .w = f64_nan })
#define m_V4f64_inf       ((m_V4f64){ .x = f64_inf, .y = f64_inf, .z = f64_inf, .w = f64_inf })
#define m_V4f64_limit_min ((m_V4f64){ .x = f64_limit_min, .y = f64_limit_min, .z = f64_limit_min, .w = f64_limit_min })
#define m_V4f64_limit_max ((m_V4f64){ .x = f64_limit_max, .y = f64_limit_max, .z = f64_limit_max, .w = f64_limit_max })

/* Construction */
#define literal_m_V4f64_from(f64_x, f64_y, f64_z, f64_w) ((m_V4f64){ .x = (f64_x), .y = (f64_y), .z = (f64_z), .w = (f64_w) })
$inline_always m_V4f64 m_V4f64_from(f64 x, f64 y, f64 z, f64 w);
$inline_always m_V4f64 m_V4f64_from2(m_V2f64 v);
$inline_always m_V4f64 m_V4f64_from3(m_V3f64 v);
$inline_always m_V4f64 m_V4f64_fill(f64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V4f64);
cmp_fnEq_default(m_V4f64);
cmp_fnNe_default(m_V4f64);
cmp_fnLt_default(m_V4f64);
cmp_fnGt_default(m_V4f64);
cmp_fnLe_default(m_V4f64);
cmp_fnGe_default(m_V4f64);

/* Arithmetic */
$inline_always m_V4f64 m_V4f64_neg(m_V4f64 v);
$inline_always m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_mod(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_scale(m_V4f64 v, f64 scalar);

/* Range Operations */
$inline_always m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$inline_always m_V4f64 m_V4f64_clamp01(m_V4f64 v);
$inline_always m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$inline_always m_V4f64 m_V4f64_wrap01(m_V4f64 v);

/* Geometric Operations */
$inline_always f64     m_V4f64_lenSq(m_V4f64 v);
$inline_always f64     m_V4f64_len(m_V4f64 v);
$inline_always f64     m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs);
$inline_always f64     m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs);
$inline_always f64     m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs);
$inline_always m_V4f64 m_V4f64_norm(m_V4f64 v);
$inline_always m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto);
$inline_always m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from);
$inline_always m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal);

/*========== Int32 (i32) Vector Functions ===================================*/

/* Vec2i functions */
/* Constants */
#define m_V2i32_zero      ((m_V2i32){ .x = 0, .y = 0 })
#define m_V2i32_unit_x    ((m_V2i32){ .x = 1, .y = 0 })
#define m_V2i32_unit_y    ((m_V2i32){ .x = 0, .y = 1 })
#define m_V2i32_one       ((m_V2i32){ .x = 1, .y = 1 })
#define m_V2i32_left      ((m_V2i32){ .x = -1, .y = 0 })
#define m_V2i32_up        ((m_V2i32){ .x = 0, .y = 1 })
#define m_V2i32_right     ((m_V2i32){ .x = 1, .y = 0 })
#define m_V2i32_down      ((m_V2i32){ .x = 0, .y = -1 })
#define m_V2i32_limit_min ((m_V2i32){ .x = i32_limit_min, .y = i32_limit_min })
#define m_V2i32_limit_max ((m_V2i32){ .x = i32_limit_max, .y = i32_limit_max })

/* Construction */
#define literal_m_V2i32_from(i32_x, i32_y) ((m_V2i32){ .x = (i32_x), .y = (i32_y) })
$inline_always m_V2i32 m_V2i32_from(i32 x, i32 y);
$inline_always m_V2i32 m_V2i32_from3(m_V3i32 v);
$inline_always m_V2i32 m_V2i32_from4(m_V4i32 v);
$inline_always m_V2i32 m_V2i32_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V2i32);
cmp_fnEq_default(m_V2i32);
cmp_fnNe_default(m_V2i32);
cmp_fnLt_default(m_V2i32);
cmp_fnGt_default(m_V2i32);
cmp_fnLe_default(m_V2i32);
cmp_fnGe_default(m_V2i32);

/* Arithmetic */
$inline_always m_V2i32 m_V2i32_neg(m_V2i32 v);
$inline_always m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_mod(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_scale(m_V2i32 v, i32 scalar);

/* Range Operations */
$inline_always m_V2i32 m_V2i32_min(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max);
$inline_always m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max);

/* Geometric Operations */
$inline_always i32     m_V2i32_lenSq(m_V2i32 v);
$inline_always i32     m_V2i32_distSq(m_V2i32 lhs, m_V2i32 rhs);
$inline_always i32     m_V2i32_dot(m_V2i32 lhs, m_V2i32 rhs);
$inline_always i32     m_V2i32_cross(m_V2i32 lhs, m_V2i32 rhs);
$inline_always m_V2i32 m_V2i32_perp(m_V2i32 v);

/* Vec3i functions */
/* Constants */
#define m_V3i32_zero      ((m_V3i32){ .x = 0, .y = 0, .z = 0 })
#define m_V3i32_unit_x    ((m_V3i32){ .x = 1, .y = 0, .z = 0 })
#define m_V3i32_unit_y    ((m_V3i32){ .x = 0, .y = 1, .z = 0 })
#define m_V3i32_unit_z    ((m_V3i32){ .x = 0, .y = 0, .z = 1 })
#define m_V3i32_unit_xy   ((m_V3i32){ .x = 1, .y = 1, .z = 0 })
#define m_V3i32_unit_yz   ((m_V3i32){ .x = 0, .y = 1, .z = 1 })
#define m_V3i32_unit_zx   ((m_V3i32){ .x = 1, .y = 0, .z = 1 })
#define m_V3i32_one       ((m_V3i32){ .x = 1, .y = 1, .z = 1 })
#define m_V3i32_left      ((m_V3i32){ .x = -1, .y = 0, .z = 0 })
#define m_V3i32_up        ((m_V3i32){ .x = 0, .y = 1, .z = 0 })
#define m_V3i32_right     ((m_V3i32){ .x = 1, .y = 0, .z = 0 })
#define m_V3i32_down      ((m_V3i32){ .x = 0, .y = -1, .z = 0 })
#define m_V3i32_forward   ((m_V3i32){ .x = 0, .y = 0, .z = 1 })
#define m_V3i32_backward  ((m_V3i32){ .x = 0, .y = 0, .z = -1 })
#define m_V3i32_limit_min ((m_V3i32){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min })
#define m_V3i32_limit_max ((m_V3i32){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max })

/* Construction */
#define literal_m_V3i32_from(i32_x, i32_y, i32_z) ((m_V3i32){ .x = (i32_x), .y = (i32_y), .z = (i32_z) })
$inline_always m_V3i32 m_V3i32_from(i32 x, i32 y, i32 z);
$inline_always m_V3i32 m_V3i32_from2(m_V2i32 v);
$inline_always m_V3i32 m_V3i32_from4(m_V4i32 v);
$inline_always m_V3i32 m_V3i32_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V3i32);
cmp_fnEq_default(m_V3i32);
cmp_fnNe_default(m_V3i32);
cmp_fnLt_default(m_V3i32);
cmp_fnGt_default(m_V3i32);
cmp_fnLe_default(m_V3i32);
cmp_fnGe_default(m_V3i32);

/* Arithmetic */
$inline_always m_V3i32 m_V3i32_neg(m_V3i32 v);
$inline_always m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_mod(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_scale(m_V3i32 v, i32 scalar);

/* Range Operations */
$inline_always m_V3i32 m_V3i32_min(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_max(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_clamp(m_V3i32 v, m_V3i32 min, m_V3i32 max);
$inline_always m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max);

/* Geometric Operations */
$inline_always i32     m_V3i32_lenSq(m_V3i32 v);
$inline_always i32     m_V3i32_distSq(m_V3i32 lhs, m_V3i32 rhs);
$inline_always i32     m_V3i32_dot(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_cross(m_V3i32 lhs, m_V3i32 rhs);
$inline_always m_V3i32 m_V3i32_perp(m_V3i32 v);

/* Vec4i functions */
/* Constants */
#define m_V4i32_zero      ((m_V4i32){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_unit_x    ((m_V4i32){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_unit_y    ((m_V4i32){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i32_unit_z    ((m_V4i32){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i32_unit_w    ((m_V4i32){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define m_V4i32_one       ((m_V4i32){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define m_V4i32_left      ((m_V4i32){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_up        ((m_V4i32){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i32_right     ((m_V4i32){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_down      ((m_V4i32){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define m_V4i32_forward   ((m_V4i32){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i32_backward  ((m_V4i32){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define m_V4i32_limit_min ((m_V4i32){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min, .w = i32_limit_min })
#define m_V4i32_limit_max ((m_V4i32){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max, .w = i32_limit_max })

/* Construction */
#define literal_m_V4i32_from(i32_x, i32_y, i32_z, i32_w) ((m_V4i32){ .x = (i32_x), .y = (i32_y), .z = (i32_z), .w = (i32_w) })
$inline_always m_V4i32 m_V4i32_from(i32 x, i32 y, i32 z, i32 w);
$inline_always m_V4i32 m_V4i32_from2(m_V2i32 v);
$inline_always m_V4i32 m_V4i32_from3(m_V3i32 v);
$inline_always m_V4i32 m_V4i32_fill(i32 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V4i32);
cmp_fnEq_default(m_V4i32);
cmp_fnNe_default(m_V4i32);
cmp_fnLt_default(m_V4i32);
cmp_fnGt_default(m_V4i32);
cmp_fnLe_default(m_V4i32);
cmp_fnGe_default(m_V4i32);

/* Arithmetic */
$inline_always m_V4i32 m_V4i32_neg(m_V4i32 v);
$inline_always m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_mod(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_scale(m_V4i32 v, i32 scalar);

/* Range Operations */
$inline_always m_V4i32 m_V4i32_min(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_max(m_V4i32 lhs, m_V4i32 rhs);
$inline_always m_V4i32 m_V4i32_clamp(m_V4i32 v, m_V4i32 min, m_V4i32 max);
$inline_always m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max);

/* Geometric Operations */
$inline_always i32 m_V4i32_lenSq(m_V4i32 v);
$inline_always i32 m_V4i32_distSq(m_V4i32 lhs, m_V4i32 rhs);
$inline_always i32 m_V4i32_dot(m_V4i32 lhs, m_V4i32 rhs);

/*========== Int64 (i64) Vector Functions ===================================*/

/* Vec2l functions */
/* Constants */
#define m_V2i64_zero      ((m_V2i64){ .x = 0, .y = 0 })
#define m_V2i64_unit_x    ((m_V2i64){ .x = 1, .y = 0 })
#define m_V2i64_unit_y    ((m_V2i64){ .x = 0, .y = 1 })
#define m_V2i64_one       ((m_V2i64){ .x = 1, .y = 1 })
#define m_V2i64_left      ((m_V2i64){ .x = -1, .y = 0 })
#define m_V2i64_up        ((m_V2i64){ .x = 0, .y = 1 })
#define m_V2i64_right     ((m_V2i64){ .x = 1, .y = 0 })
#define m_V2i64_down      ((m_V2i64){ .x = 0, .y = -1 })
#define m_V2i64_limit_min ((m_V2i64){ .x = i64_limit_min, .y = i64_limit_min })
#define m_V2i64_limit_max ((m_V2i64){ .x = i64_limit_max, .y = i64_limit_max })

/* Construction */
#define literal_m_V2i64_from(i64_x, i64_y) ((m_V2i64){ .x = (i64_x), .y = (i64_y) })
$inline_always m_V2i64 m_V2i64_from(i64 x, i64 y);
$inline_always m_V2i64 m_V2i64_from3(m_V3i64 v);
$inline_always m_V2i64 m_V2i64_from4(m_V4i64 v);
$inline_always m_V2i64 m_V2i64_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V2i64);
cmp_fnEq_default(m_V2i64);
cmp_fnNe_default(m_V2i64);
cmp_fnLt_default(m_V2i64);
cmp_fnGt_default(m_V2i64);
cmp_fnLe_default(m_V2i64);
cmp_fnGe_default(m_V2i64);

/* Arithmetic */
$inline_always m_V2i64 m_V2i64_neg(m_V2i64 v);
$inline_always m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_mod(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_scale(m_V2i64 v, i64 scalar);

/* Range Operations */
$inline_always m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max);
$inline_always m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max);

/* Geometric Operations */
$inline_always i64     m_V2i64_lenSq(m_V2i64 v);
$inline_always i64     m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs);
$inline_always i64     m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs);
$inline_always i64     m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs);
$inline_always m_V2i64 m_V2i64_perp(m_V2i64 v);

/* Vec3l functions */
/* Constants */
#define m_V3i64_zero      ((m_V3i64){ .x = 0, .y = 0, .z = 0 })
#define m_V3i64_unit_x    ((m_V3i64){ .x = 1, .y = 0, .z = 0 })
#define m_V3i64_unit_y    ((m_V3i64){ .x = 0, .y = 1, .z = 0 })
#define m_V3i64_unit_z    ((m_V3i64){ .x = 0, .y = 0, .z = 1 })
#define m_V3i64_unit_xy   ((m_V3i64){ .x = 1, .y = 1, .z = 0 })
#define m_V3i64_unit_yz   ((m_V3i64){ .x = 0, .y = 1, .z = 1 })
#define m_V3i64_unit_zx   ((m_V3i64){ .x = 1, .y = 0, .z = 1 })
#define m_V3i64_one       ((m_V3i64){ .x = 1, .y = 1, .z = 1 })
#define m_V3i64_left      ((m_V3i64){ .x = -1, .y = 0, .z = 0 })
#define m_V3i64_up        ((m_V3i64){ .x = 0, .y = 1, .z = 0 })
#define m_V3i64_right     ((m_V3i64){ .x = 1, .y = 0, .z = 0 })
#define m_V3i64_down      ((m_V3i64){ .x = 0, .y = -1, .z = 0 })
#define m_V3i64_forward   ((m_V3i64){ .x = 0, .y = 0, .z = 1 })
#define m_V3i64_backward  ((m_V3i64){ .x = 0, .y = 0, .z = -1 })
#define m_V3i64_limit_min ((m_V3i64){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min })
#define m_V3i64_limit_max ((m_V3i64){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max })

/* Construction */
#define literal_m_V3i64_from(i64_x, i64_y, i64_z) ((m_V3i64){ .x = (i64_x), .y = (i64_y), .z = (i64_z) })
$inline_always m_V3i64 m_V3i64_from(i64 x, i64 y, i64 z);
$inline_always m_V3i64 m_V3i64_from2(m_V2i64 v);
$inline_always m_V3i64 m_V3i64_from4(m_V4i64 v);
$inline_always m_V3i64 m_V3i64_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V3i64);
cmp_fnEq_default(m_V3i64);
cmp_fnNe_default(m_V3i64);
cmp_fnLt_default(m_V3i64);
cmp_fnGt_default(m_V3i64);
cmp_fnLe_default(m_V3i64);
cmp_fnGe_default(m_V3i64);

/* Arithmetic */
$inline_always m_V3i64 m_V3i64_neg(m_V3i64 v);
$inline_always m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_mod(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_scale(m_V3i64 v, i64 scalar);

/* Range Operations */
$inline_always m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max);
$inline_always m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max);

/* Geometric Operations */
$inline_always i64     m_V3i64_lenSq(m_V3i64 v);
$inline_always i64     m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs);
$inline_always i64     m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs);
$inline_always m_V3i64 m_V3i64_perp(m_V3i64 v);

/* Vec4l functions */
/* Constants */
#define m_V4i64_zero      ((m_V4i64){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_unit_x    ((m_V4i64){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_unit_y    ((m_V4i64){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i64_unit_z    ((m_V4i64){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i64_unit_w    ((m_V4i64){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define m_V4i64_one       ((m_V4i64){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define m_V4i64_left      ((m_V4i64){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_up        ((m_V4i64){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i64_right     ((m_V4i64){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_down      ((m_V4i64){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define m_V4i64_forward   ((m_V4i64){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i64_backward  ((m_V4i64){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define m_V4i64_limit_min ((m_V4i64){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min, .w = i64_limit_min })
#define m_V4i64_limit_max ((m_V4i64){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max, .w = i64_limit_max })

/* Construction */
#define literal_m_V4i64_from(i64_x, i64_y, i64_z, i64_w) ((m_V4i64){ .x = (i64_x), .y = (i64_y), .z = (i64_z), .w = (i64_w) })
$inline_always m_V4i64 m_V4i64_from(i64 x, i64 y, i64 z, i64 w);
$inline_always m_V4i64 m_V4i64_from2(m_V2i64 v);
$inline_always m_V4i64 m_V4i64_from3(m_V3i64 v);
$inline_always m_V4i64 m_V4i64_fill(i64 scalar);

/* Comparison */
$inline_always cmp_fnCmp(m_V4i64);
cmp_fnEq_default(m_V4i64);
cmp_fnNe_default(m_V4i64);
cmp_fnLt_default(m_V4i64);
cmp_fnGt_default(m_V4i64);
cmp_fnLe_default(m_V4i64);
cmp_fnGe_default(m_V4i64);

/* Arithmetic */
$inline_always m_V4i64 m_V4i64_neg(m_V4i64 v);
$inline_always m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_mod(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_scale(m_V4i64 v, i64 scalar);

/* Range Operations */
$inline_always m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs);
$inline_always m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max);
$inline_always m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max);

/* Geometric Operations */
$inline_always i64 m_V4i64_lenSq(m_V4i64 v);
$inline_always i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs);
$inline_always i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs);

/*========== Implementations ================================================*/

/* Vec2f functions */
/* Construction */
$inline_always m_V2f32 m_V2f32_from(f32 x, f32 y) {
    return (m_V2f32){ .x = x, .y = y };
}
$inline_always m_V2f32 m_V2f32_from3(m_V3f32 v) {
    return m_V2f32_from(v.x, v.y);
}
$inline_always m_V2f32 m_V2f32_from4(m_V4f32 v) {
    return m_V2f32_from(v.x, v.y);
}
$inline_always m_V2f32 m_V2f32_fill(f32 scalar) {
    return m_V2f32_from(scalar, scalar);
}
$inline_always m_V2f32 m_V2f32_sincos(f32 radians) {
    return m_V2f32_from(math_cos(radians), math_sin(radians));
}

/* Comparison */
$inline_always cmp_fnCmp(m_V2f32) {
    for (usize i = 0; i < 2; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(m_V2f32) {
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
$inline_always m_V2f32 m_V2f32_neg(m_V2f32 v) {
    return m_V2f32_from(-v.x, -v.y);
}
$inline_always m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always m_V2f32 m_V2f32_mod(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always m_V2f32 m_V2f32_scale(m_V2f32 v, f32 scalar) {
    return m_V2f32_from(v.x * scalar, v.y * scalar);
}
$inline_always m_V2f32 m_V2f32_scaleInv(m_V2f32 v, f32 scalar) {
    return m_V2f32_from(v.x / scalar, v.y / scalar);
}

$inline_always m_V2f32 m_V2f32_addScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_add(lhs, m_V2f32_fill(rhs));
}
$inline_always m_V2f32 m_V2f32_subScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_sub(lhs, m_V2f32_fill(rhs));
}
$inline_always m_V2f32 m_V2f32_mulScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mul(lhs, m_V2f32_fill(rhs));
}
$inline_always m_V2f32 m_V2f32_divScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_div(lhs, m_V2f32_fill(rhs));
}
$inline_always m_V2f32 m_V2f32_modScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mod(lhs, m_V2f32_fill(rhs));
}

$inline_always m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_add(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_sub(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_mul(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_div(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_modAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_mod(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_scaleAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scale(*v, scalar), v;
}
$inline_always m_V2f32* m_V2f32_scaleInvAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scaleInv(*v, scalar), v;
}

$inline_always m_V2f32* m_V2f32_addScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_addScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_subScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_subScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_mulScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_mulScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_divScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_divScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f32* m_V2f32_modScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$inline_always m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
    return m_V2f32_min(m_V2f32_max(v, min), max);
}
$inline_always m_V2f32 m_V2f32_clamp01(m_V2f32 v) {
    return m_V2f32_clamp(v, m_V2f32_zero, m_V2f32_one);
}
$inline_always m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
    return m_V2f32_from(math_wrap(v.x, min.x, max.x), math_wrap(v.y, min.y, max.y));
}
$inline_always m_V2f32 m_V2f32_wrap01(m_V2f32 v) {
    return m_V2f32_wrap(v, m_V2f32_zero, m_V2f32_one);
}

/* Geometric Operations */
$inline_always f32 m_V2f32_lenSq(m_V2f32 v) {
    return m_V2f32_dot(v, v);
}
$inline_always f32 m_V2f32_len(m_V2f32 v) {
    return math_sqrt(m_V2f32_lenSq(v));
}
$inline_always f32 m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_lenSq(m_V2f32_sub(lhs, rhs));
}
$inline_always f32 m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs) {
    return math_sqrt(m_V2f32_distSq(lhs, rhs));
}
$inline_always f32 m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always m_V2f32 m_V2f32_norm(m_V2f32 v) {
    const f32 len = m_V2f32_len(v);
    return len <= 0.0f ? m_V2f32_zero : m_V2f32_scale(v, 1.0f / len);
}
$inline_always m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto) {
    return m_V2f32_scale(onto, m_V2f32_dot(v, onto) / m_V2f32_lenSq(onto));
}
$inline_always m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from) {
    return m_V2f32_sub(v, m_V2f32_project(v, from));
}
$inline_always m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal) {
    return m_V2f32_sub(v, m_V2f32_scale(normal, 2.0f * m_V2f32_dot(v, normal)));
}
$inline_always m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 radians) {
    const m_V2f32 r = m_V2f32_sincos(radians);
    return m_V2f32_from(v.x * r.x - v.y * r.y, v.x * r.y + v.y * r.x);
}
$inline_always m_V2f32 m_V2f32_perp(m_V2f32 v) {
    return m_V2f32_from(-v.y, v.x);
}

/* Vec3f functions */
/* Construction */
$inline_always m_V3f32 m_V3f32_from(f32 x, f32 y, f32 z) {
    return (m_V3f32){ .x = x, .y = y, .z = z };
}
$inline_always m_V3f32 m_V3f32_from2(m_V2f32 v) {
    return m_V3f32_from(v.x, v.y, 0.0f);
}
$inline_always m_V3f32 m_V3f32_from4(m_V4f32 v) {
    return m_V3f32_from(v.x, v.y, v.z);
}
$inline_always m_V3f32 m_V3f32_fill(f32 scalar) {
    return m_V3f32_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V3f32) {
    for (usize i = 0; i < 3; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(m_V3f32) {
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
$inline_always m_V3f32 m_V3f32_neg(m_V3f32 v) {
    return m_V3f32_from(-v.x, -v.y, -v.z);
}
$inline_always m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always m_V3f32 m_V3f32_mod(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y), math_mod(lhs.z, rhs.z));
}
$inline_always m_V3f32 m_V3f32_scale(m_V3f32 v, f32 scalar) {
    return m_V3f32_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
    return m_V3f32_min(m_V3f32_max(v, min), max);
}
$inline_always m_V3f32 m_V3f32_clamp01(m_V3f32 v) {
    return m_V3f32_clamp(v, m_V3f32_zero, m_V3f32_one);
}
$inline_always m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
    return m_V3f32_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$inline_always m_V3f32 m_V3f32_wrap01(m_V3f32 v) {
    return m_V3f32_wrap(v, m_V3f32_zero, m_V3f32_one);
}

/* Geometric Operations */
$inline_always f32 m_V3f32_lenSq(m_V3f32 v) {
    return m_V3f32_dot(v, v);
}
$inline_always f32 m_V3f32_len(m_V3f32 v) {
    return math_sqrt(m_V3f32_lenSq(v));
}
$inline_always f32 m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_lenSq(m_V3f32_sub(lhs, rhs));
}
$inline_always f32 m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_len(m_V3f32_sub(lhs, rhs));
}
$inline_always f32 m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always m_V3f32 m_V3f32_norm(m_V3f32 v) {
    const f32 len = m_V3f32_len(v);
    return len <= 0.0f ? m_V3f32_zero : m_V3f32_scale(v, 1.0f / len);
}
$inline_always m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto) {
    return m_V3f32_scale(onto, m_V3f32_dot(v, onto) / m_V3f32_lenSq(onto));
}
$inline_always m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from) {
    return m_V3f32_sub(v, m_V3f32_project(v, from));
}
$inline_always m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal) {
    return m_V3f32_sub(v, m_V3f32_scale(normal, 2.0f * m_V3f32_dot(v, normal)));
}
$inline_always m_V3f32 m_V3f32_rotate(const m_V3f32 v, const m_V3f32 axis, const f32 angle) {
    const m_V3f32 normalized_axis = m_V3f32_norm(axis);
    const m_V2f32 r               = m_V2f32_sincos(angle);
    const f32     one_sub_cos     = 1.0f - r.x;
    return m_V3f32_add(
        m_V3f32_add(
            m_V3f32_scale(v, r.x),
            m_V3f32_scale(m_V3f32_cross(normalized_axis, v), r.y)
        ),
        m_V3f32_scale(
            normalized_axis,
            m_V3f32_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$inline_always m_V3f32 m_V3f32_perp(const m_V3f32 v) {
    return v.x != 0.0f || v.y != 0.0f
             ? m_V3f32_from(-v.y, v.x, 0.0f)
             : m_V3f32_from(0.0f, -v.z, v.y);
}

/* Vec4f functions */
/* Construction */
$inline_always m_V4f32 m_V4f32_from(f32 x, f32 y, f32 z, f32 w) {
    return (m_V4f32){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always m_V4f32 m_V4f32_fill(f32 scalar) {
    return m_V4f32_from(scalar, scalar, scalar, scalar);
}
$inline_always m_V4f32 m_V4f32_from2(m_V2f32 v) {
    return m_V4f32_from(v.x, v.y, 0.0f, 0.0f);
}
$inline_always m_V4f32 m_V4f32_from3(m_V3f32 v) {
    return m_V4f32_from(v.x, v.y, v.z, 0.0f);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V4f32) {
    for (usize i = 0; i < 4; ++i) {
        if (self.s[i] < other.s[i]) { return cmp_Ord_lt; }
        if (self.s[i] > other.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$inline_always cmp_fnCmpApx(m_V4f32) {
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
$inline_always m_V4f32 m_V4f32_neg(m_V4f32 v) {
    return m_V4f32_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always m_V4f32 m_V4f32_mod(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always m_V4f32 m_V4f32_scale(m_V4f32 v, f32 scalar) {
    return m_V4f32_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
    return m_V4f32_min(m_V4f32_max(v, min), max);
}
$inline_always m_V4f32 m_V4f32_clamp01(m_V4f32 v) {
    return m_V4f32_clamp(v, m_V4f32_zero, m_V4f32_one);
}
$inline_always m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
    return m_V4f32_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$inline_always m_V4f32 m_V4f32_wrap01(m_V4f32 v) {
    return m_V4f32_wrap(v, m_V4f32_zero, m_V4f32_one);
}

/* Geometric Operations */
$inline_always f32 m_V4f32_lenSq(m_V4f32 v) {
    return m_V4f32_dot(v, v);
}
$inline_always f32 m_V4f32_len(m_V4f32 v) {
    return math_sqrt(m_V4f32_lenSq(v));
}
$inline_always f32 m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_lenSq(m_V4f32_sub(lhs, rhs));
}
$inline_always f32 m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_len(m_V4f32_sub(lhs, rhs));
}
$inline_always f32 m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$inline_always m_V4f32 m_V4f32_norm(m_V4f32 v) {
    const f32 len = m_V4f32_len(v);
    return len <= 0.0f ? m_V4f32_zero : m_V4f32_scale(v, 1.0f / len);
}
$inline_always m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto) {
    return m_V4f32_scale(onto, m_V4f32_dot(v, onto) / m_V4f32_lenSq(onto));
}
$inline_always m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from) {
    return m_V4f32_sub(v, m_V4f32_project(v, from));
}
$inline_always m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal) {
    return m_V4f32_sub(v, m_V4f32_scale(normal, 2.0f * m_V4f32_dot(v, normal)));
}

/* Vec2d functions */
/* Construction */
$inline_always m_V2f64 m_V2f64_from(f64 x, f64 y) {
    return (m_V2f64){ .x = x, .y = y };
}
$inline_always m_V2f64 m_V2f64_from3(m_V3f64 v) {
    return m_V2f64_from(v.x, v.y);
}
$inline_always m_V2f64 m_V2f64_from4(m_V4f64 v) {
    return m_V2f64_from(v.x, v.y);
}
$inline_always m_V2f64 m_V2f64_fill(f64 scalar) {
    return m_V2f64_from(scalar, scalar);
}
$inline_always m_V2f64 m_V2f64_sincos(f64 radians) {
    return m_V2f64_from(math_cos(radians), math_sin(radians));
}

/* Comparison */
$inline_always cmp_fnCmp(m_V2f64) {
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
$inline_always m_V2f64 m_V2f64_neg(m_V2f64 v) {
    return m_V2f64_from(-v.x, -v.y);
}
$inline_always m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always m_V2f64 m_V2f64_mod(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always m_V2f64 m_V2f64_scale(m_V2f64 v, f64 scalar) {
    return m_V2f64_from(v.x * scalar, v.y * scalar);
}
$inline_always m_V2f64 m_V2f64_scaleInv(m_V2f64 v, f64 scaler) {
    return m_V2f64_from(v.x / scaler, v.y / scaler);
}

$inline_always m_V2f64 m_V2f64_addScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_add(lhs, m_V2f64_fill(rhs));
}
$inline_always m_V2f64 m_V2f64_subScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_sub(lhs, m_V2f64_fill(rhs));
}
$inline_always m_V2f64 m_V2f64_mulScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mul(lhs, m_V2f64_fill(rhs));
}
$inline_always m_V2f64 m_V2f64_divScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_div(lhs, m_V2f64_fill(rhs));
}
$inline_always m_V2f64 m_V2f64_modScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mod(lhs, m_V2f64_fill(rhs));
}

$inline_always m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_add(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_sub(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_mul(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_div(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_modAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_mod(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_scaleAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scale(*v, scalar), v;
}
$inline_always m_V2f64* m_V2f64_scaleInvAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scaleInv(*v, scalar), v;
}

$inline_always m_V2f64* m_V2f64_addScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_addScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_subScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_subScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_mulScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_mulScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_divScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_divScalar(*lhs, rhs), lhs;
}
$inline_always m_V2f64* m_V2f64_modScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$inline_always m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
    return m_V2f64_min(m_V2f64_max(v, min), max);
}
$inline_always m_V2f64 m_V2f64_clamp01(m_V2f64 v) {
    return m_V2f64_clamp(v, m_V2f64_zero, m_V2f64_one);
}
$inline_always m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
    return m_V2f64_from(math_wrap(v.x, min.x, max.x), math_wrap(v.y, min.y, max.y));
}
$inline_always m_V2f64 m_V2f64_wrap01(m_V2f64 v) {
    return m_V2f64_wrap(v, m_V2f64_zero, m_V2f64_one);
}

/* Geometric Operations */
$inline_always f64 m_V2f64_lenSq(m_V2f64 v) {
    return m_V2f64_dot(v, v);
}
$inline_always f64 m_V2f64_len(m_V2f64 v) {
    return math_sqrt(m_V2f64_lenSq(v));
}
$inline_always f64 m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_lenSq(m_V2f64_sub(lhs, rhs));
}
$inline_always f64 m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs) {
    return math_sqrt(m_V2f64_distSq(lhs, rhs));
}
$inline_always f64 m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always m_V2f64 m_V2f64_norm(m_V2f64 v) {
    const f64 len = m_V2f64_len(v);
    return len <= 0.0 ? m_V2f64_zero : m_V2f64_scale(v, 1.0 / len);
}
$inline_always m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto) {
    return m_V2f64_scale(onto, m_V2f64_dot(v, onto) / m_V2f64_lenSq(onto));
}
$inline_always m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from) {
    return m_V2f64_sub(v, m_V2f64_project(v, from));
}
$inline_always m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal) {
    return m_V2f64_sub(v, m_V2f64_scale(normal, 2.0 * m_V2f64_dot(v, normal)));
}
$inline_always m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 radians) {
    const m_V2f64 r = m_V2f64_sincos(radians);
    return m_V2f64_from(
        v.x * r.x - v.y * r.y,
        v.x * r.y + v.y * r.x
    );
}
$inline_always m_V2f64 m_V2f64_perp(m_V2f64 v) {
    return m_V2f64_from(-v.y, v.x);
}

/* Vec3d functions */
/* Construction */
$inline_always m_V3f64 m_V3f64_from(f64 x, f64 y, f64 z) {
    return (m_V3f64){ .x = x, .y = y, .z = z };
}
$inline_always m_V3f64 m_V3f64_from2(m_V2f64 v) {
    return m_V3f64_from(v.x, v.y, 0.0);
}
$inline_always m_V3f64 m_V3f64_from4(m_V4f64 v) {
    return m_V3f64_from(v.x, v.y, v.z);
}
$inline_always m_V3f64 m_V3f64_fill(f64 scalar) {
    return m_V3f64_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V3f64) {
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
$inline_always m_V3f64 m_V3f64_neg(m_V3f64 v) {
    return m_V3f64_from(-v.x, -v.y, -v.z);
}
$inline_always m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always m_V3f64 m_V3f64_mod(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always m_V3f64 m_V3f64_scale(m_V3f64 v, f64 scalar) {
    return m_V3f64_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
    return m_V3f64_min(m_V3f64_max(v, min), max);
}
$inline_always m_V3f64 m_V3f64_clamp01(m_V3f64 v) {
    return m_V3f64_clamp(v, m_V3f64_zero, m_V3f64_one);
}
$inline_always m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
    return m_V3f64_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$inline_always m_V3f64 m_V3f64_wrap01(m_V3f64 v) {
    return m_V3f64_wrap(v, m_V3f64_zero, m_V3f64_one);
}

/* Geometric Operations */
$inline_always f64 m_V3f64_lenSq(m_V3f64 v) {
    return m_V3f64_dot(v, v);
}
$inline_always f64 m_V3f64_len(m_V3f64 v) {
    return math_sqrt(m_V3f64_lenSq(v));
}
$inline_always f64 m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_lenSq(m_V3f64_sub(lhs, rhs));
}
$inline_always f64 m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_len(m_V3f64_sub(lhs, rhs));
}
$inline_always f64 m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always m_V3f64 m_V3f64_norm(m_V3f64 v) {
    const f64 len = m_V3f64_len(v);
    return len <= 0.0 ? m_V3f64_zero : m_V3f64_scale(v, 1.0 / len);
}
$inline_always m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto) {
    return m_V3f64_scale(onto, m_V3f64_dot(v, onto) / m_V3f64_lenSq(onto));
}
$inline_always m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from) {
    return m_V3f64_sub(v, m_V3f64_project(v, from));
}
$inline_always m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal) {
    return m_V3f64_sub(v, m_V3f64_scale(normal, 2.0 * m_V3f64_dot(v, normal)));
}
$inline_always m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle) {
    const m_V3f64 normalized_axis = m_V3f64_norm(axis);
    const m_V2f64 r               = m_V2f64_sincos(angle);
    const f64     one_sub_cos     = 1.0 - r.x;
    return m_V3f64_add(
        m_V3f64_add(
            m_V3f64_scale(v, r.x),
            m_V3f64_scale(m_V3f64_cross(normalized_axis, v), r.y)
        ),
        m_V3f64_scale(
            normalized_axis,
            m_V3f64_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$inline_always m_V3f64 m_V3f64_perp(m_V3f64 v) {
    return v.x != 0.0 || v.y != 0.0
             ? m_V3f64_from(-v.y, v.x, 0.0)
             : m_V3f64_from(0.0, -v.z, v.y);
}

/* Vec4d functions */
/* Construction */
$inline_always m_V4f64 m_V4f64_from(f64 x, f64 y, f64 z, f64 w) {
    return (m_V4f64){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always m_V4f64 m_V4f64_fill(f64 scalar) {
    return m_V4f64_from(scalar, scalar, scalar, scalar);
}
$inline_always m_V4f64 m_V4f64_from2(m_V2f64 v) {
    return m_V4f64_from(v.x, v.y, 0.0, 0.0);
}
$inline_always m_V4f64 m_V4f64_from3(m_V3f64 v) {
    return m_V4f64_from(v.x, v.y, v.z, 0.0);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V4f64) {
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
$inline_always m_V4f64 m_V4f64_neg(m_V4f64 v) {
    return m_V4f64_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always m_V4f64 m_V4f64_mod(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always m_V4f64 m_V4f64_scale(m_V4f64 v, f64 scalar) {
    return m_V4f64_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
    return m_V4f64_min(m_V4f64_max(v, min), max);
}
$inline_always m_V4f64 m_V4f64_clamp01(m_V4f64 v) {
    return m_V4f64_clamp(v, m_V4f64_zero, m_V4f64_one);
}
$inline_always m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
    return m_V4f64_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$inline_always m_V4f64 m_V4f64_wrap01(m_V4f64 v) {
    return m_V4f64_wrap(v, m_V4f64_zero, m_V4f64_one);
}

/* Geometric Operations */
$inline_always f64 m_V4f64_lenSq(m_V4f64 v) {
    return m_V4f64_dot(v, v);
}
$inline_always f64 m_V4f64_len(m_V4f64 v) {
    return math_sqrt(m_V4f64_lenSq(v));
}
$inline_always f64 m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_lenSq(m_V4f64_sub(lhs, rhs));
}
$inline_always f64 m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_len(m_V4f64_sub(lhs, rhs));
}
$inline_always f64 m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$inline_always m_V4f64 m_V4f64_norm(m_V4f64 v) {
    const f64 len = m_V4f64_len(v);
    return len <= 0.0 ? m_V4f64_zero : m_V4f64_scale(v, 1.0 / len);
}
$inline_always m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto) {
    return m_V4f64_scale(onto, m_V4f64_dot(v, onto) / m_V4f64_lenSq(onto));
}
$inline_always m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from) {
    return m_V4f64_sub(v, m_V4f64_project(v, from));
}
$inline_always m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal) {
    return m_V4f64_sub(v, m_V4f64_scale(normal, 2.0 * m_V4f64_dot(v, normal)));
}

/* Vec2i functions */
/* Construction */
$inline_always m_V2i32 m_V2i32_from(i32 x, i32 y) {
    return (m_V2i32){ .x = x, .y = y };
}
$inline_always m_V2i32 m_V2i32_from3(m_V3i32 v) {
    return m_V2i32_from(v.x, v.y);
}
$inline_always m_V2i32 m_V2i32_from4(m_V4i32 v) {
    return m_V2i32_from(v.x, v.y);
}
$inline_always m_V2i32 m_V2i32_fill(i32 scalar) {
    return m_V2i32_from(scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V2i32) {
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
$inline_always m_V2i32 m_V2i32_neg(m_V2i32 v) {
    return m_V2i32_from(-v.x, -v.y);
}
$inline_always m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always m_V2i32 m_V2i32_mod(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}
$inline_always m_V2i32 m_V2i32_scale(m_V2i32 v, i32 scalar) {
    return m_V2i32_from(v.x * scalar, v.y * scalar);
}

/* Range Operations */
$inline_always m_V2i32 m_V2i32_min(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$inline_always m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_from(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$inline_always m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
    return m_V2i32_min(m_V2i32_max(v, min), max);
}
$inline_always m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
    return m_V2i32_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$inline_always i32 m_V2i32_lenSq(m_V2i32 v) {
    return m_V2i32_dot(v, v);
}
$inline_always i32 m_V2i32_distSq(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_lenSq(m_V2i32_sub(lhs, rhs));
}
$inline_always i32 m_V2i32_dot(m_V2i32 lhs, m_V2i32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always i32 m_V2i32_cross(m_V2i32 lhs, m_V2i32 rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always m_V2i32 m_V2i32_perp(m_V2i32 v) {
    return m_V2i32_from(-v.y, v.x);
}

/* Vec3i functions */
/* Construction */
$inline_always m_V3i32 m_V3i32_from(i32 x, i32 y, i32 z) {
    return (m_V3i32){ .x = x, .y = y, .z = z };
}
$inline_always m_V3i32 m_V3i32_from2(m_V2i32 v) {
    return m_V3i32_from(v.x, v.y, 0);
}
$inline_always m_V3i32 m_V3i32_from4(m_V4i32 v) {
    return m_V3i32_from(v.x, v.y, v.z);
}
$inline_always m_V3i32 m_V3i32_fill(i32 scalar) {
    return m_V3i32_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V3i32) {
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
$inline_always m_V3i32 m_V3i32_neg(m_V3i32 v) {
    return m_V3i32_from(-v.x, -v.y, -v.z);
}
$inline_always m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always m_V3i32 m_V3i32_mod(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always m_V3i32 m_V3i32_scale(m_V3i32 v, i32 scalar) {
    return m_V3i32_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always m_V3i32 m_V3i32_min(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always m_V3i32 m_V3i32_max(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always m_V3i32 m_V3i32_clamp(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
    return m_V3i32_min(m_V3i32_max(v, min), max);
}
$inline_always m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
    return m_V3i32_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$inline_always i32 m_V3i32_lenSq(m_V3i32 v) {
    return m_V3i32_dot(v, v);
}
$inline_always i32 m_V3i32_distSq(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_lenSq(m_V3i32_sub(lhs, rhs));
}
$inline_always i32 m_V3i32_dot(m_V3i32 lhs, m_V3i32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always m_V3i32 m_V3i32_cross(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always m_V3i32 m_V3i32_perp(m_V3i32 v) {
    return v.x != 0 || v.y != 0
             ? m_V3i32_from(-v.y, v.x, 0)
             : m_V3i32_from(0, -v.z, v.y);
}

/* Vec4i functions */
/* Construction */
$inline_always m_V4i32 m_V4i32_from(i32 x, i32 y, i32 z, i32 w) {
    return (m_V4i32){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always m_V4i32 m_V4i32_from2(m_V2i32 v) {
    return m_V4i32_from(v.x, v.y, 0, 0);
}
$inline_always m_V4i32 m_V4i32_from3(m_V3i32 v) {
    return m_V4i32_from(v.x, v.y, v.z, 0);
}
$inline_always m_V4i32 m_V4i32_fill(i32 scalar) {
    return m_V4i32_from(scalar, scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V4i32) {
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
$inline_always m_V4i32 m_V4i32_neg(m_V4i32 v) {
    return m_V4i32_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always m_V4i32 m_V4i32_mod(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always m_V4i32 m_V4i32_scale(m_V4i32 v, i32 scalar) {
    return m_V4i32_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always m_V4i32 m_V4i32_min(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always m_V4i32 m_V4i32_max(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always m_V4i32 m_V4i32_clamp(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
    return m_V4i32_min(m_V4i32_max(v, min), max);
}
$inline_always m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
    return m_V4i32_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$inline_always i32 m_V4i32_lenSq(m_V4i32 v) {
    return m_V4i32_dot(v, v);
}
$inline_always i32 m_V4i32_distSq(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_lenSq(m_V4i32_sub(lhs, rhs));
}
$inline_always i32 m_V4i32_dot(m_V4i32 lhs, m_V4i32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2l functions */
/* Construction */
$inline_always m_V2i64 m_V2i64_from(i64 x, i64 y) {
    return (m_V2i64){ .x = x, .y = y };
}
$inline_always m_V2i64 m_V2i64_from3(m_V3i64 v) {
    return m_V2i64_from(v.x, v.y);
}
$inline_always m_V2i64 m_V2i64_from4(m_V4i64 v) {
    return m_V2i64_from(v.x, v.y);
}
$inline_always m_V2i64 m_V2i64_fill(i64 scalar) {
    return m_V2i64_from(scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V2i64) {
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
$inline_always m_V2i64 m_V2i64_neg(m_V2i64 v) {
    return m_V2i64_from(-v.x, -v.y);
}
$inline_always m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(lhs.x + rhs.x, lhs.y + rhs.y);
}
$inline_always m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(lhs.x - rhs.x, lhs.y - rhs.y);
}
$inline_always m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(lhs.x * rhs.x, lhs.y * rhs.y);
}
$inline_always m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(lhs.x / rhs.x, lhs.y / rhs.y);
}
$inline_always m_V2i64 m_V2i64_mod(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y)
    );
}
$inline_always m_V2i64 m_V2i64_scale(m_V2i64 v, i64 scalar) {
    return m_V2i64_from(v.x * scalar, v.y * scalar);
}

/* Range Operations */
$inline_always m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y)
    );
}
$inline_always m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y)
    );
}
$inline_always m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
    return m_V2i64_min(m_V2i64_max(v, min), max);
}
$inline_always m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
    return m_V2i64_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$inline_always i64 m_V2i64_lenSq(m_V2i64 v) {
    return m_V2i64_dot(v, v);
}
$inline_always i64 m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_lenSq(m_V2i64_sub(lhs, rhs));
}
$inline_always i64 m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$inline_always i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$inline_always m_V2i64 m_V2i64_perp(m_V2i64 v) {
    return m_V2i64_from(-v.y, v.x);
}

/* Vec3l functions */
/* Construction */
$inline_always m_V3i64 m_V3i64_from(i64 x, i64 y, i64 z) {
    return (m_V3i64){ .x = x, .y = y, .z = z };
}
$inline_always m_V3i64 m_V3i64_from2(m_V2i64 v) {
    return m_V3i64_from(v.x, v.y, 0);
}
$inline_always m_V3i64 m_V3i64_from4(m_V4i64 v) {
    return m_V3i64_from(v.x, v.y, v.z);
}
$inline_always m_V3i64 m_V3i64_fill(i64 scalar) {
    return m_V3i64_from(scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V3i64) {
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
$inline_always m_V3i64 m_V3i64_neg(m_V3i64 v) {
    return m_V3i64_from(-v.x, -v.y, -v.z);
}
$inline_always m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$inline_always m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$inline_always m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$inline_always m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$inline_always m_V3i64 m_V3i64_mod(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}
$inline_always m_V3i64 m_V3i64_scale(m_V3i64 v, i64 scalar) {
    return m_V3i64_from(v.x * scalar, v.y * scalar, v.z * scalar);
}

/* Range Operations */
$inline_always m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$inline_always m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$inline_always m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
    return m_V3i64_min(m_V3i64_max(v, min), max);
}
$inline_always m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
    return m_V3i64_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$inline_always i64 m_V3i64_lenSq(m_V3i64 v) {
    return m_V3i64_dot(v, v);
}
$inline_always i64 m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_lenSq(m_V3i64_sub(lhs, rhs));
}
$inline_always i64 m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$inline_always m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_from(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$inline_always m_V3i64 m_V3i64_perp(m_V3i64 v) {
    return v.x != 0 || v.y != 0
             ? m_V3i64_from(-v.y, v.x, 0)
             : m_V3i64_from(0, -v.z, v.y);
}

/* Vec4l functions */
/* Construction */
$inline_always m_V4i64 m_V4i64_from(i64 x, i64 y, i64 z, i64 w) {
    return (m_V4i64){ .x = x, .y = y, .z = z, .w = w };
}
$inline_always m_V4i64 m_V4i64_from2(m_V2i64 v) {
    return m_V4i64_from(v.x, v.y, 0, 0);
}
$inline_always m_V4i64 m_V4i64_from3(m_V3i64 v) {
    return m_V4i64_from(v.x, v.y, v.z, 0);
}
$inline_always m_V4i64 m_V4i64_fill(i64 scalar) {
    return m_V4i64_from(scalar, scalar, scalar, scalar);
}

/* Comparison */
$inline_always cmp_fnCmp(m_V4i64) {
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
$inline_always m_V4i64 m_V4i64_neg(m_V4i64 v) {
    return m_V4i64_from(-v.x, -v.y, -v.z, -v.w);
}
$inline_always m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$inline_always m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$inline_always m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$inline_always m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$inline_always m_V4i64 m_V4i64_mod(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}
$inline_always m_V4i64 m_V4i64_scale(m_V4i64 v, i64 scalar) {
    return m_V4i64_from(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

/* Range Operations */
$inline_always m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$inline_always m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_from(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$inline_always m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
    return m_V4i64_min(m_V4i64_max(v, min), max);
}
$inline_always m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
    return m_V4i64_from(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$inline_always i64 m_V4i64_lenSq(m_V4i64 v) {
    return m_V4i64_dot(v, v);
}
$inline_always i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_lenSq(m_V4i64_sub(lhs, rhs));
}
$inline_always i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

#endif /* MATH_VEC_INCLUDED */
