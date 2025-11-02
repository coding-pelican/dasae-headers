/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    switch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2025-06-01 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
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

#define $fallthrough \
    /** \
     * @brief fallthrough for `switch` internal \
     */ \
    comp_attr__$fallthrough

#define case_(_Enum, _Body...) \
    /** \
     * @brief case for `switch` internal \
     */ \
    comp_syn__case_(_Enum, _Body)
#define default_(_Body...) \
    /** \
     * @brief default for `switch` internal \
     */ \
    comp_syn__default_(_Body)

/*========== Macro Implementations ==========================================*/

#define comp_attr__$fallthrough \
    __attribute__((fallthrough))

#define comp_syn__case_(_Enum, _Body...) \
    case _Enum: \
        _Body
#define comp_syn__default_(_Body...) \
    default: \
        _Body

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_SWITCH_INCLUDED */
