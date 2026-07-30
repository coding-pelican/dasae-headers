/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dal-project/da/foundation/pp
 * @prefix  pp
 *
 * @brief   Preprocessor utilities
 * @details Provides preprocessor utilities and functions for:
 *          - Stringification
 *          - Token concatenation
 *          - Token uniquification
 *          - Token counting
 *          - Token iteration
 */
#pragma once
#ifndef foundation_pp_common__included
#define foundation_pp_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define pp_begin (
#define pp_end )

#define pp_nothing(...) pp_exec_nothing(__VA_ARGS__)
#define pp_ignore(...) pp_exec_ignore(__VA_ARGS__)
#define pp_expand(...) pp_exec_expand(__VA_ARGS__)
#define pp_defer(...) pp_exec_defer(__VA_ARGS__)

#define pp_strfy(_$Tok...) pp_exec_strfy(_$Tok)

#define pp_cat(_$LTok, _$RTok...) pp_exec_cat(_$LTok, _$RTok)
#define pp_cat2(_$LTok, _$RTok...) pp_exec_cat2(_$LTok, _$RTok)
#define pp_cat3(_$LTok, _$MTok, _$RTok...) \
    pp_exec_cat3(_$LTok, _$MTok, _$RTok)
#define pp_cat4(_$LTok, _$LMTok, _$RMTok, _$RTok...) \
    pp_exec_cat4(_$LTok, _$LMTok, _$RMTok, _$RTok)
#define pp_cat5(_$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok...) \
    pp_exec_cat5(_$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok)

#define pp_join(_$SepTok, _$LTok, _$RTok...) pp_exec_join(_$SepTok, _$LTok, _$RTok)
#define pp_join2(_$SepTok, _$LTok, _$RTok...) pp_exec_join2(_$SepTok, _$LTok, _$RTok)
#define pp_join3(_$SepTok, _$LTok, _$MTok, _$RTok...) \
    pp_exec_join3(_$SepTok, _$LTok, _$MTok, _$RTok)
#define pp_join4(_$SepTok, _$LTok, _$LMTok, _$RMTok, _$RTok...) \
    pp_exec_join4(_$SepTok, _$LTok, _$LMTok, _$RMTok, _$RTok)
#define pp_join5(_$SepTok, _$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok...) \
    pp_exec_join5(_$SepTok, _$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok)

#define pp_uniqTok(_$Tok...) pp_exec_uniqTok(_$Tok)
#define pp_uniqTokByLine(_$Tok...) pp_exec_uniqTokByLine(_$Tok)

#define pp_countArg(_$Args...) \
    /** \
     * @note handles up to 64 arguments \
     * \
     * pp_countArg() => 0 \
     * pp_countArg(a0, a1, a2, ..., a63) => 64 \
     */ \
    pp_exec_countArg(_$Args)
#define pp_overload(_$Ident, ...) \
    /** \
     * @note handles up to 64 arguments \
     * \
     * pp_overload(ident): \
     * => ident_0 \
     * pp_overload(ident, a0): \
     * => ident_1 \
     * pp_overload(ident, a0, a1): \
     * => ident_2 \
     */ \
    pp_exec_overload(_$Ident, __VA_ARGS__)
#define pp_foreach(_$Exec, _$Iter, ...) \
    /** \
     * @note handles up to 64 arguments \
     * \
     * pp_foreach(exec, iter, x, y, z): \
     * => exec(iter, x) exec(iter, y) exec(iter, z) \
     */ \
    pp_exec_foreach(_$Exec, _$Iter, __VA_ARGS__)
#define pp_foldl(_$Step, _$Init, ...) \
    /** \
     * @note handles up to 64 arguments \
     * \
     * pp_foldl(step, init, a, b, c): \
     * => step(step(step(init, a), b), c) \
     */ \
    pp_exec_foldl(_$Step, _$Init, __VA_ARGS__)
#define pp_foldr(_$Step, _$Init, ...) \
    /** \
     * @note handles up to 64 arguments \
     * \
     * pp_foldr(step, init, a, b, c): \
     * => step(a, step(b, step(c, init))) \
     */ \
    pp_exec_foldr(_$Step, _$Init, __VA_ARGS__)

/*--- tok ---*/

#define pp_true 1
#define pp_false 0

#define pp_not(_$Cond...) pp_join(_, __pp_not, _$Cond)
#define __pp_not_1 0
#define __pp_not_0 1

#define pp_and(_$Lhs, _$Rhs...) pp_join3(_, __pp_and, _$Lhs, _$Rhs)
#define __pp_and_0_0 0
#define __pp_and_0_1 0
#define __pp_and_1_0 0
#define __pp_and_1_1 1

#define pp_or(_$Lhs, _$Rhs...) pp_join3(_, __pp_or, _$Lhs, _$Rhs)
#define __pp_or_0_0 0
#define __pp_or_0_1 1
#define __pp_or_1_0 1
#define __pp_or_1_1 1

#define pp_xor(_$Lhs, _$Rhs...) pp_join3(_, __pp_xor, _$Lhs, _$Rhs)
#define __pp_xor_0_0 0
#define __pp_xor_0_1 1
#define __pp_xor_1_0 1
#define __pp_xor_1_1 0

#define pp_eql(_$Lhs, _$Rhs...) pp_join3(_, __pp_eql, _$Lhs, _$Rhs)
#define __pp_eql_0_0 1
#define __pp_eql_0_1 0
#define __pp_eql_1_0 0
#define __pp_eql_1_1 1

#define pp_neq(_$Lhs, _$Rhs...) pp_join3(_, __pp_neq, _$Lhs, _$Rhs)
#define __pp_neq_0_0 0
#define __pp_neq_0_1 1
#define __pp_neq_1_0 1
#define __pp_neq_1_1 0

#define __call__pp_if() pp_if_
#define pp_if_(_$Cond...) pp_join(_, ____pp_if, _$Cond)
#define ____pp_if_1(_$Then, ...) ____pp_if__then _$Then
#define ____pp_if__then(...) __VA_ARGS__
#define ____pp_if_0(...) pp_overload(____pp_if_0, __VA_ARGS__)(__VA_ARGS__)
#define ____pp_if_0_1(_$Then...)
#define ____pp_if_0_2(_$Then, _$Else...) ____pp_if__else _$Else
#define ____pp_if__else(...) __VA_ARGS__
#define pp_then_
#define pp_else_

#define pp_Tok_cmp$pp__some(_$x) _$x
#define pp_Tok_cmp$pp__none(_$x) _$x
#define pp_some(_$Args...) (pp__some, (_$Args))
#define pp_none() (pp__none, ())
#define pp_isSome(/*(_$tag, _$val)*/...) __step__pp_isSome(__VA_ARGS__)
#define __step__pp_isSome(...) __step__pp_isSome__emit(__step__pp_isSome__parse __VA_ARGS__)
#define __step__pp_isSome__parse(_$tag, _$val...) _$tag, _$val
#define __step__pp_isSome__emit(...) ____pp_isSome(__VA_ARGS__)
#define ____pp_isSome(_$tag, _$val...) pp_if_(pp_Tok_eql(_$tag, pp__some))(pp_then_(pp_true), pp_else_(pp_false))
#define pp_isNone(/*(_$tag, _$val)*/...) pp_not(pp_isSome(__VA_ARGS__))
#define pp_orelse_(/*((_$tag, _$val))(_$default_val)*/...) __step__pp_orelse(__VA_ARGS__)
#define __step__pp_orelse(...) __step__pp_orelse__emit(__step__pp_orelse__parse __VA_ARGS__)
#define __step__pp_orelse__parse(_$pair_tag_val...) __step__pp_orelse__parsePair _$pair_tag_val,
#define __step__pp_orelse__parsePair(_$pair_tag_val...) _$pair_tag_val
#define __step__pp_orelse__emit(...) ____pp_orelse(__VA_ARGS__)
#define ____pp_orelse(_$tag, _$val, _$default_val...) pp_if_(pp_Tok_eql(_$tag, pp__some))(pp_then_ _$val, pp_else_ _$default_val)

