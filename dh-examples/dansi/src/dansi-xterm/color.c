#include "dansi-xterm/color.h"
#include <dh/io/Fixed.h>

fn_((dansi_xterm_color_RGB16_toRGB(dansi_xterm_color_RGB16 rgb))(dansi_xterm_color_RGB8)) {
    return (dansi_xterm_color_RGB8){
        .r = as$(u8)(rgb.r >> dansi_xterm_color_RGB16_to8_shift),
        .g = as$(u8)(rgb.g >> dansi_xterm_color_RGB16_to8_shift),
        .b = as$(u8)(rgb.b >> dansi_xterm_color_RGB16_to8_shift),
    };
};

fn_((dansi_xterm_color_RGB8_toRGB16(dansi_xterm_color_RGB8 rgb))(dansi_xterm_color_RGB16)) {
    return (dansi_xterm_color_RGB16){
        .r = as$(u16)(rgb.r) * dansi_xterm_color_RGB16_from8_scale,
        .g = as$(u16)(rgb.g) * dansi_xterm_color_RGB16_from8_scale,
        .b = as$(u16)(rgb.b) * dansi_xterm_color_RGB16_from8_scale,
    };
};

fn_((dansi_xterm_color_fg4bit(dansi_xterm_Palette4bit color, dansi_xterm_color_Fg4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_fg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_fg4bitWrite(dansi_xterm_Palette4bit color, io_Writer out))(E$void)) {
    let code = as$(u8)(color < dansi_xterm_color_4bit_normal_count
                       ? dansi_xterm_color_fg_normal_base + as$(u8)(color)
                       : dansi_xterm_color_fg_bright_base + as$(u8)(color)-dansi_xterm_color_4bit_normal_count);
    return io_Writer_print(out, u8_l(dansi_sgr_setRaw_static("{:uhh}")), code);
};

fn_((dansi_xterm_color_bg4bit(dansi_xterm_Palette4bit color, dansi_xterm_color_Bg4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_bg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_bg4bitWrite(dansi_xterm_Palette4bit color, io_Writer out))(E$void)) {
    let code = as$(u8)(color < dansi_xterm_color_4bit_normal_count
                       ? dansi_xterm_color_bg_normal_base + as$(u8)(color)
                       : dansi_xterm_color_bg_bright_base + as$(u8)(color)-dansi_xterm_color_4bit_normal_count);
    return io_Writer_print(out, u8_l(dansi_sgr_setRaw_static("{:uhh}")), code);
};

fn_((dansi_xterm_color_fgBright(dansi_color_Std color, dansi_xterm_color_FgBrightBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_fgBrightWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_fgBrightWrite(dansi_color_Std color, io_Writer out))(E$void)) {
    return io_Writer_print(
        out, u8_l(dansi_sgr_setRaw_static("{:uhh}")), as$(u8)(dansi_xterm_color_fg_bright_base + as$(u8)(color))
    );
};

fn_((dansi_xterm_color_bgBright(dansi_color_Std color, dansi_xterm_color_BgBrightBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_bgBrightWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_bgBrightWrite(dansi_color_Std color, io_Writer out))(E$void)) {
    return io_Writer_print(
        out, u8_l(dansi_sgr_setRaw_static("{:uhh}")), as$(u8)(dansi_xterm_color_bg_bright_base + as$(u8)(color))
    );
};

fn_((dansi_xterm_color_fg8bit(dansi_xterm_Palette8bit color, dansi_xterm_color_Fg8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_fg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_fg8bitWrite(dansi_xterm_Palette8bit color, io_Writer out))(E$void)) {
    return io_Writer_print(
        out, u8_l(dansi_sgr_setRaw_static(dansi_xterm_color_fg8bit_prefix "{:uhh}")), as$(u8)(color)
    );
};

fn_((dansi_xterm_color_bg8bit(dansi_xterm_Palette8bit color, dansi_xterm_color_Bg8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_bg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_bg8bitWrite(dansi_xterm_Palette8bit color, io_Writer out))(E$void)) {
    return io_Writer_print(
        out, u8_l(dansi_sgr_setRaw_static(dansi_xterm_color_bg8bit_prefix "{:uhh}")), as$(u8)(color)
    );
};

fn_((dansi_xterm_color_fgRGB(dansi_xterm_color_RGB8 rgb, dansi_xterm_color_FgRGBBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_fgRGBWrite(rgb, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_fgRGBWrite(dansi_xterm_color_RGB8 rgb, io_Writer out))(E$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_sgr_setRaw_static(
            dansi_xterm_color_fgRGB_prefix "{:uhh}" dansi_csi_param_sep "{:uhh}" dansi_csi_param_sep "{:uhh}"
        )),
        rgb.r, rgb.g, rgb.b
    );
};

fn_((dansi_xterm_color_bgRGB(dansi_xterm_color_RGB8 rgb, dansi_xterm_color_BgRGBBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_color_bgRGBWrite(rgb, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_color_bgRGBWrite(dansi_xterm_color_RGB8 rgb, io_Writer out))(E$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_sgr_setRaw_static(
            dansi_xterm_color_bgRGB_prefix "{:uhh}" dansi_csi_param_sep "{:uhh}" dansi_csi_param_sep "{:uhh}"
        )),
        rgb.r, rgb.g, rgb.b
    );
};
