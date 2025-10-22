/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/core/pp
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

#ifndef CORE_PP_COMMON_INCLUDED
#define CORE_PP_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define pp_nothing(...) pp_exec_nothing(__VA_ARGS__)
#define pp_ignore(...)  pp_exec_ignore(__VA_ARGS__)
#define pp_expand(...)  pp_exec_expand(__VA_ARGS__)
#define pp_defer(...)   pp_exec_defer(__VA_ARGS__)

#define pp_stringify(_Tok...) pp_exec_stringify(_Tok)
#define nameOf(_Tok...)       comp_op__nameOf(_Tok, #_Tok)

#define pp_cat(_LhsTok, _RhsTok...)            pp_exec_cat(_LhsTok, _RhsTok)
#define pp_cat2(_LhsTok, _RhsTok...)           pp_exec_cat2(_LhsTok, _RhsTok)
#define pp_cat3(_LhsTok, _Tok_mid, _RhsTok...) pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok)

#define pp_join(_SepTok, _LhsTok, _RhsTok...)            pp_exec_join(_SepTok, _LhsTok, _RhsTok)
#define pp_join2(_SepTok, _LhsTok, _RhsTok...)           pp_exec_join2(_SepTok, _LhsTok, _RhsTok)
#define pp_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok)

#define pp_uniqTok(_Tok...)       pp_exec_uniqTok(_Tok)
#define pp_uniqTokByLine(_Tok...) pp_exec_uniqTokByLine(_Tok)

#define pp_not_(_Cond...) pp_join(_, __pp_not, _Cond)
#define __pp_not_1        0
#define __pp_not_0        1

#define pp_and_(_Lhs, _Rhs...) pp_join3(_, __pp_and, _Lhs, _Rhs)
#define __pp_and_0_0           0
#define __pp_and_0_1           0
#define __pp_and_1_0           0
#define __pp_and_1_1           1

#define pp_or_(_Lhs, _Rhs...) pp_join3(_, __pp_or, _Lhs, _Rhs)
#define __pp_or_0_0           0
#define __pp_or_0_1           1
#define __pp_or_1_0           0
#define __pp_or_1_1           1

#define pp_xor_(_Lhs, _Rhs...) pp_join3(_, __pp_xor, _Lhs, _Rhs)
#define __pp_xor_0_0           0
#define __pp_xor_0_1           1
#define __pp_xor_1_0           1
#define __pp_xor_1_1           0

#define pp_if_(Cond...)            pp_join(_, __pp_if, Cond)
#define __pp_if_1(_Than, _Else...) pp_expand _Than
#define __pp_if_0(_Than, _Else)    pp_expand _Else
#define pp_than_
#define pp_else_

#define tpl_id(_tpl, _T...)           pp_join(_, _tpl, _T)
#define tpl_fn_(_tpl_T, _id, _Ret...) fn_((tpl_id(_tpl_T, _id))(_Ret))
#define tpl_T(_tpl, _T...)            pp_join($, _tpl, _T)
#define tpl_Ret(_tpl, _T...)          tpl_T(Ret, tpl_id(_tpl, _T))

#define pp_countArg(_Args...) \
    /** \
     * @note handles up to 16 arguments \
     * \
     * pp_countArg() => 0 \
     * pp_countArg(x, y, z) => 16 \
     */ \
    pp_exec_countArgs(_Args)
#define pp_overload(_Name, ...) \
    /** \
     * @note handles up to 16 arguments \
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
     * @note handles up to 16 arguments \
     * \
     * pp_foreach(macro, name, x, y, z): \
     * => macro(name, x) macro(name, y) macro(name, z) \
     */ \
    pp_exec_foreach_(pp_exec_foreach_NARG(__VA_ARGS__), _Macro, _Name, __VA_ARGS__)

// lit_num(11,644,473,600u) => 11644473600u
#define lit_num(_Comma_Sep_Lits...)      comp_syn__lit_num(_Comma_Sep_Lits)
#define lit_num$(_T, _Comma_Sep_Lits...) comp_syn__lit_num$(_T, _Comma_Sep_Lits)

/*========== Macros Implementation ==========================================*/

