#include "dansi-core/utils.h"
#include <dh/io/Fixed.h>

/*========== External Definitions ===========================================*/

fn_((dansi_utils_formatCSI(S_const$u8 code, dansi_utils_CSIBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_utils_formatCSIWrite(code, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatCSIWrite(S_const$u8 code, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_formatCSI_static("{:s}")), code);
};

fn_((dansi_utils_formatCSI_u16(S_const$u8 code, u16 val, dansi_utils_CSI_u16Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_utils_formatCSI_u16Write(code, val, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatCSI_u16Write(S_const$u8 code, u16 val, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_formatCSI_u16_static("{:s}", "{:uh}")), val, code);
};

fn_((dansi_utils_formatCSI_u16x2(S_const$u8 code, u16 val1, u16 val2, dansi_utils_CSI_u16x2Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_utils_formatCSI_u16x2Write(code, val1, val2, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatCSI_u16x2Write(S_const$u8 code, u16 val1, u16 val2, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_formatCSI_u16x2_static("{:s}", "{:uh}", "{:uh}")), val1, val2, code);
};

fn_((dansi_utils_formatCSI_u8x3(S_const$u8 code, u8 val1, u8 val2, u8 val3, dansi_utils_CSI_u8x3Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_utils_formatCSI_u8x3Write(code, val1, val2, val3, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatCSI_u8x3Write(S_const$u8 code, u8 val1, u8 val2, u8 val3, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_formatCSI_u8x3_static("{:s}", "{:uhh}", "{:uhh}", "{:uhh}")), val1, val2, val3, code);
};

fn_((dansi_utils_formatOSC(S_const$u8 code, S_const$u8 payload, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_utils_formatOSCWrite(code, payload, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatOSCWrite(S_const$u8 code, S_const$u8 payload, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_osc "{:s};{:s}" dansi_utils_st), code, payload);
};

fn_((dansi_utils_formatDCS(S_const$u8 payload, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_utils_formatDCSWrite(payload, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_utils_formatDCSWrite(S_const$u8 payload, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(dansi_utils_dcs "{:s}" dansi_utils_st), payload);
};

fn_((dansi_utils_raw(S_const$u8 bytes))(S_const$u8)) {
    return bytes;
};

fn_((dansi_utils_rawWrite(S_const$u8 bytes, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, bytes);
};
