#include "dansi-core/scroll.h"
#include <dh/io/Fixed.h>

fn_((dansi_scroll_setRegion(u16 top, u16 bottom, dansi_scroll_RegionBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_scroll_setRegionWrite(top, bottom, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_scroll_setRegionWrite(u16 top, u16 bottom, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_scroll_setRegion_static("{:uh}", "{:uh}")), top, bottom);
};

fn_((dansi_scroll_resetRegion(void))(S_const$u8)) {
    return u8_l(dansi_scroll_resetRegion_static());
};

fn_((dansi_scroll_resetRegionWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_scroll_resetRegion());
};
