#pragma once
#ifndef m_math_linalg_cfg__included
#define m_math_linalg_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if !defined(m_linalg_$inline)
#define m_linalg_$inline __attr__m_linalg_$inline
#endif /* !defined(m_linalg_$inline) */
#define m_linalg__$inline_default __attr__m_linalg__$inline_default

/*========== Macros and Definitions==========================================*/

#define __attr__m_linalg_$inline m_linalg__$inline_default
#define __attr__m_linalg__$inline_default pp_if_(debug_enabled)( \
    pp_then_($inline), pp_else_($inline_always) \
)

#if defined(DH_M_LINALG_INLINE_ALWAYS)
#undef __attr__m_linalg__$inline_default
#define __attr__m_linalg__$inline_default $inline_always
#endif /* defined(DH_M_LINALG_INLINE_ALWAYS) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_cfg__included */
