#include "stream.h"

/*========== Internal Definitions ===========================================*/

$static fn_((io_stream__printVaArgs(io_Self self, io_Stream stream, S_const$u8 fmt, va_list va_args))(void)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->printVaArgsFn);
    self.vtbl->printVaArgsFn(self.ctx, stream, fmt, va_args);
};

$static fn_((io_stream__nl(io_Self self, io_Stream stream))(void)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->nlFn);
    self.vtbl->nlFn(self.ctx, stream);
};

/*========== External Definitions ===========================================*/

fn_((io_stream_nl(io_Self self))(void)) {
    io_stream__nl(self, io_Stream_out);
};

fn_((io_stream_print(io_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var va_args = l0$((va_list))) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printVaArgs(self, fmt, va_args);
    };
};

fn_((io_stream_printVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void)) {
    io_stream__printVaArgs(self, io_Stream_out, fmt, va_args);
};

fn_((io_stream_println(io_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var va_args = l0$((va_list))) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_printlnVaArgs(self, fmt, va_args);
    };
};

fn_((io_stream_printlnVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void)) {
    io_stream__printVaArgs(self, io_Stream_out, fmt, va_args);
    io_stream__nl(self, io_Stream_out);
};

fn_((io_stream_enl(io_Self self))(void)) {
    io_stream__nl(self, io_Stream_err);
};

fn_((io_stream_eprint(io_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var va_args = l0$((va_list))) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintVaArgs(self, fmt, va_args);
    };
};

fn_((io_stream_eprintVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void)) {
    io_stream__printVaArgs(self, io_Stream_err, fmt, va_args);
};

fn_((io_stream_eprintln(io_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var va_args = l0$((va_list))) using_fini_(va_start(va_args, fmt), va_end(va_args)) {
        io_stream_eprintlnVaArgs(self, fmt, va_args);
    };
};

fn_((io_stream_eprintlnVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void)) {
    io_stream__printVaArgs(self, io_Stream_err, fmt, va_args);
    io_stream__nl(self, io_Stream_err);
};
