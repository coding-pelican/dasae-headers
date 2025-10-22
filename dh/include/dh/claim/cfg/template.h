/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    template.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim/cfg
 *
 * @brief   Template configuration for claim assertions
 * @details Template for customizing claim assertion behavior
 */

#ifndef CLAIM_CFG_TEMPLATE_INCLUDED
#define CLAIM_CFG_TEMPLATE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdio.h>

/*========== Macros and Definitions =========================================*/

/* Custom output stream for claim assertion messages
 * Examples:
 *   #define CLAIM_OUTPUT stdout
 *   #define CLAIM_OUTPUT stderr
 *   #define CLAIM_OUTPUT my_custom_output_stream
 */
// #define CLAIM_OUTPUT your_output_stream

/* Custom message format strings
 * Available format specifiers:
 * %s - expression string
 * %s - function name
 * %s - file name
 * %d - line number
 */
// #define CLAIM_ASSERT_MSG_FORMAT "Custom format: %s in %s (%s:%d)\n"
// #define CLAIM_ASSERT_FMT_MSG_FORMAT "Custom assertion message: %s\n"
// #define CLAIM_ASSERT_FMT_LOC_FORMAT "Location: %s in %s line %d\n"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_CFG_TEMPLATE_INCLUDED */
