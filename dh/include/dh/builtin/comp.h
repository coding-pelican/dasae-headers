/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 *
 * @brief   Compiler-specific configurations and optimizations
 * @details Provides compiler-specific inline directives and optimizations based on detected environment
 */
#ifndef builtin_comp__included
#define builtin_comp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "lang_cfg.h"
#include "arch_cfg.h"
#include "plat_cfg.h"
#include "comp_cfg.h"

/*========== Macros and Definitions =========================================*/

#define on_comptime__default __comp_flag__on_comptime__default
#define on_comptime __comp_bool__on_comptime
#define comptime_comp_enabled __comp_bool__comptime_comp_enabled
#define comptime_only(_inner...) __comp_syn__comptime_only(_inner)

#define __comp_flag__on_comptime__default 0
#define __comp_bool__on_comptime on_comptime__default
#define __comp_bool__comptime_comp_enabled on_comptime
#define __comp_syn__comptime_only(_inner...) pp_if_(on_comptime)(pp_then_(_inner), pp_else_())

#if defined(COMP)
#undef __comp_flag__on_comptime__default
#define __comp_flag__on_comptime__default 1
#endif /* defined(COMP) */

#define $dispatch_on_comptime /* just comment that dispatches to detailed implementation at compile-time */

#define $attr(_Attrs...) _Attrs

#define $inline comp_attr__$inline
#define $inline_always comp_attr__$inline_always
#define $inline_never comp_attr__$inline_never

#define $deprecated comp_attr__$deprecated
#define $deprecated_msg(_Msg) comp_attr__$deprecated_msg(_Msg)
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

#define $keep_symbol comp_attr__$keep_symbol
#define $maybe_unused comp_attr__$maybe_unused
#define $must_use comp_attr__$must_use

#define as$(_TDest... /*)(_src...) -> (_TDest)(*/) \
    /** \
     * @brief Cast macro for converting a value to a different type \
     * @details This macro is used to cast a value from one type to another \
     * @param _TDest The destination type to cast to \
     * @param _src The value to cast \
     * @return The casted value \
     */ \
    __syn__as$(_TDest)
#define __syn__as$(_TDest...) __syn__as$__emit(_TDest) __syn__as$__paramSrc
#define __syn__as$__paramSrc(_src...) (_src)
#if defined(__cplusplus)
#define __syn__as$__emit(_TDest...) static_cast<_TDest>
#else
#define __syn__as$__emit(_TDest...) (_TDest)
#endif

#define lit$(/*(_T){_initial...}*/... /*(_T)*/) __lit$__emit(__VA_ARGS__)
#if defined(__cplusplus)
#define __lit$__emit(...) (__lit$__expandT __VA_ARGS__)
#define __lit$__expandT(_T...) _T
#else
#define __lit$__emit(...) (__VA_ARGS__)
#endif

#if UNUSED_CODE
#define lit$(/*(_T){_initial...}*/... /*(_T)*/) __lit$__step(pp_defer(__lit$__emit)(__lit$__sep __VA_ARGS__))
#define __lit$__step(...) __VA_ARGS__
#define __lit$__sep(_T...) _T,
#define __lit$__emit(_T, _initial...) __lit$__emitNext(_T, (_initial))
#if defined(__cplusplus)
#define __lit$__emitNext(_T, _initial...) (_T __lit$__expandInitial _initial)
#else
#define __lit$__emitNext(_T, _initial...) ((_T)__lit$__expandInitial _initial)
#endif
#define __lit$__expandInitial(_initial...) _initial
#endif /* UNUSED_CODE */

#define comp_const_(_type, _initial...) lit$((_type)_initial)

#define lit_n$(_T) (_T) __op__lit_n$__parseLits
#define __op__lit_n$__parseLits(...) { lit_num(__VA_ARGS__) }

// lit_num(11,644,473,600u) => 11644473600u
#define lit_num(_Comma_Sep_Lits...) __op__lit_num(_Comma_Sep_Lits)
#define lit_num$(/*(_T)(_Comma_Sep_Lits...)*/... /*(_T)*/) __op__lit_num$(__op__lit_num$__parseT __VA_ARGS__)

#define __op__lit_num(_Comma_Sep_Lits...) pp_join(__, __op__lit_num, pp_countArg(_Comma_Sep_Lits))(_Comma_Sep_Lits)
/* Handle different numbers of arguments */
#define __op__lit_num__1(_Num1) _Num1
#define __op__lit_num__2(_Num1, _Num2) pp_cat(_Num1, _Num2)
#define __op__lit_num__3(_Num1, _Num2, _Num3) pp_cat3(_Num1, _Num2, _Num3)
#define __op__lit_num__4(_Num1, _Num2, _Num3, _Num4) pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4)
#define __op__lit_num__5(_Num1, _Num2, _Num3, _Num4, _Num5) pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4), _Num5)
#define __op__lit_num__6(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6) pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6)
#define __op__lit_num__7(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6), _Num7)
#define __op__lit_num__8(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7, _Num8) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), pp_cat(_Num6, _Num7)), _Num8)

