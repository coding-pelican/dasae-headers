#include "dh/proc/Mem.h"

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"
#endif /* plat_is_windows */
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((proc_Mem__unsupportedLock(
    P$raw ctx,
    S_const$u8 memory,
    proc_Mem_LockOpts opts
))(proc_Mem_Lock_E$void));
$attr($maybe_unused)
$static fn_((proc_Mem__unsupportedUnlock(
    P$raw ctx,
    S_const$u8 memory
))(proc_Mem_Unlock_E$void));
$attr($maybe_unused)
$static fn_((proc_Mem__unsupportedLockAll(
    P$raw ctx,
    proc_Mem_LockAllOpts opts
))(proc_Mem_Lock_E$void));
$attr($maybe_unused)
$static fn_((proc_Mem__unsupportedUnlockAll(
    P$raw ctx
))(proc_Mem_Unlock_E$void));
$attr($maybe_unused)
$static fn_((proc_Mem__unsupportedProtect(
    P$raw ctx,
    S$u8 memory,
    proc_Mem_Protcn protection
))(proc_Mem_Protect_E$void));

pp_if_(plat_is_windows)(pp_then_(
    $static fn_((proc_Mem__windowsLock(
        P$raw ctx,
        S_const$u8 memory,
        proc_Mem_LockOpts opts
    ))(proc_Mem_Lock_E$void));
    $static fn_((proc_Mem__windowsUnlock(
        P$raw ctx,
        S_const$u8 memory
    ))(proc_Mem_Unlock_E$void));
    $static fn_((proc_Mem__windowsProtect(
        P$raw ctx,
        S$u8 memory,
        proc_Mem_Protcn protection
    ))(proc_Mem_Protect_E$void));
));

pp_if_(plat_is_linux)(pp_then_(
    $static fn_((proc_Mem__linuxLock(
        P$raw ctx,
        S_const$u8 memory,
        proc_Mem_LockOpts opts
    ))(proc_Mem_Lock_E$void));
    $static fn_((proc_Mem__linuxUnlock(
        P$raw ctx,
        S_const$u8 memory
    ))(proc_Mem_Unlock_E$void));
    $static fn_((proc_Mem__linuxLockAll(
        P$raw ctx,
        proc_Mem_LockAllOpts opts
    ))(proc_Mem_Lock_E$void));
    $static fn_((proc_Mem__linuxUnlockAll(
        P$raw ctx
    ))(proc_Mem_Unlock_E$void));
    $static fn_((proc_Mem__linuxProtect(
        P$raw ctx,
        S$u8 memory,
        proc_Mem_Protcn protection
    ))(proc_Mem_Protect_E$void));
));

$static let proc_Mem__lock = pp_if_(plat_is_windows)(
    pp_then_(proc_Mem__windowsLock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Mem__linuxLock),
        pp_else_(proc_Mem__unsupportedLock)
    )));
$static let proc_Mem__unlock = pp_if_(plat_is_windows)(
    pp_then_(proc_Mem__windowsUnlock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Mem__linuxUnlock),
        pp_else_(proc_Mem__unsupportedUnlock)
    )));
$static let proc_Mem__lockAll = pp_if_(plat_is_linux)(
    pp_then_(proc_Mem__linuxLockAll),
    pp_else_(proc_Mem__unsupportedLockAll));
$static let proc_Mem__unlockAll = pp_if_(plat_is_linux)(
    pp_then_(proc_Mem__linuxUnlockAll),
    pp_else_(proc_Mem__unsupportedUnlockAll));
$static let proc_Mem__protect = pp_if_(plat_is_windows)(
    pp_then_(proc_Mem__windowsProtect),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Mem__linuxProtect),
        pp_else_(proc_Mem__unsupportedProtect)
    )));

$static let_(proc_Mem__direct_vtbl, proc_Mem_VTbl) = {
    .lockFn = proc_Mem__lock,
    .unlockFn = proc_Mem__unlock,
    .lockAllFn = proc_Mem__lockAll,
    .unlockAllFn = proc_Mem__unlockAll,
    .protectFn = proc_Mem__protect,
};

/*========== External Definitions ===========================================*/

fn_((proc_Mem_direct(void))(proc_Mem_direct_E$proc_Mem) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_linux))(
        pp_then_(
            $static var_(ctx, Void) $undefined_static;
            return_ok(proc_Mem_ensureValid((proc_Mem){
                .ctx = &ctx,
                .vtbl = &proc_Mem__direct_vtbl,
            }))
        ),
        pp_else_(
            return_err(E_cause$proc_Mem_direct_Unsupported())
        ));
} $unscoped(fn);

