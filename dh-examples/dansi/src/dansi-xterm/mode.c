#include "dansi-xterm/mode.h"
#include <dh/io/Fixed.h>

fn_((dansi_xterm_mode_setRaw(u16 mode, bool enabled, dansi_xterm_mode_SetRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mode_setRawWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mode_setRawWrite(u16 mode, bool enabled, io_Writer out))(io_PrintE$void)) {
    return enabled ? dansi_xterm_mode_enableRawWrite(mode, out)
                   : dansi_xterm_mode_disableRawWrite(mode, out);
};

fn_((dansi_xterm_mode_enableRaw(u16 mode, dansi_xterm_mode_EnableRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mode_enableRawWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mode_enableRawWrite(u16 mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_xterm_mode_enableRaw_static("{:uh}")), mode);
};

fn_((dansi_xterm_mode_disableRaw(u16 mode, dansi_xterm_mode_DisableRawBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mode_disableRawWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mode_disableRawWrite(u16 mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_xterm_mode_disableRaw_static("{:uh}")), mode);
};

fn_((dansi_xterm_mode_setMany(S_const$u16 modes, bool enabled, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_xterm_mode_setManyWrite(modes, enabled, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_mode_setManyWrite(S_const$u16 modes, bool enabled, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix dansi_xterm_mode_private_marker)));
    for_(($s(modes), $rf(0))(mode, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), *mode));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, enabled ? dansi_xterm_mode_enable_final_byte : dansi_xterm_mode_disable_final_byte)));
} $unscoped(fn);

fn_((dansi_xterm_mode_set(dansi_xterm_mode_Code mode, bool enabled, dansi_xterm_mode_SetBuf* buf))(S$u8)) {
    return dansi_xterm_mode_setRaw(as$(u16)(mode), enabled, buf);
};

fn_((dansi_xterm_mode_setWrite(dansi_xterm_mode_Code mode, bool enabled, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_mode_setRawWrite(as$(u16)(mode), enabled, out);
};

fn_((dansi_xterm_mode_enable(dansi_xterm_mode_Code mode, dansi_xterm_mode_EnableBuf* buf))(S$u8)) {
    return dansi_xterm_mode_enableRaw(as$(u16)(mode), buf);
};

fn_((dansi_xterm_mode_enableWrite(dansi_xterm_mode_Code mode, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_mode_enableRawWrite(as$(u16)(mode), out);
};

fn_((dansi_xterm_mode_disable(dansi_xterm_mode_Code mode, dansi_xterm_mode_DisableBuf* buf))(S$u8)) {
    return dansi_xterm_mode_disableRaw(as$(u16)(mode), buf);
};

fn_((dansi_xterm_mode_disableWrite(dansi_xterm_mode_Code mode, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_mode_disableRawWrite(as$(u16)(mode), out);
};

fn_((dansi_xterm_mode_saveRaw(S_const$u16 modes, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_xterm_mode_saveRawWrite(modes, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_mode_saveRawWrite(S_const$u16 modes, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix dansi_xterm_mode_private_marker)));
    for_(($s(modes), $rf(0))(mode, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), *mode));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, dansi_xterm_mode_save_final_byte)));
} $unscoped(fn);

fn_((dansi_xterm_mode_restoreRaw(S_const$u16 modes, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_xterm_mode_restoreRawWrite(modes, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_mode_restoreRawWrite(S_const$u16 modes, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix dansi_xterm_mode_private_marker)));
    for_(($s(modes), $rf(0))(mode, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), *mode));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, dansi_xterm_mode_restore_final_byte)));
} $unscoped(fn);

fn_((dansi_xterm_mode_save(S_const$dansi_xterm_mode_Code modes, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_xterm_mode_saveWrite(modes, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_mode_saveWrite(S_const$dansi_xterm_mode_Code modes, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix dansi_xterm_mode_private_marker)));
    for_(($s(modes), $rf(0))(mode, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), as$(u16)(*mode)));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, dansi_xterm_mode_save_final_byte)));
} $unscoped(fn);

fn_((dansi_xterm_mode_restore(S_const$dansi_xterm_mode_Code modes, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_xterm_mode_restoreWrite(modes, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_xterm_mode_restoreWrite(S_const$dansi_xterm_mode_Code modes, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix dansi_xterm_mode_private_marker)));
    for_(($s(modes), $rf(0))(mode, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), as$(u16)(*mode)));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, dansi_xterm_mode_restore_final_byte)));
} $unscoped(fn);

fn_((dansi_xterm_mode_saveOne(dansi_xterm_mode_Code mode, dansi_xterm_mode_SaveOneBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mode_saveOneWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mode_saveOneWrite(dansi_xterm_mode_Code mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_xterm_mode_saveRaw_static("{:uh}")), as$(u16)(mode));
};

fn_((dansi_xterm_mode_restoreOne(dansi_xterm_mode_Code mode, dansi_xterm_mode_RestoreOneBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_mode_restoreOneWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_mode_restoreOneWrite(dansi_xterm_mode_Code mode, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_xterm_mode_restoreRaw_static("{:uh}")), as$(u16)(mode));
};