#define __call__pp_switch_() __pp_switch_
#define pp_switch_(/*(_$pp_cond)(_$pp_cases...)*/...) __step__pp_switch_(__step__pp_switch___parseCond __VA_ARGS__)
#define __step__pp_switch___parseCond(_$pp_cond...) _$pp_cond, __step__pp_switch___parseCases
#define __step__pp_switch___parseCases(_$pp_cases...) _$pp_cases
#define __step__pp_switch_(...) __pp_switch_(__VA_ARGS__)
#define __pp_switch_(_$pp_cond, _$pp_cases...) __pp_switch__expand(__pp_switch___eval(_$pp_cond, _$pp_cases))
#define __pp_switch__expand(...) /* evaluate 2^5 */ __pp_switch__expand5(__VA_ARGS__)
#define __pp_switch__expand5(...) __pp_switch__expand4(__pp_switch__expand4(__VA_ARGS__))
#define __pp_switch__expand4(...) __pp_switch__expand3(__pp_switch__expand3(__VA_ARGS__))
#define __pp_switch__expand3(...) __pp_switch__expand2(__pp_switch__expand2(__VA_ARGS__))
#define __pp_switch__expand2(...) __pp_switch__expand1(__pp_switch__expand1(__VA_ARGS__))
#define __pp_switch__expand1(...) __VA_ARGS__
#define __call__pp_switch___eval() __pp_switch___eval
#define __pp_switch___eval(_$pp_cond, _$tag, ...) \
    pp_cat(__pp_switch___eval, _$tag)(_$pp_cond, __VA_ARGS__)
#define __pp_switch___eval$_pp_case(_$pp_cond, _$pp_enum, _$pp_expr, ...) \
    pp_defer(__call__pp_if)()(pp_Tok_eql(_$pp_cond, _$pp_enum))(_$pp_expr, (__call__pp_switch___eval)()(_$pp_cond, __VA_ARGS__))
#define __pp_switch___eval$_pp_default(_$pp_cond, _$pp_expr...) \
    _$pp_expr

#define pp_case_(/*(_$pp_enum)(_$pp_expr)*/...) __step__pp_case_(__step__pp_case___parse __VA_ARGS__)
#define __step__pp_case___parse(_$pp_enum...) _$pp_enum,
#define __step__pp_case_(...) __pp_case_(__VA_ARGS__)
#define __pp_case_(_$pp_enum, _$pp_expr...) $_pp_case, _$pp_enum, _$pp_expr
#define pp_default_(_$pp_expr...) __pp_default_(_$pp_expr)
#define __pp_default_(_$pp_expr...) $_pp_default, _$pp_expr

/*========== Macros and Definitions =========================================*/

#define pp_exec_nothing(...)
#define pp_exec_ignore(...)
#define pp_exec_expand(...) __VA_ARGS__
#define pp_exec_defer(...) __VA_ARGS__ pp_exec_nothing()

#define pp_exec_strfy(_$Tok...) #_$Tok

#define pp_exec_cat(_$LTok, _$RTok...) _$LTok##_$RTok
#define pp_exec_cat2(_$LTok, _$RTok...) _$LTok##_$RTok
#define pp_exec_cat3(_$LTok, _$MTok, _$RTok...) \
    _$LTok##_$MTok##_$RTok
#define pp_exec_cat4(_$LTok, _$LMTok, _$RMTok, _$RTok...) \
    _$LTok##_$LMTok##_$RMTok##_$RTok
#define pp_exec_cat5(_$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok...) \
    _$LTok##_$LMTok##_$MTok##_$RMTok##_$RTok

#define pp_exec_join(_$SepTok, _$LTok, _$RTok...) _$LTok##_$SepTok##_$RTok
#define pp_exec_join2(_$SepTok, _$LTok, _$RTok...) _$LTok##_$SepTok##_$RTok
#define pp_exec_join3(_$SepTok, _$LTok, _$MTok, _$RTok...) \
    _$LTok##_$SepTok##_$MTok##_$SepTok##_$RTok
#define pp_exec_join4(_$SepTok, _$LTok, _$LMTok, _$RMTok, _$RTok...) \
    _$LTok##_$SepTok##_$LMTok##_$SepTok##_$RMTok##_$SepTok##_$RTok
#define pp_exec_join5(_$SepTok, _$LTok, _$LMTok, _$MTok, _$RMTok, _$RTok...) \
    _$LTok##_$SepTok##_$LMTok##_$SepTok##_$MTok##_$SepTok##_$RMTok##_$SepTok##_$RTok

#define pp_exec_uniqTok(_$Tok...) pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _$Tok)
#define pp_exec_uniqTokByLine(_$Tok...) pp_join(_, pp_join2(_, _, __LINE__), _$Tok)

#define pp_exec_countArg(...) pp_exec_countArg__selectNthInRSeqNList( \
    __VA_OPT__(, ) __VA_ARGS__, pp_exec_countArg__listRSeqN() \
)
#define pp_exec_countArg__selectNthInRSeqNList(_$Args...) \
    pp_exec_countArg__selectNth(_$Args)
#define pp_exec_countArg__listRSeqN() \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_exec_countArg__selectNth( \
    _A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7, _A8, _A9, _A10, _A11, _A12, _A13, _A14, _A15, _A16, _A17, _A18, _A19, _A20, _A21, _A22, _A23, _A24, _A25, _A26, _A27, _A28, _A29, _A30, _A31, _A32, _A33, _A34, _A35, _A36, _A37, _A38, _A39, _A40, _A41, _A42, _A43, _A44, _A45, _A46, _A47, _A48, _A49, _A50, _A51, _A52, _A53, _A54, _A55, _A56, _A57, _A58, _A59, _A60, _A61, _A62, _A63, _A64, \
    _$N, ... \
) _$N

#define pp_exec_overload(_$Ident, ...) \
    pp_join(_, _$Ident, pp_countArg(__VA_ARGS__))

