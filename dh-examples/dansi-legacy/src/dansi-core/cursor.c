#include "dansi-core/cursor.h"
#include "dansi-core/Seq.h"
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>
#include <dh/fmt/common.h>

/*========== External Definitions ===========================================*/

fn_((dansi_cursor_moveTo(u16 col, u16 row, dansi_cursor_MovePosBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToWrite(col, row, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToWrite(u16 col, u16 row, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveTo_static("{:uh}", "{:uh}")), row, col);
};

fn_((dansi_cursor_moveUp(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveUpWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveUpWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveUp_static("{:uh}")), rows);
};

fn_((dansi_cursor_moveDown(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveDownWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveDownWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveDown_static("{:uh}")), rows);
};

fn_((dansi_cursor_moveRight(u16 cols, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveRightWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveRightWrite(u16 cols, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveRight_static("{:uh}")), cols);
};

fn_((dansi_cursor_moveLeft(u16 cols, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveLeftWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveLeftWrite(u16 cols, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveLeft_static("{:uh}")), cols);
};

fn_((dansi_cursor_moveToRow(u16 row, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToRowWrite(row, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToRowWrite(u16 row, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveToRow_static("{:uh}")), row);
};

fn_((dansi_cursor_moveToCol(u16 col, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveToColWrite(col, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveToColWrite(u16 col, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveToCol_static("{:uh}")), col);
};

fn_((dansi_cursor_moveNextLine(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_moveNextLineWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_moveNextLineWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_moveNextLine_static("{:uh}")), rows);
};

fn_((dansi_cursor_movePrevLine(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_movePrevLineWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_movePrevLineWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_movePrevLine_static("{:uh}")), rows);
};

fn_((dansi_cursor_hide(void))(S_const$u8)) {
    return u8_l(dansi_cursor_hide_static());
};

fn_((dansi_cursor_hideWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_hide_static()));
};

fn_((dansi_cursor_show(void))(S_const$u8)) {
    return u8_l(dansi_cursor_show_static());
};

fn_((dansi_cursor_showWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_show_static()));
};

fn_((dansi_cursor_storePos(void))(S_const$u8)) {
    return u8_l(dansi_cursor_storePos_static());
};

fn_((dansi_cursor_storePosWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_storePos_static()));
};

fn_((dansi_cursor_restorePos(void))(S_const$u8)) {
    return u8_l(dansi_cursor_restorePos_static());
};

fn_((dansi_cursor_restorePosWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_restorePos_static()));
};

fn_((dansi_cursor_setStyle(dansi_cursor_Style style, dansi_cursor_StyleBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_cursor_setStyleWrite(style, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_cursor_setStyleWrite(dansi_cursor_Style style, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_cursor_setStyle_static("{:uh}")), as$(u16)(style));
};

fn_((dansi_cursor_setTabStop(void))(S_const$u8)) {
    return u8_l(dansi_cursor_setTabStop_static());
};

fn_((dansi_cursor_setTabStopWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_cursor_setTabStop());
};

fn_((dansi_cursor_clearTabStop(void))(S_const$u8)) {
    return u8_l(dansi_cursor_clearTabStop_static());
};

fn_((dansi_cursor_clearTabStopWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_cursor_clearTabStop());
};

fn_((dansi_cursor_clearAllTabStops(void))(S_const$u8)) {
    return u8_l(dansi_cursor_clearAllTabStops_static());
};

fn_((dansi_cursor_clearAllTabStopsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_cursor_clearAllTabStops());
};

fn_((dansi_cursor_requestPos(void))(S_const$u8)) {
    return u8_l(dansi_cursor_requestPos_static());
};

fn_((dansi_cursor_requestPosWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_cursor_requestPos());
};

fn_((dansi_cursor_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_cursor_parsePosReport(S_const$u8 report))(dansi_cursor_E$dansi_cursor_Pos) $scope) {
    if (!mem_startsWithBytes(report, u8_l(dansi_utils_csi))) {
        return_err(E_cause$dansi_cursor_InvalidResponse());
    }
    if (report.len <= u8_l(dansi_utils_csi).len || *S_at((report)[report.len - 1]) != 'R') {
        return_err(E_cause$dansi_cursor_InvalidResponse());
    }
    var it = mem_tokzAnyBytes(
        S_suffix((report)(u8_l(dansi_utils_csi).len)),
        u8_l(dansi_utils_sep dansi_utils_cursor_response_pos)
    );
    let row_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_cursor_InvalidResponse())));
    let col_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_cursor_InvalidResponse())));
    let row = catch_((fmt_parse$u16(row_str, 10))($ignore, return_err(E_cause$dansi_cursor_InvalidResponse())));
    let col = catch_((fmt_parse$u16(col_str, 10))($ignore, return_err(E_cause$dansi_cursor_InvalidResponse())));
    return_ok({ .row = row, .col = col });
} $unscoped(fn);

fn_((dansi_cursor_fetchPos(io_Writer out, io_Reader in, S$u8 buf))(dansi_cursor_E$dansi_cursor_Pos) $scope) {
    try_(dansi_cursor_requestPosWrite(out));
    let report = try_(dansi_cursor_receivePosReport(in, buf));
    return dansi_cursor_parsePosReport(report.as_const);
} $unscoped(fn);
