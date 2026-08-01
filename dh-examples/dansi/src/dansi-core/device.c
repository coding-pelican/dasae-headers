#include "dansi-core/device.h"
#include "dansi-core/Seq.h"

fn_((dansi_device_requestStatus(void))(S_const$u8)) {
    return u8_l(dansi_device_requestStatus_static());
};

fn_((dansi_device_requestStatusWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_device_requestStatus());
};

fn_((dansi_device_receiveStatusReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_device_parseStatusReport(S_const$u8 report))(dansi_device_E$dansi_device_Status) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_err(E_cause$dansi_device_InvalidResponse())));
    if (frame.final != dansi_device_status_report_final_byte) return_err(E_cause$dansi_device_InvalidResponse());

    let status = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_device_status_report_param_status))(
        return_err(E_cause$dansi_device_InvalidResponse())
    ));
    switch (status) {
    case_((dansi_device_Status_ready)) return_ok(dansi_device_Status_ready) $end(case);
    case_((dansi_device_Status_malformed)) return_ok(dansi_device_Status_malformed) $end(case);
    default_() return_err(E_cause$dansi_device_InvalidResponse()) $end(default);
    }
} $unscoped(fn);

fn_((dansi_device_fetchStatus(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Status) $scope) {
    try_(dansi_device_requestStatusWrite(out));
    let report = try_(dansi_device_receiveStatusReport(in, buf));
    return dansi_device_parseStatusReport(report.as_const);
} $unscoped(fn);

fn_((dansi_device_requestAttrs(void))(S_const$u8)) {
    return u8_l(dansi_device_requestAttrs_static());
};

fn_((dansi_device_requestAttrsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_device_requestAttrs());
};

fn_((dansi_device_receiveAttrsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_device_parseAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_err(E_cause$dansi_device_InvalidResponse())));
    if (frame.final != dansi_device_attrs_report_final_byte) return_err(E_cause$dansi_device_InvalidResponse());
    return_ok({ .params = frame.params });
} $unscoped(fn);

fn_((dansi_device_fetchAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs) $scope) {
    try_(dansi_device_requestAttrsWrite(out));
    let report = try_(dansi_device_receiveAttrsReport(in, buf));
    return dansi_device_parseAttrsReport(report.as_const);
} $unscoped(fn);
