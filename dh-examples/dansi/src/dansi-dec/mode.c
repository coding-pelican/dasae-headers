#include "dansi-dec/mode.h"
#include "dansi-core/Seq.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>

fn_((dansi_dec_mode_set(dansi_dec_mode_Code mode, bool enabled, dansi_dec_mode_SetBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_setWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_setWrite(dansi_dec_mode_Code mode, bool enabled, io_Writer out))(E$void)) {
    return enabled ? dansi_dec_mode_enableWrite(mode, out)
                   : dansi_dec_mode_disableWrite(mode, out);
};

fn_((dansi_dec_mode_enable(dansi_dec_mode_Code mode, dansi_dec_mode_EnableBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_enableWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_enableWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_makePrivate1_static("{:uhh}", "h")), as$(u8)(mode));
};

fn_((dansi_dec_mode_disable(dansi_dec_mode_Code mode, dansi_dec_mode_DisableBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_disableWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_disableWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_makePrivate1_static("{:uhh}", "l")), as$(u8)(mode));
};

fn_((dansi_dec_mode_save(dansi_dec_mode_Code mode, dansi_dec_mode_SaveBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_saveWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_saveWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_makePrivate1_static("{:uhh}", "s")), as$(u8)(mode));
};

fn_((dansi_dec_mode_restore(dansi_dec_mode_Code mode, dansi_dec_mode_RestoreBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_restoreWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_restoreWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_makePrivate1_static("{:uhh}", "r")), as$(u8)(mode));
};

fn_((dansi_dec_mode_request(dansi_dec_mode_Code mode, dansi_dec_mode_RequestBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_mode_requestWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_mode_requestWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void)) {
    return io_Writer_print(out, u8_l(dansi_csi_make_static("?{:uhh}", "$", "p")), as$(u8)(mode));
};

fn_((dansi_dec_mode_receiveReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_dec_mode_parseReport(S_const$u8 report))(dansi_dec_mode_E$dansi_dec_mode_Report) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_err(E_cause$dansi_dec_mode_InvalidResponse())));
    if (frame.final != u8_c('y') || !dansi_csi_Frame_isPrivate(frame, u8_c('?'))) {
        return_err(E_cause$dansi_dec_mode_InvalidResponse());
    }
    if (frame.intermediates.len != 1 || *S_at((frame.intermediates)[0]) != u8_c('$')) {
        return_err(E_cause$dansi_dec_mode_InvalidResponse());
    }

    let mode = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 0))(return_err(E_cause$dansi_dec_mode_InvalidResponse())));
    let status_raw = orelse_((dansi_csi_Frame_paramAtAsU16(frame, 1))(return_err(E_cause$dansi_dec_mode_InvalidResponse())));
    if (status_raw > as$(u16)(dansi_dec_mode_Status_permanently_reset)) {
        return_err(E_cause$dansi_dec_mode_InvalidResponse());
    }
    return_ok({
        .mode = mode,
        .status = as$(dansi_dec_mode_Status)(status_raw),
    });
} $unscoped(fn);

fn_((dansi_dec_mode_fetch(
    io_Writer out, io_Reader in, dansi_dec_mode_Code mode, S$u8 buf
))(dansi_dec_mode_E$dansi_dec_mode_Report) $scope) {
    try_(dansi_dec_mode_requestWrite(mode, out));
    let report = try_(dansi_dec_mode_receiveReport(in, buf));
    return dansi_dec_mode_parseReport(report.as_const);
} $unscoped(fn);
