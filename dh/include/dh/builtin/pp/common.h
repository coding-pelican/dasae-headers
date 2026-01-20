/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2025-11-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/foundation/pp
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
#ifndef foundation_pp_common__included
#define foundation_pp_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define pp_nothing(...) pp_exec_nothing(__VA_ARGS__)
#define pp_ignore(...) pp_exec_ignore(__VA_ARGS__)
#define pp_expand(...) pp_exec_expand(__VA_ARGS__)
#define pp_defer(...) pp_exec_defer(__VA_ARGS__)

#define pp_strfy(_Tok...) pp_exec_strfy(_Tok)
#define nameOf(_Tok...) __comp_op__nameOf(_Tok, #_Tok)

#define pp_cat(_LhsTok, _RhsTok...) pp_exec_cat(_LhsTok, _RhsTok)
#define pp_cat2(_LhsTok, _RhsTok...) pp_exec_cat2(_LhsTok, _RhsTok)
#define pp_cat3(_LhsTok, _MidTok, _RhsTok...) pp_exec_cat3(_LhsTok, _MidTok, _RhsTok)
#define pp_cat4(_LhsTok, _LMidTok, _RMidTok, _RhsTok...) pp_exec_cat4(_LhsTok, _LMidTok, _RMidTok, _RhsTok)

#define pp_join(_SepTok, _LhsTok, _RhsTok...) pp_exec_join(_SepTok, _LhsTok, _RhsTok)
#define pp_join2(_SepTok, _LhsTok, _RhsTok...) pp_exec_join2(_SepTok, _LhsTok, _RhsTok)
#define pp_join3(_SepTok, _LhsTok, _MidTok, _RhsTok...) pp_exec_join3(_SepTok, _LhsTok, _MidTok, _RhsTok)
#define pp_join4(_SepTok, _LhsTok, _LMidTok, _RMidTok, _RhsTok...) pp_exec_join4(_SepTok, _LhsTok, _LMidTok, _RMidTok, _RhsTok)

#define pp_uniqTok(_Tok...) pp_exec_uniqTok(_Tok)
#define pp_uniqTokByLine(_Tok...) pp_exec_uniqTokByLine(_Tok)

#define pp_begin (
#define pp_end )

#define pp_true 1
#define pp_false 0

#define pp_not(_Cond...) pp_join(_, __pp_not, _Cond)
#define __pp_not_1 0
#define __pp_not_0 1

#define pp_and(_Lhs, _Rhs...) pp_join3(_, __pp_and, _Lhs, _Rhs)
#define __pp_and_0_0 0
#define __pp_and_0_1 0
#define __pp_and_1_0 0
#define __pp_and_1_1 1

#define pp_or(_Lhs, _Rhs...) pp_join3(_, __pp_or, _Lhs, _Rhs)
#define __pp_or_0_0 0
#define __pp_or_0_1 1
#define __pp_or_1_0 1
#define __pp_or_1_1 1

#define pp_xor(_Lhs, _Rhs...) pp_join3(_, __pp_xor, _Lhs, _Rhs)
#define __pp_xor_0_0 0
#define __pp_xor_0_1 1
#define __pp_xor_1_0 1
#define __pp_xor_1_1 0

#define pp_eq(_Lhs, _Rhs...) pp_join3(_, __pp_eq, _Lhs, _Rhs)
#define __pp_eq_0_0 1
#define __pp_eq_0_1 0
#define __pp_eq_1_0 0
#define __pp_eq_1_1 1

#define pp_ne(_Lhs, _Rhs...) pp_join3(_, __pp_ne, _Lhs, _Rhs)
#define __pp_ne_0_0 0
#define __pp_ne_0_1 1
#define __pp_ne_1_0 1
#define __pp_ne_1_1 0

#define __call__pp_if() pp_if_
#define pp_if_(_Cond...) pp_join(_, ____pp_if, _Cond)
#define ____pp_if_1(_Then, ...) ____pp_if__then _Then
#define ____pp_if__then(...) __VA_ARGS__
#define ____pp_if_0(...) pp_overload(____pp_if_0, __VA_ARGS__)(__VA_ARGS__)
#define ____pp_if_0_1(_Then...)
#define ____pp_if_0_2(_Then, _Else...) ____pp_if__else _Else
#define ____pp_if__else(...) __VA_ARGS__
#define pp_then_
#define pp_else_

