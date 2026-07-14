#pragma once
#ifndef prl_tpl_V_Val__included
#define prl_tpl_V_Val__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

// #define V$$(_$T...) __comp_anon__V$$(_$T)
// #define V$(_$T...) __comp_alias__V$(_$T)
// #define T_decl_V$(_$T...) __comp_gen__T_decl_V$(_$T)
// #define T_impl_V$(_$T...) __comp_gen__T_impl_V$(_$T)
// #define T_use_V$(_$T...) __comp_gen__T_use_V$(_$T)

#define V_ref(_$v /*: T*/... /*P$$(_$T)*/) ref(_$v)
#define refV V_ref

/*========== Macros and Definitions =========================================*/

// #define __comp_anon__V$$(_$T...) TypeOf(_$T $flexible)
// #define __comp_alias__V$(_$T...) tpl$(V, _$T)
// #define __comp_gen__T_decl_V$(_$T...) \
//     $maybe_unused typedef TypeOf(_$T $flexible) V$(_$T);
// #define __comp_gen__T_impl_V$(_$T...) \
//     $maybe_unused typedef TypeOf(_$T $flexible) V$(_$T)
// #define __comp_gen__T_use_V$(_$T...) \
//     T_impl_V$(_$T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_V_Val__included */
