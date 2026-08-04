/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dal-project/da/builtin
 * @prefix  (none)
 *
 * @brief   Compiler-specific configurations and optimizations
 * @details Provides compiler-specific inline directives and optimizations based on detected environment
 */
#pragma once
#ifndef builtin_comp__included
#define builtin_comp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

#define in_analysis eval_in_analysis
#define analysis_enabled eval_analysis_enabled
#define analysis_mode eval_analysis_mode
#define analysis_mode_active_only eval_analysis_mode_active_only
#define analysis_mode_full eval_analysis_mode_full
#define analysis_is_active_only eval_analysis_is_active_only
#define analysis_is_full eval_analysis_is_full
#define in_analysis_active_only eval_in_analysis_active_only
#define in_analysis_full eval_in_analysis_full
#define analysis_only eval_analysis_only
#define analysis_unless eval_analysis_unless

#define in_comptime eval_in_comptime
#define comptime_enabled eval_comptime_enabled
#define comptime_only eval_comptime_only
#define comptime_unless eval_comptime_unless

#define in_runtime eval_in_runtime
#define runtime_enabled eval_runtime_enabled
#define runtime_only eval_runtime_only
#define runtime_unless eval_runtime_unless

#define $dispatch_in_comptime /* just comment that dispatches to detailed implementation at compile-time */

#define $attr(_$Attrs...) _$Attrs

#define $inline __attr__$inline
#define $inline_always __attr__$inline_always
#define $inline_never __attr__$inline_never
#define $flatten __attr__$flatten

#define $pure __attr__$pure
#define $view __attr__$view

#define $deprecated __attr__$deprecated
#define $deprecated_msg(_$Msg) __attr__$deprecated_msg(_$Msg)
#define $deprecated_instead(_$Msg, _$Replacement) __attr__$deprecated_instead(_$Msg, _$Replacement)

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
#define $must_tail \
    /** \
     * @brief Attribute marks a function as a must tail function \
     * @details This attribute can be used to ensure that a function is a must tail function \
     */ \
    __attr__$must_tail

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
#define $noop \
    /** \
     * @brief Does nothing \
     */ \
    __stmt__$noop

#define $used(_$Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as used to suppress compiler warnings \
     * @details In macro functions, the arguments are marked as used \
     * @param _$Expr... Variable number of arguments to be marked as used \
     */ \
    __attr__$used(_$Expr)
#define $unused(_$Expr... /*void*/) \
    /** \
     * @brief Marks variables or expressions as unused to suppress compiler warnings \
     * @param _$Expr... Variable number of arguments to be marked as unused \
     */ \
    __attr__$unused(_$Expr)

#define $keep_symbol __attr__$keep_symbol
#define $maybe_unused __attr__$maybe_unused
#define $must_use __attr__$must_use
#define $undefined __attr__$undefined
#define $undefined_static __attr__$undefined_static

#define $import __attr__$import
#define $export __attr__$export

#define as$(_$TDest... /*)(_$src...) -> (_$TDest*/) \
    /** \
     * @brief Cast macro for converting a value to a different type \
     * @details This macro is used to cast a value from one type to another \
     * @param _$TDest The destination type to cast to \
     * @param _$src The value to cast \
     * @return The casted value \
     */ \
    __expr__as$(_$TDest)
#define __expr__as$(_$TDest...) __expr__as$__emit(_$TDest) __expr__as$__paramSrc
#define __expr__as$__paramSrc(_$src...) (_$src)
#if defined(__cplusplus)
#define __expr__as$__emit(_$TDest...) static_cast<_$TDest>
#else
#define __expr__as$__emit(_$TDest...) (_$TDest)
#endif

#define l$(/*(_$T){_$initial...}*/... /*(_$T)*/) __val__l$(__VA_ARGS__)
#if defined(__cplusplus)
#define __val__l$(...) (__val__l$__expandT __VA_ARGS__)
#define __val__l$__expandT(_$T...) _$T
#else
#define __val__l$(...) (__VA_ARGS__)
#endif

