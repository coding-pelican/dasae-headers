#include <dh-main.h>
#include <dansi-core.h>

TEST_fn_("dansi-core/Event: parse printable character" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_raw(u8_l("A"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_text)(text)) {
        try_(TEST_expect(text.codepoint == 'A'));
        try_(TEST_expect(text.mods.shift == 0));
        try_(TEST_expect(text.mods.ctrl == 0));
        try_(TEST_expect(text.mods.alt == 0));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Ctrl-C" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_raw(u8_l("\x03"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_text)(text)) {
        try_(TEST_expect(text.codepoint == 'c'));
        try_(TEST_expect(text.mods.ctrl == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Alt-x escape pair" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_esc(u8_l("\x1bx"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_text)(text)) {
        try_(TEST_expect(text.codepoint == 'x'));
        try_(TEST_expect(text.mods.alt == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Shift+Up CSI" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[1;2A"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_special)(special)) {
        try_(TEST_expect(special.code == dansi_key_Code_up));
        try_(TEST_expect(special.mods.shift == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Ctrl+Left CSI" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[1;5D"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_special)(special)) {
        try_(TEST_expect(special.code == dansi_key_Code_left));
        try_(TEST_expect(special.mods.ctrl == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Shift+Enter CSI u" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[13;2u"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_special)(special)) {
        try_(TEST_expect(special.code == dansi_key_Code_enter));
        try_(TEST_expect(special.mods.shift == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Ctrl+F5 tilde CSI" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[15;5~"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_special)(special)) {
        try_(TEST_expect(special.code == dansi_key_Code_f5));
        try_(TEST_expect(special.mods.ctrl == 1));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse F13 tilde CSI" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[25~"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_special)(special)) {
        try_(TEST_expect(special.code == dansi_key_Code_fn(13)));
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse focus-in and focus-out CSI" $scope) {
    let focus_in = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1B[I"))));
    let focus_out = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1B[O"))));
    $suppress_(switch_enum)(match_(focus_in)) {
    patt_((dansi_Event_focus)(focus)) try_(TEST_expect(focus == dansi_Event_Focus_in)) $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    $suppress_(switch_enum)(match_(focus_out)) {
    patt_((dansi_Event_focus)(focus)) try_(TEST_expect(focus == dansi_Event_Focus_out)) $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse SGR mouse press" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[<0;12;34M"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_mouse)(mouse)) {
        $suppress_(switch_enum)(match_(mouse)) {
        patt_((dansi_Event_Mouse_press)(press)) {
            try_(TEST_expect(press.x == 12));
            try_(TEST_expect(press.y == 34));
            try_(TEST_expect(press.btn == dansi_mouse_Btn_left));
        } $end(patt);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse SGR mouse release" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[<0;12;34m"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_mouse)(mouse)) {
        $suppress_(switch_enum)(match_(mouse)) {
        patt_((dansi_Event_Mouse_release)(release)) {
            try_(TEST_expect(release.btn == dansi_mouse_Btn_left));
        } $end(patt);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: reject SGR mouse release without button" $scope) {
    try_(TEST_expect(isNone(dansi_Event_tryParse(dansi_Seq_csi(u8_l("\x1b[<3;12;34m"))))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse SGR mouse horizontal wheel" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[<66;7;9M"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_mouse)(mouse)) {
        $suppress_(switch_enum)(match_(mouse)) {
        patt_((dansi_Event_Mouse_wheel)(wheel)) {
            try_(TEST_expect(wheel.wheel == dansi_mouse_Wheel_right));
        } $end(patt);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse SGR mouse extra buttons" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[<128;7;9M"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_mouse)(mouse)) {
        $suppress_(switch_enum)(match_(mouse)) {
        patt_((dansi_Event_Mouse_press)(press)) {
            try_(TEST_expect(press.btn == dansi_mouse_Btn_backward));
        } $end(patt);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse SGR mouse wheel with modifiers" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_csi(u8_l("\x1b[<92;7;9M"))));
    $suppress_(switch_enum)(match_(event)) {
    patt_((dansi_Event_mouse)(mouse)) {
        $suppress_(switch_enum)(match_(mouse)) {
        patt_((dansi_Event_Mouse_wheel)(wheel)) {
            try_(TEST_expect(wheel.x == 7));
            try_(TEST_expect(wheel.y == 9));
            try_(TEST_expect(wheel.wheel == dansi_mouse_Wheel_up));
            try_(TEST_expect(wheel.mods.shift == 1));
            try_(TEST_expect(wheel.mods.alt == 1));
            try_(TEST_expect(wheel.mods.ctrl == 1));
        } $end(patt);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(patt);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: cursor position report is not an input event" $scope) {
    try_(TEST_expect(isNone(dansi_Event_tryParse(dansi_Seq_csi(u8_l("\x1b[12;34R"))))));
    return_ok({});
} $unscoped(TEST_fn);
