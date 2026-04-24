/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dal-project/da/builtin
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

/*========== Macros and Declarations ========================================*/

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

#define $inline __attr__$inline
#define $inline_always __attr__$inline_always
#define $inline_never __attr__$inline_never
#define $flatten __attr__$flatten

#define $pure __attr__$pure
#define $view __attr__$view

#define $deprecated __attr__$deprecated
#define $deprecated_msg(_Msg) __attr__$deprecated_msg(_Msg)
#define $deprecated_instead(_Msg, _Replacement) __attr__$deprecated_instead(_Msg, _Replacement)

#define $on_load __attr__$on_load
#define $on_exit __attr__$on_exit

#define $must_check \
    /** \
     * @brief Attribute marks a function as returning a value that must be \
     *        checked to avoid potential errors or warnings \
     * @details This attribute can be used to ensure that a function's return \
     *          value is checked to avoid potential errors or warnings \
     */ \
    __attr__$must_check
#define $no_return \
    /** \
     * @brief Attribute marks a function as not returning a value \
     * @details This attribute can be used to ensure that a function does \
     *          not return a value \
     */ \
    __attr__$no_return

#define $ignore_void \
    /** \
     * @brief Ignores expression or return value \
     */ \
    __oper__$ignore_void
#define $ignore \
    /** \
     * @brief Ignores payload capture \
     */ \
    __capt__$ignore
#define let_ignore \
    /** \
     * @brief Ignores expression or return value \
     */ \
    __stmt__let_ignore
#define $do_nothing \
    /** \
     * @brief Does nothing \
     */ \
    __stmt__$do_nothing

#define $used(_Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as used to suppress compiler warnings \
     * @details In macro functions, the arguments are marked as used \
     * @param _Expr... Variable number of arguments to be marked as used \
     */ \
    __attr__$used(_Expr)
#define $unused(_Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as unused to suppress compiler warnings \
     * @param _Expr... Variable number of arguments to be marked as unused \
     */ \
    __attr__$unused(_Expr)

#define $keep_symbol __attr__$keep_symbol
#define $maybe_unused __attr__$maybe_unused
#define $must_use __attr__$must_use

#define $import __attr__$import
#define $export __attr__$export

#define as$(_TDest... /*)(_src...) -> (_TDest*/) \
    /** \
     * @brief Cast macro for converting a value to a different type \
     * @details This macro is used to cast a value from one type to another \
     * @param _TDest The destination type to cast to \
     * @param _src The value to cast \
     * @return The casted value \
     */ \
    __expr__as$(_TDest)
#define __expr__as$(_TDest...) __expr__as$__emit(_TDest) __expr__as$__paramSrc
#define __expr__as$__paramSrc(_src...) (_src)
#if defined(__cplusplus)
#define __expr__as$__emit(_TDest...) static_cast<_TDest>
#else
#define __expr__as$__emit(_TDest...) (_TDest)
#endif

#define l$(/*(_T){_initial...}*/... /*(_T)*/) __val__l$(__VA_ARGS__)
#if defined(__cplusplus)
#define __val__l$(...) (__val__l$__expandT __VA_ARGS__)
#define __val__l$__expandT(_T...) _T
#else
#define __val__l$(...) (__VA_ARGS__)
#endif

#define l0$(/*(_T)*/... /*(_T)*/) __val__l0$(__step__val__l0$__parse __VA_ARGS__)
#define __step__val__l0$__parse(_T...) _T
#define __val__l0$(_T...) (l$((struct { _T val; }){}).val)

#define comp_const_(_type, _initial...) l$((_type)_initial)

// n_(11,644,473,600u) => 11644473600u
#define n_(_Comma_Sep_Lits...) __val__n_(_Comma_Sep_Lits)
#define n$(_T) (_T) __val__n$__parseLits

#define __val__n_(_Comma_Sep_Lits...) pp_join(_, __val__n_, pp_countArg(_Comma_Sep_Lits))(_Comma_Sep_Lits)
#define __val__n__1(_Num1) _Num1
#define __val__n__2(_Num1, _Num2) pp_cat(_Num1, _Num2)
#define __val__n__3(_Num1, _Num2, _Num3) pp_cat3(_Num1, _Num2, _Num3)
#define __val__n__4(_Num1, _Num2, _Num3, _Num4) pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4)
#define __val__n__5(_Num1, _Num2, _Num3, _Num4, _Num5) pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4), _Num5)
#define __val__n__6(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6) pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6)
#define __val__n__7(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6), _Num7)
#define __val__n__8(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7, _Num8) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), pp_cat(_Num6, _Num7)), _Num8)
#define __val__n$__parseLits(...) (n_(__VA_ARGS__))

