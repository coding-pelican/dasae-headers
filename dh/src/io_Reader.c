#include "dh/io/Reader.h"
#include "dh/io/common.h"
#include "dh/mem/common.h"

fn_((io_Reader_read(io_Reader self, S$u8 buf))(E$usize)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.read);
    claim_assert_nonnullS(buf);
    return self.read(self.ctx, buf);
};

fn_((io_Reader_readExact(io_Reader self, S$u8 buf))(E$void) $scope) {
    var_(idx, usize) = 0;
    while (idx != buf.len) {
        let bytes_read = try_(io_Reader_read(self, S_suffix((buf)idx)));
        if (bytes_read == 0) {
            return_err(io_Err_UnexpectedEof());
        }
        idx += bytes_read;
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Reader_readByte(io_Reader self))(E$u8) $scope) {
    var_(buf, A$$(1, u8)) = A_zero();
    let bytes_read = try_(io_Reader_read(self, A_ref$((S$u8)buf)));
    if (bytes_read == 0) {
        return_err(io_Err_UnexpectedEof());
    }
    return_ok(*A_at((buf)[0]));
} $unscoped_(fn);

fn_((io_Reader_skip(io_Reader self, usize n))(E$void) $scope) {
    var_(discard_buf, A$$(256, u8)) = A_zero();
    var_(remaining, usize) = n;
    while (remaining > 0) {
        let to_read = prim_min(remaining, A_len(discard_buf));
        let bytes_read = try_(io_Reader_read(self, A_slice$((S$u8)(discard_buf)$r(0, to_read))));
        if (bytes_read == 0) {
            return_err(io_Err_UnexpectedEof());
        }
        remaining -= bytes_read;
    }
    return_ok({});
} $unscoped_(fn);
