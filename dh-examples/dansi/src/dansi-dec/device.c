#include "dansi-dec/device.h"
#include "dansi-core/Seq.h"

fn_((dansi_dec_device_requestPrimaryAttrs(void))(S_const$u8)) {
    return u8_l(dansi_dec_device_requestPrimaryAttrs_static());
};

fn_((dansi_dec_device_requestPrimaryAttrsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_dec_device_requestPrimaryAttrs());
};

fn_((dansi_dec_device_fetchPrimaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs) $scope) {
    try_(dansi_dec_device_requestPrimaryAttrsWrite(out));
    let report = try_(dansi_dec_device_receiveAttrsReport(in, buf));
    let attrs = try_(dansi_dec_device_parseAttrsReport(report.as_const));
    if (attrs.kind != dansi_dec_device_Kind_primary) return_err(E_cause$dansi_dec_device_InvalidResponse());
    return_ok(attrs);
} $unscoped(fn);

fn_((dansi_dec_device_requestSecondaryAttrs(void))(S_const$u8)) {
    return u8_l(dansi_dec_device_requestSecondaryAttrs_static());
};

fn_((dansi_dec_device_requestSecondaryAttrsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_dec_device_requestSecondaryAttrs());
};

fn_((dansi_dec_device_fetchSecondaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs) $scope) {
    try_(dansi_dec_device_requestSecondaryAttrsWrite(out));
    let report = try_(dansi_dec_device_receiveAttrsReport(in, buf));
    let attrs = try_(dansi_dec_device_parseAttrsReport(report.as_const));
    if (attrs.kind != dansi_dec_device_Kind_secondary) return_err(E_cause$dansi_dec_device_InvalidResponse());
    return_ok(attrs);
} $unscoped(fn);

fn_((dansi_dec_device_requestTertiaryAttrs(void))(S_const$u8)) {
    return u8_l(dansi_dec_device_requestTertiaryAttrs_static());
};

fn_((dansi_dec_device_requestTertiaryAttrsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_dec_device_requestTertiaryAttrs());
};

fn_((dansi_dec_device_fetchTertiaryAttrs(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_dec_device_E$dansi_dec_device_Attrs) $scope) {
    try_(dansi_dec_device_requestTertiaryAttrsWrite(out));
    let report = try_(dansi_dec_device_receiveAttrsReport(in, buf));
    let attrs = try_(dansi_dec_device_parseAttrsReport(report.as_const));
    if (attrs.kind != dansi_dec_device_Kind_tertiary) return_err(E_cause$dansi_dec_device_InvalidResponse());
    return_ok(attrs);
} $unscoped(fn);

fn_((dansi_dec_device_identify(void))(S_const$u8)) {
    return u8_l(dansi_dec_device_identify_static());
};

fn_((dansi_dec_device_identifyWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_dec_device_identify());
};

fn_((dansi_dec_device_receiveAttrsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_dec_device_parseAttrsReport(S_const$u8 report))(dansi_dec_device_E$dansi_dec_device_Attrs) $scope) {
    let frame = catch_((dansi_csi_parse(report))($ignore, return_err(E_cause$dansi_dec_device_InvalidResponse())));
    if (frame.final != dansi_dec_device_attrs_report_final_byte) return_err(E_cause$dansi_dec_device_InvalidResponse());
    if (frame.params.len == 0) return_err(E_cause$dansi_dec_device_InvalidResponse());

    let first = *S_at((frame.params)[0]);
    if (first == dansi_dec_device_primary_marker_byte) {
        return_ok({ .kind = dansi_dec_device_Kind_primary, .params = frame.params });
    }
    if (first == dansi_dec_device_secondary_marker_byte) {
        return_ok({ .kind = dansi_dec_device_Kind_secondary, .params = frame.params });
    }
    if (first == dansi_dec_device_tertiary_marker_byte) {
        return_ok({ .kind = dansi_dec_device_Kind_tertiary, .params = frame.params });
    }
    return_err(E_cause$dansi_dec_device_InvalidResponse());
} $unscoped(fn);
