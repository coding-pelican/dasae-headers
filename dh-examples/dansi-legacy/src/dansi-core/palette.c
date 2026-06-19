#include "dansi-core/palette.h"
#include <dh/io/Fixed.h>

fn_((dansi_palette_push(u8 index, dansi_palette_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_palette_pushWrite(index, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_palette_pushWrite(u8 index, io_Writer writer))(E$void)) {
    if (index == 0) return io_Writer_writeBytes(writer, u8_l(dansi_palette_push_static()));
    return io_Writer_print(writer, u8_l(dansi_palette_pushAt_static("{:uhh}")), index);
};

fn_((dansi_palette_pop(u8 index, dansi_palette_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_palette_popWrite(index, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_palette_popWrite(u8 index, io_Writer writer))(E$void)) {
    if (index == 0) return io_Writer_writeBytes(writer, u8_l(dansi_palette_pop_static()));
    return io_Writer_print(writer, u8_l(dansi_palette_popAt_static("{:uhh}")), index);
};

fn_((dansi_palette_reportStack(u8 index, dansi_palette_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_palette_reportStackWrite(index, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_palette_reportStackWrite(u8 index, io_Writer writer))(E$void)) {
    if (index == 0) return io_Writer_writeBytes(writer, u8_l(dansi_palette_reportStack_static()));
    return io_Writer_print(writer, u8_l(dansi_palette_reportStackAt_static("{:uhh}")), index);
};
