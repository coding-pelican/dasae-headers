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
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_attr_push_static()), u8_l("\x1b[#p"))));
    try_(TEST_expect(mem_eqlBytes(dansi_attr_pop(), u8_l("\x1b[#q"))));
    var_(report_buf, dansi_attr_ReportRectBuf) $undefined;
    let report = dansi_attr_reportRect(1, 2, 3, 4, &report_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_attr_reportRect_static("1", "2", "3", "4")), u8_l("\x1b[1;2;3;4#|"))));
    try_(TEST_expect(mem_eqlBytes(report.as_const, u8_l("\x1b[1;2;3;4#|"))));
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

TEST_fn_("dansi-core/window: xterm window ops have static and runtime forms" $scope) {
    var_(buf, dansi_window_Buf) $undefined;
    let move = dansi_window_move(12, 34, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_op_static("1")), u8_l("\x1b[1t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_move_static("12", "34")), u8_l("\x1b[3;12;34t"))));
    try_(TEST_expect(mem_eqlBytes(move.as_const, u8_l("\x1b[3;12;34t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_resizePixels_static("480", "640")), u8_l("\x1b[4;480;640t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_resizeChars_static("24", "80")), u8_l("\x1b[8;24;80t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_maximize_static("1")), u8_l("\x1b[9;1t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_window_fullscreen_static("2")), u8_l("\x1b[10;2t"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/palette: xterm palette stack controls have static and runtime forms" $scope) {
    var_(buf, dansi_palette_StackBuf) $undefined;
    let push = dansi_palette_push(2, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_palette_push_static()), u8_l("\x1b[#P"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_palette_pushAt_static("2")), u8_l("\x1b[2#P"))));
    try_(TEST_expect(mem_eqlBytes(push.as_const, u8_l("\x1b[2#P"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_palette_pop_static()), u8_l("\x1b[#Q"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_palette_reportStackAt_static("2")), u8_l("\x1b[2#R"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/graphics: xterm graphics attributes have static and runtime forms" $scope) {
    var_(buf, dansi_graphics_Buf) $undefined;
    let read = dansi_graphics_read(dansi_graphics_Item_color_registers, &buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_graphics_read_static("1")), u8_l("\x1b[?1;1;0S"))));
    try_(TEST_expect(mem_eqlBytes(read.as_const, u8_l("\x1b[?1;1;0S"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_graphics_reset_static("2")), u8_l("\x1b[?2;2;0S"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_graphics_readMax_static("3")), u8_l("\x1b[?3;4;0S"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_graphics_setColorRegisters_static("256")), u8_l("\x1b[?1;3;256S"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_graphics_setGeometry_static("2", "640", "480")), u8_l("\x1b[?2;3;640;480S"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/resource: xterm resource and termcap requests encode names" $scope) {
    var_(buf, A$$(64, u8)) $undefined;
    let resource = dansi_resource_requestValues(u8_l("allowWindowOps"), A_ref$((S$u8)(buf)));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_resource_requestValues_static("616C6C6F7757696E646F774F7073")), u8_l("\x1bP+Q616C6C6F7757696E646F774F7073\x1b\\"))));
    try_(TEST_expect(mem_eqlBytes(resource.as_const, u8_l("\x1bP+Q616C6C6F7757696E646F774F7073\x1b\\"))));
    let termcap = dansi_resource_requestTermcap(u8_l("Co;RGB"), A_ref$((S$u8)(buf)));
    try_(TEST_expect(mem_eqlBytes(termcap.as_const, u8_l("\x1bP+q436F;524742\x1b\\"))));
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
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_enablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_alt_scroll))), u8_l("\x1b[?1007h"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_enablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_meta_sends_esc))), u8_l("\x1b[?1036h"))));
    let private_disabled = dansi_mode_setPrivate(as$(u16)(dansi_mode_Private_bracketed_paste), false, &private_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setPrivate_static("2004", 0)), u8_l("\x1b[?2004l"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setPrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_bracketed_paste), 0)), u8_l("\x1b[?2004l"))));
    try_(TEST_expect(mem_eqlBytes(private_disabled.as_const, u8_l("\x1b[?2004l"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_savePrivate_static("2004")), u8_l("\x1b[?2004s"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_restorePrivate_static("2004")), u8_l("\x1b[?2004r"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_requestPrivate_static("2004")), u8_l("\x1b[?2004$p"))));

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
