#include "dansi-core/mode.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_mode_setRaw(u16 mode, bool enabled, dansi_mode_SetRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_setRawWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_setRawWrite(u16 mode, bool enabled, io_Writer out))(E$void)) {
    return enabled ? dansi_mode_enableRawWrite(mode, out)
                   : dansi_mode_disableRawWrite(mode, out);
};

fn_((dansi_mode_enableRaw(u16 mode, dansi_mode_EnableRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_enableRawWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_enableRawWrite(u16 mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_mode_enableRaw_static("{:uh}")), mode);
};

fn_((dansi_mode_disableRaw(u16 mode, dansi_mode_DisableRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mode_disableRawWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mode_disableRawWrite(u16 mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_mode_disableRaw_static("{:uh}")), mode);
};

fn_((dansi_mode_setMany(S_const$u16 modes, bool enabled, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_mode_setManyWrite(modes, enabled, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_mode_setManyWrite(S_const$u16 modes, bool enabled, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix)));
    for_(($s(modes), $rf(0))(ch, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), *ch));
    } $end(for);
    return io_Writer_writeByte(out, enabled ? dansi_mode_enable_final_byte : dansi_mode_disable_final_byte);
} $unscoped(fn);

fn_((dansi_mode_set(dansi_mode_Code mode, bool enabled, dansi_mode_SetBuf* buf))(S$u8)) {
    return dansi_mode_setRaw(as$(u16)(mode), enabled, buf);
};

fn_((dansi_mode_setWrite(dansi_mode_Code mode, bool enabled, io_Writer out))(E$void)) {
    return dansi_mode_setRawWrite(as$(u16)(mode), enabled, out);
};

fn_((dansi_mode_enable(dansi_mode_Code mode, dansi_mode_EnableBuf* buf))(S$u8)) {
    return dansi_mode_enableRaw(as$(u16)(mode), buf);
};

fn_((dansi_mode_enableWrite(dansi_mode_Code mode, io_Writer out))(E$void)) {
    return dansi_mode_enableRawWrite(as$(u16)(mode), out);
};

fn_((dansi_mode_disable(dansi_mode_Code mode, dansi_mode_DisableBuf* buf))(S$u8)) {
    return dansi_mode_disableRaw(as$(u16)(mode), buf);
};

fn_((dansi_mode_disableWrite(dansi_mode_Code mode, io_Writer out))(E$void)) {
    return dansi_mode_disableRawWrite(as$(u16)(mode), out);
};
