#pragma once
#ifndef proc_mem__included
#define proc_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../heap/common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_mem_Lock_Opts)(struct proc_mem_Lock_Opts {
    var_(on_fault, bool);
}));
$static const proc_mem_Lock_Opts proc_mem_Lock_Opts_default = {
    .on_fault = false,
};

T_alias$((proc_mem_LockAll_Opts)(struct proc_mem_LockAll_Opts {
    var_(current, bool);
    var_(future, bool);
    var_(on_fault, bool);
}));
$static const proc_mem_LockAll_Opts proc_mem_LockAll_Opts_default = {
    .current = false,
    .future = false,
    .on_fault = false,
};

T_alias$((proc_mem_Self_VTbl)(struct proc_mem_Self_VTbl));
/// Copyable capability for process-wide memory residency policy.
T_alias$((proc_mem_Self)(struct proc_mem_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_mem_Self_VTbl));
    var_(geom, heap_Geom);
}));
T_use_prl$(proc_mem_Self);
$attr($inline_always)
$static fn_((proc_mem_isValid(proc_mem_Self self))(bool));
$attr($inline_always)
$static fn_((proc_mem_ensureValid(proc_mem_Self self))(proc_mem_Self));

$extern fn_((proc_mem_geom(proc_mem_Self self))(heap_Geom));
$attr($must_check)
$extern fn_((proc_mem_lock(proc_mem_Self self, S_const$u8 memory, proc_mem_Lock_Opts opts))(proc_mem_Lock_E$void));
$attr($must_check)
$extern fn_((proc_mem_unlock(proc_mem_Self self, S_const$u8 memory))(proc_mem_Unlock_E$void));
$attr($must_check)
$extern fn_((proc_mem_lockAll(proc_mem_Self self, proc_mem_LockAll_Opts opts))(proc_mem_Lock_E$void));
$attr($must_check)
$extern fn_((proc_mem_unlockAll(proc_mem_Self self))(proc_mem_Unlock_E$void));

struct proc_mem_Self_VTbl {
    $attr($must_check)
    fn_(((*lockFn)(P$raw ctx, S_const$u8 memory, proc_mem_Lock_Opts opts))(proc_mem_Lock_E$void));
    $attr($must_check)
    fn_(((*unlockFn)(P$raw ctx, S_const$u8 memory))(proc_mem_Unlock_E$void));
    $attr($must_check)
    fn_(((*lockAllFn)(P$raw ctx, proc_mem_LockAll_Opts opts))(proc_mem_Lock_E$void));
    $attr($must_check)
    fn_(((*unlockAllFn)(P$raw ctx))(proc_mem_Unlock_E$void));
};

/// Caller-owned native process-memory provider.
T_alias$((proc_mem_Direct)(struct proc_mem_Direct {
    var_(geom, heap_Geom);
}));
$attr($inline_always)
$static fn_((proc_mem_Direct_init(heap_Geom geom))(proc_mem_Direct));
$extern fn_((proc_mem_Direct_self(proc_mem_Direct* self))(proc_mem_Self));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_mem_isValid(proc_mem_Self self))(bool)) {
    return heap_Geom_isValid(self.geom)
        && isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->lockFn)
        && isNonnull(self.vtbl->unlockFn)
        && isNonnull(self.vtbl->lockAllFn)
        && isNonnull(self.vtbl->unlockAllFn);
};
fn_((proc_mem_ensureValid(proc_mem_Self self))(proc_mem_Self)) {
    claim_assert(proc_mem_isValid(self));
    return self;
};
fn_((proc_mem_Direct_init(heap_Geom geom))(proc_mem_Direct)) {
    return (proc_mem_Direct){ .geom = heap_Geom_ensureValid(geom) };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_mem__included */
