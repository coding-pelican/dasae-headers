#include <dh-main.h>
#include <dansi-core.h>

TEST_fn_("dansi-core/Event: parse printable character" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_text(u8_l("A"))));
    match_(event) {
    pattern_((dansi_Event_key)(key)) {
        try_(TEST_expect(key.code == dansi_Event_KeyCode_char));
        try_(TEST_expect(key.codepoint == 'A'));
        try_(TEST_expect(key.mods.shift == 0));
        try_(TEST_expect(key.mods.ctrl == 0));
        try_(TEST_expect(key.mods.alt == 0));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Ctrl-C" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_text(u8_l("\x03"))));
    match_(event) {
    pattern_((dansi_Event_key)(key)) {
        try_(TEST_expect(key.code == dansi_Event_KeyCode_char));
        try_(TEST_expect(key.codepoint == 'c'));
        try_(TEST_expect(key.mods.ctrl == 1));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: parse Alt-x escape pair" $scope) {
    let event = try_(dansi_Event_parse(dansi_Seq_esc(u8_l("\x1bx"))));
    match_(event) {
    pattern_((dansi_Event_key)(key)) {
        try_(TEST_expect(key.code == dansi_Event_KeyCode_char));
        try_(TEST_expect(key.codepoint == 'x'));
        try_(TEST_expect(key.mods.alt == 1));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Event: cursor position report is not an input event" $scope) {
    try_(TEST_expect(isNone(dansi_Event_tryParse(dansi_Seq_csi(u8_l("\x1b[12;34R"))))));
    return_ok({});
} $unscoped(TEST_fn);
