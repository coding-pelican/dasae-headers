/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    log.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-03 (date of creation)
 * @updated 2025-01-04 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  log
 *
 * @brief   logging header
 * @details Provides logging functionality with configurable output destination,
 *          log levels, and formatting options. Supports file and stderr output,
 *          timestamps, log levels, source location, and function name display.
 */

#ifndef LOG_INCLUDED
#define LOG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

#include <stdio.h>

use_Err(
    io_FileErr,
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
);

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
    FILE*     output_file;     // Output file (null means stderr)
    log_Level min_level;       // Minimum level to log
    bool      shows_timestamp; // Whether to show timestamps
    bool      shows_level;     // Whether to show log level
    bool      shows_location;  // Whether to show file and line
    bool      shows_function;  // Whether to show function name
} log_Config;

// Initialize logging with a file
extern Err$void log_init(const char* filename) must_check;
// Initialize logging with an existing file handle
extern void     log_initWithFile(FILE* file);
// Close logging
extern void     log_fini(void);

// Configuration setters
extern void log_setLevel(log_Level level);
extern void log_showTimestamp(bool shows);
extern void log_showLevel(bool shows);
extern void log_showLocation(bool shows);
extern void log_showFunction(bool shows);

// Configuration getters
extern log_Level log_getLevel(void);
extern FILE*     log_getOutputFile(void);

// Internal logging function
extern void log_message(log_Level /* level */, const char* /* file */, int /* line */, const char* /* func */, const char* /* fmt */, ...);

// Convenience macros for different log levels
#if DEBUG_ENABLED || defined(LOG_NO_DISABLE_RELEASE)
#if COMP_TIME

#define log_debug(...) log_message(log_Level_debug, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_info(...)  log_message(log_Level_info, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_warn(...)  log_message(log_Level_warn, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_message(log_Level_error, __FILE__, __LINE__, __func__, __VA_ARGS__)

#else

force_inline void log_debug(const char* /* fmt */, ...);
force_inline void log_info(const char* /* fmt */, ...);
force_inline void log_warn(const char* /* fmt */, ...);
force_inline void log_error(const char* /* fmt */, ...);

#endif
#else

#define log_debug(...) unused(0)
#define log_info(...)  unused(0)
#define log_warn(...)  unused(0)
#define log_error(...) unused(0)

#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* LOG_INCLUDED */
