/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2026-07-14 (date of last update)
 * @ingroup dasae-headers(dh)/m-math-linalg/V-Vec
 * @prefix  m_V
 *
 * @brief   Mathematical common vector operations
 * @details Provides a set of vector priematical operations and functions
 */
#pragma once
#ifndef m_math_linalg_V_Vec_base_included
#define m_math_linalg_V_Vec_base_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../m-math/common.h"
#include "../../prl/simd.h"
#include "types.h"

/*========== Macros and Declarations ========================================*/

#define m_V_simd_from2(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y })
#define m_V_simd_from3(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, 0 })
#define m_V_simd_from3Divisor(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, 1 })
#define m_V_simd_from4(_v...) simd_V_from$((TypeOfUnqual((_v).x)){ (_v).x, (_v).y, (_v).z, (_v).w })
#define m_V_simd_splat2$(_T, _x...) simd_V_from$((_T){ (_x), (_x) })
#define m_V_simd_splat3$(_T, _x...) simd_V_from$((_T){ (_x), (_x), (_x), (_x) })
#define m_V_simd_splat4$(_T, _x...) simd_V_from$((_T){ (_x), (_x), (_x), (_x) })
#define m_V_simd_to2$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1] })
#define m_V_simd_to3$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1], .z = (_v).val[2] })
#define m_V_simd_to4$(_T, _v...) l$((_T){ .x = (_v).val[0], .y = (_v).val[1], .z = (_v).val[2], .w = (_v).val[3] })

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

/*========== Macros and Definitions =========================================*/

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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_V_Vec_base_included */
