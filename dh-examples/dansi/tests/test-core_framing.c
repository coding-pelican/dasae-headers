#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/esc: make and parse frame" $scope) {
    var_(buf, A$$(8, u8)) $undefined;
    let made = try_(dansi_esc_make(u8_l("("), u8_c('0'), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(made.as_const, u8_l(dansi_esc_make_static("(", "0")))));

    let frame = try_(dansi_esc_parse(made.as_const));
    try_(TEST_expect(mem_eqlBytes(frame.intermediates, u8_l("("))));
    try_(TEST_expect(frame.final == u8_c('0')));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/csi: make parse params and subparams" $scope) {
    var_(buf, A$$(64, u8)) $undefined;
    let made = try_(dansi_csi_make(u8_l("?38:2::1:2:3;4"), u8_l("$"), u8_c('p'), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(made.as_const, u8_l("\x1b[?38:2::1:2:3;4$p"))));

    let frame = try_(dansi_csi_parse(made.as_const));
    try_(TEST_expect(mem_eqlBytes(frame.params, u8_l("?38:2::1:2:3;4"))));
    try_(TEST_expect(mem_eqlBytes(frame.intermediates, u8_l("$"))));
    try_(TEST_expect(frame.final == u8_c('p')));
    try_(TEST_expect(dansi_csi_Frame_isPrivate(frame, u8_c('?'))));
    try_(TEST_expect(isSome(dansi_csi_Frame_privateMarker(frame))));
    try_(TEST_expect(unwrap_(dansi_csi_Frame_privateMarker(frame)) == u8_c('?')));

    let param0 = unwrap_(dansi_csi_Frame_paramAt(frame, 0));
    try_(TEST_expect(mem_eqlBytes(param0.raw, u8_l("38:2::1:2:3"))));
    try_(TEST_expect(unwrap_(dansi_csi_Frame_paramAtAsU16(frame, 1)) == 4));

    var subparams = dansi_csi_Param_subparamIter(param0);
    try_(TEST_expect(mem_eqlBytes(unwrap_(dansi_csi_SubparamIter_next(&subparams)), u8_l("38"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(dansi_csi_SubparamIter_next(&subparams)), u8_l("2"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(dansi_csi_SubparamIter_next(&subparams)), u8_l(""))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(dansi_csi_SubparamIter_next(&subparams)), u8_l("1"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/osc: raw and command forms preserve terminators" $scope) {
    var_(raw_buf, A$$(64, u8)) $undefined;
    let raw = try_(dansi_osc_makeRawWithEOS(u8_l("133;A"), dansi_Seq_EOS_bel, A_ref$((S$u8)(raw_buf))));
    try_(TEST_expect(mem_eqlBytes(raw.as_const, u8_l("\x1b]133;A\x07"))));

    let raw_frame = try_(dansi_osc_parse(raw.as_const));
    try_(TEST_expect(raw_frame.terminator == dansi_Seq_EOS_bel));
    try_(TEST_expect(mem_eqlBytes(raw_frame.payload, u8_l("133;A"))));

    let split = unwrap_(dansi_osc_Frame_splitCmd(raw_frame));
    try_(TEST_expect(mem_eqlBytes(split.cmd, u8_l("133"))));
    try_(TEST_expect(mem_eqlBytes(split.payload, u8_l("A"))));
    try_(TEST_expect(unwrap_(dansi_osc_CmdSplit_cmdAsU16(split)) == 133));

    var_(cmd_buf, A$$(64, u8)) $undefined;
    let cmd = try_(dansi_osc_make(8, u8_l(";https://example.invalid"), A_ref$((S$u8)(cmd_buf))));
    try_(TEST_expect(mem_eqlBytes(cmd.as_const, u8_l("\x1b]8;;https://example.invalid\x1b\\"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/dcs: structured and raw control string forms" $scope) {
    var_(buf, A$$(64, u8)) $undefined;
    let made = try_(dansi_dcs_make(u8_l("1"), u8_l("+"), u8_c('q'), u8_l("payload"), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(made.as_const, u8_l("\x1bP1+qpayload\x1b\\"))));

    let frame = try_(dansi_dcs_parse(made.as_const));
    try_(TEST_expect(mem_eqlBytes(frame.params, u8_l("1"))));
    try_(TEST_expect(mem_eqlBytes(frame.intermediates, u8_l("+"))));
    try_(TEST_expect(frame.final == u8_c('q')));
    try_(TEST_expect(mem_eqlBytes(frame.payload, u8_l("payload"))));
    try_(TEST_expect(frame.terminator == dansi_Seq_EOS_st_7bit));

    var_(raw_buf, A$$(64, u8)) $undefined;
    let raw = try_(dansi_dcs_makeRawWithEOS(u8_l("123"), dansi_Seq_EOS_st_8bit, A_ref$((S$u8)(raw_buf))));
    try_(TEST_expect(mem_eqlBytes(raw.as_const, u8_l("\x1bP123\x9c"))));
    let raw_frame = try_(dansi_dcs_parse(raw.as_const));
    try_(TEST_expect(raw_frame.final == 0));
    try_(TEST_expect(mem_eqlBytes(raw_frame.payload, u8_l("123"))));
    try_(TEST_expect(raw_frame.terminator == dansi_Seq_EOS_st_8bit));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/pm apc sos: control strings use EOS helpers" $scope) {
    var_(pm_buf, A$$(32, u8)) $undefined;
    let pm = try_(dansi_pm_makeWithEOS(u8_l("pm"), dansi_Seq_EOS_st_8bit, A_ref$((S$u8)(pm_buf))));
    try_(TEST_expect(mem_eqlBytes(pm.as_const, u8_l("\x1b^pm\x9c"))));
    let pm_frame = try_(dansi_pm_parse(pm.as_const));
    try_(TEST_expect(pm_frame.terminator == dansi_Seq_EOS_st_8bit));
    try_(TEST_expect(mem_eqlBytes(pm_frame.payload, u8_l("pm"))));

    var_(apc_buf, A$$(32, u8)) $undefined;
    let apc = try_(dansi_apc_make(u8_l("apc"), A_ref$((S$u8)(apc_buf))));
    try_(TEST_expect(mem_eqlBytes(apc.as_const, u8_l(dansi_apc_make_static("apc")))));
    let apc_frame = try_(dansi_apc_parse(apc.as_const));
    try_(TEST_expect(apc_frame.terminator == dansi_Seq_EOS_st_7bit));
    try_(TEST_expect(mem_eqlBytes(apc_frame.payload, u8_l("apc"))));

    var_(sos_buf, A$$(32, u8)) $undefined;
    let sos = try_(dansi_sos_makeWithEOS(u8_l("sos"), dansi_Seq_EOS_bel, A_ref$((S$u8)(sos_buf))));
    try_(TEST_expect(mem_eqlBytes(sos.as_const, u8_l("\x1bXsos\x07"))));
    let sos_frame = try_(dansi_sos_parse(sos.as_const));
    try_(TEST_expect(sos_frame.terminator == dansi_Seq_EOS_bel));
    try_(TEST_expect(mem_eqlBytes(sos_frame.payload, u8_l("sos"))));
    return_ok({});
} $unscoped(TEST_fn);