#define l0$(/*(_$T)*/... /*(_$T)*/) __val__l0$(__step__val__l0$__parse __VA_ARGS__)
#define __step__val__l0$__parse(_$T...) _$T
#define __val__l0$(_$T...) (l$((struct { _$T val; }){}).val)

#define comp_const_(_$type, _$initial...) l$((_$type)_$initial)

// n_(11,644,473,600u) => 11644473600u
#define n_(_$Comma_Sep_Lits...) __val__n_(_$Comma_Sep_Lits)
#define n$(/*(_$T)(_$Comma_Sep_Lits)*/... /*(_$T)*/) __val__n$(__VA_ARGS__)

#define __val__n_(_$Comma_Sep_Lits...) pp_join(_, __val__n_, pp_countArg(_$Comma_Sep_Lits))(_$Comma_Sep_Lits)
#define __val__n__1(_$Num1) _$Num1
#define __val__n__2(_$Num1, _$Num2) pp_cat(_$Num1, _$Num2)
#define __val__n__3(_$Num1, _$Num2, _$Num3) pp_cat3(_$Num1, _$Num2, _$Num3)
#define __val__n__4(_$Num1, _$Num2, _$Num3, _$Num4) pp_cat(pp_cat3(_$Num1, _$Num2, _$Num3), _$Num4)
#define __val__n__5(_$Num1, _$Num2, _$Num3, _$Num4, _$Num5) pp_cat(pp_cat(pp_cat3(_$Num1, _$Num2, _$Num3), _$Num4), _$Num5)
#define __val__n__6(_$Num1, _$Num2, _$Num3, _$Num4, _$Num5, _$Num6) pp_cat(pp_cat(pp_cat3(_$Num1, _$Num2, _$Num3), pp_cat(_$Num4, _$Num5)), _$Num6)
#define __val__n__7(_$Num1, _$Num2, _$Num3, _$Num4, _$Num5, _$Num6, _$Num7) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_$Num1, _$Num2, _$Num3), pp_cat(_$Num4, _$Num5)), _$Num6), _$Num7)
#define __val__n__8(_$Num1, _$Num2, _$Num3, _$Num4, _$Num5, _$Num6, _$Num7, _$Num8) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_$Num1, _$Num2, _$Num3), pp_cat(_$Num4, _$Num5)), pp_cat(_$Num6, _$Num7)), _$Num8)
#define __val__n$(...) __step__n$__emit(__step__n$__parse __VA_ARGS__)
#define __step__n$__parse(_$T...) _$T,
#define __step__n$__emit(...) ____n$(__VA_ARGS__)
#define ____n$(_$T, _$Comma_Sep_Lits...) (as$(_$T)(n_(_$Comma_Sep_Lits)))

#define from$(/*(_$T){_$initial...}*/... /*(_$T)*/) __val__from$(__VA_ARGS__)
#define __val__from$(...) __step__from$__emit(__step__from$__parse __VA_ARGS__)
#define __step__from$__parse(_$T...) _$T,
#define __step__from$__emit(...) ____from$(__VA_ARGS__)
#define ____from$(_$T, _$initial...) (*l$((_$T[1]){ [0] = _$initial }))

#define type$ typeV$

#define typeV$(/*(_$T)(_$raw...)*/... /*(_$T)*/) \
    /* TODO: Add type checking */ \
    __typeV$__step(pp_defer(__typeV$__emit)(__typeV$__sep __VA_ARGS__))
#define __typeV$__step(...) __VA_ARGS__
#define __typeV$__sep(_$T...) _$T, __typeV$__sepRaw
#define __typeV$__sepRaw(_$raw...) _$raw
#define __typeV$__emit(_$T, _$raw...) __typeV$__emitNext(_$T, _$raw)
#define __typeV$__emitNext(_$T, _$raw...) local_({ \
    typedef _$T __typeV__$T; \
    let __typeV__$raw = _$raw; \
    local_return_(*(__typeV__$T*)raw_memcpy( \
        &l0$((__typeV__$T)), \
        &__typeV__$raw, \
        sizeOf$(__typeV__$T) \
    )); \
})

