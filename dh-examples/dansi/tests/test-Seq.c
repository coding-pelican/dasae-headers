#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/Seq: construct caller-provided text sequence" $scope) {
    let seq = dansi_Seq_text(u8_l("abc"));
    try_(TEST_expect(seq.kind == dansi_Seq_Kind_text));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("abc"))));
    try_(TEST_expect(!dansi_Seq_isEmpty(seq)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/Seq: construct caller-provided CSI sequence" $scope) {
    let seq = dansi_Seq_csi(u8_l("\x1b[12;34R"));
    try_(TEST_expect(seq.kind == dansi_Seq_Kind_csi));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("\x1b[12;34R"))));
    return_ok({});
} $unscoped(TEST_fn);

