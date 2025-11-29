/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug
 *
 * @brief   Common debug utilities and macros
 * @details Provides common debug utilities and macros for:
 *          - Debug breakpoints
 *          - Debug-only code execution
 *          - Debugger detection
 *          - Debug configuration
 */
#ifndef core_debug_common__included
#define core_debug_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../fn.h"

/*========== Macros and Declarations ========================================*/

/// Function to detect if debugger is attached
$extern fn_((debug_isDebuggerPresent(void))(bool));

#define debug_only(_inner...) \
    /* Used only when `debug_comp_enabled`. */ \
    __comp_syn__debug_only(_inner)
#define debug_break() \
    /* Breakpoint. */ \
    __comp_inline__debug_break()
#define $debug_point \
    /* breakpoint attribute */ \
    __comp_attr__$debug_point

/*========== Macros and Definitions =========================================*/

#define __comp_syn__debug_only(_inner...) pp_if_(debug_comp_enabled)(pp_then_(_inner), pp_else_())
#if !debug_comp_enabled
#define __comp_inline__debug_break() $unused(0)
#else /* debug_comp_enabled */

/* Only trigger debug break if a debugger is attached */
#if defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#define __comp_inline__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { __builtin_debugtrap(); } \
})
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define __comp_inline__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { __debugbreak(); } \
})
#else /* others */
/* Fallback using signal */
#include <signal.h>
#define __comp_inline__debug_break() blk({ \
    if (debug_isDebuggerPresent()) { raise(SIGTRAP); } \
})
#endif /* others */

#endif /* debug_comp_enabled */

#define __comp_attr__$debug_point debug_break(),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_common__included */
