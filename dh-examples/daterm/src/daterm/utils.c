#include "daterm/utils.h"
#include <dh/io/Fixed.h>

/*========== External Definitions ===========================================*/

fn_((daterm_utils_formatCSI(S_const$u8 code, daterm_utils_CSIBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_utils_formatCSIWrite(code, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_utils_formatCSIWrite(S_const$u8 code, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_utils_formatCSI_static("{:s}")), code);
};

fn_((daterm_utils_formatCSI_u16(S_const$u8 code, u16 val, daterm_utils_CSI_u16Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_utils_formatCSI_u16Write(code, val, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_utils_formatCSI_u16Write(S_const$u8 code, u16 val, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_utils_formatCSI_u16_static("{:s}", "{:uh}")), code, val);
};

fn_((daterm_utils_formatCSI_u16x2(S_const$u8 code, u16 val1, u16 val2, daterm_utils_CSI_u16x2Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_utils_formatCSI_u16x2Write(code, val1, val2, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_utils_formatCSI_u16x2Write(S_const$u8 code, u16 val1, u16 val2, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_utils_formatCSI_u16x2_static("{:s}", "{:uh}", "{:uh}")), code, val1, val2);
};

fn_((daterm_utils_formatCSI_u8x3(S_const$u8 code, u8 val1, u8 val2, u8 val3, daterm_utils_CSI_u8x3Buf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((daterm_utils_formatCSI_u8x3Write(code, val1, val2, val3, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((daterm_utils_formatCSI_u8x3Write(S_const$u8 code, u8 val1, u8 val2, u8 val3, io_Writer writer))(E$void)) {
    return io_Writer_print(writer, u8_l(daterm_utils_formatCSI_u8x3_static("{:s}", "{:uhh}", "{:uhh}", "{:uhh}")), code, val1, val2, val3);
};
