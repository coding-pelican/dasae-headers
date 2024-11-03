#ifndef COMPAT_AUTO_INCLUDED
#define COMPAT_AUTO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

/*========== Macros and Definitions =========================================*/

/* Dynamically-typed variable definition */
#define auto RETURN_auto
#define var  RETURN_var
#define let  RETURN_let

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define RETURN_var auto
#define RETURN_let auto
#else
/* Pre-C++11 */
#define RETURN_auto no_auto
#define RETURN_var  no_var
#define RETURN_let  no_let
// #    error "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201710L)
/* C18 or later supports the auto keyword */
#undef auto
#define RETURN_var auto
#define RETURN_let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define RETURN_auto __auto_type
#define RETURN_var  __auto_type
#define RETURN_let  __auto_type
#else
/* Compiler does not support auto type deduction */
#define RETURN_auto no_auto
#define RETURN_var  no_var
#define RETURN_let  no_let
// #  error "No support for auto type deduction in this compiler"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_AUTO_INCLUDED */
