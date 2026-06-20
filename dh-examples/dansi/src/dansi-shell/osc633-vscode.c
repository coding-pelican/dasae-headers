#include "dansi-shell/osc633-vscode.h"
#include "dansi-core/ctrl.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_shell_osc633__writeEscapedByte(u8 byte, io_Writer out))(E$void) $scope) {
    $static let hex = u8_l(dansi_shell_osc633_escape_hex_digits);
    try_(io_Writer_writeBytes(out, u8_l(dansi_shell_osc633_escape_hex_prefix)));
    try_(io_Writer_writeByte(out, *S_at((hex)[(byte >> dansi_shell_osc633_escape_hex_high_shift) & dansi_shell_osc633_escape_hex_nibble_mask])));
    return io_Writer_writeByte(out, *S_at((hex)[byte & dansi_shell_osc633_escape_hex_nibble_mask]));
} $unscoped(fn);

$static fn_((dansi_shell_osc633__writeEscaped(S_const$u8 bytes, io_Writer out))(E$void) $scope) {
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte <= dansi_Seq_c0_max_byte || byte == dansi_ctrl_Code_del || byte == dansi_shell_osc633_sep_byte || byte == dansi_shell_osc633_escape_byte || byte == dansi_Seq_st_8bit_byte) {
            try_(dansi_shell_osc633__writeEscapedByte(byte, out));
        } else {
            try_(io_Writer_writeByte(out, byte));
        }
        idx += 1;
    }
    return_ok({});
} $unscoped(fn);

fn_((dansi_shell_osc633_mark(dansi_shell_osc633_Mark mark, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_markWrite(mark, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_markWrite(dansi_shell_osc633_Mark mark, io_Writer out))(E$void) $scope) {
    var_(payload, A$$(1, u8)) $undefined;
    *S_at((A_ref$((S$u8)(payload)))[0]) = as$(u8)(mark);
    return dansi_osc_write(dansi_shell_osc633_cmd_u16, A_ref$((S$u8)(payload)).as_const, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_commandEnd(O$i32 exit_code, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_commandEndWrite(exit_code, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_commandEndWrite(O$i32 exit_code, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc633_cmd dansi_osc_cmd_sep dansi_shell_osc633_command_end)));
    if_some((exit_code)(code)) {
        try_(io_Writer_writeByte(out, dansi_shell_osc633_sep_byte));
        try_(io_Writer_print(out, u8_l("{:d}"), code));
    }
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLine(S_const$u8 command_line, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_commandLineWrite(command_line, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLineWrite(S_const$u8 command_line, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc633_cmd dansi_osc_cmd_sep dansi_shell_osc633_command_line dansi_osc_cmd_sep)));
    try_(dansi_shell_osc633__writeEscaped(command_line, out));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLineRaw(S_const$u8 command_line, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_commandLineRawWrite(command_line, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLineRawWrite(S_const$u8 command_line, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc633_cmd dansi_osc_cmd_sep dansi_shell_osc633_command_line dansi_osc_cmd_sep)));
    try_(io_Writer_writeBytes(out, command_line));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_property(dansi_shell_osc633_Prop property, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_propertyWrite(property, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyWrite(dansi_shell_osc633_Prop property, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc633_cmd dansi_osc_cmd_sep dansi_shell_osc633_property_op dansi_osc_cmd_sep)));
    try_(dansi_shell_osc633__writeEscaped(property.key, out));
    try_(io_Writer_writeByte(out, dansi_shell_osc633_property_sep_byte));
    try_(dansi_shell_osc633__writeEscaped(property.value, out));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyRaw(S_const$u8 property, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_propertyRawWrite(property, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyRawWrite(S_const$u8 property, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc633_cmd dansi_osc_cmd_sep dansi_shell_osc633_property_op dansi_osc_cmd_sep)));
    try_(io_Writer_writeBytes(out, property));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_parse(dansi_osc_Frame frame))(dansi_shell_osc633_E$dansi_shell_osc633_Frame) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(return_err(E_cause$dansi_shell_osc633_Invalid())));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(return_err(E_cause$dansi_shell_osc633_Invalid())));
    if (cmd != dansi_shell_osc633_cmd_u16 || split.payload.len == 0) return_err(E_cause$dansi_shell_osc633_Invalid());
    let mark = *S_at((split.payload)[dansi_shell_osc633_payload_mark_index]);
    switch (mark) {
    case_((dansi_shell_osc633_Mark_prompt_start)) return_ok(union_of((dansi_shell_osc633_Frame_prompt_start){})) $end(case);
    case_((dansi_shell_osc633_Mark_prompt_end)) return_ok(union_of((dansi_shell_osc633_Frame_prompt_end){})) $end(case);
    case_((dansi_shell_osc633_Mark_command_start)) return_ok(union_of((dansi_shell_osc633_Frame_command_start){})) $end(case);
    case_((dansi_shell_osc633_command_end_byte)) {
        var_(exit_code, O$i32) = none$((O$i32));
        if (split.payload.len > dansi_shell_osc633_payload_arg_start && *S_at((split.payload)[dansi_shell_osc633_payload_sep_index]) == dansi_shell_osc633_sep_byte) {
            exit_code = some$((O$i32)(catch_((fmt_parse$i32(S_suffix((split.payload)(dansi_shell_osc633_payload_arg_start)), dansi_shell_osc633_exit_code_radix))($ignore, return_err(E_cause$dansi_shell_osc633_Invalid())))));
        }
        return_ok(union_of((dansi_shell_osc633_Frame_command_end){ .exit_code = exit_code }));
    } $end(case);
    case_((dansi_shell_osc633_command_line_byte)) {
        if (split.payload.len < dansi_shell_osc633_payload_arg_start || *S_at((split.payload)[dansi_shell_osc633_payload_sep_index]) != dansi_shell_osc633_sep_byte) return_err(E_cause$dansi_shell_osc633_Invalid());
        return_ok(union_of((dansi_shell_osc633_Frame_command_line){
            .command_line = S_suffix((split.payload)(dansi_shell_osc633_payload_arg_start)),
        }));
    } $end(case);
    case_((dansi_shell_osc633_property_op_byte)) {
        if (split.payload.len < dansi_shell_osc633_payload_arg_start || *S_at((split.payload)[dansi_shell_osc633_payload_sep_index]) != dansi_shell_osc633_sep_byte) return_err(E_cause$dansi_shell_osc633_Invalid());
        let property = S_suffix((split.payload)(dansi_shell_osc633_payload_arg_start));
        let delim = orelse_((mem_findFirstUnitBytes(property, dansi_shell_osc633_property_sep_byte))(return_err(E_cause$dansi_shell_osc633_Invalid())));
        return_ok(union_of((dansi_shell_osc633_Frame_property){
            .key = S_prefix((property)(delim)),
            .value = S_suffix((property)(delim + 1)),
        }));
    } $end(case);
    default_() return_err(E_cause$dansi_shell_osc633_Invalid()) $end(default);
    }
} $unscoped(fn);