#define pp_Tok_cmp$pp__some(_x) _x
#define pp_Tok_cmp$pp__none(_x) _x
#define pp_some(_Args...) (pp__some, (_Args))
#define pp_none() (pp__none, ())
#define pp_isSome(/*(_tag, _val)*/...) __step__pp_isSome(__VA_ARGS__)
#define __step__pp_isSome(...) __step__pp_isSome__emit(__step__pp_isSome__parse __VA_ARGS__)
#define __step__pp_isSome__parse(_tag, _val...) _tag, _val
#define __step__pp_isSome__emit(...) ____pp_isSome(__VA_ARGS__)
#define ____pp_isSome(_tag, _val...) pp_if_(pp_Tok_eq(_tag, pp__some))(pp_then_(pp_true), pp_else_(pp_false))
#define pp_isNone(/*(_tag, _val)*/...) pp_not(pp_isSome(__VA_ARGS__))
#define pp_orelse_(/*((_tag, _val))(_default_val)*/...) __step__pp_orelse(__VA_ARGS__)
#define __step__pp_orelse(...) __step__pp_orelse__emit(__step__pp_orelse__parse __VA_ARGS__)
#define __step__pp_orelse__parse(_pair_tag_val...) __step__pp_orelse__parsePair _pair_tag_val,
#define __step__pp_orelse__parsePair(_pair_tag_val...) _pair_tag_val
#define __step__pp_orelse__emit(...) ____pp_orelse(__VA_ARGS__)
#define ____pp_orelse(_tag, _val, _default_val...) pp_if_(pp_Tok_eq(_tag, pp__some))(pp_then_ _val, pp_else_ _default_val)

#define __call__pp_switch_() __pp_switch_
#define pp_switch_(/*(_pp_cond)(_pp_cases...)*/...) __step__pp_switch_(__step__pp_switch___parseCond __VA_ARGS__)
#define __step__pp_switch___parseCond(_pp_cond...) _pp_cond, __step__pp_switch___parseCases
#define __step__pp_switch___parseCases(_pp_cases...) _pp_cases
#define __step__pp_switch_(...) __pp_switch_(__VA_ARGS__)
#define __pp_switch_(_pp_cond, _pp_cases...) __pp_switch__expand(__pp_switch___eval(_pp_cond, _pp_cases))
#define __pp_switch__expand(...) /* evaluate 2^5 */ __pp_switch__expand5(__VA_ARGS__)
#define __pp_switch__expand5(...) __pp_switch__expand4(__pp_switch__expand4(__VA_ARGS__))
#define __pp_switch__expand4(...) __pp_switch__expand3(__pp_switch__expand3(__VA_ARGS__))
#define __pp_switch__expand3(...) __pp_switch__expand2(__pp_switch__expand2(__VA_ARGS__))
#define __pp_switch__expand2(...) __pp_switch__expand1(__pp_switch__expand1(__VA_ARGS__))
#define __pp_switch__expand1(...) __VA_ARGS__
#define __call__pp_switch___eval() __pp_switch___eval
#define __pp_switch___eval(_pp_cond, _tag, ...) \
    pp_cat(__pp_switch___eval, _tag)(_pp_cond, __VA_ARGS__)
#define __pp_switch___eval$_pp_case(_pp_cond, _pp_enum, _pp_expr, ...) \
    pp_defer(__call__pp_if)()(pp_Tok_eq(_pp_cond, _pp_enum))(_pp_expr, (__call__pp_switch___eval)()(_pp_cond, __VA_ARGS__))
#define __pp_switch___eval$_pp_default(_pp_cond, _pp_expr...) \
    _pp_expr

