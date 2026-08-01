#include "dansi-core/scroll.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_scroll_up(u16 lines, dansi_scroll_UpBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_scroll_upWrite(lines, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_scroll_upWrite(u16 lines, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_scroll_up_static("{:uh}")), lines);
};

fn_((dansi_scroll_down(u16 lines, dansi_scroll_DownBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_scroll_downWrite(lines, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_scroll_downWrite(u16 lines, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_scroll_down_static("{:uh}")), lines);
};
