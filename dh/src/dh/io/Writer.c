#include "dh/io/Writer.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"

fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(E$usize)) {
    self = io_Writer_ensureValid(self);
    claim_assert_nonnullS(bytes);
    return self.writeFn(self.ctx, bytes);
};

fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(E$void) $scope) {
    var_(idx, usize) = 0;
    while (idx != bytes.len) {
        idx += try_(io_Writer_write(self, S_suffix((bytes)(idx))));
    }
    return_ok({});
} $unscoped(fn);

fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(E$void) $scope) {
    for_(($rt(n))($ignore)) {
        try_(io_Writer_writeBytes(self, bytes));
    } $end(for);
    return_ok({});
} $unscoped(fn);

fn_((io_Writer_writeByte(io_Writer self, u8 byte))(E$void)) {
    var_(bytes, A$$(1, u8)) = A_init({ [0] = byte });
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};

fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(E$void) $scope) {
    var_(bytes, A$$(256, u8)) $undefined;
    mem_setBytes(A_ref$((S$u8)(bytes)), byte);
    var_(remaining, usize) = n;
    while (0 < remaining) {
        let to_write = pri_min(remaining, A_len(bytes));
        try_(io_Writer_writeBytes(self, A_slice$((S_const$u8)(bytes)$r(0, to_write))));
        remaining -= to_write;
    }
    return_ok({});
} $unscoped(fn);

fn_((io_Writer_lf(io_Writer self))(E$void)) {
    return io_Writer_writeByte(self, io_lf_byte);
};

fn_((io_Writer_crlf(io_Writer self))(E$void)) {
    return io_Writer_writeBytes(self, u8_l(io_crlf));
};

fn_((io_Writer_nl(io_Writer self))(E$void)) {
    return io_Writer_writeBytes(self, u8_l(io_nl));
};

fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(E$void) $guard) {
    var_(va_args, va_list) $undefined;
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printVaArgs(self, fmt, va_args)));
} $unguarded(fn);

fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    return_ok(try_(fmt_formatVaArgs(self, fmt, va_args)));
} $unscoped(fn);

fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(E$void) $guard) {
    var_(va_args, va_list) $undefined;
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printlnVaArgs(self, fmt, va_args)));
} $unguarded(fn);

fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    try_(fmt_formatVaArgs(self, fmt, va_args));
    try_(io_Writer_nl(self));
    return_ok({});
} $unscoped(fn);
