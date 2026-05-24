#include "dh/io/stream.h"
#include "dh/io/common.h"
#include "dh/io/Writer.h"
#include "dh/fs/File.h"
#include "dh/Thrd/Mtx.h"

#if plat_is_windows
#include "dh/os/windows/console.h"
#endif /* plat_is_windows */
#if io_stream_using_libc
#include <locale.h>
#endif /* io_stream_using_libc */

$static var_(io_stream__s_out_mtx, Thrd_Mtx_Recur) = cleared();
$static var_(io_stream__s_err_mtx, Thrd_Mtx_Recur) = cleared();

$attr($on_load)
$static fn_((io_stream__init(void))(void)) {
    io_stream__s_out_mtx = Thrd_Mtx_Recur_init();
    io_stream__s_err_mtx = Thrd_Mtx_Recur_init();
#if plat_is_windows
    // [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
    // chcp 65001
    let_ignore = SetConsoleCP(CP_UTF8);
    let_ignore = SetConsoleOutputCP(CP_UTF8);
#endif /* plat_is_windows */
#if io_stream_using_libc
    /* NOLINTNEXTLINE(concurrency-mt-unsafe) */
    let_ignore = setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
#endif /* io_stream_using_libc */
};

$attr($on_exit)
$static fn_((io_stream__fini(void))(void)) {
    Thrd_Mtx_Recur_fini(&io_stream__s_out_mtx);
    Thrd_Mtx_Recur_fini(&io_stream__s_err_mtx);
};

fn_((io_stream_nl(void))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_out_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_out_mtx));

    var stream_out_file = fs_File_io(io_getStdOut());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_nl(stream_out))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_crlf(void))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_out_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_out_mtx));

    var stream_out_file = fs_File_io(io_getStdOut());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_crlf(stream_out))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_print(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    };
};

fn_((io_stream_printVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_out_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_out_mtx));

    var stream_out_file = fs_File_io(io_getStdOut());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_printVaArgs(stream_out, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_println(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(fmt, va_args);
    };
};

fn_((io_stream_printlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_out_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_out_mtx));

    var stream_out_file = fs_File_io(io_getStdOut());
    let stream_out = fs_File_IO_writer(&stream_out_file);
    let_ignore = catch_((io_Writer_printlnVaArgs(stream_out, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_enl(void))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_err_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_err_mtx));

    var stream_err_file = fs_File_io(io_getStdErr());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_nl(stream_err))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_ecrlf(void))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_err_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_err_mtx));

    var stream_err_file = fs_File_io(io_getStdErr());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_crlf(stream_err))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_eprint(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    };
};

fn_((io_stream_eprintVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_err_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_err_mtx));

    var stream_err_file = fs_File_io(io_getStdErr());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_printVaArgs(stream_err, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_eprintln(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(fmt, va_args);
    };
};

fn_((io_stream_eprintlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    Thrd_Mtx_Recur_lock(&io_stream__s_err_mtx);
    defer_(Thrd_Mtx_Recur_unlock(&io_stream__s_err_mtx));

    var stream_err_file = fs_File_io(io_getStdErr());
    let stream_err = fs_File_IO_writer(&stream_err_file);
    let_ignore = catch_((io_Writer_printlnVaArgs(stream_err, fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);