#define from$(/*(_T){_initial...}*/... /*(_T)*/) __val__from$(__VA_ARGS__)
#define __val__from$(...) __step__from$__emit(__step__from$__parse __VA_ARGS__)
#define __step__from$__parse(_T...) _T,
#define __step__from$__emit(...) ____from$(__VA_ARGS__)
#define ____from$(_T, _initial...) (*l$((_T[1]){ [0] = _initial }))

#define type$ typeV$

#define typeV$(/*(_T)(_raw...)*/... /*(_T)*/) \
    /* TODO: Add type checking */ \
    __typeV$__step(pp_defer(__typeV$__emit)(__typeV$__sep __VA_ARGS__))
#define __typeV$__step(...) __VA_ARGS__
#define __typeV$__sep(_T...) _T, __typeV$__sepRaw
#define __typeV$__sepRaw(_raw...) _raw
#define __typeV$__emit(_T, _raw...) __typeV$__emitNext(_T, _raw)
#define __typeV$__emitNext(_T, _raw...) (*(_T*)raw_memcpy(&l0$((_T)), &copy(_raw), sizeOf$(_T)))

#define typeO$(/*(_T)(_raw...)*/... /*(_T)*/) __typeO$__step(pp_defer(__typeO$__emit)(__typeO$__sep __VA_ARGS__))
#define __typeO$__step(...) __VA_ARGS__
#define __typeO$__sep(_O_T...) _O_T, __typeO$__sepRaw
#define __typeO$__sepRaw(_raw...) _raw
#define __typeO$__emit(_O_T, _raw...) __typeO$__emitNext(_O_T, _raw)
#define __typeO$__emitNext(_O_T, _raw...) (*(_O_T*)raw_memcpy(&l$((_O_T){}), (_raw).as_raw, sizeOf$(_O_T)))

#define typeE$(/*(_T)(_raw...)*/... /*(_T)*/) __typeE$__step(pp_defer(__typeE$__emit)(__typeE$__sep __VA_ARGS__))
#define __typeE$__step(...) __VA_ARGS__
#define __typeE$__sep(_E_T...) _E_T, __typeE$__sepRaw
#define __typeE$__sepRaw(_raw...) _raw
#define __typeE$__emit(_E_T, _raw...) __typeE$__emitNext(_E_T, _raw)
#define __typeE$__emitNext(_E_T, _raw...) (*(_E_T*)raw_memcpy(&l$((_E_T){}), (_raw).as_raw, sizeOf$(_E_T)))

#define asg(_p_lhs, _rhs, _fields...) pp_overload(__asg, _fields)(_p_lhs, _rhs, _fields)
#define __asg_0(_p_lhs, _rhs, ...) __op__asg(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define __op__asg(__p_lhs, __rhs, _p_lhs, _rhs, ...) ({ \
    let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_nonnull(__p_lhs); \
    claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(eqlType$(TypeOfUnqual(*__p_lhs), TypeOfUnqual(__rhs))); \
    *_p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
    __p_lhs; \
})
#define __asg_1(_p_lhs, _rhs, _fields...) __op__asg_compat(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs, __asg_1__expandFields _fields)
#define __asg_1__expandFields(_fields...) _fields
#define __op__asg_compat(__p_lhs, __rhs, _p_lhs, _rhs, _fields...) ({ \
    let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_nonnull(__p_lhs); \
    claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    pp_foreach(__op__asg_compat__assert, (TypeOfUnqual(*__p_lhs), TypeOfUnqual(__rhs)), _fields); \
    *_p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
    __p_lhs; \
})
#define __op__asg_compat__assert(...) __op__asg_compat____assert(__op__asg_compat____assert__parse __VA_ARGS__)
#define __op__asg_compat____assert__parse(...) __VA_ARGS__,
#define __op__asg_compat____assert(...) __op__asg_compat____assert__emit(__VA_ARGS__)
#define __op__asg_compat____assert__emit(_lhs, _rhs, _field...) claim_assert_static(eqlType$(FieldType$(_lhs _field), FieldType$(_rhs _field)));

