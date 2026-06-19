#include "dansi-core/window.h"
#include <dh/io/Fixed.h>

fn_((dansi_window_op(dansi_window_Op op, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_opWrite(op, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_opWrite(dansi_window_Op op, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_op_static("{:uhh}")), as$(u8)(op));
};

fn_((dansi_window_move(u16 x, u16 y, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_moveWrite(x, y, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_moveWrite(u16 x, u16 y, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_move_static("{:uh}", "{:uh}")), x, y);
};

fn_((dansi_window_resizePixels(u16 height, u16 width, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_resizePixelsWrite(height, width, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_resizePixelsWrite(u16 height, u16 width, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_resizePixels_static("{:uh}", "{:uh}")), height, width);
};

fn_((dansi_window_resizeChars(u16 height, u16 width, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_resizeCharsWrite(height, width, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_resizeCharsWrite(u16 height, u16 width, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_resizeChars_static("{:uh}", "{:uh}")), height, width);
};

fn_((dansi_window_maximize(dansi_window_Maximize mode, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_maximizeWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_maximizeWrite(dansi_window_Maximize mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_maximize_static("{:uhh}")), as$(u8)(mode));
};

fn_((dansi_window_fullscreen(dansi_window_Fullscreen mode, dansi_window_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_window_fullscreenWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_window_fullscreenWrite(dansi_window_Fullscreen mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_window_fullscreen_static("{:uhh}")), as$(u8)(mode));
};
