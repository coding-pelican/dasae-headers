#include "dansi-core/cursor.h"
#include "dansi-core/Seq.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_cursor_moveTo(u16 row, u16 col, dansi_cursor_MoveToBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToWrite(row, col, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToWrite(u16 row, u16 col, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveTo_static("{:uh}", "{:uh}")), row, col);
};

fn_((dansi_cursor_moveUp(u16 rows, dansi_cursor_MoveUpBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveUpWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveUpWrite(u16 rows, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveUp_static("{:uh}")), rows);
};

fn_((dansi_cursor_moveDown(u16 rows, dansi_cursor_MoveDownBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveDownWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveDownWrite(u16 rows, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveDown_static("{:uh}")), rows);
};

fn_((dansi_cursor_moveRight(u16 cols, dansi_cursor_MoveRightBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveRightWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveRightWrite(u16 cols, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveRight_static("{:uh}")), cols);
};

fn_((dansi_cursor_moveLeft(u16 cols, dansi_cursor_MoveLeftBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveLeftWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveLeftWrite(u16 cols, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveLeft_static("{:uh}")), cols);
};

fn_((dansi_cursor_moveToRow(u16 row, dansi_cursor_MoveToRowBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToRowWrite(row, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToRowWrite(u16 row, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveToRow_static("{:uh}")), row);
};

fn_((dansi_cursor_moveToCol(u16 col, dansi_cursor_MoveToColBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToColWrite(col, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToColWrite(u16 col, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveToCol_static("{:uh}")), col);
};

fn_((dansi_cursor_moveNextLine(u16 rows, dansi_cursor_MoveNextLineBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveNextLineWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveNextLineWrite(u16 rows, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_moveNextLine_static("{:uh}")), rows);
};

fn_((dansi_cursor_movePrevLine(u16 rows, dansi_cursor_MovePrevLineBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_movePrevLineWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_movePrevLineWrite(u16 rows, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_cursor_movePrevLine_static("{:uh}")), rows);
};

fn_((dansi_cursor_setTabStop(void))(S_const$u8)) {
    return u8_l(dansi_cursor_setTabStop_static());
};

fn_((dansi_cursor_setTabStopWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_cursor_setTabStop());
};

fn_((dansi_cursor_clearTabStop(void))(S_const$u8)) {
    return u8_l(dansi_cursor_clearTabStop_static());
};

fn_((dansi_cursor_clearTabStopWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_cursor_clearTabStop());
};

fn_((dansi_cursor_clearAllTabStops(void))(S_const$u8)) {
    return u8_l(dansi_cursor_clearAllTabStops_static());
};

fn_((dansi_cursor_clearAllTabStopsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_cursor_clearAllTabStops());
};

fn_((dansi_cursor_requestPos(void))(S_const$u8)) {
    return u8_l(dansi_cursor_requestPos_static());
};

fn_((dansi_cursor_requestPosWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_cursor_requestPos());
};

fn_((dansi_cursor_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_cursor_parsePosReport(S_const$u8 report))(dansi_cursor_E$dansi_cursor_Pos) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_err(E_cause$dansi_cursor_InvalidResponse())));
    if (frame.final != dansi_cursor_pos_report_final_byte) return_err(E_cause$dansi_cursor_InvalidResponse());

    let row = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_cursor_pos_report_param_row))(
        return_err(E_cause$dansi_cursor_InvalidResponse())
    ));
    let col = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_cursor_pos_report_param_col))(
        return_err(E_cause$dansi_cursor_InvalidResponse())
    ));
    return_ok({ .row = row, .col = col });
} $unscoped(fn);

fn_((dansi_cursor_fetchPos(io_Writer out, io_Reader in, S$u8 buf))(dansi_cursor_E$dansi_cursor_Pos) $scope) {
    try_(dansi_cursor_requestPosWrite(out));
    let report = try_(dansi_cursor_receivePosReport(in, buf));
    return dansi_cursor_parsePosReport(report.as_const);
} $unscoped(fn);
