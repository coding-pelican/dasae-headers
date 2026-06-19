#include "dansi-dec/cursor.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_dec_cursor_show(dansi_dec_cursor_ShowBuf* buf))(S$u8)) {
    return dansi_dec_mode_enable(dansi_dec_mode_Code_text_cursor, buf);
};

fn_((dansi_dec_cursor_showWrite(io_Writer out))(E$void)) {
    return dansi_dec_mode_enableWrite(dansi_dec_mode_Code_text_cursor, out);
};

fn_((dansi_dec_cursor_hide(dansi_dec_cursor_HideBuf* buf))(S$u8)) {
    return dansi_dec_mode_disable(dansi_dec_mode_Code_text_cursor, buf);
};

fn_((dansi_dec_cursor_hideWrite(io_Writer out))(E$void)) {
    return dansi_dec_mode_disableWrite(dansi_dec_mode_Code_text_cursor, out);
};

fn_((dansi_dec_cursor_save(void))(S_const$u8)) {
    return u8_l(dansi_dec_cursor_save_static());
};

fn_((dansi_dec_cursor_saveWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_cursor_save());
};

fn_((dansi_dec_cursor_restore(void))(S_const$u8)) {
    return u8_l(dansi_dec_cursor_restore_static());
};

fn_((dansi_dec_cursor_restoreWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_cursor_restore());
};

fn_((dansi_dec_cursor_setStyle(dansi_dec_cursor_Style style, dansi_dec_cursor_SetStyleBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_cursor_setStyleWrite(style, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_cursor_setStyleWrite(dansi_dec_cursor_Style style, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("{:uhh}", " ", "q")), as$(u8)(style));
};
