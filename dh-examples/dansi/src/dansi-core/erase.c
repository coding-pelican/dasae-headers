#include "dansi-core/erase.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_erase_inDisplay(dansi_erase_Area area, dansi_erase_InDisplayBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_erase_inDisplayWrite(area, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_erase_inDisplayWrite(dansi_erase_Area area, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make1_static("{:uhh}", dansi_erase_in_display_final)), as$(u8)(area));
};

fn_((dansi_erase_inLine(dansi_erase_Area area, dansi_erase_InLineBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_erase_inLineWrite(area, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_erase_inLineWrite(dansi_erase_Area area, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make1_static("{:uhh}", dansi_erase_in_line_final)), as$(u8)(area));
};

fn_((dansi_erase_chars(u16 count, dansi_erase_CharsBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_erase_charsWrite(count, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_erase_charsWrite(u16 count, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_erase_chars_static("{:uh}")), count);
};
