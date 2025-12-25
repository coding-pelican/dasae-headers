/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vec.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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
#include "../Vec.h"

/*========== Float32 (f32) Vector Functions =================================*/

/* Vec2f functions */
/* Constants */
#define m_V2f32_zero ((m_V2f32){ .x = 0.0f, .y = 0.0f })
#define m_V2f32_unit_x ((m_V2f32){ .x = 1.0f, .y = 0.0f })
#define m_V2f32_unit_y ((m_V2f32){ .x = 0.0f, .y = 1.0f })
#define m_V2f32_one ((m_V2f32){ .x = 1.0f, .y = 1.0f })
#define m_V2f32_left ((m_V2f32){ .x = -1.0f, .y = 0.0f })
#define m_V2f32_up ((m_V2f32){ .x = 0.0f, .y = 1.0f })
#define m_V2f32_right ((m_V2f32){ .x = 1.0f, .y = 0.0f })
#define m_V2f32_down ((m_V2f32){ .x = 0.0f, .y = -1.0f })
#define m_V2f32_eps ((m_V2f32){ .x = f32_eps, .y = f32_eps })
#define m_V2f32_nan ((m_V2f32){ .x = f32_nan, .y = f32_nan })
#define m_V2f32_inf ((m_V2f32){ .x = f32_inf, .y = f32_inf })
#define m_V2f32_limit_min ((m_V2f32){ .x = f32_limit_min, .y = f32_limit_min })
#define m_V2f32_limit_max ((m_V2f32){ .x = f32_limit_max, .y = f32_limit_max })

