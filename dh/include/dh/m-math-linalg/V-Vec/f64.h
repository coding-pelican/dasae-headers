#pragma once
#ifndef m_math_linalg_V_Vec_f64__included
#define m_math_linalg_V_Vec_f64__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/*--- Float64 (f64) Vector ---*/

/* Vec2d functions */
/* Constants */
#define m_V2f64_zero m_V2f64_of_static(0.0, 0.0)
#define m_V2f64_unit_x m_V2f64_of_static(1.0, 0.0)
#define m_V2f64_unit_y m_V2f64_of_static(0.0, 1.0)
#define m_V2f64_one m_V2f64_of_static(1.0, 1.0)
#define m_V2f64_left m_V2f64_of_static(-1.0, 0.0)
#define m_V2f64_up m_V2f64_of_static(0.0, 1.0)
#define m_V2f64_right m_V2f64_of_static(1.0, 0.0)
#define m_V2f64_down m_V2f64_of_static(0.0, -1.0)
#define m_V2f64_eps m_V2f64_of_static(f64_eps, f64_eps)
#define m_V2f64_nan m_V2f64_of_static(f64_nan, f64_nan)
#define m_V2f64_inf m_V2f64_of_static(f64_inf, f64_inf)
#define m_V2f64_limit_min m_V2f64_of_static(f64_limit_min, f64_limit_min)
#define m_V2f64_limit_max m_V2f64_of_static(f64_limit_max, f64_limit_max)

