#ifndef BUILTIN_AUTO_INCLUDED
#define BUILTIN_AUTO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"

/*========== Macros and Definitions =========================================*/

/* Dynamically-typed variable definition */
#define auto SYN__auto
#define var  SYN__var
#define let  SYN__let

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define SYN__var auto
#define SYN__let const auto
#else
/* Pre-C++11 */
#define SYN__auto _not_support_auto
#define SYN__var  _not_support_var
#define SYN__let  _not_support_let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L)
/* C18 later supports the auto keyword */
#undef auto
#define SYN__var auto
#define SYN__let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define SYN__auto __auto_type
#define SYN__var  __auto_type
#define SYN__let  const __auto_type
#else
/* Compiler does not support auto type deduction */
#define SYN__auto _not_support_auto
#define SYN__var  _not_support_var
#define SYN__let  _not_support_let
#warning "No support for auto type deduction in this compiler"
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_AUTO_INCLUDED */
