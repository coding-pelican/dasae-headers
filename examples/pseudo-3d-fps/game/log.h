/**
 * @copyright Copyright 2024. Your Name All rights reserved.
 *
 * @file    log.h
 * @brief   Enhanced logging system with file output support
 */

#ifndef LOG_INCLUDED
#define LOG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/claim/unreachable.h"

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

// Global state
static log_Config g_log_config = {
    .output_file     = null,           // Default to stderr
    .min_level       = log_Level_info, // Default to Info level
    .shows_timestamp = true,           // Show timestamps by default
    .shows_level     = true,           // Show log level by default
    .shows_location  = true,           // Show location by default
    .shows_function  = true            // Show function name by default
};

impl_Err(
    io_FileErr,
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
);

// Initialize logging with a file
static Err$void log_init(const char* filename) {
    reserveReturn(Err$void);

    FILE* file = fopen(filename, "w");
    if (!file) {
        return_err(io_FileErr_err(io_FileErrType_OpenFailed));
    }

    if (g_log_config.output_file && g_log_config.output_file != stderr) {
        ignore fclose(g_log_config.output_file);
    }

    g_log_config.output_file = file;
    return_void();
}

// Initialize logging with an existing file handle
static void log_initWithFile(FILE* file) {
    if (g_log_config.output_file && g_log_config.output_file != stderr) {
        ignore fclose(g_log_config.output_file);
    }
    g_log_config.output_file = file;
}

// Close logging
static void log_fini(void) {
    if (g_log_config.output_file && g_log_config.output_file != stderr) {
        ignore fclose(g_log_config.output_file);
        g_log_config.output_file = stderr;
    }
}

// Configuration setters
force_inline void log_setLevel(log_Level level) {
    g_log_config.min_level = level;
}

force_inline void log_showTimestamp(bool shows) {
    g_log_config.shows_timestamp = shows;
}

force_inline void log_showLevel(bool shows) {
    g_log_config.shows_level = shows;
}

force_inline void log_showLocation(bool shows) {
    g_log_config.shows_location = shows;
}

force_inline void log_showFunction(bool shows) {
    g_log_config.shows_function = shows;
}

// Configuration getters
force_inline log_Level log_getLevel(void) {
    return g_log_config.min_level;
}

force_inline FILE* log_getOutputFile(void) {
    return g_log_config.output_file ? g_log_config.output_file : stderr;
}

// Internal logging function
static void log_message(log_Level level, const char* file, int line, const char* func, const char* fmt, ...) {
    if (level < g_log_config.min_level) { return; }

    let output = log_getOutputFile();

    // Get current time if needed
    if (g_log_config.shows_timestamp) {
        time_t     t            = time(null);
        struct tm* lt           = localtime(&t);
        char       time_str[16] = cleared();
        ignore     strftime(time_str, sizeof(time_str), "%H:%M:%S", lt);
        ignore     fprintf(output, "[%s]", time_str);
    }

    // Add level if needed
    if (g_log_config.shows_level) {
        const char* level_str = "????";
        switch (level) {
        case log_Level_debug:
            level_str = "DEBUG";
            break;
        case log_Level_info:
            level_str = "INFO";
            break;
        case log_Level_warn:
            level_str = "WARN";
            break;
        case log_Level_error:
            level_str = "ERROR";
            break;
        default:
            claim_unreachable();
            break;
        }
        ignore fprintf(output, "[%s]", level_str);
    }

    // Add location if needed
    if (g_log_config.shows_location) {
        ignore fprintf(output, "[%s:%d]", file, line);
    }

    // Add function name if needed
    if (g_log_config.shows_function) {
        ignore fprintf(output, "[%s]", func);
    }

    // Add a space before the message if we added any prefixes
    if (g_log_config.shows_timestamp
        || g_log_config.shows_level
        || g_log_config.shows_location
        || g_log_config.shows_function) {
        ignore fprintf(output, " ");
    }

    // Print the actual message
    scope_with(va_list args = null) {
        va_start(args, fmt);
        ignore vfprintf(output, fmt, args);
        va_end(args);
    }

    // Add newline and flush
    ignore fprintf(output, "\n");
    ignore fflush(output);
}

// Convenience macros for different log levels
#if DEBUG_ENABLED
#if COMP_TIME
#define log_debug(...) log_message(log_Level_debug, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_info(...)  log_message(log_Level_info, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_warn(...)  log_message(log_Level_warn, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_message(log_Level_error, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
force_inline void log_debug(const char* fmt, ...);
force_inline void log_info(const char* fmt, ...);
force_inline void log_warn(const char* fmt, ...);
force_inline void log_error(const char* fmt, ...);
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