#define __op__lit_num$(...) __op__lit_num$__emit(__VA_ARGS__)
#define __op__lit_num$__parseT(_T...) _T,
#define __op__lit_num$__emit(_T, _Comma_Sep_Lits...) lit$((_T){ lit_num _Comma_Sep_Lits })

#define make$(/*(_T){_initial...}*/... /*(_T)*/) __make$__step(pp_defer(__make$__emit)(__make$__sep __VA_ARGS__))
#define __make$__step(...) __VA_ARGS__
#define __make$__sep(_T...) _T,
#define __make$__emit(_T, _initial...) __make$__emitNext(_T, (_initial))
#define __make$__emitNext(_T, _initial...) (*lit$((_T[1]){ [0] = __make$__expandInitial _initial }))
#define __make$__expandInitial(_initial...) _initial

#define create$(/*(_T){_initial...}*/... /*(P$$(_T))*/) __create$__step(pp_defer(__create$__emit)(__create$__sep __VA_ARGS__))
#define __create$__step(...) __VA_ARGS__
#define __create$__sep(_T...) _T,
#define __create$__emit(_T, _initial...) __create$__emitNext(_T, (_initial))
#define __create$__emitNext(_T, _initial...) (&make$((_T)__create$__expandInitial _initial))
#define __create$__expandInitial(_initial...) _initial

#define type$ typeV$

#define typeV$(/*(_T)(_raw...)*/... /*(_T)*/) \
    /* TODO: Add type checking */ \
    __typeV$__step(pp_defer(__typeV$__emit)(__typeV$__sep __VA_ARGS__))
#define __typeV$__step(...) __VA_ARGS__
#define __typeV$__sep(_T...) _T, __typeV$__sepRaw
#define __typeV$__sepRaw(_raw...) _raw
#define __typeV$__emit(_T, _raw...) __typeV$__emitNext(_T, _raw)
#define __typeV$__emitNext(_T, _raw...) (*(_T*)prim_memcpy(&lit$((_T){}), &copy(_raw), sizeOf$(_T)))

#define typeO$(/*(_T)(_raw...)*/... /*(_T)*/) __typeO$__step(pp_defer(__typeO$__emit)(__typeO$__sep __VA_ARGS__))
#define __typeO$__step(...) __VA_ARGS__
#define __typeO$__sep(_O_T...) _O_T, __typeO$__sepRaw
#define __typeO$__sepRaw(_raw...) _raw
#define __typeO$__emit(_O_T, _raw...) __typeO$__emitNext(_O_T, _raw)
#define __typeO$__emitNext(_O_T, _raw...) (*(_O_T*)prim_memcpy(&lit$((_O_T){}), (_raw).as_raw, sizeOf$(_O_T)))

#define typeE$(/*(_T)(_raw...)*/... /*(_T)*/) __typeE$__step(pp_defer(__typeE$__emit)(__typeE$__sep __VA_ARGS__))
#define __typeE$__step(...) __VA_ARGS__
#define __typeE$__sep(_E_T...) _E_T, __typeE$__sepRaw
#define __typeE$__sepRaw(_raw...) _raw
#define __typeE$__emit(_E_T, _raw...) __typeE$__emitNext(_E_T, _raw)
#define __typeE$__emitNext(_E_T, _raw...) (*(_E_T*)prim_memcpy(&lit$((_E_T){}), (_raw).as_raw, sizeOf$(_E_T)))

#define asg(_p_lhs, _rhs, _fields...) pp_overload(__asg, _fields)(_p_lhs, _rhs, _fields)
#define __asg_0(_p_lhs, _rhs, ...) __op__asg(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define __op__asg(__p_lhs, __rhs, _p_lhs, _rhs, ...) \
    ({ \
        let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
        let_(__rhs, TypeOf(_rhs)) = _rhs; \
        claim_assert_nonnull(__p_lhs); \
        claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
        claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
        claim_assert_static(isSameType$(TypeOf(*__p_lhs), TypeOf(__rhs))); \
        *_p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
        __p_lhs; \
    })
#define __asg_1(_p_lhs, _rhs, _fields...) __op__asg_compat(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs, __asg_1__expandFields _fields)
#define __asg_1__expandFields(_fields...) _fields
#define __op__asg_compat(__p_lhs, __rhs, _p_lhs, _rhs, _fields...) \
    ({ \
        let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
        let_(__rhs, TypeOf(_rhs)) = _rhs; \
        claim_assert_nonnull(__p_lhs); \
        claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
        claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
        pp_foreach(__op__asg_compat__assert, (TypeOf(*__p_lhs), TypeOf(__rhs)), _fields); \
        *_p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
        __p_lhs; \
    })