#define typeO$(/*(_$T)(_$raw...)*/... /*(_$T)*/) __typeO$__step(pp_defer(__typeO$__emit)(__typeO$__sep __VA_ARGS__))
#define __typeO$__step(...) __VA_ARGS__
#define __typeO$__sep(_$O_T...) _$O_T, __typeO$__sepRaw
#define __typeO$__sepRaw(_$raw...) _$raw
#define __typeO$__emit(_$O_T, _$raw...) __typeO$__emitNext(_$O_T, _$raw)
#define __typeO$__emitNext(_$O_T, _$raw...) (*(_$O_T*)raw_memcpy(&l$((_$O_T){}), (_$raw).as_raw, sizeOf$(_$O_T)))

#define typeE$(/*(_$T)(_$raw...)*/... /*(_$T)*/) __typeE$__step(pp_defer(__typeE$__emit)(__typeE$__sep __VA_ARGS__))
#define __typeE$__step(...) __VA_ARGS__
#define __typeE$__sep(_$E_T...) _$E_T, __typeE$__sepRaw
#define __typeE$__sepRaw(_$raw...) _$raw
#define __typeE$__emit(_$E_T, _$raw...) __typeE$__emitNext(_$E_T, _$raw)
#define __typeE$__emitNext(_$E_T, _$raw...) (*(_$E_T*)raw_memcpy(&l$((_$E_T){}), (_$raw).as_raw, sizeOf$(_$E_T)))

#define asg(_$p_lhs, _$rhs, _$fields...) pp_overload(__asg, _$fields)(_$p_lhs, _$rhs, _$fields)
#define __asg_0(_$p_lhs, _$rhs, ...) __op__asg(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _$p_lhs, _$rhs)
#define __op__asg(__p_lhs, __rhs, _$p_lhs, _$rhs, ...) ({ \
    let_(__p_lhs, TypeOf(_$p_lhs)) = _$p_lhs; \
    let_(__rhs, TypeOf(_$rhs)) = _$rhs; \
    claim_assert_nonnull(__p_lhs); \
    claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(eqlType$(TypeOfUnqual(*__p_lhs), TypeOfUnqual(__rhs))); \
    *_$p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
    __p_lhs; \
})
#define __asg_1(_$p_lhs, _$rhs, _$fields...) __op__asg_compat(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _$p_lhs, _$rhs, __asg_1__expandFields _$fields)
#define __asg_1__expandFields(_$fields...) _$fields
#define __op__asg_compat(__p_lhs, __rhs, _$p_lhs, _$rhs, _$fields...) ({ \
    let_(__p_lhs, TypeOf(_$p_lhs)) = _$p_lhs; \
    let_(__rhs, TypeOf(_$rhs)) = _$rhs; \
    claim_assert_nonnull(__p_lhs); \
    claim_assert(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    pp_foreach(__op__asg_compat__assert, (TypeOfUnqual(*__p_lhs), TypeOfUnqual(__rhs)), _$fields); \
    *_$p_lhs = *as$(TypeOf(__p_lhs))(&__rhs); \
    __p_lhs; \
})
#define __op__asg_compat__assert(...) __op__asg_compat____assert(__op__asg_compat____assert__parse __VA_ARGS__)
#define __op__asg_compat____assert__parse(...) __VA_ARGS__,
#define __op__asg_compat____assert(...) __op__asg_compat____assert__emit(__VA_ARGS__)
#define __op__asg_compat____assert__emit(_lhs, _$rhs, _$field...) claim_assert_static(eqlType$(FieldType$(_lhs _$field), FieldType$(_$rhs _$field)));

