#include "dh-main.h"
#include "dh/proc/Args.h"

TEST_fn_("proc/Args: empty source terminates next and skip without advancing" $scope) {
    var_(scratch_mem, A$$(8, u8)) $undefined;
    var iter = proc_Args_iter(proc_Args_empty);

    let item = try_(proc_Args_Iter_next(&iter, A_ref$((S$u8)(scratch_mem))));
    try_(TEST_expect(isNone(item)));
    try_(TEST_expect(iter.idx == 0));
    try_(TEST_expect(iter.offset == 0));

    try_(TEST_expect(!try_(proc_Args_Iter_skip(&iter))));
    try_(TEST_expect(iter.idx == 0));
    try_(TEST_expect(iter.offset == 0));
} $unscoped(TEST_fn);
