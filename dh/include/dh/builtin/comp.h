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
    __as$__emit(__VA_ARGS__)
#if defined(__cplusplus)
#define __as$__emit(...)      (__as$__expandT __VA_ARGS__)
#define __as$__expandT(_T...) static_cast<_T>
#else
#define __as$__emit(...) (__VA_ARGS__)
#endif

#if UNUSED_CODE
#define as$(/*(_TDest)(_src...)*/... /*(_TDest)*/) \
    /** \
     * @brief Cast macro for converting a value to a different type \
     * @details This macro is used to cast a value from one type to another \
     * @param _TDest The destination type to cast to \
     * @param _src The value to cast \
     * @return The casted value \
     */ \
    __as$__step(pp_defer(__as$__emit)(__as$__sep __VA_ARGS__))
#define __as$__step(...)  __VA_ARGS__
#define __as$__sep(...)   __VA_ARGS__, __as$__next$
#define __as$__next$(...) __VA_ARGS__
#if defined(__cplusplus)
#define __as$__emit(_TDest, _src...) (static_cast<_TDest>(_src))
#else
#define __as$__emit(_TDest, _src...) ((_TDest)(_src))
#endif
#endif /* UNUSED_CODE */

#define lit$(/*(_T){_initial...}*/... /*(_T)*/) \
    __lit$__emit(__VA_ARGS__)
#if defined(__cplusplus)
#define __lit$__emit(...)      (__lit$__expandT __VA_ARGS__)
#define __lit$__expandT(_T...) _T
#else
#define __lit$__emit(...) (__VA_ARGS__)
#endif

#if UNUSED_CODE
#define lit$(/*(_T){_initial...}*/... /*(_T)*/) \
    __lit$__step(pp_defer(__lit$__emit)(__lit$__sep __VA_ARGS__))
#define __lit$__step(...)             __VA_ARGS__
#define __lit$__sep(_T...)            _T,
#define __lit$__emit(_T, _initial...) __lit$__emitNext(_T, (_initial))
#if defined(__cplusplus)
#define __lit$__emitNext(_T, _initial...) (_T __lit$__expandInitial _initial)
#else
#define __lit$__emitNext(_T, _initial...) ((_T)__lit$__expandInitial _initial)
#endif
#define __lit$__expandInitial(_initial...) _initial
#endif /* UNUSED_CODE */

#define make$(/*(_T){_initial...}*/... /*(_T)*/) \
    __make$__step(pp_defer(__make$__emit)(__make$__sep __VA_ARGS__))
#define __make$__step(...)             __VA_ARGS__
#define __make$__sep(_T...)            _T,
#define __make$__emit(_T, _initial...) __make$__emitNext(_T, (_initial))
#define __make$__emitNext(_T, _initial...) \
    (*lit$((_T[1]){ [0] = __make$__expandInitial _initial }))
#define __make$__expandInitial(_initial...) _initial

#define create$(/*(_T){_initial...}*/... /*(P$$(_T))*/) \
    __create$__step(pp_defer(__create$__emit)(__create$__sep __VA_ARGS__))
#define __create$__step(...)             __VA_ARGS__
#define __create$__sep(_T...)            _T,
#define __create$__emit(_T, _initial...) __create$__emitNext(_T, (_initial))
#define __create$__emitNext(_T, _initial...) \
    (&make$((_T)__create$__expandInitial _initial))
#define __create$__expandInitial(_initial...) _initial

#define type$(/*(_T)(_raw...)*/... /*(_T)*/) \
    /* TODO: Add type checking */ \
    __type$__step(pp_defer(__type$__emit)(__type$__sep __VA_ARGS__))
#define __type$__step(...)             __VA_ARGS__
#define __type$__sep(_T...)            _T, __type$__sepRaw
#define __type$__sepRaw(_raw...)       _raw
#define __type$__emit(_T, _raw...)     __type$__emitNext(_T, _raw)
#define __type$__emitNext(_T, _raw...) make$((_T){ .as_raw = _raw })
#define type$O$(/*(_T)(_raw...)*/... /*(_T)*/) \
    __type$O$__step(pp_defer(__type$O$__emit)(__type$O$__sep __VA_ARGS__))
#define __type$O$__step(...)             __VA_ARGS__
#define __type$O$__sep(_T...)            _T, __type$O$__sepRaw
#define __type$O$__sepRaw(_raw...)       _raw
#define __type$O$__emit(_T, _raw...)     __type$O$__emitNext(_T, _raw)
#define __type$O$__emitNext(_T, _raw...) make$((O$(_T)){ .as_raw = _raw.as_raw })
#define type$E$(/*(_T)(_raw...)*/... /*(_T)*/) \
    __type$E$__step(pp_defer(__type$E$__emit)(__type$E$__sep __VA_ARGS__))
#define __type$E$__step(...)             __VA_ARGS__
#define __type$E$__sep(_T...)            _T, __type$E$__sepRaw
#define __type$E$__sepRaw(_raw...)       _raw
#define __type$E$__emit(_T, _raw...)     __type$E$__emitNext(_T, _raw)
#define __type$E$__emitNext(_T, _raw...) make$((E$(_T)){ .as_raw = _raw.as_raw })

#define cleared() comp_syn__cleared()
#define comp_syn__cleared() \
    {}

#define move(_p_val... /*(TypeOf(*_p_val))*/) comp_syn__move(_p_val)
#define comp_syn__move(_p_val...)             ({ \
    let_(__p_val, TypeOf(_p_val)) = _p_val; \
    let_(__val, TypeOf(*__p_val)) = *__p_val; \
    *__p_val = ((TypeOf(__val)){}); \
    __val; \
})
#define copy(_val... /*(TypeOf(_val))*/) comp_syn__copy(_val)
#define comp_syn__copy(_val...) \
    (*&*((TypeOf(_val)[1]){ [0] = _val }))

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
