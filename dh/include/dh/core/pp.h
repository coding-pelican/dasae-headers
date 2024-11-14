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

#define pp_fallthrough                          \
    /**                                         \
     * @brief fallthrough for `switch` internal \
     */                                         \
    IMPL_pp_fallthrough

#define pp_unused(...)                                                \
    /**                                                               \
     * @brief Marks variables or expressions as intentionally unused  \
    to suppress compiler warnings                                     \
     * @param ... Variable number of arguments to be marked as unused \
     */                                                               \
    IMPL_pp_unused(__VA_ARGS__)

#define pp_ignore                                                        \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    IMPL_pp_ignore

#define pp_ensureNotNull(VAL, ...) \
    pp_ensureNotNull(VAL, __VA_ARGS__)

#define pp_func(...)                                                        \
    /**                                                                     \
     * @brief Creates a single statement block from multiple expressions    \
     * @param ... Multiple statements to be executed as a single block      \
     * @details Wraps multiple statements in a parentheses and braces block \
    for macro safety                                                        \
     */                                                                     \
    IMPL_pp_func(__VA_ARGS__)

#define pp_concat(TOKEN, ...) \
    IMPL_pp_concat(TOKEN, __VA_ARGS__)

#define pp_uniqueToken(TOKEN) \
    IMPL_pp_uniqueToken(TOKEN)

/*========== Macros Implementation ==========================================*/

#define IMPL_pp_fallthrough

#define IMPL_pp_unused(...) \
    ((void)(__VA_ARGS__))

#define IMPL_pp_ignore \
    (void)

#define IMPL_pp_ensureNotNull(VAL, ...) \
    ((VAL) ? (VAL) : (__VA_ARGS__))

#define IMPL_pp_func(...) \
    ({ __VA_ARGS__ })

#define IMPL_pp_concat(TOKEN, ...) \
    TOKEN##__VA_ARGS__

#define IMPL_pp_uniqueToken(TOKEN) \
    pp_concat(TOKEN, __LINE__)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_INCLUDED  */
