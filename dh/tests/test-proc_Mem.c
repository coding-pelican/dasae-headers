#include "dh-main.h"
#include "dh/proc/Mem.h"

$static var_(test_proc_Mem__page, A$$(4096, u8) $align(4096));

TEST_fn_("proc/Mem: direct capability controls independent range residency" $guard) {
    let memory = catch_((proc_Mem_direct())(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("direct process-memory residency is unavailable"))))
    ));
    let bytes = A_ref$((S_const$u8)(test_proc_Mem__page));
    catch_((proc_Mem_lock(memory, bytes, proc_Mem_LockOpts_default))(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("process memory-lock policy rejected the request"))))
    ));
    try_(proc_Mem_unlock(memory, bytes));
} $unguarded(TEST_fn);
