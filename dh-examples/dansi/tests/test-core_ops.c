#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/sgr: static and runtime forms match" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_sgr_reset_static()), u8_l("\x1b[0m"))));
    try_(TEST_expect(mem_eqlBytes(dansi_sgr_reset(), u8_l("\x1b[0m"))));

    var_(one_buf, dansi_sgr_SetOneBuf) $undefined;
    let bold = dansi_sgr_setOne(dansi_sgr_Code_bold, &one_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_sgr_setOne_static(dansi_sgr_Code_bold)), u8_l("\x1b[1m"))));
    try_(TEST_expect(mem_eqlBytes(bold.as_const, u8_l("\x1b[1m"))));

    var_(raw_buf, A$$(16, u8)) $undefined;
    let raw = try_(dansi_sgr_setRaw(u8_l("31;44"), A_ref$((S$u8)(raw_buf))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_sgr_setRaw_static("31;44")), u8_l("\x1b[31;44m"))));
    try_(TEST_expect(mem_eqlBytes(raw.as_const, u8_l("\x1b[31;44m"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/style and color: convenience APIs use SGR codes" $scope) {
    var_(style_buf, dansi_style_ItalicBuf) $undefined;
    let italic = dansi_style_italic(true, &style_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_style_italic_static(1)), u8_l("\x1b[3m"))));
    try_(TEST_expect(mem_eqlBytes(italic.as_const, u8_l("\x1b[3m"))));

    var_(reset_buf, dansi_style_ResetUnderlineBuf) $undefined;
    let reset = dansi_style_resetUnderline(&reset_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_style_resetUnderline_static()), u8_l("\x1b[24m"))));
    try_(TEST_expect(mem_eqlBytes(reset.as_const, u8_l("\x1b[24m"))));

    var_(fg_buf, dansi_color_FgBuf) $undefined;
    let fg = dansi_color_fg(dansi_color_Std_red, &fg_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_color_fg_static(dansi_color_Std_red)), u8_l("\x1b[31m"))));
    try_(TEST_expect(mem_eqlBytes(fg.as_const, u8_l("\x1b[31m"))));

    var_(bg_buf, dansi_color_ResetBGBuf) $undefined;
    let bg = dansi_color_resetBG(&bg_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_color_resetBG_static()), u8_l("\x1b[49m"))));
    try_(TEST_expect(mem_eqlBytes(bg.as_const, u8_l("\x1b[49m"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/cursor erase scroll mode: static and runtime forms match" $scope) {
    var_(move_buf, dansi_cursor_MoveToBuf) $undefined;
    let move = dansi_cursor_moveTo(12, 34, &move_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_moveTo_static("12", "34")), u8_l("\x1b[12;34H"))));
    try_(TEST_expect(mem_eqlBytes(move.as_const, u8_l("\x1b[12;34H"))));

    var_(erase_buf, dansi_erase_InDisplayBuf) $undefined;
    let erase = dansi_erase_inDisplay(dansi_erase_Area_all, &erase_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_erase_inDisplay_static(dansi_erase_Area_all)), u8_l("\x1b[2J"))));
    try_(TEST_expect(mem_eqlBytes(erase.as_const, u8_l("\x1b[2J"))));

    var_(scroll_buf, dansi_scroll_UpBuf) $undefined;
    let scroll = dansi_scroll_up(3, &scroll_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_scroll_up_static("3")), u8_l("\x1b[3S"))));
    try_(TEST_expect(mem_eqlBytes(scroll.as_const, u8_l("\x1b[3S"))));

    var_(mode_buf, dansi_mode_SetRawBuf) $undefined;
    let mode = dansi_mode_setRaw(4, false, &mode_buf);
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_mode_setRaw_static("4", 0)), u8_l("\x1b[4l"))));
    try_(TEST_expect(mem_eqlBytes(mode.as_const, u8_l("\x1b[4l"))));

    var_(modes, A$$(2, u16)) = A_init({ 4, 20 });
    var_(many_buf, A$$(16, u8)) $undefined;
    let many = try_(dansi_mode_setMany(A_ref$((S$u16)(modes)).as_const, true, A_ref$((S$u8)(many_buf))));
    try_(TEST_expect(mem_eqlBytes(many.as_const, u8_l("\x1b[4;20h"))));
    return_ok({});
} $unscoped(TEST_fn);
