#include "dansi-core/dcs.h"
#include "dansi-core/csi.h"
#include <dh/io/Fixed.h>

fn_((dansi_dcs_parse(S_const$u8 bytes))(dansi_dcs_E$dansi_dcs_Frame) $scope) {
    var_(prefix_len, usize) = 0;
    if (bytes.len >= 3 && *S_at((bytes)[0]) == dansi_Seq_esc_byte && *S_at((bytes)[1]) == dansi_dcs_7bit_intro_byte) {
        prefix_len = 2;
    } else if (bytes.len >= 2 && *S_at((bytes)[0]) == dansi_dcs_8bit_intro_byte) {
        prefix_len = 1;
    } else {
        return_err(E_cause$dansi_dcs_Invalid());
    }
    let term = orelse_((dansi_Seq_CtrlTer_find(bytes, prefix_len))(return_err(E_cause$dansi_dcs_Invalid())));
    let body = S_prefix((S_suffix((bytes)(prefix_len)))(term.payload_end - prefix_len));
    var_(final_idx, usize) = body.len;
    var_(idx, usize) = 0;
    while (idx < body.len) {
        let byte = *S_at((body)[idx]);
        if (dansi_csi_final_min_byte <= byte && byte <= dansi_csi_final_max_byte) {
            final_idx = idx;
            break;
        }
        if (byte < dansi_csi_intermediate_min_byte || byte > dansi_csi_private_marker_max_byte) {
            return_err(E_cause$dansi_dcs_Invalid());
        }
        idx += 1;
    }

    if (final_idx == body.len) {
        return_ok({
            .params = u8_l(""),
            .intermediates = u8_l(""),
            .final = 0,
            .payload = body,
            .terminator = term.eos,
            .bytes = bytes,
        });
    }

    let header = S_prefix((body)(final_idx));
    var_(intermediates_at, usize) = header.len;
    idx = 0;
    while (idx < header.len) {
        let byte = *S_at((header)[idx]);
        if (dansi_csi_intermediate_min_byte <= byte && byte <= dansi_csi_intermediate_max_byte) {
            intermediates_at = idx;
            break;
        }
        idx += 1;
    }
    return_ok({
        .params = S_prefix((header)(intermediates_at)),
        .intermediates = S_suffix((header)(intermediates_at)),
        .final = *S_at((body)[final_idx]),
        .payload = S_suffix((body)(final_idx + 1)),
        .terminator = term.eos,
        .bytes = bytes,
    });
} $unscoped(fn);

fn_((dansi_dcs_makeRaw(S_const$u8 payload, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_makeRawWithEOS(payload, dansi_Seq_EOS_st_7bit, buf);
};

fn_((dansi_dcs_writeRaw(S_const$u8 payload, io_Writer out))(E$void)) {
    return dansi_dcs_writeRawWithEOS(payload, dansi_Seq_EOS_st_7bit, out);
};

fn_((dansi_dcs_makeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_dcs_writeRawWithEOS(payload, eos, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_dcs_writeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_dcs_7bit_prefix)));
    try_(io_Writer_writeBytes(out, payload));
    return dansi_Seq_EOS_write(eos, out);
} $unscoped(fn);

fn_((dansi_dcs_make(S_const$u8 params, S_const$u8 intermediates, u8 final, S_const$u8 payload, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_makeWithEOS(params, intermediates, final, payload, dansi_Seq_EOS_st_7bit, buf);
};

fn_((dansi_dcs_write(S_const$u8 params, S_const$u8 intermediates, u8 final, S_const$u8 payload, io_Writer out))(E$void)) {
    return dansi_dcs_writeWithEOS(params, intermediates, final, payload, dansi_Seq_EOS_st_7bit, out);
};

fn_((dansi_dcs_makeWithEOS(S_const$u8 params, S_const$u8 intermediates, u8 final, S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_dcs_writeWithEOS(params, intermediates, final, payload, eos, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_dcs_writeWithEOS(S_const$u8 params, S_const$u8 intermediates, u8 final, S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_dcs_7bit_prefix)));
    try_(io_Writer_writeBytes(out, params));
    try_(io_Writer_writeBytes(out, intermediates));
    try_(io_Writer_writeByte(out, final));
    try_(io_Writer_writeBytes(out, payload));
    return dansi_Seq_EOS_write(eos, out);
} $unscoped(fn);
