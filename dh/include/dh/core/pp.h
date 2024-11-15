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

#define pp_func(STMT...)                                                    \
    /**                                                                     \
     * @brief Creates a single statement block from multiple expressions    \
     * @param STMT... Multiple statements to be executed as a single block  \
     * @details Wraps multiple statements in a parentheses and braces block \
    for macro safety                                                        \
     */                                                                     \
    IMPL_pp_func(STMT)

#define pp_concat(TOKEN_LHS, TOKEN_RHS...) \
    IMPL_pp_concat(TOKEN_LHS, TOKEN_RHS)

#define pp_uniqueToken(TOKEN...) \
    IMPL_pp_uniqueToken(TOKEN)

/*========== Macros Implementation ==========================================*/

#define IMPL_pp_func(STMT...) \
    ({ STMT })

#define IMPL_pp_concat(TOKEN_LHS, TOKEN_RHS...) \
    TOKEN_LHS##TOKEN_RHS

#define IMPL_pp_uniqueToken(TOKEN...) \
    pp_concat(pp_concat(pp_concat(_, __LINE__), _), TOKEN)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