#define pp_case_(/*(_pp_enum)(_pp_expr)*/...) __step__pp_case_(__step__pp_case___parse __VA_ARGS__)
#define __step__pp_case___parse(_pp_enum...) _pp_enum,
#define __step__pp_case_(...) __pp_case_(__VA_ARGS__)
#define __pp_case_(_pp_enum, _pp_expr...) $_pp_case, _pp_enum, _pp_expr
#define pp_default_(_pp_expr...) __pp_default_(_pp_expr)
#define __pp_default_(_pp_expr...) $_pp_default, _pp_expr

#define pp_countArg(_Args...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * pp_countArg() => 0 \
     * pp_countArg(a0, a1, a2, ..., a31) => 32 \
     */ \
    pp_exec_countArgs(_Args)
#define pp_overload(_Name, ...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * func_(_Name_With_Params, T_Return, ...) \
     * => pp_overload(func, __VA_ARGS__)(_Name_With_Params, T_Return, __VA_ARGS__) \
     * func_0(_Name_With_Params, T_Return, ...): \
     * => fn_(_Name_With_Params, T_Return) \
     * func_1(_Name_With_Params, T_Return, _Body...): \
     * => fn_scope(_Name_With_Params, T_Return) _Body $unscoped \
     * func_2(_Name_With_Params, T_Return, _Expand_Type, _Body...): \
     * => pp_cat(fn_scope_, _Expand_Type)(_Name_With_Params, T_Return) \
    _Body pp_cat(unscoped_, _Expand_Type) \
     */ \
    pp_exec_overload(_Name, __VA_ARGS__)
#define pp_foreach(_Macro, _Name, ...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * pp_foreach(macro, name, x, y, z): \
     * => macro(name, x) macro(name, y) macro(name, z) \
     */ \
    pp_exec_foreach_(pp_exec_foreach_NARG(__VA_ARGS__), _Macro, _Name, __VA_ARGS__)

#define tpl_id(_tpl, _T...) pp_join($, _tpl, _T)
#define tpl_fn_(_tpl_T, _id, _Ret_T...) fn_((tpl_id(_tpl_T, _id))(_Ret_T))
#define tpl_T(_tpl, _T...) pp_join($, _tpl, _T)
#define tpl_Ret(_tpl, _T...) tpl_T(Ret, tpl_id(_tpl, _T))

#define tpl_id$T(_tpl, _T...) \
    pp_join($, _tpl, _T)
#define tpl_id$1T(_tpl, _T...) \
    pp_join($, _tpl, _T)
#define tpl_id$1T$2U(_tpl, _T, _U...) \
    pp_cat(_tpl, pp_cat2(pp_cat($1, _T), pp_cat($2, _U)))
#define tpl_id$2T$2U$3V(_tpl, _T, _U, _V...) \
    pp_cat(_tpl, pp_cat3(pp_cat($1, _T), pp_cat($2, _U), pp_cat($3, _V)))
#define tpl_id$2T$2U$3V$4W(_tpl, _T, _U, _V, _W...) \
    pp_cat(_tpl, pp_cat4(pp_cat($1, _T), pp_cat($2, _U), pp_cat($3, _V), pp_cat($4, _W)))
#define tpl_fn$T(_tpl_w_T_and_Params, _Ret_T...) \
    fn_((tpl_id$T _tpl_w_T_and_Params)(_Ret_T))
#define tpl_fn$1T(_tpl_w_T_and_Params, _Ret_T...) \
    fn_((tpl_id$1T _tpl_w_T_and_Params)(_Ret_T))
#define tpl_fn$1T$2U(_tpl_w_T_w_U_and_Params, _Ret_T...) \
    fn_((tpl_id$1T$2U _tpl_w_T_w_U_and_Params)(_Ret_T))
#define tpl_fn$2T$2U$3V(_tpl_w_T_w_U_w_V_and_Params, _Ret_T...) \
    fn_((tpl_id$2T$2U$3V _tpl_w_T_w_U_w_V_and_Params)(_Ret_T))
#define tpl_fn$2T$2U$3V$4W(_tpl_w_T_w_U_w_V_w_W_and_Params, _Ret_T...) \
    fn_((tpl_id$2T$2U$3V$4W _tpl_w_T_w_U_w_V_w_W_and_Params)(_Ret_T))

