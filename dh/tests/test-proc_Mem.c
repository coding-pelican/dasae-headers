#include "dh-main.h"
#include "dh/proc/Mem.h"

$static var_(test_proc_Mem__page, A$$(4096, u8) $align(4096));

TEST_fn_("proc/Mem: direct capability controls independent range residency" $guard) {
    let proc_mem = catch_((proc_Mem_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$proc_Mem_direct_Unsupported));
        return_ok_void();
    }));
    let bytes = A_ref$((S_const$u8)(test_proc_Mem__page));
    catch_((proc_Mem_lock(proc_mem, bytes, proc_Mem_LockOpts_default))(err, {
        let tag = E_tag(err.as_any);
        try_(TEST_expect(
            tag == E_Tag$proc_Mem_Lock_Unsupported
            || tag == E_Tag$proc_Mem_Lock_PermissionDenied
            || tag == E_Tag$proc_Mem_Lock_LimitExceeded
            || tag == E_Tag$proc_Mem_Lock_SysResrcs
        ));
        return_ok_void();
    }));
    try_(proc_Mem_unlock(proc_mem, bytes));
} $unguarded(TEST_fn);
