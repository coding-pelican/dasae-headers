#include "dansi-shell/osc133-finalterm.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_shell_osc133__payload(S_const$u8 payload, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_osc_write(dansi_shell_osc133_cmd_u16, payload, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc133_mark(dansi_shell_osc133_Mark mark, S$u8 buf))(E$S$u8) $scope) {
    var_(payload, A$$(1, u8)) $undefined;
    *S_at((A_ref$((S$u8)(payload)))[0]) = as$(u8)(mark);
    return dansi_shell_osc133__payload(A_ref$((S$u8)(payload)).as_const, buf);
} $unscoped(fn);

fn_((dansi_shell_osc133_markWrite(dansi_shell_osc133_Mark mark, io_Writer out))(io_PrintE$void) $scope) {
    var_(payload, A$$(1, u8)) $undefined;
    *S_at((A_ref$((S$u8)(payload)))[0]) = as$(u8)(mark);
    return dansi_osc_write(dansi_shell_osc133_cmd_u16, A_ref$((S$u8)(payload)).as_const, out);
} $unscoped(fn);

fn_((dansi_shell_osc133_commandEnd(O$i32 exit_code, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_shell_osc133_commandEndWrite(exit_code, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc133_commandEndWrite(O$i32 exit_code, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_shell_osc133_cmd dansi_osc_cmd_sep dansi_shell_osc133_command_end)));
    if_some((exit_code)(code)) {
        try_(io_Writer_writeByte(out, dansi_shell_osc133_sep_byte));
        try_(io_Writer_print(out, u8_l("{:d}"), code));
    }
    return_ok(try_(dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out)));
} $unscoped(fn);

fn_((dansi_shell_osc133_parse(dansi_osc_Frame frame))(dansi_shell_osc133_E$dansi_shell_osc133_Frame) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(return_err(E_cause$dansi_shell_osc133_Invalid())));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(return_err(E_cause$dansi_shell_osc133_Invalid())));
    if (cmd != dansi_shell_osc133_cmd_u16 || split.payload.len == 0) return_err(E_cause$dansi_shell_osc133_Invalid());
    let mark = *S_at((split.payload)[dansi_shell_osc133_payload_mark_index]);
    switch (mark) {
    case_((dansi_shell_osc133_Mark_prompt_start)) return_ok(union_of((dansi_shell_osc133_Frame_prompt_start){})) $end(case);
    case_((dansi_shell_osc133_Mark_prompt_end)) return_ok(union_of((dansi_shell_osc133_Frame_prompt_end){})) $end(case);
    case_((dansi_shell_osc133_Mark_command_start)) return_ok(union_of((dansi_shell_osc133_Frame_command_start){})) $end(case);
    case_((dansi_shell_osc133_command_end_byte)) {
        var_(exit_code, O$i32) = none$((O$i32));
        if (split.payload.len > dansi_shell_osc133_payload_arg_start
            && *S_at((split.payload)[dansi_shell_osc133_payload_sep_index]) == dansi_shell_osc133_sep_byte) {
            exit_code = some$((O$i32)(catch_((fmt_parse$i32(
                S_suffix((split.payload)(dansi_shell_osc133_payload_arg_start)),
                dansi_shell_osc133_exit_code_radix
            ))($ignore, return_err(E_cause$dansi_shell_osc133_Invalid())))));
        }
        return_ok(union_of((dansi_shell_osc133_Frame_command_end){ .exit_code = exit_code }));
    } $end(case);
    default_() return_err(E_cause$dansi_shell_osc133_Invalid()) $end(default);
    }
} $unscoped(fn);
