#include <dh-main.h>
#include <dansi-core.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>
#include "test-support.h"

TEST_fn_("dansi-core/cursor: parse cursor position report" $scope) {
    try_(TEST_expect(mem_eqlBytes(dansi_cursor_requestPos(), u8_l(dansi_cursor_requestPos_static()))));

    let pos = try_(dansi_cursor_parsePosReport(u8_l("\x1b[12;34R")));
    try_(TEST_expect(pos.row == 12));
    try_(TEST_expect(pos.col == 34));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/cursor: receive split cursor position report" $scope) {
    var_(reader, dansi_test_ChunkReader) = {
        .bytes = u8_l("\x1b[7;9R"),
        .pos = 0,
        .chunk = 1,
    };
    var_(buf, dansi_cursor_PosReportBuf) $undefined;
    let report = try_(dansi_cursor_receivePosReport(dansi_test_ChunkReader_reader(&reader), A_ref$((S$u8)(buf))));
    let pos = try_(dansi_cursor_parsePosReport(report.as_const));
    try_(TEST_expect(pos.row == 7));
    try_(TEST_expect(pos.col == 9));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/device: parse status and attrs reports" $scope) {
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestStatus(), u8_l(dansi_device_requestStatus_static()))));
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestAttrs(), u8_l(dansi_device_requestAttrs_static()))));

    let ready = try_(dansi_device_parseStatusReport(u8_l("\x1b[0n")));
    try_(TEST_expect(ready == dansi_device_Status_ready));
    let malformed = try_(dansi_device_parseStatusReport(u8_l("\x1b[3n")));
    try_(TEST_expect(malformed == dansi_device_Status_malformed));

    let attrs = try_(dansi_device_parseAttrsReport(u8_l("\x1b[?1;2c")));
    try_(TEST_expect(mem_eqlBytes(attrs.params, u8_l("?1;2"))));
    return_ok({});
} $unscoped(TEST_fn);