#define asg_l(/*(_$p_lhs: P(T))(_$rhs: T)*/... /*(P(T))*/) __op__asg_l(__op__asg_l__parsePLhs __VA_ARGS__)
#define __op__asg_l(...) __op__asg_l__emit(__VA_ARGS__)
#define __op__asg_l__parsePLhs(_$p_lhs...) pp_uniqTok(p_lhs), _$p_lhs, __op__asg_l__expandRhs
#define __op__asg_l__expandRhs(_$rhs...) _$rhs
#define __op__asg_l__emit(__p_lhs, _$p_lhs, _$rhs...) ({ \
    let_(__p_lhs, TypeOf(_$p_lhs)) = _$p_lhs; \
    typedef TypeOf(*__p_lhs) LitType; \
    claim_assert_nonnull(__p_lhs); \
    *__p_lhs = from$((LitType)_$rhs); \
    __p_lhs; \
})

#define $init(/*(_$T){_$initial...}*/... /*(_$T)*/) __op__$init(__op__$init__parseT __VA_ARGS__)
#define __op__$init(...) __op__$init__emit(__VA_ARGS__)
#define __op__$init__parseT(_$T...) _$T,
#define __op__$init__emit(_$T, _$initial...) ({ \
    $maybe_unused typedef _$T InitType; \
    (InitType) _$initial; \
})

#define $asg(_$val...) , $_$asg, (_$val)
#define $field(/*(_$field)*/...) __op__$field(__VA_ARGS__)
#define __op__$field(...) pp_overload(__op__$field, __VA_ARGS__)(__VA_ARGS__)
#define __op__$field_1(...) __op__$field_1__emit(__VA_ARGS__)
#define __op__$field_1__emit(_$field_val...) \
    .__op__$field__expand _$field_val = ({ \
        $maybe_unused typedef InitType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _$field_val) FieldType; \
        _$field_val; \
    })
#define __op__$field_3(...) __op__$field_3__emit(__VA_ARGS__)
#define __op__$field_3__emit(_$field, _$op, _$val...) \
    .__op__$field__expand _$field = ({ \
        $maybe_unused typedef InitType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$field__expand _$field) FieldType; \
        _$val; \
    })
#define __op__$field__expand(...) __VA_ARGS__
#define $in_field(...) __op__$in_field__emit(__VA_ARGS__)
#define __op__$in_field(...) pp_overload(__op__$in_field, __VA_ARGS__)(__VA_ARGS__)
#define __op__$in_field_1(...) __op__$in_field_1__emit(__VA_ARGS__)
#define __op__$in_field_1__emit(_$field_val...) \
    .__op__$in_field__expand _$field_val = ({ \
        $maybe_unused typedef FieldType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _$field_val) FieldType; \
        _$field_val; \
    })
#define __op__$in_field_3(...) __op__$in_field_3__emit(__VA_ARGS__)
#define __op__$in_field_3__emit(_$field, _$op, _$val...) \
    .__op__$in_field__expand _$field = ({ \
        $maybe_unused typedef FieldType ParentType; \
        $maybe_unused typedef FieldType$(ParentType, __op__$in_field__expand _$field) FieldType; \
        _$val; \
    })
#define __op__$in_field__expand(...) __VA_ARGS__

#define cleared$ l0$
#define cleared() ____cleared()
#define ____cleared() \
    {}
#define initial$(/*(_$T)(_$expr...)*/...) __step__$initial$(__VA_ARGS__)
#define __step__$initial$(...) __step__$initial$__emit(__step__$initial$__parse __VA_ARGS__)
#define __step__$initial$__parse(_$T...) _$T, __step__$initial$__parseExpr
#define __step__$initial$__parseExpr(_$expr...) _$expr
#define __step__$initial$__emit(...) ____$initial$(__VA_ARGS__)
#define ____$initial$(_$T, _$expr...) l$((_$T){ _$expr })
#define initial(/*_$expr...*/...) ____$initial(__VA_ARGS__)
#define ____$initial(_$expr...) \
    { _$expr }

