#include "dansi-xterm/window.h"
#include "dansi-core/Seq.h"
#include <dh/io/Fixed.h>

fn_((dansi_xterm_window_op(dansi_xterm_window_Op op, dansi_xterm_window_OpBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_opWrite(op, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_opWrite(dansi_xterm_window_Op op, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make1_static("{:uhh}", dansi_xterm_window_report_final)), as$(u8)(op));
};

fn_((dansi_xterm_window_move(u16 x, u16 y, dansi_xterm_window_MoveBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_moveWrite(x, y, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_moveWrite(u16 x, u16 y, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uhh}" dansi_csi_param_sep "{:uh}" dansi_csi_param_sep "{:uh}",
            "",
            dansi_xterm_window_report_final
        )),
        dansi_xterm_window_Op_move, x, y
    );
};

fn_((dansi_xterm_window_resizePixels(u16 height, u16 width, dansi_xterm_window_ResizePixelsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_resizePixelsWrite(height, width, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_resizePixelsWrite(u16 height, u16 width, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uhh}" dansi_csi_param_sep "{:uh}" dansi_csi_param_sep "{:uh}",
            "",
            dansi_xterm_window_report_final
        )),
        dansi_xterm_window_Op_resize_pixels, height, width
    );
};

fn_((dansi_xterm_window_resizeCells(u16 height, u16 width, dansi_xterm_window_ResizeCellsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_resizeCellsWrite(height, width, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_resizeCellsWrite(u16 height, u16 width, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uhh}" dansi_csi_param_sep "{:uh}" dansi_csi_param_sep "{:uh}",
            "",
            dansi_xterm_window_report_final
        )),
        dansi_xterm_window_Op_resize_cells, height, width
    );
};

fn_((dansi_xterm_window_maximize(dansi_xterm_window_Maximize mode, dansi_xterm_window_MaximizeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_maximizeWrite(mode, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_maximizeWrite(dansi_xterm_window_Maximize mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uhh}" dansi_csi_param_sep "{:uhh}",
            "", dansi_xterm_window_report_final
        )),
        dansi_xterm_window_Op_maximize, as$(u8)(mode)
    );
};

fn_((dansi_xterm_window_fullscreen(dansi_xterm_window_Fullscreen mode, dansi_xterm_window_FullscreenBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_window_fullscreenWrite(mode, io_Fixed_writer(&writing)))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_window_fullscreenWrite(dansi_xterm_window_Fullscreen mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uhh}" dansi_csi_param_sep "{:uhh}",
            "", dansi_xterm_window_report_final
        )),
        dansi_xterm_window_Op_fullscreen, as$(u8)(mode)
    );
};

fn_((dansi_xterm_window_requestState(void))(S_const$u8)) {
    return u8_l(dansi_xterm_window_requestState_static());
};

fn_((dansi_xterm_window_requestStateWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_window_requestState());
};

fn_((dansi_xterm_window_receiveStateReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_window_parseStateReport(S_const$u8 report))(dansi_xterm_window_E$dansi_xterm_window_State) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (frame.final != dansi_xterm_window_report_final_byte) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let state = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_window_report_param_code))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (state != dansi_xterm_window_State_non_iconified && state != dansi_xterm_window_State_iconified) {
        return_err(E_cause$dansi_xterm_window_InvalidResponse());
    }
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

fn_((dansi_xterm_window_requestPosWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_window_requestPos());
};

fn_((dansi_xterm_window_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_window_parsePosReport(S_const$u8 report))(dansi_xterm_window_E$dansi_xterm_window_Pos) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (frame.final != dansi_xterm_window_report_final_byte) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_window_report_param_code))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    if (code != dansi_xterm_window_Op_move) return_err(E_cause$dansi_xterm_window_InvalidResponse());
    let x = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_window_report_param_x))(
        return_err(E_cause$dansi_xterm_window_InvalidResponse())
    ));
    let y = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_window_report_param_y))(
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
