#include "dh/io/stream.h"
#include "dh/io/common.h"
#include "dh/io/Writer.h"
#include "dh/fs/File.h"

fn_((io_stream_nl(void))(void)) {
    let stream_out = fs_File_writer(io_getStdOut());
    catch_((io_Writer_nl(stream_out))($ignore, claim_unreachable));
}

fn_((io_stream_print(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    }
}

fn_((io_stream_printVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_out = fs_File_writer(io_getStdOut());
    catch_((io_Writer_printVaArgs(stream_out, fmt, va_args))($ignore, claim_unreachable));
}

fn_((io_stream_println(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(fmt, va_args);
    }
}

fn_((io_stream_printlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_out = fs_File_writer(io_getStdOut());
    catch_((io_Writer_printlnVaArgs(stream_out, fmt, va_args))($ignore, claim_unreachable));
}

fn_((io_stream_enl(void))(void)) {
    let stream_err = fs_File_writer(io_getStdErr());
    catch_((io_Writer_nl(stream_err))($ignore, claim_unreachable));
}

fn_((io_stream_eprint(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    }
}

fn_((io_stream_eprintVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_err = fs_File_writer(io_getStdErr());
    catch_((io_Writer_printlnVaArgs(stream_err, fmt, va_args))($ignore, claim_unreachable));
}

fn_((io_stream_eprintln(Sli_const$u8 fmt, ...))(void)) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(fmt, va_args);
    }
}

fn_((io_stream_eprintlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void)) {
    let stream_err = fs_File_writer(io_getStdErr());
    catch_((io_Writer_printlnVaArgs(stream_err, fmt, va_args))($ignore, claim_unreachable));
}