/* Construction */
#define m_V2f64_of_static(_x, _y...) l$((m_V2f64){ .x = (_x), .y = (_y) })
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_of(f64 x, f64 y);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_splat(f64 scalar);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_from3(m_V3f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_from4(m_V4f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_sincos(f64 radians);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V2f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V2f64)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V2f64)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V2f64)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V2f64)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_neg(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_inv(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_rem(m_V2f64 lhs, m_V2f64 rhs);

$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler);

$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_addS(m_V2f64 lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_subS(m_V2f64 lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_mulS(m_V2f64 lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_divS(m_V2f64 lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_remS(m_V2f64 lhs, f64 rhs);

$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_remAsg(m_V2f64* lhs, m_V2f64 rhs);

$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_scalAsg(m_V2f64* v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_scalInvAsg(m_V2f64* v, f64 scalar);

$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_addSAsg(m_V2f64* lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_subSAsg(m_V2f64* lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_mulSAsg(m_V2f64* lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_divSAsg(m_V2f64* lhs, f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_remSAsg(m_V2f64* lhs, f64 rhs);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_clamp01(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_wrap01(m_V2f64 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V2f64_lenSq(m_V2f64 v);
$attr(m_linalg_$inline)
$static f64 m_V2f64_len(m_V2f64 v);
$attr(m_linalg_$inline)
$static f64 m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V2f64_det(m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V2f64_orient(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V2f64_orientDir(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f64_isCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f64_isCollinear(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2f64_isCCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_cw(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_ccw(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_skew(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_norm(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 angle);
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_perp(m_V2f64 v);

/* Vec3d functions */
/* Constants */
#define m_V3f64_zero m_V3f64_of_static(0.0, 0.0, 0.0)
#define m_V3f64_unit_x m_V3f64_of_static(1.0, 0.0, 0.0)
#define m_V3f64_unit_y m_V3f64_of_static(0.0, 1.0, 0.0)
#define m_V3f64_unit_z m_V3f64_of_static(0.0, 0.0, 1.0)
#define m_V3f64_unit_xy m_V3f64_of_static(1.0, 1.0, 0.0)
#define m_V3f64_unit_yz m_V3f64_of_static(0.0, 1.0, 1.0)
#define m_V3f64_unit_zx m_V3f64_of_static(1.0, 0.0, 1.0)
#define m_V3f64_one m_V3f64_of_static(1.0, 1.0, 1.0)
#define m_V3f64_left m_V3f64_of_static(-1.0, 0.0, 0.0)
#define m_V3f64_up m_V3f64_of_static(0.0, 1.0, 0.0)
#define m_V3f64_right m_V3f64_of_static(1.0, 0.0, 0.0)
#define m_V3f64_down m_V3f64_of_static(0.0, -1.0, 0.0)
#define m_V3f64_forward m_V3f64_of_static(0.0, 0.0, 1.0)
#define m_V3f64_backward m_V3f64_of_static(0.0, 0.0, -1.0)
#define m_V3f64_view_forward_rh m_V3f64_backward
#define m_V3f64_view_backward_rh m_V3f64_forward
#define m_V3f64_view_forward_lh m_V3f64_forward
#define m_V3f64_view_backward_lh m_V3f64_backward
#define m_V3f64_eps m_V3f64_of_static(f64_eps, f64_eps, f64_eps)
#define m_V3f64_nan m_V3f64_of_static(f64_nan, f64_nan, f64_nan)
#define m_V3f64_inf m_V3f64_of_static(f64_inf, f64_inf, f64_inf)

/* Construction */
#define m_V3f64_of_static(_x, _y, _z...) l$((m_V3f64){ .x = (_x), .y = (_y), .z = (_z) })
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_of(f64 x, f64 y, f64 z);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_splat(f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_from2(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_from4(m_V4f64 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V3f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V3f64)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V3f64)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V3f64)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V3f64)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_neg(m_V3f64 v);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_inv(m_V3f64 v);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_rem(m_V3f64 lhs, m_V3f64 rhs);

$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar);

$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_addS(m_V3f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_subS(m_V3f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_mulS(m_V3f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_divS(m_V3f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_remS(m_V3f64 v, f64 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_clamp01(m_V3f64 v);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_wrap01(m_V3f64 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V3f64_lenSq(m_V3f64 v);
$attr(m_linalg_$inline)
$static f64 m_V3f64_len(m_V3f64 v);
$attr(m_linalg_$inline)
$static f64 m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V3f64_det(m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V3f64_orient(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V3f64_orientDir(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f64_isOrientNeg(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f64_isCoplanar(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3f64_isOrientPos(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_norm(m_V3f64 v);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle);
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_perp(m_V3f64 v);

/* Vec4d functions */
/* Constants */
#define m_V4f64_zero m_V4f64_of_static(0.0, 0.0, 0.0, 0.0)
#define m_V4f64_unit_x m_V4f64_of_static(1.0, 0.0, 0.0, 0.0)
#define m_V4f64_unit_y m_V4f64_of_static(0.0, 1.0, 0.0, 0.0)
#define m_V4f64_unit_z m_V4f64_of_static(0.0, 0.0, 1.0, 0.0)
#define m_V4f64_unit_w m_V4f64_of_static(0.0, 0.0, 0.0, 1.0)
#define m_V4f64_one m_V4f64_of_static(1.0, 1.0, 1.0, 1.0)
#define m_V4f64_left m_V4f64_of_static(-1.0, 0.0, 0.0, 0.0)
#define m_V4f64_up m_V4f64_of_static(0.0, 1.0, 0.0, 0.0)
#define m_V4f64_right m_V4f64_of_static(1.0, 0.0, 0.0, 0.0)
#define m_V4f64_down m_V4f64_of_static(0.0, -1.0, 0.0, 0.0)
#define m_V4f64_forward m_V4f64_of_static(0.0, 0.0, 1.0, 0.0)
#define m_V4f64_backward m_V4f64_of_static(0.0, 0.0, -1.0, 0.0)
#define m_V4f64_view_forward_rh m_V4f64_backward
#define m_V4f64_view_backward_rh m_V4f64_forward
#define m_V4f64_view_forward_lh m_V4f64_forward
#define m_V4f64_view_backward_lh m_V4f64_backward
#define m_V4f64_eps m_V4f64_of_static(f64_eps, f64_eps, f64_eps, f64_eps)
#define m_V4f64_nan m_V4f64_of_static(f64_nan, f64_nan, f64_nan, f64_nan)
#define m_V4f64_inf m_V4f64_of_static(f64_inf, f64_inf, f64_inf, f64_inf)
#define m_V4f64_limit_min m_V4f64_of_static(f64_limit_min, f64_limit_min, f64_limit_min, f64_limit_min)
#define m_V4f64_limit_max m_V4f64_of_static(f64_limit_max, f64_limit_max, f64_limit_max, f64_limit_max)

/* Construction */
#define m_V4f64_of_static(_x, _y, _z, _w...) l$((m_V4f64){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_of(f64 x, f64 y, f64 z, f64 w);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_splat(f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_from2(m_V2f64 v);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_from3(m_V3f64 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V4f64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V4f64)(lhs, rhs));

$attr(m_linalg_$inline)
$static cmp_fn_ordApx$((m_V4f64)(lhs, rhs, threshold, mode));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxAbs$((m_V4f64)(lhs, rhs, threshold));
$attr(m_linalg_$inline)
$static cmp_fn_ordApxRel$((m_V4f64)(lhs, rhs, threshold));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_neg(m_V4f64 v);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_inv(m_V4f64 v);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_rem(m_V4f64 lhs, m_V4f64 rhs);

$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar);

$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_addS(m_V4f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_subS(m_V4f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_mulS(m_V4f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_divS(m_V4f64 v, f64 scalar);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_remS(m_V4f64 v, f64 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_clamp01(m_V4f64 v);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_wrap01(m_V4f64 v);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V4f64_lenSq(m_V4f64 v);
$attr(m_linalg_$inline)
$static f64 m_V4f64_len(m_V4f64 v);
$attr(m_linalg_$inline)
$static f64 m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static f64 m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_norm(m_V4f64 v);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from);
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2d functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_of(f64 x, f64 y) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y });
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return (m_V2f64){ .x = x, .y = y };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat2$(f64, scalar);
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_from3(m_V3f64 v) {
    return m_V2f64_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_from4(m_V4f64 v) {
    return m_V2f64_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_sincos(f64 radians) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ flt_cos(radians), flt_sin(radians) });
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(flt_cos(radians), flt_sin(radians));
#endif /* arch_simd_supported */
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2f64)(lhs, rhs)) {
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
$static cmp_fn_ordApxAbs$((m_V2f64)(lhs, rhs, threshold)) {
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
$static cmp_fn_ordApxRel$((m_V2f64)(lhs, rhs, threshold)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_neg(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(-v.x, -v.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_inv(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(1.0 / v.x, 1.0 / v.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(lhs.x + rhs.x, lhs.y + rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(lhs.x - rhs.x, lhs.y - rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(lhs.x * rhs.x, lhs.y * rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(lhs.x / rhs.x, lhs.y / rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_rem(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from2(lhs), m_V_simd_from2(rhs), flt_rem);
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(v), m_V_simd_splat2$(f64, scalar));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(v.x * scalar, v.y * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(v), m_V_simd_splat2$(f64, scaler));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(v.x / scaler, v.y / scaler);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_addS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_add(lhs, m_V2f64_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_subS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_sub(lhs, m_V2f64_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_mulS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mul(lhs, m_V2f64_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_divS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_div(lhs, m_V2f64_splat(rhs));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_remS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_rem(lhs, m_V2f64_splat(rhs));
}

$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_add(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_sub(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_mul(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_div(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_remAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_rem(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_scalAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scal(*v, scalar), v;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_scalInvAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scalInv(*v, scalar), v;
}

$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_addSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_addS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_subSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_subS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_mulSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_mulS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_divSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_divS(*lhs, rhs), lhs;
}
$attr(m_linalg_$inline)
$static m_V2f64* m_V2f64_remSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_remS(*lhs, rhs), lhs;
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(pri_min(lhs.x, rhs.x), pri_min(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(pri_max(lhs.x, rhs.x), pri_max(lhs.y, rhs.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_min(m_V2f64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_clamp01(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(m_V2f64_zero), m_V_simd_from2(m_V2f64_one));
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_clamp(v, m_V2f64_zero, m_V2f64_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max), flt_wrap);
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    return m_V2f64_of(flt_wrap(v.x, min.x, max.x), flt_wrap(v.y, min.y, max.y));
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_wrap01(m_V2f64 v) {
    return m_V2f64_wrap(v, m_V2f64_zero, m_V2f64_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V2f64_lenSq(m_V2f64 v) {
    return m_V2f64_dot(v, v);
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_len(m_V2f64 v) {
    return flt_sqrt(m_V2f64_lenSq(v));
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_distSq(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_lenSq(m_V2f64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_dist(m_V2f64 lhs, m_V2f64 rhs) {
    return flt_sqrt(m_V2f64_distSq(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_dot(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return l$((f64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((f64){ lhs.x, lhs.y, 0.0, 0 });
    let r = simd_V_from$((f64){ rhs.x, rhs.y, 0.0, 0 });
    let result = simd_V_cross3(l, r);
    return m_V_simd_to3$(m_V3f64, result).z;
#else /* !arch_simd_supported */
    return lhs.x * rhs.y - lhs.y * rhs.x;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_norm(m_V2f64 v) {
    const f64 len = m_V2f64_len(v);
    return len <= 0.0 ? m_V2f64_zero : m_V2f64_scal(v, 1.0 / len);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_project(m_V2f64 v, m_V2f64 onto) {
    return m_V2f64_scal(onto, m_V2f64_dot(v, onto) / m_V2f64_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_reject(m_V2f64 v, m_V2f64 from) {
    return m_V2f64_sub(v, m_V2f64_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_reflect(m_V2f64 v, m_V2f64 normal) {
    return m_V2f64_sub(v, m_V2f64_scal(normal, 2.0 * m_V2f64_dot(v, normal)));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_rotate(m_V2f64 v, f64 radians) {
#if arch_simd_supported
    const m_V2f64 rotation = m_V2f64_sincos(radians);
    let source = m_V_simd_from2(v);
    let swapped = simd_V_shuffle(source, source, 1, 0);
    let result = simd_V_add(
        simd_V_mul(source, m_V_simd_splat2$(f64, rotation.x)),
        simd_V_mul(swapped, simd_V_from$((f64){ -rotation.y, rotation.y }))
    );
    return m_V_simd_to2$(m_V2f64, result);
#else /* !arch_simd_supported */
    const m_V2f64 r = m_V2f64_sincos(radians);
    return m_V2f64_of(
        v.x * r.x - v.y * r.y,
        v.x * r.y + v.y * r.x
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_det(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_cross(lhs, rhs);
}
$attr(m_linalg_$inline)
$static f64 m_V2f64_orient(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_det(m_V2f64_sub(lhs, origin), m_V2f64_sub(rhs, origin));
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V2f64_orientDir(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    let det = m_V2f64_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V2f64_isCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCW(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2f64_isCollinear(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCollinear(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2f64_isCCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCCW(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_cw(m_V2f64 v) {
    return m_V2f64_of(v.y, -v.x);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_ccw(m_V2f64 v) {
    return m_V2f64_of(-v.y, v.x);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_skew(m_V2f64 v) {
    return m_V2f64_ccw(v);
}
$attr(m_linalg_$inline)
$static m_V2f64 m_V2f64_perp(m_V2f64 v) {
    return m_V2f64_ccw(v);
}

/* Vec3d functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_of(f64 x, f64 y, f64 z) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y, z, 0 });
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return (m_V3f64){ .x = x, .y = y, .z = z };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat3$(f64, scalar);
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(scalar, scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_from2(m_V2f64 v) {
    return m_V3f64_of(v.x, v.y, 0.0);
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_from4(m_V4f64 v) {
    return m_V3f64_of(v.x, v.y, v.z);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3f64)(lhs, rhs)) {
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
$static cmp_fn_ordApxAbs$((m_V3f64)(lhs, rhs, threshold)) {
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
$static cmp_fn_ordApxRel$((m_V3f64)(lhs, rhs, threshold)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_neg(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from3(v));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(-v.x, -v.y, -v.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_inv(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from3Divisor(v));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_rem(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs), flt_rem);
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(v), m_V_simd_splat3$(f64, scalar));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(v.x * scalar, v.y * scalar, v.z * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(v), m_V_simd_splat3$(f64, scalar));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(v.x / scalar, v.y / scalar, v.z / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_addS(m_V3f64 v, f64 scalar) {
    return m_V3f64_add(v, m_V3f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_subS(m_V3f64 v, f64 scalar) {
    return m_V3f64_sub(v, m_V3f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_mulS(m_V3f64 v, f64 scalar) {
    return m_V3f64_mul(v, m_V3f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_divS(m_V3f64 v, f64 scalar) {
    return m_V3f64_div(v, m_V3f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_remS(m_V3f64 v, f64 scalar) {
    return m_V3f64_rem(v, m_V3f64_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3(max));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_min(m_V3f64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_clamp01(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(m_V3f64_zero), m_V_simd_from3(m_V3f64_one));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_clamp(v, m_V3f64_zero, m_V3f64_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3Divisor(max), flt_wrap);
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_wrap01(m_V3f64 v) {
    return m_V3f64_wrap(v, m_V3f64_zero, m_V3f64_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V3f64_lenSq(m_V3f64 v) {
    return m_V3f64_dot(v, v);
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_len(m_V3f64 v) {
    return flt_sqrt(m_V3f64_lenSq(v));
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_distSq(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_lenSq(m_V3f64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_dist(m_V3f64 lhs, m_V3f64 rhs) {
    return m_V3f64_len(m_V3f64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_dot(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return l$((f64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return m_V3f64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_det(m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V3f64_dot(lhs, m_V3f64_cross(mid, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V3f64_orient(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V3f64_det(
        m_V3f64_sub(lhs, origin),
        m_V3f64_sub(mid, origin),
        m_V3f64_sub(rhs, origin)
    );
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V3f64_orientDir(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    let det = m_V3f64_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V3f64_isOrientNeg(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isNeg(m_V3f64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3f64_isCoplanar(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isCoplanar(m_V3f64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3f64_isOrientPos(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isPos(m_V3f64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_norm(m_V3f64 v) {
    const f64 len = m_V3f64_len(v);
    return len <= 0.0 ? m_V3f64_zero : m_V3f64_scal(v, 1.0 / len);
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_project(m_V3f64 v, m_V3f64 onto) {
    return m_V3f64_scal(onto, m_V3f64_dot(v, onto) / m_V3f64_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_reject(m_V3f64 v, m_V3f64 from) {
    return m_V3f64_sub(v, m_V3f64_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_reflect(m_V3f64 v, m_V3f64 normal) {
    return m_V3f64_sub(v, m_V3f64_scal(normal, 2.0 * m_V3f64_dot(v, normal)));
}
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_rotate(m_V3f64 v, m_V3f64 axis, f64 angle) {
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
$attr(m_linalg_$inline)
$static m_V3f64 m_V3f64_perp(m_V3f64 v) {
#if arch_simd_supported
    let source = m_V_simd_from3(v);
    if (v.x != 0.0 || v.y != 0.0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((f64){ -1.0, 1.0, 0.0, 0.0 })
        );
        return m_V_simd_to3$(m_V3f64, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((f64){ 0.0, -1.0, 1.0, 0.0 })
    );
    return m_V_simd_to3$(m_V3f64, result);
#else /* !arch_simd_supported */
    return v.x != 0.0 || v.y != 0.0
             ? m_V3f64_of(-v.y, v.x, 0.0)
             : m_V3f64_of(0.0, -v.z, v.y);
#endif /* arch_simd_supported */
}

/* Vec4d functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_of(f64 x, f64 y, f64 z, f64 w) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y, z, w });
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return (m_V4f64){ .x = x, .y = y, .z = z, .w = w };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat4$(f64, scalar);
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(scalar, scalar, scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_from2(m_V2f64 v) {
    return m_V4f64_of(v.x, v.y, 0.0, 0.0);
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_from3(m_V3f64 v) {
    return m_V4f64_of(v.x, v.y, v.z, 0.0);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4f64)(lhs, rhs)) {
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
$static cmp_fn_ordApxAbs$((m_V4f64)(lhs, rhs, threshold)) {
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
$static cmp_fn_ordApxRel$((m_V4f64)(lhs, rhs, threshold)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_neg(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(-v.x, -v.y, -v.z, -v.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_inv(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_rem(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from4(lhs), m_V_simd_from4(rhs), flt_rem);
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z),
        flt_rem(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(v), m_V_simd_splat4$(f64, scalar));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(v), m_V_simd_splat4$(f64, scalar));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_addS(m_V4f64 v, f64 scalar) {
    return m_V4f64_add(v, m_V4f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_subS(m_V4f64 v, f64 scalar) {
    return m_V4f64_sub(v, m_V4f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_mulS(m_V4f64 v, f64 scalar) {
    return m_V4f64_mul(v, m_V4f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_divS(m_V4f64 v, f64 scalar) {
    return m_V4f64_div(v, m_V4f64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_remS(m_V4f64 v, f64 scalar) {
    return m_V4f64_rem(v, m_V4f64_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_min(m_V4f64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_clamp01(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(m_V4f64_zero), m_V_simd_from4(m_V4f64_one));
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_clamp(v, m_V4f64_zero, m_V4f64_one);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max), flt_wrap);
    return m_V_simd_to4$(m_V4f64, result);
#else /* !arch_simd_supported */
    return m_V4f64_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z),
        flt_wrap(v.w, min.w, max.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_wrap01(m_V4f64 v) {
    return m_V4f64_wrap(v, m_V4f64_zero, m_V4f64_one);
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static f64 m_V4f64_lenSq(m_V4f64 v) {
    return m_V4f64_dot(v, v);
}
$attr(m_linalg_$inline)
$static f64 m_V4f64_len(m_V4f64 v) {
    return flt_sqrt(m_V4f64_lenSq(v));
}
$attr(m_linalg_$inline)
$static f64 m_V4f64_distSq(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_lenSq(m_V4f64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V4f64_dist(m_V4f64 lhs, m_V4f64 rhs) {
    return m_V4f64_len(m_V4f64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static f64 m_V4f64_dot(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return l$((f64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_norm(m_V4f64 v) {
    const f64 len = m_V4f64_len(v);
    return len <= 0.0 ? m_V4f64_zero : m_V4f64_scal(v, 1.0 / len);
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_project(m_V4f64 v, m_V4f64 onto) {
    return m_V4f64_scal(onto, m_V4f64_dot(v, onto) / m_V4f64_lenSq(onto));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_reject(m_V4f64 v, m_V4f64 from) {
    return m_V4f64_sub(v, m_V4f64_project(v, from));
}
$attr(m_linalg_$inline)
$static m_V4f64 m_V4f64_reflect(m_V4f64 v, m_V4f64 normal) {
    return m_V4f64_sub(v, m_V4f64_scal(normal, 2.0 * m_V4f64_dot(v, normal)));
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_f64__included */
