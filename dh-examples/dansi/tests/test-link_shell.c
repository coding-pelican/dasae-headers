#include <dh-main.h>
#include <dansi-link.h>
#include <dansi-shell.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-link/osc8: build and parse hyperlink frames" $scope) {
    var_(plain_buf, A$$(96, u8)) $undefined;
    let plain = try_(dansi_link_osc8_openPlain(u8_l("https://example.invalid"), A_ref$((S$u8)(plain_buf))));
    try_(TEST_expect(mem_eqlBytes(plain.as_const, u8_l("\x1b]8;;https://example.invalid\x1b\\"))));

    var_(id_buf, A$$(96, u8)) $undefined;
    let with_id = try_(dansi_link_osc8_openWithId(u8_l("https://example.invalid"), u8_l("abc"), A_ref$((S$u8)(id_buf))));
    try_(TEST_expect(mem_eqlBytes(with_id.as_const, u8_l("\x1b]8;id=abc;https://example.invalid\x1b\\"))));

    let frame = try_(dansi_link_osc8_parse(try_(dansi_osc_parse(with_id.as_const))));
    try_(TEST_expect(matches(frame, dansi_link_osc8_Frame_open)));
    let link = union_to((frame)(dansi_link_osc8_Frame_open));
    try_(TEST_expect(mem_eqlBytes(link.uri, u8_l("https://example.invalid"))));
    try_(TEST_expect(mem_eqlBytes(link.params.raw, u8_l("id=abc"))));
    try_(TEST_expect(isSome(link.params.id)));
    try_(TEST_expect(mem_eqlBytes(unwrap_(link.params.id), u8_l("abc"))));

    var_(raw_buf, A$$(96, u8)) $undefined;
    let raw = try_(dansi_link_osc8_openRaw(u8_l("https://example.invalid"), u8_l("id=abc:foo=bar"), A_ref$((S$u8)(raw_buf))));
    let raw_frame = try_(dansi_link_osc8_parse(try_(dansi_osc_parse(raw.as_const))));
    try_(TEST_expect(mem_eqlBytes(union_to((raw_frame)(dansi_link_osc8_Frame_open)).params.raw, u8_l("id=abc:foo=bar"))));

    var_(close_buf, A$$(16, u8)) $undefined;
    let close = try_(dansi_link_osc8_close(A_ref$((S$u8)(close_buf))));
    try_(TEST_expect(mem_eqlBytes(close.as_const, u8_l("\x1b]8;;\x1b\\"))));
    let close_frame = try_(dansi_link_osc8_parse(try_(dansi_osc_parse(close.as_const))));
    try_(TEST_expect(matches(close_frame, dansi_link_osc8_Frame_close)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-shell/osc7: build and parse cwd uri" $scope) {
    var_(buf, A$$(96, u8)) $undefined;
    let made = try_(dansi_shell_osc7_setRaw(u8_l("file://host/home/dev"), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(made.as_const, u8_l("\x1b]7;file://host/home/dev\x1b\\"))));

    let cwd = try_(dansi_shell_osc7_parse(try_(dansi_osc_parse(made.as_const))));
    try_(TEST_expect(mem_eqlBytes(cwd.file_uri, u8_l("file://host/home/dev"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-shell/osc133: build and parse prompt and command end" $scope) {
    var_(mark_buf, A$$(32, u8)) $undefined;
    let mark = try_(dansi_shell_osc133_mark(dansi_shell_osc133_Mark_prompt_start, A_ref$((S$u8)(mark_buf))));
    try_(TEST_expect(mem_eqlBytes(mark.as_const, u8_l("\x1b]133;A\x1b\\"))));
    let mark_frame = try_(dansi_shell_osc133_parse(try_(dansi_osc_parse(mark.as_const))));
    try_(TEST_expect(matches(mark_frame, dansi_shell_osc133_Frame_prompt_start)));

    var_(end_buf, A$$(32, u8)) $undefined;
    let ended = try_(dansi_shell_osc133_commandEnd(some$((O$i32)(7)), A_ref$((S$u8)(end_buf))));
    try_(TEST_expect(mem_eqlBytes(ended.as_const, u8_l("\x1b]133;D;7\x1b\\"))));
    let end_frame = try_(dansi_shell_osc133_parse(try_(dansi_osc_parse(ended.as_const))));
    try_(TEST_expect(matches(end_frame, dansi_shell_osc133_Frame_command_end)));
    try_(TEST_expect(unwrap_(union_to((end_frame)(dansi_shell_osc133_Frame_command_end)).exit_code) == 7));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-shell/osc633: build parse and escape raw-sensitive payloads" $scope) {
    var_(cmd_buf, A$$(96, u8)) $undefined;
    let command = try_(dansi_shell_osc633_commandLine(u8_l("echo a;b\\c"), A_ref$((S$u8)(cmd_buf))));
    try_(TEST_expect(mem_eqlBytes(command.as_const, u8_l("\x1b]633;E;echo a\\x3Bb\\x5Cc\x1b\\"))));

    var_(raw_buf, A$$(96, u8)) $undefined;
    let raw = try_(dansi_shell_osc633_commandLineRaw(u8_l("echo raw;value"), A_ref$((S$u8)(raw_buf))));
    let raw_frame = try_(dansi_shell_osc633_parse(try_(dansi_osc_parse(raw.as_const))));
    try_(TEST_expect(matches(raw_frame, dansi_shell_osc633_Frame_command_line)));
    try_(TEST_expect(mem_eqlBytes(union_to((raw_frame)(dansi_shell_osc633_Frame_command_line)).command_line, u8_l("echo raw;value"))));

    var_(prop_buf, A$$(96, u8)) $undefined;
    let prop = try_(dansi_shell_osc633_property(
        (dansi_shell_osc633_Prop){
            .key = u8_l("cwd"),
            .value = u8_l("a;b"),
        },
        A_ref$((S$u8)(prop_buf))
    ));
    try_(TEST_expect(mem_eqlBytes(prop.as_const, u8_l("\x1b]633;P;cwd=a\\x3Bb\x1b\\"))));

    var_(end_buf, A$$(32, u8)) $undefined;
    let ended = try_(dansi_shell_osc633_commandEnd(some$((O$i32)(0)), A_ref$((S$u8)(end_buf))));
    let end_frame = try_(dansi_shell_osc633_parse(try_(dansi_osc_parse(ended.as_const))));
    try_(TEST_expect(matches(end_frame, dansi_shell_osc633_Frame_command_end)));
    try_(TEST_expect(unwrap_(union_to((end_frame)(dansi_shell_osc633_Frame_command_end)).exit_code) == 0));
    return_ok({});
} $unscoped(TEST_fn);
