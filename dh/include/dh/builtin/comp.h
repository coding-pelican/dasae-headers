/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/bti
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

#define $on_load comp_attr__$on_load
#define $on_exit comp_attr__$on_exit

#define must_check                                                            \
    /**                                                                       \
     * @brief Attribute marks a function as returning a value that must be    \
     *        checked to avoid potential errors or warnings                   \
     * @details This attribute can be used to ensure that a function's return \
     *          value is checked to avoid potential errors or warnings        \
     */                                                                       \
    ATTR__must_check
#define no_return                                                         \
    /**                                                                   \
     * @brief Attribute marks a function as not returning a value         \
     * @details This attribute can be used to ensure that a function does \
     *          not return a value                                        \
     */                                                                   \
    ATTR__no_return
#define ignore                                                           \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    ATTR__ignore

#define used(_Expr... /*void*/)                                                    \
    /**                                                                            \
     * @brief Marks variables or expressions as used to suppress compiler warnings \
     * @details In macro functions, the arguments are marked as used               \
     * @param _Expr... Variable number of arguments to be marked as used           \
     */                                                                            \
    ATTR__used(_Expr)
#define unused(_Expr... /*void*/)                                                    \
    /**                                                                              \
     * @brief Marks variables or expressions as unused to suppress compiler warnings \
     * @param _Expr... Variable number of arguments to be marked as unused           \
     */                                                                              \
    ATTR__unused(_Expr)

#define as$(T_Dest, val_src... /*T_Dest*/)                                  \
    /**                                                                     \
     * @brief Cast macro for converting a value to a different type         \
     * @details This macro is used to cast a value from one type to another \
     * @param T_Dest The destination type to cast to                        \
     * @param val_src The value to cast                                     \
     * @return The casted value                                             \
     */                                                                     \
    comp_syn__as$(T_Dest, val_src)
#define lit$(T_Lit, _Inital... /*T_Lit*/)                                    \
    /**                                                                      \
     * @brief Literal macro for creating a compound lit                      \
     * @details Plain structures in C++ (without constructors) can be        \
     *          initialized with { } This is called aggregate initialization \
     *          (C++11)                                                      \
     * @note MSVC C++ compiler does not support compound literals (C99)      \
     * @param T_Lit The type of the literal to create                        \
     * @param _Inital The initial values for the literal                     \
     * @return The created literal                                           \
     */                                                                      \
    comp_syn__lit$(T_Lit, _Inital)

#define initial(_Inital...)                      comp_syn__initial(_Inital)
#define cleared()                                comp_syn__cleared()
#define make$(T_Lit, _Inital... /*T*/)           comp_syn__make$(T_Lit, _Inital)
#define makeCleared$(T_Lit /*T*/)                comp_syn__makeCleared$(T_Lit)
#define create$(T_Lit, _Initial... /*Ptr$T*/)    comp_syn__create$(T_Lit, _Initial)
#define createCleared$(T_Lit /*Ptr$T*/)          comp_syn__createCleared$(T_Lit)
#define createFrom(var_src... /*Ptr$T*/)         comp_syn__createFrom(var_src)
#define createFrom$(T_Lit, var_src... /*Ptr$T*/) comp_syn__createFrom$(T_Lit, var_src)

#define bti_Generic_match$(T, _Pattern...) comp_syn__bti_Generic_match$(T, _Pattern)
#define bti_Generic_pattern$(T)            comp_syn__bti_Generic_pattern$(T)
#define bti_Generic_fallback_              comp_syn__bti_Generic_fallback_
#define eval                               comp_syn__eval
#define eval_return                        comp_syn__eval_return
#define eval_return_(...)                  comp_syn__eval_return_(__VA_ARGS__)

#define likely(_Expr... /*bool*/)   comp_syn__likely(_Expr)
#define unlikely(_Expr... /*bool*/) comp_syn__unlikely(_Expr)

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

#define comp_attr__$on_load __attribute__((constructor))
#define comp_attr__$on_exit __attribute__((destructor))

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
#define comp_syn__as$(T_Dest, val_src...) (static_cast<T_Dest>(val_src))
#else
#define comp_syn__as$(T_Dest, val_src...) ((T_Dest)(val_src))
#endif
#if defined(__cplusplus)
#define comp_syn__lit$(T_Lit, _Inital...) \
    T_Lit { _Inital }
#else
#define comp_syn__lit$(T_Lit, _Inital...) \
    (T_Lit) { _Inital }
#endif

#define comp_syn__initial(_Inital...) \
    { _Inital }
#define comp_syn__cleared() \
    {}

#define comp_syn__make$(T_Lit, _Inital...) (lit$(T_Lit, _Inital))
#define comp_syn__makeCleared$(T_Lit)      (lit$(T_Lit))

// NOLINTBEGIN(bugprone-macro-parentheses)
#define comp_syn__create$(T_Lit, _Inital...) \
    (&*lit$(T_Lit[1], [0] = make$(T_Lit, _Inital)))
#define comp_syn__createCleared$(T_Lit) \
    (&*lit$(T_Lit[1], [0] = makeCleared$(T_Lit)))
#define comp_syn__createFrom(var_src...) \
    (&*lit$(TypeOf(var_src)[1], [0] = var_src))
#define comp_syn__createFrom$(T_Lit, var_src...) \
    (&*lit$(T_Lit[1], [0] = var_src))
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

#define comp_syn__likely(_Expr...)   __builtin_expect(!!(_Expr), 1)
#define comp_syn__unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
