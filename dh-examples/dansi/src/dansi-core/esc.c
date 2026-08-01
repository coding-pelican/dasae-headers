#include "dansi-core/esc.h"
#include <dh/io/Fixed.h>

fn_((dansi_esc_parse(S_const$u8 bytes))(dansi_esc_E$dansi_esc_Frame) $scope) {
    if (bytes.len < 2 || *S_at((bytes)[0]) != dansi_Seq_esc_byte) {
        return_err(E_cause$dansi_esc_Invalid());
    }
    let final = *S_at((bytes)[bytes.len - 1]);
    if (final < dansi_esc_final_min_byte || final > dansi_esc_final_max_byte) {
        return_err(E_cause$dansi_esc_Invalid());
    }
    var_(idx, usize) = 1;
    while (idx + 1 < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte < dansi_esc_intermediate_min_byte || byte > dansi_esc_intermediate_max_byte) {
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
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_esc_write(intermediates, final, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_esc_write(S_const$u8 intermediates, u8 final, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeByte(out, dansi_Seq_esc_byte));
    try_(io_Writer_writeBytes(out, intermediates));
    return_ok(try_(io_Writer_writeByte(out, final)));
} $unscoped(fn);
