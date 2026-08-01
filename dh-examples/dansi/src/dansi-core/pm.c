#include "dansi-core/pm.h"
#include <dh/io/Fixed.h>

fn_((dansi_pm_parse(S_const$u8 bytes))(dansi_pm_E$dansi_pm_Frame) $scope) {
    var_(prefix_len, usize) = 0;
    if (bytes.len >= 3 && *S_at((bytes)[0]) == dansi_Seq_esc_byte && *S_at((bytes)[1]) == dansi_pm_7bit_intro_byte) {
        prefix_len = 2;
    } else if (bytes.len >= 2 && *S_at((bytes)[0]) == dansi_pm_8bit_intro_byte) {
        prefix_len = 1;
    } else {
        return_err(E_cause$dansi_pm_Invalid());
    }
    let term = orelse_((dansi_Seq_CtrlTer_find(bytes, prefix_len))(return_err(E_cause$dansi_pm_Invalid())));
    return_ok({
        .payload = S_prefix((S_suffix((bytes)(prefix_len)))(term.payload_end - prefix_len)),
        .terminator = term.eos,
        .bytes = bytes,
    });
} $unscoped(fn);

fn_((dansi_pm_make(S_const$u8 payload, S$u8 buf))(E$S$u8)) {
    return dansi_pm_makeWithEOS(payload, dansi_Seq_EOS_st_7bit, buf);
};

fn_((dansi_pm_write(S_const$u8 payload, io_Writer out))(io_PrintE$void)) {
    return dansi_pm_writeWithEOS(payload, dansi_Seq_EOS_st_7bit, out);
};

fn_((dansi_pm_makeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_pm_writeWithEOS(payload, eos, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_pm_writeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_pm_7bit_prefix)));
    try_(io_Writer_writeBytes(out, payload));
    return_ok(try_(dansi_Seq_EOS_write(eos, out)));
} $unscoped(fn);
