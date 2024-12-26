#ifndef BUILTIN_AUTO_INCLUDED
#define BUILTIN_AUTO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

/*========== Macros and Definitions =========================================*/

/* Dynamically-typed variable definition */
#define auto IMPL_auto
#define var  IMPL_var
#define let  IMPL_let

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define IMPL_var auto
#define IMPL_let const auto
#else
/* Pre-C++11 */
#define IMPL_auto no_auto
#define IMPL_var  no_var
#define IMPL_let  const no_let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201710L)
/* C18 or later supports the auto keyword */
#undef auto
// #define IMPL_var auto
// #define IMPL_let auto
#define IMPL_auto __auto_type
#define IMPL_var  __auto_type
#define IMPL_let  const __auto_type
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define IMPL_auto __auto_type
#define IMPL_var  __auto_type
#define IMPL_let  const __auto_type
#else
/* Compiler does not support auto type deduction */
#define IMPL_auto no_auto
#define IMPL_var  no_var
#define IMPL_let  const no_let
#warning "No support for auto type deduction in this compiler"
#endif

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_AUTO_INCLUDED */
