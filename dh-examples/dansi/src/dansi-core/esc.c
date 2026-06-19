#include "dansi-core/esc.h"
#include <dh/io/Fixed.h>

fn_((dansi_esc_parse(S_const$u8 bytes))(dansi_esc_E$dansi_esc_Frame) $scope) {
    if (bytes.len < 2 || *S_at((bytes)[0]) != 0x1b) {
        return_err(E_cause$dansi_esc_Invalid());
    }
    let final = *S_at((bytes)[bytes.len - 1]);
    if (final < 0x30 || final > 0x7e) {
        return_err(E_cause$dansi_esc_Invalid());
    }
    var_(idx, usize) = 1;
    while (idx + 1 < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte < 0x20 || byte > 0x2f) {
            return_err(E_cause$dansi_esc_Invalid());
        }
        idx += 1;
    }
    return_ok({
        .intermediates = S_prefix((S_suffix((bytes)1))(bytes.len - 2)),
        .final = final,
        .bytes = bytes,
    });
} $unscoped(fn);

fn_((dansi_esc_make(S_const$u8 intermediates, u8 final, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_esc_write(intermediates, final, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_esc_write(S_const$u8 intermediates, u8 final, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeByte(out, 0x1b));
    try_(io_Writer_writeBytes(out, intermediates));
    return io_Writer_writeByte(out, final);
} $unscoped(fn);
