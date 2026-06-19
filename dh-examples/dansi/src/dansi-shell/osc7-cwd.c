#include "dansi-shell/osc7-cwd.h"
#include <dh/io/Fixed.h>

fn_((dansi_shell_osc7_set(dansi_shell_osc7_Cwd cwd, S$u8 buf))(E$S$u8)) {
    return dansi_shell_osc7_setRaw(cwd.file_uri, buf);
};

fn_((dansi_shell_osc7_setWrite(dansi_shell_osc7_Cwd cwd, io_Writer out))(E$void)) {
    return dansi_shell_osc7_setRawWrite(cwd.file_uri, out);
};

fn_((dansi_shell_osc7_setRaw(S_const$u8 file_uri, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_shell_osc7_setRawWrite(file_uri, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_shell_osc7_setRawWrite(S_const$u8 file_uri, io_Writer out))(E$void)) {
    return dansi_osc_write(7, file_uri, out);
};

fn_((dansi_shell_osc7_parse(dansi_osc_Frame frame))(dansi_shell_osc7_E$dansi_shell_osc7_Cwd) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(return_err(E_cause$dansi_shell_osc7_Invalid())));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(return_err(E_cause$dansi_shell_osc7_Invalid())));
    if (cmd != 7) return_err(E_cause$dansi_shell_osc7_Invalid());
    return_ok({ .file_uri = split.payload });
} $unscoped(fn);