#define asg_l(/*(p_lhs: P(T))(_rhs: T)*/... /*(P(T))*/) __op__asg_l(__op__asg_l__parsePLhs __VA_ARGS__)
#define __op__asg_l(...) __op__asg_l__emit(__VA_ARGS__)
#define __op__asg_l__parsePLhs(_p_lhs...) pp_uniqTok(p_lhs), _p_lhs, __op__asg_l__expandRhs
#define __op__asg_l__expandRhs(_rhs...) _rhs
#define __op__asg_l__emit(__p_lhs, _p_lhs, _rhs...) ({ \
    let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
    typedef TypeOf(*__p_lhs) LitType; \
    claim_assert_nonnull(__p_lhs); \
    *__p_lhs = from$((LitType)_rhs); \
    __p_lhs; \
})

#define $init(/*(_T){_initial...}*/... /*(_T)*/) __op__$init(__op__$init__parseT __VA_ARGS__)
#define __op__$init(...) __op__$init__emit(__VA_ARGS__)
#define __op__$init__parseT(_T...) _T,
#define __op__$init__emit(_T, _initial...) ({ \
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

#define cleared$ l0$
#define cleared() ____cleared()
#define ____cleared() \
    {}
#define initial$(/*(_T)(_expr...)*/...) __step__initial$(__VA_ARGS__)
#define __step__initial$(...) __step__initial$__emit(__step__initial$__parse __VA_ARGS__)
#define __step__initial$__parse(_T...) _T, __step__initial$__parseExpr
#define __step__initial$__parseExpr(_expr...) _expr
#define __step__initial$__emit(...) ____initial$(__VA_ARGS__)
#define ____initial$(_T, _expr...) l$((_T){ _expr })
#define initial(/*_expr...*/...) ____initial(__VA_ARGS__)
#define ____initial(_expr...) \
    { _expr }

#define move(_p_val... /*(TypeOf(*_p_val))*/) ____move(_p_val)
#define ____move(_p_val...) ({ \
    let_(__p_val, TypeOf(_p_val)) = _p_val; \
    let_(__val, TypeOfUnqual(*__p_val)) = *__p_val; \
    *__p_val = l0$((TypeOf(__val))); \
    __val; \
})
#define copy(_val... /*(TypeOf(_val))*/) ____copy(_val)
#define ____copy(_val...) (*&*((TypeOfUnqual(_val)[1]){ [0] = _val }))

#define with_(/*(_expr: _T)(_initial...: (_field)(_asg)*/... /*(_T)*/) __step__with_(__VA_ARGS__)
#define __step__with_(...) __step__with$__emit(__step__with$__parseExpr __VA_ARGS__)
#define __step__with$__parseExpr(_expr...) pp_uniqTok(expr_copied), _expr, __step__with$__parseInitial
#define __step__with$__parseInitial(_initial...) _initial
#define __step__with$__emit(...) \
    ____with_(__VA_ARGS__)
#define ____with_(__expr_copied, _expr, _initial...) local_({ \
    var __expr_copied = _expr; \
    pp_foreach(____with___each, __expr_copied, _initial); \
    local_return_(__expr_copied); \
})
#define ____with___each(__expr_copied, /*_initial*/...) __VA_OPT__( \
    ____with___each__emit(__expr_copied, ____with___each__parseField __VA_ARGS__) \
)
#define ____with___each__parseField(_field...) _field, ____with___each__parseAsg
#define ____with___each__parseAsg(_asg...) _asg
#define ____with___each__emit(...) \
    ____with_____each(__VA_ARGS__)
#define ____with_____each(__expr_copied, _field, _asg...) \
    asg_l((&__expr_copied _field)(_asg));

#define T_switch$(/*(_T_Cond)(_T_Cases...)*/...) \
    __step__T_switch$(__step__T_switch$__parseTCond __VA_ARGS__)
#define __step__T_switch$(...) __T_switch$(__VA_ARGS__)
#define __step__T_switch$__parseTCond(_T_Cond...) _T_Cond, __step__T_switch$__parseTCases
#define __step__T_switch$__parseTCases(_T_Cases...) _T_Cases
#define __T_switch$(_T_Cond, _T_Cases...) _Generic(null$($P$(_T_Cond)), _T_Cases)
#define T_case$(/*(_T_Case)_expr...*/...) \
    __step__T_case$(__step__T_case$__parseTCase __VA_ARGS__)
