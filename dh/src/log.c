#include "dh/log.h"
#include "dh/fs/dir.h"

#include <stdarg.h>
#include <time.h>

// Global state
static log_Config log_s_config = {
    .output_file     = null,           // Default to stderr
    .min_level       = log_Level_info, // Default to Info level
    .shows_timestamp = true,           // Show timestamps by default
    .shows_level     = true,           // Show log level by default
    .shows_location  = true,           // Show location by default
    .shows_function  = true            // Show function name by default
};

io_FileErr$void log_init(const char* filename) {
    scope_reserveReturn(io_FileErr$void);

    // Extract directory path
    char dir_path[256] = { 0 };
    if_(let dir_last_slash = strrchr(filename, '/'), dir_last_slash) {
        let dir_len = dir_last_slash - filename;
        strncpy(dir_path, filename, dir_len);
        dir_path[dir_len] = '\0';

        // Create directory
        try(fs_dir_create(Str_view(as$(const u8*, dir_path), dir_len)));
    }

    let file = fopen(filename, "w");
    if (!file) { return_err(io_FileErr_OpenFailed()); }
    errdefer(ignore fclose(file));

    if (log_s_config.output_file && log_s_config.output_file != stderr) {
        ignore fclose(log_s_config.output_file);
    }
    log_s_config.output_file = file;
    return_void();

    scope_returnReserved;
}

void log_initWithFile(FILE* file) {
    if (log_s_config.output_file && log_s_config.output_file != stderr) {
        ignore fclose(log_s_config.output_file);
    }
    log_s_config.output_file = file;
}

void log_fini(void) {
    if (log_s_config.output_file && log_s_config.output_file != stderr) {
        ignore fclose(log_s_config.output_file);
        log_s_config.output_file = stderr;
    }
}

void log_setLevel(log_Level level) {
    log_s_config.min_level = level;
}

void log_showTimestamp(bool shows) {
    log_s_config.shows_timestamp = shows;
}

void log_showLevel(bool shows) {
    log_s_config.shows_level = shows;
}

void log_showLocation(bool shows) {
    log_s_config.shows_location = shows;
}

void log_showFunction(bool shows) {
    log_s_config.shows_function = shows;
}

log_Level log_getLevel(void) {
    return log_s_config.min_level;
}

FILE* log_getOutputFile(void) {
    return log_s_config.output_file ? log_s_config.output_file : stderr;
}

void log_message(log_Level level, const char* file, int line, const char* func, const char* fmt, ...) {
    if (level < log_s_config.min_level) { return; }

    let output = log_getOutputFile();

    // Get current time if needed
    if (log_s_config.shows_timestamp) {
        time_t     t            = time(null);
        struct tm* lt           = localtime(&t);
        char       time_str[16] = cleared();
        ignore     strftime(time_str, sizeof(time_str), "%H:%M:%S", lt);
        ignore     fprintf(output, "[%s]", time_str);
    }

    // Add level if needed
    if (log_s_config.shows_level) {
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
            claim_unreachable;
        }
        ignore fprintf(output, "[%s]", level_str);
    }

    // Add location if needed
    if (log_s_config.shows_location) {
        ignore fprintf(output, "[%s:%d]", file, line);
    }

    // Add function name if needed
    if (log_s_config.shows_function) {
        ignore fprintf(output, "[%s]", func);
    }

    // Add a space before the message if we added any prefixes
    if (log_s_config.shows_timestamp
        || log_s_config.shows_level
        || log_s_config.shows_location
        || log_s_config.shows_function) {
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
