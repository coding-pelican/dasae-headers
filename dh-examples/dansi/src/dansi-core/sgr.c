#include "dansi-core/sgr.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_sgr_reset(void))(S_const$u8)) {
    return u8_l(dansi_sgr_reset_static());
};

fn_((dansi_sgr_resetWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_sgr_reset());
};

fn_((dansi_sgr_setRaw(S_const$u8 params, S$u8 buf))(E$S$u8) $scope) {
    return dansi_csi_make(params, u8_l(""), dansi_sgr_final_byte, buf);
} $unscoped(fn);

fn_((dansi_sgr_setRawWrite(S_const$u8 params, io_Writer out))(io_PrintE$void)) {
    return dansi_csi_write(params, u8_l(""), dansi_sgr_final_byte, out);
};

fn_((dansi_sgr_setOne(dansi_sgr_Code code, dansi_sgr_SetOneBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_sgr_setOneWrite(code, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_sgr_setOneWrite(dansi_sgr_Code code, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_sgr_setRaw_static("{:uhh}")), as$(u8)(code));
};

fn_((dansi_sgr_set(S_const$u16 codes, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_sgr_setWrite(codes, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_sgr_setWrite(S_const$u16 codes, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix)));
    for_(($s(codes), $rf(0))(ch, i)) {
        if (i != 0) try_(io_Writer_writeByte(out, dansi_csi_param_sep_byte));
        try_(io_Writer_print(out, u8_l("{:uh}"), *ch));
    } $end(for);
    return_ok(try_(io_Writer_writeByte(out, dansi_sgr_final_byte)));
} $unscoped(fn);
