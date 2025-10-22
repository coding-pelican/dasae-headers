/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim
 *
 * @brief   Configuration synthesis for claim assertions
 * @details Combines default and custom configurations for claim assertions
 */

#ifndef CLAIM_CFG_INCLUDED
#define CLAIM_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

/* Include default configuration first */
#include "dh/claim/cfg/default.h"

/* Include custom configuration if it exists */
#if __has_include("dh/claim/cfg/custom.h")
#include "dh/claim/cfg/custom.h"
#endif /* __has_include("dh/claim/cfg/custom.h") */

/*========== Macros and Definitions =========================================*/

/* Synthesize configuration with fallbacks to defaults */

#if !defined(CLAIM_OUTPUT)
#define CLAIM_OUTPUT CLAIM_DEFAULT_OUTPUT
#endif /* !defined(CLAIM_OUTPUT) */

#if !defined(CLAIM_ASSERT_MSG_FORMAT)
#define CLAIM_ASSERT_MSG_FORMAT CLAIM_DEFAULT_ASSERT_MSG_FORMAT
#endif /* !defined(CLAIM_ASSERT_MSG_FORMAT) */

#if !defined(CLAIM_ASSERT_FMT_MSG_FORMAT)
#define CLAIM_ASSERT_FMT_MSG_FORMAT CLAIM_DEFAULT_ASSERT_FMT_MSG_FORMAT
#endif /* !defined(CLAIM_ASSERT_FMT_MSG_FORMAT) */

#if !defined(CLAIM_ASSERT_FMT_LOC_FORMAT)
#define CLAIM_ASSERT_FMT_LOC_FORMAT CLAIM_DEFAULT_ASSERT_FMT_LOC_FORMAT
#endif /* !defined(CLAIM_ASSERT_FMT_LOC_FORMAT) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_CFG_INCLUDED */
