#pragma once
#ifndef prl_tpl_Val__included
#define prl_tpl_Val__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

// #define V$$(_T...) __comp_anon__V$$(_T)
// #define V$(_T...) __comp_alias__V$(_T)
// #define T_decl_V$(_T...) __comp_gen__T_decl_V$(_T)
// #define T_impl_V$(_T...) __comp_gen__T_impl_V$(_T)
// #define T_use_V$(_T...) __comp_gen__T_use_V$(_T)

#define V_ref(_v /*: T*/... /*P$$(_T)*/) ref(_v)
#define refV V_ref

/*========== Macros and Definitions =========================================*/

// #define __comp_anon__V$$(_T...) TypeOf(_T $flexible)
// #define __comp_alias__V$(_T...) tpl$(V, _T)
// #define __comp_gen__T_decl_V$(_T...) \
//     $maybe_unused typedef TypeOf(_T $flexible) V$(_T);
// #define __comp_gen__T_impl_V$(_T...) \
//     $maybe_unused typedef TypeOf(_T $flexible) V$(_T)
// #define __comp_gen__T_use_V$(_T...) \
//     T_impl_V$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Val__included */
