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

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define pp_func(_Statement...)                                                   \
    /**                                                                          \
     * @brief Creates a single statement block from multiple expressions         \
     * @param _Statement... Multiple statements to be executed as a single block \
     * @details Wraps multiple statements in a parentheses and braces block      \
    for macro safety                                                             \
     */                                                                          \
    IMPL_pp_func(_Statement)

#define pp_concat(_Token_lhs, _Token_rhs...) \
    IMPL_pp_concat(_Token_lhs, _Token_rhs)

#define pp_concat3(_Token_lhs, _Token_mid, _Token_rhs...) \
    IMPL_pp_concat3(_Token_lhs, _Token_mid, _Token_rhs)

#define pp_join(_Token_sep, _Token_lhs, _Token_rhs...) \
    IMPL_pp_join(_Token_sep, _Token_lhs, _Token_rhs)

#define pp_join3(_Token_sep, _Token_lhs, _Token_mid, _Token_rhs...) \
    IMPL_pp_join3(_Token_sep, _Token_lhs, _Token_mid, _Token_rhs)

#define pp_uniqueToken(_Token...) \
    IMPL_pp_uniqueToken(_Token)

/*========== Macros Implementation ==========================================*/

#define IMPL_pp_func(_Statement...) \
    ({ _Statement })

#define IMPL_pp_concat(_Token_lhs, _Token_rhs...) \
    _Token_lhs##_Token_rhs

#define IMPL_pp_concat3(_Token_lhs, _Token_mid, _Token_rhs...) \
    _Token_lhs##_Token_mid##_Token_rhs

#define IMPL_pp_join(_Token_sep, _Token_lhs, _Token_rhs...) \
    _Token_lhs##_Token_sep##_Token_rhs

#define IMPL_pp_join3(_Token_sep, _Token_lhs, _Token_mid, _Token_rhs...) \
    _Token_lhs##_Token_sep##_Token_mid##_Token_sep##_Token_rhs

#define IMPL_pp_uniqueToken(_Token...) \
    pp_concat(pp_concat3(_, __LINE__, _), _Token)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
