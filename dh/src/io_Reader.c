#include "dh/io/Reader.h"
#include "dh/io/common.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/mem/common.h"
#include "dh/math/common.h"

fn_((io_Reader_read(io_Reader self, S$u8 buf))(E$usize)) {
    return self.read(self.ctx, buf);
}

fn_((io_Reader_readExact(io_Reader self, S$u8 buf))(E$void) $scope) {
    usize index = 0;
    while (index != buf.len) {
        let bytes_read = try_(io_Reader_read(self, suffix$S(buf, index)));
        if (bytes_read == 0) {
            return_err(io_Err_UnexpectedEof());
        }
        index += bytes_read;
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Reader_readByte(io_Reader self))(E$u8) $scope) {
    A$$(1, u8) buf = zero$A();
    let bytes_read = try_(io_Reader_read(self, ref$A$((u8)(buf))));
    if (bytes_read == 0) {
        return_err(io_Err_UnexpectedEof());
    }
    return_ok(buf.val[0]);
} $unscoped_(fn);

fn_((io_Reader_skip(io_Reader self, usize n))(E$void) $scope) {
    A$$(256, u8) discard_buffer = zero$A();
    usize remaining = n;
    while (0 < remaining) {
        let to_read = prim_min(remaining, len$A(discard_buffer));
        let bytes_read = try_(io_Reader_read(self, slice$A$((u8)(discard_buffer, $r(0, to_read)))));
        if (bytes_read == 0) {
            return_err(io_Err_UnexpectedEof());
        }
        remaining -= bytes_read;
    }
    return_ok({});
} $unscoped_(fn);
