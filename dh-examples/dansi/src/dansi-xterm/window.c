#include "dansi-xterm/window.h"
#include "dansi-core/Seq.h"
#include <dh/io/Fixed.h>

fn_((dansi_xterm_window_op(dansi_xterm_window_Op op, dansi_xterm_window_OpBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_opWrite(op, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_opWrite(dansi_xterm_window_Op op, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make1_static("{:uhh}", "t")), as$(u8)(op));
};

fn_((dansi_xterm_window_move(u16 x, u16 y, dansi_xterm_window_MoveBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_moveWrite(x, y, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_moveWrite(u16 x, u16 y, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("3;{:uh};{:uh}", "", "t")), x, y);
};

fn_((dansi_xterm_window_resizePixels(u16 height, u16 width, dansi_xterm_window_ResizePixelsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_resizePixelsWrite(height, width, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_resizePixelsWrite(u16 height, u16 width, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("4;{:uh};{:uh}", "", "t")), height, width);
};

fn_((dansi_xterm_window_resizeCells(u16 height, u16 width, dansi_xterm_window_ResizeCellsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_resizeCellsWrite(height, width, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_resizeCellsWrite(u16 height, u16 width, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("8;{:uh};{:uh}", "", "t")), height, width);
};

fn_((dansi_xterm_window_maximize(dansi_xterm_window_Maximize mode, dansi_xterm_window_MaximizeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_maximizeWrite(mode, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_maximizeWrite(dansi_xterm_window_Maximize mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("9;{:uhh}", "", "t")), as$(u8)(mode));
};

fn_((dansi_xterm_window_fullscreen(dansi_xterm_window_Fullscreen mode, dansi_xterm_window_FullscreenBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_fullscreenWrite(mode, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_fullscreenWrite(dansi_xterm_window_Fullscreen mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("10;{:uhh}", "", "t")), as$(u8)(mode));
};

fn_((dansi_xterm_window_requestState(void))(S_const$u8)) {
    return u8_l(dansi_xterm_window_requestState_static());
};

fn_((dansi_xterm_window_requestStateWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_window_requestState());
};

fn_((dansi_xterm_window_receiveStateReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_window_parseStateReport(S_const$u8 report))(dansi_xterm_window_E$dansi_xterm_window_State) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (frame.final != u8_c('t')) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let state = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 0))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (state != 1 && state != 2) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    return_ok(as$(dansi_xterm_window_State)(state));
} $unscoped(fn);

fn_((dansi_xterm_window_fetchState(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_window_E$dansi_xterm_window_State) $scope) {
    try_(dansi_xterm_window_requestStateWrite(out));
    let report = try_(dansi_xterm_window_receiveStateReport(in, buf));
    return dansi_xterm_window_parseStateReport(report.as_const);
} $unscoped(fn);

fn_((dansi_xterm_window_requestPos(void))(S_const$u8)) {
    return u8_l(dansi_xterm_window_requestPos_static());
};

fn_((dansi_xterm_window_requestPosWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_window_requestPos());
};

fn_((dansi_xterm_window_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_window_parsePosReport(S_const$u8 report))(dansi_xterm_window_E$dansi_xterm_window_Pos) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (frame.final != u8_c('t')) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 0))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (code != 3) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let x = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 1))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    let y = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 2))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    return_ok({ .x = x, .y = y });
} $unscoped(fn);

fn_((dansi_xterm_window_fetchPos(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_window_E$dansi_xterm_window_Pos) $scope) {
    try_(dansi_xterm_window_requestPosWrite(out));
    let report = try_(dansi_xterm_window_receivePosReport(in, buf));
    return dansi_xterm_window_parsePosReport(report.as_const);
} $unscoped(fn);
