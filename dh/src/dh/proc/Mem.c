#include "dh/proc/Mem.h"

#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"
#include "dh/sys/call/linux.h"

#if in_analysis || plat_is_windows
$attr($maybe_unused)
$static fn_((proc_Memory__windows_lock(
    S_const$u8 memory,
    proc_MemLock_Opts opts
))(proc_MemLock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_MemLock_Unsupported());
    if (VirtualLock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED:
        return_err(E_cause$proc_MemLock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_WORKING_SET_QUOTA:
        return_err(E_cause$proc_MemLock_LimitExceeded());
    default_()
        return_err(E_cause$proc_MemLock_SystemResources());
    $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__windows_unlock(
    S_const$u8 memory
))(proc_MemUnlock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (VirtualUnlock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED:
        return_err(E_cause$proc_MemUnlock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
        return_err(E_cause$proc_MemUnlock_OutOfMemory());
    default_()
        return_err(E_cause$proc_MemUnlock_SystemResources());
    $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__windows_lockAll(
    proc_MemLockAll_Opts opts
))(proc_MemLock_E$void) $scope) {
    let_ignore = opts;
    return_err(E_cause$proc_MemLock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__windows_unlockAll(void))(proc_MemUnlock_E$void) $scope) {
    return_ok({});
} $unscoped(fn);
#endif /* in_analysis || plat_is_windows */

#if in_analysis || plat_is_linux
$attr($maybe_unused)
$static fn_((proc_Memory__linux_lock(
    S_const$u8 memory,
    proc_MemLock_Opts opts
))(proc_MemLock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_MemLock_Unsupported());
    let rc = sys_call_linux_mlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_MemLock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_MemLock_LimitExceeded());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_MemLock_SystemResources());
    case sys_call_linux_EINVAL:
        claim_unreachable;
    default_()
        return_err(E_cause$proc_MemLock_SystemResources());
    $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__linux_unlock(
    S_const$u8 memory
))(proc_MemUnlock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    let rc = sys_call_linux_munlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_MemUnlock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_MemUnlock_OutOfMemory());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_MemUnlock_SystemResources());
    case sys_call_linux_EINVAL:
        claim_unreachable;
    default_()
        return_err(E_cause$proc_MemUnlock_SystemResources());
    $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__linux_lockAll(
    proc_MemLockAll_Opts opts
))(proc_MemLock_E$void) $scope) {
    claim_assert(opts.current || opts.future);
    var_(flags, sys_call_linux_mlockall_flags_t) = 0;
    if (opts.current) flags |= sys_call_linux_MCL_CURRENT;
    if (opts.future) flags |= sys_call_linux_MCL_FUTURE;
    if (opts.on_fault) flags |= sys_call_linux_MCL_ONFAULT;
    let rc = sys_call_linux_mlockall(flags);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_MemLock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_MemLock_LimitExceeded());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_MemLock_SystemResources());
    case sys_call_linux_EINVAL:
        if (opts.on_fault) return_err(E_cause$proc_MemLock_Unsupported());
        claim_unreachable;
    default_()
        return_err(E_cause$proc_MemLock_SystemResources());
    $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__linux_unlockAll(void))(proc_MemUnlock_E$void) $scope) {
    let rc = sys_call_linux_munlockall();
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_MemUnlock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_MemUnlock_OutOfMemory());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_MemUnlock_SystemResources());
    default_()
        return_err(E_cause$proc_MemUnlock_SystemResources());
    $end(default);
    }
} $unscoped(fn);
#endif /* in_analysis || plat_is_linux */

$attr($maybe_unused)
$static fn_((proc_Memory__unsupported_lock(
    S_const$u8 memory,
    proc_MemLock_Opts opts
))(proc_MemLock_E$void) $scope) {
    let_ignore = memory;
    let_ignore = opts;
    return_err(E_cause$proc_MemLock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__unsupported_unlock(
    S_const$u8 memory
))(proc_MemUnlock_E$void) $scope) {
    let_ignore = memory;
    return_ok({});
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__unsupported_lockAll(
    proc_MemLockAll_Opts opts
))(proc_MemLock_E$void) $scope) {
    let_ignore = opts;
    return_err(E_cause$proc_MemLock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_Memory__unsupported_unlockAll(void))(proc_MemUnlock_E$void) $scope) {
    return_ok({});
} $unscoped(fn);

$static let proc_Memory__lock = pp_if_(plat_is_windows)(
    pp_then_(proc_Memory__windows_lock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Memory__linux_lock),
        pp_else_(proc_Memory__unsupported_lock)
    )));
$static let proc_Memory__unlock = pp_if_(plat_is_windows)(
    pp_then_(proc_Memory__windows_unlock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Memory__linux_unlock),
        pp_else_(proc_Memory__unsupported_unlock)
    )));
$static let proc_Memory__lockAll = pp_if_(plat_is_windows)(
    pp_then_(proc_Memory__windows_lockAll),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Memory__linux_lockAll),
        pp_else_(proc_Memory__unsupported_lockAll)
    )));
$static let proc_Memory__unlockAll = pp_if_(plat_is_windows)(
    pp_then_(proc_Memory__windows_unlockAll),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Memory__linux_unlockAll),
        pp_else_(proc_Memory__unsupported_unlockAll)
    )));

fn_((proc_lockMemory(
    S_const$u8 memory,
    proc_MemLock_Opts opts
))(proc_MemLock_E$void)) {
    return proc_Memory__lock(memory, opts);
};

fn_((proc_unlockMemory(
    S_const$u8 memory
))(proc_MemUnlock_E$void)) {
    return proc_Memory__unlock(memory);
};

fn_((proc_lockMemoryAll(
    proc_MemLockAll_Opts opts
))(proc_MemLock_E$void)) {
    return proc_Memory__lockAll(opts);
};

fn_((proc_unlockMemoryAll(void))(proc_MemUnlock_E$void)) {
    return proc_Memory__unlockAll();
};
