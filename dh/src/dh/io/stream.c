#include "dh/io/stream.h"
#include "dh/proc/std.h"
#include "dh/io/common.h"
#include "dh/io/Writer.h"
#include "dh/fs/File.h"

fn_((io_stream_lf(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_out_file = fs_File_io(proc_std_out());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_lf(stream_out))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_crlf(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_out_file = fs_File_io(proc_std_out());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_crlf(stream_out))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_nl(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_out_file = fs_File_io(proc_std_out());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_nl(stream_out))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_print(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    };
};

fn_((io_stream_printVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_out_file = fs_File_io(proc_std_out());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_printVaArgs(stream_out, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_println(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(fmt, va_args);
    };
};

fn_((io_stream_printlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_out_file = fs_File_io(proc_std_out());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_printlnVaArgs(stream_out, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_elf(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_err_file = fs_File_io(proc_std_err());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_lf(stream_err))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_ecrlf(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdOut();
    defer_(io_unlockStdOut());
#endif /* io_locked_std_enabled */
    var stream_err_file = fs_File_io(proc_std_err());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_crlf(stream_err))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_enl(void))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdErr();
    defer_(io_unlockStdErr());
#endif /* io_locked_std_enabled */
    var stream_err_file = fs_File_io(proc_std_err());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_nl(stream_err))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_eprint(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    };
};

fn_((io_stream_eprintVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdErr();
    defer_(io_unlockStdErr());
#endif /* io_locked_std_enabled */
    var stream_err_file = fs_File_io(proc_std_err());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_printVaArgs(stream_err, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_eprintln(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(fmt, va_args);
    };
};

fn_((io_stream_eprintlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
#if io_locked_std_enabled
    io_lockStdErr();
    defer_(io_unlockStdErr());
#endif /* io_locked_std_enabled */
    var stream_err_file = fs_File_io(proc_std_err());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_printlnVaArgs(stream_err, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);
