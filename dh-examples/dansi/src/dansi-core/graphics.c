#include "dansi-core/graphics.h"
#include <dh/io/Fixed.h>

fn_((dansi_graphics_read(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_graphics_readWrite(item, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_graphics_readWrite(dansi_graphics_Item item, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_graphics_read_static("{:uhh}")), as$(u8)(item));
};

fn_((dansi_graphics_reset(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_graphics_resetWrite(item, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_graphics_resetWrite(dansi_graphics_Item item, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_graphics_reset_static("{:uhh}")), as$(u8)(item));
};

fn_((dansi_graphics_readMax(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_graphics_readMaxWrite(item, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_graphics_readMaxWrite(dansi_graphics_Item item, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_graphics_readMax_static("{:uhh}")), as$(u8)(item));
};

fn_((dansi_graphics_setColorRegisters(u16 count, dansi_graphics_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_graphics_setColorRegistersWrite(count, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_graphics_setColorRegistersWrite(u16 count, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_graphics_setColorRegisters_static("{:uh}")), count);
};

fn_((dansi_graphics_setGeometry(dansi_graphics_Item item, u16 width, u16 height, dansi_graphics_Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_graphics_setGeometryWrite(item, width, height, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_graphics_setGeometryWrite(dansi_graphics_Item item, u16 width, u16 height, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_graphics_setGeometry_static("{:uhh}", "{:uh}", "{:uh}")), as$(u8)(item), width, height);
};
