#include "daterm/line.h"
#include <dh/io/Fixed.h>

/*========== External Definitions ===========================================*/

fn_((daterm_line_scrollUp(u16 lines, daterm_line_ScrollDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_line_scrollUpWrite(lines, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_line_scrollUpWrite(u16 lines, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_line_scrollUp_static("{:uh}")), lines);
};

fn_((daterm_line_scrollDown(u16 lines, daterm_line_ScrollDirBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_line_scrollDownWrite(lines, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_line_scrollDownWrite(u16 lines, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_line_scrollDown_static("{:uh}")), lines);
};

fn_((daterm_line_clearFromCursor(void))(S_const$u8)) {
    return u8_l(daterm_line_clearFromCursor_static());
};

fn_((daterm_line_clearFromCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_line_clearFromCursor_static()));
};

fn_((daterm_line_clearToCursor(void))(S_const$u8)) {
    return u8_l(daterm_line_clearToCursor_static());
};

fn_((daterm_line_clearToCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_line_clearToCursor_static()));
};

fn_((daterm_line_clear(void))(S_const$u8)) {
    return u8_l(daterm_line_clear_static());
};

fn_((daterm_line_clearWrite(io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_line_clear_static()));
};
