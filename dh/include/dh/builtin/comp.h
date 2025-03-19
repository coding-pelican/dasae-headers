/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-02-26 (date of last update)
 * @version v0.1-alpha.3
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

/*========== Includes =======================================================*/

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

#define static_inline ATTR__static_inline
#define extern_inline ATTR__extern_inline
#define force_inline  ATTR__force_inline
#define no_inline     ATTR__no_inline

#define warn_deprecated                             ATTR__warn_deprecated
#define warn_deprecated_msg(_Msg)                   ATTR__warn_deprecated_msg(_Msg)
#define warn_deprecated_instead(_Msg, _Replacement) ATTR__warn_deprecated_instead(_Msg, _Replacement)

#define must_check                                                                  \
    /**                                                                             \
     * @brief Attribute marks a function as returning a value that must be checked  \
     to avoid potential errors or warnings                                          \
     * @details This attribute can be used to ensure that a function's return value \
     */                                                                             \
    ATTR__must_check
#define no_return \
    /**           \
     * @brief     \
     *            \
     */           \
    ATTR__no_return
#define ignore                                                           \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    ATTR__ignore

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

#define bti_Generic_match$(T, _Pattern...) comp_syn__bti_Generic_match$(T, _Pattern)
#define bti_Generic_pattern$(T)            comp_syn__bti_Generic_pattern$(T)
#define bti_Generic_fallback_              comp_syn__bti_Generic_fallback_
#define eval                               comp_syn__eval
#define eval_return                        comp_syn__eval_return
#define eval_return_(...)                  comp_syn__eval_return_(__VA_ARGS__)

#define likely(_Expr...)   FUNC__likely(_Expr)
#define unlikely(_Expr...) FUNC__unlikely(_Expr)

/*========== Macros and Implementations =====================================*/

/* Inline Definitions Based on Language Mode */
#if BUILTIN_LANG_MODE_CPP
/* C++ always supports 'inline' */
#define ATTR__static_inline static inline
#define ATTR__extern_inline inline
#elif BUILTIN_LANG_C_99
/* C99 or later */
#define ATTR__static_inline static inline
#define ATTR__extern_inline inline
#else
/* Use compiler-specific inline directives */
#define ATTR__static_inline BUILTIN_COMP_INLINE static
#define ATTR__extern_inline BUILTIN_COMP_INLINE
#endif

#define ATTR__force_inline BUILTIN_COMP_FORCE_INLINE
#define ATTR__no_inline    BUILTIN_COMP_NO_INLINE

#define ATTR__warn_deprecated                             BUILTIN_COMP_DEPRECATED
#define ATTR__warn_deprecated_msg(_Msg)                   BUILTIN_COMP_DEPRECATED_MSG(_Msg)
#define ATTR__warn_deprecated_instead(_Msg, _Replacement) BUILTIN_COMP_DEPRECATED_MSG(_Msg, _Replacement)

#define ATTR__must_check BUILTIN_COMP_MUST_CHECK
#define ATTR__no_return  BUILTIN_COMP_NO_RETURN
#define ATTR__ignore     (void)

#define ATTR__used(_Expr...) _Expr
/* begin unused */
#define ATTR__unused(...) \
    ATTR__unused_IMPL(ATTR__unused_COUNT(__VA_ARGS__), __VA_ARGS__)

#define ATTR__unused_IMPL(N, ...) pp_cat(ATTR__unused_, N)(__VA_ARGS__)

#define ATTR__unused_COUNT(...) \
    ATTR__unused_SELECT_COUNT(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define ATTR__unused_SELECT_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define ATTR__unused_1(x1)                                                                     (void)(x1)
#define ATTR__unused_2(x1, x2)                                                                 (void)(x1), (void)(x2)
#define ATTR__unused_3(x1, x2, x3)                                                             (void)(x1), (void)(x2), (void)(x3)
#define ATTR__unused_4(x1, x2, x3, x4)                                                         (void)(x1), (void)(x2), (void)(x3), (void)(x4)
#define ATTR__unused_5(x1, x2, x3, x4, x5)                                                     (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5)
#define ATTR__unused_6(x1, x2, x3, x4, x5, x6)                                                 (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6)
#define ATTR__unused_7(x1, x2, x3, x4, x5, x6, x7)                                             (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7)
#define ATTR__unused_8(x1, x2, x3, x4, x5, x6, x7, x8)                                         (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8)
#define ATTR__unused_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)                                     (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9)
#define ATTR__unused_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)                               (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10)
#define ATTR__unused_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)                          (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11)
#define ATTR__unused_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)                     (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12)
#define ATTR__unused_13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13)                (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13)
#define ATTR__unused_14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14)           (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14)
#define ATTR__unused_15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15)      (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14), (void)(x15)
#define ATTR__unused_16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14), (void)(x15), (void)(x16)
/* end unused */
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
    (literal$(T[1], [0] = make$(T, _Inital)))

#define FUNC__createCleared$(T) \
    (literal$(T[1], [0] = makeCleared$(T)))

#define FUNC__createFrom$(T, var_src...) \
    (literal$(T[1], [0] = var_src))
// NOLINTEND(bugprone-macro-parentheses)

#define comp_syn__bti_Generic_match$(T, _Pattern...) \
    _Generic(T, _Pattern)
#define comp_syn__bti_Generic_pattern$(T) \
    T:
#define comp_syn__bti_Generic_fallback_ \
    default:

#define comp_syn__eval              /* just comment for expr stmt ({...}) */
#define comp_syn__eval_return       /* just comment for expr stmt ({...}) */
#define comp_syn__eval_return_(...) __VA_ARGS__

#define FUNC__likely(_Expr...)   __builtin_expect(!!(_Expr), 1)
#define FUNC__unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
