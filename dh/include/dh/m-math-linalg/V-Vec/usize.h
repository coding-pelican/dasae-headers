#pragma once
#ifndef m_math_linalg_V_Vec_usize__included
#define m_math_linalg_V_Vec_usize__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Usize (usize) Vector --- */

/* Vec2uz functions */
/* Constants */
#define m_V2usize_zero m_V2usize_of_static(0, 0)
#define m_V2usize_unit_x m_V2usize_of_static(1, 0)
#define m_V2usize_unit_y m_V2usize_of_static(0, 1)
#define m_V2usize_one m_V2usize_of_static(1, 1)
#define m_V2usize_right m_V2usize_of_static(1, 0)
#define m_V2usize_up m_V2usize_of_static(0, 1)
#define m_V2usize_limit_min m_V2usize_of_static(usize_limit_min, usize_limit_min)
#define m_V2usize_limit_max m_V2usize_of_static(usize_limit_max, usize_limit_max)

/* Construction */
#define m_V2usize_of_static(_x, _y...) l$((m_V2usize){ .x = (_x), .y = (_y) })
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_of(usize x, usize y);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_splat(usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_from3(m_V3usize v);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_from4(m_V4usize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V2usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V2usize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_add(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_sub(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_mul(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_div(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_rem(m_V2usize lhs, m_V2usize rhs);

$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_scal(m_V2usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_scalInv(m_V2usize v, usize scalar);

$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_addS(m_V2usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_subS(m_V2usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_mulS(m_V2usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_divS(m_V2usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_remS(m_V2usize v, usize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_min(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_max(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_clamp(m_V2usize v, m_V2usize min, m_V2usize max);
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_wrap(m_V2usize v, m_V2usize min, m_V2usize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V2usize_lenSq(m_V2usize v);
$attr(m_linalg_$inline)
$static usize m_V2usize_distSq(m_V2usize lhs, m_V2usize rhs);
$attr(m_linalg_$inline)
$static usize m_V2usize_dot(m_V2usize lhs, m_V2usize rhs);

/* Vec3uz functions */
/* Constants */
#define m_V3usize_zero m_V3usize_of_static(0, 0, 0)
#define m_V3usize_unit_x m_V3usize_of_static(1, 0, 0)
#define m_V3usize_unit_y m_V3usize_of_static(0, 1, 0)
#define m_V3usize_unit_z m_V3usize_of_static(0, 0, 1)
#define m_V3usize_unit_xy m_V3usize_of_static(1, 1, 0)
#define m_V3usize_unit_yz m_V3usize_of_static(0, 1, 1)
#define m_V3usize_unit_zx m_V3usize_of_static(1, 0, 1)
#define m_V3usize_one m_V3usize_of_static(1, 1, 1)
#define m_V3usize_right m_V3usize_of_static(1, 0, 0)
#define m_V3usize_up m_V3usize_of_static(0, 1, 0)
#define m_V3usize_forward m_V3usize_of_static(0, 0, 1)
#define m_V3usize_view_backward_rh m_V3usize_forward
#define m_V3usize_view_forward_lh m_V3usize_forward
#define m_V3usize_limit_min m_V3usize_of_static(usize_limit_min, usize_limit_min, usize_limit_min)
#define m_V3usize_limit_max m_V3usize_of_static(usize_limit_max, usize_limit_max, usize_limit_max)

/* Construction */
#define m_V3usize_of_static(_x, _y, _z...) l$((m_V3usize){ .x = (_x), .y = (_y), .z = (_z) })
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_of(usize x, usize y, usize z);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_splat(usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_from2(m_V2usize v);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_from4(m_V4usize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V3usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V3usize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_add(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_sub(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_mul(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_div(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_rem(m_V3usize lhs, m_V3usize rhs);

$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_scal(m_V3usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_scalInv(m_V3usize v, usize scalar);

$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_addS(m_V3usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_subS(m_V3usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_mulS(m_V3usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_divS(m_V3usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_remS(m_V3usize v, usize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_min(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_max(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_clamp(m_V3usize v, m_V3usize min, m_V3usize max);
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_wrap(m_V3usize v, m_V3usize min, m_V3usize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V3usize_lenSq(m_V3usize v);
$attr(m_linalg_$inline)
$static usize m_V3usize_distSq(m_V3usize lhs, m_V3usize rhs);
$attr(m_linalg_$inline)
$static usize m_V3usize_dot(m_V3usize lhs, m_V3usize rhs);

/* Vec4uz functions */
/* Constants */
#define m_V4usize_zero m_V4usize_of_static(0, 0, 0, 0)
#define m_V4usize_unit_x m_V4usize_of_static(1, 0, 0, 0)
#define m_V4usize_unit_y m_V4usize_of_static(0, 1, 0, 0)
#define m_V4usize_unit_z m_V4usize_of_static(0, 0, 1, 0)
#define m_V4usize_unit_w m_V4usize_of_static(0, 0, 0, 1)
#define m_V4usize_one m_V4usize_of_static(1, 1, 1, 1)
#define m_V4usize_right m_V4usize_of_static(1, 0, 0, 0)
#define m_V4usize_up m_V4usize_of_static(0, 1, 0, 0)
#define m_V4usize_forward m_V4usize_of_static(0, 0, 1, 0)
#define m_V4usize_view_backward_rh m_V4usize_forward
#define m_V4usize_view_forward_lh m_V4usize_forward
#define m_V4usize_limit_min m_V4usize_of_static(usize_limit_min, usize_limit_min, usize_limit_min, usize_limit_min)
#define m_V4usize_limit_max m_V4usize_of_static(usize_limit_max, usize_limit_max, usize_limit_max, usize_limit_max)

/* Construction */
#define m_V4usize_of_static(_x, _y, _z, _w...) l$((m_V4usize){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_of(usize x, usize y, usize z, usize w);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_splat(usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_from2(m_V2usize v);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_from3(m_V3usize v);

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_eq_default$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ne_default$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_lt_default$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_gt_default$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_le_default$((m_V4usize)(lhs, rhs));
$attr(m_linalg_$inline)
$static cmp_fn_ge_default$((m_V4usize)(lhs, rhs));

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_add(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_sub(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_mul(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_div(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_rem(m_V4usize lhs, m_V4usize rhs);

$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_scal(m_V4usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_scalInv(m_V4usize v, usize scalar);

$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_addS(m_V4usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_subS(m_V4usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_mulS(m_V4usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_divS(m_V4usize v, usize scalar);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_remS(m_V4usize v, usize scalar);

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_min(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_max(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_clamp(m_V4usize v, m_V4usize min, m_V4usize max);
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_wrap(m_V4usize v, m_V4usize min, m_V4usize max);

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V4usize_lenSq(m_V4usize v);
$attr(m_linalg_$inline)
$static usize m_V4usize_distSq(m_V4usize lhs, m_V4usize rhs);
$attr(m_linalg_$inline)
$static usize m_V4usize_dot(m_V4usize lhs, m_V4usize rhs);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2uz functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_of(usize x, usize y) {
    return (m_V2usize){ .x = x, .y = y };
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_splat(usize scalar) {
    return m_V2usize_of(scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_from3(m_V3usize v) {
    return m_V2usize_of(v.x, v.y);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_from4(m_V4usize v) {
    return m_V2usize_of(v.x, v.y);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V2usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_add(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_sub(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_mul(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_div(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_rem(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_scal(m_V2usize v, usize scalar) {
    return m_V2usize_of(v.x * scalar, v.y * scalar);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_scalInv(m_V2usize v, usize scalar) {
    return m_V2usize_of(v.x / scalar, v.y / scalar);
}

$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_addS(m_V2usize v, usize scalar) {
    return m_V2usize_add(v, m_V2usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_subS(m_V2usize v, usize scalar) {
    return m_V2usize_sub(v, m_V2usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_mulS(m_V2usize v, usize scalar) {
    return m_V2usize_mul(v, m_V2usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_divS(m_V2usize v, usize scalar) {
    return m_V2usize_div(v, m_V2usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_remS(m_V2usize v, usize scalar) {
    return m_V2usize_rem(v, m_V2usize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_min(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_max(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_clamp(m_V2usize v, m_V2usize min, m_V2usize max) {
    return m_V2usize_min(m_V2usize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V2usize m_V2usize_wrap(m_V2usize v, m_V2usize min, m_V2usize max) {
    return m_V2usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V2usize_lenSq(m_V2usize v) {
    return m_V2usize_dot(v, v);
}
$attr(m_linalg_$inline)
$static usize m_V2usize_distSq(m_V2usize lhs, m_V2usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    return d_x * d_x + d_y * d_y;
}
$attr(m_linalg_$inline)
$static usize m_V2usize_dot(m_V2usize lhs, m_V2usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/* Vec3uz functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_of(usize x, usize y, usize z) {
    return (m_V3usize){ .x = x, .y = y, .z = z };
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_splat(usize scalar) {
    return m_V3usize_of(scalar, scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_from2(m_V2usize v) {
    return m_V3usize_of(v.x, v.y, 0);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_from4(m_V4usize v) {
    return m_V3usize_of(v.x, v.y, v.z);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V3usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_add(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_sub(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_mul(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_div(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_rem(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_scal(m_V3usize v, usize scalar) {
    return m_V3usize_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_scalInv(m_V3usize v, usize scalar) {
    return m_V3usize_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_addS(m_V3usize v, usize scalar) {
    return m_V3usize_add(v, m_V3usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_subS(m_V3usize v, usize scalar) {
    return m_V3usize_sub(v, m_V3usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_mulS(m_V3usize v, usize scalar) {
    return m_V3usize_mul(v, m_V3usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_divS(m_V3usize v, usize scalar) {
    return m_V3usize_div(v, m_V3usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_remS(m_V3usize v, usize scalar) {
    return m_V3usize_rem(v, m_V3usize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_min(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_max(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_clamp(m_V3usize v, m_V3usize min, m_V3usize max) {
    return m_V3usize_min(m_V3usize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V3usize m_V3usize_wrap(m_V3usize v, m_V3usize min, m_V3usize max) {
    return m_V3usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V3usize_lenSq(m_V3usize v) {
    return m_V3usize_dot(v, v);
}
$attr(m_linalg_$inline)
$static usize m_V3usize_distSq(m_V3usize lhs, m_V3usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    return d_x * d_x + d_y * d_y + d_z * d_z;
}
$attr(m_linalg_$inline)
$static usize m_V3usize_dot(m_V3usize lhs, m_V3usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

/* Vec4uz functions */
/* Construction */
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_of(usize x, usize y, usize z, usize w) {
    return (m_V4usize){ .x = x, .y = y, .z = z, .w = w };
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_splat(usize scalar) {
    return m_V4usize_of(scalar, scalar, scalar, scalar);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_from2(m_V2usize v) {
    return m_V4usize_of(v.x, v.y, 0, 0);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_from3(m_V3usize v) {
    return m_V4usize_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr(m_linalg_$inline)
$static cmp_fn_ord$((m_V4usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_add(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_sub(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_mul(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_div(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_rem(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_scal(m_V4usize v, usize scalar) {
    return m_V4usize_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_scalInv(m_V4usize v, usize scalar) {
    return m_V4usize_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_addS(m_V4usize v, usize scalar) {
    return m_V4usize_add(v, m_V4usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_subS(m_V4usize v, usize scalar) {
    return m_V4usize_sub(v, m_V4usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_mulS(m_V4usize v, usize scalar) {
    return m_V4usize_mul(v, m_V4usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_divS(m_V4usize v, usize scalar) {
    return m_V4usize_div(v, m_V4usize_splat(scalar));
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_remS(m_V4usize v, usize scalar) {
    return m_V4usize_rem(v, m_V4usize_splat(scalar));
}

/* Range Operations */
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_min(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_max(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_clamp(m_V4usize v, m_V4usize min, m_V4usize max) {
    return m_V4usize_min(m_V4usize_max(v, min), max);
}
$attr(m_linalg_$inline)
$static m_V4usize m_V4usize_wrap(m_V4usize v, m_V4usize min, m_V4usize max) {
    return m_V4usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr(m_linalg_$inline)
$static usize m_V4usize_lenSq(m_V4usize v) {
    return m_V4usize_dot(v, v);
}
$attr(m_linalg_$inline)
$static usize m_V4usize_distSq(m_V4usize lhs, m_V4usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    let d_w = lhs.w < rhs.w ? rhs.w - lhs.w : lhs.w - rhs.w;
    return d_x * d_x + d_y * d_y + d_z * d_z + d_w * d_w;
}
$attr(m_linalg_$inline)
$static usize m_V4usize_dot(m_V4usize lhs, m_V4usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_usize__included */
