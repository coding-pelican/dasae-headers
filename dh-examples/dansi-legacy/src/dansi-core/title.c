#include "dansi-core/title.h"
#include <dh/io/Fixed.h>

fn_((dansi_title_setIcon(S_const$u8 title, S$u8 buf))(S$u8)) {
    return dansi_utils_formatOSC(u8_l("1"), title, buf);
};

fn_((dansi_title_setIconWrite(S_const$u8 title, io_Writer writer))(E$void)) {
    return dansi_utils_formatOSCWrite(u8_l("1"), title, writer);
};

fn_((dansi_title_setWindow(S_const$u8 title, S$u8 buf))(S$u8)) {
    return dansi_utils_formatOSC(u8_l("2"), title, buf);
};

fn_((dansi_title_setWindowWrite(S_const$u8 title, io_Writer writer))(E$void)) {
    return dansi_utils_formatOSCWrite(u8_l("2"), title, writer);
};

fn_((dansi_title_setBoth(S_const$u8 title, S$u8 buf))(S$u8)) {
    return dansi_utils_formatOSC(u8_l("0"), title, buf);
};

fn_((dansi_title_setBothWrite(S_const$u8 title, io_Writer writer))(E$void)) {
    return dansi_utils_formatOSCWrite(u8_l("0"), title, writer);
};

fn_((dansi_title_push(dansi_title_StackTarget target, dansi_title_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_title_pushWrite(target, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_title_pushWrite(dansi_title_StackTarget target, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_title_push_static("{:uh}")), as$(u16)(target));
};

fn_((dansi_title_pop(dansi_title_StackTarget target, dansi_title_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_title_popWrite(target, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_title_popWrite(dansi_title_StackTarget target, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_title_pop_static("{:uh}")), as$(u16)(target));
};