fn_((proc_Mem_lock(
    proc_Mem self,
    S_const$u8 memory,
    proc_Mem_LockOpts opts
))(proc_Mem_Lock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->lockFn(self.ctx, memory, opts);
};

fn_((proc_Mem_unlock(
    proc_Mem self,
    S_const$u8 memory
))(proc_Mem_Unlock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->unlockFn(self.ctx, memory);
};

fn_((proc_Mem_lockAll(
    proc_Mem self,
    proc_Mem_LockAllOpts opts
))(proc_Mem_Lock_E$void)) {
    self = proc_Mem_ensureValid(self);
    claim_assert(opts.current || opts.future);
    return self.vtbl->lockAllFn(self.ctx, opts);
};

fn_((proc_Mem_unlockAll(proc_Mem self))(proc_Mem_Unlock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->unlockAllFn(self.ctx);
};

fn_((proc_Mem_protect(
    proc_Mem self,
    S$u8 memory,
    proc_Mem_Protcn protection
))(proc_Mem_Protect_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->protectFn(self.ctx, memory, protection);
};

/*========== Internal Definitions ===========================================*/

/*--- Unsupported ---*/

fn_((proc_Mem__unsupportedLock(
    P$raw ctx,
    S_const$u8 memory,
    proc_Mem_LockOpts opts
))(proc_Mem_Lock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = memory;
    let_ignore = opts;
    return_err(E_cause$proc_Mem_Lock_Unsupported());
} $unscoped(fn);

fn_((proc_Mem__unsupportedUnlock(
    P$raw ctx,
    S_const$u8 memory
))(proc_Mem_Unlock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = memory;
    return_err(E_cause$proc_Mem_Unlock_Unsupported());
} $unscoped(fn);

fn_((proc_Mem__unsupportedLockAll(
    P$raw ctx,
    proc_Mem_LockAllOpts opts
))(proc_Mem_Lock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = opts;
    return_err(E_cause$proc_Mem_Lock_Unsupported());
} $unscoped(fn);

fn_((proc_Mem__unsupportedUnlockAll(
    P$raw ctx
))(proc_Mem_Unlock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    return_err(E_cause$proc_Mem_Unlock_Unsupported());
} $unscoped(fn);

fn_((proc_Mem__unsupportedProtect(
    P$raw ctx,
    S$u8 memory,
    proc_Mem_Protcn protection
))(proc_Mem_Protect_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = memory;
    let_ignore = protection;
    return_err(E_cause$proc_Mem_Protect_Unsupported());
} $unscoped(fn);

/*--- Windows ---*/

#if plat_is_windows
fn_((proc_Mem__windowsLock(
    P$raw ctx,
    S_const$u8 memory,
    proc_Mem_LockOpts opts
))(proc_Mem_Lock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_Mem_Lock_Unsupported());
    if (VirtualLock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED:
        return_err(E_cause$proc_Mem_Lock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_WORKING_SET_QUOTA:
        return_err(E_cause$proc_Mem_Lock_LimitExceeded());
    default_()
        return_err(E_cause$proc_Mem_Lock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

fn_((proc_Mem__windowsUnlock(
    P$raw ctx,
    S_const$u8 memory
))(proc_Mem_Unlock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});
    if (VirtualUnlock(constCast(memory.ptr), memory.len)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED:
        return_err(E_cause$proc_Mem_Unlock_PermissionDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
        return_err(E_cause$proc_Mem_Unlock_OutOfMemory());
    default_()
        return_err(E_cause$proc_Mem_Unlock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);
fn_((proc_Mem__windowsProtect(
    P$raw ctx,
    S$u8 memory,
    proc_Mem_Protcn protection
))(proc_Mem_Protect_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});

    DWORD native = PAGE_NOACCESS;
    if (protection.execute) {
        native = protection.write ? PAGE_EXECUTE_READWRITE
               : protection.read  ? PAGE_EXECUTE_READ
                                  : PAGE_EXECUTE;
    } else if (protection.write) {
        native = PAGE_READWRITE;
    } else if (protection.read) {
        native = PAGE_READONLY;
    }

    DWORD old = 0;
    if (VirtualProtect(memory.ptr, memory.len, native, &old)) return_ok({});
    switch (GetLastError()) {
    case ERROR_ACCESS_DENIED:
        return_err(E_cause$proc_Mem_Protect_AccessDenied());
    case ERROR_NOT_ENOUGH_MEMORY:
        return_err(E_cause$proc_Mem_Protect_OutOfMemory());
    default_()
        return_err(E_cause$proc_Mem_Protect_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

#endif /* plat_is_windows */

/*--- Linux ---*/

#if plat_is_linux
fn_((proc_Mem__linuxLock(
    P$raw ctx,
    S_const$u8 memory,
    proc_Mem_LockOpts opts
))(proc_Mem_Lock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});
    let rc = opts.on_fault
               ? sys_call_linux_mlock2(
                     memory.ptr,
                     memory.len,
                     sys_call_linux_MLOCK_ONFAULT
                 )
               : sys_call_linux_mlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_ENOSYS:
        return_err(E_cause$proc_Mem_Lock_Unsupported());
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_Mem_Lock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_Mem_Lock_LimitExceeded());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_Mem_Lock_SystemResources());
    case sys_call_linux_EINVAL:
        claim_unreachable;
    default_()
        return_err(E_cause$proc_Mem_Lock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

fn_((proc_Mem__linuxUnlock(
    P$raw ctx,
    S_const$u8 memory
))(proc_Mem_Unlock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});
    let rc = sys_call_linux_munlock(memory.ptr, memory.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_Mem_Unlock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_Mem_Unlock_OutOfMemory());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_Mem_Unlock_SystemResources());
    case sys_call_linux_EINVAL:
        claim_unreachable;
    default_()
        return_err(E_cause$proc_Mem_Unlock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

fn_((proc_Mem__linuxLockAll(
    P$raw ctx,
    proc_Mem_LockAllOpts opts
))(proc_Mem_Lock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert(opts.current || opts.future);
    var_(flags, sys_call_linux_mlockall_flags_t) = 0;
    if (opts.current) flags |= sys_call_linux_MCL_CURRENT;
    if (opts.future) flags |= sys_call_linux_MCL_FUTURE;
    if (opts.on_fault) flags |= sys_call_linux_MCL_ONFAULT;
    let rc = sys_call_linux_mlockall(flags);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_Mem_Lock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_Mem_Lock_LimitExceeded());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_Mem_Lock_SystemResources());
    case sys_call_linux_EINVAL: {
        if (opts.on_fault) {
            return_err(E_cause$proc_Mem_Lock_Unsupported());
        }
        claim_unreachable;
    };
    default_()
        return_err(E_cause$proc_Mem_Lock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

fn_((proc_Mem__linuxUnlockAll(
    P$raw ctx
))(proc_Mem_Unlock_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let rc = sys_call_linux_munlockall();
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_Mem_Unlock_PermissionDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_Mem_Unlock_OutOfMemory());
    case sys_call_linux_EAGAIN:
        return_err(E_cause$proc_Mem_Unlock_SystemResources());
    default_()
        return_err(E_cause$proc_Mem_Unlock_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);
fn_((proc_Mem__linuxProtect(
    P$raw ctx,
    S$u8 memory,
    proc_Mem_Protcn protection
))(proc_Mem_Protect_E$void) $scope) {
    claim_assert_nonnull(ctx);
    if (memory.len == 0) return_ok({});

    var_(native, sys_call_linux_mmap_prot_t) = sys_call_linux_PROT_NONE;
    if (protection.read) native |= sys_call_linux_PROT_READ;
    if (protection.write) native |= sys_call_linux_PROT_WRITE;
    if (protection.execute) native |= sys_call_linux_PROT_EXEC;

    let rc = sys_call_linux_mprotect(memory.ptr, memory.len, native);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case sys_call_linux_EACCES:
    case sys_call_linux_EPERM:
        return_err(E_cause$proc_Mem_Protect_AccessDenied());
    case sys_call_linux_ENOMEM:
        return_err(E_cause$proc_Mem_Protect_OutOfMemory());
    case sys_call_linux_ENOSYS:
        return_err(E_cause$proc_Mem_Protect_Unsupported());
    case sys_call_linux_EINVAL:
        claim_unreachable;
    default_()
        return_err(E_cause$proc_Mem_Protect_SystemResources());
            $end(default)
        ;
    }
} $unscoped(fn);

#endif /* plat_is_linux */