#define move(_$p_val... /*(TypeOf(*_$p_val))*/) ____move(pp_uniqTok(p_val), pp_uniqTok(val), _$p_val)
#define ____move(__p_val, __val, _$p_val...) local_({ \
    let_(__p_val, TypeOf(_$p_val)) = _$p_val; \
    let_(__val, TypeOfUnqual(*__p_val)) = *__p_val; \
    *__p_val = l0$((TypeOf(__val))); \
    local_return_(__val); \
})
#define copy(_$val... /*(TypeOf(_$val))*/) ____copy(pp_uniqTok(val), pp_uniqTok(dst), _$val)
#define ____copy(__val, __dst, _$val...) (*local_({ \
    let __val = _$val; \
    typedef TypeOfUnqual(__val) CopyType; \
    let __dst = as$(CopyType*)(raw_allocaAlign(sizeOf$(CopyType), alignOfLog2$(CopyType))); \
    raw_memcpy(__dst, &__val, sizeOf$(CopyType)); \
    local_return_(__dst); \
}))

/* TODO: Support bitfield */
#define with_(/*(_$expr: _$T)(_$initial...: (_$field)(_$asg)*/... /*(_$T)*/) __step__with_(__VA_ARGS__)
#define __step__with_(...) __step__with$__emit(__step__with$__parseExpr __VA_ARGS__)
#define __step__with$__parseExpr(_$expr...) pp_uniqTok(expr_copied), _$expr, __step__with$__parseInitial
#define __step__with$__parseInitial(_$initial...) _$initial
#define __step__with$__emit(...) \
    ____with_(__VA_ARGS__)
#define ____with_(__expr_copied, _$expr, _$initial...) (*local_({ \
    var __expr_copied = &copy(_$expr); \
    pp_foreach(____with___each, __expr_copied, _$initial); \
    local_return_(__expr_copied); \
}))
#define ____with___each(__expr_copied, /*_$initial*/...) __VA_OPT__( \
    ____with___each__emit(__expr_copied, ____with___each__parseField __VA_ARGS__) \
)
#define ____with___each__parseField(_$field...) _$field, ____with___each__parseAsg
#define ____with___each__parseAsg(_$asg...) _$asg
#define ____with___each__emit(...) \
    ____with_____each(__VA_ARGS__)
#define ____with_____each(__expr_copied, _$field, _$asg...) \
    asg_l((&(*__expr_copied)_$field)(_$asg));

#define T_switch$(/*(_$T_Cond)(_$T_Cases...)*/...) \
    __step__$T_switch$(__step__$T_switch$__parseTCond __VA_ARGS__)
#define __step__$T_switch$(...) __$T_switch$(__VA_ARGS__)
#define __step__$T_switch$__parseTCond(_$T_Cond...) _$T_Cond, __step__$T_switch$__parseTCases
#define __step__$T_switch$__parseTCases(_$T_Cases...) _$T_Cases
#define __$T_switch$(_$T_Cond, _$T_Cases...) _Generic(null$($P$(_$T_Cond)), _$T_Cases)
#define T_case$(/*(_$T_Case)_$expr...*/...) \
    __step__$T_case$(__step__$T_case$__parseTCase __VA_ARGS__)
#define __step__$T_case$(...) __$T_case$(__VA_ARGS__)
#define __step__$T_case$__parseTCase(_$T_Case...) _$T_Case,
#define __$T_case$(_$T_Case, _$expr...) /* clang-format off */ \
    $P_const$(_$T_Case): \
        _$expr, \
    $P$(_$T_Case): \
        _$expr /* clang-format on */
#define T_qual$(/*(_$T_Case)_$expr...*/...) \
    __step__$T_qual$(__step__$T_qual$__parseTCase __VA_ARGS__)
#define __step__$T_qual$(...) __$T_qual$(__VA_ARGS__)
#define __step__$T_qual$__parseTCase(_$T_Case...) _$T_Case,
#define __$T_qual$(_$T_Case, _$expr...) /* clang-format off */ \
    $P$(_$T_Case): \
        _$expr /* clang-format on */
#define T_default_(_$expr...) \
    default: \
        _$expr
#define T_delim() T_case$((struct {})($unreachable))

#if UNUSED_CODE
#define T_case$(_$T_Case...) $P$(_$T_Case)
#define T_default_() default
#endif /* UNUSED_CODE */

#define local_ comp_syn__local_
#define local_label comp_syn__local_label
#define local_return_(...) comp_syn__local_return_(__VA_ARGS__)

