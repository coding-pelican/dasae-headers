/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Compiler-specific configurations and optimizations
 * @details Provides compiler-specific inline directives and optimizations based on detected environment
 */

#ifndef BUILTIN_COMP_INCLUDED
#define BUILTIN_COMP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "arch_cfg.h"
#include "comp_cfg.h"
#include "lang_cfg.h"
#include "pltf_cfg.h"

/*========== Macros and Definitions =========================================*/

#if !defined(COMP)
#define COMP_TIME (0)
#else
#define COMP_TIME (1)
#endif

#define static_inline SYN__static_inline
#define extern_inline SYN__extern_inline
#define force_inline  SYN__force_inline
#define must_check    ATTR__must_check

#define used(_Expr...)                                                    \
    /**                                                                   \
     * @brief Marks variables or expressions as used to suppress compiler \
    warnings                                                              \
     * @details In macro functions, the arguments are marked as used      \
     * @param _Expr... Variable number of arguments to be marked as used  \
     */                                                                   \
    ATTR__used(_Expr)

#define unused(_Expr...)                                                   \
    /**                                                                    \
     * @brief Marks variables or expressions as intentionally unused       \
    to suppress compiler warnings                                          \
     * @param _Expr... Variable number of arguments to be marked as unused \
     */                                                                    \
    ATTR__unused(_Expr)

#define ignore                                                           \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    ATTR__ignore

#define as(_T, _val) \
    FUNC_as(_T, _val)

#define literal(_T, _Inital...)                                              \
    /**                                                                      \
     * @brief Literal macro for creating a compound literal                  \
     * @details Plain structures in C++ (without constructors) can be        \
        initialized with { } This is called aggregate initialization (C++11) \
     * @note MSVC C++ compiler does not support compound literals (C99)      \
     */                                                                      \
    SYN__literal(_T, _Inital)

#define eval(expr...) SYN__eval(expr)
#define eval_return   /* just comment */

/*========== Macro Implementations ==========================================*/

/* Inline Definitions Based on Language Mode */
#if BUILTIN_LANG_MODE_CPP
/* C++ always supports 'inline' */
#define SYN__static_inline static inline
#define SYN__extern_inline inline
#elif BUILTIN_LANG_C_99
/* C99 or later */
#define SYN__static_inline static inline
#define SYN__extern_inline inline
#else
/* Use compiler-specific inline directives */
#define SYN__static_inline BUILTIN_COMP_INLINE static
#define SYN__extern_inline BUILTIN_COMP_INLINE
#endif
#define SYN__force_inline BUILTIN_COMP_FORCE_INLINE
#define ATTR__must_check  BUILTIN_COMP_MUST_CHECK

#define ATTR__used(_Expr...)   _Expr
#define ATTR__unused(_Expr...) ((void)(_Expr))
#define ATTR__ignore           (void)

#if defined(__cplusplus)
#define FUNC_as(_T, _val) static_cast<_T>(_val)
#else
#define FUNC_as(_T, _val) ((_T)(_val))
#endif

#if defined(__cplusplus)
#define SYN__literal(_T, _Inital...) \
    _T { _Inital }
#else
#define SYN__literal(_T, _Inital...) \
    (_T) { _Inital }
#endif

#define SYN__eval(expr...) ({ expr })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
