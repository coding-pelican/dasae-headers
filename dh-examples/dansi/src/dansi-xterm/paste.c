#include "dansi-xterm/paste.h"

fn_((dansi_xterm_paste_setBracketed(bool enabled, dansi_xterm_paste_SetBracketedBuf* buf))(S$u8)) {
    return dansi_xterm_mode_set(dansi_xterm_mode_Code_bracketed_paste, enabled, buf);
};

fn_((dansi_xterm_paste_setBracketedWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_xterm_mode_setWrite(dansi_xterm_mode_Code_bracketed_paste, enabled, out);
};

fn_((dansi_xterm_paste_enableBracketed(dansi_xterm_paste_EnableBracketedBuf* buf))(S$u8)) {
    return dansi_xterm_mode_enable(dansi_xterm_mode_Code_bracketed_paste, buf);
};

fn_((dansi_xterm_paste_enableBracketedWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mode_enableWrite(dansi_xterm_mode_Code_bracketed_paste, out);
};

fn_((dansi_xterm_paste_disableBracketed(dansi_xterm_paste_DisableBracketedBuf* buf))(S$u8)) {
    return dansi_xterm_mode_disable(dansi_xterm_mode_Code_bracketed_paste, buf);
};

fn_((dansi_xterm_paste_disableBracketedWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mode_disableWrite(dansi_xterm_mode_Code_bracketed_paste, out);
};

fn_((dansi_xterm_paste_parseBoundary(S_const$u8 report))(O$dansi_xterm_paste_Event) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_none()));
    if (frame.final != u8_c('~')) return_none();
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 0))(return_none()));
    switch (code) {
    case_((200)) return_some(dansi_xterm_paste_Event_begin) $end(case);
    case_((201)) return_some(dansi_xterm_paste_Event_end) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);
