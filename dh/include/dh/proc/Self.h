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

/*========== Macros and Declarations ========================================*/

#define proc_Direct_supported pp_or(plat_is_windows, plat_is_linux)

T_alias$((proc_Self_VTbl)(struct proc_Self_VTbl));
T_alias$((proc_Self)(struct proc_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Self_VTbl));
}));
T_use_prl$(proc_Self);
T_use_E$($set(proc_Direct_E)(proc_Self));
$attr($inline_always)
$static fn_((proc_isValid(proc_Self self))(bool));
$attr($inline_always)
$static fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_ensureValid(proc_Self self))(proc_Self));

$attr($must_check)
$extern fn_((proc_executablePath(proc_Self self, S$u8 out_buf))(proc_Path_E$S$u8));
$attr($must_check)
$extern fn_((proc_currentPath(proc_Self self, S$u8 out_buf))(proc_Path_E$S$u8));
$attr($must_check)
$extern fn_((proc_setCurrentPath(proc_Self self, S_const$u8 path))(proc_Path_E$void));
$attr($must_check)
$extern fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_replace(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$void));
$attr($must_check)
$extern fn_((proc_replacePath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$void));

/// Native process provider whose borrowed environment is used when a child
/// inherits the current process environment.
T_alias$((proc_Direct)(struct proc_Direct {
    var_(env, proc_Env);
}));
$attr($inline_always)
$static fn_((proc_Direct_init(proc_Env env))(proc_Direct));
/// Return the platform capability backed by `self`.
$attr($must_check)
$extern fn_((proc_Direct_self(proc_Direct* self))(proc_Direct_E$proc_Self));

struct proc_Self_VTbl {
    $attr($must_check)
    fn_(((*executablePathFn)(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8));
    $attr($must_check)
    fn_(((*currentPathFn)(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8));
    $attr($must_check)
    fn_(((*setCurrentPathFn)(P$raw ctx, S_const$u8 path))(proc_Path_E$void));
    $attr($must_check)
    fn_(((*spawnFn)(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*spawnPathFn)(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*replaceFn)(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$void));
    $attr($must_check)
    fn_(((*replacePathFn)(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$void));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_isValid(proc_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->executablePathFn)
        && isNonnull(self.vtbl->currentPathFn)
        && isNonnull(self.vtbl->setCurrentPathFn)
        && isNonnull(self.vtbl->spawnFn)
        && isNonnull(self.vtbl->spawnPathFn)
        && isNonnull(self.vtbl->replaceFn)
        && isNonnull(self.vtbl->replacePathFn);
};
fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->executablePathFn);
    claim_assert_nonnull(vtbl->currentPathFn);
    claim_assert_nonnull(vtbl->setCurrentPathFn);
    claim_assert_nonnull(vtbl->spawnFn);
    claim_assert_nonnull(vtbl->spawnPathFn);
    claim_assert_nonnull(vtbl->replaceFn);
    claim_assert_nonnull(vtbl->replacePathFn);
};
fn_((proc_ensureValid(proc_Self self))(proc_Self)) {
    return proc_assertValid(self.ctx, self.vtbl), self;
};
fn_((proc_Direct_init(proc_Env env))(proc_Direct)) {
    return (proc_Direct){ .env = env };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Self__included */
