#include <dh-main.h>
#include <dansi-core.h>

TEST_fn_("dansi-core/focus: tracking mode controls have static and runtime forms" $scope) {
    var_(enable_buf, dansi_focus_TrackingBuf) $undefined;
    var_(disable_buf, dansi_focus_TrackingBuf) $undefined;
    let enabled = dansi_focus_enableTracking(&enable_buf);
    let disabled = dansi_focus_disableTracking(&disable_buf);

    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_focus_enableTracking_static()), u8_l("\x1b[?1004h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_focus_disableTracking_static()), u8_l("\x1b[?1004l"))));
    try_(TEST_expect(mem_eqlBytes(enabled.as_const, u8_l("\x1b[?1004h"))));
    try_(TEST_expect(mem_eqlBytes(disabled.as_const, u8_l("\x1b[?1004l"))));
    return_ok({});
} $unscoped(TEST_fn);