#define $fallthrough __attr__$fallthrough
#define $branch_hot __attr__$branch_hot
#define $branch_cold __attr__$branch_cold
#define $branch_predict(_$expected /*: bool*/, _$expr... /*(bool)*/) __attr__$branch_predict(_$expected, _$expr)
#define $branch_predict_at(_$prob /*: FltType*/, _$expr... /*(bool)*/) __attr__$branch_predict_at(_$prob, _$expr)
#define $branch_likely(_$expr... /*(bool)*/) __attr__$branch_likely(_$expr)
#define $branch_unlikely(_$expr... /*(bool)*/) __attr__$branch_unlikely(_$expr)
#define $branch_unpredictable(_$expr... /*(bool)*/) __attr__$branch_unpredictable(_$expr)
#define $unreachable __attr__$unreachable

#define $zero_sized __attr__$zero_sized
#define $flexible __attr__$flexible
#define $like_ref __attr__$like_ref
#define $like_deref __attr__$like_deref

#define $static static
#define $extern extern
#define $thrd_local _Thread_local

#define $callconv_cdecl __attr__$callconv_cdecl
#define $callconv_stdcall __attr__$callconv_stdcall
#define $callconv_fastcall __attr__$callconv_fastcall
#define $callconv_vectorcall __attr__$callconv_vectorcall
#define $callconv_naked __attr__$callconv_naked
#define $callconv_preserve_none __attr__$callconv_preserve_none
#define $callconv_preserve_all __attr__$callconv_preserve_all

#define $packed __attr__$packed
#define $section(_$name) __attr__$section(_$name)
#define $bits(_$width... /*8|16|32|64*/) __attr__$bits(_$width)
#define $align(_$align...) __attr__$align(_$align)

#define not !
#define and &&
#define or ||

#define $listed /* just formatting comment for list expansion */
#define $str /* just formatting comment for string literal */

#define $loop_unroll __pragma(clang loop unroll(enable))
#define $loop_inline __pragma(clang loop unroll(full))
#define $loop_rolled __pragma(clang loop unroll(disable))

#define $pragma_guard_(_$push, _$ctx, _$pop, _$code...) /* clang-format off */ \
    _Pragma(_$push) \
    _Pragma(_$ctx) \
    _$code \
    _Pragma(_$pop) /* clang-format on */

#define $suppress_(_$diagnostic...) pp_cat($suppress_, _$diagnostic)

#define $suppress_compound_token_split_by_macro(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_implicit_int_conversion(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wimplicit-int-conversion\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_pointer_arith(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wpointer-arith\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_cast_align(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcast-align\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_cast_qual(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcast-qual\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)

#if comp_has_warning("-Wunterminated-string-initialization")
#define $suppress_unterminated_string_initialization(...) /* clang-format off */ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunterminated-string-initialization\"") \
    __VA_ARGS__ \
    _Pragma("clang diagnostic pop") /* clang-format on */
#else
#define $suppress_unterminated_string_initialization(...) __VA_ARGS__
#endif
#define $suppress_return_stack_address(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wreturn-stack-address\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_frame_larger_than(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wframe-larger-than\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_infinite_recursion(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Winfinite-recursion\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_loop_analysis(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wloop-analysis\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_switch_enum(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wswitch-enum\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_thread_safety(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wthread-safety\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)
#define $suppress_microsoft_anon_tag(...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wmicrosoft-anon-tag\"", \
    "clang diagnostic pop", \
    __VA_ARGS__ \
)

#define $suppressing_(_$diagnostic...) /* clang-format off */ \
    _Pragma("clang diagnostic push") \
    pp_foreach(__each__$suppressing_, ~, _$diagnostic)
#define __each__$suppressing_(_$ignored, _$diagnostic) \
    pp_cat(__$suppress_, _$diagnostic)()
#define $suppressed \
    _Pragma("clang diagnostic pop") /* clang-format on */
#define __$suppress_compound_token_split_by_macro() \
    _Pragma("clang diagnostic ignored \"-Wcompound-token-split-by-macro\"")
