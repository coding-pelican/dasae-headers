#include "dansi-xterm/mouse.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_mouse__btnFromBase(u16 base))(O$dansi_xterm_mouse_Btn) $scope) {
    switch (base) {
    case_((0)) return_some(dansi_xterm_mouse_Btn_left) $end(case);
    case_((1)) return_some(dansi_xterm_mouse_Btn_middle) $end(case);
    case_((2)) return_some(dansi_xterm_mouse_Btn_right) $end(case);
    case_((8)) return_some(dansi_xterm_mouse_Btn_backward) $end(case);
    case_((9)) return_some(dansi_xterm_mouse_Btn_forward) $end(case);
    case_((10)) return_some(dansi_xterm_mouse_Btn_aux1) $end(case);
    case_((11)) return_some(dansi_xterm_mouse_Btn_aux2) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_xterm_mouse__wheelFromBase(u16 base))(O$dansi_xterm_mouse_Wheel) $scope) {
    switch (base) {
    case_((64)) return_some(dansi_xterm_mouse_Wheel_up) $end(case);
    case_((65)) return_some(dansi_xterm_mouse_Wheel_down) $end(case);
    case_((66)) return_some(dansi_xterm_mouse_Wheel_left) $end(case);
    case_((67)) return_some(dansi_xterm_mouse_Wheel_right) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

fn_((dansi_xterm_mouse_setReportMode(
    dansi_xterm_mouse_ReportMode mode, bool enabled, dansi_xterm_mouse_SetReportModeBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_setRaw(as$(u16)(mode), enabled, buf);
};

fn_((dansi_xterm_mouse_setReportModeWrite(
    dansi_xterm_mouse_ReportMode mode, bool enabled, io_Writer out
))(E$void)) {
    return dansi_xterm_mode_setRawWrite(as$(u16)(mode), enabled, out);
};

fn_((dansi_xterm_mouse_enableReportMode(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_EnableReportModeBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_enableRaw(as$(u16)(mode), buf);
};

fn_((dansi_xterm_mouse_enableReportModeWrite(dansi_xterm_mouse_ReportMode mode, io_Writer out))(E$void)) {
    return dansi_xterm_mode_enableRawWrite(as$(u16)(mode), out);
};

fn_((dansi_xterm_mouse_disableReportMode(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_DisableReportModeBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_disableRaw(as$(u16)(mode), buf);
};

fn_((dansi_xterm_mouse_disableReportModeWrite(dansi_xterm_mouse_ReportMode mode, io_Writer out))(E$void)) {
    return dansi_xterm_mode_disableRawWrite(as$(u16)(mode), out);
};

fn_((dansi_xterm_mouse_setEncoding(
    dansi_xterm_mouse_Encoding encoding, bool enabled, dansi_xterm_mouse_SetEncodingBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_setRaw(as$(u16)(encoding), enabled, buf);
};

fn_((dansi_xterm_mouse_setEncodingWrite(dansi_xterm_mouse_Encoding encoding, bool enabled, io_Writer out))(E$void)) {
    return dansi_xterm_mode_setRawWrite(as$(u16)(encoding), enabled, out);
};

fn_((dansi_xterm_mouse_enableEncoding(
    dansi_xterm_mouse_Encoding encoding, dansi_xterm_mouse_EnableEncodingBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_enableRaw(as$(u16)(encoding), buf);
};

fn_((dansi_xterm_mouse_enableEncodingWrite(dansi_xterm_mouse_Encoding encoding, io_Writer out))(E$void)) {
    return dansi_xterm_mode_enableRawWrite(as$(u16)(encoding), out);
};

fn_((dansi_xterm_mouse_disableEncoding(
    dansi_xterm_mouse_Encoding encoding, dansi_xterm_mouse_DisableEncodingBuf* buf
))(S$u8)) {
    return dansi_xterm_mode_disableRaw(as$(u16)(encoding), buf);
};

fn_((dansi_xterm_mouse_disableEncodingWrite(dansi_xterm_mouse_Encoding encoding, io_Writer out))(E$void)) {
    return dansi_xterm_mode_disableRawWrite(as$(u16)(encoding), out);
};

fn_((dansi_xterm_mouse_enableAny(dansi_xterm_mouse_EnableAnyBuf* buf))(S$u8)) {
    return dansi_xterm_mouse_enableReportMode(dansi_xterm_mouse_ReportMode_any_event, buf);
};

fn_((dansi_xterm_mouse_enableAnyWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mouse_enableReportModeWrite(dansi_xterm_mouse_ReportMode_any_event, out);
};

fn_((dansi_xterm_mouse_disableAny(dansi_xterm_mouse_DisableAnyBuf* buf))(S$u8)) {
    return dansi_xterm_mouse_disableReportMode(dansi_xterm_mouse_ReportMode_any_event, buf);
};

fn_((dansi_xterm_mouse_disableAnyWrite(io_Writer out))(E$void)) {
    return dansi_xterm_mouse_disableReportModeWrite(dansi_xterm_mouse_ReportMode_any_event, out);
};

fn_((dansi_xterm_mouse_enableSGR(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_EnableSGRBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mouse_enableSGRWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mouse_enableSGRWrite(dansi_xterm_mouse_ReportMode mode, io_Writer out))(E$void) $scope) {
    try_(dansi_xterm_mouse_enableReportModeWrite(mode, out));
    return dansi_xterm_mouse_enableEncodingWrite(dansi_xterm_mouse_Encoding_sgr, out);
} $unscoped(fn);

fn_((dansi_xterm_mouse_disableSGR(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_DisableSGRBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mouse_disableSGRWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mouse_disableSGRWrite(dansi_xterm_mouse_ReportMode mode, io_Writer out))(E$void) $scope) {
    try_(dansi_xterm_mouse_disableEncodingWrite(dansi_xterm_mouse_Encoding_sgr, out));
    return dansi_xterm_mouse_disableReportModeWrite(mode, out);
} $unscoped(fn);

fn_((dansi_xterm_mouse_parseSGRReport(S_const$u8 report))(O$dansi_xterm_mouse_SGRReport) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_none()));
    if (!dansi_csi_Frame_isPrivate(frame, u8_c('<'))) return_none();
    if (frame.final != u8_c('M') && frame.final != u8_c('m')) return_none();
    let cb = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 0))(return_none()));
    let x = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 1))(return_none()));
    let y = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 2))(return_none()));
    return_some({ .cb = cb, .x = x, .y = y, .final = frame.final });
} $unscoped(fn);

fn_((dansi_xterm_mouse_interpretSGR(
    dansi_xterm_mouse_SGRReport report
))(dansi_xterm_mouse_E$dansi_xterm_mouse_Event) $scope) {
    let mods = dansi_xterm_mouse_modsFromCb(report.cb);
    let pos = (dansi_xterm_mouse_Pos){ .x = report.x, .y = report.y };
    let_(base, u16) = report.cb & ~(u16)(4 | 8 | 16 | 32);
    let is_motion = (report.cb & 32) != 0;

    if (report.final == u8_c('m')) {
        return_ok(union_of((dansi_xterm_mouse_Event_release){
            .pos = pos,
            .mods = mods,
        }));
    }

    if_some((dansi_xterm_mouse__wheelFromBase(base))(wheel)) {
        return_ok(union_of((dansi_xterm_mouse_Event_wheel){
            .wheel = wheel,
            .pos = pos,
            .mods = mods,
        }));
    }

    if (is_motion) {
        let btn = dansi_xterm_mouse__btnFromBase(base);
        return_ok(union_of((dansi_xterm_mouse_Event_motion){
            .btn = btn,
            .pos = pos,
            .mods = mods,
        }));
    }

    let btn = orelse_((dansi_xterm_mouse__btnFromBase(base))(
        return_err(E_cause$dansi_xterm_mouse_InvalidReport())
    ));
    return_ok(union_of((dansi_xterm_mouse_Event_press){
        .btn = btn,
        .pos = pos,
        .mods = mods,
    }));
} $unscoped(fn);

fn_((dansi_xterm_mouse_parseSGR(S_const$u8 report))(O$dansi_xterm_mouse_Event) $scope) {
    let sgr = orelse_((dansi_xterm_mouse_parseSGRReport(report))(return_none()));
    return catch_none$((O$dansi_xterm_mouse_Event)(dansi_xterm_mouse_interpretSGR(sgr)));
} $unscoped(fn);
