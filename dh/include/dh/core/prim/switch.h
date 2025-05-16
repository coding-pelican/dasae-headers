/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    switch.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Switch utilities
 * @details Provides switch utilities and functions for:
 *          - Fallthrough
 *          - Case return
 *          - Default return
 */

#ifndef CORE_PRIM_SWITCH_INCLUDED
#define CORE_PRIM_SWITCH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define $fallthrough                            \
    /**                                         \
     * @brief fallthrough for `switch` internal \
     */                                         \
    comp_attr__$fallthrough

#define case_return(_Expr)                      \
    /**                                         \
     * @brief case return for `switch` internal \
     */                                         \
    SYN_case_return(_Expr)

#define default_return(_Expr)                      \
    /**                                            \
     * @brief default return for `switch` internal \
     */                                            \
    SYN_default_return(_Expr)

/*========== Macro Implementations ==========================================*/

#define comp_attr__$fallthrough \
    __attribute__((fallthrough))

#define SYN_case_return(_Expr) \
    case _Expr:                \
        return

#define SYN_default_return(_Expr) \
    default:                      \
        return

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_SWITCH_INCLUDED */
