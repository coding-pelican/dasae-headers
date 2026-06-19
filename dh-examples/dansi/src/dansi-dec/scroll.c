#include "dansi-dec/scroll.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_dec_scroll_setMargins(u16 top, u16 bottom, dansi_dec_scroll_SetMarginsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_scroll_setMarginsWrite(top, bottom, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_scroll_setMarginsWrite(u16 top, u16 bottom, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_dec_scroll_setMargins_static("{:uh}", "{:uh}")), top, bottom);
};

fn_((dansi_dec_scroll_resetMargins(void))(S_const$u8)) {
    return u8_l(dansi_dec_scroll_resetMargins_static());
};

fn_((dansi_dec_scroll_resetMarginsWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_scroll_resetMargins());
};