#define pp_exec_foreach(_$Exec, _$Iter, ...) \
    pp_overload(pp_exec_foreach, __VA_ARGS__)(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_0(...)
#define pp_exec_foreach_1(_$Exec, _$Iter, _Arg) _$Exec(_$Iter, _Arg) pp_exec_foreach_0()
#define pp_exec_foreach_2(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_1(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_3(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_2(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_4(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_3(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_5(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_4(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_6(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_5(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_7(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_6(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_8(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_7(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_9(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_8(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_10(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_9(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_11(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_10(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_12(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_11(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_13(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_12(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_14(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_13(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_15(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_14(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_16(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_15(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_17(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_16(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_18(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_17(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_19(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_18(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_20(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_19(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_21(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_20(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_22(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_21(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_23(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_22(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_24(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_23(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_25(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_24(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_26(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_25(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_27(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_26(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_28(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_27(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_29(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_28(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_30(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_29(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_31(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_30(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_32(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_31(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_33(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_32(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_34(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_33(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_35(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_34(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_36(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_35(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_37(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_36(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_38(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_37(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_39(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_38(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_40(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_39(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_41(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_40(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_42(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_41(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_43(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_42(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_44(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_43(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_45(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_44(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_46(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_45(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_47(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_46(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_48(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_47(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_49(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_48(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_50(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_49(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_51(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_50(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_52(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_51(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_53(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_52(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_54(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_53(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_55(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_54(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_56(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_55(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_57(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_56(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_58(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_57(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_59(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_58(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_60(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_59(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_61(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_60(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_62(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_61(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_63(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_62(_$Exec, _$Iter, __VA_ARGS__)
#define pp_exec_foreach_64(_$Exec, _$Iter, _Arg, ...) _$Exec(_$Iter, _Arg) pp_exec_foreach_63(_$Exec, _$Iter, __VA_ARGS__)

#define pp_exec_foldl(_$Step, _$Init, ...) \
    pp_overload(pp_exec_foldl, __VA_ARGS__)(_$Step, _$Init, __VA_ARGS__)
#define pp_exec_foldl_0(_$Step, _$Init...) \
    _$Init
#define pp_exec_foldl_1(_$Step, _$Init, _$A...) \
    _$Step(_$Init, _$A)
#define pp_exec_foldl_2(_$Step, _$Init, _$A, _$B...) \
    _$Step(pp_exec_foldl_1(_$Step, _$Init, _$A), _$B)
#define pp_exec_foldl_3(_$Step, _$Init, _$A, _$B, _$C...) \
    _$Step(pp_exec_foldl_2(_$Step, _$Init, _$A, _$B), _$C)
#define pp_exec_foldl_4(_$Step, _$Init, _$A, _$B, _$C, _$D...) \
    _$Step(pp_exec_foldl_3(_$Step, _$Init, _$A, _$B, _$C), _$D)
#define pp_exec_foldl_5(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E...) \
    _$Step(pp_exec_foldl_4(_$Step, _$Init, _$A, _$B, _$C, _$D), _$E)
#define pp_exec_foldl_6(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F...) \
    _$Step(pp_exec_foldl_5(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E), _$F)
#define pp_exec_foldl_7(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G...) \
    _$Step(pp_exec_foldl_6(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F), _$G)
#define pp_exec_foldl_8(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H...) \
    _$Step(pp_exec_foldl_7(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G), _$H)
#define pp_exec_foldl_9(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I...) \
    _$Step(pp_exec_foldl_8(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H), _$I)
#define pp_exec_foldl_10(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J...) \
    _$Step(pp_exec_foldl_9(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I), _$J)
#define pp_exec_foldl_11(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K...) \
    _$Step(pp_exec_foldl_10(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J), _$K)
#define pp_exec_foldl_12(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L...) \
    _$Step(pp_exec_foldl_11(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K), _$L)
#define pp_exec_foldl_13(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M...) \
    _$Step(pp_exec_foldl_12(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L), _$M)
#define pp_exec_foldl_14(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N...) \
    _$Step(pp_exec_foldl_13(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M), _$N)
#define pp_exec_foldl_15(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O...) \
    _$Step(pp_exec_foldl_14(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N), _$O)
#define pp_exec_foldl_16(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P...) \
    _$Step(pp_exec_foldl_15(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O), _$P)
#define pp_exec_foldl_17(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q...) \
    _$Step(pp_exec_foldl_16(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P), _$Q)
#define pp_exec_foldl_18(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R...) \
    _$Step(pp_exec_foldl_17(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q), _$R)
#define pp_exec_foldl_19(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S...) \
    _$Step(pp_exec_foldl_18(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R), _$S)
#define pp_exec_foldl_20( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T... \
) \
    _$Step( \
        pp_exec_foldl_19( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S \
        ), \
        _$T \
    )
#define pp_exec_foldl_21( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U... \
) \
    _$Step( \
        pp_exec_foldl_20( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T \
        ), \
        _$U \
    )
#define pp_exec_foldl_22( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V... \
) \
    _$Step( \
        pp_exec_foldl_21( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U \
        ), \
        _$V \
    )
#define pp_exec_foldl_23( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W... \
) \
    _$Step( \
        pp_exec_foldl_22( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V \
        ), \
        _$W \
    )
#define pp_exec_foldl_24( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X... \
) \
    _$Step( \
        pp_exec_foldl_23( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W \
        ), \
        _$X \
    )
#define pp_exec_foldl_25( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y... \
) \
    _$Step( \
        pp_exec_foldl_24( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X \
        ), \
        _$Y \
    )
#define pp_exec_foldl_26( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z... \
) \
    _$Step( \
        pp_exec_foldl_25( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y \
        ), \
        _$Z \
    )
#define pp_exec_foldl_27( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0... \
) \
    _$Step( \
        pp_exec_foldl_26( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z \
        ), \
        _$A0 \
    )
#define pp_exec_foldl_28( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0... \
) \
    _$Step( \
        pp_exec_foldl_27( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0 \
        ), \
        _$B0 \
    )
#define pp_exec_foldl_29( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0... \
) \
    _$Step( \
        pp_exec_foldl_28( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0 \
        ), \
        _$C0 \
    )
#define pp_exec_foldl_30( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0, _$D0... \
) \
    _$Step( \
        pp_exec_foldl_29( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0, _$C0 \
        ), \
        _$D0 \
    )
#define pp_exec_foldl_31( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0, _$D0, _$E0... \
) \
    _$Step( \
        pp_exec_foldl_30( \
            _$Step, _$Init, \
            _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0, _$C0, _$D0 \
        ), \
        _$E0 \
    )

#define pp_exec_foldl_32(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31) \
    _$Step(pp_exec_foldl_31(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30), _$A31)
#define pp_exec_foldl_33(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32) \
    _$Step(pp_exec_foldl_32(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31), _$A32)
#define pp_exec_foldl_34(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33) \
    _$Step(pp_exec_foldl_33(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32), _$A33)
#define pp_exec_foldl_35(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34) \
    _$Step(pp_exec_foldl_34(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33), _$A34)
#define pp_exec_foldl_36(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35) \
    _$Step(pp_exec_foldl_35(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34), _$A35)
#define pp_exec_foldl_37(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36) \
    _$Step(pp_exec_foldl_36(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35), _$A36)
#define pp_exec_foldl_38(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37) \
    _$Step(pp_exec_foldl_37(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36), _$A37)
#define pp_exec_foldl_39(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38) \
    _$Step(pp_exec_foldl_38(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37), _$A38)
#define pp_exec_foldl_40(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39) \
    _$Step(pp_exec_foldl_39(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38), _$A39)
#define pp_exec_foldl_41(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40) \
    _$Step(pp_exec_foldl_40(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39), _$A40)
#define pp_exec_foldl_42(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41) \
    _$Step(pp_exec_foldl_41(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40), _$A41)
#define pp_exec_foldl_43(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42) \
    _$Step(pp_exec_foldl_42(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41), _$A42)
#define pp_exec_foldl_44(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43) \
    _$Step(pp_exec_foldl_43(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42), _$A43)
#define pp_exec_foldl_45(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44) \
    _$Step(pp_exec_foldl_44(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43), _$A44)
#define pp_exec_foldl_46(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45) \
    _$Step(pp_exec_foldl_45(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44), _$A45)
#define pp_exec_foldl_47(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46) \
    _$Step(pp_exec_foldl_46(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45), _$A46)