#define __$suppress_implicit_int_conversion() \
    _Pragma("clang diagnostic ignored \"-Wimplicit-int-conversion\"")
#define __$suppress_pointer_arith() \
    _Pragma("clang diagnostic ignored \"-Wpointer-arith\"")
#define __$suppress_cast_align() \
    _Pragma("clang diagnostic ignored \"-Wcast-align\"")
#define __$suppress_cast_qual() \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#if comp_has_warning("-Wunterminated-string-initialization")
#define __$suppress_unterminated_string_initialization() \
    _Pragma("clang diagnostic ignored \"-Wunterminated-string-initialization\"")
#else
#define __$suppress_unterminated_string_initialization()
#endif
#define __$suppress_return_stack_address() \
    _Pragma("clang diagnostic ignored \"-Wreturn-stack-address\"")
#define __$suppress_frame_larger_than() \
    _Pragma("clang diagnostic ignored \"-Wframe-larger-than\"")
#define __$suppress_infinite_recursion() \
    _Pragma("clang diagnostic ignored \"-Winfinite-recursion\"")
#define __$suppress_loop_analysis() \
    _Pragma("clang diagnostic ignored \"-Wloop-analysis\"")
#define __$suppress_switch_enum() \
    _Pragma("clang diagnostic ignored \"-Wswitch-enum\"")
#define __$suppress_thread_safety() \
    _Pragma("clang diagnostic ignored \"-Wthread-safety\"")
#define __$suppress_microsoft_anon_tag() \
    _Pragma("clang diagnostic ignored \"-Wmicrosoft-anon-tag\"")

/*========== Macros and Definitions =========================================*/

#define __attr__$inline comp_inline
#define __attr__$inline_always comp_inline_always
#define __attr__$inline_never comp_inline_never
#define __attr__$flatten comp_flatten

#define __attr__$deprecated comp_deprecated
#define __attr__$deprecated_msg(_$Msg) comp_deprecated_msg(_$Msg)
#define __attr__$deprecated_instead(_$Msg, _$Replacement) comp_deprecated_instead(_$Msg, _$Replacement)

#define __attr__$pure comp_pure
#define __attr__$view comp_view

#define __attr__$on_load comp_on_load
#define __attr__$on_exit comp_on_exit

#define __attr__$must_check comp_must_use
#define __attr__$no_return comp_no_return
#define __attr__$must_tail comp_must_tail
#define __oper__$ignore_void (void)
#define __capt__$ignore \
    $maybe_unused pp_uniqTok(ignored) = l0$((Void)); \
    let_ignore
#define __stmt__let_ignore $maybe_unused let pp_uniqTok(ignored)
#define __stmt__$do_nothing \
    {}
#define __stmt__$noop \
    {}

#define __attr__$used(_$Expr...) _$Expr
/* begin unused */
#define __attr__$unused(_$Expr...) __attr__$unused__payload(__attr__$unused__countArgs(_$Expr), _$Expr)
#define __attr__$unused__payload(_Count, _Args...) pp_join(__, __attr__$unused, _Count)(_Args)
#define __attr__$unused__countArgs(_Args...) __attr__$unused__countArgs__select(_Args, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __attr__$unused__countArgs__select(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ...) _N
#define __attr__$unused__1(_$x1) (void)(_$x1)
#define __attr__$unused__2(_$x1, _$x2) (void)(_$x1), (void)(_$x2)
#define __attr__$unused__3(_$x1, _$x2, _$x3) (void)(_$x1), (void)(_$x2), (void)(_$x3)
#define __attr__$unused__4(_$x1, _$x2, _$x3, _$x4) (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4)
#define __attr__$unused__5(_$x1, _$x2, _$x3, _$x4, _$x5) (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5)
#define __attr__$unused__6(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6) (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6)
#define __attr__$unused__7(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7) (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7)
#define __attr__$unused__8(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8) (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8)
#define __attr__$unused__9(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9)
#define __attr__$unused__10(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10)
#define __attr__$unused__11(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11)
#define __attr__$unused__12(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11, _$x12) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11), (void)(_$x12)
#define __attr__$unused__13(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11, _$x12, _$x13) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11), (void)(_$x12), \
        (void)(_$x13)
