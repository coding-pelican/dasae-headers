#include "dansi-core/device.h"
#include <dh/mem/common.h>

$static fn_((dansi_device__receiveReport(io_Reader in, S$u8 buf))(E$S$u8) $scope) {
    let data = S_prefix((buf)(try_(io_Reader_read(in, buf))));
    return_ok(data);
} $unscoped(fn);

$static fn_((dansi_device__parseAttributesReport(
    S_const$u8 report,
    dansi_device_AttrsKind expected
))(dansi_device_E$dansi_device_Attrs) $scope) {
    if (!mem_startsWithBytes(report, u8_l(dansi_utils_csi))) {
        return_err(E_cause$dansi_device_InvalidResponse());
    }
    if (report.len <= u8_l(dansi_utils_csi).len || *S_at((report)[report.len - 1]) != 'c') {
        return_err(E_cause$dansi_device_InvalidResponse());
    }

    let payload = S_prefix((S_suffix((report)(u8_l(dansi_utils_csi).len)))(report.len - u8_l(dansi_utils_csi).len - 1));
    if (payload.len == 0) {
        return_err(E_cause$dansi_device_InvalidResponse());
    }

    let first = *S_at((payload)[0]);
    let_(kind, dansi_device_AttrsKind) = first == '>'
        ? dansi_device_AttrsKind_sec
        : first == '='
            ? dansi_device_AttrsKind_ter
            : dansi_device_AttrsKind_pri;
    if (kind != expected) {
        return_err(E_cause$dansi_device_InvalidResponse());
    }

    return_ok({
        .kind = kind,
        .params = payload,
    });
} $unscoped(fn);

fn_((dansi_device_requestStatus(void))(S_const$u8)) {
    return u8_l(dansi_device_requestStatus_static());
};

fn_((dansi_device_requestStatusWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_device_requestStatus());
};

fn_((dansi_device_receiveStatusReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_device__receiveReport(in, buf);
};

fn_((dansi_device_parseStatusReport(S_const$u8 report))(dansi_device_E$dansi_device_Status) $scope) {
    if (mem_eqlBytes(report, u8_l(dansi_utils_csi dansi_utils_device_response_status_ok))) {
        return_ok(dansi_device_Status_ok);
    }
    if (mem_eqlBytes(report, u8_l(dansi_utils_csi dansi_utils_device_response_status_malformed))) {
        return_ok(dansi_device_Status_malformed);
    }
    return_err(E_cause$dansi_device_InvalidResponse());
} $unscoped(fn);

fn_((dansi_device_fetchStatus(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Status) $scope) {
    try_(dansi_device_requestStatusWrite(out));
    let report = try_(dansi_device_receiveStatusReport(in, buf));
    return dansi_device_parseStatusReport(report.as_const);
} $unscoped(fn);

fn_((dansi_device_requestPriAttrs(void))(S_const$u8)) {
    return u8_l(dansi_device_requestPriAttrs_static());
};

fn_((dansi_device_requestPriAttrsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_device_requestPriAttrs());
};

fn_((dansi_device_receivePriAttrsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_device__receiveReport(in, buf);
};

fn_((dansi_device_parsePriAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs)) {
    return dansi_device__parseAttributesReport(report, dansi_device_AttrsKind_pri);
};

fn_((dansi_device_fetchPriAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs) $scope) {
    try_(dansi_device_requestPriAttrsWrite(out));
    let report = try_(dansi_device_receivePriAttrsReport(in, buf));
    return dansi_device_parsePriAttrsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_device_requestSecAttrs(void))(S_const$u8)) {
    return u8_l(dansi_device_requestSecAttrs_static());
};

fn_((dansi_device_requestSecAttrsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_device_requestSecAttrs());
};

fn_((dansi_device_receiveSecAttrsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_device__receiveReport(in, buf);
};

fn_((dansi_device_parseSecAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs)) {
    return dansi_device__parseAttributesReport(report, dansi_device_AttrsKind_sec);
};

fn_((dansi_device_fetchSecAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs) $scope) {
    try_(dansi_device_requestSecAttrsWrite(out));
    let report = try_(dansi_device_receiveSecAttrsReport(in, buf));
    return dansi_device_parseSecAttrsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_device_requestTerAttrs(void))(S_const$u8)) {
    return u8_l(dansi_device_requestTerAttrs_static());
};

fn_((dansi_device_requestTerAttrsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_device_requestTerAttrs());
};

fn_((dansi_device_receiveTerAttrsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_device__receiveReport(in, buf);
};

fn_((dansi_device_parseTerAttrsReport(S_const$u8 report))(dansi_device_E$dansi_device_Attrs)) {
    return dansi_device__parseAttributesReport(report, dansi_device_AttrsKind_ter);
};

fn_((dansi_device_fetchTerAttrs(io_Writer out, io_Reader in, S$u8 buf))(dansi_device_E$dansi_device_Attrs) $scope) {
    try_(dansi_device_requestTerAttrsWrite(out));
    let report = try_(dansi_device_receiveTerAttrsReport(in, buf));
    return dansi_device_parseTerAttrsReport(report.as_const);
} $unscoped(fn);
