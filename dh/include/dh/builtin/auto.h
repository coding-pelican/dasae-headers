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

#define var_(_identifier, _Type...) comp_syn__var_(_identifier, _Type)
#define let_(_identifier, _Type...) comp_syn__let_(_identifier, _Type)

#define var_const comp_syn__var_const
#define let_const comp_syn__let_const

#define var_const_(_identifier, _Type...) comp_syn__var_const_(_identifier, _Type)
#define let_const_(_identifier, _Type...) comp_syn__let_const_(_identifier, _Type)

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
#define comp_syn__auto __no_support_auto
#define comp_syn__var  __no_support_var
#define comp_syn__let  __no_support_let
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
#define comp_syn__auto __no_support_auto
#define comp_syn__var  __no_support_var
#define comp_syn__let  __no_support_let
#warning "No support for auto type deduction in this compiler"
#endif

#define comp_syn__var_(_identifier, _Type...) _Type _identifier
#define comp_syn__let_(_identifier, _Type...) _Type const _identifier

#define comp_syn__var_const const var*
#define comp_syn__let_const let* const

#define comp_syn__var_const_(_identifier, _Type...) const _Type _identifier
#define comp_syn__let_const_(_identifier, _Type...) const _Type const _identifier

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_AUTO_INCLUDED */
