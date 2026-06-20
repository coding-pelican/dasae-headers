#include "dansi-xterm/sgr.h"
#include <dh/io/Fixed.h>

fn_((dansi_xterm_sgr_push(dansi_xterm_sgr_PushBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_sgr_pushWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_sgr_pushWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, u8_l(dansi_xterm_sgr_push_static()));
};

fn_((dansi_xterm_sgr_pop(dansi_xterm_sgr_PopBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_sgr_popWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_sgr_popWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, u8_l(dansi_xterm_sgr_pop_static()));
};
