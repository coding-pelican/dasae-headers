#ifndef COMPAT_LAMBDA_INCLUDED
#define COMPAT_LAMBDA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Lambda macro definition taking into account compiler and language versions */

#if defined(__cplusplus)
/* In C++, lambdas are available since C++11 */
#  if (__cplusplus >= 201103L)
/* C++11 or later */
#    define lambda(capture, params) [capture] params
#  else
/* Pre-C++11 */
// #    error "C++11 or later is required for lambda support"
#  endif

#elif defined(__GNUC__) && !defined(__clang__)
/* GCC supports nested functions in C */
/* Use GCC's nested functions to simulate lambdas */
#  define lambda(params, body)                    \
      ({                                          \
        __auto_type __lambda_function params body \
            __lambda_function;                    \
      })

#elif defined(__clang__) || defined(__clang_major__) && defined(__GNUC__)
/* Clang in C mode */
/* Clang does not support nested functions in C */
#  if __has_extension(blocks)
/* If Clang supports blocks, use them */
#    define lambda(params, body) \
        (^params body)
#  else
/* Clang without blocks extension */
// #    error "Lambdas are not supported in this compiler (Clang in C mode without blocks)"
#  endif

#else
/* Compiler does not support lambda-like constructs */
// #  error "Lambda expressions are not supported in this compiler"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_LAMBDA_INCLUDED */