#define pp_exec_foldl_48(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47) \
    _$Step(pp_exec_foldl_47(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46), _$A47)
#define pp_exec_foldl_49(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48) \
    _$Step(pp_exec_foldl_48(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47), _$A48)
#define pp_exec_foldl_50(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49) \
    _$Step(pp_exec_foldl_49(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48), _$A49)
#define pp_exec_foldl_51(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50) \
    _$Step(pp_exec_foldl_50(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49), _$A50)
#define pp_exec_foldl_52(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51) \
    _$Step(pp_exec_foldl_51(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50), _$A51)
#define pp_exec_foldl_53(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52) \
    _$Step(pp_exec_foldl_52(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51), _$A52)
#define pp_exec_foldl_54(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53) \
    _$Step(pp_exec_foldl_53(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52), _$A53)
#define pp_exec_foldl_55(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54) \
    _$Step(pp_exec_foldl_54(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53), _$A54)
#define pp_exec_foldl_56(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55) \
    _$Step(pp_exec_foldl_55(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54), _$A55)
#define pp_exec_foldl_57(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56) \
    _$Step(pp_exec_foldl_56(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55), _$A56)
#define pp_exec_foldl_58(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57) \
    _$Step(pp_exec_foldl_57(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56), _$A57)
#define pp_exec_foldl_59(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58) \
    _$Step(pp_exec_foldl_58(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57), _$A58)
#define pp_exec_foldl_60(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59) \
    _$Step(pp_exec_foldl_59(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58), _$A59)
#define pp_exec_foldl_61(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60) \
    _$Step(pp_exec_foldl_60(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59), _$A60)
#define pp_exec_foldl_62(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61) \
    _$Step(pp_exec_foldl_61(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60), _$A61)
#define pp_exec_foldl_63(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62) \
    _$Step(pp_exec_foldl_62(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61), _$A62)
#define pp_exec_foldl_64(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62, _$A63) \
    _$Step(pp_exec_foldl_63(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62), _$A63)

#define pp_exec_foldr(_$Step, _$Init, ...) \
    pp_overload(pp_exec_foldr, __VA_ARGS__)(_$Step, _$Init, __VA_ARGS__)
#define pp_exec_foldr_0(_$Step, _$Init...) \
    _$Init
#define pp_exec_foldr_1(_$Step, _$Init, _$A...) \
    _$Step(_$A, _$Init)
#define pp_exec_foldr_2(_$Step, _$Init, _$A, _$B...) \
    _$Step(_$A, pp_exec_foldr_1(_$Step, _$Init, _$B))
#define pp_exec_foldr_3(_$Step, _$Init, _$A, _$B, _$C...) \
    _$Step(_$A, pp_exec_foldr_2(_$Step, _$Init, _$B, _$C))
#define pp_exec_foldr_4(_$Step, _$Init, _$A, _$B, _$C, _$D...) \
    _$Step(_$A, pp_exec_foldr_3(_$Step, _$Init, _$B, _$C, _$D))
#define pp_exec_foldr_5(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E...) \
    _$Step(_$A, pp_exec_foldr_4(_$Step, _$Init, _$B, _$C, _$D, _$E))
#define pp_exec_foldr_6(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F...) \
    _$Step(_$A, pp_exec_foldr_5(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F))
#define pp_exec_foldr_7(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G...) \
    _$Step(_$A, pp_exec_foldr_6(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G))
#define pp_exec_foldr_8(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H...) \
    _$Step(_$A, pp_exec_foldr_7(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H))
#define pp_exec_foldr_9(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I...) \
    _$Step(_$A, pp_exec_foldr_8(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I))
#define pp_exec_foldr_10(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J...) \
    _$Step(_$A, pp_exec_foldr_9(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J))
#define pp_exec_foldr_11(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K...) \
    _$Step(_$A, pp_exec_foldr_10(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K))
#define pp_exec_foldr_12(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L...) \
    _$Step(_$A, pp_exec_foldr_11(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L))
#define pp_exec_foldr_13(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M...) \
    _$Step(_$A, pp_exec_foldr_12(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M))
#define pp_exec_foldr_14(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N...) \
    _$Step(_$A, pp_exec_foldr_13(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N))
#define pp_exec_foldr_15(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O...) \
    _$Step(_$A, pp_exec_foldr_14(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O))
#define pp_exec_foldr_16(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P...) \
    _$Step(_$A, pp_exec_foldr_15(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P))
#define pp_exec_foldr_17(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q...) \
    _$Step(_$A, pp_exec_foldr_16(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q))
#define pp_exec_foldr_18(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R...) \
    _$Step(_$A, pp_exec_foldr_17(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R))
#define pp_exec_foldr_19(_$Step, _$Init, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S...) \
    _$Step(_$A, pp_exec_foldr_18(_$Step, _$Init, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S))
#define pp_exec_foldr_20( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_19( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T \
        ) \
    )
#define pp_exec_foldr_21( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_20( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U \
        ) \
    )
#define pp_exec_foldr_22( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_21( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V \
        ) \
    )
#define pp_exec_foldr_23( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_22( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W \
        ) \
    )
#define pp_exec_foldr_24( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_23( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X \
        ) \
    )
#define pp_exec_foldr_25( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_24( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y \
        ) \
    )
#define pp_exec_foldr_26( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_25( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z \
        ) \
    )
#define pp_exec_foldr_27( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_26( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0 \
        ) \
    )
#define pp_exec_foldr_28( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_27( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0 \
        ) \
    )
#define pp_exec_foldr_29( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_28( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0, _$C0 \
        ) \
    )
#define pp_exec_foldr_30( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0, _$D0... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_29( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0, _$C0, _$D0 \
        ) \
    )
#define pp_exec_foldr_31( \
    _$Step, _$Init, \
    _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
    _$A0, _$B0, _$C0, _$D0, _$E0... \
) \
    _$Step( \
        _$A, \
        pp_exec_foldr_30( \
            _$Step, _$Init, \
            _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I, _$J, _$K, _$L, _$M, _$N, _$O, _$P, _$Q, _$R, _$S, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, \
            _$A0, _$B0, _$C0, _$D0, _$E0 \
        ) \
    )

/*--- tok ---*/

#define pp_bitCmpl pp_bitNot
#define pp_bitNot(_$b) pp_cat(__pp_bitNot_, _$b)
#define __pp_bitNot_0 1
#define __pp_bitNot_1 0

#define pp_bitAnd(_$x) pp_cat(__pp_bitAnd_, _$x)
#define __pp_bitAnd_0(_$y) 0
#define __pp_bitAnd_1(_$y) _$y
#define pp_bitOr(_$x) pp_cat(__pp_bitOr_, _$x)
#define __pp_bitOr_0(_$y) 0
#define __pp_bitOr_1(_$y) _$y
#define pp_bitXor(_$x) pp_cat(__pp_bitXor_, _$x)
#define __pp_bitXor_0(_$y) 0
#define __pp_bitXor_1(_$y) _$y

#define __pp_check_n(_$x, _$n, ...) _$n
#define pp_check(...) __pp_check_n(__VA_ARGS__, 0, )
#define pp_probe(_$x) _$x, 1,

#define pp_isParen(_$x) pp_check(pp_isParenProbe _$x)
#define pp_isParenProbe(...) pp_probe(~)

#define pp_Tok_not(_$x) pp_check(pp_cat(__pp_Tok_not_, _$x))
#define __pp_Tok_not_0 pp_probe(~)

