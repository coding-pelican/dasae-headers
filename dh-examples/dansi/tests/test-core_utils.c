#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/utils: format CSI without numeric parameter" $scope) {
    var_(buf, dansi_utils_CSIBuf) $undefined;
    let out = dansi_utils_formatCSI(u8_l(dansi_utils_screen_clear), &buf);
    try_(TEST_expect(mem_eqlBytes(out.as_const, u8_l("\x1b[2J"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/utils: format CSI with one numeric parameter" $scope) {
    var_(buf, dansi_utils_CSI_u16Buf) $undefined;
    let out = dansi_utils_formatCSI_u16(u8_l(dansi_utils_cursor_move_dir_up), 5, &buf);
    try_(TEST_expect(mem_eqlBytes(out.as_const, u8_l("\x1b[5A"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/utils: format CSI with two numeric parameters" $scope) {
    var_(buf, dansi_utils_CSI_u16x2Buf) $undefined;
    let out = dansi_utils_formatCSI_u16x2(u8_l(dansi_utils_cursor_move_pos), 10, 20, &buf);
    try_(TEST_expect(mem_eqlBytes(out.as_const, u8_l("\x1b[10;20H"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/utils: format OSC with static and runtime forms" $scope) {
    var_(buf, A$$(32, u8)) $undefined;
    let out = dansi_utils_formatOSC(u8_l("2"), u8_l("dansi"), A_ref$((S$u8)(buf)));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_utils_formatOSC_static("2", "dansi")), u8_l("\x1b]2;dansi\x1b\\"))));
    try_(TEST_expect(mem_eqlBytes(out.as_const, u8_l("\x1b]2;dansi\x1b\\"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/utils: format DCS and raw with static and runtime forms" $scope) {
    var_(buf, A$$(32, u8)) $undefined;
    let dcs = dansi_utils_formatDCS(u8_l("payload"), A_ref$((S$u8)(buf)));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_utils_formatDCS_static("payload")), u8_l("\x1bPpayload\x1b\\"))));
    try_(TEST_expect(mem_eqlBytes(dcs.as_const, u8_l("\x1bPpayload\x1b\\"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_utils_raw_static("raw")), u8_l("raw"))));
    try_(TEST_expect(mem_eqlBytes(dansi_utils_raw(u8_l("raw")), u8_l("raw"))));
    return_ok({});
} $unscoped(TEST_fn);
