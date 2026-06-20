#include <dh-main.h>
#include <dansi-xterm.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-xterm/mode focus paste mouse: builders and simple reports" $scope) {
    var_(mode_buf, dansi_xterm_mode_SetRawBuf) $undefined;
    let mode = dansi_xterm_mode_enableRaw(1004, &mode_buf);
    try_(TEST_expect(mem_eqlBytes(mode.as_const, u8_l("\x1b[?1004h"))));

    var_(focus_buf, dansi_xterm_focus_SetTrackingBuf) $undefined;
    let focus_on = dansi_xterm_focus_enableTracking(&focus_buf);
    try_(TEST_expect(mem_eqlBytes(focus_on.as_const, u8_l("\x1b[?1004h"))));
    try_(TEST_expect(unwrap_(dansi_xterm_focus_parseReport(u8_l("\x1b[I"))) == dansi_xterm_focus_Event_in));
    try_(TEST_expect(unwrap_(dansi_xterm_focus_parseReport(u8_l("\x1b[O"))) == dansi_xterm_focus_Event_out));

    var_(paste_buf, dansi_xterm_paste_SetBracketedBuf) $undefined;
    let paste_on = dansi_xterm_paste_enableBracketed(&paste_buf);
    try_(TEST_expect(mem_eqlBytes(paste_on.as_const, u8_l("\x1b[?2004h"))));
    try_(TEST_expect(unwrap_(dansi_xterm_paste_parseBoundary(u8_l("\x1b[200~"))) == dansi_xterm_paste_Event_begin));
    try_(TEST_expect(unwrap_(dansi_xterm_paste_parseBoundary(u8_l("\x1b[201~"))) == dansi_xterm_paste_Event_end));

    var_(mouse_buf, dansi_xterm_mouse_EnableSGRBuf) $undefined;
    let mouse_on = dansi_xterm_mouse_enableSGR(dansi_xterm_mouse_ReportMode_any_event, &mouse_buf);
    try_(TEST_expect(mem_eqlBytes(mouse_on.as_const, u8_l("\x1b[?1003h\x1b[?1006h"))));

    let report = unwrap_(dansi_xterm_mouse_parseSGRReport(u8_l("\x1b[<0;12;34M")));
    try_(TEST_expect(report.cb == 0));
    try_(TEST_expect(report.x == 12));
    try_(TEST_expect(report.y == 34));
    let event = try_(dansi_xterm_mouse_interpretSGR(report));
    try_(TEST_expect(matches(event, dansi_xterm_mouse_Event_press)));
    let press = union_to((event)(dansi_xterm_mouse_Event_press));
    try_(TEST_expect(press.btn == dansi_xterm_mouse_Btn_left));
    try_(TEST_expect(press.pos.x == 12 && press.pos.y == 34));

    let wheel = unwrap_(dansi_xterm_mouse_parseSGR(u8_l("\x1b[<65;5;6M")));
    try_(TEST_expect(matches(wheel, dansi_xterm_mouse_Event_wheel)));
    try_(TEST_expect(union_to((wheel)(dansi_xterm_mouse_Event_wheel)).wheel == dansi_xterm_mouse_Wheel_down));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/key: configuration and enhanced report parse" $scope) {
    var_(modify_buf, dansi_xterm_key_SetModifyLevelBuf) $undefined;
    let modify = dansi_xterm_key_setModifyLevel(
        dansi_xterm_key_Resrc_other_keys,
        dansi_xterm_key_ModifyLevel_other_keys,
        &modify_buf
    );
    try_(TEST_expect(mem_eqlBytes(modify.as_const, u8_l("\x1b[>4;2m"))));

    var_(format_buf, dansi_xterm_key_SetFormatBuf) $undefined;
    let format = dansi_xterm_key_setFormat(
        dansi_xterm_key_Resrc_other_keys,
        dansi_xterm_key_Format_csi_u,
        &format_buf
    );
    try_(TEST_expect(mem_eqlBytes(format.as_const, u8_l("\x1b[>4;1f"))));

    let ctrl_up = unwrap_(dansi_xterm_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1b[1;5A"))));
    try_(TEST_expect(matches(ctrl_up, dansi_xterm_key_Event_special)));
    let up = union_to((ctrl_up)(dansi_xterm_key_Event_special));
    try_(TEST_expect(up.key == dansi_xterm_key_Named_up));
    try_(TEST_expect(up.mods.ctrl));

    let csi_u = unwrap_(dansi_xterm_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1b[65;5u"))));
    try_(TEST_expect(matches(csi_u, dansi_xterm_key_Event_text)));
    let text = union_to((csi_u)(dansi_xterm_key_Event_text));
    try_(TEST_expect(text.codepoint == 65));
    try_(TEST_expect(text.mods.ctrl));

    let other = unwrap_(dansi_xterm_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1b[27;3;97~"))));
    try_(TEST_expect(matches(other, dansi_xterm_key_Event_modify_other)));
    let modify_other = union_to((other)(dansi_xterm_key_Event_modify_other));
    try_(TEST_expect(modify_other.codepoint == 97));
    try_(TEST_expect(modify_other.mods.alt));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/color palette sgr selection: builders and palette report parse" $scope) {
    var_(fg_buf, dansi_xterm_color_Fg4bitBuf) $undefined;
    let fg = dansi_xterm_color_fg4bit(dansi_xterm_Palette4bit_red, &fg_buf);
    try_(TEST_expect(mem_eqlBytes(fg.as_const, u8_l("\x1b[91m"))));

    var_(rgb_buf, dansi_xterm_color_FgRGBBuf) $undefined;
    let rgb = dansi_xterm_color_fgRGB((dansi_xterm_color_RGB8){ .r = 1, .g = 2, .b = 3 }, &rgb_buf);
    try_(TEST_expect(mem_eqlBytes(rgb.as_const, u8_l("\x1b[38;2;1;2;3m"))));

    var_(push_buf, dansi_xterm_sgr_PushBuf) $undefined;
    try_(TEST_expect(mem_eqlBytes(dansi_xterm_sgr_push(&push_buf).as_const, u8_l("\x1b[#p"))));

    var_(pal_buf, dansi_xterm_palette_SetColorBuf) $undefined;
    let set_color = dansi_xterm_palette_setColor16(2, (dansi_xterm_color_RGB16){ .r = 0x0101, .g = 0x0202, .b = 0xffff }, &pal_buf);
    try_(TEST_expect(mem_eqlBytes(set_color.as_const, u8_l("\x1b]4;2;rgb:0101/0202/ffff\x1b\\"))));

    let parsed = try_(dansi_xterm_palette_parseColorReport(
        try_(dansi_osc_parse(u8_l("\x1b]4;2;rgb:0101/0202/ffff\x1b\\")))
    ));
    try_(TEST_expect(parsed.index == 2));
    try_(TEST_expect(parsed.rgb.r == 0x0101 && parsed.rgb.g == 0x0202 && parsed.rgb.b == 0xffff));

    var_(target_buf, A$$(12, u8)) $undefined;
    let targets = try_(dansi_xterm_selection_targetsToRaw(
        (dansi_xterm_selection_Targets){ .clipboard = 1, .primary = 1 },
        A_ref$((S$u8)(target_buf))
    ));
    try_(TEST_expect(mem_eqlBytes(targets.as_const, u8_l("cp"))));

    var_(sel_buf, A$$(32, u8)) $undefined;
    let selection = try_(dansi_xterm_selection_setRaw(u8_l("cp"), u8_l("SGk="), A_ref$((S$u8)(sel_buf))));
    try_(TEST_expect(mem_eqlBytes(selection.as_const, u8_l("\x1b]52;cp;SGk=\x1b\\"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-xterm/screen window title resource: report parse surfaces" $scope) {
    try_(TEST_expect(mem_eqlBytes(dansi_xterm_screen_requestCellPixels(), u8_l("\x1b[16t"))));
    let cell = try_(dansi_xterm_screen_parseCellPixelsReport(u8_l("\x1b[6;18;9t")));
    try_(TEST_expect(cell.width == 9 && cell.height == 18));

    var_(move_buf, dansi_xterm_window_MoveBuf) $undefined;
    let move = dansi_xterm_window_move(10, 20, &move_buf);
    try_(TEST_expect(mem_eqlBytes(move.as_const, u8_l("\x1b[3;10;20t"))));
    let pos = try_(dansi_xterm_window_parsePosReport(u8_l("\x1b[3;10;20t")));
    try_(TEST_expect(pos.x == 10 && pos.y == 20));
    try_(TEST_expect(try_(dansi_xterm_window_parseStateReport(u8_l("\x1b[2t"))) == dansi_xterm_window_State_iconified));

    var_(title_buf, A$$(32, u8)) $undefined;
    let title = try_(dansi_xterm_title_setWindow(u8_l("hello"), A_ref$((S$u8)(title_buf))));
    try_(TEST_expect(mem_eqlBytes(title.as_const, u8_l("\x1b]2;hello\x1b\\"))));
    let title_report = try_(dansi_xterm_title_parseWindowTitleReport(
        try_(dansi_osc_parse(u8_l("\x1b]lhello\x1b\\")))
    ));
    try_(TEST_expect(title_report.target == dansi_xterm_title_Target_window));
    try_(TEST_expect(mem_eqlBytes(title_report.text, u8_l("hello"))));

    var_(res_buf, A$$(32, u8)) $undefined;
    let req = try_(dansi_xterm_resrc_requestTermcapRaw(u8_l("436f"), A_ref$((S$u8)(res_buf))));
    try_(TEST_expect(mem_eqlBytes(req.as_const, u8_l("\x1bP+q436f\x1b\\"))));
    let res = try_(dansi_xterm_resrc_parseTermcapResponse(
        try_(dansi_dcs_parse(u8_l("\x1bP1+r436f=6869\x1b\\")))
    ));
    try_(TEST_expect(res.valid));
    try_(TEST_expect(mem_eqlBytes(res.payload, u8_l("436f=6869"))));
    return_ok({});
} $unscoped(TEST_fn);
