/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-01-22 (date of last update)
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
#include "plat_cfg.h"

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

#define as$(TDest, val_src) \
    FUNC__as$(TDest, val_src)

#define literal$(TLit, _Inital...)                                           \
    /**                                                                      \
     * @brief Literal macro for creating a compound literal                  \
     * @details Plain structures in C++ (without constructors) can be        \
        initialized with { } This is called aggregate initialization (C++11) \
     * @note MSVC C++ compiler does not support compound literals (C99)      \
     */                                                                      \
    SYN__literal$(TLit, _Inital)

#define initial(_Inital...)        SYN__initial(_Inital)
#define cleared()                  SYN__cleared()
#define make$(T, _Inital...)       FUNC__make$(T, _Inital)
#define makeCleared$(T)            FUNC__makeCleared$(T)
#define create$(T, _Initial...)    FUNC__create$(T, _Initial)
#define createCleared$(T)          FUNC__createCleared$(T)
#define createFrom$(T, var_src...) FUNC__createFrom$(T, var_src)

#define eval(_Expr...) SYN__eval(_Expr)
#define eval_return    /* just comment */

#define likely(_Expr...)   FUNC__likely(_Expr)
#define unlikely(_Expr...) FUNC__unlikely(_Expr)

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
#define FUNC__as$(TDest, val_src) static_cast<TDest>(val_src)
#else
#define FUNC__as$(TDest, val_src) ((TDest)(val_src))
#endif

#if defined(__cplusplus)
#define SYN__literal$(TLit, _Inital...) \
    TLit { _Inital }
#else
#define SYN__literal$(TLit, _Inital...) \
    (TLit) { _Inital }
#endif

#define SYN__initial(_Inital...) \
    { _Inital }

#define SYN__cleared() \
    { 0 }

#define FUNC__make$(T, _Inital...) \
    (literal$(T, _Inital))

#define FUNC__makeCleared$(T) \
    (literal$(T, 0))

// NOLINTBEGIN(bugprone-macro-parentheses)
#define FUNC__create$(T, _Inital...) \
    (literal$(T[1], make$(T, _Inital)))

#define FUNC__createCleared$(T) \
    (literal$(T[1], makeCleared$(T)))

#define FUNC__createFrom$(T, var_src...) \
    (literal$(T[1], var_src))
// NOLINTEND(bugprone-macro-parentheses)

#define SYN__eval(_Expr...) ({ _Expr })

#define FUNC__likely(_Expr...)   __builtin_expect(!!(_Expr), 1)
#define FUNC__unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
