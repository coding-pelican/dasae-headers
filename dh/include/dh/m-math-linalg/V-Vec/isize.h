#pragma once
#ifndef m_math_linalg_V_Vec_isize__included
#define m_math_linalg_V_Vec_isize__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/*--- Isize (isize) Vector ---*/

/* Vec2z functions */
/* Constants */
#define m_V2isize_zero m_V2isize_of_static(0, 0)
#define m_V2isize_unit_x m_V2isize_of_static(1, 0)
#define m_V2isize_unit_y m_V2isize_of_static(0, 1)
#define m_V2isize_one m_V2isize_of_static(1, 1)
#define m_V2isize_left m_V2isize_of_static(-1, 0)
#define m_V2isize_up m_V2isize_of_static(0, 1)
#define m_V2isize_right m_V2isize_of_static(1, 0)
#define m_V2isize_down m_V2isize_of_static(0, -1)
#define m_V2isize_limit_min m_V2isize_of_static(isize_limit_min, isize_limit_min)
#define m_V2isize_limit_max m_V2isize_of_static(isize_limit_max, isize_limit_max)

/* Construction */
#define m_V2isize_of_static(_x, _y...) l$((m_V2isize){ .x = (_x), .y = (_y) })
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_of(isize x, isize y);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_splat(isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_from3(m_V3isize v);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_from4(m_V4isize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V2isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V2isize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_neg(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_add(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_sub(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_mul(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_div(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_rem(m_V2isize lhs, m_V2isize rhs);

$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_scal(m_V2isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_scalInv(m_V2isize v, isize scalar);

$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_addS(m_V2isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_subS(m_V2isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_mulS(m_V2isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_divS(m_V2isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_remS(m_V2isize v, isize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_min(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_max(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_clamp(m_V2isize v, m_V2isize min, m_V2isize max);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_wrap(m_V2isize v, m_V2isize min, m_V2isize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V2isize_lenSq(m_V2isize v);
$attr(m_linalg_$inline)
$static isize m_V2isize_distSq(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static isize m_V2isize_dot(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static isize m_V2isize_cross(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static isize m_V2isize_det(m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static isize m_V2isize_orient(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V2isize_orientDir(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static bool m_V2isize_isCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static bool m_V2isize_isCollinear(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static bool m_V2isize_isCCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_cw(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_ccw(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_skew(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_perp(m_V2isize v);

/* Vec3z functions */
/* Constants */
#define m_V3isize_zero m_V3isize_of_static(0, 0, 0)
#define m_V3isize_unit_x m_V3isize_of_static(1, 0, 0)
#define m_V3isize_unit_y m_V3isize_of_static(0, 1, 0)
#define m_V3isize_unit_z m_V3isize_of_static(0, 0, 1)
#define m_V3isize_unit_xy m_V3isize_of_static(1, 1, 0)
#define m_V3isize_unit_yz m_V3isize_of_static(0, 1, 1)
#define m_V3isize_unit_zx m_V3isize_of_static(1, 0, 1)
#define m_V3isize_one m_V3isize_of_static(1, 1, 1)
#define m_V3isize_left m_V3isize_of_static(-1, 0, 0)
#define m_V3isize_up m_V3isize_of_static(0, 1, 0)
#define m_V3isize_right m_V3isize_of_static(1, 0, 0)
#define m_V3isize_down m_V3isize_of_static(0, -1, 0)
#define m_V3isize_forward m_V3isize_of_static(0, 0, 1)
#define m_V3isize_backward m_V3isize_of_static(0, 0, -1)
#define m_V3isize_view_forward_rh m_V3isize_backward
#define m_V3isize_view_backward_rh m_V3isize_forward
#define m_V3isize_view_forward_lh m_V3isize_forward
#define m_V3isize_view_backward_lh m_V3isize_backward
#define m_V3isize_limit_min m_V3isize_of_static(isize_limit_min, isize_limit_min, isize_limit_min)
#define m_V3isize_limit_max m_V3isize_of_static(isize_limit_max, isize_limit_max, isize_limit_max)

/* Construction */
#define m_V3isize_of_static(_x, _y, _z...) l$((m_V3isize){ .x = (_x), .y = (_y), .z = (_z) })
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_of(isize x, isize y, isize z);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_splat(isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_from2(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_from4(m_V4isize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V3isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V3isize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_neg(m_V3isize v);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_add(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_sub(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_mul(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_div(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_rem(m_V3isize lhs, m_V3isize rhs);

$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_scal(m_V3isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_scalInv(m_V3isize v, isize scalar);

$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_addS(m_V3isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_subS(m_V3isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_mulS(m_V3isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_divS(m_V3isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_remS(m_V3isize v, isize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_min(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_max(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_clamp(m_V3isize v, m_V3isize min, m_V3isize max);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_wrap(m_V3isize v, m_V3isize min, m_V3isize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V3isize_lenSq(m_V3isize v);
$attr(m_linalg_$inline)
$static isize m_V3isize_distSq(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static isize m_V3isize_dot(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_cross(m_V3isize lhs, m_V3isize rhs);
$attr(m_linalg_$inline)
$static isize m_V3isize_det(m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static isize m_V3isize_orient(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V3isize_orientDir(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static bool m_V3isize_isOrientNeg(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static bool m_V3isize_isCoplanar(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static bool m_V3isize_isOrientPos(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_perp(m_V3isize v);

/* Vec4z functions */
/* Constants */
#define m_V4isize_zero m_V4isize_of_static(0, 0, 0, 0)
#define m_V4isize_unit_x m_V4isize_of_static(1, 0, 0, 0)
#define m_V4isize_unit_y m_V4isize_of_static(0, 1, 0, 0)
#define m_V4isize_unit_z m_V4isize_of_static(0, 0, 1, 0)
#define m_V4isize_unit_w m_V4isize_of_static(0, 0, 0, 1)
#define m_V4isize_one m_V4isize_of_static(1, 1, 1, 1)
#define m_V4isize_left m_V4isize_of_static(-1, 0, 0, 0)
#define m_V4isize_up m_V4isize_of_static(0, 1, 0, 0)
#define m_V4isize_right m_V4isize_of_static(1, 0, 0, 0)
#define m_V4isize_down m_V4isize_of_static(0, -1, 0, 0)
#define m_V4isize_forward m_V4isize_of_static(0, 0, 1, 0)
#define m_V4isize_backward m_V4isize_of_static(0, 0, -1, 0)
#define m_V4isize_view_forward_rh m_V4isize_backward
#define m_V4isize_view_backward_rh m_V4isize_forward
#define m_V4isize_view_forward_lh m_V4isize_forward
#define m_V4isize_view_backward_lh m_V4isize_backward
#define m_V4isize_limit_min m_V4isize_of_static(isize_limit_min, isize_limit_min, isize_limit_min, isize_limit_min)
#define m_V4isize_limit_max m_V4isize_of_static(isize_limit_max, isize_limit_max, isize_limit_max, isize_limit_max)

/* Construction */
#define m_V4isize_of_static(_x, _y, _z, _w...) l$((m_V4isize){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_of(isize x, isize y, isize z, isize w);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_splat(isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_from2(m_V2isize v);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_from3(m_V3isize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V4isize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V4isize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_neg(m_V4isize v);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_add(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_sub(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_mul(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_div(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_rem(m_V4isize lhs, m_V4isize rhs);

$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_scal(m_V4isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_scalInv(m_V4isize v, isize scalar);

$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_addS(m_V4isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_subS(m_V4isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_mulS(m_V4isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_divS(m_V4isize v, isize scalar);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_remS(m_V4isize v, isize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_min(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_max(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_clamp(m_V4isize v, m_V4isize min, m_V4isize max);
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_wrap(m_V4isize v, m_V4isize min, m_V4isize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V4isize_lenSq(m_V4isize v);
$attr(m_linalg_$inline)
$static isize m_V4isize_distSq(m_V4isize lhs, m_V4isize rhs);
$attr(m_linalg_$inline)
$static isize m_V4isize_dot(m_V4isize lhs, m_V4isize rhs);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2z functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_of(isize x, isize y) {
    return (m_V2isize){ .x = x, .y = y };
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_splat(isize scalar) {
    return m_V2isize_of(scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_from3(m_V3isize v) {
    return m_V2isize_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_from4(m_V4isize v) {
    return m_V2isize_of(v.x, v.y);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_neg(m_V2isize v) {
    return m_V2isize_of(-v.x, -v.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_add(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_sub(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_mul(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_div(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_rem(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_scal(m_V2isize v, isize scalar) {
    return m_V2isize_of(v.x * scalar, v.y * scalar);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_scalInv(m_V2isize v, isize scalar) {
    return m_V2isize_of(v.x / scalar, v.y / scalar);
}

$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_addS(m_V2isize v, isize scalar) {
    return m_V2isize_add(v, m_V2isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_subS(m_V2isize v, isize scalar) {
    return m_V2isize_sub(v, m_V2isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_mulS(m_V2isize v, isize scalar) {
    return m_V2isize_mul(v, m_V2isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_divS(m_V2isize v, isize scalar) {
    return m_V2isize_div(v, m_V2isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_remS(m_V2isize v, isize scalar) {
    return m_V2isize_rem(v, m_V2isize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_min(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_max(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_clamp(m_V2isize v, m_V2isize min, m_V2isize max) {
    return m_V2isize_min(m_V2isize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_wrap(m_V2isize v, m_V2isize min, m_V2isize max) {
    return m_V2isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V2isize_lenSq(m_V2isize v) {
    return m_V2isize_dot(v, v);
}
$attr(m_linalg_$inline)
$static isize m_V2isize_distSq(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_lenSq(m_V2isize_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static isize m_V2isize_dot(m_V2isize lhs, m_V2isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr(m_linalg_$inline)
$static isize m_V2isize_cross(m_V2isize lhs, m_V2isize rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr(m_linalg_$inline)
$static isize m_V2isize_det(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_cross(lhs, rhs);
}
$attr(m_linalg_$inline)
$static isize m_V2isize_orient(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_det(m_V2isize_sub(lhs, origin), m_V2isize_sub(rhs, origin));
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V2isize_orientDir(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    let det = m_V2isize_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V2isize_isCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCW(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2isize_isCollinear(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCollinear(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2isize_isCCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCCW(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_cw(m_V2isize v) {
    return m_V2isize_of(v.y, -v.x);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_ccw(m_V2isize v) {
    return m_V2isize_of(-v.y, v.x);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_skew(m_V2isize v) {
    return m_V2isize_ccw(v);
}
$attr(m_linalg_$inline)
$static m_V2isize m_V2isize_perp(m_V2isize v) {
    return m_V2isize_of(-v.y, v.x);
}

/* Vec3z functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_of(isize x, isize y, isize z) {
    return (m_V3isize){ .x = x, .y = y, .z = z };
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_splat(isize scalar) {
    return m_V3isize_of(scalar, scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_from2(m_V2isize v) {
    return m_V3isize_of(v.x, v.y, 0);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_from4(m_V4isize v) {
    return m_V3isize_of(v.x, v.y, v.z);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_neg(m_V3isize v) {
    return m_V3isize_of(-v.x, -v.y, -v.z);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_add(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_sub(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_mul(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_div(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_rem(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_scal(m_V3isize v, isize scalar) {
    return m_V3isize_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_scalInv(m_V3isize v, isize scalar) {
    return m_V3isize_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_addS(m_V3isize v, isize scalar) {
    return m_V3isize_add(v, m_V3isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_subS(m_V3isize v, isize scalar) {
    return m_V3isize_sub(v, m_V3isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_mulS(m_V3isize v, isize scalar) {
    return m_V3isize_mul(v, m_V3isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_divS(m_V3isize v, isize scalar) {
    return m_V3isize_div(v, m_V3isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_remS(m_V3isize v, isize scalar) {
    return m_V3isize_rem(v, m_V3isize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_min(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_max(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_clamp(m_V3isize v, m_V3isize min, m_V3isize max) {
    return m_V3isize_min(m_V3isize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_wrap(m_V3isize v, m_V3isize min, m_V3isize max) {
    return m_V3isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V3isize_lenSq(m_V3isize v) {
    return m_V3isize_dot(v, v);
}
$attr(m_linalg_$inline)
$static isize m_V3isize_distSq(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_lenSq(m_V3isize_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static isize m_V3isize_dot(m_V3isize lhs, m_V3isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_cross(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr(m_linalg_$inline)
$static isize m_V3isize_det(m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V3isize_dot(lhs, m_V3isize_cross(mid, rhs));
}
$attr(m_linalg_$inline)
$static isize m_V3isize_orient(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V3isize_det(
        m_V3isize_sub(lhs, origin),
        m_V3isize_sub(mid, origin),
        m_V3isize_sub(rhs, origin)
    );
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V3isize_orientDir(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    let det = m_V3isize_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V3isize_isOrientNeg(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isNeg(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3isize_isCoplanar(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isCoplanar(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3isize_isOrientPos(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isPos(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static m_V3isize m_V3isize_perp(m_V3isize v) {
    return v.x != 0 || v.y != 0
             ? m_V3isize_of(-v.y, v.x, 0)
             : m_V3isize_of(0, -v.z, v.y);
}

/* Vec4z functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_of(isize x, isize y, isize z, isize w) {
    return (m_V4isize){ .x = x, .y = y, .z = z, .w = w };
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_splat(isize scalar) {
    return m_V4isize_of(scalar, scalar, scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_from2(m_V2isize v) {
    return m_V4isize_of(v.x, v.y, 0, 0);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_from3(m_V3isize v) {
    return m_V4isize_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_neg(m_V4isize v) {
    return m_V4isize_of(-v.x, -v.y, -v.z, -v.w);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_add(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_sub(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_mul(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_div(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_rem(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_scal(m_V4isize v, isize scalar) {
    return m_V4isize_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_scalInv(m_V4isize v, isize scalar) {
    return m_V4isize_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_addS(m_V4isize v, isize scalar) {
    return m_V4isize_add(v, m_V4isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_subS(m_V4isize v, isize scalar) {
    return m_V4isize_sub(v, m_V4isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_mulS(m_V4isize v, isize scalar) {
    return m_V4isize_mul(v, m_V4isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_divS(m_V4isize v, isize scalar) {
    return m_V4isize_div(v, m_V4isize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_remS(m_V4isize v, isize scalar) {
    return m_V4isize_rem(v, m_V4isize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_min(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_max(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_clamp(m_V4isize v, m_V4isize min, m_V4isize max) {
    return m_V4isize_min(m_V4isize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V4isize m_V4isize_wrap(m_V4isize v, m_V4isize min, m_V4isize max) {
    return m_V4isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static isize m_V4isize_lenSq(m_V4isize v) {
    return m_V4isize_dot(v, v);
}
$attr(m_linalg_$inline)
$static isize m_V4isize_distSq(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_lenSq(m_V4isize_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static isize m_V4isize_dot(m_V4isize lhs, m_V4isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_isize__included */
