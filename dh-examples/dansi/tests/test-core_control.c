#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/attr: selective resets have static and runtime forms" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_attr_resetFG_static()), u8_l("\x1b[39m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetFG(), u8_l("\x1b[39m"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_attr_resetBG_static()), u8_l("\x1b[49m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetBG(), u8_l("\x1b[49m"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_attr_resetIntensity_static()), u8_l("\x1b[22m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetIntensity(), u8_l("\x1b[22m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetItalic(), u8_l("\x1b[23m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetUnderline(), u8_l("\x1b[24m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetBlinking(), u8_l("\x1b[25m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetReverse(), u8_l("\x1b[27m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetInvisible(), u8_l("\x1b[28m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_resetStrikethrough(), u8_l("\x1b[29m"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/cursor: absolute and line movement have static and runtime forms" $scope) {
    var_(buf, dansi_cursor_MoveDirBuf) $undefined;
    let row = dansi_cursor_moveToRow(12, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_moveToRow_static("12")), u8_l("\x1b[12d"))));
    try_(TEST_expect(mem_eqlBytes(row.as_const, u8_l("\x1b[12d"))));

    let col = dansi_cursor_moveToCol(34, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_moveToCol_static("34")), u8_l("\x1b[34G"))));
    try_(TEST_expect(mem_eqlBytes(col.as_const, u8_l("\x1b[34G"))));

    let next = dansi_cursor_moveNextLine(2, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_moveNextLine_static("2")), u8_l("\x1b[2E"))));
    try_(TEST_expect(mem_eqlBytes(next.as_const, u8_l("\x1b[2E"))));

    let prev = dansi_cursor_movePrevLine(3, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_movePrevLine_static("3")), u8_l("\x1b[3F"))));
    try_(TEST_expect(mem_eqlBytes(prev.as_const, u8_l("\x1b[3F"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/cursor: style and tab controls have static and runtime forms" $scope) {
    var_(style_buf, dansi_cursor_StyleBuf) $undefined;
    let style = dansi_cursor_setStyle(dansi_cursor_Style_bar, &style_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_setStyle_static("6")), u8_l("\x1b[6 q"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_setStyle_static(dansi_cursor_Style_staticParse(dansi_cursor_Style_bar))), u8_l("\x1b[6 q"))));
    try_(TEST_expect(mem_eqlBytes(style.as_const, u8_l("\x1b[6 q"))));
    try_(TEST_expect(mem_eqlBytes(dansi_cursor_setTabStop(), u8_l("\x1b[H"))));
    try_(TEST_expect(mem_eqlBytes(dansi_cursor_clearTabStop(), u8_l("\x1b[0g"))));
    try_(TEST_expect(mem_eqlBytes(dansi_cursor_clearAllTabStops(), u8_l("\x1b[3g"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/title: title controls have static and runtime forms" $scope) {
    var_(title_buf, A$$(32, u8)) $undefined;
    let title = dansi_title_setWindow(u8_l("dansi"), A_ref$((S$u8)(title_buf)));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_title_setWindow_static("dansi")), u8_l("\x1b]2;dansi\x1b\\"))));
    try_(TEST_expect(mem_eqlBytes(title.as_const, u8_l("\x1b]2;dansi\x1b\\"))));

    var_(stack_buf, dansi_title_StackBuf) $undefined;
    let push = dansi_title_push(dansi_title_StackTarget_window, &stack_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_title_push_static("2")), u8_l("\x1b[22;2t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_title_push_static(dansi_title_StackTarget_staticParse(dansi_title_StackTarget_window))), u8_l("\x1b[22;2t"))));
    try_(TEST_expect(mem_eqlBytes(push.as_const, u8_l("\x1b[22;2t"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/scroll: region controls have static and runtime forms" $scope) {
    var_(buf, dansi_scroll_RegionBuf) $undefined;
    let region = dansi_scroll_setRegion(2, 20, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_scroll_setRegion_static("2", "20")), u8_l("\x1b[2;20r"))));
    try_(TEST_expect(mem_eqlBytes(region.as_const, u8_l("\x1b[2;20r"))));
    try_(TEST_expect(mem_eqlBytes(dansi_scroll_resetRegion(), u8_l("\x1b[r"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/mode: mode controls have static and runtime forms" $scope) {
    var_(private_buf, dansi_mode_PrivateBuf) $undefined;
    let private_enabled = dansi_mode_enablePrivate(as$(u16)(dansi_mode_Private_bracketed_paste), &private_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_enablePrivate_static("2004")), u8_l("\x1b[?2004h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_enablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_bracketed_paste))), u8_l("\x1b[?2004h"))));
    try_(TEST_expect(mem_eqlBytes(private_enabled.as_const, u8_l("\x1b[?2004h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_enablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_focus_events))), u8_l("\x1b[?1004h"))));
    let private_disabled = dansi_mode_setPrivate(as$(u16)(dansi_mode_Private_bracketed_paste), false, &private_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setPrivate_static("2004", 0)), u8_l("\x1b[?2004l"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setPrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_bracketed_paste), 0)), u8_l("\x1b[?2004l"))));
    try_(TEST_expect(mem_eqlBytes(private_disabled.as_const, u8_l("\x1b[?2004l"))));

    var_(ansi_buf, dansi_mode_ANSIBuf) $undefined;
    let ansi_disabled = dansi_mode_disableANSI(as$(u16)(dansi_mode_ANSI_insert), &ansi_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_disableANSI_static("4")), u8_l("\x1b[4l"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_disableANSI_static(dansi_mode_ANSI_staticParse(dansi_mode_ANSI_insert))), u8_l("\x1b[4l"))));
    try_(TEST_expect(mem_eqlBytes(ansi_disabled.as_const, u8_l("\x1b[4l"))));
    let ansi_enabled = dansi_mode_setANSI(as$(u16)(dansi_mode_ANSI_insert), true, &ansi_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setANSI_static("4", pp_true)), u8_l("\x1b[4h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setANSI_static(dansi_mode_ANSI_staticParse(dansi_mode_ANSI_insert), pp_true)), u8_l("\x1b[4h"))));
    try_(TEST_expect(mem_eqlBytes(ansi_enabled.as_const, u8_l("\x1b[4h"))));
    return_ok({});
} $unscoped(TEST_fn);
