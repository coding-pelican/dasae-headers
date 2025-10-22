/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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

#define $inline        comp_attr__$inline
#define $inline_always comp_attr__$inline_always
#define $inline_never  comp_attr__$inline_never

#define $deprecated                             comp_attr__$deprecated
#define $deprecated_msg(_Msg)                   comp_attr__$deprecated_msg(_Msg)
#define $deprecated_instead(_Msg, _Replacement) comp_attr__$deprecated_instead(_Msg, _Replacement)

#define $on_load comp_attr__$on_load
#define $on_exit comp_attr__$on_exit

#define $must_check \
    /** \
     * @brief Attribute marks a function as returning a value that must be \
     *        checked to avoid potential errors or warnings \
     * @details This attribute can be used to ensure that a function's return \
     *          value is checked to avoid potential errors or warnings \
     */ \
    comp_attr__$must_check
#define $no_return \
    /** \
     * @brief Attribute marks a function as not returning a value \
     * @details This attribute can be used to ensure that a function does \
     *          not return a value \
     */ \
    comp_attr__$no_return

#define $ignore_void \
    /** \
     * @brief Ignores expression or return value \
     */ \
    comp_attr__$ignore_void
#define $ignore \
    /** \
     * @brief Ignores payload capture \
     */ \
    comp_attr__$ignore
#define let_ignore \
    /** \
     * @brief Ignores expression or return value \
     */ \
    comp_attr__let_ignore


#define $used(_Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as used to suppress compiler warnings \
     * @details In macro functions, the arguments are marked as used \
     * @param _Expr... Variable number of arguments to be marked as used \
     */ \
    comp_attr__$used(_Expr)
#define $unused(_Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as unused to suppress compiler warnings \
     * @param _Expr... Variable number of arguments to be marked as unused \
     */ \
    comp_attr__$unused(_Expr)

#define $keep_symbol  comp_attr__$keep_symbol
#define $maybe_unused comp_attr__$maybe_unused
#define $must_use     comp_attr__$must_use


#define as$(/*(_TDest)(_src...)*/... /*(_TDest)*/) \
    /** \
     * @brief Cast macro for converting a value to a different type \
     * @details This macro is used to cast a value from one type to another \
     * @param _TDest The destination type to cast to \
     * @param _src The value to cast \
     * @return The casted value \
     */ \
    __comp_exec__as$(pp_defer(__comp_emit__as$)(__comp_split__as$ __VA_ARGS__))

#define lit$(T_Lit, _Inital... /*T_Lit*/) \
    /** \
     * @brief Literal macro for creating a compound lit \
     * @details Plain structures in C++ (without constructors) can be \
     *          initialized with { } This is called aggregate initialization \
     *          (C++11) \
     * @note MSVC C++ compiler does not support compound literals (C99) \
     * @param T_Lit The type of the literal to create \
     * @param _Inital The initial values for the literal \
     * @return The created literal \
     */ \
    comp_syn__lit$(T_Lit, _Inital)

#define initial(_Inital...)                      comp_syn__initial(_Inital)
#define initial$(T_Lit, _Inital... /*T*/)        comp_syn__initial$(T_Lit, _Inital)
#define cleared()                                comp_syn__cleared()
#define cleared$(T_Lit /*T*/)                    comp_syn__cleared$(T_Lit)
#define make$(T_Lit, _Inital... /*T*/)           comp_syn__make$(T_Lit, _Inital)
#define makeCleared$(T_Lit /*T*/)                comp_syn__makeCleared$(T_Lit)
#define create$(T_Lit, _Initial... /*Ptr$T*/)    comp_syn__create$(T_Lit, _Initial)
#define createCleared$(T_Lit /*Ptr$T*/)          comp_syn__createCleared$(T_Lit)
#define createFrom(var_src... /*Ptr$T*/)         comp_syn__createFrom(var_src)
#define createFrom$(T_Lit, var_src... /*Ptr$T*/) comp_syn__createFrom$(T_Lit, var_src)
#define move(_p_val... /*(TypeOf(*_p_val))*/)    comp_syn__move(_p_val)
#define copy(_val... /*(TypeOf(_val))*/)         comp_syn__copy(_val)

#define bti_Generic_match$(T, _Pattern...) comp_syn__bti_Generic_match$(T, _Pattern)
#define bti_Generic_pattern$(T)            comp_syn__bti_Generic_pattern$(T)
#define bti_Generic_fallback_              comp_syn__bti_Generic_fallback_

#define blk              comp_syn__blk
#define blk_return       comp_syn__blk_return
#define blk_return_(...) comp_syn__blk_return_(__VA_ARGS__)

#define local_label                 comp_syn__local_label
#define likely(_Expr... /*bool*/)   comp_syn__likely(_Expr)
#define unlikely(_Expr... /*bool*/) comp_syn__unlikely(_Expr)

#define $like_ptr comp_attr__$like_ptr
#define $flexible comp_attr__$flexible

#define pragma_guard_(_push, _ctx, _pop, _code...) \
    _Pragma(_push) _Pragma(_ctx) _code _Pragma(_pop)

/*========== Macros and Implementations =====================================*/

#define comp_attr__$inline        BUILTIN_COMP_INLINE
#define comp_attr__$inline_always BUILTIN_COMP_ALWAYS_INLINE
#define comp_attr__$inline_never  BUILTIN_COMP_NO_INLINE

