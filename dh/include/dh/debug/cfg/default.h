/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    default.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug/cfg
 *
 * @brief   Default configuration for debug system
 * @details Defines default settings for debug assertions and behavior
 */

#ifndef DEBUG_CFG_DEFAULT_INCLUDED
#define DEBUG_CFG_DEFAULT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdio.h>

/*========== Macros and Definitions =========================================*/

/* Debug mode configuration */
#if !defined(NDEBUG)
#define DEBUG_DEFAULT_ENABLED (1)
#else
#define DEBUG_DEFAULT_ENABLED (0)
#endif

/* Default output stream for debug messages */
#define DEBUG_DEFAULT_OUTPUT stderr

/* Default message format strings */
#define DEBUG_DEFAULT_ASSERT_MSG_FORMAT     "Assertion failed: %s, in function %s, at file %s, line %d\n"
#define DEBUG_DEFAULT_ASSERT_FMT_MSG_FORMAT "Assertion failed: %s\n"
#define DEBUG_DEFAULT_ASSERT_FMT_LOC_FORMAT "\nin function %s, at file %s, line %d\n"

/* Default break behavior */
#if defined(__GNUC__) || defined(__clang__)
#define DEBUG_DEFAULT_BREAK() __builtin_trap()
#elif defined(_MSC_VER)
#define DEBUG_DEFAULT_BREAK() __debugbreak()
#else
#include <signal.h>
#define DEBUG_DEFAULT_BREAK() raise(SIGTRAP)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_CFG_DEFAULT_INCLUDED */
