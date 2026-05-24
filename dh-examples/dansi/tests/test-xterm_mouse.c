#include <dh-main.h>
#include <dansi-xterm.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-xterm/mouse: mode controls have static and runtime forms" $scope) {
    var_(buf, dansi_mouse_ModeBuf) $undefined;
    let enabled = dansi_mouse_enable(dansi_mouse_Mode_any_event, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enable_static("1003")), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_enable_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_any_event))), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(enabled.as_const, u8_l("\x1b[?1003h"))));

    let disabled = dansi_mouse_set(dansi_mouse_Mode_sgr, false, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mouse_set_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_sgr), 0)), u8_l("\x1b[?1006l"))));
    try_(TEST_expect(mem_eqlBytes(disabled.as_const, u8_l("\x1b[?1006l"))));
    try_(TEST_expect(mem_eqlBytes(dansi_mouse_enableAny(), u8_l("\x1b[?1003h"))));
    try_(TEST_expect(mem_eqlBytes(dansi_mouse_disableSGR(), u8_l("\x1b[?1006l"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/mouse: parse SGR press" $scope) {
    let event = try_(dansi_mouse_parseSGR(dansi_Seq_csi(u8_l("\x1b[<0;12;34M"))));
    try_(TEST_expect(event.x == 12));
    try_(TEST_expect(event.y == 34));
    try_(TEST_expect(event.button == dansi_mouse_Button_left));
    try_(TEST_expect(event.action == dansi_mouse_Action_press));
    try_(TEST_expect(event.wheel == dansi_mouse_Wheel_none));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/mouse: parse SGR release" $scope) {
    let event = try_(dansi_mouse_parseSGR(dansi_Seq_csi(u8_l("\x1b[<0;12;34m"))));
    try_(TEST_expect(event.button == dansi_mouse_Button_left));
    try_(TEST_expect(event.action == dansi_mouse_Action_release));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/mouse: parse SGR wheel with modifiers" $scope) {
    let event = try_(dansi_mouse_parseSGR(dansi_Seq_csi(u8_l("\x1b[<92;7;9M"))));
    try_(TEST_expect(event.x == 7));
    try_(TEST_expect(event.y == 9));
    try_(TEST_expect(event.button == dansi_mouse_Button_none));
    try_(TEST_expect(event.action == dansi_mouse_Action_press));
    try_(TEST_expect(event.wheel == dansi_mouse_Wheel_up));
    try_(TEST_expect(event.mods.shift == 1));
    try_(TEST_expect(event.mods.alt == 1));
    try_(TEST_expect(event.mods.ctrl == 1));
    return_ok({});
} $unscoped(TEST_fn);
