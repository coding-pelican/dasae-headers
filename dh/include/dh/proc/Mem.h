/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Mem
 *
 * @brief   Injectable current-process mem-residency capability
 */
#pragma once
#ifndef proc_Mem__included
#define proc_Mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_Mem_direct_E)(proc_Mem_direct_Unsupported));

T_alias$((proc_Mem_VTbl)(struct proc_Mem_VTbl));
$extern let_(proc_Mem_VTbl_failing, proc_Mem_VTbl);

struct proc_Mem {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Mem_VTbl));
};
T_use_prl$(proc_Mem);
T_use_E$($set(proc_Mem_direct_E)(proc_Mem));
$attr($inline_always)
$static fn_((proc_Mem_isValid(proc_Mem self))(bool));
$attr($inline_always)
$static fn_((proc_Mem_assertValid(P$raw ctx, P_const$$(proc_Mem_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_Mem_ensureValid(proc_Mem self))(proc_Mem));

$extern let_(proc_Mem_failing, proc_Mem);
$attr($must_check)
$extern fn_((proc_Mem_direct(void))(proc_Mem_direct_E$proc_Mem));

errset_((proc_Mem_Lock_E)(
    proc_Mem_Lock_Unsupported,
    proc_Mem_Lock_PermissionDenied,
    proc_Mem_Lock_LimitExceeded,
    proc_Mem_Lock_SysResrcs
));
T_alias$((proc_Mem_LockOpts)(struct proc_Mem_LockOpts {
    var_(on_fault, bool);
}));
$static let_(proc_Mem_LockOpts_default, proc_Mem_LockOpts) = {
    .on_fault = false,
};
$attr($must_check)
$extern fn_((proc_Mem_lock(proc_Mem self, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
errset_((proc_Mem_Unlock_E)(
    proc_Mem_Unlock_Unsupported,
    proc_Mem_Unlock_PermissionDenied,
    proc_Mem_Unlock_OutOfMemory,
    proc_Mem_Unlock_SysResrcs
));
$attr($must_check)
$extern fn_((proc_Mem_unlock(proc_Mem self, S_const$u8 mem))(proc_Mem_Unlock_E$void));
T_alias$((proc_Mem_LockAllOpts)(struct proc_Mem_LockAllOpts {
    var_(current, bool);
    var_(future, bool);
    var_(on_fault, bool);
}));
$attr($must_check)
$extern fn_((proc_Mem_lockAll(proc_Mem self, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void));
$attr($must_check)
$extern fn_((proc_Mem_unlockAll(proc_Mem self))(proc_Mem_Unlock_E$void));

struct proc_Mem_VTbl {
    $attr($must_check)
    fn_(((*lockFn)(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
    $attr($must_check)
    fn_(((*unlockFn)(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void));
    $attr($must_check)
    fn_(((*lockAllFn)(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void));
    $attr($must_check)
    fn_(((*unlockAllFn)(P$raw ctx))(proc_Mem_Unlock_E$void));
};
$attr($must_check)
$extern fn_((proc_Mem_VTbl_failingLock(P$raw ctx, S_const$u8 mem, proc_Mem_LockOpts opts))(proc_Mem_Lock_E$void));
$attr($must_check)
$extern fn_((proc_Mem_VTbl_failingUnlock(P$raw ctx, S_const$u8 mem))(proc_Mem_Unlock_E$void));
$attr($must_check)
$extern fn_((proc_Mem_VTbl_failingLockAll(P$raw ctx, proc_Mem_LockAllOpts opts))(proc_Mem_Lock_E$void));
$attr($must_check)
$extern fn_((proc_Mem_VTbl_failingUnlockAll(P$raw ctx))(proc_Mem_Unlock_E$void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Mem_isValid(proc_Mem self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->lockFn)
        && isNonnull(self.vtbl->unlockFn)
        && isNonnull(self.vtbl->lockAllFn)
        && isNonnull(self.vtbl->unlockAllFn);
};
fn_((proc_Mem_assertValid(P$raw ctx, P_const$$(proc_Mem_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->lockFn);
    claim_assert_nonnull(vtbl->unlockFn);
    claim_assert_nonnull(vtbl->lockAllFn);
    claim_assert_nonnull(vtbl->unlockAllFn);
    ;
};
fn_((proc_Mem_ensureValid(proc_Mem self))(proc_Mem)) {
    return proc_Mem_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Mem__included */
