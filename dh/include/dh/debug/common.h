/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/debug
 * @prefix  debug
 *
 * @brief   Common debug utilities and macros
 * @details Provides common debug utilities and macros for:
 *          - Debug breakpoints
 *          - Debug-only code execution
 *          - Debugger detection
 *          - Debug configuration
 */

#ifndef DEBUG_COMMON_INCLUDED
#define DEBUG_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/// Function to detect if a debugger is attached
extern bool debug_isDebuggerPresent(void);

#define $debug_point \
    /* breakpoint attribute */ \
    comp_attr__$debug_point
#define debug_break() \
    /* Breakpoint. */ \
    comp_op__debug_break()
#define debug_only(...) \
    /* Used only when `debug_comp_enabled`. */ \
    comp_syn__debug_only(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define comp_attr__$debug_point debug_break(),

#if !debug_comp_enabled

#define comp_op__debug_break()    $unused(0)
#define comp_syn__debug_only(...) $unused(0)

#else /* debug_comp_enabled */

/* Only trigger debug break if a debugger is attached */
#if defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#define comp_op__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { __builtin_debugtrap(); } \
})
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define comp_op__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { __debugbreak(); } \
})
#else /* others */
/* Fallback using signal */
#include <signal.h>
#define comp_op__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { raise(SIGTRAP); } \
})
#endif /* others */

#define comp_syn__debug_only(...) __VA_ARGS__

#endif /* debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_COMMON_INCLUDED */
