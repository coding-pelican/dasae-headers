#include "daterm/color.h"
#include <dh/io/Fixed.h>

/*========== External Definitions ===========================================*/

fn_((daterm_color_fg4bit(daterm_Palette4bit color, daterm_Color_Palette4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_fg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_fg4bitWrite(daterm_Palette4bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_fg4bit_static("{:uhh}")), color);
};

fn_((daterm_color_bg4bit(daterm_Palette4bit color, daterm_Color_Palette4bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_bg4bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_bg4bitWrite(daterm_Palette4bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_bg4bit_static("{:uhh}")), color);
};

fn_((daterm_color_fg8bit(daterm_Palette8bit color, daterm_Color_Palette8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_fg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_fg8bitWrite(daterm_Palette8bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_fg8bit_static("{:uhh}")), color);
};

fn_((daterm_color_bg8bit(daterm_Palette8bit color, daterm_Color_Palette8bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_bg8bitWrite(color, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_bg8bitWrite(daterm_Palette8bit color, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_bg8bit_static("{:uhh}")), color);
};

fn_((daterm_color_fg24bit(u8 r, u8 g, u8 b, daterm_Color_RGB24bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_fg24bitWrite(r, g, b, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_fg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_fg24bit_static("{:uhh}", "{:uhh}", "{:uhh}")), r, g, b);
};

fn_((daterm_color_bg24bit(u8 r, u8 g, u8 b, daterm_Color_RGB24bitBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_color_bg24bitWrite(r, g, b, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_color_bg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_color_bg24bit_static("{:uhh}", "{:uhh}", "{:uhh}")), r, g, b);
};
