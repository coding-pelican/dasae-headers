#include "dh/io/Writer.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"
#include "dh/math/common.h"

fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(E$usize)) {
    return self.write(self.ctx, bytes);
}

fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(E$void) $scope) {
    usize index = 0;
    while (index != bytes.len) {
        index += try_(io_Writer_write(self, suffix$S(bytes, index)));
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(E$void) $scope) {
    for (usize index = 0; index < n; ++index) {
        try_(io_Writer_writeBytes(self, bytes));
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_writeByte(io_Writer self, u8 byte))(E$void)) {
    A$$(1, u8) bytes = init$A({ byte });
    return io_Writer_writeBytes(self, ref$A$((const u8)(bytes)));
};

fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(E$void) $scope) {
    A$$(256, u8) bytes = zero$A();
    bti_memset(bytes.val, byte, len$A(bytes));
    usize remaining = n;
    while (0 < remaining) {
        let to_write = prim_min(remaining, len$A(bytes));
        try_(io_Writer_writeBytes(self, slice$A$((const u8)(bytes, $r(0, to_write)))));
        remaining -= to_write;
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(E$void) $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printVaArgs(self, fmt, va_args)));
} $unguarded_(fn);

fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    return_ok(try_(fmt_formatVaArgs(self, fmt, va_args)));
} $unscoped_(fn);

fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(E$void) $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printlnVaArgs(self, fmt, va_args)));
} $unguarded_(fn);

fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    try_(fmt_formatVaArgs(self, fmt, va_args));
    try_(io_Writer_nl(self));
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_nl(io_Writer self))(E$void) $scope) {
    static let pp_if_(bti_plat_windows)(
        pp_then_(s_crlf = u8_l("\r\n")),
        pp_else_(s_lf = u8_c('\n'))
    );
    static let s_line_feed = pp_if_(bti_plat_windows)(
        pp_then_(s_crlf),
        pp_else_(s_lf)
    );
    static let s_write = pp_if_(bti_plat_windows)(
        pp_then_(io_Writer_write),
        pp_else_(io_Writer_writeByte)
    );
    try_(s_write(self, s_line_feed));
    return_ok({});
} $unscoped_(fn);
