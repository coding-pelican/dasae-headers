#include "dansi-shell/osc633-vscode.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_shell_osc633__writeEscapedByte(u8 byte, io_Writer out))(E$void) $scope) {
    $static let hex = u8_l("0123456789ABCDEF");
    try_(io_Writer_writeBytes(out, u8_l("\\x")));
    try_(io_Writer_writeByte(out, *S_at((hex)[(byte >> 4) & 0x0f])));
    return io_Writer_writeByte(out, *S_at((hex)[byte & 0x0f]));
} $unscoped(fn);

$static fn_((dansi_shell_osc633__writeEscaped(S_const$u8 bytes, io_Writer out))(E$void) $scope) {
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte < 0x20 || byte == 0x7f || byte == u8_c(';') || byte == u8_c('\\') || byte == 0x9c) {
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
    return dansi_osc_write(633, A_ref$((S$u8)(payload)).as_const, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_commandEnd(O$i32 exit_code, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_commandEndWrite(exit_code, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_commandEndWrite(O$i32 exit_code, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l("\x1b]633;D")));
    if_some((exit_code)(code)) {
        try_(io_Writer_writeByte(out, u8_c(';')));
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
    try_(io_Writer_writeBytes(out, u8_l("\x1b]633;E;")));
    try_(dansi_shell_osc633__writeEscaped(command_line, out));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLineRaw(S_const$u8 command_line, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_commandLineRawWrite(command_line, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_commandLineRawWrite(S_const$u8 command_line, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l("\x1b]633;E;")));
    try_(io_Writer_writeBytes(out, command_line));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_property(dansi_shell_osc633_Prop property, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_propertyWrite(property, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyWrite(dansi_shell_osc633_Prop property, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l("\x1b]633;P;")));
    try_(dansi_shell_osc633__writeEscaped(property.key, out));
    try_(io_Writer_writeByte(out, u8_c('=')));
    try_(dansi_shell_osc633__writeEscaped(property.value, out));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyRaw(S_const$u8 property, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc633_propertyRawWrite(property, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc633_propertyRawWrite(S_const$u8 property, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l("\x1b]633;P;")));
    try_(io_Writer_writeBytes(out, property));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_shell_osc633_parse(dansi_osc_Frame frame))(dansi_shell_osc633_E$dansi_shell_osc633_Frame) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(return_err(E_cause$dansi_shell_osc633_Invalid())));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(return_err(E_cause$dansi_shell_osc633_Invalid())));
    if (cmd != 633 || split.payload.len == 0) return_err(E_cause$dansi_shell_osc633_Invalid());
    let mark = *S_at((split.payload)[0]);
    switch (mark) {
    case_(u8_c('A')) return_ok(union_of((dansi_shell_osc633_Frame_prompt_start){})) $end(case);
    case_(u8_c('B')) return_ok(union_of((dansi_shell_osc633_Frame_prompt_end){})) $end(case);
    case_(u8_c('C')) return_ok(union_of((dansi_shell_osc633_Frame_command_start){})) $end(case);
    case_(u8_c('D')) {
        var_(exit_code, O$i32) = none$((O$i32));
        if (split.payload.len > 2 && *S_at((split.payload)[1]) == u8_c(';')) {
            exit_code = some$((O$i32)(catch_((fmt_parse$i32(S_suffix((split.payload)(2)), 10))($ignore, return_err(E_cause$dansi_shell_osc633_Invalid())))));
        }
        return_ok(union_of((dansi_shell_osc633_Frame_command_end){ .exit_code = exit_code }));
    } $end(case);
    case_(u8_c('E')) {
        if (split.payload.len < 2 || *S_at((split.payload)[1]) != u8_c(';')) return_err(E_cause$dansi_shell_osc633_Invalid());
        return_ok(union_of((dansi_shell_osc633_Frame_command_line){
            .command_line = S_suffix((split.payload)(2)),
        }));
    } $end(case);
    case_(u8_c('P')) {
        if (split.payload.len < 2 || *S_at((split.payload)[1]) != u8_c(';')) return_err(E_cause$dansi_shell_osc633_Invalid());
        let property = S_suffix((split.payload)(2));
        let delim = orelse_((mem_findFirstUnitBytes(property, u8_c('=')))(return_err(E_cause$dansi_shell_osc633_Invalid())));
        return_ok(union_of((dansi_shell_osc633_Frame_property){
            .key = S_prefix((property)(delim)),
            .value = S_suffix((property)(delim + 1)),
        }));
    } $end(case);
    default_() return_err(E_cause$dansi_shell_osc633_Invalid()) $end(default);
    }
} $unscoped(fn);
