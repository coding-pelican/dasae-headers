#include "dh/io/Writer.h"
#include "dh/Arr.h"

fn_(io_Writer_write(io_Writer self, Sli_const$u8 bytes), Err$usize) {
    return self.write(self.ctx, bytes);
}

fn_(io_Writer_writeAll(io_Writer self, Sli_const$u8 bytes), Err$void, $scope) {
    usize index = 0;
    while (index != bytes.len) {
        index += try_(io_Writer_write(self, Sli_suffix(bytes, index)));
    }
    return_ok({});
} $unscoped;

fn_(io_Writer_writeByte(io_Writer self, u8 byte), Err$void) {
    use_Arr$(1, u8);
    Arr$1$u8 bytes = Arr_init({ byte });
    return io_Writer_writeAll(self, Sli_arr$(Sli_const$u8, bytes));
};

fn_(io_Writer_writeByteNTimes(io_Writer self, u8 byte, usize n), Err$void, $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 bytes = Arr_zero();
    bti_memset(bytes.buf, byte, Arr_len(bytes));
    usize remaining = n;
    while (0 < remaining) {
        let to_write = prim_min(remaining, Arr_len(bytes));
        try_(io_Writer_writeAll(self, Arr_slice$(Sli_const$u8, bytes, (0, to_write))));
        remaining -= to_write;
    }
    return_ok({});
} $unscoped;

fn_(io_Writer_writeBytesNTimes(io_Writer self, Sli_const$u8 bytes, usize n), Err$void, $scope) {
    for (usize index = 0; index < n; ++index) {
        try_(io_Writer_writeAll(self, bytes));
    }
    return_ok({});
} $unscoped;