#define pp_bool(_$x) pp_bitCmpl(pp_Tok_not(_$x))

#define pp_iif(_$c) pp_cat(__pp_iif_, _$c)
#define __pp_iif_0(_$t, ...) __VA_ARGS__
#define __pp_iif_1(_$t, ...) _$t
#define pp_Tok_if_(_$c) pp_iif(pp_bool(_$c))
#define pp_Tok_then_(...) __VA_ARGS__
#define pp_Tok_else_(...) __VA_ARGS__

#define pp_Tok_pri_cmp(_$x, _$y) pp_isParen(pp_Tok_cmp$##_$x(pp_Tok_cmp$##_$y)(()))
#define pp_Tok_hasCmp(_$x) pp_isParen(pp_cat(pp_Tok_cmp$, _$x)(()))
#define pp_Tok_neq(_$x, _$y) pp_iif(pp_bitAnd(pp_Tok_hasCmp(_$x))(pp_Tok_hasCmp(_$y)))( \
    pp_Tok_then_(pp_Tok_pri_cmp), \
    pp_Tok_else_(1 pp_ignore) \
)(_$x, _$y)
#define pp_Tok_eql(_$x, _$y) pp_bitCmpl(pp_Tok_neq(_$x, _$y))

#define pp_comma() ,
#define pp_comma_if_(_$n) pp_Tok_if_(_$n)(pp_comma, pp_ignore)()

#define pp_Tok_cmp$0(_$x) _$x
#define pp_Tok_cmp$1(_$x) _$x
#define pp_Tok_cmp$2(_$x) _$x
#define pp_Tok_cmp$3(_$x) _$x
#define pp_Tok_cmp$4(_$x) _$x
#define pp_Tok_cmp$5(_$x) _$x
#define pp_Tok_cmp$6(_$x) _$x
#define pp_Tok_cmp$7(_$x) _$x
#define pp_Tok_cmp$8(_$x) _$x
#define pp_Tok_cmp$9(_$x) _$x
#define pp_Tok_cmp$10(_$x) _$x
#define pp_Tok_cmp$11(_$x) _$x
#define pp_Tok_cmp$12(_$x) _$x
#define pp_Tok_cmp$13(_$x) _$x
#define pp_Tok_cmp$14(_$x) _$x
#define pp_Tok_cmp$15(_$x) _$x
#define pp_Tok_cmp$16(_$x) _$x
#define pp_Tok_cmp$17(_$x) _$x
#define pp_Tok_cmp$18(_$x) _$x
#define pp_Tok_cmp$19(_$x) _$x
#define pp_Tok_cmp$20(_$x) _$x
#define pp_Tok_cmp$21(_$x) _$x
#define pp_Tok_cmp$22(_$x) _$x
#define pp_Tok_cmp$23(_$x) _$x
#define pp_Tok_cmp$24(_$x) _$x
#define pp_Tok_cmp$25(_$x) _$x
#define pp_Tok_cmp$26(_$x) _$x
#define pp_Tok_cmp$27(_$x) _$x
#define pp_Tok_cmp$28(_$x) _$x
#define pp_Tok_cmp$29(_$x) _$x
#define pp_Tok_cmp$30(_$x) _$x
#define pp_Tok_cmp$31(_$x) _$x

