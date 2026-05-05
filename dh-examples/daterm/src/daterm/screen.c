#include "daterm/screen.h"
#include "daterm/cursor.h"

/*========== External Definitions ===========================================*/

fn_((daterm_screen_enterAlternate(void))(S_const$u8)) {
    return u8_l(daterm_screen_enterAlternate_static());
};

fn_((daterm_screen_enterAlternateWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_screen_enterAlternate());
};

fn_((daterm_screen_exitAlternate(void))(S_const$u8)) {
    return u8_l(daterm_screen_exitAlternate_static());
};

fn_((daterm_screen_exitAlternateWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_screen_exitAlternate());
};

fn_((daterm_screen_clearFromCursor(void))(S_const$u8)) {
    return u8_l(daterm_screen_clearFromCursor_static());
};

fn_((daterm_screen_clearFromCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_screen_clearFromCursor());
};

fn_((daterm_screen_clearToCursor(void))(S_const$u8)) {
    return u8_l(daterm_screen_clearToCursor_static());
};

fn_((daterm_screen_clearToCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_screen_clearToCursor());
};

fn_((daterm_screen_clear(void))(S_const$u8)) {
    return u8_l(daterm_screen_clear_static());
};

fn_((daterm_screen_clearWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_screen_clear());
};

fn_((daterm_screen_querySize(io_Reader in, io_Writer out))(daterm_screen_Err$daterm_screen_Size) $scope) {
    try_(daterm_cursor_storePosWrite(out));
    // Move to "infinite" position - terminal will clamp to actual size
    try_(daterm_cursor_moveToWrite(u16_limit_max, u16_limit_max, out));
    // Query current position (will return actual terminal size)
    let pos = try_(daterm_cursor_queryPos(in, out));
    try_(daterm_cursor_restorePosWrite(out));
    return_ok({
        .cols = pos.col,
        .rows = pos.row,
    });
} $unscoped(fn);
