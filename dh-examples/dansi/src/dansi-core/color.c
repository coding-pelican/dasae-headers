#include "dansi-core/color.h"
#include <dh/io/Fixed.h>

/*========== External Definitions ===========================================*/

fn_((dansi_color_fg4bit(dansi_Palette4bit color, dansi_Color_Palette4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_fg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_fg4bitWrite(dansi_Palette4bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_fg4bit_static("{:uhh}")), color);
};

fn_((dansi_color_bg4bit(dansi_Palette4bit color, dansi_Color_Palette4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_bg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_bg4bitWrite(dansi_Palette4bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_bg4bit_static("{:uhh}")), color);
};

fn_((dansi_color_fg8bit(dansi_Palette8bit color, dansi_Color_Palette8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_fg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_fg8bitWrite(dansi_Palette8bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_fg8bit_static("{:uhh}")), color);
};

fn_((dansi_color_bg8bit(dansi_Palette8bit color, dansi_Color_Palette8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_bg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_bg8bitWrite(dansi_Palette8bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_bg8bit_static("{:uhh}")), color);
};

fn_((dansi_color_fg24bit(u8 r, u8 g, u8 b, dansi_Color_RGB24bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_fg24bitWrite(r, g, b, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_fg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_fg24bit_static("{:uhh}", "{:uhh}", "{:uhh}")), r, g, b);
};

fn_((dansi_color_bg24bit(u8 r, u8 g, u8 b, dansi_Color_RGB24bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_color_bg24bitWrite(r, g, b, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_color_bg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_color_bg24bit_static("{:uhh}", "{:uhh}", "{:uhh}")), r, g, b);
};
