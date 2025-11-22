/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/builtin/pp
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

#ifndef builtin_pp_common__included
#define builtin_pp_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define pp_nothing(...) pp_exec_nothing(__VA_ARGS__)
#define pp_ignore(...) pp_exec_ignore(__VA_ARGS__)
#define pp_expand(...) pp_exec_expand(__VA_ARGS__)
#define pp_defer(...) pp_exec_defer(__VA_ARGS__)

#define pp_stringify(_Tok...) pp_exec_stringify(_Tok)
#define nameOf(_Tok...) comp_op__nameOf(_Tok, #_Tok)

#define pp_cat(_LhsTok, _RhsTok...) pp_exec_cat(_LhsTok, _RhsTok)
#define pp_cat2(_LhsTok, _RhsTok...) pp_exec_cat2(_LhsTok, _RhsTok)
#define pp_cat3(_LhsTok, _Tok_mid, _RhsTok...) pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok)

#define pp_join(_SepTok, _LhsTok, _RhsTok...) pp_exec_join(_SepTok, _LhsTok, _RhsTok)
#define pp_join2(_SepTok, _LhsTok, _RhsTok...) pp_exec_join2(_SepTok, _LhsTok, _RhsTok)
#define pp_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok)

#define pp_uniqTok(_Tok...) pp_exec_uniqTok(_Tok)
#define pp_uniqTokByLine(_Tok...) pp_exec_uniqTokByLine(_Tok)

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
#define __pp_or_1_0 0
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

#define pp_if_(Cond...) pp_join(_, __pp_if, Cond)
#define __pp_if_1(_Than, _Else...) pp_expand _Than
#define __pp_if_0(_Than, _Else) pp_expand _Else
#define pp_then_
#define pp_else_

#define tpl_id(_tpl, _T...) pp_join($, _tpl, _T)
#define tpl_fn_(_tpl_T, _id, _Ret...) fn_((tpl_id(_tpl_T, _id))(_Ret))
#define tpl_T(_tpl, _T...) pp_join($, _tpl, _T)
#define tpl_Ret(_tpl, _T...) tpl_T(Ret, tpl_id(_tpl, _T))

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

/*========== Macros Implementation ==========================================*/

#define pp_exec_nothing(...)
#define pp_exec_ignore(...)
#define pp_exec_expand(...) __VA_ARGS__
#define pp_exec_defer(...) __VA_ARGS__ pp_exec_nothing()

#define pp_exec_stringify(_Tok...) #_Tok
#define comp_op__nameOf(_Tok, _Str...) ((void)(_Tok), #_Str)

#define pp_exec_cat(_LhsTok, _RhsTok...) _LhsTok##_RhsTok
#define pp_exec_cat2(_LhsTok, _RhsTok...) _LhsTok##_RhsTok
#define pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_Tok_mid##_RhsTok

#define pp_exec_join(_SepTok, _LhsTok, _RhsTok...) _LhsTok##_SepTok##_RhsTok
#define pp_exec_join2(_SepTok, _LhsTok, _RhsTok...) _LhsTok##_SepTok##_RhsTok
#define pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_SepTok##_Tok_mid##_SepTok##_RhsTok

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

#define pp_Tok_prim_ord(x, y) pp_isParen(pp_Tok_cmp__##x(pp_Tok_cmp__##y)(()))
#define pp_Tok_isComparable(x) pp_isParen(pp_cat(pp_Tok_cmp__, x)(()))
#define pp_Tok_ne(x, y) pp_iif(pp_bitand(pp_Tok_isComparable(x))(pp_Tok_isComparable(y)))(pp_Tok_prim_ord, 1 pp_ignore)(x, y)
#define pp_Tok_eq(x, y) pp_compl(pp_Tok_ne(x, y))

#define pp_comma() ,
#define pp_comma_if_(_n) pp_Tok_if_(_n)(pp_comma, pp_ignore)()

#define pp_Tok_cmp__const(x) x
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_pp_common__included  */
