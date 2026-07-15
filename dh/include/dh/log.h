/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    log.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-03 (date of creation)
 * @updated 2026-05-20 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  log
 *
 * @brief   logging header
 * @details Provides logging functionality with configurable output destination,
 *          log levels, and formatting options.
 */

#pragma once
#ifndef log__included
#define log__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "fs/File.h"

/*========== Macros and Declarations ========================================*/

// Log levels
typedef enum log_Level {
    log_Level_debug,
    log_Level_info,
    log_Level_warn,
    log_Level_error,
    log_Level_count
} log_Level;

// Log configuration
typedef struct log_Config {
    fs_File output_file;
    bool has_output_file;
    bool owns_output_file;
    log_Level min_level;
    bool shows_timestamp;
    bool shows_level;
    bool shows_location;
    bool shows_function;
} log_Config;

// Initialize logging with a file
extern E$void log_init(const char* filename) $must_check;
// Initialize logging with an existing file handle
extern void log_initWithFile(fs_File file);
// Close logging
extern void log_fini(void);

// Configuration setters
extern void log_setLevel(log_Level level);
extern void log_showTimestamp(bool shows);
extern void log_showLevel(bool shows);
extern void log_showLocation(bool shows);
extern void log_showFunction(bool shows);

// Configuration getters
extern log_Level log_getLevel(void);
extern fs_File log_getOutputFile(void);

// Internal logging function
extern void log_message(log_Level /* level */, const char* /* file */, int /* line */, const char* /* func */, const char* /* fmt */, ...);

#if !defined(log_comp_disabled_not_debug_enabled)
#define log_comp_disabled_not_debug_enabled pp_false
#endif /* !defined(log_comp_disabled_not_debug_enabled) */

// Convenience macros for different log levels
#if debug_enabled || !log_comp_disabled_not_debug_enabled
#if in_comptime

#define log_debug(...) log_message(log_Level_debug, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_info(...) log_message(log_Level_info, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_warn(...) log_message(log_Level_warn, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_message(log_Level_error, __FILE__, __LINE__, __func__, __VA_ARGS__)

#else

$attr($inline_always)
$static void log_debug(const char* fmt, ...) { let_ignore = fmt; };
$attr($inline_always)
$static void log_info(const char* fmt, ...) { let_ignore = fmt; };
$attr($inline_always)
$static void log_warn(const char* fmt, ...) { let_ignore = fmt; };
$attr($inline_always)
$static void log_error(const char* fmt, ...) { let_ignore = fmt; };

#endif
#else

#define log_debug(...) $unused(0)
#define log_info(...) $unused(0)
#define log_warn(...) $unused(0)
#define log_error(...) $unused(0)

#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* log__included */
