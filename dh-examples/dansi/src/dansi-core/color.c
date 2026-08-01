#include "dansi-core/color.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_color__one(dansi_sgr_Code code, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    catch_((dansi_sgr_setOneWrite(code, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_fg(dansi_color_Std color, dansi_color_FgBuf* buf))(S$u8)) {
    return dansi_color__one(as$(dansi_sgr_Code)(dansi_sgr_Code_fg_black + as$(u8)(color)), A_ref$((S$u8)(*buf)));
};

fn_((dansi_color_fgWrite(dansi_color_Std color, io_Writer out))(io_PrintE$void)) {
    return dansi_sgr_setOneWrite(as$(dansi_sgr_Code)(dansi_sgr_Code_fg_black + as$(u8)(color)), out);
};

fn_((dansi_color_bg(dansi_color_Std color, dansi_color_BgBuf* buf))(S$u8)) {
    return dansi_color__one(as$(dansi_sgr_Code)(dansi_sgr_Code_bg_black + as$(u8)(color)), A_ref$((S$u8)(*buf)));
};

fn_((dansi_color_bgWrite(dansi_color_Std color, io_Writer out))(io_PrintE$void)) {
    return dansi_sgr_setOneWrite(as$(dansi_sgr_Code)(dansi_sgr_Code_bg_black + as$(u8)(color)), out);
};

fn_((dansi_color_resetFG(dansi_color_ResetFGBuf* buf))(S$u8)) {
    return dansi_color__one(dansi_sgr_Code_fg_default, A_ref$((S$u8)(*buf)));
};

fn_((dansi_color_resetFGWrite(io_Writer out))(io_PrintE$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_fg_default, out);
};

fn_((dansi_color_resetBG(dansi_color_ResetBGBuf* buf))(S$u8)) {
    return dansi_color__one(dansi_sgr_Code_bg_default, A_ref$((S$u8)(*buf)));
};

fn_((dansi_color_resetBGWrite(io_Writer out))(io_PrintE$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_bg_default, out);
};
