#pragma once
#ifndef m_math_linalg_V_Vec_u32__included
#define m_math_linalg_V_Vec_u32__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- UInt32 (u32) Vector --- */

/* Vec2u functions */
/* Constants */
#define m_V2u32_zero m_V2u32_of_static(0, 0)
#define m_V2u32_unit_x m_V2u32_of_static(1, 0)
#define m_V2u32_unit_y m_V2u32_of_static(0, 1)
#define m_V2u32_one m_V2u32_of_static(1, 1)
#define m_V2u32_right m_V2u32_of_static(1, 0)
#define m_V2u32_up m_V2u32_of_static(0, 1)
#define m_V2u32_limit_min m_V2u32_of_static(u32_limit_min, u32_limit_min)
#define m_V2u32_limit_max m_V2u32_of_static(u32_limit_max, u32_limit_max)

/* Construction */
#define m_V2u32_of_static(_x, _y...) l$((m_V2u32){ .x = (_x), .y = (_y) })
$attr($inline_always)
$static m_V2u32 m_V2u32_of(u32 x, u32 y);
$attr($inline_always)
$static m_V2u32 m_V2u32_splat(u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_from3(m_V3u32 v);
$attr($inline_always)
$static m_V2u32 m_V2u32_from4(m_V4u32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2u32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2u32 m_V2u32_add(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_sub(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_mul(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_div(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_rem(m_V2u32 lhs, m_V2u32 rhs);

$attr($inline_always)
$static m_V2u32 m_V2u32_scal(m_V2u32 v, u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_scalInv(m_V2u32 v, u32 scalar);

$attr($inline_always)
$static m_V2u32 m_V2u32_addS(m_V2u32 v, u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_subS(m_V2u32 v, u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_mulS(m_V2u32 v, u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_divS(m_V2u32 v, u32 scalar);
$attr($inline_always)
$static m_V2u32 m_V2u32_remS(m_V2u32 v, u32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2u32 m_V2u32_min(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_max(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static m_V2u32 m_V2u32_clamp(m_V2u32 v, m_V2u32 min, m_V2u32 max);
$attr($inline_always)
$static m_V2u32 m_V2u32_wrap(m_V2u32 v, m_V2u32 min, m_V2u32 max);

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V2u32_lenSq(m_V2u32 v);
$attr($inline_always)
$static u32 m_V2u32_distSq(m_V2u32 lhs, m_V2u32 rhs);
$attr($inline_always)
$static u32 m_V2u32_dot(m_V2u32 lhs, m_V2u32 rhs);

/* Vec3u functions */
/* Constants */
#define m_V3u32_zero m_V3u32_of_static(0, 0, 0)
#define m_V3u32_unit_x m_V3u32_of_static(1, 0, 0)
#define m_V3u32_unit_y m_V3u32_of_static(0, 1, 0)
#define m_V3u32_unit_z m_V3u32_of_static(0, 0, 1)
#define m_V3u32_unit_xy m_V3u32_of_static(1, 1, 0)
#define m_V3u32_unit_yz m_V3u32_of_static(0, 1, 1)
#define m_V3u32_unit_zx m_V3u32_of_static(1, 0, 1)
#define m_V3u32_one m_V3u32_of_static(1, 1, 1)
#define m_V3u32_right m_V3u32_of_static(1, 0, 0)
#define m_V3u32_up m_V3u32_of_static(0, 1, 0)
#define m_V3u32_forward m_V3u32_of_static(0, 0, 1)
#define m_V3u32_view_backward_rh m_V3u32_forward
#define m_V3u32_view_forward_lh m_V3u32_forward
#define m_V3u32_limit_min m_V3u32_of_static(u32_limit_min, u32_limit_min, u32_limit_min)
#define m_V3u32_limit_max m_V3u32_of_static(u32_limit_max, u32_limit_max, u32_limit_max)

/* Construction */
#define m_V3u32_of_static(_x, _y, _z...) l$((m_V3u32){ .x = (_x), .y = (_y), .z = (_z) })
$attr($inline_always)
$static m_V3u32 m_V3u32_of(u32 x, u32 y, u32 z);
$attr($inline_always)
$static m_V3u32 m_V3u32_splat(u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_from2(m_V2u32 v);
$attr($inline_always)
$static m_V3u32 m_V3u32_from4(m_V4u32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3u32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3u32 m_V3u32_add(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_sub(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_mul(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_div(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_rem(m_V3u32 lhs, m_V3u32 rhs);

$attr($inline_always)
$static m_V3u32 m_V3u32_scal(m_V3u32 v, u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_scalInv(m_V3u32 v, u32 scalar);

$attr($inline_always)
$static m_V3u32 m_V3u32_addS(m_V3u32 v, u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_subS(m_V3u32 v, u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_mulS(m_V3u32 v, u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_divS(m_V3u32 v, u32 scalar);
$attr($inline_always)
$static m_V3u32 m_V3u32_remS(m_V3u32 v, u32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3u32 m_V3u32_min(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_max(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static m_V3u32 m_V3u32_clamp(m_V3u32 v, m_V3u32 min, m_V3u32 max);
$attr($inline_always)
$static m_V3u32 m_V3u32_wrap(m_V3u32 v, m_V3u32 min, m_V3u32 max);

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V3u32_lenSq(m_V3u32 v);
$attr($inline_always)
$static u32 m_V3u32_distSq(m_V3u32 lhs, m_V3u32 rhs);
$attr($inline_always)
$static u32 m_V3u32_dot(m_V3u32 lhs, m_V3u32 rhs);

/* Vec4u functions */
/* Constants */
#define m_V4u32_zero m_V4u32_of_static(0, 0, 0, 0)
#define m_V4u32_unit_x m_V4u32_of_static(1, 0, 0, 0)
#define m_V4u32_unit_y m_V4u32_of_static(0, 1, 0, 0)
#define m_V4u32_unit_z m_V4u32_of_static(0, 0, 1, 0)
#define m_V4u32_unit_w m_V4u32_of_static(0, 0, 0, 1)
#define m_V4u32_one m_V4u32_of_static(1, 1, 1, 1)
#define m_V4u32_right m_V4u32_of_static(1, 0, 0, 0)
#define m_V4u32_up m_V4u32_of_static(0, 1, 0, 0)
#define m_V4u32_forward m_V4u32_of_static(0, 0, 1, 0)
#define m_V4u32_view_backward_rh m_V4u32_forward
#define m_V4u32_view_forward_lh m_V4u32_forward
#define m_V4u32_limit_min m_V4u32_of_static(u32_limit_min, u32_limit_min, u32_limit_min, u32_limit_min)
#define m_V4u32_limit_max m_V4u32_of_static(u32_limit_max, u32_limit_max, u32_limit_max, u32_limit_max)

/* Construction */
#define m_V4u32_of_static(_x, _y, _z, _w...) l$((m_V4u32){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr($inline_always)
$static m_V4u32 m_V4u32_of(u32 x, u32 y, u32 z, u32 w);
$attr($inline_always)
$static m_V4u32 m_V4u32_splat(u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_from2(m_V2u32 v);
$attr($inline_always)
$static m_V4u32 m_V4u32_from3(m_V3u32 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4u32)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4u32)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4u32 m_V4u32_add(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_sub(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_mul(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_div(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_rem(m_V4u32 lhs, m_V4u32 rhs);

$attr($inline_always)
$static m_V4u32 m_V4u32_scal(m_V4u32 v, u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_scalInv(m_V4u32 v, u32 scalar);

$attr($inline_always)
$static m_V4u32 m_V4u32_addS(m_V4u32 v, u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_subS(m_V4u32 v, u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_mulS(m_V4u32 v, u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_divS(m_V4u32 v, u32 scalar);
$attr($inline_always)
$static m_V4u32 m_V4u32_remS(m_V4u32 v, u32 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4u32 m_V4u32_min(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_max(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static m_V4u32 m_V4u32_clamp(m_V4u32 v, m_V4u32 min, m_V4u32 max);
$attr($inline_always)
$static m_V4u32 m_V4u32_wrap(m_V4u32 v, m_V4u32 min, m_V4u32 max);

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V4u32_lenSq(m_V4u32 v);
$attr($inline_always)
$static u32 m_V4u32_distSq(m_V4u32 lhs, m_V4u32 rhs);
$attr($inline_always)
$static u32 m_V4u32_dot(m_V4u32 lhs, m_V4u32 rhs);

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/* Vec2u functions */
/* Construction */
$attr($inline_always)
$static m_V2u32 m_V2u32_of(u32 x, u32 y) {
    return (m_V2u32){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2u32 m_V2u32_splat(u32 scalar) {
    return m_V2u32_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_from3(m_V3u32 v) {
    return m_V2u32_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_from4(m_V4u32 v) {
    return m_V2u32_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2u32)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2u32 m_V2u32_add(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_sub(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_mul(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_div(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_rem(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2u32 m_V2u32_scal(m_V2u32 v, u32 scalar) {
    return m_V2u32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_scalInv(m_V2u32 v, u32 scalar) {
    return m_V2u32_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2u32 m_V2u32_addS(m_V2u32 v, u32 scalar) {
    return m_V2u32_add(v, m_V2u32_splat(scalar));
}
$attr($inline_always)
$static m_V2u32 m_V2u32_subS(m_V2u32 v, u32 scalar) {
    return m_V2u32_sub(v, m_V2u32_splat(scalar));
}
$attr($inline_always)
$static m_V2u32 m_V2u32_mulS(m_V2u32 v, u32 scalar) {
    return m_V2u32_mul(v, m_V2u32_splat(scalar));
}
$attr($inline_always)
$static m_V2u32 m_V2u32_divS(m_V2u32 v, u32 scalar) {
    return m_V2u32_div(v, m_V2u32_splat(scalar));
}
$attr($inline_always)
$static m_V2u32 m_V2u32_remS(m_V2u32 v, u32 scalar) {
    return m_V2u32_rem(v, m_V2u32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2u32 m_V2u32_min(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2u32 m_V2u32_max(m_V2u32 lhs, m_V2u32 rhs) {
    return m_V2u32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2u32 m_V2u32_clamp(m_V2u32 v, m_V2u32 min, m_V2u32 max) {
    return m_V2u32_min(m_V2u32_max(v, min), max);
}
$attr($inline_always)
$static m_V2u32 m_V2u32_wrap(m_V2u32 v, m_V2u32 min, m_V2u32 max) {
    return m_V2u32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V2u32_lenSq(m_V2u32 v) {
    return m_V2u32_dot(v, v);
}
$attr($inline_always)
$static u32 m_V2u32_distSq(m_V2u32 lhs, m_V2u32 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    return d_x * d_x + d_y * d_y;
}
$attr($inline_always)
$static u32 m_V2u32_dot(m_V2u32 lhs, m_V2u32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/* Vec3u functions */
/* Construction */
$attr($inline_always)
$static m_V3u32 m_V3u32_of(u32 x, u32 y, u32 z) {
    return (m_V3u32){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3u32 m_V3u32_splat(u32 scalar) {
    return m_V3u32_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_from2(m_V2u32 v) {
    return m_V3u32_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_from4(m_V4u32 v) {
    return m_V3u32_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3u32)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3u32 m_V3u32_add(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_sub(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_mul(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_div(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_rem(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3u32 m_V3u32_scal(m_V3u32 v, u32 scalar) {
    return m_V3u32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_scalInv(m_V3u32 v, u32 scalar) {
    return m_V3u32_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3u32 m_V3u32_addS(m_V3u32 v, u32 scalar) {
    return m_V3u32_add(v, m_V3u32_splat(scalar));
}
$attr($inline_always)
$static m_V3u32 m_V3u32_subS(m_V3u32 v, u32 scalar) {
    return m_V3u32_sub(v, m_V3u32_splat(scalar));
}
$attr($inline_always)
$static m_V3u32 m_V3u32_mulS(m_V3u32 v, u32 scalar) {
    return m_V3u32_mul(v, m_V3u32_splat(scalar));
}
$attr($inline_always)
$static m_V3u32 m_V3u32_divS(m_V3u32 v, u32 scalar) {
    return m_V3u32_div(v, m_V3u32_splat(scalar));
}
$attr($inline_always)
$static m_V3u32 m_V3u32_remS(m_V3u32 v, u32 scalar) {
    return m_V3u32_rem(v, m_V3u32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3u32 m_V3u32_min(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3u32 m_V3u32_max(m_V3u32 lhs, m_V3u32 rhs) {
    return m_V3u32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3u32 m_V3u32_clamp(m_V3u32 v, m_V3u32 min, m_V3u32 max) {
    return m_V3u32_min(m_V3u32_max(v, min), max);
}
$attr($inline_always)
$static m_V3u32 m_V3u32_wrap(m_V3u32 v, m_V3u32 min, m_V3u32 max) {
    return m_V3u32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V3u32_lenSq(m_V3u32 v) {
    return m_V3u32_dot(v, v);
}
$attr($inline_always)
$static u32 m_V3u32_distSq(m_V3u32 lhs, m_V3u32 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    return d_x * d_x + d_y * d_y + d_z * d_z;
}
$attr($inline_always)
$static u32 m_V3u32_dot(m_V3u32 lhs, m_V3u32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

/* Vec4u functions */
/* Construction */
$attr($inline_always)
$static m_V4u32 m_V4u32_of(u32 x, u32 y, u32 z, u32 w) {
    return (m_V4u32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4u32 m_V4u32_splat(u32 scalar) {
    return m_V4u32_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_from2(m_V2u32 v) {
    return m_V4u32_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_from3(m_V3u32 v) {
    return m_V4u32_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4u32)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4u32 m_V4u32_add(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_sub(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_mul(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_div(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_rem(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4u32 m_V4u32_scal(m_V4u32 v, u32 scalar) {
    return m_V4u32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_scalInv(m_V4u32 v, u32 scalar) {
    return m_V4u32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4u32 m_V4u32_addS(m_V4u32 v, u32 scalar) {
    return m_V4u32_add(v, m_V4u32_splat(scalar));
}
$attr($inline_always)
$static m_V4u32 m_V4u32_subS(m_V4u32 v, u32 scalar) {
    return m_V4u32_sub(v, m_V4u32_splat(scalar));
}
$attr($inline_always)
$static m_V4u32 m_V4u32_mulS(m_V4u32 v, u32 scalar) {
    return m_V4u32_mul(v, m_V4u32_splat(scalar));
}
$attr($inline_always)
$static m_V4u32 m_V4u32_divS(m_V4u32 v, u32 scalar) {
    return m_V4u32_div(v, m_V4u32_splat(scalar));
}
$attr($inline_always)
$static m_V4u32 m_V4u32_remS(m_V4u32 v, u32 scalar) {
    return m_V4u32_rem(v, m_V4u32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4u32 m_V4u32_min(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4u32 m_V4u32_max(m_V4u32 lhs, m_V4u32 rhs) {
    return m_V4u32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4u32 m_V4u32_clamp(m_V4u32 v, m_V4u32 min, m_V4u32 max) {
    return m_V4u32_min(m_V4u32_max(v, min), max);
}
$attr($inline_always)
$static m_V4u32 m_V4u32_wrap(m_V4u32 v, m_V4u32 min, m_V4u32 max) {
    return m_V4u32_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u32 m_V4u32_lenSq(m_V4u32 v) {
    return m_V4u32_dot(v, v);
}
$attr($inline_always)
$static u32 m_V4u32_distSq(m_V4u32 lhs, m_V4u32 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    let d_w = lhs.w < rhs.w ? rhs.w - lhs.w : lhs.w - rhs.w;
    return d_x * d_x + d_y * d_y + d_z * d_z + d_w * d_w;
}
$attr($inline_always)
$static u32 m_V4u32_dot(m_V4u32 lhs, m_V4u32 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_u32__included */
