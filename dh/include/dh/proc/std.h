#pragma once
#ifndef proc_std__included
#define proc_std__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_std_Self_VTbl)(struct proc_std_Self_VTbl));
/// Copyable capability for the process standard files.
T_alias$((proc_std_Self)(struct proc_std_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_std_Self_VTbl));
}));
T_use_prl$(proc_std_Self);
$attr($inline_always)
$static fn_((proc_std_isValid(proc_std_Self self))(bool));
$attr($inline_always)
$static fn_((proc_std_assertValid(P$raw ctx, P_const$$(proc_std_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_std_ensureValid(proc_std_Self self))(proc_std_Self));

$extern fn_((proc_std_in(proc_std_Self self))(fs_File));
$extern fn_((proc_std_out(proc_std_Self self))(fs_File));
$extern fn_((proc_std_err(proc_std_Self self))(fs_File));

struct proc_std_Self_VTbl {
    fn_(((*inFn)(P$raw ctx))(fs_File));
    fn_(((*outFn)(P$raw ctx))(fs_File));
    fn_(((*errFn)(P$raw ctx))(fs_File));
};

/// Caller-owned native standard-stream provider.
T_alias$((proc_std_Direct)(struct proc_std_Direct {
    var_(in, fs_File);
    var_(out, fs_File);
    var_(err, fs_File);
}));
$extern fn_((proc_std_Direct_init(fs_File in, fs_File out, fs_File err))(proc_std_Direct));

T_alias$((proc_std_Files)(struct proc_std_Files {
    var_(in, fs_File);
    var_(out, fs_File);
    var_(err, fs_File);
}));
$extern fn_((proc_std_nativeFiles(void))(proc_std_Files));
$extern fn_((proc_std_Direct_initNative(void))(proc_std_Direct));
$extern fn_((proc_std_Direct_self(proc_std_Direct* self))(proc_std_Self));

/// Standard-stream disposition for a spawned child process.
T_alias$((proc_std_IO)(variant_((proc_std_IO $fits($packed))(
    (proc_std_IO_inherit, Void),
    (proc_std_IO_file, fs_File),
    (proc_std_IO_ignore, Void),
    (proc_std_IO_pipe, Void),
    (proc_std_IO_close, Void)
))));
$attr($inline_always)
$static fn_((proc_std_IO_fromFile(fs_File file))(proc_std_IO));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_std_isValid(proc_std_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->inFn)
        && isNonnull(self.vtbl->outFn)
        && isNonnull(self.vtbl->errFn);
};
fn_((proc_std_assertValid(P$raw ctx, P_const$$(proc_std_Self_VTbl) vtbl))(void)) {
    claim_assert(isNonnull(ctx));
    claim_assert(isNonnull(vtbl));
    claim_assert(isNonnull(vtbl->inFn));
    claim_assert(isNonnull(vtbl->outFn));
    claim_assert(isNonnull(vtbl->errFn));
};
fn_((proc_std_ensureValid(proc_std_Self self))(proc_std_Self)) {
    claim_assert(proc_std_isValid(self));
    return self;
};
fn_((proc_std_IO_fromFile(fs_File file))(proc_std_IO)) {
    return union_of$((proc_std_IO)(proc_std_IO_file)(file));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_std__included */
