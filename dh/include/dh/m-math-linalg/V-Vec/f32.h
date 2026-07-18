#pragma once
#ifndef m_math_linalg_V_Vec_f32__included
#define m_math_linalg_V_Vec_f32__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Float32 (f32) Vector --- */

/* Vec2f functions */
/* Constants */
#define m_V2f32_zero m_V2f32_of_static(0.0f, 0.0f)
#define m_V2f32_unit_x m_V2f32_of_static(1.0f, 0.0f)
#define m_V2f32_unit_y m_V2f32_of_static(0.0f, 1.0f)
#define m_V2f32_one m_V2f32_of_static(1.0f, 1.0f)
#define m_V2f32_left m_V2f32_of_static(-1.0f, 0.0f)
#define m_V2f32_up m_V2f32_of_static(0.0f, 1.0f)
#define m_V2f32_right m_V2f32_of_static(1.0f, 0.0f)
#define m_V2f32_down m_V2f32_of_static(0.0f, -1.0f)
#define m_V2f32_eps m_V2f32_of_static(f32_eps, f32_eps)
#define m_V2f32_nan m_V2f32_of_static(f32_nan, f32_nan)
#define m_V2f32_inf m_V2f32_of_static(f32_inf, f32_inf)
#define m_V2f32_limit_min m_V2f32_of_static(f32_limit_min, f32_limit_min)
#define m_V2f32_limit_max m_V2f32_of_static(f32_limit_max, f32_limit_max)

