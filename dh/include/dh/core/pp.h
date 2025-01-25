/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    pp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-05 (date of last update)
 * @version v1.0.0
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

#define pp_func(_Statement...)                                                   \
    /**                                                                          \
     * @brief Creates a single statement block from multiple expressions         \
     * @param _Statement... Multiple statements to be executed as a single block \
     * @details Wraps multiple statements in a parentheses and braces block      \
    for macro safety                                                             \
     */                                                                          \
    IMPL_pp_func(_Statement)

#define pp_stringify(_Tok...) IMPL_pp_stringify(_Tok)

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

/*========== Macros Implementation ==========================================*/

#define IMPL_pp_func(_Statement...) \
    ({ _Statement })

#define IMPL_pp_stringify(_Tok...) #_Tok

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
    pp_cat(pp_cat3(_, __LINE__, _), _Tok)

#define IMPL_pp_countArgs(_Args...) pp_countArgs__argN(_Args)

// pp_foreach macro implementation (handles up to 8 arguments)
#define IMPL_pp_foreach_(N, macro, name, ...) pp_cat(IMPL_pp_foreach_, N)(macro, name, __VA_ARGS__)

#define IMPL_pp_foreach_NARG(...)                                     IMPL_pp_foreach_NARG_(__VA_ARGS__, IMPL_pp_foreach_RSEQ_N())
#define IMPL_pp_foreach_NARG_(...)                                    IMPL_pp_foreach_ARG_N(__VA_ARGS__)
#define IMPL_pp_foreach_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define IMPL_pp_foreach_RSEQ_N()                                      8, 7, 6, 5, 4, 3, 2, 1, 0

#define IMPL_pp_foreach_1(macro, name, x)      macro(name, x)
#define IMPL_pp_foreach_2(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_1(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_3(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_2(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_4(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_3(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_5(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_4(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_6(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_5(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_7(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_6(macro, name, __VA_ARGS__)
#define IMPL_pp_foreach_8(macro, name, x, ...) macro(name, x) IMPL_pp_foreach_7(macro, name, __VA_ARGS__)

// // FOR_EACH macro implementation(handles up to 8 arguments)
// #define FOR_EACH_1(macro, name, x)      macro(name, x)
// #define FOR_EACH_2(macro, name, x, ...) macro(name, x) FOR_EACH_1(macro, name, __VA_ARGS__)
// #define FOR_EACH_3(macro, name, x, ...) macro(name, x) FOR_EACH_2(macro, name, __VA_ARGS__)
// #define FOR_EACH_4(macro, name, x, ...) macro(name, x) FOR_EACH_3(macro, name, __VA_ARGS__)
// #define FOR_EACH_5(macro, name, x, ...) macro(name, x) FOR_EACH_4(macro, name, __VA_ARGS__)
// #define FOR_EACH_6(macro, name, x, ...) macro(name, x) FOR_EACH_5(macro, name, __VA_ARGS__)
// #define FOR_EACH_7(macro, name, x, ...) macro(name, x) FOR_EACH_6(macro, name, __VA_ARGS__)
// #define FOR_EACH_8(macro, name, x, ...) macro(name, x) FOR_EACH_7(macro, name, __VA_ARGS__)

// #define FOR_EACH_NARG(...)                                     FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
// #define FOR_EACH_NARG_(...)                                    FOR_EACH_ARG_N(__VA_ARGS__)
// #define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
// #define FOR_EACH_RSEQ_N()                                      8, 7, 6, 5, 4, 3, 2, 1, 0

// #define FOR_EACH_(N, macro, name, ...) pp_cat(FOR_EACH_, N)(macro, name, __VA_ARGS__)
// #define FOR_EACH(macro, name, ...)     FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), macro, name, __VA_ARGS__)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
