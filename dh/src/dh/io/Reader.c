#include "dh/io/Reader.h"
#include "dh/io/Writer.h"
#include "dh/mem/common.h"

$static var_(io_Reader__ctx_failing, Void) $undefined_static;
let_(io_Reader_failing, io_Reader) = {
    .ctx = &io_Reader__ctx_failing,
    .readFn = io_Reader_failingRead
};
fn_((io_Reader_failingRead(P$raw ctx, S$u8 out_buf))(io_ReadE$usize) $scope) {
    let_ignore = ctx;
    let_ignore = out_buf;
    return_err(E_cause$io_ReadFailed());
} $unscoped(fn);

fn_((io_Reader_read(io_Reader self, S$u8 out_bytes))(io_ReadE$usize)) {
    self = io_Reader_ensureValid(self);
    claim_assert_nonnullS(out_bytes);
    return self.readFn(self.ctx, out_bytes);
};

fn_((io_Reader_readByte(io_Reader self))(io_ReadExactE$u8) $scope) {
    var_(byte_buf, A$$(1, u8)) $undefined;
    let bytes_read = try_(io_Reader_read(self, A_ref$((S$u8)byte_buf)));
    if (bytes_read == 0) return_err(E_cause$UnexpectedEOF());
    return_ok(*A_at((byte_buf)[0]));
} $unscoped(fn);

fn_((io_Reader_readExact(io_Reader self, S$u8 out_bytes))(io_ReadExactE$void) $scope) {
    let bytes_read = try_(io_Reader_readAtLeast(self, out_bytes, out_bytes.len));
    if (bytes_read != out_bytes.len) return_err(E_cause$UnexpectedEOF());
    return_ok({});
} $unscoped(fn);

fn_((io_Reader_readAtLeast(io_Reader self, S$u8 out_bytes, usize least_len))(io_ReadE$usize) $scope) {
    claim_assert(least_len <= out_bytes.len);
    var_(idx, usize) = 0;
    while (idx < least_len) {
        let bytes_read = try_(io_Reader_read(self, S_suffix((out_bytes)(idx))));
        if (bytes_read == 0) return_ok(idx);
        idx += bytes_read;
    }
    return_ok(idx);
} $unscoped(fn);

fn_((io_Reader_skip(io_Reader self, usize discard_len))(io_ReadExactE$void) $scope) {
    let skipped_len = try_(io_Reader_skipAtLeast(self, discard_len));
    if (skipped_len != discard_len) return_err(E_cause$UnexpectedEOF());
    return_ok({});
} $unscoped(fn);

fn_((io_Reader_skipByte(io_Reader self))(io_ReadExactE$void) $scope) {
    return io_Reader_skip(self, 1);
} $unscoped(fn);

fn_((io_Reader_skipAtLeast(io_Reader self, usize least_len))(io_ReadE$usize) $scope) {
    var_(discard_buf, A$$(256, u8)) $undefined;
    var_(skipped_len, usize) = 0;
    while (skipped_len < least_len) {
        let remaining_len = least_len - skipped_len;
        let to_read = pri_min(remaining_len, A_len(discard_buf));
        let bytes_read = try_(io_Reader_read(self, A_slice$((S$u8)(discard_buf)$r(0, to_read))));
        if (bytes_read == 0) return_ok(skipped_len);
        skipped_len += bytes_read;
    }
    return_ok(skipped_len);
} $unscoped(fn);

fn_((io_Reader_copy(io_Reader self, io_Writer writer))(io_CopyE$usize) $scope) {
    var_(temp_buf, A$$(1024, u8)) $undefined;
    var_(total, usize) = 0;
    while (true) {
        let bytes_read = try_(io_Reader_read(self, A_ref$((S$u8)(temp_buf))));
        if (bytes_read == 0) return_ok(total);
        try_(io_Writer_writeBytes(writer, A_slice$((S_const$u8)(temp_buf)$r(0, bytes_read))));
        total += bytes_read;
    }
} $unscoped(fn);

fn_((io_Reader_copyByte(io_Reader self, io_Writer writer))(io_CopyExactE$void) $scope) {
    let byte = try_(io_Reader_readByte(self));
    try_(io_Writer_writeByte(writer, byte));
    return_ok({});
} $unscoped(fn);

fn_((io_Reader_copyExact(io_Reader self, io_Writer writer, usize copy_len))(io_CopyExactE$void) $scope) {
    let copied_len = try_(io_Reader_copyAtLeast(self, writer, copy_len));
    if (copied_len != copy_len) return_err(E_cause$UnexpectedEOF());
    return_ok({});
} $unscoped(fn);

fn_((io_Reader_copyAtLeast(io_Reader self, io_Writer writer, usize least_len))(io_CopyE$usize) $scope) {
    var_(temp_buf, A$$(1024, u8)) $undefined;
    var_(copied_len, usize) = 0;
    while (copied_len < least_len) {
        let remaining_len = least_len - copied_len;
        let to_read = pri_min(remaining_len, A_len(temp_buf));
        let bytes_read = try_(io_Reader_read(self, A_slice$((S$u8)(temp_buf)$r(0, to_read))));
        if (bytes_read == 0) return_ok(copied_len);
        try_(io_Writer_writeBytes(writer, A_slice$((S_const$u8)(temp_buf)$r(0, bytes_read))));
        copied_len += bytes_read;
    }
    return_ok(copied_len);
} $unscoped(fn);
