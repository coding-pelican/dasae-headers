#include "dh-main.h"
#include "dh/proc/Mem.h"

$static var_(test_proc_Mem__page, A$$(4096, u8) $align(4096));

TEST_fn_("proc/Mem: direct capability controls independent range residency" $guard) {
    let proc_mem = catch_((proc_Mem_direct())(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("direct process-memory residency is unavailable"))))
    ));
    let bytes = A_ref$((S_const$u8)(test_proc_Mem__page));
    catch_((proc_Mem_lock(proc_mem, bytes, proc_Mem_LockOpts_default))(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("process memory-lock policy rejected the request"))))
    ));
    try_(proc_Mem_unlock(proc_mem, bytes));
} $unguarded(TEST_fn);

TEST_fn_("proc/Mem: direct capability changes page protection" $guard) {
    let proc_mem = catch_((proc_Mem_direct())(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("direct process-memory protection is unavailable"))))
    ));
    let bytes = A_ref$((S$u8)(test_proc_Mem__page));

    catch_((proc_Mem_protect(
        proc_mem, bytes,
        (proc_Mem_Protcn){
            .read = true,
            .write = false,
            .execute = false,
        }
    ))($ignore, return_ok(try_(TEST_skipMsg(u8_l("process memory-protection policy rejected the request"))))));
    try_(proc_Mem_protect(
        proc_mem, bytes,
        (proc_Mem_Protcn){
            .read = true,
            .write = true,
            .execute = false,
        }
    ));
} $unguarded(TEST_fn);
