/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    StkTrace.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-11 (date of creation)
 * @updated 2026-01-16 (date of last update)
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug_StkTrace
 *
 * @brief   Debug stack trace utilities
 * @details Provides stack trace functionality for debugging purposes.
 *          Only available when debug mode is enabled.
 */
#ifndef core_debug_StkTrace__included
#define core_debug_StkTrace__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../fn.h"

/*========== Macros and Declarations ========================================*/

#if debug_comp_enabled
$attr($keep_symbol $on_load)
$extern fn_((debug_StkTrace_setupCrashHandler(void))(void));
/// @brief Prints the current stack trace to stderr.
///
/// @details This function captures and prints the current call stack,
///          including thread ID, frame addresses, symbol names, and module names.
///          The output format is platform-specific but includes:
///          - Thread ID
///          - Number of frames
///          - For each frame: index, address, symbol name, offset, module name
///
/// @note This function is only available when debug mode is enabled.
/// @note This function is thread-safe (uses thread-safe `io_stream_eprintln`).
$extern fn_((debug_StkTrace_print(void))(void));
#else
#define debug_StkTrace_setupCrashHandler() $unused(0)
#define debug_StkTrace_print() $unused(0)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_StkTrace__included */
