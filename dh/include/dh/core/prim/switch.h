/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    switch.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-20 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PRIM_SWITCH_INCLUDED
#define CORE_PRIM_SWITCH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define fallthrough                             \
    /**                                         \
     * @brief fallthrough for `switch` internal \
     */                                         \
    IMPL_fallthrough

#define case_return(_Expr)                      \
    /**                                         \
     * @brief case return for `switch` internal \
     */                                         \
    IMPL_case_return(_Expr)

#define default_return(_Expr)                      \
    /**                                            \
     * @brief default return for `switch` internal \
     */                                            \
    IMPL_default_return(_Expr)

/*========== Macro Implementations ==========================================*/

#define IMPL_fallthrough

#define IMPL_case_return(_Expr) \
    case _Expr:                 \
        return

#define IMPL_default_return(_Expr) \
    default:                       \
        return

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_SWITCH_INCLUDED */