#define __step__T_case$(...) __T_case$(__VA_ARGS__)
#define __step__T_case$__parseTCase(_T_Case...) _T_Case,
#define __T_case$(_T_Case, _expr...) /* clang-format off */ \
    $P_const$(_T_Case): \
        _expr, \
    $P$(_T_Case): \
        _expr /* clang-format on */
#define T_qual$(/*(_T_Case)_expr...*/...) \
    __step__T_qual$(__step__T_qual$__parseTCase __VA_ARGS__)
#define __step__T_qual$(...) __T_qual$(__VA_ARGS__)
#define __step__T_qual$__parseTCase(_T_Case...) _T_Case,
#define __T_qual$(_T_Case, _expr...) /* clang-format off */ \
    $P$(_T_Case): \
        _expr /* clang-format on */
#define T_default_(_expr...) \
    default: \
        _expr

#if UNUSED_CODE
#define T_case$(_T_Case...) $P$(_T_Case)
#define T_default_() default
#endif /* UNUSED_CODE */

#define local_ comp_syn__local_
#define local_label comp_syn__local_label
#define local_return_(...) comp_syn__local_return_(__VA_ARGS__)

#define $fallthrough __attr__$fallthrough
#define $branch_hot __attr__$branch_hot
#define $branch_cold __attr__$branch_cold
#define $branch_predict_at(_prob /*: FltType*/, _expr... /*(bool)*/) __attr__$branch_predict_at(_prob, _expr)
#define $branch_unpredict_at(_prob /*: FltType*/, _expr... /*(bool)*/) __attr__$branch_predict_at(_prob, _expr)
#define $branch_likely(_expr... /*(bool)*/) __attr__$branch_likely(_expr)
#define $branch_unlikely(_expr... /*(bool)*/) __attr__$branch_unlikely(_expr)
#define $branch_unpredictable(_expr... /*(bool)*/) __attr__$branch_unpredictable(_expr)
#define $unreachable __attr__$unreachable

#define $zero_sized __attr__$zero_sized
#define $flexible __attr__$flexible
#define $like_ref __attr__$like_ref
#define $like_deref __attr__$like_deref

#define $static static
#define $extern extern
#define $thrd_local _Thread_local

#define $cdecl __attr__$cdecl
#define $stdcall __attr__$stdcall
#define $fastcall __attr__$fastcall
#define $vectorcall __attr__$vectorcall

#define $packed __attr__$packed
#define $bits(_width... /*8|16|32|64*/) __attr__$bits(_width)
#define $align(_align...) __attr__$align(_align)

#define not !
#define and &&
#define or ||

#define $pragma_guard_(_push, _ctx, _pop, _code...) /* clang-format off */ \
    _Pragma(_push) \
    _Pragma(_ctx) \
    _code \
    _Pragma(_pop) /* clang-format on */
#define $supress_microsoft_anon_tag(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wmicrosoft-anon-tag\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_compound_token_split_by_macro(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_unterminated_string_initialization(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wunterminated-string-initialization\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_implicit_int_conversion(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wimplicit-int-conversion\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_cast_qual(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcast-qual\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_cast_align(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcast-align\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_pointer_arith(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wpointer-arith\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_return_stack_address(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wreturn-stack-address\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_switch_enum(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wswitch-enum\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_infinite_recursion(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Winfinite-recursion\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_loop_analysis(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wloop-analysis\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $supress_thread_safety(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wthread-safety\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
/*========== Macros and Definitions =========================================*/

#define __attr__$inline comp_inline
#define __attr__$inline_always comp_inline_always
#define __attr__$inline_never comp_inline_never
#define __attr__$flatten comp_flatten

#define __attr__$deprecated comp_deprecated
#define __attr__$deprecated_msg(_Msg) comp_deprecated_msg(_Msg)
#define __attr__$deprecated_instead(_Msg, _Replacement) comp_deprecated_instead(_Msg, _Replacement)

#define __attr__$pure comp_pure
#define __attr__$view comp_view

#define __attr__$on_load comp_on_load
#define __attr__$on_exit comp_on_exit

#define __attr__$must_check comp_must_use
#define __attr__$no_return comp_no_return
#define __oper__$ignore_void (void)
#define __capt__$ignore \
    $maybe_unused pp_uniqTok(ignored) = l0$((Void)); \
    let_ignore
#define __stmt__let_ignore $maybe_unused let pp_uniqTok(ignored)
#define __stmt__$do_nothing \
    {}

