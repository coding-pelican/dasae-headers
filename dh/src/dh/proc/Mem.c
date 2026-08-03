#include "dh/proc/Mem.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused $must_check)
$static fn_((proc_Mem__unsupported_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
$attr($maybe_unused $must_check)
$static fn_((proc_Mem__unsupported_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void));
$attr($maybe_unused $must_check)
$static fn_((proc_Mem__unsupported_lockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void));
$attr($maybe_unused $must_check)
$static fn_((proc_Mem__unsupported_unlockAll(P$raw ctx))(proc_Mem_Unlock_E$void));

pp_if_(plat_is_windows)(pp_then_(
    $attr($must_check)
    $static fn_((proc_Mem__windows_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
    $attr($must_check)
    $static fn_((proc_Mem__windows_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void));
));

pp_if_(plat_is_linux)(pp_then_(
    $attr($must_check)
    $static fn_((proc_Mem__linux_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
    $attr($must_check)
    $static fn_((proc_Mem__linux_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void));
    $attr($must_check)
    $static fn_((proc_Mem__linux_lockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void));
    $attr($must_check)
    $static fn_((proc_Mem__linux_unlockAll(P$raw ctx))(proc_Mem_Unlock_E$void));
));

$static let proc_Mem_direct__lock = pp_if_(plat_is_windows)(
    pp_then_(proc_Mem__windows_lock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Mem__linux_lock),
        pp_else_(proc_Mem__unsupported_lock)
    )));
$static let proc_Mem_direct__unlock = pp_if_(plat_is_windows)(
    pp_then_(proc_Mem__windows_unlock),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Mem__linux_unlock),
        pp_else_(proc_Mem__unsupported_unlock)
    )));
$static let proc_Mem_direct__lockAll = pp_if_(plat_is_linux)(
    pp_then_(proc_Mem__linux_lockAll),
    pp_else_(proc_Mem__unsupported_lockAll));
$static let proc_Mem_direct__unlockAll = pp_if_(plat_is_linux)(
    pp_then_(proc_Mem__linux_unlockAll),
    pp_else_(proc_Mem__unsupported_unlockAll));

/*========== External Definitions ===========================================*/

$static var_(proc_Mem__ctx_failing, Void) $undefined_static;
let_(proc_Mem_VTbl_failing, proc_Mem_VTbl) = {
    .lockFn = proc_Mem_VTbl_failingLock,
    .unlockFn = proc_Mem_VTbl_failingUnlock,
    .lockAllFn = proc_Mem_VTbl_failingLockAll,
    .unlockAllFn = proc_Mem_VTbl_failingUnlockAll,
};
let_(proc_Mem_failing, proc_Mem) = {
    .ctx = &proc_Mem__ctx_failing,
    .vtbl = &proc_Mem_VTbl_failing,
};
fn_((proc_Mem_direct(void))(proc_Mem_direct_E$proc_Mem) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_linux))(
        pp_then_({
            $static var_(ctx, Void) $undefined_static;
            $static let_(vtbl, proc_Mem_VTbl) = {
                .lockFn = proc_Mem_direct__lock,
                .unlockFn = proc_Mem_direct__unlock,
                .lockAllFn = proc_Mem_direct__lockAll,
                .unlockAllFn = proc_Mem_direct__unlockAll,
            };
            return_ok(proc_Mem_ensureValid((proc_Mem){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$proc_Mem_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((proc_Mem_lock(proc_Mem self, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->lockFn(self.ctx, mem, opts);
};
fn_((proc_Mem_unlock(proc_Mem self, S_const$u8 mem))(proc_Mem_Unlock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->unlockFn(self.ctx, mem);
};
fn_((proc_Mem_lockAll(proc_Mem self, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void)) {
    self = proc_Mem_ensureValid(self);
    claim_assert(opts.current || opts.future);
    return self.vtbl->lockAllFn(self.ctx, opts);
};
fn_((proc_Mem_unlockAll(proc_Mem self))(proc_Mem_Unlock_E$void)) {
    self = proc_Mem_ensureValid(self);
    return self.vtbl->unlockAllFn(self.ctx);
};

fn_((proc_Mem_VTbl_failingLock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnullS(mem);
    let_ignore = opts;
    return_err(E_cause$proc_Mem_Lock_Unsupported());
} $unscoped(fn);
fn_((proc_Mem_VTbl_failingUnlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnullS(mem);
    return_err(E_cause$proc_Mem_Unlock_Unsupported());
} $unscoped(fn);
fn_((proc_Mem_VTbl_failingLockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = opts;
    return_err(E_cause$proc_Mem_Lock_Unsupported());
} $unscoped(fn);
fn_((proc_Mem_VTbl_failingUnlockAll(P$raw ctx))(proc_Mem_Unlock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    return_err(E_cause$proc_Mem_Unlock_Unsupported());
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

/*--- Unsupported ---*/

fn_((proc_Mem__unsupported_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void)) {
    return proc_Mem_VTbl_failingLock(ctx, mem, opts);
};
fn_((proc_Mem__unsupported_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void)) {
    return proc_Mem_VTbl_failingUnlock(ctx, mem);
};
fn_((proc_Mem__unsupported_lockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void)) {
    return proc_Mem_VTbl_failingLockAll(ctx, opts);
};
fn_((proc_Mem__unsupported_unlockAll(P$raw ctx))(proc_Mem_Unlock_E$void)) {
    return proc_Mem_VTbl_failingUnlockAll(ctx);
};

/*--- Windows ---*/

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"

fn_((proc_Mem__windows_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (mem.len == 0) return_ok({});
    if (opts.on_fault) return_err(E_cause$proc_Mem_Lock_Unsupported());
    if (VirtualLock(constCast(mem.ptr), mem.len)) return_ok({});
    switch (GetLastError()) {
    case_((ERROR_ACCESS_DENIED)) return_err(E_cause$proc_Mem_Lock_PermissionDenied()) $end(case);
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case_((ERROR_WORKING_SET_QUOTA)) return_err(E_cause$proc_Mem_Lock_LimitExceeded()) $end(case);
    default_() return_err(E_cause$proc_Mem_Lock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
fn_((proc_Mem__windows_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (mem.len == 0) return_ok({});
    if (VirtualUnlock(constCast(mem.ptr), mem.len)) return_ok({});
    switch (GetLastError()) {
    case_((ERROR_ACCESS_DENIED)) return_err(E_cause$proc_Mem_Unlock_PermissionDenied()) $end(case);
    case_((ERROR_NOT_ENOUGH_MEMORY)) return_err(E_cause$proc_Mem_Unlock_OutOfMemory()) $end(case);
    default_() return_err(E_cause$proc_Mem_Unlock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
#endif /* plat_is_windows */

/*--- Linux ---*/

#if plat_is_linux
#include "dh/sys/call/linux.h"

fn_((proc_Mem__linux_lock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (mem.len == 0) return_ok({});
    let rc = when_(opts.on_fault)(
        provide_(sys_call_linux_mlock2(mem.ptr, mem.len, sys_call_linux_MLOCK_ONFAULT)),
        instead_(sys_call_linux_mlock(mem.ptr, mem.len))
    );
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case_((sys_call_linux_ENOSYS)) return_err(E_cause$proc_Mem_Lock_Unsupported()) $end(case);
    case_((sys_call_linux_EPERM)) return_err(E_cause$proc_Mem_Lock_PermissionDenied()) $end(case);
    case_((sys_call_linux_ENOMEM)) return_err(E_cause$proc_Mem_Lock_LimitExceeded()) $end(case);
    case_((sys_call_linux_EAGAIN)) return_err(E_cause$proc_Mem_Lock_SysResrcs()) $end(case);
    case_((sys_call_linux_EINVAL)) claim_unreachable $end(case);
    default_() return_err(E_cause$proc_Mem_Lock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
fn_((proc_Mem__linux_unlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (mem.len == 0) return_ok({});
    let rc = sys_call_linux_munlock(mem.ptr, mem.len);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case_((sys_call_linux_EPERM)) return_err(E_cause$proc_Mem_Unlock_PermissionDenied()) $end(case);
    case_((sys_call_linux_ENOMEM)) return_err(E_cause$proc_Mem_Unlock_OutOfMemory()) $end(case);
    case_((sys_call_linux_EAGAIN)) return_err(E_cause$proc_Mem_Unlock_SysResrcs()) $end(case);
    case_((sys_call_linux_EINVAL)) claim_unreachable $end(case);
    default_() return_err(E_cause$proc_Mem_Unlock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
fn_((proc_Mem__linux_lockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    claim_assert(opts.current || opts.future);
    var_(flags, sys_call_linux_mlockall_flags_t) = 0;
    if (opts.current) flags |= sys_call_linux_MCL_CURRENT;
    if (opts.future) flags |= sys_call_linux_MCL_FUTURE;
    if (opts.on_fault) flags |= sys_call_linux_MCL_ONFAULT;
    let rc = sys_call_linux_mlockall(flags);
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case_((sys_call_linux_EPERM)) return_err(E_cause$proc_Mem_Lock_PermissionDenied()) $end(case);
    case_((sys_call_linux_ENOMEM)) return_err(E_cause$proc_Mem_Lock_LimitExceeded()) $end(case);
    case_((sys_call_linux_EAGAIN)) return_err(E_cause$proc_Mem_Lock_SysResrcs()) $end(case);
    case_((sys_call_linux_EINVAL)) {
        if (opts.on_fault) return_err(E_cause$proc_Mem_Lock_Unsupported());
        claim_unreachable;
    } $end(case);
    default_() return_err(E_cause$proc_Mem_Lock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
fn_((proc_Mem__linux_unlockAll(P$raw ctx))(proc_Mem_Unlock_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let rc = sys_call_linux_munlockall();
    if (!sys_call_linux_syscall_isErr(rc)) return_ok({});
    switch (sys_call_linux_syscall_err(rc)) {
    case_((sys_call_linux_EPERM)) return_err(E_cause$proc_Mem_Unlock_PermissionDenied()) $end(case);
    case_((sys_call_linux_ENOMEM)) return_err(E_cause$proc_Mem_Unlock_OutOfMemory()) $end(case);
    case_((sys_call_linux_EAGAIN)) return_err(E_cause$proc_Mem_Unlock_SysResrcs()) $end(case);
    default_() return_err(E_cause$proc_Mem_Unlock_SysResrcs()) $end(default);
    }
} $unscoped(fn);
#endif /* plat_is_linux */