#define __attr__$unused__14(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11, _$x12, _$x13, _$x14) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11), (void)(_$x12), \
        (void)(_$x13), (void)(_$x14)
#define __attr__$unused__15(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11, _$x12, _$x13, _$x14, _$x15) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11), (void)(_$x12), \
        (void)(_$x13), (void)(_$x14), (void)(_$x15)
#define __attr__$unused__16(_$x1, _$x2, _$x3, _$x4, _$x5, _$x6, _$x7, _$x8, _$x9, _$x10, _$x11, _$x12, _$x13, _$x14, _$x15, _$x16) \
    (void)(_$x1), (void)(_$x2), (void)(_$x3), (void)(_$x4), (void)(_$x5), (void)(_$x6), (void)(_$x7), (void)(_$x8), (void)(_$x9), (void)(_$x10), (void)(_$x11), (void)(_$x12), \
        (void)(_$x13), (void)(_$x14), (void)(_$x15), (void)(_$x16)
/* end unused */

#define __attr__$keep_symbol comp_keep_symbol
#define __attr__$maybe_unused comp_maybe_unused
#define __attr__$must_use comp_must_use
#define __attr__$undefined comp_undefined
#define __attr__$undefined_static comp_undefined_static

#define __attr__$import comp_import
#define __attr__$export comp_export

#define comp_syn__local_ /* just comment for compound statement expression ({...}) */
#define comp_syn__local_label __label__
#define comp_syn__local_return_(...) __VA_ARGS__

#define __attr__$fallthrough comp_fallthrough
#define __attr__$branch_hot comp_branch_hot
#define __attr__$branch_cold comp_branch_cold
#define __attr__$branch_predict(_$expected /*: bool*/, _$expr... /*(bool)*/) comp_branch_predict(_$expected, _$expr)
#define __attr__$branch_predict_at(_$prob /*: FltType*/, _$expr... /*(bool)*/) comp_branch_predict_at(_$prob, _$expr)
#define __attr__$branch_likely(_$expr... /*(bool)*/) comp_branch_likely(_$expr)
#define __attr__$branch_unlikely(_$expr... /*(bool)*/) comp_branch_unlikely(_$expr)
#define __attr__$branch_unpredictable(_$expr... /*(bool)*/) comp_branch_unpredictable(_$expr)
#define __attr__$unreachable comp_unreachable

#define __attr__$zero_sized [0]
#define __attr__$flexible [0]
#define __attr__$like_ref [1]
#define __attr__$like_deref [0]

/* Compiler syntax is owned by cfg/comp; target applicability is assembled
 * here after cfg/arch and cfg/plat are both available. */
#define __attr__$callconv_cdecl pp_if_(pp_or(plat_is_windows, arch_family_is_x86))( \
    pp_then_(comp_callconv_cdecl), \
    pp_else_() \
)
#define __attr__$callconv_stdcall pp_if_(pp_or(plat_is_windows, arch_is_x86))( \
    pp_then_(comp_callconv_stdcall), \
    pp_else_() \
)
#define __attr__$callconv_fastcall pp_if_(pp_or(plat_is_windows, arch_is_x86))( \
    pp_then_(comp_callconv_fastcall), \
    pp_else_() \
)
#define __attr__$callconv_vectorcall pp_if_(pp_or(plat_is_windows, arch_family_is_x86))( \
    pp_then_(comp_callconv_vectorcall), \
    pp_else_() \
)
#define __attr__$callconv_naked comp_callconv_naked
#define __attr__$callconv_preserve_none comp_callconv_preserve_none
#define __attr__$callconv_preserve_all comp_callconv_preserve_all

#define __attr__$packed comp_packed
#define __attr__$section(_$name) comp_section(_$name)
#define __attr__$bits(_$width... /*8|16|32|64*/) pp_join(_, __attr__$bits, _$width)
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
#define __attr__$align(_$align...) comp_align(_$align)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_comp__included */
