#include <dh-main.h>
#include <dansi-core.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_test_expectSeq(S_const$u8 input, dansi_Seq_Kind kind, S_const$u8 bytes))(E$void) $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(input));
    var_(buf, A$$(64, u8)) $undefined;
    let seq = try_(dansi_Seq_receive(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(buf))));
    try_(TEST_expect(seq.kind == kind));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, bytes)));
    return_ok({});
} $unscoped(fn);

$static fn_((dansi_test_expectReceiveCSI(S_const$u8 input))(E$void) $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(input));
    var_(buf, A$$(32, u8)) $undefined;
    let seq = try_(dansi_Seq_receiveCSI(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(seq.as_const, input)));
    return_ok({});
} $unscoped(fn);

$static fn_((dansi_test_expectReceiveOSC(S_const$u8 input))(E$void) $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(input));
    var_(buf, A$$(32, u8)) $undefined;
    let seq = try_(dansi_Seq_receiveOSC(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(seq.as_const, input)));
    return_ok({});
} $unscoped(fn);

TEST_fn_("dansi-core/Seq: receive classifies text controls and ESC forms" $scope) {
    try_(dansi_test_expectSeq(u8_l("A"), dansi_Seq_Kind_text, u8_l("A")));
    try_(dansi_test_expectSeq(u8_l("\n"), dansi_Seq_Kind_c0, u8_l("\n")));
    try_(dansi_test_expectSeq(u8_l("\x1b(0"), dansi_Seq_Kind_esc, u8_l("\x1b(")));
    try_(dansi_test_expectSeq(u8_l("\x84"), dansi_Seq_Kind_c1, u8_l("\x84")));
    try_(dansi_test_expectSeq(u8_l("\x1bN0"), dansi_Seq_Kind_ss2, u8_l("\x1bN0")));
    try_(dansi_test_expectSeq(u8_l("\x8e" "0"), dansi_Seq_Kind_ss2, u8_l("\x8e" "0")));
    try_(dansi_test_expectSeq(u8_l("\x1bOP"), dansi_Seq_Kind_ss3, u8_l("\x1bOP")));
    try_(dansi_test_expectSeq(u8_l("\x8f" "P"), dansi_Seq_Kind_ss3, u8_l("\x8f" "P")));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Seq: receive classifies CSI and control strings" $scope) {
    try_(dansi_test_expectSeq(u8_l("\x1b[12;34R"), dansi_Seq_Kind_csi, u8_l("\x1b[12;34R")));
    try_(dansi_test_expectSeq(u8_l("\x9b" "12;34R"), dansi_Seq_Kind_csi, u8_l("\x9b" "12;34R")));
    try_(dansi_test_expectSeq(u8_l("\x1b]0;title\x1b\\"), dansi_Seq_Kind_osc, u8_l("\x1b]0;title\x1b\\")));
    try_(dansi_test_expectSeq(u8_l("\x9d" "0;title\x9c"), dansi_Seq_Kind_osc, u8_l("\x9d" "0;title\x9c")));
    try_(dansi_test_expectSeq(u8_l("\x1bPdata\x1b\\"), dansi_Seq_Kind_dcs, u8_l("\x1bPdata\x1b\\")));
    try_(dansi_test_expectSeq(u8_l("\x90" "data\x9c"), dansi_Seq_Kind_dcs, u8_l("\x90" "data\x9c")));
    try_(dansi_test_expectSeq(u8_l("\x1b^pm\x07"), dansi_Seq_Kind_pm, u8_l("\x1b^pm\x07")));
    try_(dansi_test_expectSeq(u8_l("\x1b_apc\x1b\\"), dansi_Seq_Kind_apc, u8_l("\x1b_apc\x1b\\")));
    try_(dansi_test_expectSeq(u8_l("\x1bXsos\x1b\\"), dansi_Seq_Kind_sos, u8_l("\x1bXsos\x1b\\")));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Seq: typed receivers accept 7-bit and 8-bit introducers" $scope) {
    try_(dansi_test_expectReceiveCSI(u8_l("\x1b[6n")));
    try_(dansi_test_expectReceiveCSI(u8_l("\x9b" "6n")));
    try_(dansi_test_expectReceiveOSC(u8_l("\x1b]0;title\x1b\\")));
    try_(dansi_test_expectReceiveOSC(u8_l("\x9d" "0;title\x9c")));

    var ss2_reader = io_Fixed_Reader_from(io_Fixed_reading(u8_l("\x8e" "A")));
    var_(ss2_buf, A$$(2, u8)) $undefined;
    let ss2 = try_(dansi_Seq_receiveSS2(io_Fixed_reader(&ss2_reader), A_ref$((S$u8)(ss2_buf))));
    try_(TEST_expect(mem_eqlBytes(ss2.as_const, u8_l("\x8e" "A"))));

    var ss3_reader = io_Fixed_Reader_from(io_Fixed_reading(u8_l("\x8f" "P")));
    var_(ss3_buf, A$$(2, u8)) $undefined;
    let ss3 = try_(dansi_Seq_receiveSS3(io_Fixed_reader(&ss3_reader), A_ref$((S$u8)(ss3_buf))));
    try_(TEST_expect(mem_eqlBytes(ss3.as_const, u8_l("\x8f" "P"))));
    return_ok({});
} $unscoped(TEST_fn);
