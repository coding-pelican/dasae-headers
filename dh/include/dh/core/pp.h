/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    pp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core
 * @prefix  pp
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PP_INCLUDED
#define CORE_PP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define pp_nothing(...) comp_op__pp_nothing(__VA_ARGS__)

#define pp_stringify(_Tok...) comp_op__pp_stringify(_Tok)
#define nameOf(_Tok...)       comp_op__nameOf(_Tok)

#define pp_cat(_Tok_lhs, _Tok_rhs...) \
    comp_op__pp_cat(_Tok_lhs, _Tok_rhs)
#define pp_cat2(_Tok_lhs, _Tok_rhs...) \
    comp_op__pp_cat2(_Tok_lhs, _Tok_rhs)
#define pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs...) \
    comp_op__pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs)

#define pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    comp_op__pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs)
#define pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    comp_op__pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs)
#define pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs...) \
    comp_op__pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs)

#define pp_uniqTok(_Tok...) \
    comp_op__pp_uniqTok(_Tok)
#define pp_uniqTokByLine(_Tok...) \
    comp_op__pp_uniqTokByLine(_Tok)

#define pp_countArgs(_Args...) \
    comp_op__pp_countArgs(_Args)

// pp_foreach macro implementation (handles up to 8 arguments)
#define pp_foreach(macro, name, ...) \
    comp_op__pp_foreach_(comp_op__pp_foreach_NARG(__VA_ARGS__), macro, name, __VA_ARGS__)

// lit_num(11,644,473,600u) => 11644473600u
#define lit_num(_Comma_Sep_Lits...) \
    comp_syn__lit_num(_Comma_Sep_Lits)
#define lit_num$(T, _Comma_Sep_Lits...) \
    comp_syn__lit_num$(T, _Comma_Sep_Lits)

/*========== Macros Implementation ==========================================*/

#define comp_op__pp_nothing(...)
#define comp_op__pp_stringify(_Tok...) #_Tok
#define comp_op__nameOf(_Tok...)       ($unused(_Tok), #_Tok)

#define comp_op__pp_cat(_Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_rhs
#define comp_op__pp_cat2(_Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_rhs
#define comp_op__pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs...) \
    _Tok_lhs##_Tok_mid##_Tok_rhs

#define comp_op__pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_rhs
#define comp_op__pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_rhs
#define comp_op__pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_mid##_Tok_sep##_Tok_rhs

#define comp_op__pp_uniqTok(_Tok...) \
    pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)
#define comp_op__pp_uniqTokByLine(_Tok...) \
    pp_join(_, pp_join2(_, _, __LINE__), _Tok)

#define comp_op__pp_countArgs(_Args...)               \
    comp_op__pp_countArgs__selectArgCountInListRseqN( \
        _Args, comp_op__pp_countArgs__getListRseqN()  \
    )
#define comp_op__pp_countArgs__selectArgCountInListRseqN(_Args...) \
    comp_op__pp_countArgs__argN(_Args)
#define comp_op__pp_countArgs__getListRseqN() \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define comp_op__pp_countArgs__argN(                                               \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N

// pp_foreach macro implementation (handles up to 16 arguments)
#define comp_op__pp_foreach_(N, macro, name, ...) \
    pp_join(__, comp_op__pp_foreach, N)(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach_NARG(...) \
    comp_op__pp_foreach_NARG_(__VA_ARGS__, comp_op__pp_foreach_RSEQ_N())
#define comp_op__pp_foreach_NARG_(...) \
    comp_op__pp_foreach_ARG_N(__VA_ARGS__)
#define comp_op__pp_foreach_ARG_N(                                                 \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N
#define comp_op__pp_foreach_RSEQ_N() \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define comp_op__pp_foreach__1(macro, name, x)       macro(name, x)
#define comp_op__pp_foreach__2(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__1(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__3(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__2(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__4(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__3(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__5(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__4(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__6(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__5(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__7(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__6(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__8(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__7(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__9(macro, name, x, ...)  macro(name, x) comp_op__pp_foreach__8(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__10(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__9(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__11(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__10(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__12(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__11(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__13(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__12(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__14(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__13(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__15(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__14(macro, name, __VA_ARGS__)
#define comp_op__pp_foreach__16(macro, name, x, ...) macro(name, x) comp_op__pp_foreach__15(macro, name, __VA_ARGS__)

#define comp_syn__lit_num(_Comma_Sep_Lits...) \
    pp_join(__, comp_syn__lit_num, pp_countArgs(_Comma_Sep_Lits))(_Comma_Sep_Lits)
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
    as$(T, lit_num(_Comma_Sep_Lits))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
