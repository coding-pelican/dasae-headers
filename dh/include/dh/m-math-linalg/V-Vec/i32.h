#pragma once
#ifndef m_math_linalg_V_Vec_i32__included
#define m_math_linalg_V_Vec_i32__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Int32 (i32) Vector --- */

/* Vec2i functions */
/* Constants */
#define m_V2i32_zero m_V2i32_of_static(0, 0)
#define m_V2i32_unit_x m_V2i32_of_static(1, 0)
#define m_V2i32_unit_y m_V2i32_of_static(0, 1)
#define m_V2i32_one m_V2i32_of_static(1, 1)
#define m_V2i32_left m_V2i32_of_static(-1, 0)
#define m_V2i32_up m_V2i32_of_static(0, 1)
#define m_V2i32_right m_V2i32_of_static(1, 0)
#define m_V2i32_down m_V2i32_of_static(0, -1)
#define m_V2i32_limit_min m_V2i32_of_static(i32_limit_min, i32_limit_min)
#define m_V2i32_limit_max m_V2i32_of_static(i32_limit_max, i32_limit_max)

/* Construction */
#define m_V2i32_of_static(_x, _y...) l$((m_V2i32){ .x = (_x), .y = (_y) })
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
$static m_V2i32 m_V2i32_rem(m_V2i32 lhs, m_V2i32 rhs);

