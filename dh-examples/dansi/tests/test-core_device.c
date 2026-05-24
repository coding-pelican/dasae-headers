#include <dh-main.h>
#include <dansi-core.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>
#include "test-support.h"

TEST_fn_("dansi-core/device: parse status report" $scope) {
    let ok = try_(dansi_device_parseStatusReport(u8_l("\x1b[0n")));
    try_(TEST_expect(ok == dansi_device_Status_ok));
    let malformed = try_(dansi_device_parseStatusReport(u8_l("\x1b[3n")));
    try_(TEST_expect(malformed == dansi_device_Status_malformed));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/device: receive and parse status report" $scope) {
    var reader = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1b[0n")));
    var_(buf, dansi_device_StatusReportBuf) $undefined;
    let report = try_(dansi_device_receiveStatusReport(io_Fixed_reader(&reader), A_ref$((S$u8)(buf))));
    let status = try_(dansi_device_parseStatusReport(report.as_const));
    try_(TEST_expect(status == dansi_device_Status_ok));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/device: receive status report split across reads" $scope) {
    var reader = (dansi_test_ChunkReader){
        .bytes = u8_l("\x1b[0n"),
        .pos = 0,
        .chunk = 1,
    };
    var_(buf, dansi_device_StatusReportBuf) $undefined;
    let report = try_(dansi_device_receiveStatusReport(dansi_test_ChunkReader_reader(&reader), A_ref$((S$u8)(buf))));
    let status = try_(dansi_device_parseStatusReport(report.as_const));
    try_(TEST_expect(status == dansi_device_Status_ok));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/device: parse device attributes reports" $scope) {
    let primary = try_(dansi_device_parsePriAttrsReport(u8_l("\x1b[?1;2c")));
    try_(TEST_expect(primary.kind == dansi_device_AttrsKind_pri));
    try_(TEST_expect(mem_eqlBytes(primary.params, u8_l("?1;2"))));

    let secondary = try_(dansi_device_parseSecAttrsReport(u8_l("\x1b[>0;95;0c")));
    try_(TEST_expect(secondary.kind == dansi_device_AttrsKind_sec));
    try_(TEST_expect(mem_eqlBytes(secondary.params, u8_l(">0;95;0"))));

    let tertiary = try_(dansi_device_parseTerAttrsReport(u8_l("\x1b[=1;2c")));
    try_(TEST_expect(tertiary.kind == dansi_device_AttrsKind_ter));
    try_(TEST_expect(mem_eqlBytes(tertiary.params, u8_l("=1;2"))));
    return_ok({});
} $unscoped(TEST_fn);
