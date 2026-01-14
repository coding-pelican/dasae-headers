#include "dh/log.h"
#include "dh/fs/Dir.h"
#include <stdarg.h>
#include <time.h>

// Global state
$static var_(log__config, log_Config) = {
    .output_file = null, // Default to stderr
    .min_level = log_Level_info, // Default to Info level
    .shows_timestamp = true, // Show timestamps by default
    .shows_level = true, // Show log level by default
    .shows_location = true, // Show location by default
    .shows_function = true // Show function name by default
};

fn_((log_init(const char* filename))(fs_File_Err$void) $guard) {
    // Extract directory path
    var_(dir_path, A$$(256, u8)) = A_zero();
    if_(let dir_last_slash = strrchr(filename, '/'), dir_last_slash) {
        let dir_len = dir_last_slash - filename;
        strncpy(ptrCast$((char*)(A_ptr(dir_path))), filename, intCast$((usize)(dir_len)));
        *A_at((dir_path)[dir_len]) = '\0';

        // Create directory
        try_(fs_Dir_create(A_slice$((S_const$u8)(dir_path)$r(0, as$(usize)(dir_len)))));
    }

    let file = fopen(filename, "w");
    if (!file) { return_err(fs_File_Err_OpenFailed()); }
    errdefer_($ignore, let_ignore = fclose(file));

    if (log__config.output_file && log__config.output_file != stderr) {
        let_ignore = fclose(log__config.output_file);
    }
    log__config.output_file = file;
    return_ok({});
} $unguarded_(fn);

fn_((log_initWithFile(FILE* file))(void)) {
    if (log__config.output_file && log__config.output_file != stderr) {
        let_ignore = fclose(log__config.output_file);
    }
    log__config.output_file = file;
};

fn_((log_fini(void))(void)) {
    if (log__config.output_file && log__config.output_file != stderr) {
        let_ignore = fclose(log__config.output_file);
        log__config.output_file = stderr;
    }
};

fn_((log_setLevel(log_Level level))(void)) {
    log__config.min_level = level;
};

fn_((log_showTimestamp(bool shows))(void)) {
    log__config.shows_timestamp = shows;
};

fn_((log_showLevel(bool shows))(void)) {
    log__config.shows_level = shows;
};

fn_((log_showLocation(bool shows))(void)) {
    log__config.shows_location = shows;
};

fn_((log_showFunction(bool shows))(void)) {
    log__config.shows_function = shows;
};

fn_((log_getLevel(void))(log_Level)) {
    return log__config.min_level;
};

fn_((log_getOutputFile(void))(FILE*)) {
    return log__config.output_file ? log__config.output_file : stderr;
};

fn_((log_message(log_Level level, const char* file, int line, const char* func, const char* fmt, ...))(void)) {
    if (level < log__config.min_level) { return; }

    let output = log_getOutputFile();

    // Get current time if needed
    if (log__config.shows_timestamp) {
        time_t t = time(null);
        struct tm* lt = localtime(&t);
        char time_str[16] = cleared();
        let_ignore = strftime(time_str, sizeof(time_str), "%H:%M:%S", lt);
        let_ignore = fprintf(output, "[%s]", time_str);
    }

    // Add level if needed
    if (log__config.shows_level) {
        const char* level_str = "????";
        switch (level) {
        case log_Level_debug: level_str = "DEBUG"; break;
        case log_Level_info:  level_str = "INFO"; break;
        case log_Level_warn:  level_str = "WARN"; break;
        case log_Level_error: level_str = "ERROR"; break;
        case log_Level_count: claim_unreachable;
        }
        let_ignore = fprintf(output, "[%s]", level_str);
    }

    // Add location if needed
    if (log__config.shows_location) {
        let_ignore = fprintf(output, "[%s:%d]", file, line);
    }

    // Add function name if needed
    if (log__config.shows_function) {
        let_ignore = fprintf(output, "[%s]", func);
    }

    // Add a space before the message if we added any prefixes
    if (log__config.shows_timestamp
        || log__config.shows_level
        || log__config.shows_location
        || log__config.shows_function) {
        let_ignore = fprintf(output, " ");
    }

    // Print the actual message
    with_(va_list args = null) with_fini_(va_start(args, fmt), va_end(args)) {
        let_ignore = vfprintf(output, fmt, args);
    }

    // Add newline and flush
    let_ignore = fprintf(output, "\n");
    let_ignore = fflush(output);
};
