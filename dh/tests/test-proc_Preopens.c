#include "dh/proc/Preopens.h"
#include "dh/TEST.h"
#include "dh/mem/common.h"

$static fn_((test_proc_Preopens__by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc) $scope) {
    let dir = ptrCast$((fs_Dir*)(ensureNonnull(ctx)));
    if (!mem_eqlBytes(name, u8_l("workspace"))) {
        return_none();
    }
    return_some(union_of$((proc_Preopens_Resrc)(proc_Preopens_Resrc_dir)(*dir)));
} $unscoped(fn);
$static let_(test_proc_Preopens__vtbl, proc_Preopens_VTbl) = {
    .byFn = test_proc_Preopens__by,
};

TEST_fn_("proc/Preopens: empty set has no named resources" $scope) {
    try_(TEST_expect(isNone(proc_Preopens_by(proc_Preopens_empty, u8_l("stdin")))));
} $unscoped(TEST_fn);

TEST_fn_("proc/Preopens: caller set injects named resources" $scope) {
    var_(handle_token, u8) = 0;
    var dir = fs_Dir_Handle_promote(&handle_token);
    let preopens = proc_Preopens_ensureValid((proc_Preopens){
        .ctx = &dir,
        .vtbl = &test_proc_Preopens__vtbl,
    });
    let resource = unwrap_(proc_Preopens_by(
        preopens, u8_l("workspace")
    ));

    try_(TEST_expect(matches(resource, proc_Preopens_Resrc_dir)));
    try_(TEST_expect(
        fs_Dir_handle(union_to((resource)(proc_Preopens_Resrc_dir)))
        == as$(fs_Dir_Handle)(&handle_token)
    ));
} $unscoped(TEST_fn);

TEST_fn_("proc/Preopens: direct capability exposes native standard files" $scope) {
    let preopens = try_(proc_Preopens_direct());

    let in = unwrap_(proc_Preopens_by(preopens, u8_l("stdin")));
    let out = unwrap_(proc_Preopens_by(preopens, u8_l("stdout")));
    let err = unwrap_(proc_Preopens_by(preopens, u8_l("stderr")));

    try_(TEST_expect(matches(in, proc_Preopens_Resrc_file)));
    try_(TEST_expect(matches(out, proc_Preopens_Resrc_file)));
    try_(TEST_expect(matches(err, proc_Preopens_Resrc_file)));
    try_(TEST_expect(isNone(proc_Preopens_by(preopens, u8_l("missing")))));
} $unscoped(TEST_fn);
