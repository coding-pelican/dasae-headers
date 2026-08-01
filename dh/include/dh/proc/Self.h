/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc
 *
 * @brief   Copyable injectable process-operation capability
 */
#pragma once
#ifndef proc_Self__included
#define proc_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Env.h"
#include "Stream.h"
#include "Child.h"
#include "../mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

/// Explicit child-process environment override.
///
/// `none()` inherits the environment present at process entry; `some(entries)`
/// replaces it with the exact `KEY=VALUE` entries in this slice.
T_alias$((proc_Cmd_Env)(S$S_const$u8));
T_use_O$(proc_Cmd_Env);
T_alias$((proc_Cmd_CWD)(variant_((proc_Cmd_CWD $fits($packed))(
    (proc_Cmd_CWD_inherit, Void),
    (proc_Cmd_CWD_dir, fs_Dir),
    (proc_Cmd_CWD_path, S_const$u8)
))));
T_alias$((proc_Cmd_ArgExpsn)(enum_((proc_Cmd_ArgExpsn $fits($packed))(
    proc_Cmd_ArgExpsn_no_expand = 0,
    proc_Cmd_ArgExpsn_expand
))));
T_alias$((proc_Cmd)(struct proc_Cmd {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_Cmd_Env);
    var_(cwd, proc_Cmd_CWD);
    var_(std_in, proc_Stream);
    var_(std_out, proc_Stream);
    var_(std_err, proc_Stream);
    var_(expand_arg0, proc_Cmd_ArgExpsn);
    var_(start_suspended, bool);
    var_(create_no_window, bool);
}));
T_alias$((proc_Cmd_Replace_Opts)(struct proc_Cmd_Replace_Opts {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_Cmd_Env);
    var_(expand_arg0, proc_Cmd_ArgExpsn);
}));

T_alias$((proc_Self_VTbl)(struct proc_Self_VTbl));
struct proc_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Self_VTbl));
};
T_use_prl$(proc_Self);
T_use_E$($set(proc_direct_E)(proc_Self));
$attr($inline_always)
$static fn_((proc_isValid(proc_Self self))(bool));
$attr($inline_always)
$static fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_ensureValid(proc_Self self))(proc_Self));

$extern let_(proc_noop, proc_Self);
$extern let_(proc_failing, proc_Self);
$attr($must_check)
$extern fn_((proc_direct(void))(proc_direct_E$proc_Self));

$attr($must_check)
$extern fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExecutablePath_E$S$u8));
$attr($must_check)
$extern fn_((proc_exePathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExecutablePathAlloc_E$S$u8));
$attr($must_check)
$extern fn_((proc_exeDirPath(proc_Self self, S$u8 out_buf))(proc_ExecutablePath_E$S$u8));
$attr($must_check)
$extern fn_((proc_exeDirPathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExecutablePathAlloc_E$S$u8));
$attr($must_check)
$extern fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrentPath_E$S$u8));
$attr($must_check)
$extern fn_((proc_currPathAlloc(proc_Self self, mem_Alctr gpa))(proc_CurrentPathAlloc_E$S$u8));
$attr($must_check)
$extern fn_((proc_setCurrPath(proc_Self self, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrentPath_E$void));
$attr($must_check)
$extern fn_((proc_setCurrDir(proc_Self self, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrentDir_E$void));
$attr($must_check)
$extern fn_((proc_replace(proc_Self self, mem_Alctr gpa, proc_Env env, proc_Cmd_Replace_Opts opts))(proc_Replace_E$void));
$attr($must_check)
$extern fn_((proc_replacePath(proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Cmd_Replace_Opts opts))(proc_Replace_E$void));

$attr($must_check)
$extern fn_((proc_spawn(proc_Self self, mem_Alctr gpa, proc_Env env, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));

$attr($no_return)
$extern fn_((proc_exit(proc_Self self, u8 status))(void));
$attr($no_return $branch_cold)
$extern fn_((proc_abort(proc_Self self))(void));

struct proc_Self_VTbl {
    $attr($must_check)
    fn_(((*exePathFn)(P$raw ctx, S$u8 out_buf))(proc_ExecutablePath_E$S$u8));
    $attr($must_check)
    fn_(((*currPathFn)(P$raw ctx, S$u8 out_buf))(proc_CurrentPath_E$S$u8));
    $attr($must_check)
    fn_(((*setCurrPathFn)(P$raw ctx, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrentPath_E$void));
    $attr($must_check)
    fn_(((*setCurrDirFn)(P$raw ctx, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrentDir_E$void));
    $attr($must_check)
    fn_(((*replaceFn)(P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Cmd_Replace_Opts opts))(proc_Replace_E$void));
    $attr($must_check)
    fn_(((*replacePathFn)(P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Cmd_Replace_Opts opts))(proc_Replace_E$void));

    $attr($must_check)
    fn_(((*spawnFn)(P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*spawnPathFn)(P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    var_(child, struct proc_Self_VTbl_Child {
        $attr($must_check)
        fn_(((*waitFn)(P$raw ctx, P$$(proc_Child) child))(proc_Child_Wait_E$proc_Child_Trm));
        fn_(((*killFn)(P$raw ctx, P$$(proc_Child) child))(void));
    });

    $attr($no_return)
    fn_(((*exitFn)(P$raw ctx, u8 status))(void));
    $attr($no_return)
    fn_(((*abortFn)(P$raw ctx))(void));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_isValid(proc_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->exePathFn)
        && isNonnull(self.vtbl->currPathFn)
        && isNonnull(self.vtbl->setCurrPathFn)
        && isNonnull(self.vtbl->setCurrDirFn)
        && isNonnull(self.vtbl->replaceFn)
        && isNonnull(self.vtbl->replacePathFn)
        && isNonnull(self.vtbl->spawnFn)
        && isNonnull(self.vtbl->spawnPathFn)
        && isNonnull(self.vtbl->child.waitFn)
        && isNonnull(self.vtbl->child.killFn)
        && isNonnull(self.vtbl->exitFn)
        && isNonnull(self.vtbl->abortFn);
};
fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->exePathFn);
    claim_assert_nonnull(vtbl->currPathFn);
    claim_assert_nonnull(vtbl->setCurrPathFn);
    claim_assert_nonnull(vtbl->setCurrDirFn);
    claim_assert_nonnull(vtbl->replaceFn);
    claim_assert_nonnull(vtbl->replacePathFn);
    claim_assert_nonnull(vtbl->spawnFn);
    claim_assert_nonnull(vtbl->spawnPathFn);
    claim_assert_nonnull(vtbl->child.waitFn);
    claim_assert_nonnull(vtbl->child.killFn);
    claim_assert_nonnull(vtbl->exitFn);
    claim_assert_nonnull(vtbl->abortFn);
};
fn_((proc_ensureValid(proc_Self self))(proc_Self)) {
    return proc_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Self__included */
