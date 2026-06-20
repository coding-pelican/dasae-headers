#include "dansi-xterm/focus.h"

fn_((dansi_xterm_focus_setTracking(bool enabled, dansi_xterm_focus_SetTrackingBuf* buf))(S$u8)) {
    return dansi_xterm_mode_set(dansi_xterm_mode_Code_focus_event, enabled, buf);
};

fn_((dansi_xterm_focus_setTrackingWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_xterm_mode_setWrite(dansi_xterm_mode_Code_focus_event, enabled, out);
};

fn_((dansi_xterm_focus_enableTracking(dansi_xterm_focus_EnableTrackingBuf* buf))(S$u8)) {
    return dansi_xterm_mode_enable(dansi_xterm_mode_Code_focus_event, buf);
};

fn_((dansi_xterm_focus_enableTrackingWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mode_enableWrite(dansi_xterm_mode_Code_focus_event, out);
};

fn_((dansi_xterm_focus_disableTracking(dansi_xterm_focus_DisableTrackingBuf* buf))(S$u8)) {
    return dansi_xterm_mode_disable(dansi_xterm_mode_Code_focus_event, buf);
};

fn_((dansi_xterm_focus_disableTrackingWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mode_disableWrite(dansi_xterm_mode_Code_focus_event, out);
};

fn_((dansi_xterm_focus_parseReport(S_const$u8 report))(O$dansi_xterm_focus_Event) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_none()));
    if (frame.params.len != 0 || frame.intermediates.len != 0) return_none();
    switch (frame.final) {
    case_((u8_c('I'))) return_some(dansi_xterm_focus_Event_in) $end(case);
    case_((u8_c('O'))) return_some(dansi_xterm_focus_Event_out) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);