/* Construction */
#define m_V2f32_of_static(_x, _y) ((m_V2f32){ .x = (_x), .y = (_y) })
$attr($inline_always)
$static m_V2f32 m_V2f32_of(f32 x, f32 y);
$attr($inline_always)
$static m_V2f32 m_V2f32_splat(f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_from3(m_V3f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_from4(m_V4f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_sincos(f32 radians);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2f32)(lhs, rhs));

$attr($inline_always)
$static cmp_fn_ordApx$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_eqApx_default$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_neApx_default$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ltApx_default$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_gtApx_default$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_leApx_default$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_geApx_default$((m_V2f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr($inline_always)
$static m_V2f32 m_V2f32_neg(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_inv(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_mod(m_V2f32 lhs, m_V2f32 rhs);

$attr($inline_always)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar);

$attr($inline_always)
$static m_V2f32 m_V2f32_addScalar(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_subScalar(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_mulScalar(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_divScalar(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_modScalar(m_V2f32 v, f32 scalar);

$attr($inline_always)
$static m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_modAsg(m_V2f32* lhs, m_V2f32 rhs);

$attr($inline_always)
$static m_V2f32* m_V2f32_scaleAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_scaleInvAsg(m_V2f32* v, f32 scalar);

$attr($inline_always)
$static m_V2f32* m_V2f32_addScalarAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_subScalarAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_mulScalarAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_divScalarAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_modScalarAsg(m_V2f32* v, f32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp01(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap01(m_V2f32 v);

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V2f32_lenSq(m_V2f32 v);
$attr($inline_always)
$static f32 m_V2f32_len(m_V2f32 v);
$attr($inline_always)
$static f32 m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static f32 m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static f32 m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_norm(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto);
$attr($inline_always)
$static m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from);
$attr($inline_always)
$static m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal);
$attr($inline_always)
$static m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 angle);
$attr($inline_always)
$static m_V2f32 m_V2f32_perp(m_V2f32 v);

/* Vec3f functions */
/* Constants */
#define m_V3f32_zero ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_unit_x ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_unit_y ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_unit_z ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_unit_xy ((m_V3f32){ .x = 1.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_unit_yz ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 1.0f })
#define m_V3f32_unit_zx ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_one ((m_V3f32){ .x = 1.0f, .y = 1.0f, .z = 1.0f })
#define m_V3f32_left ((m_V3f32){ .x = -1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_up ((m_V3f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f })
#define m_V3f32_right ((m_V3f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f })
#define m_V3f32_down ((m_V3f32){ .x = 0.0f, .y = -1.0f, .z = 0.0f })
#define m_V3f32_forward ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f })
#define m_V3f32_backward ((m_V3f32){ .x = 0.0f, .y = 0.0f, .z = -1.0f })
#define m_V3f32_eps ((m_V3f32){ .x = f32_eps, .y = f32_eps, .z = f32_eps })
#define m_V3f32_nan ((m_V3f32){ .x = f32_nan, .y = f32_nan, .z = f32_nan })
#define m_V3f32_inf ((m_V3f32){ .x = f32_inf, .y = f32_inf, .z = f32_inf })
#define m_V3f32_limit_min ((m_V3f32){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min })
#define m_V3f32_limit_max ((m_V3f32){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max })

/* Construction */
#define m_V3f32_of_static(_x, _y, _z) ((m_V3f32){ .x = (_x), .y = (_y), .z = (_z) })
$attr($inline_always)
$static m_V3f32 m_V3f32_of(f32 x, f32 y, f32 z);
$attr($inline_always)
$static m_V3f32 m_V3f32_splat(f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_from2(m_V2f32 v);
$attr($inline_always)
$static m_V3f32 m_V3f32_from4(m_V4f32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3f32)(lhs, rhs));

$attr($inline_always)
$static cmp_fn_ordApx$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_eqApx_default$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_neApx_default$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ltApx_default$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_gtApx_default$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_leApx_default$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_geApx_default$((m_V3f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr($inline_always)
$static m_V3f32 m_V3f32_neg(m_V3f32 v);
$attr($inline_always)
$static m_V3f32 m_V3f32_inv(m_V3f32 v);
$attr($inline_always)
$static m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_mod(m_V3f32 lhs, m_V3f32 rhs);

$attr($inline_always)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar);

$attr($inline_always)
$static m_V3f32 m_V3f32_addScalar(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_subScalar(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_mulScalar(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_divScalar(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_modScalar(m_V3f32 v, f32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp01(m_V3f32 v);
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap01(m_V3f32 v);

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V3f32_lenSq(m_V3f32 v);
$attr($inline_always)
$static f32 m_V3f32_len(m_V3f32 v);
$attr($inline_always)
$static f32 m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static f32 m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static f32 m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs);
$attr($inline_always)
$static m_V3f32 m_V3f32_norm(m_V3f32 v);
$attr($inline_always)
$static m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto);
$attr($inline_always)
$static m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from);
$attr($inline_always)
$static m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal);
$attr($inline_always)
$static m_V3f32 m_V3f32_rotate(m_V3f32 v, m_V3f32 axis, f32 angle);
$attr($inline_always)
$static m_V3f32 m_V3f32_perp(m_V3f32 v);

/* Vec4f functions */
/* Constants */
#define m_V4f32_zero ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_x ((m_V4f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_y ((m_V4f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_unit_z ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define m_V4f32_unit_w ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f })
#define m_V4f32_one ((m_V4f32){ .x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f })
#define m_V4f32_left ((m_V4f32){ .x = -1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_up ((m_V4f32){ .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_right ((m_V4f32){ .x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_down ((m_V4f32){ .x = 0.0f, .y = -1.0f, .z = 0.0f, .w = 0.0f })
#define m_V4f32_forward ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f })
#define m_V4f32_backward ((m_V4f32){ .x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 0.0f })
#define m_V4f32_eps ((m_V4f32){ .x = f32_eps, .y = f32_eps, .z = f32_eps, .w = f32_eps })
#define m_V4f32_nan ((m_V4f32){ .x = f32_nan, .y = f32_nan, .z = f32_nan, .w = f32_nan })
#define m_V4f32_inf ((m_V4f32){ .x = f32_inf, .y = f32_inf, .z = f32_inf, .w = f32_inf })
#define m_V4f32_limit_min ((m_V4f32){ .x = f32_limit_min, .y = f32_limit_min, .z = f32_limit_min, .w = f32_limit_min })
#define m_V4f32_limit_max ((m_V4f32){ .x = f32_limit_max, .y = f32_limit_max, .z = f32_limit_max, .w = f32_limit_max })

/* Construction */
#define m_V4f32_of_static(_x, _y, _z, _w) ((m_V3f32){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr($inline_always)
$static m_V4f32 m_V4f32_of(f32 x, f32 y, f32 z, f32 w);
$attr($inline_always)
$static m_V4f32 m_V4f32_splat(f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_from2(m_V2f32 v);
$attr($inline_always)
$static m_V4f32 m_V4f32_from3(m_V3f32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4f32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4f32)(lhs, rhs));

$attr($inline_always)
$static cmp_fn_ordApx$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_eqApx_default$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_neApx_default$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ltApx_default$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_gtApx_default$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_leApx_default$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_geApx_default$((m_V4f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr($inline_always)
$static m_V4f32 m_V4f32_neg(m_V4f32 v);
$attr($inline_always)
$static m_V4f32 m_V4f32_inv(m_V4f32 v);
$attr($inline_always)
$static m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_mod(m_V4f32 lhs, m_V4f32 rhs);

$attr($inline_always)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar);

$attr($inline_always)
$static m_V4f32 m_V4f32_addScalar(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_subScalar(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_mulScalar(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_divScalar(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_modScalar(m_V4f32 v, f32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp01(m_V4f32 v);
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap01(m_V4f32 v);

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V4f32_lenSq(m_V4f32 v);
$attr($inline_always)
$static f32 m_V4f32_len(m_V4f32 v);
$attr($inline_always)
$static f32 m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static f32 m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static f32 m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs);
$attr($inline_always)
$static m_V4f32 m_V4f32_norm(m_V4f32 v);
$attr($inline_always)
$static m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto);
$attr($inline_always)
$static m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from);
$attr($inline_always)
$static m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal);

/*========== Float64 (f64) Vector Functions =================================*/

/* Vec2d functions */
/* Constants */
#define m_V2f64_zero ((m_V2f64){ .x = 0.0, .y = 0.0 })
#define m_V2f64_unit_x ((m_V2f64){ .x = 1.0, .y = 0.0 })
#define m_V2f64_unit_y ((m_V2f64){ .x = 0.0, .y = 1.0 })
#define m_V2f64_one ((m_V2f64){ .x = 1.0, .y = 1.0 })
#define m_V2f64_left ((m_V2f64){ .x = -1.0, .y = 0.0 })
#define m_V2f64_up ((m_V2f64){ .x = 0.0, .y = 1.0 })
#define m_V2f64_right ((m_V2f64){ .x = 1.0, .y = 0.0 })
#define m_V2f64_down ((m_V2f64){ .x = 0.0, .y = -1.0 })
#define m_V2f64_eps ((m_V2f64){ .x = f64_eps, .y = f64_eps })
#define m_V2f64_nan ((m_V2f64){ .x = f64_nan, .y = f64_nan })
#define m_V2f64_inf ((m_V2f64){ .x = f64_inf, .y = f64_inf })
#define m_V2f64_limit_min ((m_V2f64){ .x = f64_limit_min, .y = f64_limit_min })
#define m_V2f64_limit_max ((m_V2f64){ .x = f64_limit_max, .y = f64_limit_max })

/* Construction */
#define m_V2f64_of_static(_x, _y) ((m_V2f64){ .x = (_x), .y = (_y) })
$attr($inline_always)
$static m_V2f64 m_V2f64_of(f64 x, f64 y);
$attr($inline_always)
$static m_V2f64 m_V2f64_splat(f64 scalar);
$attr($inline_always)
$static m_V2f64 m_V2f64_from3(m_V3f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_from4(m_V4f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_sincos(f64 radians);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2f64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2f64 m_V2f64_neg(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_inv(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_mod(m_V2f64 lhs, m_V2f64 rhs);

$attr($inline_always)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar);
$attr($inline_always)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler);

$attr($inline_always)
$static m_V2f64 m_V2f64_addScalar(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_subScalar(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_mulScalar(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_divScalar(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_modScalar(m_V2f64 lhs, f64 rhs);

$attr($inline_always)
$static m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_modAsg(m_V2f64* lhs, m_V2f64 rhs);

$attr($inline_always)
$static m_V2f64* m_V2f64_scaleAsg(m_V2f64* v, f64 scalar);
$attr($inline_always)
$static m_V2f64* m_V2f64_scaleInvAsg(m_V2f64* v, f64 scalar);

$attr($inline_always)
$static m_V2f64* m_V2f64_addScalarAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_subScalarAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_mulScalarAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_divScalarAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_modScalarAsg(m_V2f64* lhs, f64 rhs);

/* Range Operations */
$attr($inline_always)
$static m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp01(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap01(m_V2f64 v);

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V2f64_lenSq(m_V2f64 v);
$attr($inline_always)
$static f64 m_V2f64_len(m_V2f64 v);
$attr($inline_always)
$static f64 m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static f64 m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static f64 m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_norm(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto);
$attr($inline_always)
$static m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from);
$attr($inline_always)
$static m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal);
$attr($inline_always)
$static m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 angle);
$attr($inline_always)
$static m_V2f64 m_V2f64_perp(m_V2f64 v);

/* Vec3d functions */
/* Constants */
#define m_V3f64_zero ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_unit_x ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_unit_y ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_unit_z ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_unit_xy ((m_V3f64){ .x = 1.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_unit_yz ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 1.0 })
#define m_V3f64_unit_zx ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_one ((m_V3f64){ .x = 1.0, .y = 1.0, .z = 1.0 })
#define m_V3f64_left ((m_V3f64){ .x = -1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_up ((m_V3f64){ .x = 0.0, .y = 1.0, .z = 0.0 })
#define m_V3f64_right ((m_V3f64){ .x = 1.0, .y = 0.0, .z = 0.0 })
#define m_V3f64_down ((m_V3f64){ .x = 0.0, .y = -1.0, .z = 0.0 })
#define m_V3f64_forward ((m_V3f64){ .x = 0.0, .y = 0.0, .z = 1.0 })
#define m_V3f64_backward ((m_V3f64){ .x = 0.0, .y = 0.0, .z = -1.0 })
#define m_V3f64_eps ((m_V3f64){ .x = f64_eps, .y = f64_eps, .z = f64_eps })
#define m_V3f64_nan ((m_V3f64){ .x = f64_nan, .y = f64_nan, .z = f64_nan })
#define m_V3f64_inf ((m_V3f64){ .x = f64_inf, .y = f64_inf, .z = f64_inf })

/* Construction */
#define m_V3f64_of_static(_x, _y, _z) ((m_V3f64){ .x = (_x), .y = (_y), .z = (_z) })
$attr($inline_always)
$static m_V3f64 m_V3f64_of(f64 x, f64 y, f64 z);
$attr($inline_always)
$static m_V3f64 m_V3f64_splat(f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_from2(m_V2f64 v);
$attr($inline_always)
$static m_V3f64 m_V3f64_from4(m_V4f64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3f64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3f64 m_V3f64_neg(m_V3f64 v);
$attr($inline_always)
$static m_V3f64 m_V3f64_inv(m_V3f64 v);
$attr($inline_always)
$static m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_mod(m_V3f64 lhs, m_V3f64 rhs);

$attr($inline_always)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar);

$attr($inline_always)
$static m_V3f64 m_V3f64_addScalar(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_subScalar(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_mulScalar(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_divScalar(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_modScalar(m_V3f64 v, f64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp01(m_V3f64 v);
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap01(m_V3f64 v);

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V3f64_lenSq(m_V3f64 v);
$attr($inline_always)
$static f64 m_V3f64_len(m_V3f64 v);
$attr($inline_always)
$static f64 m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static f64 m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static f64 m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs);
$attr($inline_always)
$static m_V3f64 m_V3f64_norm(m_V3f64 v);
$attr($inline_always)
$static m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto);
$attr($inline_always)
$static m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from);
$attr($inline_always)
$static m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal);
$attr($inline_always)
$static m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle);
$attr($inline_always)
$static m_V3f64 m_V3f64_perp(m_V3f64 v);

/* Vec4d functions */
/* Constants */
#define m_V4f64_zero ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_x ((m_V4f64){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_y ((m_V4f64){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_unit_z ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define m_V4f64_unit_w ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0 })
#define m_V4f64_one ((m_V4f64){ .x = 1.0, .y = 1.0, .z = 1.0, .w = 1.0 })
#define m_V4f64_left ((m_V4f64){ .x = -1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_up ((m_V4f64){ .x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_right ((m_V4f64){ .x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_down ((m_V4f64){ .x = 0.0, .y = -1.0, .z = 0.0, .w = 0.0 })
#define m_V4f64_forward ((m_V4f64){ .x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0 })
#define m_V4f64_backward ((m_V4f64){ .x = 0.0, .y = 0.0, .z = -1.0, .w = 0.0 })
#define m_V4f64_eps ((m_V4f64){ .x = f64_eps, .y = f64_eps, .z = f64_eps, .w = f64_eps })
#define m_V4f64_nan ((m_V4f64){ .x = f64_nan, .y = f64_nan, .z = f64_nan, .w = f64_nan })
#define m_V4f64_inf ((m_V4f64){ .x = f64_inf, .y = f64_inf, .z = f64_inf, .w = f64_inf })
#define m_V4f64_limit_min ((m_V4f64){ .x = f64_limit_min, .y = f64_limit_min, .z = f64_limit_min, .w = f64_limit_min })
#define m_V4f64_limit_max ((m_V4f64){ .x = f64_limit_max, .y = f64_limit_max, .z = f64_limit_max, .w = f64_limit_max })

/* Construction */
#define m_V4f64_of_static(_x, _y, _z, _w) ((m_V4f64){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr($inline_always)
$static m_V4f64 m_V4f64_of(f64 x, f64 y, f64 z, f64 w);
$attr($inline_always)
$static m_V4f64 m_V4f64_splat(f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_from2(m_V2f64 v);
$attr($inline_always)
$static m_V4f64 m_V4f64_from3(m_V3f64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4f64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4f64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4f64 m_V4f64_neg(m_V4f64 v);
$attr($inline_always)
$static m_V4f64 m_V4f64_inv(m_V4f64 v);
$attr($inline_always)
$static m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_mod(m_V4f64 lhs, m_V4f64 rhs);

$attr($inline_always)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar);

$attr($inline_always)
$static m_V4f64 m_V4f64_addScalar(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_subScalar(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_mulScalar(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_divScalar(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_modScalar(m_V4f64 v, f64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp01(m_V4f64 v);
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap01(m_V4f64 v);

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V4f64_lenSq(m_V4f64 v);
$attr($inline_always)
$static f64 m_V4f64_len(m_V4f64 v);
$attr($inline_always)
$static f64 m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static f64 m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static f64 m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs);
$attr($inline_always)
$static m_V4f64 m_V4f64_norm(m_V4f64 v);
$attr($inline_always)
$static m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto);
$attr($inline_always)
$static m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from);
$attr($inline_always)
$static m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal);

/*========== Int32 (i32) Vector Functions ===================================*/

/* Vec2i functions */
/* Constants */
#define m_V2i32_zero ((m_V2i32){ .x = 0, .y = 0 })
#define m_V2i32_unit_x ((m_V2i32){ .x = 1, .y = 0 })
#define m_V2i32_unit_y ((m_V2i32){ .x = 0, .y = 1 })
#define m_V2i32_one ((m_V2i32){ .x = 1, .y = 1 })
#define m_V2i32_left ((m_V2i32){ .x = -1, .y = 0 })
#define m_V2i32_up ((m_V2i32){ .x = 0, .y = 1 })
#define m_V2i32_right ((m_V2i32){ .x = 1, .y = 0 })
#define m_V2i32_down ((m_V2i32){ .x = 0, .y = -1 })
#define m_V2i32_limit_min ((m_V2i32){ .x = i32_limit_min, .y = i32_limit_min })
#define m_V2i32_limit_max ((m_V2i32){ .x = i32_limit_max, .y = i32_limit_max })

/* Construction */
#define m_V2i32_of_static(_x, _y) ((m_V2i32){ .x = (_x), .y = (_y) })
$attr($inline_always)
$static m_V2i32 m_V2i32_of(i32 x, i32 y);
$attr($inline_always)
$static m_V2i32 m_V2i32_splat(i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_from3(m_V3i32 v);
$attr($inline_always)
$static m_V2i32 m_V2i32_from4(m_V4i32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2i32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2i32 m_V2i32_neg(m_V2i32 v);
$attr($inline_always)
$static m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_mod(m_V2i32 lhs, m_V2i32 rhs);

$attr($inline_always)
$static m_V2i32 m_V2i32_scal(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_scalInv(m_V2i32 v, i32 scalar);

$attr($inline_always)
$static m_V2i32 m_V2i32_addScalar(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_subScalar(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_mulScalar(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_divScalar(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_modScalar(m_V2i32 v, i32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2i32 m_V2i32_min(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max);
$attr($inline_always)
$static m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max);

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V2i32_lenSq(m_V2i32 v);
$attr($inline_always)
$static i32 m_V2i32_distSq(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static i32 m_V2i32_dot(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static i32 m_V2i32_cross(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_perp(m_V2i32 v);

/* Vec3i functions */
/* Constants */
#define m_V3i32_zero ((m_V3i32){ .x = 0, .y = 0, .z = 0 })
#define m_V3i32_unit_x ((m_V3i32){ .x = 1, .y = 0, .z = 0 })
#define m_V3i32_unit_y ((m_V3i32){ .x = 0, .y = 1, .z = 0 })
#define m_V3i32_unit_z ((m_V3i32){ .x = 0, .y = 0, .z = 1 })
#define m_V3i32_unit_xy ((m_V3i32){ .x = 1, .y = 1, .z = 0 })
#define m_V3i32_unit_yz ((m_V3i32){ .x = 0, .y = 1, .z = 1 })
#define m_V3i32_unit_zx ((m_V3i32){ .x = 1, .y = 0, .z = 1 })
#define m_V3i32_one ((m_V3i32){ .x = 1, .y = 1, .z = 1 })
#define m_V3i32_left ((m_V3i32){ .x = -1, .y = 0, .z = 0 })
#define m_V3i32_up ((m_V3i32){ .x = 0, .y = 1, .z = 0 })
#define m_V3i32_right ((m_V3i32){ .x = 1, .y = 0, .z = 0 })
#define m_V3i32_down ((m_V3i32){ .x = 0, .y = -1, .z = 0 })
#define m_V3i32_forward ((m_V3i32){ .x = 0, .y = 0, .z = 1 })
#define m_V3i32_backward ((m_V3i32){ .x = 0, .y = 0, .z = -1 })
#define m_V3i32_limit_min ((m_V3i32){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min })
#define m_V3i32_limit_max ((m_V3i32){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max })

/* Construction */
#define m_V3i32_of_static(_x, _y, _z) ((m_V3i32){ .x = (_x), .y = (_y), .z = (_z) })
$attr($inline_always)
$static m_V3i32 m_V3i32_of(i32 x, i32 y, i32 z);
$attr($inline_always)
$static m_V3i32 m_V3i32_splat(i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_from2(m_V2i32 v);
$attr($inline_always)
$static m_V3i32 m_V3i32_from4(m_V4i32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3i32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3i32 m_V3i32_neg(m_V3i32 v);
$attr($inline_always)
$static m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_mod(m_V3i32 lhs, m_V3i32 rhs);

$attr($inline_always)
$static m_V3i32 m_V3i32_scal(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_scalInv(m_V3i32 v, i32 scalar);

$attr($inline_always)
$static m_V3i32 m_V3i32_addScalar(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_subScalar(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_mulScalar(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_divScalar(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_modScalar(m_V3i32 v, i32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3i32 m_V3i32_min(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_max(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_clamp(m_V3i32 v, m_V3i32 min, m_V3i32 max);
$attr($inline_always)
$static m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max);

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V3i32_lenSq(m_V3i32 v);
$attr($inline_always)
$static i32 m_V3i32_distSq(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static i32 m_V3i32_dot(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_cross(m_V3i32 lhs, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_perp(m_V3i32 v);

/* Vec4i functions */
/* Constants */
#define m_V4i32_zero ((m_V4i32){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_unit_x ((m_V4i32){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_unit_y ((m_V4i32){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i32_unit_z ((m_V4i32){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i32_unit_w ((m_V4i32){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define m_V4i32_one ((m_V4i32){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define m_V4i32_left ((m_V4i32){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_up ((m_V4i32){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i32_right ((m_V4i32){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i32_down ((m_V4i32){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define m_V4i32_forward ((m_V4i32){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i32_backward ((m_V4i32){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define m_V4i32_limit_min ((m_V4i32){ .x = i32_limit_min, .y = i32_limit_min, .z = i32_limit_min, .w = i32_limit_min })
#define m_V4i32_limit_max ((m_V4i32){ .x = i32_limit_max, .y = i32_limit_max, .z = i32_limit_max, .w = i32_limit_max })

/* Construction */
#define m_V4i32_of_static(_x, _y, _z, _w) ((m_V4i32){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr($inline_always)
$static m_V4i32 m_V4i32_of(i32 x, i32 y, i32 z, i32 w);
$attr($inline_always)
$static m_V4i32 m_V4i32_splat(i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_from2(m_V2i32 v);
$attr($inline_always)
$static m_V4i32 m_V4i32_from3(m_V3i32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4i32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4i32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4i32 m_V4i32_neg(m_V4i32 v);
$attr($inline_always)
$static m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_mod(m_V4i32 lhs, m_V4i32 rhs);

$attr($inline_always)
$static m_V4i32 m_V4i32_scal(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_scalInv(m_V4i32 v, i32 scalar);

$attr($inline_always)
$static m_V4i32 m_V4i32_addScalar(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_subScalar(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_mulScalar(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_divScalar(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_modScalar(m_V4i32 v, i32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4i32 m_V4i32_min(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_max(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static m_V4i32 m_V4i32_clamp(m_V4i32 v, m_V4i32 min, m_V4i32 max);
$attr($inline_always)
$static m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max);

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V4i32_lenSq(m_V4i32 v);
$attr($inline_always)
$static i32 m_V4i32_distSq(m_V4i32 lhs, m_V4i32 rhs);
$attr($inline_always)
$static i32 m_V4i32_dot(m_V4i32 lhs, m_V4i32 rhs);

/*========== Int64 (i64) Vector Functions ===================================*/

/* Vec2l functions */
/* Constants */
#define m_V2i64_zero ((m_V2i64){ .x = 0, .y = 0 })
#define m_V2i64_unit_x ((m_V2i64){ .x = 1, .y = 0 })
#define m_V2i64_unit_y ((m_V2i64){ .x = 0, .y = 1 })
#define m_V2i64_one ((m_V2i64){ .x = 1, .y = 1 })
#define m_V2i64_left ((m_V2i64){ .x = -1, .y = 0 })
#define m_V2i64_up ((m_V2i64){ .x = 0, .y = 1 })
#define m_V2i64_right ((m_V2i64){ .x = 1, .y = 0 })
#define m_V2i64_down ((m_V2i64){ .x = 0, .y = -1 })
#define m_V2i64_limit_min ((m_V2i64){ .x = i64_limit_min, .y = i64_limit_min })
#define m_V2i64_limit_max ((m_V2i64){ .x = i64_limit_max, .y = i64_limit_max })

/* Construction */
#define m_V2i64_of_static(i64_x, i64_y) ((m_V2i64){ .x = (i64_x), .y = (i64_y) })
$attr($inline_always)
$static m_V2i64 m_V2i64_of(i64 x, i64 y);
$attr($inline_always)
$static m_V2i64 m_V2i64_splat(i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_from3(m_V3i64 v);
$attr($inline_always)
$static m_V2i64 m_V2i64_from4(m_V4i64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2i64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2i64 m_V2i64_neg(m_V2i64 v);
$attr($inline_always)
$static m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_mod(m_V2i64 lhs, m_V2i64 rhs);

$attr($inline_always)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar);

$attr($inline_always)
$static m_V2i64 m_V2i64_addScalar(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_subScalar(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_mulScalar(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_divScalar(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_modScalar(m_V2i64 v, i64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max);
$attr($inline_always)
$static m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max);

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V2i64_lenSq(m_V2i64 v);
$attr($inline_always)
$static i64 m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static i64 m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_perp(m_V2i64 v);

/* Vec3l functions */
/* Constants */
#define m_V3i64_zero ((m_V3i64){ .x = 0, .y = 0, .z = 0 })
#define m_V3i64_unit_x ((m_V3i64){ .x = 1, .y = 0, .z = 0 })
#define m_V3i64_unit_y ((m_V3i64){ .x = 0, .y = 1, .z = 0 })
#define m_V3i64_unit_z ((m_V3i64){ .x = 0, .y = 0, .z = 1 })
#define m_V3i64_unit_xy ((m_V3i64){ .x = 1, .y = 1, .z = 0 })
#define m_V3i64_unit_yz ((m_V3i64){ .x = 0, .y = 1, .z = 1 })
#define m_V3i64_unit_zx ((m_V3i64){ .x = 1, .y = 0, .z = 1 })
#define m_V3i64_one ((m_V3i64){ .x = 1, .y = 1, .z = 1 })
#define m_V3i64_left ((m_V3i64){ .x = -1, .y = 0, .z = 0 })
#define m_V3i64_up ((m_V3i64){ .x = 0, .y = 1, .z = 0 })
#define m_V3i64_right ((m_V3i64){ .x = 1, .y = 0, .z = 0 })
#define m_V3i64_down ((m_V3i64){ .x = 0, .y = -1, .z = 0 })
#define m_V3i64_forward ((m_V3i64){ .x = 0, .y = 0, .z = 1 })
#define m_V3i64_backward ((m_V3i64){ .x = 0, .y = 0, .z = -1 })
#define m_V3i64_limit_min ((m_V3i64){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min })
#define m_V3i64_limit_max ((m_V3i64){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max })

/* Construction */
#define m_V3i64_of_static(i64_x, i64_y, i64_z) ((m_V3i64){ .x = (i64_x), .y = (i64_y), .z = (i64_z) })
$attr($inline_always)
$static m_V3i64 m_V3i64_of(i64 x, i64 y, i64 z);
$attr($inline_always)
$static m_V3i64 m_V3i64_splat(i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_from2(m_V2i64 v);
$attr($inline_always)
$static m_V3i64 m_V3i64_from4(m_V4i64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3i64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3i64 m_V3i64_neg(m_V3i64 v);
$attr($inline_always)
$static m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_mod(m_V3i64 lhs, m_V3i64 rhs);

$attr($inline_always)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar);

$attr($inline_always)
$static m_V3i64 m_V3i64_addScalar(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_subScalar(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_mulScalar(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_divScalar(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_modScalar(m_V3i64 v, i64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max);
$attr($inline_always)
$static m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max);

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V3i64_lenSq(m_V3i64 v);
$attr($inline_always)
$static i64 m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static i64 m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs);
$attr($inline_always)
$static m_V3i64 m_V3i64_perp(m_V3i64 v);

/* Vec4l functions */
/* Constants */
#define m_V4i64_zero ((m_V4i64){ .x = 0, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_unit_x ((m_V4i64){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_unit_y ((m_V4i64){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i64_unit_z ((m_V4i64){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i64_unit_w ((m_V4i64){ .x = 0, .y = 0, .z = 0, .w = 1 })
#define m_V4i64_one ((m_V4i64){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define m_V4i64_left ((m_V4i64){ .x = -1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_up ((m_V4i64){ .x = 0, .y = 1, .z = 0, .w = 0 })
#define m_V4i64_right ((m_V4i64){ .x = 1, .y = 0, .z = 0, .w = 0 })
#define m_V4i64_down ((m_V4i64){ .x = 0, .y = -1, .z = 0, .w = 0 })
#define m_V4i64_forward ((m_V4i64){ .x = 0, .y = 0, .z = 1, .w = 0 })
#define m_V4i64_backward ((m_V4i64){ .x = 0, .y = 0, .z = -1, .w = 0 })
#define m_V4i64_limit_min ((m_V4i64){ .x = i64_limit_min, .y = i64_limit_min, .z = i64_limit_min, .w = i64_limit_min })
#define m_V4i64_limit_max ((m_V4i64){ .x = i64_limit_max, .y = i64_limit_max, .z = i64_limit_max, .w = i64_limit_max })

/* Construction */
#define m_V4i64_of_static(i64_x, i64_y, i64_z, i64_w) ((m_V4i64){ .x = (i64_x), .y = (i64_y), .z = (i64_z), .w = (i64_w) })
$attr($inline_always)
$static m_V4i64 m_V4i64_of(i64 x, i64 y, i64 z, i64 w);
$attr($inline_always)
$static m_V4i64 m_V4i64_splat(i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_from2(m_V2i64 v);
$attr($inline_always)
$static m_V4i64 m_V4i64_from3(m_V3i64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4i64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4i64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4i64 m_V4i64_neg(m_V4i64 v);
$attr($inline_always)
$static m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_mod(m_V4i64 lhs, m_V4i64 rhs);

$attr($inline_always)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar);

$attr($inline_always)
$static m_V4i64 m_V4i64_addScalar(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_subScalar(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_mulScalar(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_divScalar(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_modScalar(m_V4i64 v, i64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max);
$attr($inline_always)
$static m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max);

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V4i64_lenSq(m_V4i64 v);
$attr($inline_always)
$static i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs);
$attr($inline_always)
$static i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs);

/*========== Implementations ================================================*/

/* Vec2f functions */
/* Construction */
$attr($inline_always)
$static m_V2f32 m_V2f32_of(f32 x, f32 y) {
#if arch_simd_use
    let result = Vec_from$((f32){ x, y });
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V2f32){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2f32 m_V2f32_splat(f32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(2, f32))scalar);
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_from3(m_V3f32 v) {
    return m_V2f32_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_from4(m_V4f32 v) {
    return m_V2f32_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_sincos(f32 radians) {
#if arch_simd_use
    let result = Vec_from$((f32){ flt_cos(radians), flt_sin(radians) });
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(flt_cos(radians), flt_sin(radians));
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 2; ++i) {
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApx$((m_V2f32)(lhs, rhs, threshold)) {
#if arch_simd_use
/* TODO: Implement Vec_ordApx for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 2; ++i) {
        // Calculate absolute difference
        let diff = flt_abs(lhs.s[i] - rhs.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2f32 m_V2f32_neg(m_V2f32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_inv(m_V2f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V2f32_of(1.0f / v.x, 1.0f / v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_mod(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V2f32_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(2, f32))scalar));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(2, f32))scalar));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2f32 m_V2f32_addScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_add(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_subScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_sub(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_mulScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mul(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_divScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_div(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_modScalar(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mod(lhs, m_V2f32_splat(rhs));
}

$attr($inline_always)
$static m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_add(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_sub(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_mul(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_div(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_modAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_mod(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_scaleAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scal(*v, scalar), v;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_scaleInvAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scalInv(*v, scalar), v;
}

$attr($inline_always)
$static m_V2f32* m_V2f32_addScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_addScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_subScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_subScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_mulScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_mulScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_divScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_divScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_modScalarAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$attr($inline_always)
$static m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(flt_min(lhs.x, rhs.x), flt_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_of(flt_max(lhs.x, rhs.x), flt_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_min(m_V2f32_max(v, min), max);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp01(m_V2f32 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V2f32_zero.a), Vec_fromA(m_V2f32_one.a));
    return lit$((m_V2f32){ .a = Vec_toA$((FieldType$(m_V2f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f32_clamp(v, m_V2f32_zero, m_V2f32_one);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V2f32_of(flt_wrap(v.x, min.x, max.x), flt_wrap(v.y, min.y, max.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap01(m_V2f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V2f32_wrap(v, m_V2f32_zero, m_V2f32_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V2f32_lenSq(m_V2f32 v) {
    return m_V2f32_dot(v, v);
}
$attr($inline_always)
$static f32 m_V2f32_len(m_V2f32 v) {
    return flt_sqrt(m_V2f32_lenSq(v));
}
$attr($inline_always)
$static f32 m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_lenSq(m_V2f32_sub(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs) {
    return flt_sqrt(m_V2f32_distSq(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((f32)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_use
    let l = Vec_from$((f32){ lhs.x, lhs.y, 0.0f });
    let r = Vec_from$((f32){ rhs.x, rhs.y, 0.0f });
    let result = Vec_cross(l, r);
    return lit$((m_V3f32){ .a = Vec_toA$((FieldType$(m_V3f32, a))(result)) }).z;
#endif /* arch_simd_use */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static m_V2f32 m_V2f32_norm(m_V2f32 v) {
    const f32 len = m_V2f32_len(v);
    return len <= 0.0f ? m_V2f32_zero : m_V2f32_scal(v, 1.0f / len);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto) {
    return m_V2f32_scal(onto, m_V2f32_dot(v, onto) / m_V2f32_lenSq(onto));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from) {
    return m_V2f32_sub(v, m_V2f32_project(v, from));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal) {
    return m_V2f32_sub(v, m_V2f32_scal(normal, 2.0f * m_V2f32_dot(v, normal)));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 radians) {
#if arch_simd_use
/* TODO: Implement Vec_rotate for SIMD */
#endif /* arch_simd_use */
    const m_V2f32 r = m_V2f32_sincos(radians);
    return m_V2f32_of(v.x * r.x - v.y * r.y, v.x * r.y + v.y * r.x);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_perp(m_V2f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return m_V2f32_of(-v.y, v.x);
}

/* Vec3f functions */
/* Construction */
$attr($inline_always)
$static m_V3f32 m_V3f32_of(f32 x, f32 y, f32 z) {
#if arch_simd_use
    let result = Vec_from$((f32){ x, y, z });
    return lit$((m_V3f32){ .a = Vec_toA$((FieldType$(m_V3f32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V3f32){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3f32 m_V3f32_from2(m_V2f32 v) {
    return m_V3f32_of(v.x, v.y, 0.0f);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_from4(m_V4f32 v) {
    return m_V3f32_of(v.x, v.y, v.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_splat(f32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(3, f32))scalar);
    return lit$((m_V3f32){ .a = Vec_toA$((FieldType$(m_V3f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(scalar, scalar, scalar);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3f32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 3; ++i) {
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApx$((m_V3f32)(lhs, rhs, threshold)) {
#if arch_simd_use
/* TODO: Implement Vec_ordApx for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 3; ++i) {
        // Calculate absolute difference
        let diff = flt_abs(lhs.s[i] - rhs.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3f32 m_V3f32_neg(m_V3f32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_inv(m_V3f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V3f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_mod(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V3f32_of(flt_mod(lhs.x, rhs.x), flt_mod(lhs.y, rhs.y), flt_mod(lhs.z, rhs.z));
}

$attr($inline_always)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(3, f32))scalar));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(3, f32))scalar));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3f32 m_V3f32_addScalar(m_V3f32 v, f32 scalar) {
    return m_V3f32_add(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_subScalar(m_V3f32 v, f32 scalar) {
    return m_V3f32_sub(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_mulScalar(m_V3f32 v, f32 scalar) {
    return m_V3f32_mul(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_divScalar(m_V3f32 v, f32 scalar) {
    return m_V3f32_div(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_modScalar(m_V3f32 v, f32 scalar) {
    return m_V3f32_mod(v, m_V3f32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_min(m_V3f32_max(v, min), max);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp01(m_V3f32 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V3f32_zero.a), Vec_fromA(m_V3f32_one.a));
    return lit$((m_V3f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_clamp(v, m_V3f32_zero, m_V3f32_one);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V3f32_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap01(m_V3f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V3f32_wrap(v, m_V3f32_zero, m_V3f32_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V3f32_lenSq(m_V3f32 v) {
    return m_V3f32_dot(v, v);
}
$attr($inline_always)
$static f32 m_V3f32_len(m_V3f32 v) {
    return flt_sqrt(m_V3f32_lenSq(v));
}
$attr($inline_always)
$static f32 m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_lenSq(m_V3f32_sub(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_len(m_V3f32_sub(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((f32)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_use
    let result = Vec_cross(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f32){ .a = Vec_toA$((FieldType$(m_V3f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f32_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_norm(m_V3f32 v) {
    const f32 len = m_V3f32_len(v);
    return len <= 0.0f ? m_V3f32_zero : m_V3f32_scal(v, 1.0f / len);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto) {
    return m_V3f32_scal(onto, m_V3f32_dot(v, onto) / m_V3f32_lenSq(onto));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from) {
    return m_V3f32_sub(v, m_V3f32_project(v, from));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal) {
    return m_V3f32_sub(v, m_V3f32_scal(normal, 2.0f * m_V3f32_dot(v, normal)));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_rotate(const m_V3f32 v, const m_V3f32 axis, const f32 angle) {
    const m_V3f32 normalized_axis = m_V3f32_norm(axis);
    const m_V2f32 r = m_V2f32_sincos(angle);
    const f32 one_sub_cos = 1.0f - r.x;
    return m_V3f32_add(
        m_V3f32_add(
            m_V3f32_scal(v, r.x),
            m_V3f32_scal(m_V3f32_cross(normalized_axis, v), r.y)
        ),
        m_V3f32_scal(
            normalized_axis,
            m_V3f32_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_perp(const m_V3f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return v.x != 0.0f || v.y != 0.0f
             ? m_V3f32_of(-v.y, v.x, 0.0f)
             : m_V3f32_of(0.0f, -v.z, v.y);
}

/* Vec4f functions */
/* Construction */
$attr($inline_always)
$static m_V4f32 m_V4f32_of(f32 x, f32 y, f32 z, f32 w) {
#if arch_simd_use
    let result = Vec_from$((f32){ x, y, z, w });
    return lit$((m_V4f32){ .a = Vec_toA$((FieldType$(m_V4f32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V4f32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4f32 m_V4f32_splat(f32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(4, f32))scalar);
    return lit$((m_V4f32){ .a = Vec_toA$((FieldType$(m_V4f32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_from2(m_V2f32 v) {
    return m_V4f32_of(v.x, v.y, 0.0f, 0.0f);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_from3(m_V3f32 v) {
    return m_V4f32_of(v.x, v.y, v.z, 0.0f);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4f32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 4; ++i) {
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApx$((m_V4f32)(lhs, rhs, threshold)) {
#if arch_simd_use
/* TODO: Implement Vec_ordApx for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 4; ++i) {
        // Calculate absolute difference
        let diff = flt_abs(lhs.s[i] - rhs.s[i]);
        // Check if difference is greater than threshold
        if (diff < threshold.s[i]) { continue; }
        // If difference is significant, compare normally
        if (lhs.s[i] < rhs.s[i]) { return cmp_Ord_lt; }
        if (lhs.s[i] > rhs.s[i]) { return cmp_Ord_gt; }
    }
    // If all components are within threshold, consider equal
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4f32 m_V4f32_neg(m_V4f32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_inv(m_V4f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V4f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_mod(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V4f32_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$(4, f32))scalar));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(4, f32))scalar));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4f32 m_V4f32_addScalar(m_V4f32 v, f32 scalar) {
    return m_V4f32_add(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_subScalar(m_V4f32 v, f32 scalar) {
    return m_V4f32_sub(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_mulScalar(m_V4f32 v, f32 scalar) {
    return m_V4f32_mul(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_divScalar(m_V4f32 v, f32 scalar) {
    return m_V4f32_div(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_modScalar(m_V4f32 v, f32 scalar) {
    return m_V4f32_mod(v, m_V4f32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(lhs.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_min(m_V4f32_max(v, min), max);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp01(m_V4f32 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V4f32_zero.a), Vec_fromA(m_V4f32_one.a));
    return lit$((m_V4f32){ .a = Vec_toA$((TypeOf(v.a))(result)) });
#endif /* arch_simd_use */
    return m_V4f32_clamp(v, m_V4f32_zero, m_V4f32_one);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V4f32_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap01(m_V4f32 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V4f32_wrap(v, m_V4f32_zero, m_V4f32_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f32 m_V4f32_lenSq(m_V4f32 v) {
    return m_V4f32_dot(v, v);
}
$attr($inline_always)
$static f32 m_V4f32_len(m_V4f32 v) {
    return flt_sqrt(m_V4f32_lenSq(v));
}
$attr($inline_always)
$static f32 m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_lenSq(m_V4f32_sub(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_len(m_V4f32_sub(lhs, rhs));
}
$attr($inline_always)
$static f32 m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return result;
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$attr($inline_always)
$static m_V4f32 m_V4f32_norm(m_V4f32 v) {
    const f32 len = m_V4f32_len(v);
    return len <= 0.0f ? m_V4f32_zero : m_V4f32_scal(v, 1.0f / len);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto) {
    return m_V4f32_scal(onto, m_V4f32_dot(v, onto) / m_V4f32_lenSq(onto));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from) {
    return m_V4f32_sub(v, m_V4f32_project(v, from));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal) {
    return m_V4f32_sub(v, m_V4f32_scal(normal, 2.0f * m_V4f32_dot(v, normal)));
}

/* Vec2d functions */
/* Construction */
$attr($inline_always)
$static m_V2f64 m_V2f64_of(f64 x, f64 y) {
#if arch_simd_use
    let result = Vec_from$((f64){ x, y });
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V2f64){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2f64 m_V2f64_splat(f64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(2, f64))scalar);
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_from3(m_V3f64 v) {
    return m_V2f64_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_from4(m_V4f64 v) {
    return m_V2f64_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_sincos(f64 radians) {
#if arch_simd_use
    let result = Vec_from$((f64){ flt_cos(radians), flt_sin(radians) });
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(flt_cos(radians), flt_sin(radians));
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 2; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2f64 m_V2f64_neg(m_V2f64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_inv(m_V2f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V2f64_of(1.0 / v.x, 1.0 / v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_mod(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V2f64_of(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}

$attr($inline_always)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(2, f64))scalar));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(2, f64))scaler));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(v.x / scaler, v.y / scaler);
}

$attr($inline_always)
$static m_V2f64 m_V2f64_addScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_add(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_subScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_sub(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_mulScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mul(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_divScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_div(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_modScalar(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mod(lhs, m_V2f64_splat(rhs));
}

$attr($inline_always)
$static m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_add(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_sub(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_mul(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_div(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_modAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_mod(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_scaleAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scal(*v, scalar), v;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_scaleInvAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scalInv(*v, scalar), v;
}

$attr($inline_always)
$static m_V2f64* m_V2f64_addScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_addScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_subScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_subScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_mulScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_mulScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_divScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_divScalar(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_modScalarAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_modScalar(*lhs, rhs), lhs;
}

/* Range Operations */
$attr($inline_always)
$static m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_of(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_min(m_V2f64_max(v, min), max);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp01(m_V2f64 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V2f64_zero.a), Vec_fromA(m_V2f64_one.a));
    return lit$((m_V2f64){ .a = Vec_toA$((FieldType$(m_V2f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2f64_clamp(v, m_V2f64_zero, m_V2f64_one);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V2f64_of(math_wrap(v.x, min.x, max.x), math_wrap(v.y, min.y, max.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap01(m_V2f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V2f64_wrap(v, m_V2f64_zero, m_V2f64_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V2f64_lenSq(m_V2f64 v) {
    return m_V2f64_dot(v, v);
}
$attr($inline_always)
$static f64 m_V2f64_len(m_V2f64 v) {
    return flt_sqrt(m_V2f64_lenSq(v));
}
$attr($inline_always)
$static f64 m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_lenSq(m_V2f64_sub(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs) {
    return flt_sqrt(m_V2f64_distSq(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((f64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_use
    let l = Vec_from$((f64){ lhs.x, lhs.y, 0.0 });
    let r = Vec_from$((f64){ rhs.x, rhs.y, 0.0 });
    let result = Vec_cross(l, r);
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) }).z;
#endif /* arch_simd_use */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static m_V2f64 m_V2f64_norm(m_V2f64 v) {
    const f64 len = m_V2f64_len(v);
    return len <= 0.0 ? m_V2f64_zero : m_V2f64_scal(v, 1.0 / len);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto) {
    return m_V2f64_scal(onto, m_V2f64_dot(v, onto) / m_V2f64_lenSq(onto));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from) {
    return m_V2f64_sub(v, m_V2f64_project(v, from));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal) {
    return m_V2f64_sub(v, m_V2f64_scal(normal, 2.0 * m_V2f64_dot(v, normal)));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 radians) {
#if arch_simd_use
/* TODO: Implement Vec_rotate for SIMD */
#endif /* arch_simd_use */
    const m_V2f64 r = m_V2f64_sincos(radians);
    return m_V2f64_of(
        v.x * r.x - v.y * r.y,
        v.x * r.y + v.y * r.x
    );
}
$attr($inline_always)
$static m_V2f64 m_V2f64_perp(m_V2f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return m_V2f64_of(-v.y, v.x);
}

/* Vec3d functions */
/* Construction */
$attr($inline_always)
$static m_V3f64 m_V3f64_of(f64 x, f64 y, f64 z) {
#if arch_simd_use
    let result = Vec_from$((f64){ x, y, z });
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V3f64){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3f64 m_V3f64_splat(f64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(3, f64))scalar);
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_from2(m_V2f64 v) {
    return m_V3f64_of(v.x, v.y, 0.0);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_from4(m_V4f64 v) {
    return m_V3f64_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3f64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 3; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3f64 m_V3f64_neg(m_V3f64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_inv(m_V3f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V3f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_mod(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V3f64_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(3, f64))scalar));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(3, f64))scalar));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3f64 m_V3f64_addScalar(m_V3f64 v, f64 scalar) {
    return m_V3f64_add(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_subScalar(m_V3f64 v, f64 scalar) {
    return m_V3f64_sub(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_mulScalar(m_V3f64 v, f64 scalar) {
    return m_V3f64_mul(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_divScalar(m_V3f64 v, f64 scalar) {
    return m_V3f64_div(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_modScalar(m_V3f64 v, f64 scalar) {
    return m_V3f64_mod(v, m_V3f64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_min(m_V3f64_max(v, min), max);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp01(m_V3f64 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V3f64_zero.a), Vec_fromA(m_V3f64_one.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_clamp(v, m_V3f64_zero, m_V3f64_one);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V3f64_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap01(m_V3f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V3f64_wrap(v, m_V3f64_zero, m_V3f64_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V3f64_lenSq(m_V3f64 v) {
    return m_V3f64_dot(v, v);
}
$attr($inline_always)
$static f64 m_V3f64_len(m_V3f64 v) {
    return flt_sqrt(m_V3f64_lenSq(v));
}
$attr($inline_always)
$static f64 m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_lenSq(m_V3f64_sub(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_len(m_V3f64_sub(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((f64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_use
    let result = Vec_cross(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3f64){ .a = Vec_toA$((FieldType$(m_V3f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3f64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_norm(m_V3f64 v) {
    const f64 len = m_V3f64_len(v);
    return len <= 0.0 ? m_V3f64_zero : m_V3f64_scal(v, 1.0 / len);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto) {
    return m_V3f64_scal(onto, m_V3f64_dot(v, onto) / m_V3f64_lenSq(onto));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from) {
    return m_V3f64_sub(v, m_V3f64_project(v, from));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal) {
    return m_V3f64_sub(v, m_V3f64_scal(normal, 2.0 * m_V3f64_dot(v, normal)));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle) {
#if arch_simd_use
/* TODO: Implement Vec_rotate for SIMD */
#endif /* arch_simd_use */
    const m_V3f64 normalized_axis = m_V3f64_norm(axis);
    const m_V2f64 r = m_V2f64_sincos(angle);
    const f64 one_sub_cos = 1.0 - r.x;
    return m_V3f64_add(
        m_V3f64_add(
            m_V3f64_scal(v, r.x),
            m_V3f64_scal(m_V3f64_cross(normalized_axis, v), r.y)
        ),
        m_V3f64_scal(
            normalized_axis,
            m_V3f64_dot(normalized_axis, v) * one_sub_cos
        )
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_perp(m_V3f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return v.x != 0.0 || v.y != 0.0
             ? m_V3f64_of(-v.y, v.x, 0.0)
             : m_V3f64_of(0.0, -v.z, v.y);
}

/* Vec4d functions */
/* Construction */
$attr($inline_always)
$static m_V4f64 m_V4f64_of(f64 x, f64 y, f64 z, f64 w) {
#if arch_simd_use
    let result = Vec_from$((f64){ x, y, z, w });
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V4f64){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4f64 m_V4f64_splat(f64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(4, f64))scalar);
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_from2(m_V2f64 v) {
    return m_V4f64_of(v.x, v.y, 0.0, 0.0);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_from3(m_V3f64 v) {
    return m_V4f64_of(v.x, v.y, v.z, 0.0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4f64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 4; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4f64 m_V4f64_neg(m_V4f64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_inv(m_V4f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_inv for SIMD */
#endif /* arch_simd_use */
    return m_V4f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_mod(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V4f64_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(4, f64))scalar));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(4, f64))scalar));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4f64 m_V4f64_addScalar(m_V4f64 v, f64 scalar) {
    return m_V4f64_add(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_subScalar(m_V4f64 v, f64 scalar) {
    return m_V4f64_sub(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_mulScalar(m_V4f64 v, f64 scalar) {
    return m_V4f64_mul(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_divScalar(m_V4f64 v, f64 scalar) {
    return m_V4f64_div(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_modScalar(m_V4f64 v, f64 scalar) {
    return m_V4f64_mod(v, m_V4f64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_min(m_V4f64_max(v, min), max);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp01(m_V4f64 v) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(m_V4f64_zero.a), Vec_fromA(m_V4f64_one.a));
    return lit$((m_V4f64){ .a = Vec_toA$((FieldType$(m_V4f64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4f64_clamp(v, m_V4f64_zero, m_V4f64_one);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V4f64_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap01(m_V4f64 v) {
#if arch_simd_use
/* TODO: Implement Vec_wrap01 for SIMD */
#endif /* arch_simd_use */
    return m_V4f64_wrap(v, m_V4f64_zero, m_V4f64_one);
}

/* Geometric Operations */
$attr($inline_always)
$static f64 m_V4f64_lenSq(m_V4f64 v) {
    return m_V4f64_dot(v, v);
}
$attr($inline_always)
$static f64 m_V4f64_len(m_V4f64 v) {
    return flt_sqrt(m_V4f64_lenSq(v));
}
$attr($inline_always)
$static f64 m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_lenSq(m_V4f64_sub(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_len(m_V4f64_sub(lhs, rhs));
}
$attr($inline_always)
$static f64 m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((f64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
$attr($inline_always)
$static m_V4f64 m_V4f64_norm(m_V4f64 v) {
    const f64 len = m_V4f64_len(v);
    return len <= 0.0 ? m_V4f64_zero : m_V4f64_scal(v, 1.0 / len);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto) {
    return m_V4f64_scal(onto, m_V4f64_dot(v, onto) / m_V4f64_lenSq(onto));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from) {
    return m_V4f64_sub(v, m_V4f64_project(v, from));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal) {
    return m_V4f64_sub(v, m_V4f64_scal(normal, 2.0 * m_V4f64_dot(v, normal)));
}

/* Vec2i functions */
/* Construction */
$attr($inline_always)
$static m_V2i32 m_V2i32_of(i32 x, i32 y) {
#if arch_simd_use
    let result = Vec_from$((i32){ x, y });
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V2i32){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2i32 m_V2i32_splat(i32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(2, i32))scalar);
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_from3(m_V3i32 v) {
    return m_V2i32_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_from4(m_V4i32 v) {
    return m_V2i32_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2i32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 2; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2i32 m_V2i32_neg(m_V2i32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mod(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V2i32_of(math_mod(lhs.x, rhs.x), math_mod(lhs.y, rhs.y));
}

$attr($inline_always)
$static m_V2i32 m_V2i32_scal(m_V2i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(2, i32))scalar));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_scalInv(m_V2i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(2, i32))scalar));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2i32 m_V2i32_addScalar(m_V2i32 v, i32 scalar) {
    return m_V2i32_add(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_subScalar(m_V2i32 v, i32 scalar) {
    return m_V2i32_sub(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mulScalar(m_V2i32 v, i32 scalar) {
    return m_V2i32_mul(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_divScalar(m_V2i32 v, i32 scalar) {
    return m_V2i32_div(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_modScalar(m_V2i32 v, i32 scalar) {
    return m_V2i32_mod(v, m_V2i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2i32 m_V2i32_min(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(math_min(lhs.x, rhs.x), math_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_of(math_max(lhs.x, rhs.x), math_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V2i32){ .a = Vec_toA$((FieldType$(m_V2i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i32_min(m_V2i32_max(v, min), max);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V2i32_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V2i32_lenSq(m_V2i32 v) {
    return m_V2i32_dot(v, v);
}
$attr($inline_always)
$static i32 m_V2i32_distSq(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_lenSq(m_V2i32_sub(lhs, rhs));
}
$attr($inline_always)
$static i32 m_V2i32_dot(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i32)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static i32 m_V2i32_cross(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_use
    let l = Vec_from$((i32){ lhs.x, lhs.y, 0 });
    let r = Vec_from$((i32){ rhs.x, rhs.y, 0 });
    let result = Vec_cross(l, r);
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) }).z;
#endif /* arch_simd_use */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static m_V2i32 m_V2i32_perp(m_V2i32 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return m_V2i32_of(-v.y, v.x);
}

/* Vec3i functions */
/* Construction */
$attr($inline_always)
$static m_V3i32 m_V3i32_of(i32 x, i32 y, i32 z) {
#if arch_simd_use
    let result = Vec_from$((i32){ x, y, z });
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V3i32){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3i32 m_V3i32_from2(m_V2i32 v) {
    return m_V3i32_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_from4(m_V4i32 v) {
    return m_V3i32_of(v.x, v.y, v.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_splat(i32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(3, i32))scalar);
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(scalar, scalar, scalar);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 3; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3i32 m_V3i32_neg(m_V3i32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mod(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V3i32_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3i32 m_V3i32_scal(m_V3i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(3, i32))scalar));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_scalInv(m_V3i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(3, i32))scalar));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3i32 m_V3i32_addScalar(m_V3i32 v, i32 scalar) {
    return m_V3i32_add(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_subScalar(m_V3i32 v, i32 scalar) {
    return m_V3i32_sub(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mulScalar(m_V3i32 v, i32 scalar) {
    return m_V3i32_mul(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_divScalar(m_V3i32 v, i32 scalar) {
    return m_V3i32_div(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_modScalar(m_V3i32 v, i32 scalar) {
    return m_V3i32_mod(v, m_V3i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3i32 m_V3i32_min(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i32 m_V3i32_max(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i32 m_V3i32_clamp(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_min(m_V3i32_max(v, min), max);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V3i32_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V3i32_lenSq(m_V3i32 v) {
    return m_V3i32_dot(v, v);
}
$attr($inline_always)
$static i32 m_V3i32_distSq(m_V3i32 lhs, m_V3i32 rhs) {
    return m_V3i32_lenSq(m_V3i32_sub(lhs, rhs));
}
$attr($inline_always)
$static i32 m_V3i32_dot(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i32)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3i32 m_V3i32_cross(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_use
    let l = Vec_from$((i32){ lhs.x, lhs.y, 0 });
    let r = Vec_from$((i32){ rhs.x, rhs.y, 0 });
    let result = Vec_cross(l, r);
    return lit$((m_V3i32){ .a = Vec_toA$((FieldType$(m_V3i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i32_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static m_V3i32 m_V3i32_perp(m_V3i32 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return v.x != 0 || v.y != 0
             ? m_V3i32_of(-v.y, v.x, 0)
             : m_V3i32_of(0, -v.z, v.y);
}

/* Vec4i functions */
/* Construction */
$attr($inline_always)
$static m_V4i32 m_V4i32_of(i32 x, i32 y, i32 z, i32 w) {
#if arch_simd_use
    let result = Vec_from$((i32){ x, y, z, w });
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return (m_V4i32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4i32 m_V4i32_splat(i32 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(4, i32))scalar);
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_from2(m_V2i32 v) {
    return m_V4i32_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_from3(m_V3i32 v) {
    return m_V4i32_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4i32)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 4; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4i32 m_V4i32_neg(m_V4i32 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mod(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V4i32_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4i32 m_V4i32_scal(m_V4i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(4, i32))scalar));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_scalInv(m_V4i32 v, i32 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(4, i32))scalar));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4i32 m_V4i32_addScalar(m_V4i32 v, i32 scalar) {
    return m_V4i32_add(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_subScalar(m_V4i32 v, i32 scalar) {
    return m_V4i32_sub(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mulScalar(m_V4i32 v, i32 scalar) {
    return m_V4i32_mul(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_divScalar(m_V4i32 v, i32 scalar) {
    return m_V4i32_div(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_modScalar(m_V4i32 v, i32 scalar) {
    return m_V4i32_mod(v, m_V4i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4i32 m_V4i32_min(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i32 m_V4i32_max(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i32 m_V4i32_clamp(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V4i32){ .a = Vec_toA$((FieldType$(m_V4i32, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i32_min(m_V4i32_max(v, min), max);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V4i32_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i32 m_V4i32_lenSq(m_V4i32 v) {
    return m_V4i32_dot(v, v);
}
$attr($inline_always)
$static i32 m_V4i32_distSq(m_V4i32 lhs, m_V4i32 rhs) {
    return m_V4i32_lenSq(m_V4i32_sub(lhs, rhs));
}
$attr($inline_always)
$static i32 m_V4i32_dot(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i32)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2l functions */
/* Construction */
$attr($inline_always)
$static m_V2i64 m_V2i64_of(i64 x, i64 y) {
#if arch_simd_use
    let result = Vec_from$((i64){ x, y });
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V2i64){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2i64 m_V2i64_splat(i64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(2, i64))scalar);
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_from3(m_V3i64 v) {
    return m_V2i64_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_from4(m_V4i64 v) {
    return m_V2i64_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2i64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 2; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2i64 m_V2i64_neg(m_V2i64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_mod(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V2i64_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(2, i64))scalar));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(2, i64))scalar));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2i64 m_V2i64_addScalar(m_V2i64 v, i64 scalar) {
    return m_V2i64_add(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_subScalar(m_V2i64 v, i64 scalar) {
    return m_V2i64_sub(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_mulScalar(m_V2i64 v, i64 scalar) {
    return m_V2i64_mul(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_divScalar(m_V2i64 v, i64 scalar) {
    return m_V2i64_div(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_modScalar(m_V2i64 v, i64 scalar) {
    return m_V2i64_mod(v, m_V2i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V2i64){ .a = Vec_toA$((FieldType$(m_V2i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V2i64_min(m_V2i64_max(v, min), max);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V2i64_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V2i64_lenSq(m_V2i64 v) {
    return m_V2i64_dot(v, v);
}
$attr($inline_always)
$static i64 m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_lenSq(m_V2i64_sub(lhs, rhs));
}
$attr($inline_always)
$static i64 m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_use
    let l = Vec_from$((i64){ lhs.x, lhs.y, 0 });
    let r = Vec_from$((i64){ rhs.x, rhs.y, 0 });
    let result = Vec_cross(l, r);
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) }).z;
#endif /* arch_simd_use */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static m_V2i64 m_V2i64_perp(m_V2i64 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return m_V2i64_of(-v.y, v.x);
}

/* Vec3l functions */
/* Construction */
$attr($inline_always)
$static m_V3i64 m_V3i64_of(i64 x, i64 y, i64 z) {
#if arch_simd_use
    let result = Vec_from$((i64){ x, y, z });
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V3i64){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3i64 m_V3i64_splat(i64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(3, i64))scalar);
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_from2(m_V2i64 v) {
    return m_V3i64_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_from4(m_V4i64 v) {
    return m_V3i64_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 3; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3i64 m_V3i64_neg(m_V3i64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_mod(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V3i64_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(3, i64))scalar));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(3, i64))scalar));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3i64 m_V3i64_addScalar(m_V3i64 v, i64 scalar) {
    return m_V3i64_add(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_subScalar(m_V3i64 v, i64 scalar) {
    return m_V3i64_sub(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_mulScalar(m_V3i64 v, i64 scalar) {
    return m_V3i64_mul(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_divScalar(m_V3i64 v, i64 scalar) {
    return m_V3i64_div(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_modScalar(m_V3i64 v, i64 scalar) {
    return m_V3i64_mod(v, m_V3i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_min(m_V3i64_max(v, min), max);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V3i64_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V3i64_lenSq(m_V3i64 v) {
    return m_V3i64_dot(v, v);
}
$attr($inline_always)
$static i64 m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_lenSq(m_V3i64_sub(lhs, rhs));
}
$attr($inline_always)
$static i64 m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_use
    let result = Vec_cross(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V3i64){ .a = Vec_toA$((FieldType$(m_V3i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V3i64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static m_V3i64 m_V3i64_perp(m_V3i64 v) {
#if arch_simd_use
/* TODO: Implement Vec_perp for SIMD */
#endif /* arch_simd_use */
    return v.x != 0 || v.y != 0
             ? m_V3i64_of(-v.y, v.x, 0)
             : m_V3i64_of(0, -v.z, v.y);
}

/* Vec4l functions */
/* Construction */
$attr($inline_always)
$static m_V4i64 m_V4i64_of(i64 x, i64 y, i64 z, i64 w) {
#if arch_simd_use
    let result = Vec_from$((i64){ x, y, z, w });
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return (m_V4i64){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4i64 m_V4i64_splat(i64 scalar) {
#if arch_simd_use
    let result = Vec_splat$((Vec$$(4, i64))scalar);
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_from2(m_V2i64 v) {
    return m_V4i64_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_from3(m_V3i64 v) {
    return m_V4i64_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4i64)(lhs, rhs)) {
#if arch_simd_use
/* TODO: Implement Vec_ord for SIMD */
#endif /* arch_simd_use */
    for (usize i = 0; i < 4; ++i) {
        if (lhs.s[i] < rhs.s[i]) {
            return cmp_Ord_lt;
        }
        if (lhs.s[i] > rhs.s[i]) {
            return cmp_Ord_gt;
        }
    }
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4i64 m_V4i64_neg(m_V4i64 v) {
#if arch_simd_use
    let result = Vec_neg(Vec_fromA(v.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_add(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_sub(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_mod(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
/* TODO: Implement Vec_mod for SIMD */
#endif /* arch_simd_use */
    return m_V4i64_of(
        math_mod(lhs.x, rhs.x),
        math_mod(lhs.y, rhs.y),
        math_mod(lhs.z, rhs.z),
        math_mod(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_mul(Vec_fromA(v.a), Vec_splat$((Vec$$(4, i64))scalar));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar) {
#if arch_simd_use
    let result = Vec_div(Vec_fromA(v.a), Vec_splat$((Vec$$(4, i64))scalar));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4i64 m_V4i64_addScalar(m_V4i64 v, i64 scalar) {
    return m_V4i64_add(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_subScalar(m_V4i64 v, i64 scalar) {
    return m_V4i64_sub(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_mulScalar(m_V4i64 v, i64 scalar) {
    return m_V4i64_mul(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_divScalar(m_V4i64 v, i64 scalar) {
    return m_V4i64_div(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_modScalar(m_V4i64 v, i64 scalar) {
    return m_V4i64_mod(v, m_V4i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_min(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(
        math_min(lhs.x, rhs.x),
        math_min(lhs.y, rhs.y),
        math_min(lhs.z, rhs.z),
        math_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_max(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_of(
        math_max(lhs.x, rhs.x),
        math_max(lhs.y, rhs.y),
        math_max(lhs.z, rhs.z),
        math_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_use
    let result = Vec_clamp(Vec_fromA(v.a), Vec_fromA(min.a), Vec_fromA(max.a));
    return lit$((m_V4i64){ .a = Vec_toA$((FieldType$(m_V4i64, a))(result)) });
#endif /* arch_simd_use */
    return m_V4i64_min(m_V4i64_max(v, min), max);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_use
/* TODO: Implement Vec_wrap for SIMD */
#endif /* arch_simd_use */
    return m_V4i64_of(
        math_wrap(v.x, min.x, max.x),
        math_wrap(v.y, min.y, max.y),
        math_wrap(v.z, min.z, max.z),
        math_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static i64 m_V4i64_lenSq(m_V4i64 v) {
    return m_V4i64_dot(v, v);
}
$attr($inline_always)
$static i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_lenSq(m_V4i64_sub(lhs, rhs));
}
$attr($inline_always)
$static i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_use
    let result = Vec_dot(Vec_fromA(lhs.a), Vec_fromA(rhs.a));
    return lit$((i64)(result));
#endif /* arch_simd_use */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

#endif /* MATH_VEC_INCLUDED */
