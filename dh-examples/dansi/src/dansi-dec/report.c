#include "dansi-dec/report.h"
#include "dansi-core/Seq.h"

$static fn_((dansi_dec_report__parse(S_const$u8 report, u16 request_code))(O$u16) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_none()));
    if (frame.final != dansi_dec_report_final_byte
        || !dansi_csi_Frame_isPrivate(frame, dansi_dec_report_private_marker_byte)) {
        return_none();
    }
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_dec_report_status_param_code))(return_none()));
    if (code < request_code) return_none();
    return_some(code);
} $unscoped(fn);

fn_((dansi_dec_report_requestPrinterStatus(void))(S_const$u8)) {
    return u8_l(dansi_dec_report_requestPrinterStatus_static());
};

fn_((dansi_dec_report_requestPrinterStatusWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_report_requestPrinterStatus());
};

fn_((dansi_dec_report_receivePrinterStatusReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_dec_report_parsePrinterStatus(S_const$u8 report))(dansi_dec_report_E$dansi_dec_report_PrinterStatus) $scope) {
    let code = orelse_((dansi_dec_report__parse(report, dansi_dec_report_PrinterStatus_ready))(
        return_err(E_cause$dansi_dec_report_InvalidResponse())
    ));
    if (code != dansi_dec_report_PrinterStatus_ready && code != dansi_dec_report_PrinterStatus_not_ready) {
        return_err(E_cause$dansi_dec_report_InvalidResponse());
    }
    return_ok(as$(dansi_dec_report_PrinterStatus)(code));
} $unscoped(fn);

fn_((dansi_dec_report_fetchPrinterStatus(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_report_E$dansi_dec_report_PrinterStatus) $scope) {
    try_(dansi_dec_report_requestPrinterStatusWrite(out));
    let report = try_(dansi_dec_report_receivePrinterStatusReport(in, buf));
    return dansi_dec_report_parsePrinterStatus(report.as_const);
} $unscoped(fn);

fn_((dansi_dec_report_requestKeyboardStatus(void))(S_const$u8)) {
    return u8_l(dansi_dec_report_requestKeyboardStatus_static());
};

fn_((dansi_dec_report_requestKeyboardStatusWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_report_requestKeyboardStatus());
};

fn_((dansi_dec_report_receiveKeyboardStatusReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_dec_report_parseKeyboardStatus(S_const$u8 report))(dansi_dec_report_E$dansi_dec_report_KeyboardStatus) $scope) {
    let code = orelse_((dansi_dec_report__parse(report, dansi_dec_report_KeyboardStatus_ready))(
        return_err(E_cause$dansi_dec_report_InvalidResponse())
    ));
    if (code != dansi_dec_report_KeyboardStatus_ready && code != dansi_dec_report_KeyboardStatus_locked) {
        return_err(E_cause$dansi_dec_report_InvalidResponse());
    }
    return_ok(as$(dansi_dec_report_KeyboardStatus)(code));
} $unscoped(fn);

fn_((dansi_dec_report_fetchKeyboardStatus(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_report_E$dansi_dec_report_KeyboardStatus) $scope) {
    try_(dansi_dec_report_requestKeyboardStatusWrite(out));
    let report = try_(dansi_dec_report_receiveKeyboardStatusReport(in, buf));
    return dansi_dec_report_parseKeyboardStatus(report.as_const);
} $unscoped(fn);
