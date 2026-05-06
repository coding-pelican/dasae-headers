#include "daterm/cursor.h"
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>
#include <dh/fmt/common.h>

/*========== External Definitions ===========================================*/

fn_((daterm_cursor_moveTo(u16 col, u16 row, daterm_cursor_MovePosBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_cursor_moveToWrite(col, row, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_cursor_moveToWrite(u16 col, u16 row, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_moveTo_static("{:uh}", "{:uh}")), row, col);
};

fn_((daterm_cursor_moveUp(u16 rows, daterm_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_cursor_moveUpWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_cursor_moveUpWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_moveUp_static("{:uh}")), rows);
};

fn_((daterm_cursor_moveDown(u16 rows, daterm_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_cursor_moveDownWrite(rows, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_cursor_moveDownWrite(u16 rows, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_moveDown_static("{:uh}")), rows);
};

fn_((daterm_cursor_moveRight(u16 cols, daterm_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_cursor_moveRightWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_cursor_moveRightWrite(u16 cols, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_moveRight_static("{:uh}")), cols);
};

fn_((daterm_cursor_moveLeft(u16 cols, daterm_cursor_MoveDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_cursor_moveLeftWrite(cols, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_cursor_moveLeftWrite(u16 cols, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_moveLeft_static("{:uh}")), cols);
};

fn_((daterm_cursor_hide(void))(S_const$u8)) {
    return u8_l(daterm_cursor_hide_static());
};

fn_((daterm_cursor_hideWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_hide_static()));
};

fn_((daterm_cursor_show(void))(S_const$u8)) {
    return u8_l(daterm_cursor_show_static());
};

fn_((daterm_cursor_showWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_show_static()));
};

fn_((daterm_cursor_storePos(void))(S_const$u8)) {
    return u8_l(daterm_cursor_storePos_static());
};

fn_((daterm_cursor_storePosWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_storePos_static()));
};

fn_((daterm_cursor_restorePos(void))(S_const$u8)) {
    return u8_l(daterm_cursor_restorePos_static());
};

fn_((daterm_cursor_restorePosWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_cursor_restorePos_static()));
};

T_use$((u8)(
    mem_Delim,
    mem_TokzIter,
    mem_tokzAny,
    mem_TokzIter_next
));
fn_((daterm_cursor_queryPos(io_Reader in, io_Writer out))(daterm_cursor_E$daterm_cursor_Pos) $scope) {
    try_(io_Writer_print(out, u8_l(daterm_utils_formatCSI_static(daterm_utils_cursor_request_pos))));
    var_(buf, A$$(4 + 10, u8)) = A_zero();
    let data = A_prefix$((S$u8)(buf)try_(io_Reader_read(in, A_ref$((S$u8)(buf)))));
    // example response: \x1B[12;45R
    if (!mem_startsWithBytes(data.as_const, u8_l(daterm_utils_csi))) {
        return_err(E_cause$daterm_cursor_InvalidResponse());
    }
    var it = mem_tokzAny$u8(
        S_suffix((data.as_const)(u8_l(daterm_utils_csi).len)),
        u8_l(daterm_utils_sep daterm_utils_cursor_response_pos)
    );
    let row_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$daterm_cursor_InvalidResponse())));
    let col_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$daterm_cursor_InvalidResponse())));
    let row = try_(fmt_parse$u16(row_str, 10));
    let col = try_(fmt_parse$u16(col_str, 10));
    return_ok({ .row = row, .col = col });
} $unscoped(fn);
