#include "dh/io/stream.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"

fn_(io_stream_print(Sli_const$u8 fmt, ...), void) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(fmt, va_args);
    }
}

fn_(io_stream_printVaArgs(Sli_const$u8 fmt, va_list va_args), void) {
    let stream_out = fs_File_writer(io_getStdOut());
    catch_((io_Writer_printVaArgs(stream_out, fmt, va_args))($ignore_capture, claim_unreachable));
}

fn_(io_stream_eprint(Sli_const$u8 fmt, ...), void) {
    va_list va_args = {};
    with_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(fmt, va_args);
    }
}

fn_(io_stream_eprintVaArgs(Sli_const$u8 fmt, va_list va_args), void) {
    let stream_err = fs_File_writer(io_getStdErr());
    catch_((io_Writer_printVaArgs(stream_err, fmt, va_args))($ignore_capture, claim_unreachable));
}
