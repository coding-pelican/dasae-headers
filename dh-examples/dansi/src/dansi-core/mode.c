#include "dansi-core/mode.h"
#include <dh/io/Fixed.h>

fn_((dansi_mode_enablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_enablePrivateWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_enablePrivateWrite(u16 mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_mode_enablePrivate_static("{:uh}")), mode);
};

fn_((dansi_mode_disablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_disablePrivateWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_disablePrivateWrite(u16 mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_mode_disablePrivate_static("{:uh}")), mode);
};

fn_((dansi_mode_setPrivate(u16 mode, bool enabled, dansi_mode_PrivateBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_setPrivateWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_setPrivateWrite(u16 mode, bool enabled, io_Writer writer))(E$void)) {
    return enabled ? dansi_mode_enablePrivateWrite(mode, writer)
                   : dansi_mode_disablePrivateWrite(mode, writer);
};

fn_((dansi_mode_enableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_enableANSIWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_enableANSIWrite(u16 mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_mode_enableANSI_static("{:uh}")), mode);
};

fn_((dansi_mode_disableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_disableANSIWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_disableANSIWrite(u16 mode, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_mode_disableANSI_static("{:uh}")), mode);
};

fn_((dansi_mode_setANSI(u16 mode, bool enabled, dansi_mode_ANSIBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_setANSIWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_setANSIWrite(u16 mode, bool enabled, io_Writer writer))(E$void)) {
    return enabled ? dansi_mode_enableANSIWrite(mode, writer)
                   : dansi_mode_disableANSIWrite(mode, writer);
};