/*========== Macros and Definitions =========================================*/

#define pp_exec_nothing(...)
#define pp_exec_ignore(...)
#define pp_exec_expand(...) __VA_ARGS__
#define pp_exec_defer(...) __VA_ARGS__ pp_exec_nothing()

#define pp_exec_strfy(_Tok...) #_Tok
#define __comp_op__nameOf(_Tok, _Str...) ((void)(_Tok), #_Str)

#define pp_exec_cat(_LhsTok, _RhsTok...) _LhsTok##_RhsTok
#define pp_exec_cat2(_LhsTok, _RhsTok...) _LhsTok##_RhsTok
#define pp_exec_cat3(_LhsTok, _MidTok, _RhsTok...) _LhsTok##_MidTok##_RhsTok
#define pp_exec_cat4(_LhsTok, _LMidTok, _RMidTok, _RhsTok...) _LhsTok##_LMidTok##_RMidTok##_RhsTok

#define pp_exec_join(_SepTok, _LhsTok, _RhsTok...) _LhsTok##_SepTok##_RhsTok
#define pp_exec_join2(_SepTok, _LhsTok, _RhsTok...) _LhsTok##_SepTok##_RhsTok
#define pp_exec_join3(_SepTok, _LhsTok, _MidTok, _RhsTok...) _LhsTok##_SepTok##_MidTok##_SepTok##_RhsTok
#define pp_exec_join4(_SepTok, _LhsTok, _LMidTok, _RMidTok, _RhsTok...) _LhsTok##_SepTok##_LMidTok##_SepTok##_RMidTok##_SepTok##_RhsTok

#define pp_exec_uniqTok(_Tok...) pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)
#define pp_exec_uniqTokByLine(_Tok...) pp_join(_, pp_join2(_, _, __LINE__), _Tok)

#define pp_exec_countArgs(...) pp_exec_countArgs__selectArgCountInListRseqN(__VA_OPT__(, ) __VA_ARGS__, pp_exec_countArgs__getListRseqN())
#define pp_exec_countArgs__selectArgCountInListRseqN(_Args...) pp_exec_countArgs__argN(_Args)
#define pp_exec_countArgs__getListRseqN() 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_exec_countArgs__argN( \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _N, \
    ... \
) \
    _N
#define pp_exec_overload(_Name, ...) pp_join(_, _Name, pp_countArg(__VA_ARGS__))

// pp_foreach _Macro implementation (handles up to 16 arguments)
#define pp_exec_foreach_(N, _Macro, _Name, ...) pp_join(__, pp_exec_foreach, N)(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach_NARG(...) pp_exec_foreach_NARG_(__VA_ARGS__, pp_exec_foreach_RSEQ_N())
#define pp_exec_foreach_NARG_(...) pp_exec_foreach_ARG_N(__VA_ARGS__)
#define pp_exec_foreach_ARG_N( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _N, \
    ... \
) \
    _N
#define pp_exec_foreach_RSEQ_N() 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_exec_foreach__1(_Macro, _Name, _Arg) _Macro(_Name, _Arg)
#define pp_exec_foreach__2(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__1(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__3(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__2(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__4(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__3(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__5(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__4(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__6(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__5(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__7(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__6(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__8(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__7(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__9(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__8(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__10(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__9(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__11(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__10(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__12(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__11(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__13(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__12(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__14(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__13(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__15(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__14(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__16(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__15(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__17(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__16(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__18(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__17(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__19(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__18(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__20(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__19(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__21(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__20(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__22(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__21(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__23(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__22(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__24(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__23(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__25(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__24(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__26(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__25(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__27(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__26(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__28(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__27(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__29(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__28(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__30(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__29(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__31(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__30(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__32(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__31(_Macro, _Name, __VA_ARGS__)

#define pp_compl(_b) pp_cat(__pp_compl_, _b)
#define __pp_compl_0 1
#define __pp_compl_1 0

