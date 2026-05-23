#include <dh-main.h>
#include <dansi-core.h>
#include <dh/io/Fixed.h>

TEST_fn_("dansi-core/cursor: parse cursor position report" $scope) {
    let pos = try_(dansi_cursor_parsePosReport(u8_l("\x1b[12;34R")));
    try_(TEST_expect(pos.row == 12));
    try_(TEST_expect(pos.col == 34));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/cursor: read cursor position report from reader" $scope) {
    var reader = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1b[7;9R")));
    var_(buf, dansi_cursor_PosReportBuf) $undefined;
    let report = try_(dansi_cursor_receivePosReport(io_Fixed_reader(&reader), A_ref$((S$u8)(buf))));
    let pos = try_(dansi_cursor_parsePosReport(report.as_const));
    try_(TEST_expect(pos.row == 7));
    try_(TEST_expect(pos.col == 9));
    return_ok({});
} $unscoped(TEST_fn);
