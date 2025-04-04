#ifndef BTI_AUTO_INCLUDED
#define BTI_AUTO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"

/*========== Macros and Definitions =========================================*/

#define auto comp_syn__auto
#define var  comp_syn__var
#define let  comp_syn__let

#define var_(_identifier, T...) comp_syn__var_(_identifier, T)
#define let_(_identifier, T...) comp_syn__let_(_identifier, T)

#define var_type(_identifier, T, _Expr...) comp_syn__var_type(_identifier, T, _Expr)
#define let_type(_identifier, T, _Expr...) comp_syn__let_type(_identifier, T, _Expr)

/*========== Macros and Implementations =====================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define comp_syn__var auto
#define comp_syn__let auto const
#else
/* Pre-C++11 */
#define comp_syn__auto _not_support_auto
#define comp_syn__var  _not_support_var
#define comp_syn__let  _not_support_let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L)
/* C18 later supports the auto keyword */
#undef auto
#define comp_syn__var auto
#define comp_syn__let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define comp_syn__auto __auto_type
#define comp_syn__var  __auto_type
#define comp_syn__let  __auto_type const
#else
/* Compiler does not support auto type deduction */
#define comp_syn__auto _not_support_auto
#define comp_syn__var  _not_support_var
#define comp_syn__let  _not_support_let
#warning "No support for auto type deduction in this compiler"
#endif

#define comp_syn__var_(_identifier, T...) T _identifier
#define comp_syn__let_(_identifier, T...) T const _identifier

#define comp_syn__var_type(_identifier, T, _Expr...) T _identifier = type$(T, _Expr)
#define comp_syn__let_type(_identifier, T, _Expr...) T const _identifier = type$(T, _Expr)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_AUTO_INCLUDED */
