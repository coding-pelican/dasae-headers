#pragma once
#ifndef m_math_linalg_V_Vec_i64__included
#define m_math_linalg_V_Vec_i64__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/*--- Int64 (i64) Vector ---*/

/* Vec2l functions */
/* Constants */
#define m_V2i64_zero m_V2i64_of_static(0, 0)
#define m_V2i64_unit_x m_V2i64_of_static(1, 0)
#define m_V2i64_unit_y m_V2i64_of_static(0, 1)
#define m_V2i64_one m_V2i64_of_static(1, 1)
#define m_V2i64_left m_V2i64_of_static(-1, 0)
#define m_V2i64_up m_V2i64_of_static(0, 1)
#define m_V2i64_right m_V2i64_of_static(1, 0)
#define m_V2i64_down m_V2i64_of_static(0, -1)
#define m_V2i64_limit_min m_V2i64_of_static(i64_limit_min, i64_limit_min)
#define m_V2i64_limit_max m_V2i64_of_static(i64_limit_max, i64_limit_max)

/* Construction */
#define m_V2i64_of_static(_x, _y...) l$((m_V2i64){ .x = (_x), .y = (_y) })
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_of(i64 x, i64 y);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_splat(i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_from3(m_V3i64 v);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_from4(m_V4i64 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V2i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V2i64)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_neg(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_rem(m_V2i64 lhs, m_V2i64 rhs);

$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar);

$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_addS(m_V2i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_subS(m_V2i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_mulS(m_V2i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_divS(m_V2i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_remS(m_V2i64 v, i64 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V2i64_lenSq(m_V2i64 v);
$attr(m_linalg_$inline)
$static i64 m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V2i64_det(m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V2i64_orient(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V2i64_orientDir(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2i64_isCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2i64_isCollinear(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V2i64_isCCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_cw(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_ccw(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_skew(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_perp(m_V2i64 v);

/* Vec3l functions */
/* Constants */
#define m_V3i64_zero m_V3i64_of_static(0, 0, 0)
#define m_V3i64_unit_x m_V3i64_of_static(1, 0, 0)
#define m_V3i64_unit_y m_V3i64_of_static(0, 1, 0)
#define m_V3i64_unit_z m_V3i64_of_static(0, 0, 1)
#define m_V3i64_unit_xy m_V3i64_of_static(1, 1, 0)
#define m_V3i64_unit_yz m_V3i64_of_static(0, 1, 1)
#define m_V3i64_unit_zx m_V3i64_of_static(1, 0, 1)
#define m_V3i64_one m_V3i64_of_static(1, 1, 1)
#define m_V3i64_left m_V3i64_of_static(-1, 0, 0)
#define m_V3i64_up m_V3i64_of_static(0, 1, 0)
#define m_V3i64_right m_V3i64_of_static(1, 0, 0)
#define m_V3i64_down m_V3i64_of_static(0, -1, 0)
#define m_V3i64_forward m_V3i64_of_static(0, 0, 1)
#define m_V3i64_backward m_V3i64_of_static(0, 0, -1)
#define m_V3i64_view_forward_rh m_V3i64_backward
#define m_V3i64_view_backward_rh m_V3i64_forward
#define m_V3i64_view_forward_lh m_V3i64_forward
#define m_V3i64_view_backward_lh m_V3i64_backward
#define m_V3i64_limit_min m_V3i64_of_static(i64_limit_min, i64_limit_min, i64_limit_min)
#define m_V3i64_limit_max m_V3i64_of_static(i64_limit_max, i64_limit_max, i64_limit_max)

/* Construction */
#define m_V3i64_of_static(_x, _y, _z...) l$((m_V3i64){ .x = (_x), .y = (_y), .z = (_z) })
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_of(i64 x, i64 y, i64 z);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_splat(i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_from2(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_from4(m_V4i64 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V3i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V3i64)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_neg(m_V3i64 v);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_rem(m_V3i64 lhs, m_V3i64 rhs);

$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar);

$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_addS(m_V3i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_subS(m_V3i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_mulS(m_V3i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_divS(m_V3i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_remS(m_V3i64 v, i64 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V3i64_lenSq(m_V3i64 v);
$attr(m_linalg_$inline)
$static i64 m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V3i64_det(m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V3i64_orient(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V_Orient m_V3i64_orientDir(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3i64_isOrientNeg(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3i64_isCoplanar(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static bool m_V3i64_isOrientPos(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_perp(m_V3i64 v);

/* Vec4l functions */
/* Constants */
#define m_V4i64_zero m_V4i64_of_static(0, 0, 0, 0)
#define m_V4i64_unit_x m_V4i64_of_static(1, 0, 0, 0)
#define m_V4i64_unit_y m_V4i64_of_static(0, 1, 0, 0)
#define m_V4i64_unit_z m_V4i64_of_static(0, 0, 1, 0)
#define m_V4i64_unit_w m_V4i64_of_static(0, 0, 0, 1)
#define m_V4i64_one m_V4i64_of_static(1, 1, 1, 1)
#define m_V4i64_left m_V4i64_of_static(-1, 0, 0, 0)
#define m_V4i64_up m_V4i64_of_static(0, 1, 0, 0)
#define m_V4i64_right m_V4i64_of_static(1, 0, 0, 0)
#define m_V4i64_down m_V4i64_of_static(0, -1, 0, 0)
#define m_V4i64_forward m_V4i64_of_static(0, 0, 1, 0)
#define m_V4i64_backward m_V4i64_of_static(0, 0, -1, 0)
#define m_V4i64_view_forward_rh m_V4i64_backward
#define m_V4i64_view_backward_rh m_V4i64_forward
#define m_V4i64_view_forward_lh m_V4i64_forward
#define m_V4i64_view_backward_lh m_V4i64_backward
#define m_V4i64_limit_min m_V4i64_of_static(i64_limit_min, i64_limit_min, i64_limit_min, i64_limit_min)
#define m_V4i64_limit_max m_V4i64_of_static(i64_limit_max, i64_limit_max, i64_limit_max, i64_limit_max)

/* Construction */
#define m_V4i64_of_static(_x, _y, _z, _w...) l$((m_V4i64){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_of(i64 x, i64 y, i64 z, i64 w);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_splat(i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_from2(m_V2i64 v);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_from3(m_V3i64 v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V4i64)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V4i64)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_neg(m_V4i64 v);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_rem(m_V4i64 lhs, m_V4i64 rhs);

$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar);

$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_addS(m_V4i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_subS(m_V4i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_mulS(m_V4i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_divS(m_V4i64 v, i64 scalar);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_remS(m_V4i64 v, i64 scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max);
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V4i64_lenSq(m_V4i64 v);
$attr(m_linalg_$inline)
$static i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs);
$attr(m_linalg_$inline)
$static i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2l functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_of(i64 x, i64 y) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y });
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return (m_V2i64){ .x = x, .y = y };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat2$(i64, scalar);
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_from3(m_V3i64 v) {
    return m_V2i64_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_from4(m_V4i64 v) {
    return m_V2i64_of(v.x, v.y);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2i64)(lhs, rhs)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_neg(m_V2i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(-v.x, -v.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(lhs.x + rhs.x, lhs.y + rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(lhs.x - rhs.x, lhs.y - rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(lhs.x * rhs.x, lhs.y * rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(lhs.x / rhs.x, lhs.y / rhs.y);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_rem(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from2(lhs), m_V_simd_from2(rhs), int_rem);
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(v), m_V_simd_splat2$(i64, scalar));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(v.x * scalar, v.y * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(v), m_V_simd_splat2$(i64, scalar));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(v.x / scalar, v.y / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_addS(m_V2i64 v, i64 scalar) {
    return m_V2i64_add(v, m_V2i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_subS(m_V2i64 v, i64 scalar) {
    return m_V2i64_sub(v, m_V2i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_mulS(m_V2i64 v, i64 scalar) {
    return m_V2i64_mul(v, m_V2i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_divS(m_V2i64 v, i64 scalar) {
    return m_V2i64_div(v, m_V2i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_remS(m_V2i64 v, i64 scalar) {
    return m_V2i64_rem(v, m_V2i64_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max));
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_min(m_V2i64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max), int_wrap);
    return m_V_simd_to2$(m_V2i64, result);
#else /* !arch_simd_supported */
    return m_V2i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
#endif /* arch_simd_supported */
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V2i64_lenSq(m_V2i64 v) {
    return m_V2i64_dot(v, v);
}
$attr(m_linalg_$inline)
$static i64 m_V2i64_distSq(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_lenSq(m_V2i64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static i64 m_V2i64_dot(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return l$((i64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((i64){ lhs.x, lhs.y, 0, 0 });
    let r = simd_V_from$((i64){ rhs.x, rhs.y, 0, 0 });
    let result = simd_V_cross3(l, r);
    return m_V_simd_to3$(m_V3i64, result).z;
#else /* !arch_simd_supported */
    return lhs.x * rhs.y - lhs.y * rhs.x;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static i64 m_V2i64_det(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_cross(lhs, rhs);
}
$attr(m_linalg_$inline)
$static i64 m_V2i64_orient(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_det(m_V2i64_sub(lhs, origin), m_V2i64_sub(rhs, origin));
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V2i64_orientDir(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    let det = m_V2i64_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V2i64_isCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCW(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2i64_isCollinear(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCollinear(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V2i64_isCCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCCW(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_cw(m_V2i64 v) {
    return m_V2i64_of(v.y, -v.x);
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_ccw(m_V2i64 v) {
    return m_V2i64_of(-v.y, v.x);
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_skew(m_V2i64 v) {
    return m_V2i64_ccw(v);
}
$attr(m_linalg_$inline)
$static m_V2i64 m_V2i64_perp(m_V2i64 v) {
    return m_V2i64_ccw(v);
}

/* Vec3l functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_of(i64 x, i64 y, i64 z) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y, z, 0 });
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return (m_V3i64){ .x = x, .y = y, .z = z };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat3$(i64, scalar);
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(scalar, scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_from2(m_V2i64 v) {
    return m_V3i64_of(v.x, v.y, 0);
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_from4(m_V4i64 v) {
    return m_V3i64_of(v.x, v.y, v.z);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3i64)(lhs, rhs)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_neg(m_V3i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from3(v));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(-v.x, -v.y, -v.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_rem(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs), int_rem);
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(v), m_V_simd_splat3$(i64, scalar));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(v.x * scalar, v.y * scalar, v.z * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(v), m_V_simd_splat3$(i64, scalar));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(v.x / scalar, v.y / scalar, v.z / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_addS(m_V3i64 v, i64 scalar) {
    return m_V3i64_add(v, m_V3i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_subS(m_V3i64 v, i64 scalar) {
    return m_V3i64_sub(v, m_V3i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_mulS(m_V3i64 v, i64 scalar) {
    return m_V3i64_mul(v, m_V3i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_divS(m_V3i64 v, i64 scalar) {
    return m_V3i64_div(v, m_V3i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_remS(m_V3i64 v, i64 scalar) {
    return m_V3i64_rem(v, m_V3i64_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3(max));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_min(m_V3i64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3Divisor(max), int_wrap);
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
#endif /* arch_simd_supported */
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V3i64_lenSq(m_V3i64 v) {
    return m_V3i64_dot(v, v);
}
$attr(m_linalg_$inline)
$static i64 m_V3i64_distSq(m_V3i64 lhs, m_V3i64 rhs) {
    return m_V3i64_lenSq(m_V3i64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static i64 m_V3i64_dot(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return l$((i64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return m_V3i64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static i64 m_V3i64_det(m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V3i64_dot(lhs, m_V3i64_cross(mid, rhs));
}
$attr(m_linalg_$inline)
$static i64 m_V3i64_orient(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V3i64_det(
        m_V3i64_sub(lhs, origin),
        m_V3i64_sub(mid, origin),
        m_V3i64_sub(rhs, origin)
    );
}
$attr(m_linalg_$inline)
$static m_V_Orient m_V3i64_orientDir(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    let det = m_V3i64_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr(m_linalg_$inline)
$static bool m_V3i64_isOrientNeg(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isNeg(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3i64_isCoplanar(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isCoplanar(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static bool m_V3i64_isOrientPos(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isPos(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr(m_linalg_$inline)
$static m_V3i64 m_V3i64_perp(m_V3i64 v) {
#if arch_simd_supported
    let source = m_V_simd_from3(v);
    if (v.x != 0 || v.y != 0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((i64){ -1, 1, 0, 0 })
        );
        return m_V_simd_to3$(m_V3i64, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((i64){ 0, -1, 1, 0 })
    );
    return m_V_simd_to3$(m_V3i64, result);
#else /* !arch_simd_supported */
    return v.x != 0 || v.y != 0
             ? m_V3i64_of(-v.y, v.x, 0)
             : m_V3i64_of(0, -v.z, v.y);
#endif /* arch_simd_supported */
}

/* Vec4l functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_of(i64 x, i64 y, i64 z, i64 w) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y, z, w });
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return (m_V4i64){ .x = x, .y = y, .z = z, .w = w };
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat4$(i64, scalar);
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(scalar, scalar, scalar, scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_from2(m_V2i64 v) {
    return m_V4i64_of(v.x, v.y, 0, 0);
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_from3(m_V3i64 v) {
    return m_V4i64_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4i64)(lhs, rhs)) {
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

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_neg(m_V4i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(-v.x, -v.y, -v.z, -v.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_rem(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from4(lhs), m_V_simd_from4(rhs), int_rem);
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(v), m_V_simd_splat4$(i64, scalar));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(v), m_V_simd_splat4$(i64, scalar));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
#endif /* arch_simd_supported */
}

$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_addS(m_V4i64 v, i64 scalar) {
    return m_V4i64_add(v, m_V4i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_subS(m_V4i64 v, i64 scalar) {
    return m_V4i64_sub(v, m_V4i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_mulS(m_V4i64 v, i64 scalar) {
    return m_V4i64_mul(v, m_V4i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_divS(m_V4i64 v, i64 scalar) {
    return m_V4i64_div(v, m_V4i64_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_remS(m_V4i64 v, i64 scalar) {
    return m_V4i64_rem(v, m_V4i64_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max));
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_min(m_V4i64_max(v, min), max);
#endif /* arch_simd_supported */
}
$attr(m_linalg_$inline)
$static m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max), int_wrap);
    return m_V_simd_to4$(m_V4i64, result);
#else /* !arch_simd_supported */
    return m_V4i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
#endif /* arch_simd_supported */
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static i64 m_V4i64_lenSq(m_V4i64 v) {
    return m_V4i64_dot(v, v);
}
$attr(m_linalg_$inline)
$static i64 m_V4i64_distSq(m_V4i64 lhs, m_V4i64 rhs) {
    return m_V4i64_lenSq(m_V4i64_sub(lhs, rhs));
}
$attr(m_linalg_$inline)
$static i64 m_V4i64_dot(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return l$((i64)(result));
#else /* !arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
#endif /* arch_simd_supported */
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_i64__included */