/* Construction */
#define m_V2f32_of_static(_x, _y...) l$((m_V2f32){ .x = (_x), .y = (_y) })
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_of(f32 x, f32 y);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_splat(f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_from3(m_V3f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_from4(m_V4f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_sincos(f32 radians);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V2f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V2f32)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V2f32)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V2f32)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V2f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_neg(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_inv(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_rem(m_V2f32 lhs, m_V2f32 rhs);

$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar);

$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_addS(m_V2f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_subS(m_V2f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_mulS(m_V2f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_divS(m_V2f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_remS(m_V2f32 v, f32 scalar);

$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_remAsg(m_V2f32* lhs, m_V2f32 rhs);

$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_scalAsg(m_V2f32* v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_scalInvAsg(m_V2f32* v, f32 scalar);

$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_addSAsg(m_V2f32* v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_subSAsg(m_V2f32* v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_mulSAsg(m_V2f32* v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_divSAsg(m_V2f32* v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_remSAsg(m_V2f32* v, f32 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_clamp01(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_wrap01(m_V2f32 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V2f32_lenSq(m_V2f32 v);
$attr(m_linalg_$inline)
$static f32 m_V2f32_len(m_V2f32 v);
$attr(m_linalg_$inline)
$static f32 m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V2f32_det(m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V2f32_orient(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V2f32_orientDir(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f32_isCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f32_isCollinear(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f32_isCCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_cw(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_ccw(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_skew(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_norm(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 angle);
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_perp(m_V2f32 v);

/* Vec3f functions */
/* Constants */
#define m_V3f32_zero m_V3f32_of_static(0.0f, 0.0f, 0.0f)
#define m_V3f32_unit_x m_V3f32_of_static(1.0f, 0.0f, 0.0f)
#define m_V3f32_unit_y m_V3f32_of_static(0.0f, 1.0f, 0.0f)
#define m_V3f32_unit_z m_V3f32_of_static(0.0f, 0.0f, 1.0f)
#define m_V3f32_unit_xy m_V3f32_of_static(1.0f, 1.0f, 0.0f)
#define m_V3f32_unit_yz m_V3f32_of_static(0.0f, 1.0f, 1.0f)
#define m_V3f32_unit_zx m_V3f32_of_static(1.0f, 0.0f, 1.0f)
#define m_V3f32_one m_V3f32_of_static(1.0f, 1.0f, 1.0f)
#define m_V3f32_left m_V3f32_of_static(-1.0f, 0.0f, 0.0f)
#define m_V3f32_up m_V3f32_of_static(0.0f, 1.0f, 0.0f)
#define m_V3f32_right m_V3f32_of_static(1.0f, 0.0f, 0.0f)
#define m_V3f32_down m_V3f32_of_static(0.0f, -1.0f, 0.0f)
#define m_V3f32_forward m_V3f32_of_static(0.0f, 0.0f, 1.0f)
#define m_V3f32_backward m_V3f32_of_static(0.0f, 0.0f, -1.0f)
#define m_V3f32_view_forward_rh m_V3f32_backward
#define m_V3f32_view_backward_rh m_V3f32_forward
#define m_V3f32_view_forward_lh m_V3f32_forward
#define m_V3f32_view_backward_lh m_V3f32_backward
#define m_V3f32_eps m_V3f32_of_static(f32_eps, f32_eps, f32_eps)
#define m_V3f32_nan m_V3f32_of_static(f32_nan, f32_nan, f32_nan)
#define m_V3f32_inf m_V3f32_of_static(f32_inf, f32_inf, f32_inf)
#define m_V3f32_limit_min m_V3f32_of_static(f32_limit_min, f32_limit_min, f32_limit_min)
#define m_V3f32_limit_max m_V3f32_of_static(f32_limit_max, f32_limit_max, f32_limit_max)

/* Construction */
#define m_V3f32_of_static(_x, _y, _z...) l$((m_V3f32){ .x = (_x), .y = (_y), .z = (_z) })
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_of(f32 x, f32 y, f32 z);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_splat(f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_from2(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_from4(m_V4f32 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V3f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V3f32)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V3f32)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V3f32)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V3f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_neg(m_V3f32 v);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_inv(m_V3f32 v);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_rem(m_V3f32 lhs, m_V3f32 rhs);

$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar);

$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_addS(m_V3f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_subS(m_V3f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_mulS(m_V3f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_divS(m_V3f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_remS(m_V3f32 v, f32 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_clamp01(m_V3f32 v);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_wrap01(m_V3f32 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V3f32_lenSq(m_V3f32 v);
$attr(m_linalg_$inline)
$static f32 m_V3f32_len(m_V3f32 v);
$attr(m_linalg_$inline)
$static f32 m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V3f32_det(m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V3f32_orient(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V3f32_orientDir(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f32_isOrientNeg(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f32_isCoplanar(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f32_isOrientPos(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_norm(m_V3f32 v);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_rotate(m_V3f32 v, m_V3f32 axis, f32 angle);
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_perp(m_V3f32 v);

/* Vec4f functions */
/* Constants */
#define m_V4f32_zero m_V4f32_of_static(0.0f, 0.0f, 0.0f, 0.0f)
#define m_V4f32_unit_x m_V4f32_of_static(1.0f, 0.0f, 0.0f, 0.0f)
#define m_V4f32_unit_y m_V4f32_of_static(0.0f, 1.0f, 0.0f, 0.0f)
#define m_V4f32_unit_z m_V4f32_of_static(0.0f, 0.0f, 1.0f, 0.0f)
#define m_V4f32_unit_w m_V4f32_of_static(0.0f, 0.0f, 0.0f, 1.0f)
#define m_V4f32_one m_V4f32_of_static(1.0f, 1.0f, 1.0f, 1.0f)
#define m_V4f32_left m_V4f32_of_static(-1.0f, 0.0f, 0.0f, 0.0f)
#define m_V4f32_up m_V4f32_of_static(0.0f, 1.0f, 0.0f, 0.0f)
#define m_V4f32_right m_V4f32_of_static(1.0f, 0.0f, 0.0f, 0.0f)
#define m_V4f32_down m_V4f32_of_static(0.0f, -1.0f, 0.0f, 0.0f)
#define m_V4f32_forward m_V4f32_of_static(0.0f, 0.0f, 1.0f, 0.0f)
#define m_V4f32_backward m_V4f32_of_static(0.0f, 0.0f, -1.0f, 0.0f)
#define m_V4f32_view_forward_rh m_V4f32_backward
#define m_V4f32_view_backward_rh m_V4f32_forward
#define m_V4f32_view_forward_lh m_V4f32_forward
#define m_V4f32_view_backward_lh m_V4f32_backward
#define m_V4f32_eps m_V4f32_of_static(f32_eps, f32_eps, f32_eps, f32_eps)
#define m_V4f32_nan m_V4f32_of_static(f32_nan, f32_nan, f32_nan, f32_nan)
#define m_V4f32_inf m_V4f32_of_static(f32_inf, f32_inf, f32_inf, f32_inf)
#define m_V4f32_limit_min m_V4f32_of_static(f32_limit_min, f32_limit_min, f32_limit_min, f32_limit_min)
#define m_V4f32_limit_max m_V4f32_of_static(f32_limit_max, f32_limit_max, f32_limit_max, f32_limit_max)

/* Construction */
#define m_V4f32_of_static(_x, _y, _z, _w...) l$((m_V4f32){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_of(f32 x, f32 y, f32 z, f32 w);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_splat(f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_from2(m_V2f32 v);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_from3(m_V3f32 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V4f32)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V4f32)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V4f32)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V4f32)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V4f32)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_neg(m_V4f32 v);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_inv(m_V4f32 v);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_rem(m_V4f32 lhs, m_V4f32 rhs);

$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar);

$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_addS(m_V4f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_subS(m_V4f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_mulS(m_V4f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_divS(m_V4f32 v, f32 scalar);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_remS(m_V4f32 v, f32 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_clamp01(m_V4f32 v);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_wrap01(m_V4f32 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V4f32_lenSq(m_V4f32 v);
$attr(m_linalg_$inline)
$static f32 m_V4f32_len(m_V4f32 v);
$attr(m_linalg_$inline)
$static f32 m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static f32 m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_norm(m_V4f32 v);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from);
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2f functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_of(f32 x, f32 y) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y });
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of_static(x, y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_splat(f32 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat2$(f32, scalar);
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of_static(scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_from3(m_V3f32 v) {
    return m_V2f32_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_from4(m_V4f32 v) {
    return m_V2f32_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_sincos(f32 radians) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ flt_cos(radians), flt_sin(radians) });
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(flt_cos(radians), flt_sin(radians));
#endif /* arch_simd_supported */
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2f32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V2f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_V_simd_from2(lhs), m_V_simd_from2(rhs), m_V_simd_from2(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V2f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_V_simd_from2(lhs), m_V_simd_from2(rhs), m_V_simd_from2(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApx_default$((m_V2f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_neg(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(-v.x, -v.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_inv(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(1.0f / v.x, 1.0f / v.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(lhs.x + rhs.x, lhs.y + rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(lhs.x - rhs.x, lhs.y - rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(lhs.x * rhs.x, lhs.y * rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(lhs.x / rhs.x, lhs.y / rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_rem(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_flt_rem(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(v), m_V_simd_splat2$(f32, scalar));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(v.x * scalar, v.y * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(v), m_V_simd_splat2$(f32, scalar));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(v.x / scalar, v.y / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_addS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_add(lhs, m_V2f32_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_subS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_sub(lhs, m_V2f32_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_mulS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mul(lhs, m_V2f32_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_divS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_div(lhs, m_V2f32_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_remS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_rem(lhs, m_V2f32_splat(rhs));
}

$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_add(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_sub(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_mul(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_div(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_remAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_rem(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_scalAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scal(*v, scalar), v;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_scalInvAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scalInv(*v, scalar), v;
}

$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_addSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_addS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_subSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_subS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_mulSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_mulS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_divSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_divS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f32* m_V2f32_remSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_remS(*lhs, rhs), lhs;
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(flt_min(lhs.x, rhs.x), flt_min(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(flt_max(lhs.x, rhs.x), flt_max(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_min(m_V2f32_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_clamp01(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(m_V2f32_zero), m_V_simd_from2(m_V2f32_one));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_clamp(v, m_V2f32_zero, m_V2f32_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_supported
    let result = simd_V_flt_wrap(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max));
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    return m_V2f32_of(flt_wrap(v.x, min.x, max.x), flt_wrap(v.y, min.y, max.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_wrap01(m_V2f32 v) {
    return m_V2f32_wrap(v, m_V2f32_zero, m_V2f32_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V2f32_lenSq(m_V2f32 v) {
    return m_V2f32_dot(v, v);
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_len(m_V2f32 v) {
    return flt_sqrt(m_V2f32_lenSq(v));
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_distSq(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_lenSq(m_V2f32_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_dist(m_V2f32 lhs, m_V2f32 rhs) {
    return flt_sqrt(m_V2f32_distSq(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_dot(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return l$((f32)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((f32){ lhs.x, lhs.y, 0.0f, 0 });
    let r = simd_V_from$((f32){ rhs.x, rhs.y, 0.0f, 0 });
    let result = simd_V_cross3(l, r);
    return m_V_simd_to3$(m_V3f32, result).z;
#else /* !arch_simd_supported */
    return lhs.x * rhs.y - lhs.y * rhs.x;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_norm(m_V2f32 v) {
    const f32 len = m_V2f32_len(v);
    return len <= 0.0f ? m_V2f32_zero : m_V2f32_scal(v, 1.0f / len);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_project(m_V2f32 v, m_V2f32 onto) {
    return m_V2f32_scal(onto, m_V2f32_dot(v, onto) / m_V2f32_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_reject(m_V2f32 v, m_V2f32 from) {
    return m_V2f32_sub(v, m_V2f32_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_reflect(m_V2f32 v, m_V2f32 normal) {
    return m_V2f32_sub(v, m_V2f32_scal(normal, 2.0f * m_V2f32_dot(v, normal)));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_rotate(m_V2f32 v, f32 radians) {
#if arch_simd_supported
    const m_V2f32 rotation = m_V2f32_sincos(radians);
    let source = m_V_simd_from2(v);
    let swapped = simd_V_shuffle(source, source, 1, 0);
    let result = simd_V_add(
        simd_V_mul(source, m_V_simd_splat2$(f32, rotation.x)),
        simd_V_mul(swapped, simd_V_from$((f32){ -rotation.y, rotation.y }))
    );
    return m_V_simd_to2$(m_V2f32, result);
#else /* !arch_simd_supported */
    const m_V2f32 r = m_V2f32_sincos(radians);
    return m_V2f32_of(v.x * r.x - v.y * r.y, v.x * r.y + v.y * r.x);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_det(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_cross(lhs, rhs);
}
$attr(m_linalg_$inline)
$static f32 m_V2f32_orient(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_det(m_V2f32_sub(lhs, origin), m_V2f32_sub(rhs, origin));
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V2f32_orientDir(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    let det = m_V2f32_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V2f32_isCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCW(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2f32_isCollinear(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCollinear(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2f32_isCCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCCW(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_cw(m_V2f32 v) {
    return m_V2f32_of(v.y, -v.x);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_ccw(m_V2f32 v) {
    return m_V2f32_of(-v.y, v.x);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_skew(m_V2f32 v) {
    return m_V2f32_ccw(v);
}
$attr(m_linalg_$inline)
$static m_V2f32 m_V2f32_perp(m_V2f32 v) {
    return m_V2f32_ccw(v);
}

/* Vec3f functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_of(f32 x, f32 y, f32 z) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y, z, 0 });
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return (m_V3f32){ .x = x, .y = y, .z = z };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_from2(m_V2f32 v) {
    return m_V3f32_of(v.x, v.y, 0.0f);
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_from4(m_V4f32 v) {
    return m_V3f32_of(v.x, v.y, v.z);
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_splat(f32 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat3$(f32, scalar);
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(scalar, scalar, scalar);
#endif /* arch_simd_supported */
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3f32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V3f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_V_simd_from3(lhs), m_V_simd_from3(rhs), m_V_simd_from3(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V3f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_V_simd_from3(lhs), m_V_simd_from3(rhs), m_V_simd_from3(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApx_default$((m_V3f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_neg(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from3(v));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(-v.x, -v.y, -v.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_inv(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from3Divisor(v));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_rem(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs), flt_rem);
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y), flt_rem(lhs.z, rhs.z));
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(v), m_V_simd_splat3$(f32, scalar));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(v.x * scalar, v.y * scalar, v.z * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(v), m_V_simd_splat3$(f32, scalar));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(v.x / scalar, v.y / scalar, v.z / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_addS(m_V3f32 v, f32 scalar) {
    return m_V3f32_add(v, m_V3f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_subS(m_V3f32 v, f32 scalar) {
    return m_V3f32_sub(v, m_V3f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_mulS(m_V3f32 v, f32 scalar) {
    return m_V3f32_mul(v, m_V3f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_divS(m_V3f32 v, f32 scalar) {
    return m_V3f32_div(v, m_V3f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_remS(m_V3f32 v, f32 scalar) {
    return m_V3f32_rem(v, m_V3f32_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3(max));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_min(m_V3f32_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_clamp01(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(m_V3f32_zero), m_V_simd_from3(m_V3f32_one));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_clamp(v, m_V3f32_zero, m_V3f32_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3Divisor(max), flt_wrap);
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_wrap01(m_V3f32 v) {
    return m_V3f32_wrap(v, m_V3f32_zero, m_V3f32_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V3f32_lenSq(m_V3f32 v) {
    return m_V3f32_dot(v, v);
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_len(m_V3f32 v) {
    return flt_sqrt(m_V3f32_lenSq(v));
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_distSq(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_lenSq(m_V3f32_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_dist(m_V3f32 lhs, m_V3f32 rhs) {
    return m_V3f32_len(m_V3f32_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_dot(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return l$((f32)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return m_V3f32_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_det(m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V3f32_dot(lhs, m_V3f32_cross(mid, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V3f32_orient(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V3f32_det(
        m_V3f32_sub(lhs, origin),
        m_V3f32_sub(mid, origin),
        m_V3f32_sub(rhs, origin)
    );
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V3f32_orientDir(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    let det = m_V3f32_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V3f32_isOrientNeg(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isNeg(m_V3f32_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3f32_isCoplanar(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isCoplanar(m_V3f32_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3f32_isOrientPos(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isPos(m_V3f32_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_norm(m_V3f32 v) {
    const f32 len = m_V3f32_len(v);
    return len <= 0.0f ? m_V3f32_zero : m_V3f32_scal(v, 1.0f / len);
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_project(m_V3f32 v, m_V3f32 onto) {
    return m_V3f32_scal(onto, m_V3f32_dot(v, onto) / m_V3f32_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_reject(m_V3f32 v, m_V3f32 from) {
    return m_V3f32_sub(v, m_V3f32_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_reflect(m_V3f32 v, m_V3f32 normal) {
    return m_V3f32_sub(v, m_V3f32_scal(normal, 2.0f * m_V3f32_dot(v, normal)));
}
$attr(m_linalg_$inline)
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
$attr(m_linalg_$inline)
$static m_V3f32 m_V3f32_perp(const m_V3f32 v) {
#if arch_simd_supported
    let source = m_V_simd_from3(v);
    if (v.x != 0.0f || v.y != 0.0f) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((f32){ -1.0f, 1.0f, 0.0f, 0.0f })
        );
        return m_V_simd_to3$(m_V3f32, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((f32){ 0.0f, -1.0f, 1.0f, 0.0f })
    );
    return m_V_simd_to3$(m_V3f32, result);
#else /* !arch_simd_supported */
    return v.x != 0.0f || v.y != 0.0f
             ? m_V3f32_of(-v.y, v.x, 0.0f)
             : m_V3f32_of(0.0f, -v.z, v.y);
#endif /* arch_simd_supported */
}

/* Vec4f functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_of(f32 x, f32 y, f32 z, f32 w) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y, z, w });
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return (m_V4f32){ .x = x, .y = y, .z = z, .w = w };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_splat(f32 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat4$(f32, scalar);
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(scalar, scalar, scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_from2(m_V2f32 v) {
    return m_V4f32_of(v.x, v.y, 0.0f, 0.0f);
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_from3(m_V3f32 v) {
    return m_V4f32_of(v.x, v.y, v.z, 0.0f);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4f32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V4f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_V_simd_from4(lhs), m_V_simd_from4(rhs), m_V_simd_from4(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V4f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_V_simd_from4(lhs), m_V_simd_from4(rhs), m_V_simd_from4(threshold));
#else /* !arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static cmp_fn_ordApx_default$((m_V4f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_neg(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(-v.x, -v.y, -v.z, -v.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_inv(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_rem(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from4(lhs), m_V_simd_from4(rhs), flt_rem);
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z),
        flt_rem(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(v), m_V_simd_splat4$(f32, scalar));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(v), m_V_simd_splat4$(f32, scalar));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_addS(m_V4f32 v, f32 scalar) {
    return m_V4f32_add(v, m_V4f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_subS(m_V4f32 v, f32 scalar) {
    return m_V4f32_sub(v, m_V4f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_mulS(m_V4f32 v, f32 scalar) {
    return m_V4f32_mul(v, m_V4f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_divS(m_V4f32 v, f32 scalar) {
    return m_V4f32_div(v, m_V4f32_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_remS(m_V4f32 v, f32 scalar) {
    return m_V4f32_rem(v, m_V4f32_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_min(m_V4f32_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_clamp01(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(m_V4f32_zero), m_V_simd_from4(m_V4f32_one));
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_clamp(v, m_V4f32_zero, m_V4f32_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max), flt_wrap);
    return m_V_simd_to4$(m_V4f32, result);
#else /* !arch_simd_supported */
    return m_V4f32_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z),
        flt_wrap(v.w, min.w, max.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_wrap01(m_V4f32 v) {
    return m_V4f32_wrap(v, m_V4f32_zero, m_V4f32_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f32 m_V4f32_lenSq(m_V4f32 v) {
    return m_V4f32_dot(v, v);
}
$attr(m_linalg_$inline)
$static f32 m_V4f32_len(m_V4f32 v) {
    return flt_sqrt(m_V4f32_lenSq(v));
}
$attr(m_linalg_$inline)
$static f32 m_V4f32_distSq(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_lenSq(m_V4f32_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V4f32_dist(m_V4f32 lhs, m_V4f32 rhs) {
    return m_V4f32_len(m_V4f32_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f32 m_V4f32_dot(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return result;
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_norm(m_V4f32 v) {
    const f32 len = m_V4f32_len(v);
    return len <= 0.0f ? m_V4f32_zero : m_V4f32_scal(v, 1.0f / len);
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_project(m_V4f32 v, m_V4f32 onto) {
    return m_V4f32_scal(onto, m_V4f32_dot(v, onto) / m_V4f32_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_reject(m_V4f32 v, m_V4f32 from) {
    return m_V4f32_sub(v, m_V4f32_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V4f32 m_V4f32_reflect(m_V4f32 v, m_V4f32 normal) {
    return m_V4f32_sub(v, m_V4f32_scal(normal, 2.0f * m_V4f32_dot(v, normal)));
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_f32__included */