#define __attr__$used(_Expr...) _Expr
/* begin unused */
#define __attr__$unused(_Expr...) __attr__$unused__payload(__attr__$unused__countArgs(_Expr), _Expr)
#define __attr__$unused__payload(_Count, _Args...) pp_join(__, __attr__$unused, _Count)(_Args)
#define __attr__$unused__countArgs(_Args...) __attr__$unused__countArgs__select(_Args, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __attr__$unused__countArgs__select(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ...) _N
#define __attr__$unused__1(x1) (void)(x1)
#define __attr__$unused__2(x1, x2) (void)(x1), (void)(x2)
#define __attr__$unused__3(x1, x2, x3) (void)(x1), (void)(x2), (void)(x3)
#define __attr__$unused__4(x1, x2, x3, x4) (void)(x1), (void)(x2), (void)(x3), (void)(x4)
#define __attr__$unused__5(x1, x2, x3, x4, x5) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5)
#define __attr__$unused__6(x1, x2, x3, x4, x5, x6) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6)
#define __attr__$unused__7(x1, x2, x3, x4, x5, x6, x7) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7)
#define __attr__$unused__8(x1, x2, x3, x4, x5, x6, x7, x8) (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8)
#define __attr__$unused__9(x1, x2, x3, x4, x5, x6, x7, x8, x9) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9)
#define __attr__$unused__10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10)
#define __attr__$unused__11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11)
#define __attr__$unused__12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12)
#define __attr__$unused__13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13)
#define __attr__$unused__14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14)
#define __attr__$unused__15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14), (void)(x15)
#define __attr__$unused__16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) \
    (void)(x1), (void)(x2), (void)(x3), (void)(x4), (void)(x5), (void)(x6), (void)(x7), (void)(x8), (void)(x9), (void)(x10), (void)(x11), (void)(x12), \
        (void)(x13), (void)(x14), (void)(x15), (void)(x16)
/* end unused */

#define __attr__$keep_symbol comp_keep_symbol
#define __attr__$maybe_unused comp_maybe_unused
#define __attr__$must_use comp_must_use

#define __attr__$import comp_import
#define __attr__$export comp_export

#define comp_syn__local_ /* just comment for compound statement expression ({...}) */
#define comp_syn__local_label __label__
#define comp_syn__local_return_(...) __VA_ARGS__

#define __attr__$fallthrough comp_fallthrough
#define __attr__$branch_hot comp_branch_hot
#define __attr__$branch_cold comp_branch_cold
#define __attr__$branch_predict_at(_prob /*: FltType*/, _expr... /*(bool)*/) comp_branch_predict_at(_prob, _expr)
#define __attr__$branch_likely(_expr... /*(bool)*/) comp_branch_likely(_expr)
#define __attr__$branch_unlikely(_expr... /*(bool)*/) comp_branch_unlikely(_expr)
#define __attr__$branch_unpredictable(_expr... /*(bool)*/) comp_branch_unpredictable(_expr)
#define __attr__$unreachable comp_unreachable

#define __attr__$zero_sized [0]
#define __attr__$flexible [0]
#define __attr__$like_ref [1]
#define __attr__$like_deref [0]

#define __attr__$cdecl plat_cdecl
#define __attr__$stdcall plat_stdcall
#define __attr__$fastcall plat_fastcall
#define __attr__$vectorcall plat_vectorcall

#define __attr__$packed comp_packed
#define __attr__$bits(_width... /*8|16|32|64*/) pp_join(_, __attr__$bits, _width)
#define __attr__$bits_0 "0 bits is not allowed"
#define __attr__$bits_8 \
    /* mode: __QI__ | meaning: Quarter Integer  | size: 1 byte  | corresponding type: "u8, i8"*/ \
    __attribute__((__mode__(__QI__)))
#define __attr__$bits_16 \
    /* mode: __HI__ | meaning: Half Integer     | size: 2 bytes | corresponding type: "u16, i16"*/ \
    __attribute__((__mode__(__HI__)))
#define __attr__$bits_32 \
    /* mode: __SI__ | meaning: Standard Integer | size: 4 bytes | corresponding type: "u32, i32"*/ \
    __attribute__((__mode__(__SI__)))
#define __attr__$bits_64 \
    /* mode: __DI__ | meaning: Double Integer   | size: 8 bytes | corresponding type: "u64, i64"*/ \
    __attribute__((__mode__(__DI__)))
#define __attr__$align(_align...) comp_align(_align)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_comp__included */
