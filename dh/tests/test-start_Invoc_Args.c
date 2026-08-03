#include "dh-main.h"
#include "dh/start/Invoc/Args.h"
#include "dh/mem/common.h"

TEST_fn_("start/Invoc/Args: VecZ adapter preserves ordered arguments" $scope) {
    var_(native, A$$(3, P_const$u8)) = A_init({
        [0] = as$(P_const$u8)("program"),
        [1] = as$(P_const$u8)("first"),
        [2] = as$(P_const$u8)("second"),
    });
    var adapter = start_Invoc_Args_initVecZ(
        A_len(native), as$(P_const$P_const$u8)(A_ptr(native))
    );
    var iter = proc_Args_iter(start_Invoc_Args_self(&adapter));
    var_(scratch_mem, A$$(16, u8)) $undefined;
    let scratch = A_ref$((S$u8)(scratch_mem));

    try_(TEST_expect(mem_eqlBytes(
        unwrap_(try_(proc_Args_Iter_next(&iter, scratch))), u8_l("program")
    )));
    try_(TEST_expect(try_(proc_Args_Iter_skip(&iter))));
    try_(TEST_expect(mem_eqlBytes(
        unwrap_(try_(proc_Args_Iter_next(&iter, scratch))), u8_l("second")
    )));
    try_(TEST_expect(isNone(try_(proc_Args_Iter_next(&iter, scratch)))));
} $unscoped(TEST_fn);
