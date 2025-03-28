/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-28 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti
 *
 * @brief   Common utilities and macros
 * @details Provides commonly used macros and utilities shared between BTI modules.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_INCLUDED
#define BTI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations =========================================*/

#define bti_noop()               \
    /**                          \
     * @brief No-operation macro \
     * @details Does nothing     \
     */                          \
    comp_impl__bti_noop()
#define bti_unused(_x)                   \
    /**                                  \
     * @brief Mark variable as unused    \
     * @param _x Variable to mark unused \
     */                                  \
    comp_impl__bti_unused(_x)
#define bti_concat(_a, _b)           \
    /**                              \
     * @brief Concatenate two tokens \
     * @param _a First token         \
     * @param _b Second token        \
     * @return Concatenated token    \
     */                              \
    comp_impl__bti_concat(_a, _b)
#define bti_stringify(_x)             \
    /**                               \
     * @brief Convert token to string \
     * @param _x Token to stringify   \
     * @return String representation  \
     */                               \
    comp_impl__bti_stringify(_x)

/*========== Macros and Definitions =========================================*/

/* Implementation */
#define comp_impl__bti_noop()         ((void)0)
#define comp_impl__bti_unused(_x)     ((void)(_x))
#define comp_impl__bti_concat(_a, _b) _a##_b
#define comp_impl__bti_stringify(_x)  #_x

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_INCLUDED */
