#ifndef builtin_auto__included
#define builtin_auto__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"

/*========== Macros and Definitions =========================================*/

#define auto __comp_syn__auto
#define var  __comp_syn__var
#define let  __comp_syn__let

#define var_(_ident, _T...) __comp_syn__var_(_ident, _T)
#define let_(_ident, _T...) __comp_syn__let_(_ident, _T)

#define var_const __comp_syn__var_const
#define let_const __comp_syn__let_const

#define var_const_(_ident, _T...) __comp_syn__var_const_(_ident, _T)
#define let_const_(_ident, _T...) __comp_syn__let_const_(_ident, _T)

/*========== Macros and Implementations =====================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define __comp_syn__var auto
#define __comp_syn__let auto const
#else
/* Pre-C++11 */
#define __comp_syn__auto __no_support__auto
#define __comp_syn__var  __no_support__var
#define __comp_syn__let  __no_support__let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L)
/* C18 later supports the auto keyword */
#undef auto
#define __comp_syn__var auto
#define __comp_syn__let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define __comp_syn__auto __auto_type
#define __comp_syn__var  __auto_type
#define __comp_syn__let  __auto_type const
#else
/* Compiler does not support auto type deduction */
#define __comp_syn__auto __no_support__auto
#define __comp_syn__var  __no_support__var
#define __comp_syn__let  __no_support__let
#warning "No support for auto type deduction in this compiler"
#endif

#define __comp_syn__var_(_ident, _T...) _T _ident
#define __comp_syn__let_(_ident, _T...) _T const _ident

#define __comp_syn__var_const const var*
#define __comp_syn__let_const let* const

#define __comp_syn__var_const_(_ident, _T...) const _T _ident
#define __comp_syn__let_const_(_ident, _T...) const _T const _ident

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_auto__included */
