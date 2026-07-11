/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vec.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-12-25 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)/pri
 * @prefix  m_V
 *
 * @brief   Mathematical common vector operations
 * @details Provides a set of vector priematical operations and functions
 */
#pragma once
#ifndef pri_vec__included
#define pri_vec__included 1

/*========== Includes =======================================================*/

#include "common.h"
#include "vec_types.h"
#include "../prl/simd.h"

#define m_simd_from2(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y })
#define m_simd_from3(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, 0 })
#define m_simd_from3Divisor(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, 1 })
#define m_simd_from4(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, (_v).w })
#define m_simd_splat2$(_T, _x...) simd_V_from$((_T){ (_x), (_x) })
#define m_simd_splat3$(_T, _x...) simd_V_from$((_T){ (_x), (_x), (_x), (_x) })
#define m_simd_splat4$(_T, _x...) simd_V_from$((_T){ (_x), (_x), (_x), (_x) })
#define m_simd_to2$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1] })
#define m_simd_to3$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1], .z = (_v).val[2] })
#define m_simd_to4$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1], .z = (_v).val[2], .w = (_v).val[3] })

/*========== Macros and Declarations ========================================*/

#define m_V_as$(/*(_T: m_V_Type)(_src)*/... /*(m_V_Type)*/) __step__m_V_as$(__VA_ARGS__)

T_alias$((m_V_Orient)(enum_((m_V_Orient $fits($packed))(
    m_V_Orient_neg = -1,
    m_V_Orient_ngtv = m_V_Orient_neg,
    m_V_Orient_cw = m_V_Orient_neg,
    m_V_Orient_zero = 0,
    m_V_Orient_collinear = m_V_Orient_zero,
    m_V_Orient_coplanar = m_V_Orient_zero,
    m_V_Orient_pos = 1,
    m_V_Orient_pstv = m_V_Orient_pos,
    m_V_Orient_ccw = m_V_Orient_pos
))));
claim_assert_static(eqlType$(m_V_Orient, i8));
#define m_V_Orient_isNeg(_orient /*: m_V_Orient*/... /*(bool)*/) bool_((_orient) < m_V_Orient_zero)
#define m_V_Orient_isNgtv(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isNeg(_orient)
#define m_V_Orient_isCW(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isNeg(_orient)
#define m_V_Orient_isZero(_orient /*: m_V_Orient*/... /*(bool)*/) bool_((_orient) == m_V_Orient_zero)
#define m_V_Orient_isNonzero(_orient /*: m_V_Orient*/... /*(bool)*/) bool_((_orient) != m_V_Orient_zero)
#define m_V_Orient_isCollinear(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isZero(_orient)
#define m_V_Orient_isCoplanar(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isZero(_orient)
#define m_V_Orient_isPos(_orient /*: m_V_Orient*/... /*(bool)*/) bool_((_orient) > m_V_Orient_zero)
#define m_V_Orient_isPstv(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isPos(_orient)
#define m_V_Orient_isCCW(_orient /*: m_V_Orient*/... /*(bool)*/) m_V_Orient_isPos(_orient)

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

/* --- UInt64 (u64) Vector --- */

/* Vec2ul functions */
/* Constants */
#define m_V2u64_zero m_V2u64_of_static(0, 0)
#define m_V2u64_unit_x m_V2u64_of_static(1, 0)
#define m_V2u64_unit_y m_V2u64_of_static(0, 1)
#define m_V2u64_one m_V2u64_of_static(1, 1)
#define m_V2u64_right m_V2u64_of_static(1, 0)
#define m_V2u64_up m_V2u64_of_static(0, 1)
#define m_V2u64_limit_min m_V2u64_of_static(u64_limit_min, u64_limit_min)
#define m_V2u64_limit_max m_V2u64_of_static(u64_limit_max, u64_limit_max)