$attr($inline_always)
$static m_V2i32 m_V2i32_scal(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_scalInv(m_V2i32 v, i32 scalar);

$attr($inline_always)
$static m_V2i32 m_V2i32_addS(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_subS(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_mulS(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_divS(m_V2i32 v, i32 scalar);
$attr($inline_always)
$static m_V2i32 m_V2i32_remS(m_V2i32 v, i32 scalar);

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
$static i32 m_V2i32_det(m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static i32 m_V2i32_orient(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V_Orient m_V2i32_orientDir(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static bool m_V2i32_isCW(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static bool m_V2i32_isCollinear(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static bool m_V2i32_isCCW(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs);
$attr($inline_always)
$static m_V2i32 m_V2i32_cw(m_V2i32 v);
$attr($inline_always)
$static m_V2i32 m_V2i32_ccw(m_V2i32 v);
$attr($inline_always)
$static m_V2i32 m_V2i32_skew(m_V2i32 v);
$attr($inline_always)
$static m_V2i32 m_V2i32_perp(m_V2i32 v);

/* Vec3i functions */
/* Constants */
#define m_V3i32_zero m_V3i32_of_static(0, 0, 0)
#define m_V3i32_unit_x m_V3i32_of_static(1, 0, 0)
#define m_V3i32_unit_y m_V3i32_of_static(0, 1, 0)
#define m_V3i32_unit_z m_V3i32_of_static(0, 0, 1)
#define m_V3i32_unit_xy m_V3i32_of_static(1, 1, 0)
#define m_V3i32_unit_yz m_V3i32_of_static(0, 1, 1)
#define m_V3i32_unit_zx m_V3i32_of_static(1, 0, 1)
#define m_V3i32_one m_V3i32_of_static(1, 1, 1)
#define m_V3i32_left m_V3i32_of_static(-1, 0, 0)
#define m_V3i32_up m_V3i32_of_static(0, 1, 0)
#define m_V3i32_right m_V3i32_of_static(1, 0, 0)
#define m_V3i32_down m_V3i32_of_static(0, -1, 0)
#define m_V3i32_forward m_V3i32_of_static(0, 0, 1)
#define m_V3i32_backward m_V3i32_of_static(0, 0, -1)
#define m_V3i32_view_forward_rh m_V3i32_backward
#define m_V3i32_view_backward_rh m_V3i32_forward
#define m_V3i32_view_forward_lh m_V3i32_forward
#define m_V3i32_view_backward_lh m_V3i32_backward
#define m_V3i32_limit_min m_V3i32_of_static(i32_limit_min, i32_limit_min, i32_limit_min)
#define m_V3i32_limit_max m_V3i32_of_static(i32_limit_max, i32_limit_max, i32_limit_max)

/* Construction */
#define m_V3i32_of_static(_x, _y, _z...) l$((m_V3i32){ .x = (_x), .y = (_y), .z = (_z) })
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
$static m_V3i32 m_V3i32_rem(m_V3i32 lhs, m_V3i32 rhs);

$attr($inline_always)
$static m_V3i32 m_V3i32_scal(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_scalInv(m_V3i32 v, i32 scalar);

$attr($inline_always)
$static m_V3i32 m_V3i32_addS(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_subS(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_mulS(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_divS(m_V3i32 v, i32 scalar);
$attr($inline_always)
$static m_V3i32 m_V3i32_remS(m_V3i32 v, i32 scalar);

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
$static i32 m_V3i32_det(m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static i32 m_V3i32_orient(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static m_V_Orient m_V3i32_orientDir(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static bool m_V3i32_isOrientNeg(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static bool m_V3i32_isCoplanar(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static bool m_V3i32_isOrientPos(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs);
$attr($inline_always)
$static m_V3i32 m_V3i32_perp(m_V3i32 v);

/* Vec4i functions */
/* Constants */
#define m_V4i32_zero m_V4i32_of_static(0, 0, 0, 0)
#define m_V4i32_unit_x m_V4i32_of_static(1, 0, 0, 0)
#define m_V4i32_unit_y m_V4i32_of_static(0, 1, 0, 0)
#define m_V4i32_unit_z m_V4i32_of_static(0, 0, 1, 0)
#define m_V4i32_unit_w m_V4i32_of_static(0, 0, 0, 1)
#define m_V4i32_one m_V4i32_of_static(1, 1, 1, 1)
#define m_V4i32_left m_V4i32_of_static(-1, 0, 0, 0)
#define m_V4i32_up m_V4i32_of_static(0, 1, 0, 0)
#define m_V4i32_right m_V4i32_of_static(1, 0, 0, 0)
#define m_V4i32_down m_V4i32_of_static(0, -1, 0, 0)
#define m_V4i32_forward m_V4i32_of_static(0, 0, 1, 0)
#define m_V4i32_backward m_V4i32_of_static(0, 0, -1, 0)
#define m_V4i32_view_forward_rh m_V4i32_backward
#define m_V4i32_view_backward_rh m_V4i32_forward
#define m_V4i32_view_forward_lh m_V4i32_forward
#define m_V4i32_view_backward_lh m_V4i32_backward
#define m_V4i32_limit_min m_V4i32_of_static(i32_limit_min, i32_limit_min, i32_limit_min, i32_limit_min)
#define m_V4i32_limit_max m_V4i32_of_static(i32_limit_max, i32_limit_max, i32_limit_max, i32_limit_max)

/* Construction */
#define m_V4i32_of_static(_x, _y, _z, _w...) l$((m_V4i32){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
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
$static m_V4i32 m_V4i32_rem(m_V4i32 lhs, m_V4i32 rhs);

$attr($inline_always)
$static m_V4i32 m_V4i32_scal(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_scalInv(m_V4i32 v, i32 scalar);

$attr($inline_always)
$static m_V4i32 m_V4i32_addS(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_subS(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_mulS(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_divS(m_V4i32 v, i32 scalar);
$attr($inline_always)
$static m_V4i32 m_V4i32_remS(m_V4i32 v, i32 scalar);

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

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2i functions */
/* Construction */
$attr($inline_always)
$static m_V2i32 m_V2i32_of(i32 x, i32 y) {
#if arch_simd_supported
    let result = simd_V_from$((i32){ x, y });
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return (m_V2i32){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2i32 m_V2i32_splat(i32 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat2$(i32, scalar);
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2i32 m_V2i32_neg(m_V2i32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from2(v));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_rem(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from2(lhs), m_V_simd_from2(rhs), int_rem);
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(int_rem(lhs.x, rhs.x), int_rem(lhs.y, rhs.y));
}

$attr($inline_always)
$static m_V2i32 m_V2i32_scal(m_V2i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from2(v), m_V_simd_splat2$(i32, scalar));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_scalInv(m_V2i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from2(v), m_V_simd_splat2$(i32, scalar));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2i32 m_V2i32_addS(m_V2i32 v, i32 scalar) {
    return m_V2i32_add(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_subS(m_V2i32 v, i32 scalar) {
    return m_V2i32_sub(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mulS(m_V2i32 v, i32 scalar) {
    return m_V2i32_mul(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_divS(m_V2i32 v, i32 scalar) {
    return m_V2i32_div(v, m_V2i32_splat(scalar));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_remS(m_V2i32 v, i32 scalar) {
    return m_V2i32_rem(v, m_V2i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2i32 m_V2i32_min(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(pri_min(lhs.x, rhs.x), pri_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(pri_max(lhs.x, rhs.x), pri_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max));
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_min(m_V2i32_max(v, min), max);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from2(v), m_V_simd_from2(min), m_V_simd_from2(max), int_wrap);
    return m_V_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
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
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from2(lhs), m_V_simd_from2(rhs));
    return l$((i32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static i32 m_V2i32_cross(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((i32){ lhs.x, lhs.y, 0, 0 });
    let r = simd_V_from$((i32){ rhs.x, rhs.y, 0, 0 });
    let result = simd_V_cross3(l, r);
    return m_V_simd_to3$(m_V3i32, result).z;
#endif /* arch_simd_supported */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static i32 m_V2i32_det(m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_cross(lhs, rhs);
}
$attr($inline_always)
$static i32 m_V2i32_orient(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs) {
    return m_V2i32_det(m_V2i32_sub(lhs, origin), m_V2i32_sub(rhs, origin));
}
$attr($inline_always)
$static m_V_Orient m_V2i32_orientDir(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs) {
    let det = m_V2i32_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V2i32_isCW(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs) {
    return m_V_Orient_isCW(m_V2i32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2i32_isCollinear(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs) {
    return m_V_Orient_isCollinear(m_V2i32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2i32_isCCW(m_V2i32 origin, m_V2i32 lhs, m_V2i32 rhs) {
    return m_V_Orient_isCCW(m_V2i32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_cw(m_V2i32 v) {
    return m_V2i32_of(v.y, -v.x);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_ccw(m_V2i32 v) {
    return m_V2i32_of(-v.y, v.x);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_skew(m_V2i32 v) {
    return m_V2i32_ccw(v);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_perp(m_V2i32 v) {
    return m_V2i32_ccw(v);
}

/* Vec3i functions */
/* Construction */
$attr($inline_always)
$static m_V3i32 m_V3i32_of(i32 x, i32 y, i32 z) {
#if arch_simd_supported
    let result = simd_V_from$((i32){ x, y, z, 0 });
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    let result = m_V_simd_splat3$(i32, scalar);
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(scalar, scalar, scalar);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3i32 m_V3i32_neg(m_V3i32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from3(v));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_rem(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from3(lhs), m_V_simd_from3Divisor(rhs), int_rem);
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3i32 m_V3i32_scal(m_V3i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from3(v), m_V_simd_splat3$(i32, scalar));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_scalInv(m_V3i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from3(v), m_V_simd_splat3$(i32, scalar));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3i32 m_V3i32_addS(m_V3i32 v, i32 scalar) {
    return m_V3i32_add(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_subS(m_V3i32 v, i32 scalar) {
    return m_V3i32_sub(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mulS(m_V3i32 v, i32 scalar) {
    return m_V3i32_mul(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_divS(m_V3i32 v, i32 scalar) {
    return m_V3i32_div(v, m_V3i32_splat(scalar));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_remS(m_V3i32 v, i32 scalar) {
    return m_V3i32_rem(v, m_V3i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3i32 m_V3i32_min(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i32 m_V3i32_max(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i32 m_V3i32_clamp(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3(max));
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_min(m_V3i32_max(v, min), max);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from3(v), m_V_simd_from3(min), m_V_simd_from3Divisor(max), int_wrap);
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
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
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from3(lhs), m_V_simd_from3(rhs));
    return l$((i32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3i32 m_V3i32_cross(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((i32){ lhs.x, lhs.y, 0, 0 });
    let r = simd_V_from$((i32){ rhs.x, rhs.y, 0, 0 });
    let result = simd_V_cross3(l, r);
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static i32 m_V3i32_det(m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    return m_V3i32_dot(lhs, m_V3i32_cross(mid, rhs));
}
$attr($inline_always)
$static i32 m_V3i32_orient(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    return m_V3i32_det(
        m_V3i32_sub(lhs, origin),
        m_V3i32_sub(mid, origin),
        m_V3i32_sub(rhs, origin)
    );
}
$attr($inline_always)
$static m_V_Orient m_V3i32_orientDir(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    let det = m_V3i32_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V3i32_isOrientNeg(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    return m_V_Orient_isNeg(m_V3i32_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3i32_isCoplanar(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    return m_V_Orient_isCoplanar(m_V3i32_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3i32_isOrientPos(m_V3i32 origin, m_V3i32 lhs, m_V3i32 mid, m_V3i32 rhs) {
    return m_V_Orient_isPos(m_V3i32_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static m_V3i32 m_V3i32_perp(m_V3i32 v) {
#if arch_simd_supported
    let source = m_V_simd_from3(v);
    if (v.x != 0 || v.y != 0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((i32){ -1, 1, 0, 0 })
        );
        return m_V_simd_to3$(m_V3i32, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((i32){ 0, -1, 1, 0 })
    );
    return m_V_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return v.x != 0 || v.y != 0
             ? m_V3i32_of(-v.y, v.x, 0)
             : m_V3i32_of(0, -v.z, v.y);
}

/* Vec4i functions */
/* Construction */
$attr($inline_always)
$static m_V4i32 m_V4i32_of(i32 x, i32 y, i32 z, i32 w) {
#if arch_simd_supported
    let result = simd_V_from$((i32){ x, y, z, w });
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return (m_V4i32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4i32 m_V4i32_splat(i32 scalar) {
#if arch_simd_supported
    let result = m_V_simd_splat4$(i32, scalar);
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4i32 m_V4i32_neg(m_V4i32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_V_simd_from4(v));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_rem(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_V_simd_from4(lhs), m_V_simd_from4(rhs), int_rem);
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4i32 m_V4i32_scal(m_V4i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_V_simd_from4(v), m_V_simd_splat4$(i32, scalar));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_scalInv(m_V4i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_V_simd_from4(v), m_V_simd_splat4$(i32, scalar));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4i32 m_V4i32_addS(m_V4i32 v, i32 scalar) {
    return m_V4i32_add(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_subS(m_V4i32 v, i32 scalar) {
    return m_V4i32_sub(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mulS(m_V4i32 v, i32 scalar) {
    return m_V4i32_mul(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_divS(m_V4i32 v, i32 scalar) {
    return m_V4i32_div(v, m_V4i32_splat(scalar));
}
$attr($inline_always)
$static m_V4i32 m_V4i32_remS(m_V4i32 v, i32 scalar) {
    return m_V4i32_rem(v, m_V4i32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4i32 m_V4i32_min(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i32 m_V4i32_max(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i32 m_V4i32_clamp(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max));
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_min(m_V4i32_max(v, min), max);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_V_simd_from4(v), m_V_simd_from4(min), m_V_simd_from4(max), int_wrap);
    return m_V_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
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
#if arch_simd_supported
    let result = simd_V_dot(m_V_simd_from4(lhs), m_V_simd_from4(rhs));
    return l$((i32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_i32__included */
