#include "dh/io/Writer.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"
#include "dh/math/common.h"

fn_((io_Writer_write(io_Writer self, Sli_const$u8 bytes))(Err$usize)) {
    return self.write(self.ctx, bytes);
}

fn_((io_Writer_writeBytes(io_Writer self, Sli_const$u8 bytes))(Err$void) $scope) {
    usize index = 0;
    while (index != bytes.len) {
        index += try_(io_Writer_write(self, Sli_suffix(bytes, index)));
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_writeBytesN(io_Writer self, Sli_const$u8 bytes, usize n))(Err$void) $scope) {
    for (usize index = 0; index < n; ++index) {
        try_(io_Writer_writeBytes(self, bytes));
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_writeByte(io_Writer self, u8 byte))(Err$void)) {
    use_Arr$(1, u8);
    Arr$1$u8 bytes = Arr_init({ byte });
    return io_Writer_writeBytes(self, Arr_ref$(Sli$u8, bytes).as_const);
};

fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(Err$void) $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 bytes = Arr_zero();
    bti_memset(bytes.buf, byte, Arr_len(bytes));
    usize remaining = n;
    while (0 < remaining) {
        let to_write = prim_min(remaining, Arr_len(bytes));
        try_(io_Writer_writeBytes(self, Arr_slice$(Sli_const$u8, bytes, $r(0, to_write))));
        remaining -= to_write;
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_print(io_Writer self, Sli_const$u8 fmt, ...))(Err$void) $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printVaArgs(self, fmt, va_args)));
} $unguarded_(fn);

fn_((io_Writer_printVaArgs(io_Writer self, Sli_const$u8 fmt, va_list va_args))(Err$void) $scope) {
    return_ok(try_(fmt_formatVaArgs(self, fmt, va_args)));
} $unscoped_(fn);

fn_((io_Writer_println(io_Writer self, Sli_const$u8 fmt, ...))(Err$void) $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(io_Writer_printlnVaArgs(self, fmt, va_args)));
} $unguarded_(fn);

fn_((io_Writer_printlnVaArgs(io_Writer self, Sli_const$u8 fmt, va_list va_args))(Err$void) $scope) {
    try_(fmt_formatVaArgs(self, fmt, va_args));
    try_(io_Writer_nl(self));
    return_ok({});
} $unscoped_(fn);

fn_((io_Writer_nl(io_Writer self))(Err$void) $scope) {
    static let pp_if_(bti_plat_windows)(
        pp_than_(s_crlf = u8_l("\r\n")),
        pp_else_(s_lf = u8_c('\n'))
    );
    static let s_line_feed = pp_if_(bti_plat_windows)(
        pp_than_(s_crlf),
        pp_else_(s_lf)
    );
    static let s_write = pp_if_(bti_plat_windows)(
        pp_than_(io_Writer_write),
        pp_else_(io_Writer_writeByte)
    );
    try_(s_write(self, s_line_feed));
    return_ok({});
} $unscoped_(fn);
