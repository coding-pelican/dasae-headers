#include <dh-main.h>
#include <dansi-core.h>

TEST_fn_("dansi-core/mouse: tracking mode controls have static and runtime forms" $scope) {
    var_(buf, dansi_mouse_TrackingModeBuf) $undefined;
    let enabled = dansi_mouse_enableTracking(dansi_mouse_TrackingMode_any_event, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enableTracking_static("1003")), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_any_event))), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_hilite))), u8_l("\x1b[?1001h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_sgr_pixels))), u8_l("\x1b[?1016h"))));
    try_(TEST_expect(mem_eqlBytes(enabled.as_const, u8_l("\x1b[?1003h"))));

    let disabled = dansi_mouse_setTracking(dansi_mouse_TrackingMode_sgr, false, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_setTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_sgr), 0)), u8_l("\x1b[?1006l"))));
    try_(TEST_expect(mem_eqlBytes(disabled.as_const, u8_l("\x1b[?1006l"))));
    try_(TEST_expect(mem_eqlBytes(dansi_mouse_enableAny(), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(dansi_mouse_disableSGR(), u8_l("\x1b[?1006l"))));
    return_ok({});
} $unscoped(TEST_fn);
