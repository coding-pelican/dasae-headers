#include <dh-main.h>
#include <dansi-dec.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-dec/mode: static runtime and report parse" $scope) {
    var_(set_buf, dansi_dec_mode_SetBuf) $undefined;
    let enabled = dansi_dec_mode_enable(dansi_dec_mode_Code_text_cursor, &set_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_mode_enable_static(dansi_dec_mode_Code_text_cursor)), u8_l("\x1b[?25h"))));
    try_(TEST_expect(mem_eqlBytes(enabled.as_const, u8_l("\x1b[?25h"))));

    var_(req_buf, dansi_dec_mode_RequestBuf) $undefined;
    let request = dansi_dec_mode_request(dansi_dec_mode_Code_text_cursor, &req_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_mode_request_static(dansi_dec_mode_Code_text_cursor)), u8_l("\x1b[?25$p"))));
    try_(TEST_expect(mem_eqlBytes(request.as_const, u8_l("\x1b[?25$p"))));

    let report = try_(dansi_dec_mode_parseReport(u8_l("\x1b[?25;1$y")));
    try_(TEST_expect(report.mode == 25));
    try_(TEST_expect(report.status == dansi_dec_mode_Status_set));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-dec/cursor scroll charset: static and runtime forms match" $scope) {
    var_(style_buf, dansi_dec_cursor_SetStyleBuf) $undefined;
    let style = dansi_dec_cursor_setStyle(dansi_dec_cursor_Style_bar, &style_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_cursor_setStyle_static(dansi_dec_cursor_Style_bar)), u8_l("\x1b[6 q"))));
    try_(TEST_expect(mem_eqlBytes(style.as_const, u8_l("\x1b[6 q"))));
    try_(TEST_expect(mem_eqlBytes(dansi_dec_cursor_save(), u8_l("\x1b""7"))));
    try_(TEST_expect(mem_eqlBytes(dansi_dec_cursor_restore(), u8_l("\x1b""8"))));

    var_(scroll_buf, dansi_dec_scroll_SetMarginsBuf) $undefined;
    let margins = dansi_dec_scroll_setMargins(2, 20, &scroll_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_scroll_setMargins_static("2", "20")), u8_l("\x1b[2;20r"))));
    try_(TEST_expect(mem_eqlBytes(margins.as_const, u8_l("\x1b[2;20r"))));
    try_(TEST_expect(mem_eqlBytes(dansi_dec_scroll_resetMargins(), u8_l("\x1b[r"))));

    var_(charset_buf, dansi_dec_charset_DesignateBuf) $undefined;
    let charset = dansi_dec_charset_designate(
        dansi_dec_charset_Slot_g0,
        dansi_dec_charset_Set_special_graphics,
        &charset_buf
    );
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_charset_designate_static(dansi_dec_charset_Slot_g0, dansi_dec_charset_Set_special_graphics)), u8_l("\x1b(0"))));
    try_(TEST_expect(mem_eqlBytes(charset.as_const, u8_l("\x1b(0"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-dec/device: parse attrs variants" $scope) {
    let primary = try_(dansi_dec_device_parseAttrsReport(u8_l("\x1b[?1;2c")));
    try_(TEST_expect(primary.kind == dansi_dec_device_Kind_primary));
    try_(TEST_expect(mem_eqlBytes(primary.params, u8_l("?1;2"))));

    let secondary = try_(dansi_dec_device_parseAttrsReport(u8_l("\x1b[>0;95;c")));
    try_(TEST_expect(secondary.kind == dansi_dec_device_Kind_secondary));
    try_(TEST_expect(mem_eqlBytes(secondary.params, u8_l(">0;95;"))));

    let tertiary = try_(dansi_dec_device_parseAttrsReport(u8_l("\x1b[=123c")));
    try_(TEST_expect(tertiary.kind == dansi_dec_device_Kind_tertiary));
    try_(TEST_expect(mem_eqlBytes(tertiary.params, u8_l("=123"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-dec/key: modes and VT key reports" $scope) {
    var_(cursor_buf, dansi_dec_key_SetCursorBuf) $undefined;
    let cursor = dansi_dec_key_enableCursor(&cursor_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_key_enableCursor_static()), u8_l("\x1b[?1h"))));
    try_(TEST_expect(mem_eqlBytes(cursor.as_const, u8_l("\x1b[?1h"))));

    var_(keypad_buf, dansi_dec_key_SetKeypadBuf) $undefined;
    let keypad = dansi_dec_key_setKeypad(true, &keypad_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_dec_key_setKeypad_static(1)), u8_l("\x1b="))));
    try_(TEST_expect(mem_eqlBytes(keypad.as_const, u8_l("\x1b="))));

    let up = unwrap_(dansi_dec_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1b[A"))));
    try_(TEST_expect(matches(up, dansi_dec_key_Event_named)));
    try_(TEST_expect(union_to((up)(dansi_dec_key_Event_named)) == dansi_dec_key_Named_up));

    let pf2 = unwrap_(dansi_dec_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_ss3, u8_l("\x1bOQ"))));
    try_(TEST_expect(matches(pf2, dansi_dec_key_Event_named)));
    try_(TEST_expect(union_to((pf2)(dansi_dec_key_Event_named)) == dansi_dec_key_Named_pf2));

    let keypad_1 = unwrap_(dansi_dec_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_ss3, u8_l("\x1bOq"))));
    try_(TEST_expect(matches(keypad_1, dansi_dec_key_Event_keypad)));
    try_(TEST_expect(union_to((keypad_1)(dansi_dec_key_Event_keypad)) == dansi_dec_key_Keypad_1));

    let f6 = unwrap_(dansi_dec_key_parseEvent(dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1b[17~"))));
    try_(TEST_expect(matches(f6, dansi_dec_key_Event_named)));
    try_(TEST_expect(union_to((f6)(dansi_dec_key_Event_named)) == dansi_dec_key_Named_f6));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-dec/report and model: request parse and feature table" $scope) {
    try_(TEST_expect(mem_eqlBytes(dansi_dec_report_requestPrinterStatus(), u8_l("\x1b[?15n"))));
    try_(TEST_expect(mem_eqlBytes(dansi_dec_report_requestKeyboardStatus(), u8_l("\x1b[?26n"))));

    let printer = try_(dansi_dec_report_parsePrinterStatus(u8_l("\x1b[?10n")));
    try_(TEST_expect(printer == dansi_dec_report_PrinterStatus_ready));
    let keyboard = try_(dansi_dec_report_parseKeyboardStatus(u8_l("\x1b[?21n")));
    try_(TEST_expect(keyboard == dansi_dec_report_KeyboardStatus_locked));

    let vt100 = dansi_dec_model_feats(dansi_dec_model_Term_vt100);
    try_(TEST_expect(!vt100.has_csi_8bit));
    let vt320 = dansi_dec_model_feats(dansi_dec_model_Term_vt320);
    try_(TEST_expect(vt320.has_csi_8bit));
    try_(TEST_expect(vt320.has_left_right_margin));
    return_ok({});
} $unscoped(TEST_fn);