/* Construction */
#define m_V2u64_of_static(_x, _y...) l$((m_V2u64){ .x = (_x), .y = (_y) })
$attr($inline_always)
$static m_V2u64 m_V2u64_of(u64 x, u64 y);
$attr($inline_always)
$static m_V2u64 m_V2u64_splat(u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_from3(m_V3u64 v);
$attr($inline_always)
$static m_V2u64 m_V2u64_from4(m_V4u64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2u64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2u64 m_V2u64_add(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_sub(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_mul(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_div(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_rem(m_V2u64 lhs, m_V2u64 rhs);

$attr($inline_always)
$static m_V2u64 m_V2u64_scal(m_V2u64 v, u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_scalInv(m_V2u64 v, u64 scalar);

$attr($inline_always)
$static m_V2u64 m_V2u64_addS(m_V2u64 v, u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_subS(m_V2u64 v, u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_mulS(m_V2u64 v, u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_divS(m_V2u64 v, u64 scalar);
$attr($inline_always)
$static m_V2u64 m_V2u64_remS(m_V2u64 v, u64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2u64 m_V2u64_min(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_max(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static m_V2u64 m_V2u64_clamp(m_V2u64 v, m_V2u64 min, m_V2u64 max);
$attr($inline_always)
$static m_V2u64 m_V2u64_wrap(m_V2u64 v, m_V2u64 min, m_V2u64 max);

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V2u64_lenSq(m_V2u64 v);
$attr($inline_always)
$static u64 m_V2u64_distSq(m_V2u64 lhs, m_V2u64 rhs);
$attr($inline_always)
$static u64 m_V2u64_dot(m_V2u64 lhs, m_V2u64 rhs);

/* Vec3ul functions */
/* Constants */
#define m_V3u64_zero m_V3u64_of_static(0, 0, 0)
#define m_V3u64_unit_x m_V3u64_of_static(1, 0, 0)
#define m_V3u64_unit_y m_V3u64_of_static(0, 1, 0)
#define m_V3u64_unit_z m_V3u64_of_static(0, 0, 1)
#define m_V3u64_unit_xy m_V3u64_of_static(1, 1, 0)
#define m_V3u64_unit_yz m_V3u64_of_static(0, 1, 1)
#define m_V3u64_unit_zx m_V3u64_of_static(1, 0, 1)
#define m_V3u64_one m_V3u64_of_static(1, 1, 1)
#define m_V3u64_right m_V3u64_of_static(1, 0, 0)
#define m_V3u64_up m_V3u64_of_static(0, 1, 0)
#define m_V3u64_forward m_V3u64_of_static(0, 0, 1)
#define m_V3u64_view_backward_rh m_V3u64_forward
#define m_V3u64_view_forward_lh m_V3u64_forward
#define m_V3u64_limit_min m_V3u64_of_static(u64_limit_min, u64_limit_min, u64_limit_min)
#define m_V3u64_limit_max m_V3u64_of_static(u64_limit_max, u64_limit_max, u64_limit_max)

/* Construction */
#define m_V3u64_of_static(_x, _y, _z...) l$((m_V3u64){ .x = (_x), .y = (_y), .z = (_z) })
$attr($inline_always)
$static m_V3u64 m_V3u64_of(u64 x, u64 y, u64 z);
$attr($inline_always)
$static m_V3u64 m_V3u64_splat(u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_from2(m_V2u64 v);
$attr($inline_always)
$static m_V3u64 m_V3u64_from4(m_V4u64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3u64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3u64 m_V3u64_add(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_sub(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_mul(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_div(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_rem(m_V3u64 lhs, m_V3u64 rhs);

$attr($inline_always)
$static m_V3u64 m_V3u64_scal(m_V3u64 v, u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_scalInv(m_V3u64 v, u64 scalar);

$attr($inline_always)
$static m_V3u64 m_V3u64_addS(m_V3u64 v, u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_subS(m_V3u64 v, u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_mulS(m_V3u64 v, u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_divS(m_V3u64 v, u64 scalar);
$attr($inline_always)
$static m_V3u64 m_V3u64_remS(m_V3u64 v, u64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3u64 m_V3u64_min(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_max(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static m_V3u64 m_V3u64_clamp(m_V3u64 v, m_V3u64 min, m_V3u64 max);
$attr($inline_always)
$static m_V3u64 m_V3u64_wrap(m_V3u64 v, m_V3u64 min, m_V3u64 max);

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V3u64_lenSq(m_V3u64 v);
$attr($inline_always)
$static u64 m_V3u64_distSq(m_V3u64 lhs, m_V3u64 rhs);
$attr($inline_always)
$static u64 m_V3u64_dot(m_V3u64 lhs, m_V3u64 rhs);

/* Vec4ul functions */
/* Constants */
#define m_V4u64_zero m_V4u64_of_static(0, 0, 0, 0)
#define m_V4u64_unit_x m_V4u64_of_static(1, 0, 0, 0)
#define m_V4u64_unit_y m_V4u64_of_static(0, 1, 0, 0)
#define m_V4u64_unit_z m_V4u64_of_static(0, 0, 1, 0)
#define m_V4u64_unit_w m_V4u64_of_static(0, 0, 0, 1)
#define m_V4u64_one m_V4u64_of_static(1, 1, 1, 1)
#define m_V4u64_right m_V4u64_of_static(1, 0, 0, 0)
#define m_V4u64_up m_V4u64_of_static(0, 1, 0, 0)
#define m_V4u64_forward m_V4u64_of_static(0, 0, 1, 0)
#define m_V4u64_view_backward_rh m_V4u64_forward
#define m_V4u64_view_forward_lh m_V4u64_forward
#define m_V4u64_limit_min m_V4u64_of_static(u64_limit_min, u64_limit_min, u64_limit_min, u64_limit_min)
#define m_V4u64_limit_max m_V4u64_of_static(u64_limit_max, u64_limit_max, u64_limit_max, u64_limit_max)

/* Construction */
#define m_V4u64_of_static(_x, _y, _z, _w...) l$((m_V4u64){ .x = (_x), .y = (_y), .z = (_z), .w = (_w) })
$attr($inline_always)
$static m_V4u64 m_V4u64_of(u64 x, u64 y, u64 z, u64 w);
$attr($inline_always)
$static m_V4u64 m_V4u64_splat(u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_from2(m_V2u64 v);
$attr($inline_always)
$static m_V4u64 m_V4u64_from3(m_V3u64 v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4u64)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4u64)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4u64 m_V4u64_add(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_sub(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_mul(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_div(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_rem(m_V4u64 lhs, m_V4u64 rhs);

$attr($inline_always)
$static m_V4u64 m_V4u64_scal(m_V4u64 v, u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_scalInv(m_V4u64 v, u64 scalar);

$attr($inline_always)
$static m_V4u64 m_V4u64_addS(m_V4u64 v, u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_subS(m_V4u64 v, u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_mulS(m_V4u64 v, u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_divS(m_V4u64 v, u64 scalar);
$attr($inline_always)
$static m_V4u64 m_V4u64_remS(m_V4u64 v, u64 scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4u64 m_V4u64_min(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_max(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static m_V4u64 m_V4u64_clamp(m_V4u64 v, m_V4u64 min, m_V4u64 max);
$attr($inline_always)
$static m_V4u64 m_V4u64_wrap(m_V4u64 v, m_V4u64 min, m_V4u64 max);

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V4u64_lenSq(m_V4u64 v);
$attr($inline_always)
$static u64 m_V4u64_distSq(m_V4u64 lhs, m_V4u64 rhs);
$attr($inline_always)
$static u64 m_V4u64_dot(m_V4u64 lhs, m_V4u64 rhs);

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
$attr($inline_always)
$static m_V2usize m_V2usize_of(usize x, usize y);
$attr($inline_always)
$static m_V2usize m_V2usize_splat(usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_from3(m_V3usize v);
$attr($inline_always)
$static m_V2usize m_V2usize_from4(m_V4usize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2usize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2usize m_V2usize_add(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_sub(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_mul(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_div(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_rem(m_V2usize lhs, m_V2usize rhs);

$attr($inline_always)
$static m_V2usize m_V2usize_scal(m_V2usize v, usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_scalInv(m_V2usize v, usize scalar);

$attr($inline_always)
$static m_V2usize m_V2usize_addS(m_V2usize v, usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_subS(m_V2usize v, usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_mulS(m_V2usize v, usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_divS(m_V2usize v, usize scalar);
$attr($inline_always)
$static m_V2usize m_V2usize_remS(m_V2usize v, usize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2usize m_V2usize_min(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_max(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
$static m_V2usize m_V2usize_clamp(m_V2usize v, m_V2usize min, m_V2usize max);
$attr($inline_always)
$static m_V2usize m_V2usize_wrap(m_V2usize v, m_V2usize min, m_V2usize max);

/* Geometric Operations */
$attr($inline_always)
$static usize m_V2usize_lenSq(m_V2usize v);
$attr($inline_always)
$static usize m_V2usize_distSq(m_V2usize lhs, m_V2usize rhs);
$attr($inline_always)
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
$attr($inline_always)
$static m_V3usize m_V3usize_of(usize x, usize y, usize z);
$attr($inline_always)
$static m_V3usize m_V3usize_splat(usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_from2(m_V2usize v);
$attr($inline_always)
$static m_V3usize m_V3usize_from4(m_V4usize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3usize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3usize m_V3usize_add(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_sub(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_mul(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_div(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_rem(m_V3usize lhs, m_V3usize rhs);

$attr($inline_always)
$static m_V3usize m_V3usize_scal(m_V3usize v, usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_scalInv(m_V3usize v, usize scalar);

$attr($inline_always)
$static m_V3usize m_V3usize_addS(m_V3usize v, usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_subS(m_V3usize v, usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_mulS(m_V3usize v, usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_divS(m_V3usize v, usize scalar);
$attr($inline_always)
$static m_V3usize m_V3usize_remS(m_V3usize v, usize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3usize m_V3usize_min(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_max(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
$static m_V3usize m_V3usize_clamp(m_V3usize v, m_V3usize min, m_V3usize max);
$attr($inline_always)
$static m_V3usize m_V3usize_wrap(m_V3usize v, m_V3usize min, m_V3usize max);

/* Geometric Operations */
$attr($inline_always)
$static usize m_V3usize_lenSq(m_V3usize v);
$attr($inline_always)
$static usize m_V3usize_distSq(m_V3usize lhs, m_V3usize rhs);
$attr($inline_always)
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
$attr($inline_always)
$static m_V4usize m_V4usize_of(usize x, usize y, usize z, usize w);
$attr($inline_always)
$static m_V4usize m_V4usize_splat(usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_from2(m_V2usize v);
$attr($inline_always)
$static m_V4usize m_V4usize_from3(m_V3usize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4usize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4usize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4usize m_V4usize_add(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_sub(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_mul(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_div(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_rem(m_V4usize lhs, m_V4usize rhs);

$attr($inline_always)
$static m_V4usize m_V4usize_scal(m_V4usize v, usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_scalInv(m_V4usize v, usize scalar);

$attr($inline_always)
$static m_V4usize m_V4usize_addS(m_V4usize v, usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_subS(m_V4usize v, usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_mulS(m_V4usize v, usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_divS(m_V4usize v, usize scalar);
$attr($inline_always)
$static m_V4usize m_V4usize_remS(m_V4usize v, usize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4usize m_V4usize_min(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_max(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static m_V4usize m_V4usize_clamp(m_V4usize v, m_V4usize min, m_V4usize max);
$attr($inline_always)
$static m_V4usize m_V4usize_wrap(m_V4usize v, m_V4usize min, m_V4usize max);

/* Geometric Operations */
$attr($inline_always)
$static usize m_V4usize_lenSq(m_V4usize v);
$attr($inline_always)
$static usize m_V4usize_distSq(m_V4usize lhs, m_V4usize rhs);
$attr($inline_always)
$static usize m_V4usize_dot(m_V4usize lhs, m_V4usize rhs);

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

/* --- Int64 (i64) Vector --- */

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
$static m_V2i64 m_V2i64_rem(m_V2i64 lhs, m_V2i64 rhs);

$attr($inline_always)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar);

$attr($inline_always)
$static m_V2i64 m_V2i64_addS(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_subS(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_mulS(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_divS(m_V2i64 v, i64 scalar);
$attr($inline_always)
$static m_V2i64 m_V2i64_remS(m_V2i64 v, i64 scalar);

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
$static i64 m_V2i64_det(m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static i64 m_V2i64_orient(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V_Orient m_V2i64_orientDir(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static bool m_V2i64_isCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static bool m_V2i64_isCollinear(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static bool m_V2i64_isCCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs);
$attr($inline_always)
$static m_V2i64 m_V2i64_cw(m_V2i64 v);
$attr($inline_always)
$static m_V2i64 m_V2i64_ccw(m_V2i64 v);
$attr($inline_always)
$static m_V2i64 m_V2i64_skew(m_V2i64 v);
$attr($inline_always)
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
$static m_V3i64 m_V3i64_rem(m_V3i64 lhs, m_V3i64 rhs);

$attr($inline_always)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar);

$attr($inline_always)
$static m_V3i64 m_V3i64_addS(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_subS(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_mulS(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_divS(m_V3i64 v, i64 scalar);
$attr($inline_always)
$static m_V3i64 m_V3i64_remS(m_V3i64 v, i64 scalar);

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
$static i64 m_V3i64_det(m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
$static i64 m_V3i64_orient(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
$static m_V_Orient m_V3i64_orientDir(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
$static bool m_V3i64_isOrientNeg(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
$static bool m_V3i64_isCoplanar(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
$static bool m_V3i64_isOrientPos(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs);
$attr($inline_always)
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
$static m_V4i64 m_V4i64_rem(m_V4i64 lhs, m_V4i64 rhs);

$attr($inline_always)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar);

$attr($inline_always)
$static m_V4i64 m_V4i64_addS(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_subS(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_mulS(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_divS(m_V4i64 v, i64 scalar);
$attr($inline_always)
$static m_V4i64 m_V4i64_remS(m_V4i64 v, i64 scalar);

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

/* --- Isize (isize) Vector --- */

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
$attr($inline_always)
$static m_V2isize m_V2isize_of(isize x, isize y);
$attr($inline_always)
$static m_V2isize m_V2isize_splat(isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_from3(m_V3isize v);
$attr($inline_always)
$static m_V2isize m_V2isize_from4(m_V4isize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V2isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V2isize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V2isize m_V2isize_neg(m_V2isize v);
$attr($inline_always)
$static m_V2isize m_V2isize_add(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_sub(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_mul(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_div(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_rem(m_V2isize lhs, m_V2isize rhs);

$attr($inline_always)
$static m_V2isize m_V2isize_scal(m_V2isize v, isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_scalInv(m_V2isize v, isize scalar);

$attr($inline_always)
$static m_V2isize m_V2isize_addS(m_V2isize v, isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_subS(m_V2isize v, isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_mulS(m_V2isize v, isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_divS(m_V2isize v, isize scalar);
$attr($inline_always)
$static m_V2isize m_V2isize_remS(m_V2isize v, isize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V2isize m_V2isize_min(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_max(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_clamp(m_V2isize v, m_V2isize min, m_V2isize max);
$attr($inline_always)
$static m_V2isize m_V2isize_wrap(m_V2isize v, m_V2isize min, m_V2isize max);

/* Geometric Operations */
$attr($inline_always)
$static isize m_V2isize_lenSq(m_V2isize v);
$attr($inline_always)
$static isize m_V2isize_distSq(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static isize m_V2isize_dot(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static isize m_V2isize_cross(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static isize m_V2isize_det(m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static isize m_V2isize_orient(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V_Orient m_V2isize_orientDir(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static bool m_V2isize_isCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static bool m_V2isize_isCollinear(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static bool m_V2isize_isCCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs);
$attr($inline_always)
$static m_V2isize m_V2isize_cw(m_V2isize v);
$attr($inline_always)
$static m_V2isize m_V2isize_ccw(m_V2isize v);
$attr($inline_always)
$static m_V2isize m_V2isize_skew(m_V2isize v);
$attr($inline_always)
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
$attr($inline_always)
$static m_V3isize m_V3isize_of(isize x, isize y, isize z);
$attr($inline_always)
$static m_V3isize m_V3isize_splat(isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_from2(m_V2isize v);
$attr($inline_always)
$static m_V3isize m_V3isize_from4(m_V4isize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V3isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V3isize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V3isize m_V3isize_neg(m_V3isize v);
$attr($inline_always)
$static m_V3isize m_V3isize_add(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_sub(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_mul(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_div(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_rem(m_V3isize lhs, m_V3isize rhs);

$attr($inline_always)
$static m_V3isize m_V3isize_scal(m_V3isize v, isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_scalInv(m_V3isize v, isize scalar);

$attr($inline_always)
$static m_V3isize m_V3isize_addS(m_V3isize v, isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_subS(m_V3isize v, isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_mulS(m_V3isize v, isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_divS(m_V3isize v, isize scalar);
$attr($inline_always)
$static m_V3isize m_V3isize_remS(m_V3isize v, isize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V3isize m_V3isize_min(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_max(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_clamp(m_V3isize v, m_V3isize min, m_V3isize max);
$attr($inline_always)
$static m_V3isize m_V3isize_wrap(m_V3isize v, m_V3isize min, m_V3isize max);

/* Geometric Operations */
$attr($inline_always)
$static isize m_V3isize_lenSq(m_V3isize v);
$attr($inline_always)
$static isize m_V3isize_distSq(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static isize m_V3isize_dot(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static m_V3isize m_V3isize_cross(m_V3isize lhs, m_V3isize rhs);
$attr($inline_always)
$static isize m_V3isize_det(m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
$static isize m_V3isize_orient(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
$static m_V_Orient m_V3isize_orientDir(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
$static bool m_V3isize_isOrientNeg(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
$static bool m_V3isize_isCoplanar(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
$static bool m_V3isize_isOrientPos(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs);
$attr($inline_always)
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
$attr($inline_always)
$static m_V4isize m_V4isize_of(isize x, isize y, isize z, isize w);
$attr($inline_always)
$static m_V4isize m_V4isize_splat(isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_from2(m_V2isize v);
$attr($inline_always)
$static m_V4isize m_V4isize_from3(m_V3isize v);

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq_default$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((m_V4isize)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((m_V4isize)(lhs, rhs));

/* Arithmetic */
$attr($inline_always)
$static m_V4isize m_V4isize_neg(m_V4isize v);
$attr($inline_always)
$static m_V4isize m_V4isize_add(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_sub(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_mul(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_div(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_rem(m_V4isize lhs, m_V4isize rhs);

$attr($inline_always)
$static m_V4isize m_V4isize_scal(m_V4isize v, isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_scalInv(m_V4isize v, isize scalar);

$attr($inline_always)
$static m_V4isize m_V4isize_addS(m_V4isize v, isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_subS(m_V4isize v, isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_mulS(m_V4isize v, isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_divS(m_V4isize v, isize scalar);
$attr($inline_always)
$static m_V4isize m_V4isize_remS(m_V4isize v, isize scalar);

/* Range Operations */
$attr($inline_always)
$static m_V4isize m_V4isize_min(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_max(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static m_V4isize m_V4isize_clamp(m_V4isize v, m_V4isize min, m_V4isize max);
$attr($inline_always)
$static m_V4isize m_V4isize_wrap(m_V4isize v, m_V4isize min, m_V4isize max);

/* Geometric Operations */
$attr($inline_always)
$static isize m_V4isize_lenSq(m_V4isize v);
$attr($inline_always)
$static isize m_V4isize_distSq(m_V4isize lhs, m_V4isize rhs);
$attr($inline_always)
$static isize m_V4isize_dot(m_V4isize lhs, m_V4isize rhs);

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
$static cmp_fn_ordApx$((m_V2f32)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V2f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V2f32)(lhs, rhs, threshold));

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
$static m_V2f32 m_V2f32_rem(m_V2f32 lhs, m_V2f32 rhs);

$attr($inline_always)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar);

$attr($inline_always)
$static m_V2f32 m_V2f32_addS(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_subS(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_mulS(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_divS(m_V2f32 v, f32 scalar);
$attr($inline_always)
$static m_V2f32 m_V2f32_remS(m_V2f32 v, f32 scalar);

$attr($inline_always)
$static m_V2f32* m_V2f32_addAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_subAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_mulAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_divAsg(m_V2f32* lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32* m_V2f32_remAsg(m_V2f32* lhs, m_V2f32 rhs);

$attr($inline_always)
$static m_V2f32* m_V2f32_scalAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_scalInvAsg(m_V2f32* v, f32 scalar);

$attr($inline_always)
$static m_V2f32* m_V2f32_addSAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_subSAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_mulSAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_divSAsg(m_V2f32* v, f32 scalar);
$attr($inline_always)
$static m_V2f32* m_V2f32_remSAsg(m_V2f32* v, f32 scalar);

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
$static f32 m_V2f32_det(m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static f32 m_V2f32_orient(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V_Orient m_V2f32_orientDir(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static bool m_V2f32_isCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static bool m_V2f32_isCollinear(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static bool m_V2f32_isCCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs);
$attr($inline_always)
$static m_V2f32 m_V2f32_cw(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_ccw(m_V2f32 v);
$attr($inline_always)
$static m_V2f32 m_V2f32_skew(m_V2f32 v);
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
$static cmp_fn_ordApx$((m_V3f32)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V3f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V3f32)(lhs, rhs, threshold));

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
$static m_V3f32 m_V3f32_rem(m_V3f32 lhs, m_V3f32 rhs);

$attr($inline_always)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar);

$attr($inline_always)
$static m_V3f32 m_V3f32_addS(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_subS(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_mulS(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_divS(m_V3f32 v, f32 scalar);
$attr($inline_always)
$static m_V3f32 m_V3f32_remS(m_V3f32 v, f32 scalar);

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
$static f32 m_V3f32_det(m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr($inline_always)
$static f32 m_V3f32_orient(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr($inline_always)
$static m_V_Orient m_V3f32_orientDir(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr($inline_always)
$static bool m_V3f32_isOrientNeg(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr($inline_always)
$static bool m_V3f32_isCoplanar(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
$attr($inline_always)
$static bool m_V3f32_isOrientPos(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs);
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
$static cmp_fn_ordApx$((m_V4f32)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V4f32)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V4f32)(lhs, rhs, threshold));

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
$static m_V4f32 m_V4f32_rem(m_V4f32 lhs, m_V4f32 rhs);

$attr($inline_always)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar);

$attr($inline_always)
$static m_V4f32 m_V4f32_addS(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_subS(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_mulS(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_divS(m_V4f32 v, f32 scalar);
$attr($inline_always)
$static m_V4f32 m_V4f32_remS(m_V4f32 v, f32 scalar);

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

/* --- Float64 (f64) Vector --- */

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

$attr($inline_always)
$static cmp_fn_ordApx$((m_V2f64)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V2f64)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V2f64)(lhs, rhs, threshold));

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
$static m_V2f64 m_V2f64_rem(m_V2f64 lhs, m_V2f64 rhs);

$attr($inline_always)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar);
$attr($inline_always)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler);

$attr($inline_always)
$static m_V2f64 m_V2f64_addS(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_subS(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_mulS(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_divS(m_V2f64 lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_remS(m_V2f64 lhs, f64 rhs);

$attr($inline_always)
$static m_V2f64* m_V2f64_addAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_subAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_mulAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_divAsg(m_V2f64* lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_remAsg(m_V2f64* lhs, m_V2f64 rhs);

$attr($inline_always)
$static m_V2f64* m_V2f64_scalAsg(m_V2f64* v, f64 scalar);
$attr($inline_always)
$static m_V2f64* m_V2f64_scalInvAsg(m_V2f64* v, f64 scalar);

$attr($inline_always)
$static m_V2f64* m_V2f64_addSAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_subSAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_mulSAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_divSAsg(m_V2f64* lhs, f64 rhs);
$attr($inline_always)
$static m_V2f64* m_V2f64_remSAsg(m_V2f64* lhs, f64 rhs);

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
$static f64 m_V2f64_det(m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static f64 m_V2f64_orient(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V_Orient m_V2f64_orientDir(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static bool m_V2f64_isCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static bool m_V2f64_isCollinear(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static bool m_V2f64_isCCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs);
$attr($inline_always)
$static m_V2f64 m_V2f64_cw(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_ccw(m_V2f64 v);
$attr($inline_always)
$static m_V2f64 m_V2f64_skew(m_V2f64 v);
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

$attr($inline_always)
$static cmp_fn_ordApx$((m_V3f64)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V3f64)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V3f64)(lhs, rhs, threshold));

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
$static m_V3f64 m_V3f64_rem(m_V3f64 lhs, m_V3f64 rhs);

$attr($inline_always)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar);

$attr($inline_always)
$static m_V3f64 m_V3f64_addS(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_subS(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_mulS(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_divS(m_V3f64 v, f64 scalar);
$attr($inline_always)
$static m_V3f64 m_V3f64_remS(m_V3f64 v, f64 scalar);

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
$static f64 m_V3f64_det(m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr($inline_always)
$static f64 m_V3f64_orient(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr($inline_always)
$static m_V_Orient m_V3f64_orientDir(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr($inline_always)
$static bool m_V3f64_isOrientNeg(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr($inline_always)
$static bool m_V3f64_isCoplanar(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
$attr($inline_always)
$static bool m_V3f64_isOrientPos(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs);
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

$attr($inline_always)
$static cmp_fn_ordApx$((m_V4f64)(lhs, rhs, threshold, mode));
$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V4f64)(lhs, rhs, threshold));
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V4f64)(lhs, rhs, threshold));

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
$static m_V4f64 m_V4f64_rem(m_V4f64 lhs, m_V4f64 rhs);

$attr($inline_always)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar);

$attr($inline_always)
$static m_V4f64 m_V4f64_addS(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_subS(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_mulS(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_divS(m_V4f64 v, f64 scalar);
$attr($inline_always)
$static m_V4f64 m_V4f64_remS(m_V4f64 v, f64 scalar);

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

/*========== Implementations ================================================*/

#define __step__m_V_as$(...) __step__m_V_as$__emit(__step__m_V_as$__parse __VA_ARGS__)
#define __step__m_V_as$__parse(_T_Dst...) \
    pp_uniqTok(len), pp_uniqTok(d), pp_uniqTok(s), \
        pp_uniqTok(dst), _T_Dst, pp_uniqTok(src),
#define __step__m_V_as$__emit(...) ____m_V_as$(__VA_ARGS__)
#define ____m_V_as$(__len, __d, __s, __dst, _T_Dst, __src, _src...) local_({ \
    var_(__dst, _T_Dst) = cleared(); \
    let_(__src, TypeOf(_src)) = _src; \
    let_(__len, usize) = A_len$(TypeOf(__dst.s)); \
    claim_assert_static(__len == A_len$(TypeOf(__src.s))); \
    for_(($s(A_ref(__dst.s)), $s(A_ref(__src.s)))(__d, __s)) { \
        *__d = as$(TypeOf(*__d))(*__s); \
    } $end(for); \
    local_return_(__dst); \
})

#if on_analysis_active_only || on_comptime
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


/* Vec2ul functions */
/* Construction */
$attr($inline_always)
$static m_V2u64 m_V2u64_of(u64 x, u64 y) {
    return (m_V2u64){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2u64 m_V2u64_splat(u64 scalar) {
    return m_V2u64_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_from3(m_V3u64 v) {
    return m_V2u64_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_from4(m_V4u64 v) {
    return m_V2u64_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2u64)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2u64 m_V2u64_add(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_sub(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_mul(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_div(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_rem(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2u64 m_V2u64_scal(m_V2u64 v, u64 scalar) {
    return m_V2u64_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_scalInv(m_V2u64 v, u64 scalar) {
    return m_V2u64_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2u64 m_V2u64_addS(m_V2u64 v, u64 scalar) {
    return m_V2u64_add(v, m_V2u64_splat(scalar));
}
$attr($inline_always)
$static m_V2u64 m_V2u64_subS(m_V2u64 v, u64 scalar) {
    return m_V2u64_sub(v, m_V2u64_splat(scalar));
}
$attr($inline_always)
$static m_V2u64 m_V2u64_mulS(m_V2u64 v, u64 scalar) {
    return m_V2u64_mul(v, m_V2u64_splat(scalar));
}
$attr($inline_always)
$static m_V2u64 m_V2u64_divS(m_V2u64 v, u64 scalar) {
    return m_V2u64_div(v, m_V2u64_splat(scalar));
}
$attr($inline_always)
$static m_V2u64 m_V2u64_remS(m_V2u64 v, u64 scalar) {
    return m_V2u64_rem(v, m_V2u64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2u64 m_V2u64_min(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2u64 m_V2u64_max(m_V2u64 lhs, m_V2u64 rhs) {
    return m_V2u64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2u64 m_V2u64_clamp(m_V2u64 v, m_V2u64 min, m_V2u64 max) {
    return m_V2u64_min(m_V2u64_max(v, min), max);
}
$attr($inline_always)
$static m_V2u64 m_V2u64_wrap(m_V2u64 v, m_V2u64 min, m_V2u64 max) {
    return m_V2u64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V2u64_lenSq(m_V2u64 v) {
    return m_V2u64_dot(v, v);
}
$attr($inline_always)
$static u64 m_V2u64_distSq(m_V2u64 lhs, m_V2u64 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    return d_x * d_x + d_y * d_y;
}
$attr($inline_always)
$static u64 m_V2u64_dot(m_V2u64 lhs, m_V2u64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}


/* Vec3ul functions */
/* Construction */
$attr($inline_always)
$static m_V3u64 m_V3u64_of(u64 x, u64 y, u64 z) {
    return (m_V3u64){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3u64 m_V3u64_splat(u64 scalar) {
    return m_V3u64_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_from2(m_V2u64 v) {
    return m_V3u64_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_from4(m_V4u64 v) {
    return m_V3u64_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3u64)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3u64 m_V3u64_add(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_sub(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_mul(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_div(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_rem(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3u64 m_V3u64_scal(m_V3u64 v, u64 scalar) {
    return m_V3u64_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_scalInv(m_V3u64 v, u64 scalar) {
    return m_V3u64_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3u64 m_V3u64_addS(m_V3u64 v, u64 scalar) {
    return m_V3u64_add(v, m_V3u64_splat(scalar));
}
$attr($inline_always)
$static m_V3u64 m_V3u64_subS(m_V3u64 v, u64 scalar) {
    return m_V3u64_sub(v, m_V3u64_splat(scalar));
}
$attr($inline_always)
$static m_V3u64 m_V3u64_mulS(m_V3u64 v, u64 scalar) {
    return m_V3u64_mul(v, m_V3u64_splat(scalar));
}
$attr($inline_always)
$static m_V3u64 m_V3u64_divS(m_V3u64 v, u64 scalar) {
    return m_V3u64_div(v, m_V3u64_splat(scalar));
}
$attr($inline_always)
$static m_V3u64 m_V3u64_remS(m_V3u64 v, u64 scalar) {
    return m_V3u64_rem(v, m_V3u64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3u64 m_V3u64_min(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3u64 m_V3u64_max(m_V3u64 lhs, m_V3u64 rhs) {
    return m_V3u64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3u64 m_V3u64_clamp(m_V3u64 v, m_V3u64 min, m_V3u64 max) {
    return m_V3u64_min(m_V3u64_max(v, min), max);
}
$attr($inline_always)
$static m_V3u64 m_V3u64_wrap(m_V3u64 v, m_V3u64 min, m_V3u64 max) {
    return m_V3u64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V3u64_lenSq(m_V3u64 v) {
    return m_V3u64_dot(v, v);
}
$attr($inline_always)
$static u64 m_V3u64_distSq(m_V3u64 lhs, m_V3u64 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    return d_x * d_x + d_y * d_y + d_z * d_z;
}
$attr($inline_always)
$static u64 m_V3u64_dot(m_V3u64 lhs, m_V3u64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


/* Vec4ul functions */
/* Construction */
$attr($inline_always)
$static m_V4u64 m_V4u64_of(u64 x, u64 y, u64 z, u64 w) {
    return (m_V4u64){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4u64 m_V4u64_splat(u64 scalar) {
    return m_V4u64_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_from2(m_V2u64 v) {
    return m_V4u64_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_from3(m_V3u64 v) {
    return m_V4u64_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4u64)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4u64 m_V4u64_add(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_sub(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_mul(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_div(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_rem(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4u64 m_V4u64_scal(m_V4u64 v, u64 scalar) {
    return m_V4u64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_scalInv(m_V4u64 v, u64 scalar) {
    return m_V4u64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4u64 m_V4u64_addS(m_V4u64 v, u64 scalar) {
    return m_V4u64_add(v, m_V4u64_splat(scalar));
}
$attr($inline_always)
$static m_V4u64 m_V4u64_subS(m_V4u64 v, u64 scalar) {
    return m_V4u64_sub(v, m_V4u64_splat(scalar));
}
$attr($inline_always)
$static m_V4u64 m_V4u64_mulS(m_V4u64 v, u64 scalar) {
    return m_V4u64_mul(v, m_V4u64_splat(scalar));
}
$attr($inline_always)
$static m_V4u64 m_V4u64_divS(m_V4u64 v, u64 scalar) {
    return m_V4u64_div(v, m_V4u64_splat(scalar));
}
$attr($inline_always)
$static m_V4u64 m_V4u64_remS(m_V4u64 v, u64 scalar) {
    return m_V4u64_rem(v, m_V4u64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4u64 m_V4u64_min(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4u64 m_V4u64_max(m_V4u64 lhs, m_V4u64 rhs) {
    return m_V4u64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4u64 m_V4u64_clamp(m_V4u64 v, m_V4u64 min, m_V4u64 max) {
    return m_V4u64_min(m_V4u64_max(v, min), max);
}
$attr($inline_always)
$static m_V4u64 m_V4u64_wrap(m_V4u64 v, m_V4u64 min, m_V4u64 max) {
    return m_V4u64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static u64 m_V4u64_lenSq(m_V4u64 v) {
    return m_V4u64_dot(v, v);
}
$attr($inline_always)
$static u64 m_V4u64_distSq(m_V4u64 lhs, m_V4u64 rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    let d_w = lhs.w < rhs.w ? rhs.w - lhs.w : lhs.w - rhs.w;
    return d_x * d_x + d_y * d_y + d_z * d_z + d_w * d_w;
}
$attr($inline_always)
$static u64 m_V4u64_dot(m_V4u64 lhs, m_V4u64 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}


/* Vec2uz functions */
/* Construction */
$attr($inline_always)
$static m_V2usize m_V2usize_of(usize x, usize y) {
    return (m_V2usize){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2usize m_V2usize_splat(usize scalar) {
    return m_V2usize_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2usize m_V2usize_from3(m_V3usize v) {
    return m_V2usize_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2usize m_V2usize_from4(m_V4usize v) {
    return m_V2usize_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2usize m_V2usize_add(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2usize m_V2usize_sub(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2usize m_V2usize_mul(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2usize m_V2usize_div(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2usize m_V2usize_rem(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2usize m_V2usize_scal(m_V2usize v, usize scalar) {
    return m_V2usize_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2usize m_V2usize_scalInv(m_V2usize v, usize scalar) {
    return m_V2usize_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2usize m_V2usize_addS(m_V2usize v, usize scalar) {
    return m_V2usize_add(v, m_V2usize_splat(scalar));
}
$attr($inline_always)
$static m_V2usize m_V2usize_subS(m_V2usize v, usize scalar) {
    return m_V2usize_sub(v, m_V2usize_splat(scalar));
}
$attr($inline_always)
$static m_V2usize m_V2usize_mulS(m_V2usize v, usize scalar) {
    return m_V2usize_mul(v, m_V2usize_splat(scalar));
}
$attr($inline_always)
$static m_V2usize m_V2usize_divS(m_V2usize v, usize scalar) {
    return m_V2usize_div(v, m_V2usize_splat(scalar));
}
$attr($inline_always)
$static m_V2usize m_V2usize_remS(m_V2usize v, usize scalar) {
    return m_V2usize_rem(v, m_V2usize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2usize m_V2usize_min(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2usize m_V2usize_max(m_V2usize lhs, m_V2usize rhs) {
    return m_V2usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2usize m_V2usize_clamp(m_V2usize v, m_V2usize min, m_V2usize max) {
    return m_V2usize_min(m_V2usize_max(v, min), max);
}
$attr($inline_always)
$static m_V2usize m_V2usize_wrap(m_V2usize v, m_V2usize min, m_V2usize max) {
    return m_V2usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static usize m_V2usize_lenSq(m_V2usize v) {
    return m_V2usize_dot(v, v);
}
$attr($inline_always)
$static usize m_V2usize_distSq(m_V2usize lhs, m_V2usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    return d_x * d_x + d_y * d_y;
}
$attr($inline_always)
$static usize m_V2usize_dot(m_V2usize lhs, m_V2usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}


/* Vec3uz functions */
/* Construction */
$attr($inline_always)
$static m_V3usize m_V3usize_of(usize x, usize y, usize z) {
    return (m_V3usize){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3usize m_V3usize_splat(usize scalar) {
    return m_V3usize_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3usize m_V3usize_from2(m_V2usize v) {
    return m_V3usize_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3usize m_V3usize_from4(m_V4usize v) {
    return m_V3usize_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3usize m_V3usize_add(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3usize m_V3usize_sub(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3usize m_V3usize_mul(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3usize m_V3usize_div(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3usize m_V3usize_rem(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3usize m_V3usize_scal(m_V3usize v, usize scalar) {
    return m_V3usize_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3usize m_V3usize_scalInv(m_V3usize v, usize scalar) {
    return m_V3usize_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3usize m_V3usize_addS(m_V3usize v, usize scalar) {
    return m_V3usize_add(v, m_V3usize_splat(scalar));
}
$attr($inline_always)
$static m_V3usize m_V3usize_subS(m_V3usize v, usize scalar) {
    return m_V3usize_sub(v, m_V3usize_splat(scalar));
}
$attr($inline_always)
$static m_V3usize m_V3usize_mulS(m_V3usize v, usize scalar) {
    return m_V3usize_mul(v, m_V3usize_splat(scalar));
}
$attr($inline_always)
$static m_V3usize m_V3usize_divS(m_V3usize v, usize scalar) {
    return m_V3usize_div(v, m_V3usize_splat(scalar));
}
$attr($inline_always)
$static m_V3usize m_V3usize_remS(m_V3usize v, usize scalar) {
    return m_V3usize_rem(v, m_V3usize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3usize m_V3usize_min(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3usize m_V3usize_max(m_V3usize lhs, m_V3usize rhs) {
    return m_V3usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3usize m_V3usize_clamp(m_V3usize v, m_V3usize min, m_V3usize max) {
    return m_V3usize_min(m_V3usize_max(v, min), max);
}
$attr($inline_always)
$static m_V3usize m_V3usize_wrap(m_V3usize v, m_V3usize min, m_V3usize max) {
    return m_V3usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static usize m_V3usize_lenSq(m_V3usize v) {
    return m_V3usize_dot(v, v);
}
$attr($inline_always)
$static usize m_V3usize_distSq(m_V3usize lhs, m_V3usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    return d_x * d_x + d_y * d_y + d_z * d_z;
}
$attr($inline_always)
$static usize m_V3usize_dot(m_V3usize lhs, m_V3usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


/* Vec4uz functions */
/* Construction */
$attr($inline_always)
$static m_V4usize m_V4usize_of(usize x, usize y, usize z, usize w) {
    return (m_V4usize){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4usize m_V4usize_splat(usize scalar) {
    return m_V4usize_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4usize m_V4usize_from2(m_V2usize v) {
    return m_V4usize_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4usize m_V4usize_from3(m_V3usize v) {
    return m_V4usize_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4usize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4usize m_V4usize_add(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4usize m_V4usize_sub(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4usize m_V4usize_mul(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4usize m_V4usize_div(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4usize m_V4usize_rem(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4usize m_V4usize_scal(m_V4usize v, usize scalar) {
    return m_V4usize_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4usize m_V4usize_scalInv(m_V4usize v, usize scalar) {
    return m_V4usize_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4usize m_V4usize_addS(m_V4usize v, usize scalar) {
    return m_V4usize_add(v, m_V4usize_splat(scalar));
}
$attr($inline_always)
$static m_V4usize m_V4usize_subS(m_V4usize v, usize scalar) {
    return m_V4usize_sub(v, m_V4usize_splat(scalar));
}
$attr($inline_always)
$static m_V4usize m_V4usize_mulS(m_V4usize v, usize scalar) {
    return m_V4usize_mul(v, m_V4usize_splat(scalar));
}
$attr($inline_always)
$static m_V4usize m_V4usize_divS(m_V4usize v, usize scalar) {
    return m_V4usize_div(v, m_V4usize_splat(scalar));
}
$attr($inline_always)
$static m_V4usize m_V4usize_remS(m_V4usize v, usize scalar) {
    return m_V4usize_rem(v, m_V4usize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4usize m_V4usize_min(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4usize m_V4usize_max(m_V4usize lhs, m_V4usize rhs) {
    return m_V4usize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4usize m_V4usize_clamp(m_V4usize v, m_V4usize min, m_V4usize max) {
    return m_V4usize_min(m_V4usize_max(v, min), max);
}
$attr($inline_always)
$static m_V4usize m_V4usize_wrap(m_V4usize v, m_V4usize min, m_V4usize max) {
    return m_V4usize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static usize m_V4usize_lenSq(m_V4usize v) {
    return m_V4usize_dot(v, v);
}
$attr($inline_always)
$static usize m_V4usize_distSq(m_V4usize lhs, m_V4usize rhs) {
    let d_x = lhs.x < rhs.x ? rhs.x - lhs.x : lhs.x - rhs.x;
    let d_y = lhs.y < rhs.y ? rhs.y - lhs.y : lhs.y - rhs.y;
    let d_z = lhs.z < rhs.z ? rhs.z - lhs.z : lhs.z - rhs.z;
    let d_w = lhs.w < rhs.w ? rhs.w - lhs.w : lhs.w - rhs.w;
    return d_x * d_x + d_y * d_y + d_z * d_z + d_w * d_w;
}
$attr($inline_always)
$static usize m_V4usize_dot(m_V4usize lhs, m_V4usize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2i functions */
/* Construction */
$attr($inline_always)
$static m_V2i32 m_V2i32_of(i32 x, i32 y) {
#if arch_simd_supported
    let result = simd_V_from$((i32){ x, y });
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return (m_V2i32){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2i32 m_V2i32_splat(i32 scalar) {
#if arch_simd_supported
    let result = m_simd_splat2$(i32, scalar);
    return m_simd_to2$(m_V2i32, result);
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
    return simd_V_ord(m_simd_from2(lhs), m_simd_from2(rhs));
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
    let result = simd_V_neg(m_simd_from2(v));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_add(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_sub(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_mul(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_div(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_rem(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from2(lhs), m_simd_from2(rhs), int_rem);
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(int_rem(lhs.x, rhs.x), int_rem(lhs.y, rhs.y));
}

$attr($inline_always)
$static m_V2i32 m_V2i32_scal(m_V2i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(v), m_simd_splat2$(i32, scalar));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_scalInv(m_V2i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(v), m_simd_splat2$(i32, scalar));
    return m_simd_to2$(m_V2i32, result);
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
    let result = simd_V_min(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(pri_min(lhs.x, rhs.x), pri_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_max(m_V2i32 lhs, m_V2i32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_of(pri_max(lhs.x, rhs.x), pri_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2i32 m_V2i32_clamp(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max));
    return m_simd_to2$(m_V2i32, result);
#endif /* arch_simd_supported */
    return m_V2i32_min(m_V2i32_max(v, min), max);
}
$attr($inline_always)
$static m_V2i32 m_V2i32_wrap(m_V2i32 v, m_V2i32 min, m_V2i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max), int_wrap);
    return m_simd_to2$(m_V2i32, result);
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
    let result = simd_V_dot(m_simd_from2(lhs), m_simd_from2(rhs));
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
    return m_simd_to3$(m_V3i32, result).z;
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
    return m_simd_to3$(m_V3i32, result);
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
    let result = m_simd_splat3$(i32, scalar);
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(scalar, scalar, scalar);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3i32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_simd_from3(lhs), m_simd_from3(rhs));
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
    let result = simd_V_neg(m_simd_from3(v));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_add(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_sub(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_mul(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_div(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(lhs), m_simd_from3Divisor(rhs));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_rem(m_V3i32 lhs, m_V3i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from3(lhs), m_simd_from3Divisor(rhs), int_rem);
    return m_simd_to3$(m_V3i32, result);
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
    let result = simd_V_mul(m_simd_from3(v), m_simd_splat3$(i32, scalar));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_scalInv(m_V3i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(v), m_simd_splat3$(i32, scalar));
    return m_simd_to3$(m_V3i32, result);
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
    let result = simd_V_min(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i32, result);
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
    let result = simd_V_max(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i32, result);
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
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(min), m_simd_from3(max));
    return m_simd_to3$(m_V3i32, result);
#endif /* arch_simd_supported */
    return m_V3i32_min(m_V3i32_max(v, min), max);
}
$attr($inline_always)
$static m_V3i32 m_V3i32_wrap(m_V3i32 v, m_V3i32 min, m_V3i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from3(v), m_simd_from3(min), m_simd_from3Divisor(max), int_wrap);
    return m_simd_to3$(m_V3i32, result);
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
    let result = simd_V_dot(m_simd_from3(lhs), m_simd_from3(rhs));
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
    return m_simd_to3$(m_V3i32, result);
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
    let source = m_simd_from3(v);
    if (v.x != 0 || v.y != 0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((i32){ -1, 1, 0, 0 })
        );
        return m_simd_to3$(m_V3i32, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((i32){ 0, -1, 1, 0 })
    );
    return m_simd_to3$(m_V3i32, result);
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
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return (m_V4i32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4i32 m_V4i32_splat(i32 scalar) {
#if arch_simd_supported
    let result = m_simd_splat4$(i32, scalar);
    return m_simd_to4$(m_V4i32, result);
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
    return simd_V_ord(m_simd_from4(lhs), m_simd_from4(rhs));
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
    let result = simd_V_neg(m_simd_from4(v));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_add(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_sub(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_mul(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_div(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_rem(m_V4i32 lhs, m_V4i32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from4(lhs), m_simd_from4(rhs), int_rem);
    return m_simd_to4$(m_V4i32, result);
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
    let result = simd_V_mul(m_simd_from4(v), m_simd_splat4$(i32, scalar));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_scalInv(m_V4i32 v, i32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(v), m_simd_splat4$(i32, scalar));
    return m_simd_to4$(m_V4i32, result);
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
    let result = simd_V_min(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
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
    let result = simd_V_max(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i32, result);
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
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max));
    return m_simd_to4$(m_V4i32, result);
#endif /* arch_simd_supported */
    return m_V4i32_min(m_V4i32_max(v, min), max);
}
$attr($inline_always)
$static m_V4i32 m_V4i32_wrap(m_V4i32 v, m_V4i32 min, m_V4i32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max), int_wrap);
    return m_simd_to4$(m_V4i32, result);
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
    let result = simd_V_dot(m_simd_from4(lhs), m_simd_from4(rhs));
    return l$((i32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2l functions */
/* Construction */
$attr($inline_always)
$static m_V2i64 m_V2i64_of(i64 x, i64 y) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y });
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return (m_V2i64){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2i64 m_V2i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat2$(i64, scalar);
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from2(lhs), m_simd_from2(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2i64 m_V2i64_neg(m_V2i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from2(v));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_add(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_sub(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_mul(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_div(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_rem(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from2(lhs), m_simd_from2(rhs), int_rem);
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2i64 m_V2i64_scal(m_V2i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(v), m_simd_splat2$(i64, scalar));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_scalInv(m_V2i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(v), m_simd_splat2$(i64, scalar));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2i64 m_V2i64_addS(m_V2i64 v, i64 scalar) {
    return m_V2i64_add(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_subS(m_V2i64 v, i64 scalar) {
    return m_V2i64_sub(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_mulS(m_V2i64 v, i64 scalar) {
    return m_V2i64_mul(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_divS(m_V2i64 v, i64 scalar) {
    return m_V2i64_div(v, m_V2i64_splat(scalar));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_remS(m_V2i64 v, i64 scalar) {
    return m_V2i64_rem(v, m_V2i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2i64 m_V2i64_min(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2i64 m_V2i64_max(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2i64 m_V2i64_clamp(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max));
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_min(m_V2i64_max(v, min), max);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_wrap(m_V2i64 v, m_V2i64 min, m_V2i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max), int_wrap);
    return m_simd_to2$(m_V2i64, result);
#endif /* arch_simd_supported */
    return m_V2i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from2(lhs), m_simd_from2(rhs));
    return l$((i64)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static i64 m_V2i64_cross(m_V2i64 lhs, m_V2i64 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((i64){ lhs.x, lhs.y, 0, 0 });
    let r = simd_V_from$((i64){ rhs.x, rhs.y, 0, 0 });
    let result = simd_V_cross3(l, r);
    return m_simd_to3$(m_V3i64, result).z;
#endif /* arch_simd_supported */
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static i64 m_V2i64_det(m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_cross(lhs, rhs);
}
$attr($inline_always)
$static i64 m_V2i64_orient(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V2i64_det(m_V2i64_sub(lhs, origin), m_V2i64_sub(rhs, origin));
}
$attr($inline_always)
$static m_V_Orient m_V2i64_orientDir(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    let det = m_V2i64_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V2i64_isCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCW(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2i64_isCollinear(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCollinear(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2i64_isCCW(m_V2i64 origin, m_V2i64 lhs, m_V2i64 rhs) {
    return m_V_Orient_isCCW(m_V2i64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static m_V2i64 m_V2i64_cw(m_V2i64 v) {
    return m_V2i64_of(v.y, -v.x);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_ccw(m_V2i64 v) {
    return m_V2i64_of(-v.y, v.x);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_skew(m_V2i64 v) {
    return m_V2i64_ccw(v);
}
$attr($inline_always)
$static m_V2i64 m_V2i64_perp(m_V2i64 v) {
    return m_V2i64_ccw(v);
}

/* Vec3l functions */
/* Construction */
$attr($inline_always)
$static m_V3i64 m_V3i64_of(i64 x, i64 y, i64 z) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y, z, 0 });
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return (m_V3i64){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3i64 m_V3i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat3$(i64, scalar);
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from3(lhs), m_simd_from3(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3i64 m_V3i64_neg(m_V3i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from3(v));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_add(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_sub(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_mul(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_div(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(lhs), m_simd_from3Divisor(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_rem(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from3(lhs), m_simd_from3Divisor(rhs), int_rem);
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3i64 m_V3i64_scal(m_V3i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(v), m_simd_splat3$(i64, scalar));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_scalInv(m_V3i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(v), m_simd_splat3$(i64, scalar));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3i64 m_V3i64_addS(m_V3i64 v, i64 scalar) {
    return m_V3i64_add(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_subS(m_V3i64 v, i64 scalar) {
    return m_V3i64_sub(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_mulS(m_V3i64 v, i64 scalar) {
    return m_V3i64_mul(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_divS(m_V3i64 v, i64 scalar) {
    return m_V3i64_div(v, m_V3i64_splat(scalar));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_remS(m_V3i64 v, i64 scalar) {
    return m_V3i64_rem(v, m_V3i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3i64 m_V3i64_min(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i64 m_V3i64_max(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3i64 m_V3i64_clamp(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(min), m_simd_from3(max));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_min(m_V3i64_max(v, min), max);
}
$attr($inline_always)
$static m_V3i64 m_V3i64_wrap(m_V3i64 v, m_V3i64 min, m_V3i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from3(v), m_simd_from3(min), m_simd_from3Divisor(max), int_wrap);
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from3(lhs), m_simd_from3(rhs));
    return l$((i64)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3i64 m_V3i64_cross(m_V3i64 lhs, m_V3i64 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return m_V3i64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static i64 m_V3i64_det(m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V3i64_dot(lhs, m_V3i64_cross(mid, rhs));
}
$attr($inline_always)
$static i64 m_V3i64_orient(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V3i64_det(
        m_V3i64_sub(lhs, origin),
        m_V3i64_sub(mid, origin),
        m_V3i64_sub(rhs, origin)
    );
}
$attr($inline_always)
$static m_V_Orient m_V3i64_orientDir(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    let det = m_V3i64_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V3i64_isOrientNeg(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isNeg(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3i64_isCoplanar(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isCoplanar(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3i64_isOrientPos(m_V3i64 origin, m_V3i64 lhs, m_V3i64 mid, m_V3i64 rhs) {
    return m_V_Orient_isPos(m_V3i64_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static m_V3i64 m_V3i64_perp(m_V3i64 v) {
#if arch_simd_supported
    let source = m_simd_from3(v);
    if (v.x != 0 || v.y != 0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((i64){ -1, 1, 0, 0 })
        );
        return m_simd_to3$(m_V3i64, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((i64){ 0, -1, 1, 0 })
    );
    return m_simd_to3$(m_V3i64, result);
#endif /* arch_simd_supported */
    return v.x != 0 || v.y != 0
             ? m_V3i64_of(-v.y, v.x, 0)
             : m_V3i64_of(0, -v.z, v.y);
}

/* Vec4l functions */
/* Construction */
$attr($inline_always)
$static m_V4i64 m_V4i64_of(i64 x, i64 y, i64 z, i64 w) {
#if arch_simd_supported
    let result = simd_V_from$((i64){ x, y, z, w });
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return (m_V4i64){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4i64 m_V4i64_splat(i64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat4$(i64, scalar);
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from4(lhs), m_simd_from4(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4i64 m_V4i64_neg(m_V4i64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from4(v));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_add(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_sub(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_mul(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_div(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_rem(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from4(lhs), m_simd_from4(rhs), int_rem);
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4i64 m_V4i64_scal(m_V4i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(v), m_simd_splat4$(i64, scalar));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_scalInv(m_V4i64 v, i64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(v), m_simd_splat4$(i64, scalar));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4i64 m_V4i64_addS(m_V4i64 v, i64 scalar) {
    return m_V4i64_add(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_subS(m_V4i64 v, i64 scalar) {
    return m_V4i64_sub(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_mulS(m_V4i64 v, i64 scalar) {
    return m_V4i64_mul(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_divS(m_V4i64 v, i64 scalar) {
    return m_V4i64_div(v, m_V4i64_splat(scalar));
}
$attr($inline_always)
$static m_V4i64 m_V4i64_remS(m_V4i64 v, i64 scalar) {
    return m_V4i64_rem(v, m_V4i64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4i64 m_V4i64_min(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i64 m_V4i64_max(m_V4i64 lhs, m_V4i64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4i64 m_V4i64_clamp(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max));
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_min(m_V4i64_max(v, min), max);
}
$attr($inline_always)
$static m_V4i64 m_V4i64_wrap(m_V4i64 v, m_V4i64 min, m_V4i64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max), int_wrap);
    return m_simd_to4$(m_V4i64, result);
#endif /* arch_simd_supported */
    return m_V4i64_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from4(lhs), m_simd_from4(rhs));
    return l$((i64)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2z functions */
/* Construction */
$attr($inline_always)
$static m_V2isize m_V2isize_of(isize x, isize y) {
    return (m_V2isize){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2isize m_V2isize_splat(isize scalar) {
    return m_V2isize_of(scalar, scalar);
}
$attr($inline_always)
$static m_V2isize m_V2isize_from3(m_V3isize v) {
    return m_V2isize_of(v.x, v.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_from4(m_V4isize v) {
    return m_V2isize_of(v.x, v.y);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2isize m_V2isize_neg(m_V2isize v) {
    return m_V2isize_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_add(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_sub(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_mul(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_div(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2isize m_V2isize_rem(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y)
    );
}

$attr($inline_always)
$static m_V2isize m_V2isize_scal(m_V2isize v, isize scalar) {
    return m_V2isize_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2isize m_V2isize_scalInv(m_V2isize v, isize scalar) {
    return m_V2isize_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2isize m_V2isize_addS(m_V2isize v, isize scalar) {
    return m_V2isize_add(v, m_V2isize_splat(scalar));
}
$attr($inline_always)
$static m_V2isize m_V2isize_subS(m_V2isize v, isize scalar) {
    return m_V2isize_sub(v, m_V2isize_splat(scalar));
}
$attr($inline_always)
$static m_V2isize m_V2isize_mulS(m_V2isize v, isize scalar) {
    return m_V2isize_mul(v, m_V2isize_splat(scalar));
}
$attr($inline_always)
$static m_V2isize m_V2isize_divS(m_V2isize v, isize scalar) {
    return m_V2isize_div(v, m_V2isize_splat(scalar));
}
$attr($inline_always)
$static m_V2isize m_V2isize_remS(m_V2isize v, isize scalar) {
    return m_V2isize_rem(v, m_V2isize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V2isize m_V2isize_min(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2isize m_V2isize_max(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y)
    );
}
$attr($inline_always)
$static m_V2isize m_V2isize_clamp(m_V2isize v, m_V2isize min, m_V2isize max) {
    return m_V2isize_min(m_V2isize_max(v, min), max);
}
$attr($inline_always)
$static m_V2isize m_V2isize_wrap(m_V2isize v, m_V2isize min, m_V2isize max) {
    return m_V2isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static isize m_V2isize_lenSq(m_V2isize v) {
    return m_V2isize_dot(v, v);
}
$attr($inline_always)
$static isize m_V2isize_distSq(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_lenSq(m_V2isize_sub(lhs, rhs));
}
$attr($inline_always)
$static isize m_V2isize_dot(m_V2isize lhs, m_V2isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static isize m_V2isize_cross(m_V2isize lhs, m_V2isize rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}
$attr($inline_always)
$static isize m_V2isize_det(m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_cross(lhs, rhs);
}
$attr($inline_always)
$static isize m_V2isize_orient(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V2isize_det(m_V2isize_sub(lhs, origin), m_V2isize_sub(rhs, origin));
}
$attr($inline_always)
$static m_V_Orient m_V2isize_orientDir(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    let det = m_V2isize_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V2isize_isCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCW(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2isize_isCollinear(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCollinear(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2isize_isCCW(m_V2isize origin, m_V2isize lhs, m_V2isize rhs) {
    return m_V_Orient_isCCW(m_V2isize_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static m_V2isize m_V2isize_cw(m_V2isize v) {
    return m_V2isize_of(v.y, -v.x);
}
$attr($inline_always)
$static m_V2isize m_V2isize_ccw(m_V2isize v) {
    return m_V2isize_of(-v.y, v.x);
}
$attr($inline_always)
$static m_V2isize m_V2isize_skew(m_V2isize v) {
    return m_V2isize_ccw(v);
}
$attr($inline_always)
$static m_V2isize m_V2isize_perp(m_V2isize v) {
    return m_V2isize_of(-v.y, v.x);
}

/* Vec3z functions */
/* Construction */
$attr($inline_always)
$static m_V3isize m_V3isize_of(isize x, isize y, isize z) {
    return (m_V3isize){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3isize m_V3isize_splat(isize scalar) {
    return m_V3isize_of(scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V3isize m_V3isize_from2(m_V2isize v) {
    return m_V3isize_of(v.x, v.y, 0);
}
$attr($inline_always)
$static m_V3isize m_V3isize_from4(m_V4isize v) {
    return m_V3isize_of(v.x, v.y, v.z);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3isize m_V3isize_neg(m_V3isize v) {
    return m_V3isize_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3isize m_V3isize_add(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3isize m_V3isize_sub(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3isize m_V3isize_mul(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3isize m_V3isize_div(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3isize m_V3isize_rem(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3isize m_V3isize_scal(m_V3isize v, isize scalar) {
    return m_V3isize_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3isize m_V3isize_scalInv(m_V3isize v, isize scalar) {
    return m_V3isize_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3isize m_V3isize_addS(m_V3isize v, isize scalar) {
    return m_V3isize_add(v, m_V3isize_splat(scalar));
}
$attr($inline_always)
$static m_V3isize m_V3isize_subS(m_V3isize v, isize scalar) {
    return m_V3isize_sub(v, m_V3isize_splat(scalar));
}
$attr($inline_always)
$static m_V3isize m_V3isize_mulS(m_V3isize v, isize scalar) {
    return m_V3isize_mul(v, m_V3isize_splat(scalar));
}
$attr($inline_always)
$static m_V3isize m_V3isize_divS(m_V3isize v, isize scalar) {
    return m_V3isize_div(v, m_V3isize_splat(scalar));
}
$attr($inline_always)
$static m_V3isize m_V3isize_remS(m_V3isize v, isize scalar) {
    return m_V3isize_rem(v, m_V3isize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3isize m_V3isize_min(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3isize m_V3isize_max(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3isize m_V3isize_clamp(m_V3isize v, m_V3isize min, m_V3isize max) {
    return m_V3isize_min(m_V3isize_max(v, min), max);
}
$attr($inline_always)
$static m_V3isize m_V3isize_wrap(m_V3isize v, m_V3isize min, m_V3isize max) {
    return m_V3isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static isize m_V3isize_lenSq(m_V3isize v) {
    return m_V3isize_dot(v, v);
}
$attr($inline_always)
$static isize m_V3isize_distSq(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_lenSq(m_V3isize_sub(lhs, rhs));
}
$attr($inline_always)
$static isize m_V3isize_dot(m_V3isize lhs, m_V3isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3isize m_V3isize_cross(m_V3isize lhs, m_V3isize rhs) {
    return m_V3isize_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static isize m_V3isize_det(m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V3isize_dot(lhs, m_V3isize_cross(mid, rhs));
}
$attr($inline_always)
$static isize m_V3isize_orient(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V3isize_det(
        m_V3isize_sub(lhs, origin),
        m_V3isize_sub(mid, origin),
        m_V3isize_sub(rhs, origin)
    );
}
$attr($inline_always)
$static m_V_Orient m_V3isize_orientDir(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    let det = m_V3isize_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V3isize_isOrientNeg(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isNeg(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3isize_isCoplanar(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isCoplanar(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3isize_isOrientPos(m_V3isize origin, m_V3isize lhs, m_V3isize mid, m_V3isize rhs) {
    return m_V_Orient_isPos(m_V3isize_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static m_V3isize m_V3isize_perp(m_V3isize v) {
    return v.x != 0 || v.y != 0
             ? m_V3isize_of(-v.y, v.x, 0)
             : m_V3isize_of(0, -v.z, v.y);
}

/* Vec4z functions */
/* Construction */
$attr($inline_always)
$static m_V4isize m_V4isize_of(isize x, isize y, isize z, isize w) {
    return (m_V4isize){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4isize m_V4isize_splat(isize scalar) {
    return m_V4isize_of(scalar, scalar, scalar, scalar);
}
$attr($inline_always)
$static m_V4isize m_V4isize_from2(m_V2isize v) {
    return m_V4isize_of(v.x, v.y, 0, 0);
}
$attr($inline_always)
$static m_V4isize m_V4isize_from3(m_V3isize v) {
    return m_V4isize_of(v.x, v.y, v.z, 0);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V4isize)(lhs, rhs)) {
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4isize m_V4isize_neg(m_V4isize v) {
    return m_V4isize_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4isize m_V4isize_add(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4isize m_V4isize_sub(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4isize m_V4isize_mul(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4isize m_V4isize_div(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4isize m_V4isize_rem(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        int_rem(lhs.x, rhs.x),
        int_rem(lhs.y, rhs.y),
        int_rem(lhs.z, rhs.z),
        int_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4isize m_V4isize_scal(m_V4isize v, isize scalar) {
    return m_V4isize_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4isize m_V4isize_scalInv(m_V4isize v, isize scalar) {
    return m_V4isize_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4isize m_V4isize_addS(m_V4isize v, isize scalar) {
    return m_V4isize_add(v, m_V4isize_splat(scalar));
}
$attr($inline_always)
$static m_V4isize m_V4isize_subS(m_V4isize v, isize scalar) {
    return m_V4isize_sub(v, m_V4isize_splat(scalar));
}
$attr($inline_always)
$static m_V4isize m_V4isize_mulS(m_V4isize v, isize scalar) {
    return m_V4isize_mul(v, m_V4isize_splat(scalar));
}
$attr($inline_always)
$static m_V4isize m_V4isize_divS(m_V4isize v, isize scalar) {
    return m_V4isize_div(v, m_V4isize_splat(scalar));
}
$attr($inline_always)
$static m_V4isize m_V4isize_remS(m_V4isize v, isize scalar) {
    return m_V4isize_rem(v, m_V4isize_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4isize m_V4isize_min(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4isize m_V4isize_max(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4isize m_V4isize_clamp(m_V4isize v, m_V4isize min, m_V4isize max) {
    return m_V4isize_min(m_V4isize_max(v, min), max);
}
$attr($inline_always)
$static m_V4isize m_V4isize_wrap(m_V4isize v, m_V4isize min, m_V4isize max) {
    return m_V4isize_of(
        int_wrap(v.x, min.x, max.x),
        int_wrap(v.y, min.y, max.y),
        int_wrap(v.z, min.z, max.z),
        int_wrap(v.w, min.w, max.w)
    );
}

/* Geometric Operations */
$attr($inline_always)
$static isize m_V4isize_lenSq(m_V4isize v) {
    return m_V4isize_dot(v, v);
}
$attr($inline_always)
$static isize m_V4isize_distSq(m_V4isize lhs, m_V4isize rhs) {
    return m_V4isize_lenSq(m_V4isize_sub(lhs, rhs));
}
$attr($inline_always)
$static isize m_V4isize_dot(m_V4isize lhs, m_V4isize rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/* Vec2f functions */
/* Construction */
$attr($inline_always)
$static m_V2f32 m_V2f32_of(f32 x, f32 y) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y });
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of_static(x, y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_splat(f32 scalar) {
#if arch_simd_supported
    let result = m_simd_splat2$(f32, scalar);
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of_static(scalar, scalar);
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
#if arch_simd_supported
    let result = simd_V_from$((f32){ flt_cos(radians), flt_sin(radians) });
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(flt_cos(radians), flt_sin(radians));
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_simd_from2(lhs), m_simd_from2(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V2f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from2(lhs), m_simd_from2(rhs), m_simd_from2(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V2f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from2(lhs), m_simd_from2(rhs), m_simd_from2(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApx_default$((m_V2f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr($inline_always)
$static m_V2f32 m_V2f32_neg(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from2(v));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_inv(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from2(v));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(1.0f / v.x, 1.0f / v.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_add(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_sub(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_mul(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_div(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_rem(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_flt_rem(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_scal(m_V2f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(v), m_simd_splat2$(f32, scalar));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_scalInv(m_V2f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(v), m_simd_splat2$(f32, scalar));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(v.x / scalar, v.y / scalar);
}

$attr($inline_always)
$static m_V2f32 m_V2f32_addS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_add(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_subS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_sub(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_mulS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_mul(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_divS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_div(lhs, m_V2f32_splat(rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_remS(m_V2f32 lhs, f32 rhs) {
    return m_V2f32_rem(lhs, m_V2f32_splat(rhs));
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
$static m_V2f32* m_V2f32_remAsg(m_V2f32* lhs, m_V2f32 rhs) {
    return *lhs = m_V2f32_rem(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_scalAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scal(*v, scalar), v;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_scalInvAsg(m_V2f32* v, f32 scalar) {
    return *v = m_V2f32_scalInv(*v, scalar), v;
}

$attr($inline_always)
$static m_V2f32* m_V2f32_addSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_addS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_subSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_subS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_mulSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_mulS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_divSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_divS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f32* m_V2f32_remSAsg(m_V2f32* lhs, f32 rhs) {
    return *lhs = m_V2f32_remS(*lhs, rhs), lhs;
}

/* Range Operations */
$attr($inline_always)
$static m_V2f32 m_V2f32_min(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(flt_min(lhs.x, rhs.x), flt_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_max(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(flt_max(lhs.x, rhs.x), flt_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_min(m_V2f32_max(v, min), max);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_clamp01(m_V2f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(m_V2f32_zero), m_simd_from2(m_V2f32_one));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_clamp(v, m_V2f32_zero, m_V2f32_one);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap(m_V2f32 v, m_V2f32 min, m_V2f32 max) {
#if arch_simd_supported
    let result = simd_V_flt_wrap(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max));
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    return m_V2f32_of(flt_wrap(v.x, min.x, max.x), flt_wrap(v.y, min.y, max.y));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_wrap01(m_V2f32 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from2(lhs), m_simd_from2(rhs));
    return l$((f32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static f32 m_V2f32_cross(m_V2f32 lhs, m_V2f32 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((f32){ lhs.x, lhs.y, 0.0f, 0 });
    let r = simd_V_from$((f32){ rhs.x, rhs.y, 0.0f, 0 });
    let result = simd_V_cross3(l, r);
    return m_simd_to3$(m_V3f32, result).z;
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    const m_V2f32 rotation = m_V2f32_sincos(radians);
    let source = m_simd_from2(v);
    let swapped = simd_V_shuffle(source, source, 1, 0);
    let result = simd_V_add(
        simd_V_mul(source, m_simd_splat2$(f32, rotation.x)),
        simd_V_mul(swapped, simd_V_from$((f32){ -rotation.y, rotation.y }))
    );
    return m_simd_to2$(m_V2f32, result);
#endif /* arch_simd_supported */
    const m_V2f32 r = m_V2f32_sincos(radians);
    return m_V2f32_of(v.x * r.x - v.y * r.y, v.x * r.y + v.y * r.x);
}
$attr($inline_always)
$static f32 m_V2f32_det(m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_cross(lhs, rhs);
}
$attr($inline_always)
$static f32 m_V2f32_orient(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V2f32_det(m_V2f32_sub(lhs, origin), m_V2f32_sub(rhs, origin));
}
$attr($inline_always)
$static m_V_Orient m_V2f32_orientDir(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    let det = m_V2f32_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V2f32_isCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCW(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2f32_isCollinear(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCollinear(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2f32_isCCW(m_V2f32 origin, m_V2f32 lhs, m_V2f32 rhs) {
    return m_V_Orient_isCCW(m_V2f32_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static m_V2f32 m_V2f32_cw(m_V2f32 v) {
    return m_V2f32_of(v.y, -v.x);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_ccw(m_V2f32 v) {
    return m_V2f32_of(-v.y, v.x);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_skew(m_V2f32 v) {
    return m_V2f32_ccw(v);
}
$attr($inline_always)
$static m_V2f32 m_V2f32_perp(m_V2f32 v) {
    return m_V2f32_ccw(v);
}

/* Vec3f functions */
/* Construction */
$attr($inline_always)
$static m_V3f32 m_V3f32_of(f32 x, f32 y, f32 z) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y, z, 0 });
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    let result = m_simd_splat3$(f32, scalar);
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(scalar, scalar, scalar);
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V3f32)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_simd_from3(lhs), m_simd_from3(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V3f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from3(lhs), m_simd_from3(rhs), m_simd_from3(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V3f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from3(lhs), m_simd_from3(rhs), m_simd_from3(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApx_default$((m_V3f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr($inline_always)
$static m_V3f32 m_V3f32_neg(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from3(v));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_inv(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from3Divisor(v));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_add(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_sub(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_mul(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_div(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(lhs), m_simd_from3Divisor(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_rem(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from3(lhs), m_simd_from3Divisor(rhs), flt_rem);
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y), flt_rem(lhs.z, rhs.z));
}

$attr($inline_always)
$static m_V3f32 m_V3f32_scal(m_V3f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(v), m_simd_splat3$(f32, scalar));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_scalInv(m_V3f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(v), m_simd_splat3$(f32, scalar));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3f32 m_V3f32_addS(m_V3f32 v, f32 scalar) {
    return m_V3f32_add(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_subS(m_V3f32 v, f32 scalar) {
    return m_V3f32_sub(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_mulS(m_V3f32 v, f32 scalar) {
    return m_V3f32_mul(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_divS(m_V3f32 v, f32 scalar) {
    return m_V3f32_div(v, m_V3f32_splat(scalar));
}
$attr($inline_always)
$static m_V3f32 m_V3f32_remS(m_V3f32 v, f32 scalar) {
    return m_V3f32_rem(v, m_V3f32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3f32 m_V3f32_min(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_max(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(min), m_simd_from3(max));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_min(m_V3f32_max(v, min), max);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_clamp01(m_V3f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(m_V3f32_zero), m_simd_from3(m_V3f32_one));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_clamp(v, m_V3f32_zero, m_V3f32_one);
}
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap(m_V3f32 v, m_V3f32 min, m_V3f32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from3(v), m_simd_from3(min), m_simd_from3Divisor(max), flt_wrap);
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z)
    );
}
$attr($inline_always)
$static m_V3f32 m_V3f32_wrap01(m_V3f32 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from3(lhs), m_simd_from3(rhs));
    return l$((f32)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3f32 m_V3f32_cross(m_V3f32 lhs, m_V3f32 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return m_V3f32_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static f32 m_V3f32_det(m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V3f32_dot(lhs, m_V3f32_cross(mid, rhs));
}
$attr($inline_always)
$static f32 m_V3f32_orient(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V3f32_det(
        m_V3f32_sub(lhs, origin),
        m_V3f32_sub(mid, origin),
        m_V3f32_sub(rhs, origin)
    );
}
$attr($inline_always)
$static m_V_Orient m_V3f32_orientDir(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    let det = m_V3f32_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V3f32_isOrientNeg(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isNeg(m_V3f32_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3f32_isCoplanar(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isCoplanar(m_V3f32_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3f32_isOrientPos(m_V3f32 origin, m_V3f32 lhs, m_V3f32 mid, m_V3f32 rhs) {
    return m_V_Orient_isPos(m_V3f32_orientDir(origin, lhs, mid, rhs));
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
#if arch_simd_supported
    let source = m_simd_from3(v);
    if (v.x != 0.0f || v.y != 0.0f) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((f32){ -1.0f, 1.0f, 0.0f, 0.0f })
        );
        return m_simd_to3$(m_V3f32, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((f32){ 0.0f, -1.0f, 1.0f, 0.0f })
    );
    return m_simd_to3$(m_V3f32, result);
#endif /* arch_simd_supported */
    return v.x != 0.0f || v.y != 0.0f
             ? m_V3f32_of(-v.y, v.x, 0.0f)
             : m_V3f32_of(0.0f, -v.z, v.y);
}

/* Vec4f functions */
/* Construction */
$attr($inline_always)
$static m_V4f32 m_V4f32_of(f32 x, f32 y, f32 z, f32 w) {
#if arch_simd_supported
    let result = simd_V_from$((f32){ x, y, z, w });
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return (m_V4f32){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4f32 m_V4f32_splat(f32 scalar) {
#if arch_simd_supported
    let result = m_simd_splat4$(f32, scalar);
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from4(lhs), m_simd_from4(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V4f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from4(lhs), m_simd_from4(rhs), m_simd_from4(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V4f32)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from4(lhs), m_simd_from4(rhs), m_simd_from4(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApx_default$((m_V4f32)(lhs, rhs, threshold, mode));

/* Arithmetic */
$attr($inline_always)
$static m_V4f32 m_V4f32_neg(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from4(v));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_inv(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from4(v));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_add(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_sub(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_mul(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_div(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_rem(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from4(lhs), m_simd_from4(rhs), flt_rem);
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z),
        flt_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4f32 m_V4f32_scal(m_V4f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(v), m_simd_splat4$(f32, scalar));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_scalInv(m_V4f32 v, f32 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(v), m_simd_splat4$(f32, scalar));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4f32 m_V4f32_addS(m_V4f32 v, f32 scalar) {
    return m_V4f32_add(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_subS(m_V4f32 v, f32 scalar) {
    return m_V4f32_sub(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_mulS(m_V4f32 v, f32 scalar) {
    return m_V4f32_mul(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_divS(m_V4f32 v, f32 scalar) {
    return m_V4f32_div(v, m_V4f32_splat(scalar));
}
$attr($inline_always)
$static m_V4f32 m_V4f32_remS(m_V4f32 v, f32 scalar) {
    return m_V4f32_rem(v, m_V4f32_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4f32 m_V4f32_min(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_max(m_V4f32 lhs, m_V4f32 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_min(m_V4f32_max(v, min), max);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_clamp01(m_V4f32 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(m_V4f32_zero), m_simd_from4(m_V4f32_one));
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_clamp(v, m_V4f32_zero, m_V4f32_one);
}
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap(m_V4f32 v, m_V4f32 min, m_V4f32 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max), flt_wrap);
    return m_simd_to4$(m_V4f32, result);
#endif /* arch_simd_supported */
    return m_V4f32_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z),
        flt_wrap(v.w, min.w, max.w)
    );
}
$attr($inline_always)
$static m_V4f32 m_V4f32_wrap01(m_V4f32 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from4(lhs), m_simd_from4(rhs));
    return result;
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y });
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return (m_V2f64){ .x = x, .y = y };
}
$attr($inline_always)
$static m_V2f64 m_V2f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat2$(f64, scalar);
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    let result = simd_V_from$((f64){ flt_cos(radians), flt_sin(radians) });
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(flt_cos(radians), flt_sin(radians));
}

/* Comparison */
$attr($inline_always)
$static cmp_fn_ord$((m_V2f64)(lhs, rhs)) {
#if arch_simd_supported
    return simd_V_ord(m_simd_from2(lhs), m_simd_from2(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V2f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from2(lhs), m_simd_from2(rhs), m_simd_from2(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V2f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from2(lhs), m_simd_from2(rhs), m_simd_from2(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V2f64 m_V2f64_neg(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from2(v));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(-v.x, -v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_inv(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from2(v));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(1.0 / v.x, 1.0 / v.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_add(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(lhs.x + rhs.x, lhs.y + rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_sub(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(lhs.x - rhs.x, lhs.y - rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_mul(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(lhs.x * rhs.x, lhs.y * rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_div(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(lhs.x / rhs.x, lhs.y / rhs.y);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_rem(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from2(lhs), m_simd_from2(rhs), flt_rem);
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(flt_rem(lhs.x, rhs.x), flt_rem(lhs.y, rhs.y));
}

$attr($inline_always)
$static m_V2f64 m_V2f64_scal(m_V2f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from2(v), m_simd_splat2$(f64, scalar));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(v.x * scalar, v.y * scalar);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_scalInv(m_V2f64 v, f64 scaler) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from2(v), m_simd_splat2$(f64, scaler));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(v.x / scaler, v.y / scaler);
}

$attr($inline_always)
$static m_V2f64 m_V2f64_addS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_add(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_subS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_sub(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_mulS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_mul(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_divS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_div(lhs, m_V2f64_splat(rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_remS(m_V2f64 lhs, f64 rhs) {
    return m_V2f64_rem(lhs, m_V2f64_splat(rhs));
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
$static m_V2f64* m_V2f64_remAsg(m_V2f64* lhs, m_V2f64 rhs) {
    return *lhs = m_V2f64_rem(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_scalAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scal(*v, scalar), v;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_scalInvAsg(m_V2f64* v, f64 scalar) {
    return *v = m_V2f64_scalInv(*v, scalar), v;
}

$attr($inline_always)
$static m_V2f64* m_V2f64_addSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_addS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_subSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_subS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_mulSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_mulS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_divSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_divS(*lhs, rhs), lhs;
}
$attr($inline_always)
$static m_V2f64* m_V2f64_remSAsg(m_V2f64* lhs, f64 rhs) {
    return *lhs = m_V2f64_remS(*lhs, rhs), lhs;
}

/* Range Operations */
$attr($inline_always)
$static m_V2f64 m_V2f64_min(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(pri_min(lhs.x, rhs.x), pri_min(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_max(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from2(lhs), m_simd_from2(rhs));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(pri_max(lhs.x, rhs.x), pri_max(lhs.y, rhs.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_min(m_V2f64_max(v, min), max);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_clamp01(m_V2f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from2(v), m_simd_from2(m_V2f64_zero), m_simd_from2(m_V2f64_one));
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_clamp(v, m_V2f64_zero, m_V2f64_one);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap(m_V2f64 v, m_V2f64 min, m_V2f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from2(v), m_simd_from2(min), m_simd_from2(max), flt_wrap);
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    return m_V2f64_of(flt_wrap(v.x, min.x, max.x), flt_wrap(v.y, min.y, max.y));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_wrap01(m_V2f64 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from2(lhs), m_simd_from2(rhs));
    return l$((f64)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
$attr($inline_always)
$static f64 m_V2f64_cross(m_V2f64 lhs, m_V2f64 rhs) {
#if arch_simd_supported
    let l = simd_V_from$((f64){ lhs.x, lhs.y, 0.0, 0 });
    let r = simd_V_from$((f64){ rhs.x, rhs.y, 0.0, 0 });
    let result = simd_V_cross3(l, r);
    return m_simd_to3$(m_V3f64, result).z;
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    const m_V2f64 rotation = m_V2f64_sincos(radians);
    let source = m_simd_from2(v);
    let swapped = simd_V_shuffle(source, source, 1, 0);
    let result = simd_V_add(
        simd_V_mul(source, m_simd_splat2$(f64, rotation.x)),
        simd_V_mul(swapped, simd_V_from$((f64){ -rotation.y, rotation.y }))
    );
    return m_simd_to2$(m_V2f64, result);
#endif /* arch_simd_supported */
    const m_V2f64 r = m_V2f64_sincos(radians);
    return m_V2f64_of(
        v.x * r.x - v.y * r.y,
        v.x * r.y + v.y * r.x
    );
}
$attr($inline_always)
$static f64 m_V2f64_det(m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_cross(lhs, rhs);
}
$attr($inline_always)
$static f64 m_V2f64_orient(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V2f64_det(m_V2f64_sub(lhs, origin), m_V2f64_sub(rhs, origin));
}
$attr($inline_always)
$static m_V_Orient m_V2f64_orientDir(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    let det = m_V2f64_orient(origin, lhs, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V2f64_isCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCW(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2f64_isCollinear(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCollinear(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static bool m_V2f64_isCCW(m_V2f64 origin, m_V2f64 lhs, m_V2f64 rhs) {
    return m_V_Orient_isCCW(m_V2f64_orientDir(origin, lhs, rhs));
}
$attr($inline_always)
$static m_V2f64 m_V2f64_cw(m_V2f64 v) {
    return m_V2f64_of(v.y, -v.x);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_ccw(m_V2f64 v) {
    return m_V2f64_of(-v.y, v.x);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_skew(m_V2f64 v) {
    return m_V2f64_ccw(v);
}
$attr($inline_always)
$static m_V2f64 m_V2f64_perp(m_V2f64 v) {
    return m_V2f64_ccw(v);
}

/* Vec3d functions */
/* Construction */
$attr($inline_always)
$static m_V3f64 m_V3f64_of(f64 x, f64 y, f64 z) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y, z, 0 });
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return (m_V3f64){ .x = x, .y = y, .z = z };
}
$attr($inline_always)
$static m_V3f64 m_V3f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat3$(f64, scalar);
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from3(lhs), m_simd_from3(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V3f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from3(lhs), m_simd_from3(rhs), m_simd_from3(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V3f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from3(lhs), m_simd_from3(rhs), m_simd_from3(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V3f64 m_V3f64_neg(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from3(v));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(-v.x, -v.y, -v.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_inv(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from3Divisor(v));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_add(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_sub(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_mul(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_div(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(lhs), m_simd_from3Divisor(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_rem(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from3(lhs), m_simd_from3Divisor(rhs), flt_rem);
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z)
    );
}

$attr($inline_always)
$static m_V3f64 m_V3f64_scal(m_V3f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from3(v), m_simd_splat3$(f64, scalar));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(v.x * scalar, v.y * scalar, v.z * scalar);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_scalInv(m_V3f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from3(v), m_simd_splat3$(f64, scalar));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(v.x / scalar, v.y / scalar, v.z / scalar);
}

$attr($inline_always)
$static m_V3f64 m_V3f64_addS(m_V3f64 v, f64 scalar) {
    return m_V3f64_add(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_subS(m_V3f64 v, f64 scalar) {
    return m_V3f64_sub(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_mulS(m_V3f64 v, f64 scalar) {
    return m_V3f64_mul(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_divS(m_V3f64 v, f64 scalar) {
    return m_V3f64_div(v, m_V3f64_splat(scalar));
}
$attr($inline_always)
$static m_V3f64 m_V3f64_remS(m_V3f64 v, f64 scalar) {
    return m_V3f64_rem(v, m_V3f64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V3f64 m_V3f64_min(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_max(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(min), m_simd_from3(max));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_min(m_V3f64_max(v, min), max);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_clamp01(m_V3f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from3(v), m_simd_from3(m_V3f64_zero), m_simd_from3(m_V3f64_one));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_clamp(v, m_V3f64_zero, m_V3f64_one);
}
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap(m_V3f64 v, m_V3f64 min, m_V3f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from3(v), m_simd_from3(min), m_simd_from3Divisor(max), flt_wrap);
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z)
    );
}
$attr($inline_always)
$static m_V3f64 m_V3f64_wrap01(m_V3f64 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from3(lhs), m_simd_from3(rhs));
    return l$((f64)(result));
#endif /* arch_simd_supported */
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
$attr($inline_always)
$static m_V3f64 m_V3f64_cross(m_V3f64 lhs, m_V3f64 rhs) {
#if arch_simd_supported
    let result = simd_V_cross3(m_simd_from3(lhs), m_simd_from3(rhs));
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return m_V3f64_of(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}
$attr($inline_always)
$static f64 m_V3f64_det(m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V3f64_dot(lhs, m_V3f64_cross(mid, rhs));
}
$attr($inline_always)
$static f64 m_V3f64_orient(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V3f64_det(
        m_V3f64_sub(lhs, origin),
        m_V3f64_sub(mid, origin),
        m_V3f64_sub(rhs, origin)
    );
}
$attr($inline_always)
$static m_V_Orient m_V3f64_orientDir(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    let det = m_V3f64_orient(origin, lhs, mid, rhs);
    return det < 0 ? m_V_Orient_neg : det > 0 ? m_V_Orient_pos
                                              : m_V_Orient_zero;
}
$attr($inline_always)
$static bool m_V3f64_isOrientNeg(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isNeg(m_V3f64_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3f64_isCoplanar(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isCoplanar(m_V3f64_orientDir(origin, lhs, mid, rhs));
}
$attr($inline_always)
$static bool m_V3f64_isOrientPos(m_V3f64 origin, m_V3f64 lhs, m_V3f64 mid, m_V3f64 rhs) {
    return m_V_Orient_isPos(m_V3f64_orientDir(origin, lhs, mid, rhs));
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
#if arch_simd_supported
    let source = m_simd_from3(v);
    if (v.x != 0.0 || v.y != 0.0) {
        let result = simd_V_mul(
            simd_V_shuffle(source, source, 1, 0, 3, 3),
            simd_V_from$((f64){ -1.0, 1.0, 0.0, 0.0 })
        );
        return m_simd_to3$(m_V3f64, result);
    }
    let result = simd_V_mul(
        simd_V_shuffle(source, source, 3, 2, 1, 3),
        simd_V_from$((f64){ 0.0, -1.0, 1.0, 0.0 })
    );
    return m_simd_to3$(m_V3f64, result);
#endif /* arch_simd_supported */
    return v.x != 0.0 || v.y != 0.0
             ? m_V3f64_of(-v.y, v.x, 0.0)
             : m_V3f64_of(0.0, -v.z, v.y);
}

/* Vec4d functions */
/* Construction */
$attr($inline_always)
$static m_V4f64 m_V4f64_of(f64 x, f64 y, f64 z, f64 w) {
#if arch_simd_supported
    let result = simd_V_from$((f64){ x, y, z, w });
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return (m_V4f64){ .x = x, .y = y, .z = z, .w = w };
}
$attr($inline_always)
$static m_V4f64 m_V4f64_splat(f64 scalar) {
#if arch_simd_supported
    let result = m_simd_splat4$(f64, scalar);
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
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
#if arch_simd_supported
    return simd_V_ord(m_simd_from4(lhs), m_simd_from4(rhs));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)))(l, r)) {
        if (*l < *r) { return cmp_Ord_lt; }
        if (*l > *r) { return cmp_Ord_gt; }
    } $end(for);
    return cmp_Ord_eq;
}

$attr($inline_always)
$static cmp_fn_ordApxAbs$((m_V4f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxAbs(m_simd_from4(lhs), m_simd_from4(rhs), m_simd_from4(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxAbs(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}
$attr($inline_always)
$static cmp_fn_ordApxRel$((m_V4f64)(lhs, rhs, threshold)) {
#if arch_simd_supported
    return simd_V_flt_ordApxRel(m_simd_from4(lhs), m_simd_from4(rhs), m_simd_from4(threshold));
#endif /* arch_simd_supported */
    for_(($s(A_ref(lhs.s)), $s(A_ref(rhs.s)), $s(A_ref(threshold.s)))(l, r, t)) {
        let lane_ord = flt_ordApxRel(*l, *r, *t);
        if (lane_ord != cmp_Ord_eq) { return lane_ord; }
    } $end(for);
    return cmp_Ord_eq;
}

/* Arithmetic */
$attr($inline_always)
$static m_V4f64 m_V4f64_neg(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_neg(m_simd_from4(v));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(-v.x, -v.y, -v.z, -v.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_inv(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_flt_inv(m_simd_from4(v));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_add(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_add(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_sub(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_sub(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_mul(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_div(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_rem(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_map2(m_simd_from4(lhs), m_simd_from4(rhs), flt_rem);
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(
        flt_rem(lhs.x, rhs.x),
        flt_rem(lhs.y, rhs.y),
        flt_rem(lhs.z, rhs.z),
        flt_rem(lhs.w, rhs.w)
    );
}

$attr($inline_always)
$static m_V4f64 m_V4f64_scal(m_V4f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_mul(m_simd_from4(v), m_simd_splat4$(f64, scalar));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_scalInv(m_V4f64 v, f64 scalar) {
#if arch_simd_supported
    let result = simd_V_div(m_simd_from4(v), m_simd_splat4$(f64, scalar));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

$attr($inline_always)
$static m_V4f64 m_V4f64_addS(m_V4f64 v, f64 scalar) {
    return m_V4f64_add(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_subS(m_V4f64 v, f64 scalar) {
    return m_V4f64_sub(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_mulS(m_V4f64 v, f64 scalar) {
    return m_V4f64_mul(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_divS(m_V4f64 v, f64 scalar) {
    return m_V4f64_div(v, m_V4f64_splat(scalar));
}
$attr($inline_always)
$static m_V4f64 m_V4f64_remS(m_V4f64 v, f64 scalar) {
    return m_V4f64_rem(v, m_V4f64_splat(scalar));
}

/* Range Operations */
$attr($inline_always)
$static m_V4f64 m_V4f64_min(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_min(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(
        pri_min(lhs.x, rhs.x),
        pri_min(lhs.y, rhs.y),
        pri_min(lhs.z, rhs.z),
        pri_min(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_max(m_V4f64 lhs, m_V4f64 rhs) {
#if arch_simd_supported
    let result = simd_V_max(m_simd_from4(lhs), m_simd_from4(rhs));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(
        pri_max(lhs.x, rhs.x),
        pri_max(lhs.y, rhs.y),
        pri_max(lhs.z, rhs.z),
        pri_max(lhs.w, rhs.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_min(m_V4f64_max(v, min), max);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_clamp01(m_V4f64 v) {
#if arch_simd_supported
    let result = simd_V_clamp(m_simd_from4(v), m_simd_from4(m_V4f64_zero), m_simd_from4(m_V4f64_one));
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_clamp(v, m_V4f64_zero, m_V4f64_one);
}
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap(m_V4f64 v, m_V4f64 min, m_V4f64 max) {
#if arch_simd_supported
    let result = simd_V_map3(m_simd_from4(v), m_simd_from4(min), m_simd_from4(max), flt_wrap);
    return m_simd_to4$(m_V4f64, result);
#endif /* arch_simd_supported */
    return m_V4f64_of(
        flt_wrap(v.x, min.x, max.x),
        flt_wrap(v.y, min.y, max.y),
        flt_wrap(v.z, min.z, max.z),
        flt_wrap(v.w, min.w, max.w)
    );
}
$attr($inline_always)
$static m_V4f64 m_V4f64_wrap01(m_V4f64 v) {
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
#if arch_simd_supported
    let result = simd_V_dot(m_simd_from4(lhs), m_simd_from4(rhs));
    return l$((f64)(result));
#endif /* arch_simd_supported */
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
#endif /* on_analysis_active_only || on_comptime */

#undef m_simd_from2
#undef m_simd_from3
#undef m_simd_from3Divisor
#undef m_simd_from4
#undef m_simd_splat2$
#undef m_simd_splat3$
#undef m_simd_splat4$
#undef m_simd_to2$
#undef m_simd_to3$
#undef m_simd_to4$

#endif /* pri_vec__included */
