/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    StkTrace.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-11 (date of creation)
 * @updated 2026-02-24 (date of last update)
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

/// @brief Sets up the crash handler for the stack trace.
///
/// @details This function sets up the crash handler for the stack trace.
///          The crash handler is platform-specific and is used to print the stack trace
///          when a crash occurs.
///
/// @note only available when debug mode is enabled.
/// @note thread-safe (uses thread-safe `io_stream_eprintln`).
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
/// @note only available when debug mode is enabled.
/// @note thread-safe (uses thread-safe `io_stream_eprintln`).
$extern fn_((debug_StkTrace_print(void))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_StkTrace__included */