#define pp_Tok_cmp$32(_$x) _$x
#define pp_Tok_cmp$33(_$x) _$x
#define pp_Tok_cmp$34(_$x) _$x
#define pp_Tok_cmp$35(_$x) _$x
#define pp_Tok_cmp$36(_$x) _$x
#define pp_Tok_cmp$37(_$x) _$x
#define pp_Tok_cmp$38(_$x) _$x
#define pp_Tok_cmp$39(_$x) _$x
#define pp_Tok_cmp$40(_$x) _$x
#define pp_Tok_cmp$41(_$x) _$x
#define pp_Tok_cmp$42(_$x) _$x
#define pp_Tok_cmp$43(_$x) _$x
#define pp_Tok_cmp$44(_$x) _$x
#define pp_Tok_cmp$45(_$x) _$x
#define pp_Tok_cmp$46(_$x) _$x
#define pp_Tok_cmp$47(_$x) _$x
#define pp_Tok_cmp$48(_$x) _$x
#define pp_Tok_cmp$49(_$x) _$x
#define pp_Tok_cmp$50(_$x) _$x
#define pp_Tok_cmp$51(_$x) _$x
#define pp_Tok_cmp$52(_$x) _$x
#define pp_Tok_cmp$53(_$x) _$x
#define pp_Tok_cmp$54(_$x) _$x
#define pp_Tok_cmp$55(_$x) _$x
#define pp_Tok_cmp$56(_$x) _$x
#define pp_Tok_cmp$57(_$x) _$x
#define pp_Tok_cmp$58(_$x) _$x
#define pp_Tok_cmp$59(_$x) _$x
#define pp_Tok_cmp$60(_$x) _$x
#define pp_Tok_cmp$61(_$x) _$x
#define pp_Tok_cmp$62(_$x) _$x
#define pp_Tok_cmp$63(_$x) _$x
#define pp_Tok_cmp$64(_$x) _$x
#define pp_Tok_cmp$65(_$x) _$x
#define pp_Tok_cmp$66(_$x) _$x
#define pp_Tok_cmp$67(_$x) _$x
#define pp_Tok_cmp$68(_$x) _$x
#define pp_Tok_cmp$69(_$x) _$x
#define pp_Tok_cmp$70(_$x) _$x
#define pp_Tok_cmp$71(_$x) _$x
#define pp_Tok_cmp$72(_$x) _$x
#define pp_Tok_cmp$73(_$x) _$x
#define pp_Tok_cmp$74(_$x) _$x
#define pp_Tok_cmp$75(_$x) _$x
#define pp_Tok_cmp$76(_$x) _$x
#define pp_Tok_cmp$77(_$x) _$x
#define pp_Tok_cmp$78(_$x) _$x
#define pp_Tok_cmp$79(_$x) _$x
#define pp_Tok_cmp$80(_$x) _$x
#define pp_Tok_cmp$81(_$x) _$x
#define pp_Tok_cmp$82(_$x) _$x
#define pp_Tok_cmp$83(_$x) _$x
#define pp_Tok_cmp$84(_$x) _$x
#define pp_Tok_cmp$85(_$x) _$x
#define pp_Tok_cmp$86(_$x) _$x
#define pp_Tok_cmp$87(_$x) _$x
#define pp_Tok_cmp$88(_$x) _$x
#define pp_Tok_cmp$89(_$x) _$x
#define pp_Tok_cmp$90(_$x) _$x
#define pp_Tok_cmp$91(_$x) _$x
#define pp_Tok_cmp$92(_$x) _$x
#define pp_Tok_cmp$93(_$x) _$x
#define pp_Tok_cmp$94(_$x) _$x
#define pp_Tok_cmp$95(_$x) _$x
#define pp_Tok_cmp$96(_$x) _$x
#define pp_Tok_cmp$97(_$x) _$x
#define pp_Tok_cmp$98(_$x) _$x
#define pp_Tok_cmp$99(_$x) _$x
#define pp_Tok_cmp$100(_$x) _$x
#define pp_Tok_cmp$101(_$x) _$x
#define pp_Tok_cmp$102(_$x) _$x
#define pp_Tok_cmp$103(_$x) _$x
#define pp_Tok_cmp$104(_$x) _$x
#define pp_Tok_cmp$105(_$x) _$x
#define pp_Tok_cmp$106(_$x) _$x
#define pp_Tok_cmp$107(_$x) _$x
#define pp_Tok_cmp$108(_$x) _$x
#define pp_Tok_cmp$109(_$x) _$x
#define pp_Tok_cmp$110(_$x) _$x
#define pp_Tok_cmp$111(_$x) _$x
#define pp_Tok_cmp$112(_$x) _$x
#define pp_Tok_cmp$113(_$x) _$x
#define pp_Tok_cmp$114(_$x) _$x
#define pp_Tok_cmp$115(_$x) _$x
#define pp_Tok_cmp$116(_$x) _$x
#define pp_Tok_cmp$117(_$x) _$x
#define pp_Tok_cmp$118(_$x) _$x
#define pp_Tok_cmp$119(_$x) _$x
#define pp_Tok_cmp$120(_$x) _$x
#define pp_Tok_cmp$121(_$x) _$x
#define pp_Tok_cmp$122(_$x) _$x
#define pp_Tok_cmp$123(_$x) _$x
#define pp_Tok_cmp$124(_$x) _$x
#define pp_Tok_cmp$125(_$x) _$x
#define pp_Tok_cmp$126(_$x) _$x
#define pp_Tok_cmp$127(_$x) _$x
#define pp_Tok_cmp$128(_$x) _$x
#define pp_Tok_cmp$129(_$x) _$x
#define pp_Tok_cmp$130(_$x) _$x
#define pp_Tok_cmp$131(_$x) _$x
#define pp_Tok_cmp$132(_$x) _$x
#define pp_Tok_cmp$133(_$x) _$x
#define pp_Tok_cmp$134(_$x) _$x
#define pp_Tok_cmp$135(_$x) _$x
#define pp_Tok_cmp$136(_$x) _$x
#define pp_Tok_cmp$137(_$x) _$x
#define pp_Tok_cmp$138(_$x) _$x
#define pp_Tok_cmp$139(_$x) _$x
#define pp_Tok_cmp$140(_$x) _$x
#define pp_Tok_cmp$141(_$x) _$x
#define pp_Tok_cmp$142(_$x) _$x
#define pp_Tok_cmp$143(_$x) _$x
#define pp_Tok_cmp$144(_$x) _$x
#define pp_Tok_cmp$145(_$x) _$x
#define pp_Tok_cmp$146(_$x) _$x
#define pp_Tok_cmp$147(_$x) _$x
#define pp_Tok_cmp$148(_$x) _$x
#define pp_Tok_cmp$149(_$x) _$x
#define pp_Tok_cmp$150(_$x) _$x
#define pp_Tok_cmp$151(_$x) _$x
#define pp_Tok_cmp$152(_$x) _$x
#define pp_Tok_cmp$153(_$x) _$x
#define pp_Tok_cmp$154(_$x) _$x
#define pp_Tok_cmp$155(_$x) _$x
#define pp_Tok_cmp$156(_$x) _$x
#define pp_Tok_cmp$157(_$x) _$x
#define pp_Tok_cmp$158(_$x) _$x
#define pp_Tok_cmp$159(_$x) _$x
#define pp_Tok_cmp$160(_$x) _$x
#define pp_Tok_cmp$161(_$x) _$x
#define pp_Tok_cmp$162(_$x) _$x
#define pp_Tok_cmp$163(_$x) _$x
#define pp_Tok_cmp$164(_$x) _$x
#define pp_Tok_cmp$165(_$x) _$x
#define pp_Tok_cmp$166(_$x) _$x
#define pp_Tok_cmp$167(_$x) _$x
#define pp_Tok_cmp$168(_$x) _$x
#define pp_Tok_cmp$169(_$x) _$x
#define pp_Tok_cmp$170(_$x) _$x
#define pp_Tok_cmp$171(_$x) _$x
#define pp_Tok_cmp$172(_$x) _$x
#define pp_Tok_cmp$173(_$x) _$x
#define pp_Tok_cmp$174(_$x) _$x
#define pp_Tok_cmp$175(_$x) _$x
#define pp_Tok_cmp$176(_$x) _$x
#define pp_Tok_cmp$177(_$x) _$x
#define pp_Tok_cmp$178(_$x) _$x
#define pp_Tok_cmp$179(_$x) _$x
#define pp_Tok_cmp$180(_$x) _$x
#define pp_Tok_cmp$181(_$x) _$x
#define pp_Tok_cmp$182(_$x) _$x
#define pp_Tok_cmp$183(_$x) _$x
#define pp_Tok_cmp$184(_$x) _$x
#define pp_Tok_cmp$185(_$x) _$x
#define pp_Tok_cmp$186(_$x) _$x
#define pp_Tok_cmp$187(_$x) _$x
#define pp_Tok_cmp$188(_$x) _$x
#define pp_Tok_cmp$189(_$x) _$x
#define pp_Tok_cmp$190(_$x) _$x
#define pp_Tok_cmp$191(_$x) _$x
#define pp_Tok_cmp$192(_$x) _$x
#define pp_Tok_cmp$193(_$x) _$x
#define pp_Tok_cmp$194(_$x) _$x
#define pp_Tok_cmp$195(_$x) _$x
#define pp_Tok_cmp$196(_$x) _$x
#define pp_Tok_cmp$197(_$x) _$x
#define pp_Tok_cmp$198(_$x) _$x
#define pp_Tok_cmp$199(_$x) _$x
#define pp_Tok_cmp$200(_$x) _$x
#define pp_Tok_cmp$201(_$x) _$x
#define pp_Tok_cmp$202(_$x) _$x
#define pp_Tok_cmp$203(_$x) _$x
#define pp_Tok_cmp$204(_$x) _$x
#define pp_Tok_cmp$205(_$x) _$x
#define pp_Tok_cmp$206(_$x) _$x
#define pp_Tok_cmp$207(_$x) _$x
#define pp_Tok_cmp$208(_$x) _$x
#define pp_Tok_cmp$209(_$x) _$x
#define pp_Tok_cmp$210(_$x) _$x
#define pp_Tok_cmp$211(_$x) _$x
#define pp_Tok_cmp$212(_$x) _$x
#define pp_Tok_cmp$213(_$x) _$x
#define pp_Tok_cmp$214(_$x) _$x
#define pp_Tok_cmp$215(_$x) _$x
#define pp_Tok_cmp$216(_$x) _$x
#define pp_Tok_cmp$217(_$x) _$x
#define pp_Tok_cmp$218(_$x) _$x
#define pp_Tok_cmp$219(_$x) _$x
#define pp_Tok_cmp$220(_$x) _$x
#define pp_Tok_cmp$221(_$x) _$x
#define pp_Tok_cmp$222(_$x) _$x
#define pp_Tok_cmp$223(_$x) _$x
#define pp_Tok_cmp$224(_$x) _$x
#define pp_Tok_cmp$225(_$x) _$x
#define pp_Tok_cmp$226(_$x) _$x
#define pp_Tok_cmp$227(_$x) _$x
#define pp_Tok_cmp$228(_$x) _$x
#define pp_Tok_cmp$229(_$x) _$x
#define pp_Tok_cmp$230(_$x) _$x
#define pp_Tok_cmp$231(_$x) _$x
#define pp_Tok_cmp$232(_$x) _$x
#define pp_Tok_cmp$233(_$x) _$x
#define pp_Tok_cmp$234(_$x) _$x
#define pp_Tok_cmp$235(_$x) _$x
#define pp_Tok_cmp$236(_$x) _$x
#define pp_Tok_cmp$237(_$x) _$x
#define pp_Tok_cmp$238(_$x) _$x
#define pp_Tok_cmp$239(_$x) _$x
#define pp_Tok_cmp$240(_$x) _$x
#define pp_Tok_cmp$241(_$x) _$x
#define pp_Tok_cmp$242(_$x) _$x
#define pp_Tok_cmp$243(_$x) _$x
#define pp_Tok_cmp$244(_$x) _$x
#define pp_Tok_cmp$245(_$x) _$x
#define pp_Tok_cmp$246(_$x) _$x
#define pp_Tok_cmp$247(_$x) _$x
#define pp_Tok_cmp$248(_$x) _$x
#define pp_Tok_cmp$249(_$x) _$x
#define pp_Tok_cmp$250(_$x) _$x
#define pp_Tok_cmp$251(_$x) _$x
#define pp_Tok_cmp$252(_$x) _$x
#define pp_Tok_cmp$253(_$x) _$x
#define pp_Tok_cmp$254(_$x) _$x
#define pp_Tok_cmp$255(_$x) _$x

