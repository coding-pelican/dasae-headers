#include "dh/io/stream.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/Str.h"

$static $inline
fn_((io_stream__writeLineFeed(io_Writer stream))(void)) {
    static let s_line_feed = pp_if_(bti_plat_windows)(
        pp_than_(u8_l("\r\n")),
        pp_else_('\n'));
    catch_((pp_if_(bti_plat_windows)(
        pp_than_(io_Writer_write),
        pp_else_(io_Writer_writeByte))(stream, s_line_feed))(
        $ignore, claim_unreachable
    ));
}

$static $inline
fn_((io_stream__printContent(io_Writer stream, Sli_const$u8 fmt, va_list va_args))(void)) {
    catch_((io_Writer_printVaArgs(stream, fmt, va_args))($ignore, claim_unreachable));
}

$static $inline
fn_((io_stream__printlnContent(io_Writer stream, Sli_const$u8 fmt, va_list va_args))(void)) {
    io_stream__printContent(stream, fmt, va_args);
    io_stream__writeLineFeed(stream);
}

fn_((io_stream_print(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    }
}

fn_((io_stream_printVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_out = fs_File_writer(io_getStdOut());
    io_stream__printContent(stream_out, fmt, va_args);
}

fn_((io_stream_println(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(fmt, va_args);
    }
}

fn_((io_stream_printlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_out = fs_File_writer(io_getStdOut());
    io_stream__printlnContent(stream_out, fmt, va_args);
}

fn_((io_stream_eprint(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    }
}

fn_((io_stream_eprintVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_err = fs_File_writer(io_getStdErr());
    io_stream__printlnContent(stream_err, fmt, va_args);
}

fn_((io_stream_eprintln(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(fmt, va_args);
    }
}

fn_((io_stream_eprintlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_err = fs_File_writer(io_getStdErr());
    io_stream__printlnContent(stream_err, fmt, va_args);
}
