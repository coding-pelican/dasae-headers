/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    template.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug/cfg
 *
 * @brief   Template configuration for debug system
 * @details Template for customizing debug behavior
 */

#ifndef DEBUG_CFG_TEMPLATE_INCLUDED
#define DEBUG_CFG_TEMPLATE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdio.h>

/*========== Macros and Definitions =========================================*/

/* Debug enable/disable override
 * Example:
 *   #define DEBUG_ENABLED (1)  // Force debug mode on
 *   #define DEBUG_ENABLED (0)  // Force debug mode off
 */
// #define DEBUG_ENABLED your_debug_setting

/* Custom output stream for debug messages
 * Examples:
 *   #define DEBUG_OUTPUT stdout
 *   #define DEBUG_OUTPUT stderr
 *   #define DEBUG_OUTPUT my_custom_output_stream
 */
// #define DEBUG_OUTPUT your_output_stream

/* Custom message format strings
 * Available format specifiers:
 * %s - expression string
 * %s - function name
 * %s - file name
 * %d - line number
 */
// #define DEBUG_ASSERT_MSG_FORMAT "Custom format: %s in %s (%s:%d)\n"
// #define DEBUG_ASSERT_FMT_MSG_FORMAT "Custom assertion message: %s\n"
// #define DEBUG_ASSERT_FMT_LOC_FORMAT "Location: %s in %s line %d\n"

/* Custom break behavior
 * Example:
 *   #define DEBUG_BREAK() my_custom_break_function()
 */
// #define DEBUG_BREAK() your_break_function()

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_CFG_TEMPLATE_INCLUDED */