#define pp_bitand(_x) pp_cat(__pp_bitand_, _x)
#define __pp_bitand_0(_y) 0
#define __pp_bitand_1(_y) _y
#define pp_bitor(_x) pp_cat(__pp_bitor_, _x)
#define __pp_bitor_0(_y) 0
#define __pp_bitor_1(_y) _y
#define pp_bitnot(_x) pp_cat(__pp_bitnot_, _x)
#define __pp_bitnot_0 1
#define __pp_bitnot_1 0
#define pp_bitxor(_x) pp_cat(__pp_bitxor_, _x)
#define __pp_bitxor_0(_y) 0
#define __pp_bitxor_1(_y) _y

#define __pp_check_n(_x, _n, ...) _n
#define pp_check(...) __pp_check_n(__VA_ARGS__, 0, )
#define pp_probe(_x) _x, 1,

#define pp_isParen(_x) pp_check(pp_isParenProbe _x)
#define pp_isParenProbe(...) pp_probe(~)

#define pp_Tok_not(_x) pp_check(pp_cat(__pp_Tok_not_, _x))
#define __pp_Tok_not_0 pp_probe(~)

#define pp_bool(_x) pp_compl(pp_Tok_not(_x))

#define pp_iif(_c) pp_cat(__pp_iif_, _c)
#define __pp_iif_0(_t, ...) __VA_ARGS__
#define __pp_iif_1(_t, ...) _t
#define pp_Tok_if_(_c) pp_iif(pp_bool(_c))
#define pp_Tok_then_(...) __VA_ARGS__
#define pp_Tok_else_(...) __VA_ARGS__

#define pp_Tok_prim_ord(x, y) pp_isParen(pp_Tok_cmp$##x(pp_Tok_cmp$##y)(()))
#define pp_Tok_isComparable(x) pp_isParen(pp_cat(pp_Tok_cmp$, x)(()))
#define pp_Tok_ne(x, y) pp_iif(pp_bitand(pp_Tok_isComparable(x))(pp_Tok_isComparable(y)))(pp_Tok_prim_ord, 1 pp_ignore)(x, y)
#define pp_Tok_eq(x, y) pp_compl(pp_Tok_ne(x, y))

#define pp_comma() ,
#define pp_comma_if_(_n) pp_Tok_if_(_n)(pp_comma, pp_ignore)()

#define pp_Tok_cmp$const(x) x
#define ignore_and_end(...) ignore_end ignore_end
#define ignore_after_const const ignore_and_end(
#define ignore_open (
#define ignore_end )

#define Tok_isConst$(_T...) __isConstType__exec(pp_defer(__isConstType__unwrap) __isConstType__eval(_T))
#define __isConstType__exec(...) __VA_ARGS__
#define __isConstType__unwrap(...) __VA_ARGS__
#define __isConstType__eval(_T...) ignore_open pp_defer(pp_Tok_eq)(const, pp_cat(ignore_after_, _T) ignore_end)
#define Tok_removeConst$(_const_T...) pp_cat(__Tok_removeConst$__remove_, _const_T)
#define __Tok_removeConst$__remove_const

#define pp_Tok_cmp$0(_x) _x
#define pp_Tok_cmp$1(_x) _x
#define pp_Tok_cmp$2(_x) _x
#define pp_Tok_cmp$3(_x) _x
#define pp_Tok_cmp$4(_x) _x
#define pp_Tok_cmp$5(_x) _x
#define pp_Tok_cmp$6(_x) _x
#define pp_Tok_cmp$7(_x) _x
#define pp_Tok_cmp$8(_x) _x
#define pp_Tok_cmp$10(_x) _x
#define pp_Tok_cmp$11(_x) _x
#define pp_Tok_cmp$12(_x) _x
#define pp_Tok_cmp$13(_x) _x
#define pp_Tok_cmp$14(_x) _x
#define pp_Tok_cmp$15(_x) _x
#define pp_Tok_cmp$16(_x) _x
#define pp_Tok_cmp$17(_x) _x
#define pp_Tok_cmp$18(_x) _x
#define pp_Tok_cmp$19(_x) _x
#define pp_Tok_cmp$20(_x) _x
#define pp_Tok_cmp$21(_x) _x
#define pp_Tok_cmp$22(_x) _x
#define pp_Tok_cmp$23(_x) _x
#define pp_Tok_cmp$24(_x) _x
#define pp_Tok_cmp$25(_x) _x
#define pp_Tok_cmp$26(_x) _x
#define pp_Tok_cmp$27(_x) _x
#define pp_Tok_cmp$28(_x) _x
#define pp_Tok_cmp$29(_x) _x
#define pp_Tok_cmp$30(_x) _x
#define pp_Tok_cmp$31(_x) _x

