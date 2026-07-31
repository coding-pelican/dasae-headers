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
 * @brief   Copyable injectable interface to the current process
 */
#pragma once
#ifndef proc_Self__included
#define proc_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Child.h"
#include "Cmd.h"
#include "Env.h"
#include "../mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Self_VTbl)(struct proc_Self_VTbl));
T_alias$((proc_Self)(struct proc_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Self_VTbl));
}));
T_use_prl$(proc_Self);
T_use_E$($set(proc_Direct_E)(proc_Self));
$attr($must_check)
$extern fn_((proc_Child_wait(
    proc_Self provider,
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Ter));
$extern fn_((proc_Child_kill(proc_Self provider, proc_Child* self))(void));
$attr($inline_always)
$static fn_((proc_isValid(proc_Self self))(bool));
$attr($inline_always)
$static fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_ensureValid(proc_Self self))(proc_Self));

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
$extern fn_((proc_setCurrDir(proc_Self self, fs_Dir dir))(proc_SetCurrentDir_E$void));
$attr($must_check)
$extern fn_((proc_setCurrPath(proc_Self self, S_const$u8 path))(proc_SetCurrentPath_E$void));
$attr($must_check)
$extern fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_replace(proc_Self self, proc_Replace_Opts opts))(proc_Replace_E$void));
$attr($must_check)
$extern fn_((proc_replacePath(proc_Self self, fs_Dir dir, proc_Replace_Opts opts))(proc_Replace_E$void));

struct proc_Self_VTbl {
    $attr($must_check)
    fn_(((*exePathFn)(P$raw ctx, S$u8 out_buf))(proc_ExecutablePath_E$S$u8));
    $attr($must_check)
    fn_(((*currPathFn)(P$raw ctx, S$u8 out_buf))(proc_CurrentPath_E$S$u8));
    $attr($must_check)
    fn_(((*setCurrDirFn)(P$raw ctx, fs_Dir dir))(proc_SetCurrentDir_E$void));
    $attr($must_check)
    fn_(((*setCurrPathFn)(P$raw ctx, S_const$u8 path))(proc_SetCurrentPath_E$void));
    $attr($must_check)
    fn_(((*spawnFn)(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*spawnPathFn)(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*replaceFn)(P$raw ctx, proc_Replace_Opts opts))(proc_Replace_E$void));
    $attr($must_check)
    fn_(((*replacePathFn)(P$raw ctx, fs_Dir dir, proc_Replace_Opts opts))(proc_Replace_E$void));

    T_embed$(struct proc_Self_VTbl_Child {
        $attr($must_check)
        fn_(((*waitFn)(
            P$raw ctx,
            proc_Child* self
        ))(proc_Child_Wait_E$proc_Child_Ter));
        fn_(((*killFn)(P$raw ctx, proc_Child* self))(void));
    });
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_isValid(proc_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->exePathFn)
        && isNonnull(self.vtbl->currPathFn)
        && isNonnull(self.vtbl->setCurrDirFn)
        && isNonnull(self.vtbl->setCurrPathFn)
        && isNonnull(self.vtbl->spawnFn)
        && isNonnull(self.vtbl->spawnPathFn)
        && isNonnull(self.vtbl->replaceFn)
        && isNonnull(self.vtbl->replacePathFn)
        && isNonnull(self.vtbl->waitFn)
        && isNonnull(self.vtbl->killFn);
};
fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->exePathFn);
    claim_assert_nonnull(vtbl->currPathFn);
    claim_assert_nonnull(vtbl->setCurrDirFn);
    claim_assert_nonnull(vtbl->setCurrPathFn);
    claim_assert_nonnull(vtbl->spawnFn);
    claim_assert_nonnull(vtbl->spawnPathFn);
    claim_assert_nonnull(vtbl->replaceFn);
    claim_assert_nonnull(vtbl->replacePathFn);
    claim_assert_nonnull(vtbl->waitFn);
    claim_assert_nonnull(vtbl->killFn);
};
fn_((proc_ensureValid(proc_Self self))(proc_Self)) {
    return proc_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Self__included */