#define comp_attr__$deprecated                             BUILTIN_COMP_DEPRECATED
#define comp_attr__$deprecated_msg(_Msg)                   BUILTIN_COMP_DEPRECATED_MSG(_Msg)
#define comp_attr__$deprecated_instead(_Msg, _Replacement) BUILTIN_COMP_DEPRECATED_MSG(_Msg, _Replacement)

#define comp_attr__$on_load __attribute__((constructor))
#define comp_attr__$on_exit __attribute__((destructor))

#define comp_attr__$must_check  BUILTIN_COMP_MUST_CHECK
#define comp_attr__$no_return   BUILTIN_COMP_NO_RETURN
#define comp_attr__$ignore_void (void)
#define comp_attr__$ignore \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    let_ignore
#define comp_attr__let_ignore __attribute__((unused)) let pp_uniqTok(ignored)


#define comp_attr__$used(_Expr...) _Expr
/* begin unused */
#define comp_attr__$unused(_Expr...) \
    comp_attr__$unused__payload(comp_attr__$unused__countArgs(_Expr), _Expr)
#define comp_attr__$unused__payload(_Count, _Args...) \
    pp_join(__, comp_attr__$unused, _Count)(_Args)
#define comp_attr__$unused__countArgs(_Args...) \
    comp_attr__$unused__countArgs__select( \
        _Args, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 \
    )
#define comp_attr__$unused__countArgs__select( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N
#define comp_attr__$unused__1(x1) \
    (void)(x1)
#define comp_attr__$unused__2(x1, x2) \
    (void)(x1), (void)(x2)
#define comp_attr__$unused__3(x1, x2, x3) \
    (void)(x1), (void)(x2), (void)(x3)
#define comp_attr__$unused__4(x1, x2, x3, x4) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4)
#define comp_attr__$unused__5(x1, x2, x3, x4, x5) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5)
#define comp_attr__$unused__6(x1, x2, x3, x4, x5, x6) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6)
#define comp_attr__$unused__7(x1, x2, x3, x4, x5, x6, x7) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7)
#define comp_attr__$unused__8(x1, x2, x3, x4, x5, x6, x7, x8) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8)
#define comp_attr__$unused__9(x1, x2, x3, x4, x5, x6, x7, x8, x9) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9)
#define comp_attr__$unused__10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10)
#define comp_attr__$unused__11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11)
#define comp_attr__$unused__12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12)
#define comp_attr__$unused__13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13)
#define comp_attr__$unused__14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14)
#define comp_attr__$unused__15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14), (void)(x15)
#define comp_attr__$unused__16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), (void)(x13), (void)(x14), (void)(x15), (void)(x16)
/* end unused */

#define comp_attr__$keep_symbol  __attribute__((used))
#define comp_attr__$maybe_unused __attribute__((unused))
#define comp_attr__$must_use     __attribute__((warn_unused_result))

#define __comp_exec__as$(...)  __VA_ARGS__
#define __comp_split__as$(...) __VA_ARGS__, __comp_next__as$
#define __comp_next__as$(...)  __VA_ARGS__
#if defined(__cplusplus)
#define __comp_emit__as$(_TDest, _src...) (static_cast<_TDest>(_src))
#else
#define __comp_emit__as$(_TDest, _src...) ((_TDest)(_src))
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
#define comp_syn__initial$(T_Lit, _Inital... /*T*/) \
    (lit$(T_Lit, _Inital))
#define comp_syn__cleared() \
    {}
#define comp_syn__cleared$(T_Lit /*T*/) \
    (lit$(T_Lit))

#define comp_syn__make$(T_Lit, _Inital...) (lit$(T_Lit, _Inital))
#define comp_syn__makeCleared$(T_Lit)      (lit$(T_Lit))

// NOLINTBEGIN(bugprone-macro-parentheses)
#define comp_syn__create$(T_Lit, _Inital...) \
    (&*lit$(T_Lit[1], [0] = make$(T_Lit, _Inital)))
#define comp_syn__createCleared$(T_Lit) \
    (&*lit$(T_Lit[1], [0] = makeCleared$(T_Lit)))
#define comp_syn__createFrom(var_src...) \
    (&*lit$(TypeOfUnqual(var_src)[1], [0] = var_src))
#define comp_syn__createFrom$(T_Lit, var_src...) \
    (&*lit$(T_Lit[1], [0] = var_src))
// NOLINTEND(bugprone-macro-parentheses)

#define comp_syn__move(_p_val...) ({ \
    let_(__p_val, TypeOf(_p_val)) = _p_val; \
    let_(__val, TypeOf(*__p_val)) = *__p_val; \
    *__p_val                      = ((TypeOf(__val)){}); \
    __val; \
})
#define comp_syn__copy(_val...) \
    (*&*((TypeOf(_val)[1]){ [0] = _val }))

#define comp_syn__bti_Generic_match$(T, _Pattern...) \
    _Generic(T, _Pattern)
#define comp_syn__bti_Generic_pattern$(T) \
T:
#define comp_syn__bti_Generic_fallback_ \
    default:

#define comp_syn__blk              /* just comment for compound statement expression ({...}) */
#define comp_syn__blk_return       /* just comment for compound statement expression ({...}) */
#define comp_syn__blk_return_(...) __VA_ARGS__

#define comp_syn__local_label        __label__
#define comp_syn__likely(_Expr...)   __builtin_expect(!!(_Expr), 1)
#define comp_syn__unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)

#define comp_attr__$like_ptr [1]
#define comp_attr__$flexible [0]

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_COMP_INCLUDED */
