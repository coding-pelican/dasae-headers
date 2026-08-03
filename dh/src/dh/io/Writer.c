#include "dh/io/Writer.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"

/*========== Internal Declarations ==========================================*/

$static fn_((io_Writer__mapPrintError(EAny err))(EAny));

/*========== External Definitions ===========================================*/

$static var_(io_Writer__ctx_noop, Void) $undefined_static;
let_(io_Writer_noop, io_Writer) = {
    .ctx = &io_Writer__ctx_noop,
    .writeFn = io_Writer_noWrite
};
fn_((io_Writer_noWrite(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnullS(bytes);
    return_ok(0);
} $unscoped(fn);

$static var_(io_Writer__ctx_failing, Void) $undefined_static;
let_(io_Writer_failing, io_Writer) = {
    .ctx = &io_Writer__ctx_failing,
    .writeFn = io_Writer_failingWrite
};
fn_((io_Writer_failingWrite(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnullS(bytes);
    return_err(E_cause$io_WriteFailed());
} $unscoped(fn);

fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(io_WriteE$usize)) {
    self = io_Writer_ensureValid(self);
    claim_assert_nonnullS(bytes);
    return self.writeFn(self.ctx, bytes);
};
fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(io_WriteE$void) $scope) {
    var_(idx, usize) = 0;
    while (idx != bytes.len) {
        idx += try_(io_Writer_write(self, S_suffix((bytes)(idx))));
    }
    return_ok({});
} $unscoped(fn);
fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(io_WriteE$void) $scope) {
    for_(($rt(n))($ignore)) {
        try_(io_Writer_writeBytes(self, bytes));
    } $end(for);
    return_ok({});
} $unscoped(fn);
fn_((io_Writer_writeByte(io_Writer self, u8 byte))(io_WriteE$void)) {
    var_(bytes, A$$(1, u8)) = A_init({ [0] = byte });
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(io_WriteE$void) $scope) {
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

fn_((io_Writer_lf(io_Writer self))(io_WriteE$void)) {
    return io_Writer_writeByte(self, io_lf_byte);
};
fn_((io_Writer_crlf(io_Writer self))(io_WriteE$void)) {
    return io_Writer_writeBytes(self, u8_l(io_crlf));
};
fn_((io_Writer_nl(io_Writer self))(io_WriteE$void)) {
    return io_Writer_writeBytes(self, u8_l(io_nl));
};
fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(io_PrintE$void) $guard) {
    var_(va_args, va_list) $undefined;
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printVaArgs(self, fmt, va_args)));
} $unguarded(fn);
fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(io_PrintE$void) $scope) {
    catch_((fmt_formatVaArgs(self, fmt, va_args))(
        err, return_err(io_Writer__mapPrintError(err))
    ));
    return_ok({});
} $unscoped(fn);
fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(io_PrintE$void) $guard) {
    var_(va_args, va_list) $undefined;
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printlnVaArgs(self, fmt, va_args)));
} $unguarded(fn);
fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(io_PrintE$void) $scope) {
    catch_((fmt_formatVaArgs(self, fmt, va_args))(
        err, return_err(io_Writer__mapPrintError(err))
    ));
    try_(io_Writer_nl(self));
    return_ok({});
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

fn_((io_Writer__mapPrintError(EAny err))(EAny)) {
    let write_err = (io_WriteE){ .any = err };
    if (isSome(E_resolve$io_WriteE(write_err))) return err;
    let fmt_err = (fmt_E){ .any = err };
    if (isSome(E_resolve$fmt_E(fmt_err))) return err;
    return *E_cause$io_PrintFailed().as_any;
};
