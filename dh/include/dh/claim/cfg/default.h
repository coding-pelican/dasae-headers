/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    default.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim/cfg
 *
 * @brief   Default configuration for claim assertions
 * @details Defines default output stream and formatting behavior
 */

#ifndef CLAIM_CFG_DEFAULT_INCLUDED
#define CLAIM_CFG_DEFAULT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdio.h>

/*========== Macros and Definitions =========================================*/

/* Default output stream for claim assertion messages */
#define CLAIM_DEFAULT_OUTPUT stderr

/* Default message format strings */
#define CLAIM_DEFAULT_ASSERT_MSG_FORMAT     "Claim assertion failed: %s, in function %s, at file %s, line %d\n"
#define CLAIM_DEFAULT_ASSERT_FMT_MSG_FORMAT "Claim assertion failed: %s\n"
#define CLAIM_DEFAULT_ASSERT_FMT_LOC_FORMAT "\nin function %s, at file %s, line %d\n"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_CFG_DEFAULT_INCLUDED */
