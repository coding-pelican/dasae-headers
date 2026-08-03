#include "dh-main.h"
#include "dh/proc/Env.h"

TEST_fn_("proc/Env: empty source has no entries or named values" $scope) {
    var_(scratch_mem, A$$(8, u8)) $undefined;
    let scratch = A_ref$((S$u8)(scratch_mem));
    var iter = proc_Env_iter(proc_Env_empty);

    try_(TEST_expect(isNone(try_(proc_Env_Iter_next(&iter, scratch)))));
    try_(TEST_expect(iter.idx == 0));
    try_(TEST_expect(iter.offset == 0));
    try_(TEST_expect(isNone(try_(proc_Env_by(
        proc_Env_empty, u8_l("missing"), scratch
    )))));
} $unscoped(TEST_fn);