#define pp_Tok_cmp$32(_x) _x
#define pp_Tok_cmp$33(_x) _x
#define pp_Tok_cmp$34(_x) _x
#define pp_Tok_cmp$35(_x) _x
#define pp_Tok_cmp$36(_x) _x
#define pp_Tok_cmp$37(_x) _x
#define pp_Tok_cmp$38(_x) _x
#define pp_Tok_cmp$39(_x) _x
#define pp_Tok_cmp$40(_x) _x
#define pp_Tok_cmp$41(_x) _x
#define pp_Tok_cmp$42(_x) _x
#define pp_Tok_cmp$43(_x) _x
#define pp_Tok_cmp$44(_x) _x
#define pp_Tok_cmp$45(_x) _x
#define pp_Tok_cmp$46(_x) _x
#define pp_Tok_cmp$47(_x) _x
#define pp_Tok_cmp$48(_x) _x
#define pp_Tok_cmp$49(_x) _x
#define pp_Tok_cmp$50(_x) _x
#define pp_Tok_cmp$51(_x) _x
#define pp_Tok_cmp$52(_x) _x
#define pp_Tok_cmp$53(_x) _x
#define pp_Tok_cmp$54(_x) _x
#define pp_Tok_cmp$55(_x) _x
#define pp_Tok_cmp$56(_x) _x
#define pp_Tok_cmp$57(_x) _x
#define pp_Tok_cmp$58(_x) _x
#define pp_Tok_cmp$59(_x) _x
#define pp_Tok_cmp$60(_x) _x
#define pp_Tok_cmp$61(_x) _x
#define pp_Tok_cmp$62(_x) _x
#define pp_Tok_cmp$63(_x) _x
#define pp_Tok_cmp$64(_x) _x
#define pp_Tok_cmp$65(_x) _x
#define pp_Tok_cmp$66(_x) _x
#define pp_Tok_cmp$67(_x) _x
#define pp_Tok_cmp$68(_x) _x
#define pp_Tok_cmp$69(_x) _x
#define pp_Tok_cmp$70(_x) _x
#define pp_Tok_cmp$71(_x) _x
#define pp_Tok_cmp$72(_x) _x
#define pp_Tok_cmp$73(_x) _x
#define pp_Tok_cmp$74(_x) _x
#define pp_Tok_cmp$75(_x) _x
#define pp_Tok_cmp$76(_x) _x
#define pp_Tok_cmp$77(_x) _x
#define pp_Tok_cmp$78(_x) _x
#define pp_Tok_cmp$79(_x) _x
#define pp_Tok_cmp$80(_x) _x
#define pp_Tok_cmp$81(_x) _x
#define pp_Tok_cmp$82(_x) _x
#define pp_Tok_cmp$83(_x) _x
#define pp_Tok_cmp$84(_x) _x
#define pp_Tok_cmp$85(_x) _x
#define pp_Tok_cmp$86(_x) _x
#define pp_Tok_cmp$87(_x) _x
#define pp_Tok_cmp$88(_x) _x
#define pp_Tok_cmp$89(_x) _x
#define pp_Tok_cmp$90(_x) _x
#define pp_Tok_cmp$91(_x) _x
#define pp_Tok_cmp$92(_x) _x
#define pp_Tok_cmp$93(_x) _x
#define pp_Tok_cmp$94(_x) _x
#define pp_Tok_cmp$95(_x) _x
#define pp_Tok_cmp$96(_x) _x
#define pp_Tok_cmp$97(_x) _x
#define pp_Tok_cmp$98(_x) _x
#define pp_Tok_cmp$99(_x) _x
#define pp_Tok_cmp$100(_x) _x
#define pp_Tok_cmp$101(_x) _x
#define pp_Tok_cmp$102(_x) _x
#define pp_Tok_cmp$103(_x) _x
#define pp_Tok_cmp$104(_x) _x
#define pp_Tok_cmp$105(_x) _x
#define pp_Tok_cmp$106(_x) _x
#define pp_Tok_cmp$107(_x) _x
#define pp_Tok_cmp$108(_x) _x
#define pp_Tok_cmp$109(_x) _x
#define pp_Tok_cmp$110(_x) _x
#define pp_Tok_cmp$111(_x) _x
#define pp_Tok_cmp$112(_x) _x
#define pp_Tok_cmp$113(_x) _x
#define pp_Tok_cmp$114(_x) _x
#define pp_Tok_cmp$115(_x) _x
#define pp_Tok_cmp$116(_x) _x
#define pp_Tok_cmp$117(_x) _x
#define pp_Tok_cmp$118(_x) _x
#define pp_Tok_cmp$119(_x) _x
#define pp_Tok_cmp$120(_x) _x
#define pp_Tok_cmp$121(_x) _x
#define pp_Tok_cmp$122(_x) _x
#define pp_Tok_cmp$123(_x) _x
#define pp_Tok_cmp$124(_x) _x
#define pp_Tok_cmp$125(_x) _x
#define pp_Tok_cmp$126(_x) _x
#define pp_Tok_cmp$127(_x) _x
#define pp_Tok_cmp$128(_x) _x
#define pp_Tok_cmp$129(_x) _x
#define pp_Tok_cmp$130(_x) _x
#define pp_Tok_cmp$131(_x) _x
#define pp_Tok_cmp$132(_x) _x
#define pp_Tok_cmp$133(_x) _x
#define pp_Tok_cmp$134(_x) _x
#define pp_Tok_cmp$135(_x) _x
#define pp_Tok_cmp$136(_x) _x
#define pp_Tok_cmp$137(_x) _x
#define pp_Tok_cmp$138(_x) _x
#define pp_Tok_cmp$139(_x) _x
#define pp_Tok_cmp$140(_x) _x
#define pp_Tok_cmp$141(_x) _x
#define pp_Tok_cmp$142(_x) _x
#define pp_Tok_cmp$143(_x) _x
#define pp_Tok_cmp$144(_x) _x
#define pp_Tok_cmp$145(_x) _x
#define pp_Tok_cmp$146(_x) _x
#define pp_Tok_cmp$147(_x) _x
#define pp_Tok_cmp$148(_x) _x
#define pp_Tok_cmp$149(_x) _x
#define pp_Tok_cmp$150(_x) _x
#define pp_Tok_cmp$151(_x) _x
#define pp_Tok_cmp$152(_x) _x
#define pp_Tok_cmp$153(_x) _x
#define pp_Tok_cmp$154(_x) _x
#define pp_Tok_cmp$155(_x) _x
#define pp_Tok_cmp$156(_x) _x
#define pp_Tok_cmp$157(_x) _x
#define pp_Tok_cmp$158(_x) _x
#define pp_Tok_cmp$159(_x) _x
#define pp_Tok_cmp$160(_x) _x
#define pp_Tok_cmp$161(_x) _x
#define pp_Tok_cmp$162(_x) _x
#define pp_Tok_cmp$163(_x) _x
#define pp_Tok_cmp$164(_x) _x
#define pp_Tok_cmp$165(_x) _x
#define pp_Tok_cmp$166(_x) _x
#define pp_Tok_cmp$167(_x) _x
#define pp_Tok_cmp$168(_x) _x
#define pp_Tok_cmp$169(_x) _x
#define pp_Tok_cmp$170(_x) _x
#define pp_Tok_cmp$171(_x) _x
#define pp_Tok_cmp$172(_x) _x
#define pp_Tok_cmp$173(_x) _x
#define pp_Tok_cmp$174(_x) _x
#define pp_Tok_cmp$175(_x) _x
#define pp_Tok_cmp$176(_x) _x
#define pp_Tok_cmp$177(_x) _x
#define pp_Tok_cmp$178(_x) _x
#define pp_Tok_cmp$179(_x) _x
#define pp_Tok_cmp$180(_x) _x
#define pp_Tok_cmp$181(_x) _x
#define pp_Tok_cmp$182(_x) _x
#define pp_Tok_cmp$183(_x) _x
#define pp_Tok_cmp$184(_x) _x
#define pp_Tok_cmp$185(_x) _x
#define pp_Tok_cmp$186(_x) _x
#define pp_Tok_cmp$187(_x) _x
#define pp_Tok_cmp$188(_x) _x
#define pp_Tok_cmp$189(_x) _x
#define pp_Tok_cmp$190(_x) _x
#define pp_Tok_cmp$191(_x) _x
#define pp_Tok_cmp$192(_x) _x
#define pp_Tok_cmp$193(_x) _x
#define pp_Tok_cmp$194(_x) _x
#define pp_Tok_cmp$195(_x) _x
#define pp_Tok_cmp$196(_x) _x
#define pp_Tok_cmp$197(_x) _x
#define pp_Tok_cmp$198(_x) _x
#define pp_Tok_cmp$199(_x) _x
#define pp_Tok_cmp$200(_x) _x
#define pp_Tok_cmp$201(_x) _x
#define pp_Tok_cmp$202(_x) _x
#define pp_Tok_cmp$203(_x) _x
#define pp_Tok_cmp$204(_x) _x
#define pp_Tok_cmp$205(_x) _x
#define pp_Tok_cmp$206(_x) _x
#define pp_Tok_cmp$207(_x) _x
#define pp_Tok_cmp$208(_x) _x
#define pp_Tok_cmp$209(_x) _x
#define pp_Tok_cmp$210(_x) _x
#define pp_Tok_cmp$211(_x) _x
#define pp_Tok_cmp$212(_x) _x
#define pp_Tok_cmp$213(_x) _x
#define pp_Tok_cmp$214(_x) _x
#define pp_Tok_cmp$215(_x) _x
#define pp_Tok_cmp$216(_x) _x
#define pp_Tok_cmp$217(_x) _x
#define pp_Tok_cmp$218(_x) _x
#define pp_Tok_cmp$219(_x) _x
#define pp_Tok_cmp$220(_x) _x
#define pp_Tok_cmp$221(_x) _x
#define pp_Tok_cmp$222(_x) _x
#define pp_Tok_cmp$223(_x) _x
#define pp_Tok_cmp$224(_x) _x
#define pp_Tok_cmp$225(_x) _x
#define pp_Tok_cmp$226(_x) _x
#define pp_Tok_cmp$227(_x) _x
#define pp_Tok_cmp$228(_x) _x
#define pp_Tok_cmp$229(_x) _x
#define pp_Tok_cmp$230(_x) _x
#define pp_Tok_cmp$231(_x) _x
#define pp_Tok_cmp$232(_x) _x
#define pp_Tok_cmp$233(_x) _x
#define pp_Tok_cmp$234(_x) _x
#define pp_Tok_cmp$235(_x) _x
#define pp_Tok_cmp$236(_x) _x
#define pp_Tok_cmp$237(_x) _x
#define pp_Tok_cmp$238(_x) _x
#define pp_Tok_cmp$239(_x) _x
#define pp_Tok_cmp$240(_x) _x
#define pp_Tok_cmp$241(_x) _x
#define pp_Tok_cmp$242(_x) _x
#define pp_Tok_cmp$243(_x) _x
#define pp_Tok_cmp$244(_x) _x
#define pp_Tok_cmp$245(_x) _x
#define pp_Tok_cmp$246(_x) _x
#define pp_Tok_cmp$247(_x) _x
#define pp_Tok_cmp$248(_x) _x
#define pp_Tok_cmp$249(_x) _x
#define pp_Tok_cmp$250(_x) _x
#define pp_Tok_cmp$251(_x) _x
#define pp_Tok_cmp$252(_x) _x
#define pp_Tok_cmp$253(_x) _x
#define pp_Tok_cmp$254(_x) _x
#define pp_Tok_cmp$255(_x) _x

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_pp_common__included  */