/*--- const ---*/

#define pp_Tok_cmp$const(_$x) _$x
#define ignore_and_end(...) ignore_end ignore_end
#define ignore_after_const const ignore_and_end(
#define ignore_open (
#define ignore_end )

#define Tok_isConst$(_$T...) __isConstType__exec(pp_defer(__isConstType__unwrap) __isConstType__eval(_$T))
#define __isConstType__exec(...) __VA_ARGS__
#define __isConstType__unwrap(...) __VA_ARGS__
#define __isConstType__eval(_$T...) ignore_open pp_defer(pp_Tok_eql)(const, pp_cat(ignore_after_, _$T) ignore_end)
#define Tok_removeConst$(_$const_T...) pp_cat(__Tok_removeConst$__remove_, _$const_T)
#define __Tok_removeConst$__remove_const

#define pp_exec_foldr_32(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31) \
    _$Step(_$A0, pp_exec_foldr_31(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31))
#define pp_exec_foldr_33(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32) \
    _$Step(_$A0, pp_exec_foldr_32(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32))
#define pp_exec_foldr_34(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33) \
    _$Step(_$A0, pp_exec_foldr_33(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33))
#define pp_exec_foldr_35(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34) \
    _$Step(_$A0, pp_exec_foldr_34(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34))
#define pp_exec_foldr_36(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35) \
    _$Step(_$A0, pp_exec_foldr_35(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35))
#define pp_exec_foldr_37(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36) \
    _$Step(_$A0, pp_exec_foldr_36(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36))
#define pp_exec_foldr_38(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37) \
    _$Step(_$A0, pp_exec_foldr_37(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37))
#define pp_exec_foldr_39(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38) \
    _$Step(_$A0, pp_exec_foldr_38(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38))
#define pp_exec_foldr_40(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39) \
    _$Step(_$A0, pp_exec_foldr_39(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39))
#define pp_exec_foldr_41(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40) \
    _$Step(_$A0, pp_exec_foldr_40(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40))
#define pp_exec_foldr_42(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41) \
    _$Step(_$A0, pp_exec_foldr_41(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41))
#define pp_exec_foldr_43(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42) \
    _$Step(_$A0, pp_exec_foldr_42(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42))
#define pp_exec_foldr_44(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43) \
    _$Step(_$A0, pp_exec_foldr_43(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43))
#define pp_exec_foldr_45(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44) \
    _$Step(_$A0, pp_exec_foldr_44(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44))
#define pp_exec_foldr_46(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45) \
    _$Step(_$A0, pp_exec_foldr_45(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45))
#define pp_exec_foldr_47(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46) \
    _$Step(_$A0, pp_exec_foldr_46(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46))
#define pp_exec_foldr_48(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47) \
    _$Step(_$A0, pp_exec_foldr_47(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47))
#define pp_exec_foldr_49(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48) \
    _$Step(_$A0, pp_exec_foldr_48(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48))
#define pp_exec_foldr_50(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49) \
    _$Step(_$A0, pp_exec_foldr_49(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49))
#define pp_exec_foldr_51(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50) \
    _$Step(_$A0, pp_exec_foldr_50(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50))
#define pp_exec_foldr_52(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51) \
    _$Step(_$A0, pp_exec_foldr_51(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51))
#define pp_exec_foldr_53(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52) \
    _$Step(_$A0, pp_exec_foldr_52(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52))
#define pp_exec_foldr_54(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53) \
    _$Step(_$A0, pp_exec_foldr_53(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53))
#define pp_exec_foldr_55(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54) \
    _$Step(_$A0, pp_exec_foldr_54(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54))
#define pp_exec_foldr_56(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55) \
    _$Step(_$A0, pp_exec_foldr_55(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55))
#define pp_exec_foldr_57(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56) \
    _$Step(_$A0, pp_exec_foldr_56(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56))
#define pp_exec_foldr_58(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57) \
    _$Step(_$A0, pp_exec_foldr_57(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57))
#define pp_exec_foldr_59(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58) \
    _$Step(_$A0, pp_exec_foldr_58(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58))
#define pp_exec_foldr_60(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59) \
    _$Step(_$A0, pp_exec_foldr_59(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59))
#define pp_exec_foldr_61(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60) \
    _$Step(_$A0, pp_exec_foldr_60(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60))
#define pp_exec_foldr_62(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61) \
    _$Step(_$A0, pp_exec_foldr_61(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61))
#define pp_exec_foldr_63(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62) \
    _$Step(_$A0, pp_exec_foldr_62(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62))
#define pp_exec_foldr_64(_$Step, _$Init, _$A0, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62, _$A63) \
    _$Step(_$A0, pp_exec_foldr_63(_$Step, _$Init, _$A1, _$A2, _$A3, _$A4, _$A5, _$A6, _$A7, _$A8, _$A9, _$A10, _$A11, _$A12, _$A13, _$A14, _$A15, _$A16, _$A17, _$A18, _$A19, _$A20, _$A21, _$A22, _$A23, _$A24, _$A25, _$A26, _$A27, _$A28, _$A29, _$A30, _$A31, _$A32, _$A33, _$A34, _$A35, _$A36, _$A37, _$A38, _$A39, _$A40, _$A41, _$A42, _$A43, _$A44, _$A45, _$A46, _$A47, _$A48, _$A49, _$A50, _$A51, _$A52, _$A53, _$A54, _$A55, _$A56, _$A57, _$A58, _$A59, _$A60, _$A61, _$A62, _$A63))

/*--- tpl ---*/

#define tpls$(_$tpl, ...) pp_if_(pp_bool(pp_countArg(__VA_ARGS__)))( \
    pp_then_(pp_foldl(__tpls$__join, _$tpl, __VA_ARGS__)), \
    pp_else_(_$tpl) \
)
#define __tpls$__join(_$acc, _$T...) pp_join($, _$acc, _$T)

#define tpl$(_$tpl, _$T...) pp_overload(__tpl$, _$T)(_$tpl, _$T)
#define __tpl$_0(_$tpl, ...) \
    tpl$0(_$tpl)
#define __tpl$_1(_$tpl, _$T...) \
    tpl$1T(_$tpl, _$T)
#define __tpl$_2(_$tpl, _$T, _$U...) \
    tpl$1T$2U(_$tpl, _$T, _$U)
#define __tpl$_3(_$tpl, _$T, _$U, _$V...) \
    tpl$1T$2U$3V(_$tpl, _$T, _$U, _$V)
