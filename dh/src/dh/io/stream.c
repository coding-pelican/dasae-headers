#include "dh/io/stream.h"
#include "dh/io/common.h"
#include "dh/io/Writer.h"

$static fn_((io_stream__std(void))(io_std_Self)) {
    return catch_((io_std_direct())($ignore, io_std_noop));
};

fn_((io_stream_lf(void))(void) $guard) {
    let locked = io_std_lockOut(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_lf(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_crlf(void))(void) $guard) {
    let locked = io_std_lockOut(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_crlf(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_nl(void))(void) $guard) {
    let locked = io_std_lockOut(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_nl(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_print(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    };
};
fn_((io_stream_printVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    let locked = io_std_lockOut(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_printVaArgs(io_Locked_writer(locked), fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_println(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(fmt, va_args);
    };
};
fn_((io_stream_printlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    let locked = io_std_lockOut(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_printlnVaArgs(io_Locked_writer(locked), fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);

fn_((io_stream_elf(void))(void) $guard) {
    let locked = io_std_lockErr(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_lf(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_ecrlf(void))(void) $guard) {
    let locked = io_std_lockErr(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_crlf(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_enl(void))(void) $guard) {
    let locked = io_std_lockErr(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_nl(io_Locked_writer(locked)))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_eprint(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    };
};
fn_((io_stream_eprintVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    let locked = io_std_lockErr(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_printVaArgs(io_Locked_writer(locked), fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);
fn_((io_stream_eprintln(S_const$u8 fmt, ...))(void)) {
    using_(var_(va_args, va_list) $undefined) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(fmt, va_args);
    };
};
fn_((io_stream_eprintlnVaArgs(S_const$u8 fmt, va_list va_args))(void) $guard) {
    let locked = io_std_lockErr(io_stream__std());
    defer_(io_Locked_Writer_unlock(locked));
    let_ignore = catch_((io_Writer_printlnVaArgs(io_Locked_writer(locked), fmt, va_args))($ignore, $do_nothing));
} $unguarded(fn);
