/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-24 (date of creation)
 * @updated 2026-07-14 (date of last update)
 * @ingroup dasae-headers(dh)/m-math-linalg/M-Mat
 * @prefix  m_M
 *
 * @brief   Mathematical common matrix operations
 * @details Provides a set of matrix mathematical operations and functions
 */
#pragma once
#ifndef m_math_linalg_M_Mat_base_included
#define m_math_linalg_M_Mat_base_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "types.h"
#include "../V-Vec/base.h"

/*========== Macros and Declarations ========================================*/

#define m_M_as$(/*(_T: m_M_Type)(_src)*/... /*(m_M_Type)*/) __step__m_M_as$(__VA_ARGS__)

/*========== Macros and Definitions==========================================*/

#define __step__m_M_as$(...) __step__m_M_as$__emit(__step__m_M_as$__parse __VA_ARGS__)
#define __step__m_M_as$__parse(_T_Dst...) \
    pp_uniqTok(len), pp_uniqTok(d), pp_uniqTok(s), \
        pp_uniqTok(dst), _T_Dst, pp_uniqTok(src),
#define __step__m_M_as$__emit(...) ____m_M_as$(__VA_ARGS__)
#define ____m_M_as$(__len, __d, __s, __dst, _T_Dst, __src, _src...) local_({ \
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
#endif /* m_math_linalg_M_Mat_base_included */