#define __tpl$_4(_$tpl, _$T, _$U, _$V, _$W...) \
    tpl$1T$2U$3V$4W(_$tpl, _$T, _$U, _$V, _$W)
#define __tpl$_5(_$tpl, _$T, _$U, _$V, _$W, _$X...) \
    tpl$1T$2U$3V$4W$5X(_$tpl, _$T, _$U, _$V, _$W, _$X)
#define __tpl$_6(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y...) \
    tpl$1T$2U$3V$4W$5X$6Y(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y)
#define __tpl$_7(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z)
#define __tpl$_8(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A)
#define __tpl$_9(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B)
#define __tpl$_10(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C \
    )
#define __tpl$_11(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D \
    )
#define __tpl$_12(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E \
    )
#define __tpl$_13(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F \
    )
#define __tpl$_14(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G \
    )
#define __tpl$_15(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G$15H( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H \
    )
#define __tpl$_16(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I...) \
    tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G$15H$16I( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I \
    )

#define tpl$0(_$tpl...) \
    _$tpl
#define tpl$T(_$tpl, _$T...) \
    pp_join($, _$tpl, _$T)
#define tpl$1T(_$tpl, _$T...) \
    pp_join($, _$tpl, _$T)
#define tpl$1T$2U(_$tpl, _$T, _$U...) \
    pp_cat5(_$tpl, $1, _$T, $2, _$U)
#define tpl$1T$2U$3V(_$tpl, _$T, _$U, _$V...) \
    pp_cat3( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        $3, \
        _$V \
    )
#define tpl$1T$2U$3V$4W(_$tpl, _$T, _$U, _$V, _$W...) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        $3, \
        _$V, \
        $4, \
        _$W \
    )
#define tpl$1T$2U$3V$4W$5X(_$tpl, _$T, _$U, _$V, _$W, _$X...) \
    pp_cat3( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        _$X \
    )
#define tpl$1T$2U$3V$4W$5X$6Y(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y...) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        _$X, \
        $6, \
        _$Y \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z...) \
    pp_cat3( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A...) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        $8, \
        _$A \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B... \
) \
    pp_cat4( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat4($8, _$A, $9, _$B) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        _$C \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat3(_$C, $11, _$D) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat5(_$C, $11, _$D, $12, _$E) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat3(pp_cat5(_$C, $11, _$D, $12, _$E), $13, _$F) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat5(pp_cat5(_$C, $11, _$D, $12, _$E), $13, _$F, $14, _$G) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G$15H( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat3(pp_cat5(_$C, $11, _$D, $12, _$E), pp_cat5($13, _$F, $14, _$G, $15), _$H) \
    )
#define tpl$1T$2U$3V$4W$5X$6Y$7Z$8A$9B$10C$11D$12E$13F$14G$15H$16I( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, $1, _$T, $2, _$U), \
        pp_cat5($3, _$V, $4, _$W, $5), \
        pp_cat5(_$X, $6, _$Y, $7, _$Z), \
        pp_cat5($8, _$A, $9, _$B, $10), \
        pp_cat5(pp_cat5(_$C, $11, _$D, $12, _$E), pp_cat5($13, _$F, $14, _$G, $15), _$H, $16, _$I) \
    )

#define tpls_(_$tpl, ...) pp_if_(pp_bool(pp_countArg(__VA_ARGS__)))( \
    pp_then_(pp_foldl(__tpls___join, _$tpl, __VA_ARGS__)), \
    pp_else_(_$tpl) \
)
#define __tpls___join(_$acc, _$T...) pp_join(_, _$acc, _$T)

#define tpl_(_$tpl, _$T...) pp_overload(__tpl_, _$T)(_$tpl, _$T)
#define __tpl__0(_$tpl, ...) \
    tpl_0(_$tpl)
#define __tpl__1(_$tpl, _$T...) \
    tpl_1T(_$tpl, _$T)
#define __tpl__2(_$tpl, _$T, _$U...) \
    tpl_1T_2U(_$tpl, _$T, _$U)
#define __tpl__3(_$tpl, _$T, _$U, _$V...) \
    tpl_1T_2U_3V(_$tpl, _$T, _$U, _$V)
#define __tpl__4(_$tpl, _$T, _$U, _$V, _$W...) \
    tpl_1T_2U_3V_4W(_$tpl, _$T, _$U, _$V, _$W)
#define __tpl__5(_$tpl, _$T, _$U, _$V, _$W, _$X...) \
    tpl_1T_2U_3V_4W_5X(_$tpl, _$T, _$U, _$V, _$W, _$X)
#define __tpl__6(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y...) \
    tpl_1T_2U_3V_4W_5X_6Y(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y)
#define __tpl__7(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z)
#define __tpl__8(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A)
#define __tpl__9(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B)
#define __tpl__10(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C \
    )
#define __tpl__11(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D \
    )
#define __tpl__12(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E \
    )
#define __tpl__13(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F \
    )
#define __tpl__14(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G \
    )
#define __tpl__15(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G_15H( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H \
    )
#define __tpl__16(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I...) \
    tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G_15H_16I( \
        _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I \
    )

#define tpl_0(_$tpl, ...) \
    _$tpl
#define tpl_T(_$tpl, _$T...) \
    pp_join(_, _$tpl, _$T)
#define tpl_1T(_$tpl, _$T...) \
    pp_join(_, _$tpl, _$T)
#define tpl_1T_2U(_$tpl, _$T, _$U...) \
    pp_cat5(_$tpl, _1, _$T, _2, _$U)
#define tpl_1T_2U_3V(_$tpl, _$T, _$U, _$V...) \
    pp_cat3( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        _3, \
        _$V \
    )
#define tpl_1T_2U_3V_4W(_$tpl, _$T, _$U, _$V, _$W...) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        _3, \
        _$V, \
        _4, \
        _$W \
    )
#define tpl_1T_2U_3V_4W_5X(_$tpl, _$T, _$U, _$V, _$W, _$X...) \
    pp_cat3( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        _$X \
    )
#define tpl_1T_2U_3V_4W_5X_6Y(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y...) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        _$X, \
        _6, \
        _$Y \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z...) \
    pp_cat3( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A(_$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A...) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        _8, \
        _$A \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B... \
) \
    pp_cat4( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat4(_8, _$A, _9, _$B) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        _$C \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat3(_$C, _11, _$D) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat5(_$C, _11, _$D, _12, _$E) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat3(pp_cat5(_$C, _11, _$D, _12, _$E), _13, _$F) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat5(pp_cat5(_$C, _11, _$D, _12, _$E), _13, _$F, _14, _$G) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G_15H( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat3(pp_cat5(_$C, _11, _$D, _12, _$E), pp_cat5(_13, _$F, _14, _$G, _15), _$H) \
    )
#define tpl_1T_2U_3V_4W_5X_6Y_7Z_8A_9B_10C_11D_12E_13F_14G_15H_16I( \
    _$tpl, _$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I... \
) \
    pp_cat5( \
        pp_cat5(_$tpl, _1, _$T, _2, _$U), \
        pp_cat5(_3, _$V, _4, _$W, _5), \
        pp_cat5(_$X, _6, _$Y, _7, _$Z), \
        pp_cat5(_8, _$A, _9, _$B, _10), \
        pp_cat5(pp_cat5(_$C, _11, _$D, _12, _$E), pp_cat5(_13, _$F, _14, _$G, _15), _$H, _16, _$I) \
    )

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_pp_common__included  */
