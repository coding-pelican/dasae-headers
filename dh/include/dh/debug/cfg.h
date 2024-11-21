/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug
 *
 * @brief   Configuration synthesis for debug system
 * @details Combines default and custom configurations for debug system
 */

#ifndef DEBUG_CFG_INCLUDED
#define DEBUG_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

/* Include default configuration first */
#include "dh/debug/cfg/default.h"

/* Include custom configuration if it exists */
#if __has_include("dh/debug/cfg/custom.h")
#include "dh/debug/cfg/custom.h"
#endif /* __has_include("dh/debug/cfg/custom.h") */

/*========== Macros and Definitions =========================================*/

/* Synthesize configuration with fallbacks to defaults */

#if !defined(DEBUG_ENABLED)
#define DEBUG_ENABLED DEBUG_DEFAULT_ENABLED
#endif /* !defined(DEBUG_ENABLED) */

#define debug_enabled (DEBUG_ENABLED)

#if !defined(DEBUG_OUTPUT)
#define DEBUG_OUTPUT DEBUG_DEFAULT_OUTPUT
#endif /* !defined(DEBUG_OUTPUT) */

#if !defined(DEBUG_ASSERT_MSG_FORMAT)
#define DEBUG_ASSERT_MSG_FORMAT DEBUG_DEFAULT_ASSERT_MSG_FORMAT
#endif /* !defined(DEBUG_ASSERT_MSG_FORMAT) */

#if !defined(DEBUG_ASSERT_FMT_MSG_FORMAT)
#define DEBUG_ASSERT_FMT_MSG_FORMAT DEBUG_DEFAULT_ASSERT_FMT_MSG_FORMAT
#endif /* !defined(DEBUG_ASSERT_FMT_MSG_FORMAT) */

#if !defined(DEBUG_ASSERT_FMT_LOC_FORMAT)
#define DEBUG_ASSERT_FMT_LOC_FORMAT DEBUG_DEFAULT_ASSERT_FMT_LOC_FORMAT
#endif /* !defined(DEBUG_ASSERT_FMT_LOC_FORMAT) */

#if !defined(DEBUG_BREAK)
#define DEBUG_BREAK DEBUG_DEFAULT_BREAK
#endif /* !defined(DEBUG_BREAK) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_CFG_INCLUDED */
