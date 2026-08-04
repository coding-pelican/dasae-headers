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

TEST_fn_("proc/Mem: failing capability reports every residency operation" $scope) {
    let bytes = A_ref$((S_const$u8)(test_proc_Mem__page));
    let_(all_opts, proc_Mem_LockAllOpts) = {
        .current = true,
        .future = false,
        .on_fault = false,
    };

    let lock_failed = eval_(bool $scope)(catch_((proc_Mem_lock(
        proc_Mem_failing, bytes, proc_Mem_LockOpts_default
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$proc_Mem_Lock_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    let unlock_failed = eval_(bool $scope)(catch_((proc_Mem_unlock(
        proc_Mem_failing, bytes
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$proc_Mem_Unlock_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    let lock_all_failed = eval_(bool $scope)(catch_((proc_Mem_lockAll(
        proc_Mem_failing, all_opts
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$proc_Mem_Lock_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    let unlock_all_failed = eval_(bool $scope)(catch_((proc_Mem_unlockAll(
        proc_Mem_failing
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$proc_Mem_Unlock_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);

    try_(TEST_expect(proc_Mem_isValid(proc_Mem_failing)));
    try_(TEST_expect(lock_failed));
    try_(TEST_expect(unlock_failed));
    try_(TEST_expect(lock_all_failed));
    try_(TEST_expect(unlock_all_failed));
} $unscoped(TEST_fn);
