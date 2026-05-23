#include <dh-main.h>
#include <dansi-core.h>
#include <dh/io/Fixed.h>

TEST_fn_("dansi-core/screen: parse text-area size in characters" $scope) {
    let size = try_(dansi_screen_parseTextAreaSizeCharsReport(u8_l("\x1b[8;24;80t")));
    try_(TEST_expect(size.rows == 24));
    try_(TEST_expect(size.cols == 80));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/screen: parse text-area size in pixels" $scope) {
    let size = try_(dansi_screen_parseTextAreaSizePixelsReport(u8_l("\x1b[4;600;800t")));
    try_(TEST_expect(size.height == 600));
    try_(TEST_expect(size.width == 800));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/screen: parse cell size in pixels" $scope) {
    let size = try_(dansi_screen_parseCellSizePixelsReport(u8_l("\x1b[6;16;8t")));
    try_(TEST_expect(size.height == 16));
    try_(TEST_expect(size.width == 8));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/screen: receive and parse text-area size report" $scope) {
    var reader = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1b[8;30;120t")));
    var_(buf, dansi_screen_SizeReportBuf) $undefined;
    let report = try_(dansi_screen_receiveTextAreaSizeCharsReport(io_Fixed_reader(&reader), A_ref$((S$u8)(buf))));
    let size = try_(dansi_screen_parseTextAreaSizeCharsReport(report.as_const));
    try_(TEST_expect(size.rows == 30));
    try_(TEST_expect(size.cols == 120));
    return_ok({});
} $unscoped(TEST_fn);
