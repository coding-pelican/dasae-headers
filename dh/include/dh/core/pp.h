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

#define pp_nothing(...) IMPL__pp_nothing(__VA_ARGS__)

#define pp_func(_Statement...)                                                   \
    /**                                                                          \
     * @brief Creates a single statement block from multiple expressions         \
     * @param _Statement... Multiple statements to be executed as a single block \
     * @details Wraps multiple statements in a parentheses and braces block      \
    for macro safety                                                             \
     */                                                                          \
    IMPL_pp_func(_Statement)

#define pp_stringify(_Tok...) IMPL_pp_stringify(_Tok)
#define nameOf(_Tok...)       IMPL_nameOf(_Tok)

#define pp_cat(_Tok_lhs, _Tok_rhs...) \
    IMPL_pp_cat(_Tok_lhs, _Tok_rhs)

#define pp_cat2(_Tok_lhs, _Tok_rhs...) \
    IMPL_pp_cat2(_Tok_lhs, _Tok_rhs)

#define pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs...) \
    IMPL_pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs)

#define pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    IMPL_pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs)

#define pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    IMPL_pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs)

#define pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs...) \
    IMPL_pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs)

#define pp_uniqTok(_Tok...) \
    IMPL_pp_uniqTok(_Tok)

#define pp_countArgs(_Args...) \
    IMPL_pp_countArgs(_Args, pp_countArgs__rseqN())
#define pp_countArgs__rseqN() \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define pp_countArgs__argN(                                                        \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N

// pp_foreach macro implementation (handles up to 8 arguments)
#define pp_foreach(macro, name, ...) \
    IMPL_pp_foreach_(IMPL_pp_foreach_NARG(__VA_ARGS__), macro, name, __VA_ARGS__)

// lit_num(11,644,473,600u) => 11644473600u
#define lit_num(...) \
    comp_syn__lit_num(__VA_ARGS__)
#define lit_num$(T, ...) \
    comp_syn__lit_num$(T, __VA_ARGS__)

/*========== Macros Implementation ==========================================*/

#define IMPL__pp_nothing(...)

#define IMPL_pp_func(_Statement...) \
    ({ _Statement })

#define IMPL_pp_stringify(_Tok...) #_Tok

#define IMPL_nameOf(_Tok...) (unused(_Tok), #_Tok)

#define IMPL_pp_cat(_Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_rhs

#define IMPL_pp_cat2(_Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_rhs

#define IMPL_pp_cat3(_Tok_lhs, _Tok_mid, _Tok_rhs...) \
    _Tok_lhs##_Tok_mid##_Tok_rhs

#define IMPL_pp_join(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_rhs

#define IMPL_pp_join2(_Tok_sep, _Tok_lhs, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_rhs

#define IMPL_pp_join3(_Tok_sep, _Tok_lhs, _Tok_mid, _Tok_rhs...) \
    _Tok_lhs##_Tok_sep##_Tok_mid##_Tok_sep##_Tok_rhs

#define IMPL_pp_uniqTok(_Tok...) \
    pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)

#define IMPL_pp_countArgs(_Args...) pp_countArgs__argN(_Args)

// pp_foreach macro implementation (handles up to 16 arguments)
#define IMPL_pp_foreach_(N, macro, name, ...) pp_cat(IMPL_pp_foreach_, N)(macro, name, __VA_ARGS__)

#define IMPL_pp_foreach_NARG(...)  IMPL_pp_foreach_NARG_(__VA_ARGS__, IMPL_pp_foreach_RSEQ_N())
#define IMPL_pp_foreach_NARG_(...) IMPL_pp_foreach_ARG_N(__VA_ARGS__)
#define IMPL_pp_foreach_ARG_N(                                                     \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _N, ... \
) _N
#define IMPL_pp_foreach_RSEQ_N() \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define IMPL_pp_foreach_1(macro, name, x)       macro(name, x)
#define IMPL_pp_foreach_2(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_1(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_3(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_2(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_4(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_3(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_5(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_4(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_6(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_5(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_7(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_6(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_8(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_7(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_9(macro, name, x, ...)  macro(name, x) IMPL_pp_foreach_8(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_10(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_9(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_11(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_10(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_12(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_11(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_13(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_12(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_14(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_13(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_15(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_14(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_16(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_15(macro, name, __VA_ARGS__)

#define comp_syn__lit_num(...) \
    pp_cat(comp_syn__lit_num_, pp_countArgs(__VA_ARGS__))(__VA_ARGS__)
/* Handle different numbers of arguments */
#define comp_syn__lit_num_1(_Num1)                                                  _Num1
#define comp_syn__lit_num_2(_Num1, _Num2)                                           pp_cat(_Num1, _Num2)
#define comp_syn__lit_num_3(_Num1, _Num2, _Num3)                                    pp_cat3(_Num1, _Num2, _Num3)
#define comp_syn__lit_num_4(_Num1, _Num2, _Num3, _Num4)                             pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4)
#define comp_syn__lit_num_5(_Num1, _Num2, _Num3, _Num4, _Num5)                      pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), _Num4), _Num5)
#define comp_syn__lit_num_6(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6)               pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6)
#define comp_syn__lit_num_7(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7)        pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), _Num6), _Num7)
#define comp_syn__lit_num_8(_Num1, _Num2, _Num3, _Num4, _Num5, _Num6, _Num7, _Num8) pp_cat(pp_cat(pp_cat(pp_cat3(_Num1, _Num2, _Num3), pp_cat(_Num4, _Num5)), pp_cat(_Num6, _Num7)), _Num8)
#define comp_syn__lit_num$(T, ...)                                                  as$(T, lit_num(__VA_ARGS__))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