#define __op__asg_compat__assert(...) __op__asg_compat____assert(__op__asg_compat____assert__parse __VA_ARGS__)
#define __op__asg_compat____assert__parse(...) __VA_ARGS__,
#define __op__asg_compat____assert(...) __op__asg_compat____assert__emit(__VA_ARGS__)
#define __op__asg_compat____assert__emit(_lhs, _rhs, _field...) claim_assert_static(isSameType$(FieldType$(_lhs _field), FieldType$(_rhs _field)));

#define asg_lit(/*(p_lhs: P(T))(_rhs: T)*/... /*(P(T))*/) __op__asg_lit(__op__asg_lit__parsePLhs __VA_ARGS__)
#define __op__asg_lit(...) __op__asg_lit__emit(__VA_ARGS__)
#define __op__asg_lit__parsePLhs(_p_lhs...) pp_uniqTok(p_lhs), _p_lhs, __op__asg_lit__expandRhs
#define __op__asg_lit__expandRhs(_rhs...) _rhs
#define __op__asg_lit__emit(__p_lhs, _p_lhs, _rhs...) \
    ({ \
        let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
        typedef TypeOf(*__p_lhs) LitType; \
        claim_assert_nonnull(__p_lhs); \
        *__p_lhs = make$((LitType)_rhs); \
        __p_lhs; \
    })

#define $init(/*(_T){_initial...}*/... /*(_T)*/) __op__$init(__op__$init__parseT __VA_ARGS__)
#define __op__$init(...) __op__$init__emit(__VA_ARGS__)
#define __op__$init__parseT(_T...) _T,
#define __op__$init__emit(_T, _initial...) \
    ({ \
        $maybe_unused typedef _T InitType; \
        (InitType) _initial; \
    })

#define $asg(_val...) , $_asg, (_val)
#define $field(...) __op__$field(__VA_ARGS__)
#define __op__$field(...) pp_overload(__op__$field, __VA_ARGS__)(__VA_ARGS__)
#define __op__$field_1(...) __op__$field_1__emit(__VA_ARGS__)
#define __op__$field_1__emit(_field_val...) \
    .__op__$field__expand _field_val = ({ \
        $maybe_unused typedef InitType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _field_val) FieldType; \
        _field_val; \
    })
#define __op__$field_3(...) __op__$field_3__emit(__VA_ARGS__)
#define __op__$field_3__emit(_field, _op, _val...) \
    .__op__$field__expand _field = ({ \
        $maybe_unused typedef InitType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _field) FieldType; \
        _val; \
    })
#define __op__$field__expand(...) __VA_ARGS__
#define $in_field(...) __op__$in_field__emit(__VA_ARGS__)
#define __op__$in_field(...) pp_overload(__op__$in_field, __VA_ARGS__)(__VA_ARGS__)
#define __op__$in_field_1(...) __op__$in_field_1__emit(__VA_ARGS__)
#define __op__$in_field_1__emit(_field_val...) \
    .__op__$in_field__expand _field_val = ({ \
        $maybe_unused typedef FieldType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _field_val) FieldType; \
        _field_val; \
    })
#define __op__$in_field_3(...) __op__$in_field_3__emit(__VA_ARGS__)
#define __op__$in_field_3__emit(_field, _op, _val...) \
    .__op__$in_field__expand _field = ({ \
        $maybe_unused typedef FieldType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _field) FieldType; \
        _val; \
    })
#define __op__$in_field__expand(...) __VA_ARGS__

#define cleared() comp_syn__cleared()
#define comp_syn__cleared() \
    {}

#define move(_p_val... /*(TypeOf(*_p_val))*/) comp_syn__move(_p_val)
#define comp_syn__move(_p_val...) \
    ({ \
        let_(__p_val, TypeOfUnqual(_p_val)) = _p_val; \
        let_(__val, TypeOfUnqual(*__p_val)) = *__p_val; \
        *__p_val = ((TypeOfUnqual(__val)){}); \
        __val; \
    })
#define copy(_val... /*(TypeOf(_val))*/) comp_syn__copy(_val)
#define comp_syn__copy(_val...) (*&*((TypeOfUnqual(_val)[1]){ [0] = _val }))

#define Generic_match$(T, _Pattern...) comp_syn__Generic_match$(T, _Pattern)
#define Generic_pattern$(T) comp_syn__Generic_pattern$(T)
#define Generic_fallback_ comp_syn__Generic_fallback_

