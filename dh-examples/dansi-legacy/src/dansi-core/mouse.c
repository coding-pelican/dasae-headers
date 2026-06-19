#include "dansi-core/mouse.h"
#include <dh/io/Fixed.h>

fn_((dansi_mouse_enableTracking(dansi_mouse_TrackingMode mode, dansi_mouse_TrackingModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_enableTrackingWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_enableTrackingWrite(dansi_mouse_TrackingMode mode, io_Writer writer))(E$void)) {
    return dansi_mode_enablePrivateWrite(as$(u16)(mode), writer);
};

fn_((dansi_mouse_disableTracking(dansi_mouse_TrackingMode mode, dansi_mouse_TrackingModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_disableTrackingWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_disableTrackingWrite(dansi_mouse_TrackingMode mode, io_Writer writer))(E$void)) {
    return dansi_mode_disablePrivateWrite(as$(u16)(mode), writer);
};

fn_((dansi_mouse_setTracking(dansi_mouse_TrackingMode mode, bool enabled, dansi_mouse_TrackingModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_setTrackingWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_setTrackingWrite(dansi_mouse_TrackingMode mode, bool enabled, io_Writer writer))(E$void)) {
    return dansi_mode_setPrivateWrite(as$(u16)(mode), enabled, writer);
};

fn_((dansi_mouse_enableAny(void))(S_const$u8)) {
    return u8_l(dansi_mouse_enableAny_static());
};

fn_((dansi_mouse_enableAnyWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_enableAny());
};

fn_((dansi_mouse_disableAny(void))(S_const$u8)) {
    return u8_l(dansi_mouse_disableAny_static());
};

fn_((dansi_mouse_disableAnyWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_disableAny());
};

fn_((dansi_mouse_enableSGR(void))(S_const$u8)) {
    return u8_l(dansi_mouse_enableSGR_static());
};

fn_((dansi_mouse_enableSGRWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_enableSGR());
};

fn_((dansi_mouse_disableSGR(void))(S_const$u8)) {
    return u8_l(dansi_mouse_disableSGR_static());
};

fn_((dansi_mouse_disableSGRWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_disableSGR());
};
