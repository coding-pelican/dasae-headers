#include "dansi-dec/charset.h"
#include <dh/io/Fixed.h>

fn_((dansi_dec_charset_designate(
    dansi_dec_charset_Slot slot, dansi_dec_charset_Set set, dansi_dec_charset_DesignateBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_charset_designateWrite(slot, set, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_charset_designateWrite(
    dansi_dec_charset_Slot slot, dansi_dec_charset_Set set, io_Writer out
))(E$void)) {
    let slot_bytes = A_from$((u8){ [0] = as$(u8)(slot) });
    return dansi_esc_write(A_ref$((S_const$u8)(slot_bytes)), as$(u8)(set), out);
};

fn_((dansi_dec_charset_shiftIn(void))(S_const$u8)) {
    return u8_l(dansi_dec_charset_shiftIn_static());
};

fn_((dansi_dec_charset_shiftInWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_charset_shiftIn());
};

fn_((dansi_dec_charset_shiftOut(void))(S_const$u8)) {
    return u8_l(dansi_dec_charset_shiftOut_static());
};

fn_((dansi_dec_charset_shiftOutWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_charset_shiftOut());
};
