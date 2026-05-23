#include "dh/log.h"
#include "dh/fs/Dir.h"
#include "dh/io/common.h"
#include "dh/mem/common.h"
#include "dh/time/SysTime.h"

$static var_(log__config, log_Config) = {
    .output_file = cleared(),
    .has_output_file = false,
    .owns_output_file = false,
    .min_level = log_Level_info,
    .shows_timestamp = true,
    .shows_level = true,
    .shows_location = true,
    .shows_function = true,
};

$static fn_((log__closeOwnedOutput(void))(void)) {
    if (log__config.has_output_file && log__config.owns_output_file) {
        fs_File_close(log__config.output_file);
    }
    asg_l((&log__config.output_file)(cleared()));
    log__config.has_output_file = false;
    log__config.owns_output_file = false;
}

$static fn_((log__levelStr(log_Level level))(S_const$u8)) {
    switch (level) {
    case log_Level_debug: return u8_l("DEBUG");
    case log_Level_info: return u8_l("INFO");
    case log_Level_warn: return u8_l("WARN");
    case log_Level_error: return u8_l("ERROR");
    case log_Level_count: claim_unreachable;
    }
    claim_unreachable;
}

$static fn_((log__writeTimestamp(io_Writer writer))(void)) {
    let secs = time_SysTime_toUnixEpoch(time_SysTime_now()) % u64_(86, 400ull);
    let hour = as$(u32)(secs / u64_(3, 600ull));
    let minute = as$(u32)((secs / u64_(60ull)) % u64_(60ull));
    let second = as$(u32)(secs % u64_(60ull));
    let_ignore = catch_((io_Writer_print(writer, u8_l("[{:u}:{:u}:{:u}]"), hour, minute, second))($ignore, $do_nothing));
}

fn_((log_init(const char* filename))(E$void) $scope) {
    claim_assert_nonnull(filename);
    let path = mem_spanZ0$u8(ptrCast$((const u8*)(filename)));
    if_some((mem_findLastAnyBytes(path, u8_l("/\\")))(sep_idx)) {
        if (sep_idx != 0) {
            try_(fs_Dir_create(S_prefix((path)(sep_idx))));
        }
    }

    let file = try_(fs_File_create(path, fs_File_CreateFlags_default));
    log__closeOwnedOutput();
    log__config.output_file = file;
    log__config.has_output_file = true;
    log__config.owns_output_file = true;
    return_ok({});
} $unscoped(fn);

fn_((log_initWithFile(fs_File file))(void)) {
    log__closeOwnedOutput();
    log__config.output_file = file;
    log__config.has_output_file = true;
    log__config.owns_output_file = false;
};

fn_((log_fini(void))(void)) {
    log__closeOwnedOutput();
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

fn_((log_getOutputFile(void))(fs_File)) {
    return log__config.has_output_file ? log__config.output_file : io_getStdErr();
};

fn_((log_message(log_Level level, const char* file, int line, const char* func, const char* fmt, ...))(void)) {
    if (level < log__config.min_level) { return; }

    var output_io = fs_File_io(log_getOutputFile());
    let writer = fs_File_IO_writer(&output_io);

    if (log__config.shows_timestamp) {
        log__writeTimestamp(writer);
    }

    if (log__config.shows_level) {
        let_ignore = catch_((io_Writer_print(writer, u8_l("[{:s}]"), log__levelStr(level)))($ignore, $do_nothing));
    }

    if (log__config.shows_location) {
        let file_s = mem_spanZ0$u8(ptrCast$((const u8*)(file)));
        let_ignore = catch_((io_Writer_print(writer, u8_l("[{:s}:{:d}]"), file_s, line))($ignore, $do_nothing));
    }

    if (log__config.shows_function) {
        let func_s = mem_spanZ0$u8(ptrCast$((const u8*)(func)));
        let_ignore = catch_((io_Writer_print(writer, u8_l("[{:s}]"), func_s))($ignore, $do_nothing));
    }

    if (log__config.shows_timestamp
        || log__config.shows_level
        || log__config.shows_location
        || log__config.shows_function) {
        let_ignore = catch_((io_Writer_writeByte(writer, u8_c(' ')))($ignore, $do_nothing));
    }

    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        let fmt_s = mem_spanZ0$u8(ptrCast$((const u8*)(fmt)));
        let_ignore = catch_((io_Writer_printVaArgs(writer, fmt_s, args))($ignore, $do_nothing));
    }

    let_ignore = catch_((io_Writer_nl(writer))($ignore, $do_nothing));
};
