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
    return_ok(bytes.len);
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

fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    self = io_Writer_ensureValid(self);
    claim_assert_nonnullS(bytes);
    let written = try_(self.writeFn(self.ctx, bytes));
    claim_assert(written <= bytes.len);
    return_ok(written);
} $unscoped(fn);
fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(io_WriteE$void) $scope) {
    var_(idx, usize) = 0;
    while (idx != bytes.len) {
        let remaining = S_suffix((bytes)(idx));
        let written = try_(io_Writer_write(self, remaining));
        if (written == 0) return_err(E_cause$io_WriteFailed());
        idx += written;
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

fn_((io_Writer_writeInt$usize(io_Writer self, usize value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    if (endian == mem_Endian_little) {
        let bytes = mem_writeLESize(value);
        return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
    }
    let bytes = mem_writeBESize(value);
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeInt$u64(io_Writer self, u64 value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    if (endian == mem_Endian_little) {
        let bytes = mem_writeLE64(value);
        return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
    }
    let bytes = mem_writeBE64(value);
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeInt$ulong(io_Writer self, ulong value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    if (endian == mem_Endian_little) {
        let bytes = mem_writeLELong(value);
        return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
    }
    let bytes = mem_writeBELong(value);
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeInt$u32(io_Writer self, u32 value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    if (endian == mem_Endian_little) {
        let bytes = mem_writeLE32(value);
        return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
    }
    let bytes = mem_writeBE32(value);
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeInt$u16(io_Writer self, u16 value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    if (endian == mem_Endian_little) {
        let bytes = mem_writeLE16(value);
        return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
    }
    let bytes = mem_writeBE16(value);
    return io_Writer_writeBytes(self, A_ref$((S_const$u8)(bytes)));
};
fn_((io_Writer_writeInt$u8(io_Writer self, u8 value, mem_Endian endian))(io_WriteE$void)) {
    claim_assert(mem_Endian_isValid(endian));
    return io_Writer_writeByte(self, value);
};
fn_((io_Writer_writeInt$isize(io_Writer self, isize value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$usize(self, bitCast$((usize)(value)), endian);
};
fn_((io_Writer_writeInt$i64(io_Writer self, i64 value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$u64(self, bitCast$((u64)(value)), endian);
};
fn_((io_Writer_writeInt$ilong(io_Writer self, ilong value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$ulong(self, bitCast$((ulong)(value)), endian);
};
fn_((io_Writer_writeInt$i32(io_Writer self, i32 value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$u32(self, bitCast$((u32)(value)), endian);
};
fn_((io_Writer_writeInt$i16(io_Writer self, i16 value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$u16(self, bitCast$((u16)(value)), endian);
};
fn_((io_Writer_writeInt$i8(io_Writer self, i8 value, mem_Endian endian))(io_WriteE$void)) {
    return io_Writer_writeInt$u8(self, bitCast$((u8)(value)), endian);
};

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
