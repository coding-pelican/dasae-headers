#include "dh/proc/mem.h"

#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"
#include "dh/sys/call/linux.h"

#if in_analysis || plat_is_windows
$attr($maybe_unused)
$static fn_((proc_mem__windows_lock(
    S_const$u8 memory,
    proc_mem_Lock_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_mem_Lock_Unsupported());
    if (VirtualLock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED: return_err(E_cause$proc_mem_Lock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_WORKING_SET_QUOTA: return_err(E_cause$proc_mem_Lock_LimitExceeded());
    default_() return_err(E_cause$proc_mem_Lock_SystemResources()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__windows_unlock(
    S_const$u8 memory
))(proc_mem_Unlock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (VirtualUnlock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED: return_err(E_cause$proc_mem_Unlock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY: return_err(E_cause$proc_mem_Unlock_OutOfMemory());
    default_() return_err(E_cause$proc_mem_Unlock_SystemResources()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__windows_lockAll(
    proc_mem_LockAll_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    let_ignore = opts;
    return_err(E_cause$proc_mem_Lock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__windows_unlockAll(void))(proc_mem_Unlock_E$void) $scope) {
    return_ok({});
} $unscoped(fn);
#endif /* in_analysis || plat_is_windows */

#if in_analysis || plat_is_linux
$attr($maybe_unused)
$static fn_((proc_mem__linux_lock(
    S_const$u8 memory,
    proc_mem_Lock_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_mem_Lock_Unsupported());
    let rc = sys_call_linux_mlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM: return_err(E_cause$proc_mem_Lock_PermissionDenied());
    case sys_call_linux_ENOMEM: return_err(E_cause$proc_mem_Lock_LimitExceeded());
    case sys_call_linux_EAGAIN: return_err(E_cause$proc_mem_Lock_SystemResources());
    case sys_call_linux_EINVAL: claim_unreachable;
    default_() return_err(E_cause$proc_mem_Lock_SystemResources()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__linux_unlock(
    S_const$u8 memory
))(proc_mem_Unlock_E$void) $scope) {
    if (memory.len == 0) return_ok({});
    let rc = sys_call_linux_munlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM: return_err(E_cause$proc_mem_Unlock_PermissionDenied());
    case sys_call_linux_ENOMEM: return_err(E_cause$proc_mem_Unlock_OutOfMemory());
    case sys_call_linux_EAGAIN: return_err(E_cause$proc_mem_Unlock_SystemResources());
    case sys_call_linux_EINVAL: claim_unreachable;
    default_() return_err(E_cause$proc_mem_Unlock_SystemResources()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__linux_lockAll(
    proc_mem_LockAll_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    claim_assert(opts.current || opts.future);
    var_(flags, sys_call_linux_mlockall_flags_t) = 0;
    if (opts.current) flags |= sys_call_linux_MCL_CURRENT;
    if (opts.future) flags |= sys_call_linux_MCL_FUTURE;
    if (opts.on_fault) flags |= sys_call_linux_MCL_ONFAULT;
    let rc = sys_call_linux_mlockall(flags);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM: return_err(E_cause$proc_mem_Lock_PermissionDenied());
    case sys_call_linux_ENOMEM: return_err(E_cause$proc_mem_Lock_LimitExceeded());
    case sys_call_linux_EAGAIN: return_err(E_cause$proc_mem_Lock_SystemResources());
    case sys_call_linux_EINVAL: {
        if (opts.on_fault) return_err(E_cause$proc_mem_Lock_Unsupported());
        claim_unreachable;
    };
    default_() return_err(E_cause$proc_mem_Lock_SystemResources()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__linux_unlockAll(void))(proc_mem_Unlock_E$void) $scope) {
    let rc = sys_call_linux_munlockall();
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM: return_err(E_cause$proc_mem_Unlock_PermissionDenied());
    case sys_call_linux_ENOMEM: return_err(E_cause$proc_mem_Unlock_OutOfMemory());
    case sys_call_linux_EAGAIN: return_err(E_cause$proc_mem_Unlock_SystemResources());
    default_() return_err(E_cause$proc_mem_Unlock_SystemResources()) $end(default);
    }
} $unscoped(fn);
#endif /* in_analysis || plat_is_linux */

$attr($maybe_unused)
$static fn_((proc_mem__unsupported_lock(
    S_const$u8 memory,
    proc_mem_Lock_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    let_ignore = memory;
    let_ignore = opts;
    return_err(E_cause$proc_mem_Lock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__unsupported_unlock(
    S_const$u8 memory
))(proc_mem_Unlock_E$void) $scope) {
    let_ignore = memory;
    return_ok({});
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__unsupported_lockAll(
    proc_mem_LockAll_Opts opts
))(proc_mem_Lock_E$void) $scope) {
    let_ignore = opts;
    return_err(E_cause$proc_mem_Lock_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc_mem__unsupported_unlockAll(void))(proc_mem_Unlock_E$void) $scope) {
    return_ok({});
} $unscoped(fn);

$static let proc_mem__lock = pp_if_(plat_is_windows)(
    pp_then_(proc_mem__windows_lock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_mem__linux_lock),
        pp_else_(proc_mem__unsupported_lock)
    )));
$static let proc_mem__unlock = pp_if_(plat_is_windows)(
    pp_then_(proc_mem__windows_unlock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_mem__linux_unlock),
        pp_else_(proc_mem__unsupported_unlock)
    )));
$static let proc_mem__lockAll = pp_if_(plat_is_windows)(
    pp_then_(proc_mem__windows_lockAll),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_mem__linux_lockAll),
        pp_else_(proc_mem__unsupported_lockAll)
    )));
$static let proc_mem__unlockAll = pp_if_(plat_is_windows)(
    pp_then_(proc_mem__windows_unlockAll),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_mem__linux_unlockAll),
        pp_else_(proc_mem__unsupported_unlockAll)
    )));

fn_((proc_mem_geom(proc_mem_Self self))(heap_Geom)) {
    return proc_mem_ensureValid(self).geom;
};

fn_((proc_mem_lock(
    proc_mem_Self self,
    S_const$u8 memory,
    proc_mem_Lock_Opts opts
))(proc_mem_Lock_E$void)) {
    self = proc_mem_ensureValid(self);
    if (memory.len != 0) {
        claim_assert(mem_isAligned(ptrToInt(memory.ptr), self.geom.page_size));
    }
    return self.vtbl->lockFn(self.ctx, memory, opts);
};

fn_((proc_mem_unlock(
    proc_mem_Self self,
    S_const$u8 memory
))(proc_mem_Unlock_E$void)) {
    self = proc_mem_ensureValid(self);
    if (memory.len != 0) {
        claim_assert(mem_isAligned(ptrToInt(memory.ptr), self.geom.page_size));
    }
    return self.vtbl->unlockFn(self.ctx, memory);
};

fn_((proc_mem_lockAll(
    proc_mem_Self self,
    proc_mem_LockAll_Opts opts
))(proc_mem_Lock_E$void)) {
    self = proc_mem_ensureValid(self);
    return self.vtbl->lockAllFn(self.ctx, opts);
};

fn_((proc_mem_unlockAll(proc_mem_Self self))(proc_mem_Unlock_E$void)) {
    self = proc_mem_ensureValid(self);
    return self.vtbl->unlockAllFn(self.ctx);
};

$static fn_((proc_mem_Direct__lock(
    P$raw ctx,
    S_const$u8 memory,
    proc_mem_Lock_Opts opts
))(proc_mem_Lock_E$void)) {
    claim_assert_nonnull(ctx);
    return proc_mem__lock(memory, opts);
};
$static fn_((proc_mem_Direct__unlock(
    P$raw ctx,
    S_const$u8 memory
))(proc_mem_Unlock_E$void)) {
    claim_assert_nonnull(ctx);
    return proc_mem__unlock(memory);
};
$static fn_((proc_mem_Direct__lockAll(
    P$raw ctx,
    proc_mem_LockAll_Opts opts
))(proc_mem_Lock_E$void)) {
    claim_assert_nonnull(ctx);
    return proc_mem__lockAll(opts);
};
$static fn_((proc_mem_Direct__unlockAll(
    P$raw ctx
))(proc_mem_Unlock_E$void)) {
    claim_assert_nonnull(ctx);
    return proc_mem__unlockAll();
};

$static let_(proc_mem_Direct__vtbl, proc_mem_Self_VTbl) = {
    .lockFn = proc_mem_Direct__lock,
    .unlockFn = proc_mem_Direct__unlock,
    .lockAllFn = proc_mem_Direct__lockAll,
    .unlockAllFn = proc_mem_Direct__unlockAll,
};

fn_((proc_mem_Direct_self(proc_mem_Direct* self))(proc_mem_Self)) {
    claim_assert_nonnull(self);
    return proc_mem_ensureValid((proc_mem_Self){
        .geom = self->geom,
        .ctx = self,
        .vtbl = &proc_mem_Direct__vtbl,
    });
};