#define blk comp_syn__blk
#define blk_return comp_syn__blk_return
#define blk_return_(...) comp_syn__blk_return_(__VA_ARGS__)

#define $local_label comp_syn__$local_label
#define likely(_Expr... /*bool*/) comp_syn__likely(_Expr)
#define unlikely(_Expr... /*bool*/) comp_syn__unlikely(_Expr)

#define $like_ref __attr__$like_ref
#define $like_deref __attr__$like_deref
#define $flexible __attr__$flexible
#define $zero_sized __attr__$zero_sized

#define pragma_guard_(_push, _ctx, _pop, _code...) _Pragma(_push) _Pragma(_ctx) _code _Pragma(_pop)

#define unreachable __comp_syn__unreachable

#define $static static
#define $extern extern
#define $Thrd_local _Thread_local

/*========== Macros and Implementations =====================================*/

#define comp_attr__$inline comp_inline
#define comp_attr__$inline_always comp_inline_always
#define comp_attr__$inline_never comp_inline_never

#define comp_attr__$deprecated comp_deprecated
#define comp_attr__$deprecated_msg(_Msg) comp_deprecated_msg(_Msg)
#define comp_attr__$deprecated_instead(_Msg, _Replacement) comp_deprecated_instead(_Msg, _Replacement)

#define comp_attr__$on_load comp_on_load
#define comp_attr__$on_exit comp_on_exit

#define comp_attr__$must_check comp_must_use
#define comp_attr__$no_return comp_noreturn
#define comp_attr__$ignore_void (void)
#define comp_attr__$ignore \
    $maybe_unused pp_uniqTok(ignored) = (Void){}; \
    let_ignore
#define comp_attr__let_ignore $maybe_unused let pp_uniqTok(ignored)


#define comp_attr__$used(_Expr...) _Expr
/* begin unused */
#define comp_attr__$unused(_Expr...) comp_attr__$unused__payload(comp_attr__$unused__countArgs(_Expr), _Expr)
#define comp_attr__$unused__payload(_Count, _Args...) pp_join(__, comp_attr__$unused, _Count)(_Args)
#define comp_attr__$unused__countArgs(_Args...) comp_attr__$unused__countArgs__select(_Args, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define comp_attr__$unused__countArgs__select(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ...) _N
#define comp_attr__$unused__1(x1) (void)(x1)
#define comp_attr__$unused__2(x1, x2) (void)(x1), (void)(x2)
#define comp_attr__$unused__3(x1, x2, x3) (void)(x1), (void)(x2), (void)(x3)
#define comp_attr__$unused__4(x1, x2, x3, x4) (void)(x1), (void)(x2), (void)(x3), (void)(x4)
#define comp_attr__$unused__5(x1, x2, x3, x4, x5) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5)
#define comp_attr__$unused__6(x1, x2, x3, x4, x5, x6) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6)
#define comp_attr__$unused__7(x1, x2, x3, x4, x5, x6, x7) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7)
#define comp_attr__$unused__8(x1, x2, x3, x4, x5, x6, x7, x8) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8)
#define comp_attr__$unused__9(x1, x2, x3, x4, x5, x6, x7, x8, x9) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9)
#define comp_attr__$unused__10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10)
#define comp_attr__$unused__11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11)
#define comp_attr__$unused__12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12)
#define comp_attr__$unused__13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13)
#define comp_attr__$unused__14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14)
#define comp_attr__$unused__15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14), (void)(x15)
#define comp_attr__$unused__16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14), (void)(x15), (void)(x16)
/* end unused */

#define comp_attr__$keep_symbol comp_keep_symbol
#define comp_attr__$maybe_unused comp_maybe_unused
#define comp_attr__$must_use comp_must_use

#define comp_syn__Generic_match$(T, _Pattern...) _Generic(T, _Pattern)
#define comp_syn__Generic_pattern$(T) \
T:
#define comp_syn__Generic_fallback_ default:

#define comp_syn__blk        /* just comment for compound statement expression ({...}) */
#define comp_syn__blk_return /* just comment for compound statement expression ({...}) */
#define comp_syn__blk_return_(...) __VA_ARGS__

#define comp_syn__$local_label __label__
#define comp_syn__likely(_Expr...) __builtin_expect(!!(_Expr), 1)
#define comp_syn__unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)

#define __attr__$like_ref [1]
#define __attr__$like_deref [0]
#define __attr__$flexible [0]
#define __attr__$zero_sized [0]

#if defined(__GNUC__) || defined(__clang__)
#define __comp_syn__unreachable __builtin_unreachable()
#elif defined(_MSC_VER)
#define __comp_syn__unreachable __assume(0)
#else
/* TODO: Add support for other compilers */
#define __comp_syn__unreachable $unused(0)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_comp__included */
