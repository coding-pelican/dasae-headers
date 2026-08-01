#include "dansi-core/osc.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_osc__parseU16(S_const$u8 text))(O$u16)) {
    if (text.len == 0) return none$((O$u16));
    return catch_none$((O$u16)(fmt_parse$u16(text, dansi_osc_cmd_radix)));
};

fn_((dansi_osc_parse(S_const$u8 bytes))(dansi_osc_E$dansi_osc_Frame) $scope) {
    var_(prefix_len, usize) = 0;
    if (bytes.len >= 3 && *S_at((bytes)[0]) == dansi_Seq_esc_byte && *S_at((bytes)[1]) == dansi_osc_7bit_intro_byte) {
        prefix_len = 2;
    } else if (bytes.len >= 2 && *S_at((bytes)[0]) == dansi_osc_8bit_intro_byte) {
        prefix_len = 1;
    } else {
        return_err(E_cause$dansi_osc_Invalid());
    }
    let term = orelse_((dansi_Seq_CtrlTer_find(bytes, prefix_len))(return_err(E_cause$dansi_osc_Invalid())));
    return_ok({
        .payload = S_prefix((S_suffix((bytes)(prefix_len)))(term.payload_end - prefix_len)),
        .terminator = term.eos,
        .bytes = bytes,
    });
} $unscoped(fn);

fn_((dansi_osc_makeRaw(S_const$u8 payload, S$u8 buf))(E$S$u8)) {
    return dansi_osc_makeRawWithEOS(payload, dansi_Seq_EOS_st_7bit, buf);
};

fn_((dansi_osc_writeRaw(S_const$u8 payload, io_Writer out))(io_PrintE$void)) {
    return dansi_osc_writeRawWithEOS(payload, dansi_Seq_EOS_st_7bit, out);
};

fn_((dansi_osc_makeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_osc_writeRawWithEOS(payload, eos, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_osc_writeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix)));
    try_(io_Writer_writeBytes(out, payload));
    return_ok(try_(dansi_Seq_EOS_write(eos, out)));
} $unscoped(fn);

fn_((dansi_osc_make(u16 cmd, S_const$u8 payload, S$u8 buf))(E$S$u8)) {
    return dansi_osc_makeWithEOS(cmd, payload, dansi_Seq_EOS_st_7bit, buf);
};

fn_((dansi_osc_write(u16 cmd, S_const$u8 payload, io_Writer out))(io_PrintE$void)) {
    return dansi_osc_writeWithEOS(cmd, payload, dansi_Seq_EOS_st_7bit, out);
};

fn_((dansi_osc_makeWithEOS(u16 cmd, S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_osc_writeWithEOS(cmd, payload, eos, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_osc_writeWithEOS(u16 cmd, S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix)));
    try_(io_Writer_print(out, u8_l("{:uh}" dansi_osc_cmd_sep), cmd));
    try_(io_Writer_writeBytes(out, payload));
    return_ok(try_(dansi_Seq_EOS_write(eos, out)));
} $unscoped(fn);

fn_((dansi_osc_Frame_splitCmd(dansi_osc_Frame self))(O$dansi_osc_CmdSplit) $scope) {
    if_some((mem_findFirstUnitBytes(self.payload, dansi_osc_cmd_sep_byte))(delim)) {
        return_some({
            .cmd = S_prefix((self.payload)(delim)),
            .payload = S_suffix((self.payload)(delim + 1)),
        });
    }
    return_none();
} $unscoped(fn);

fn_((dansi_osc_CmdSplit_cmdAsU16(dansi_osc_CmdSplit self))(O$u16)) {
    return dansi_osc__parseU16(self.cmd);
};
