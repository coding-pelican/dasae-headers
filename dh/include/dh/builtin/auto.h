#ifndef BUILTIN_AUTO_INCLUDED
#define BUILTIN_AUTO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"

/*========== Macros and Definitions =========================================*/

/* Dynamically-typed variable definition */
#define auto SYN_auto
#define var  SYN_var
#define let  SYN_let

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define SYN_var auto
#define SYN_let const auto
#else
/* Pre-C++11 */
#define SYN_auto _not_support_auto
#define SYN_var  _not_support_var
#define SYN_let  _not_support_let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L)
/* C18 later supports the auto keyword */
#undef auto
#define SYN_var auto
#define SYN_let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define SYN_auto __auto_type
#define SYN_var  __auto_type
#define SYN_let  const __auto_type
#else
/* Compiler does not support auto type deduction */
#define SYN_auto _not_support_auto
#define SYN_var  _not_support_var
#define SYN_let  _not_support_let
#warning "No support for auto type deduction in this compiler"
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_AUTO_INCLUDED */