#define pp_exec_nothing(...)
#define pp_exec_ignore(...)
#define pp_exec_expand(...) __VA_ARGS__
#define pp_exec_defer(...)  __VA_ARGS__ pp_exec_nothing()

#define pp_exec_stringify(_Tok...)     #_Tok
#define comp_op__nameOf(_Tok, _Str...) ((void)(_Tok), #_Str)

#define pp_exec_cat(_LhsTok, _RhsTok...)            _LhsTok##_RhsTok
#define pp_exec_cat2(_LhsTok, _RhsTok...)           _LhsTok##_RhsTok
#define pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_Tok_mid##_RhsTok

#define pp_exec_join(_SepTok, _LhsTok, _RhsTok...)            _LhsTok##_SepTok##_RhsTok
#define pp_exec_join2(_SepTok, _LhsTok, _RhsTok...)           _LhsTok##_SepTok##_RhsTok
#define pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_SepTok##_Tok_mid##_SepTok##_RhsTok

#define pp_exec_uniqTok(_Tok...)       pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)
#define pp_exec_uniqTokByLine(_Tok...) pp_join(_, pp_join2(_, _, __LINE__), _Tok)

#define pp_exec_countArgs(...) \
    pp_exec_countArgs__selectArgCountInListRseqN( \
        __VA_OPT__(, ) __VA_ARGS__, pp_exec_countArgs__getListRseqN() \
    )
#define pp_exec_countArgs__selectArgCountInListRseqN(_Args...) \
    pp_exec_countArgs__argN(_Args)
#define pp_exec_countArgs__getListRseqN() \
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_exec_countArgs__argN( \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _N, ... \
)                                    _N
#define pp_exec_overload(_Name, ...) pp_join(_, _Name, pp_countArg(__VA_ARGS__))

// pp_foreach _Macro implementation (handles up to 16 arguments)
#define pp_exec_foreach_(N, _Macro, _Name, ...) \
    pp_join(__, pp_exec_foreach, N)(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach_NARG(...) \
    pp_exec_foreach_NARG_(__VA_ARGS__, pp_exec_foreach_RSEQ_N())
#define pp_exec_foreach_NARG_(...) \
    pp_exec_foreach_ARG_N(__VA_ARGS__)
#define pp_exec_foreach_ARG_N( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N
#define pp_exec_foreach_RSEQ_N() \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_exec_foreach__1(_Macro, _Name, _Arg)       _Macro(_Name, _Arg)
#define pp_exec_foreach__2(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__1(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__3(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__2(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__4(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__3(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__5(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__4(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__6(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__5(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__7(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__6(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__8(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__7(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__9(_Macro, _Name, _Arg, ...)  _Macro(_Name, _Arg) pp_exec_foreach__8(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__10(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__9(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__11(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__10(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__12(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__11(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__13(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__12(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__14(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__13(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__15(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__14(_Macro, _Name, __VA_ARGS__)
#define pp_exec_foreach__16(_Macro, _Name, _Arg, ...) _Macro(_Name, _Arg) pp_exec_foreach__15(_Macro, _Name, __VA_ARGS__)

#define comp_syn__lit_num(_Comma_Sep_Lits...) \
    pp_join(__, comp_syn__lit_num, pp_countArg(_Comma_Sep_Lits))(_Comma_Sep_Lits)
/* Handle different numbers of arguments */
#define comp_syn__lit_num__1(_Num1) \
    _Num1
#define comp_syn__lit_num__2(_Num1, _Num2) \
    pp_cat(_Num1, _Num2)
#define comp_syn__lit_num__3(_Num1, _Num2, _Num3) \
    pp_cat3(_Num1, _Num2, _Num3)
#define comp_syn__lit_num__4(_Num1, _Num2, _Num3, _Num4) \
    pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4)
#define comp_syn__lit_num__5(_Num1, _Num2, _Num3, _Num4, _Num5) \
    pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4), _Num5)
#define comp_syn__lit_num__6(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6) \
    pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6)
#define comp_syn__lit_num__7(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6), _Num7)
#define comp_syn__lit_num__8(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7, _Num8) \
    pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), pp_cat(_Num6, _Num7)), _Num8)
#define comp_syn__lit_num$(T, _Comma_Sep_Lits...) \
    as$((T)(lit_num(_Comma_Sep_Lits)))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_COMMON_INCLUDED  */
