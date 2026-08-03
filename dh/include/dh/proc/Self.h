/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-08-02 (date of last update)
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
#include "Child.h"
#include "../fs/Dir.h"

/*========== Macros and Declarations ========================================*/

/// Explicit child-process environment override.
///
/// `none()` inherits the environment present at process entry; `some(entries)`
/// replaces it with the exact `KEY=VALUE` entries in this slice.
T_alias$((proc_cmd_Env)(S$S_const$u8));
T_use_O$(proc_cmd_Env);
T_alias$((proc_cmd_CWD)(variant_((proc_cmd_CWD $fits($packed))(
    (proc_cmd_CWD_inherit, Void),
    (proc_cmd_CWD_dir, fs_Dir),
    (proc_cmd_CWD_path, S_const$u8)
))));
T_alias$((proc_cmd_StdIO)(variant_((proc_cmd_StdIO $fits($packed))(
    (proc_cmd_StdIO_inherit, Void),
    (proc_cmd_StdIO_file, fs_File),
    (proc_cmd_StdIO_ignore, Void),
    (proc_cmd_StdIO_pipe, Void),
    (proc_cmd_StdIO_close, Void)
))));
$attr($inline_always)
$static fn_((proc_cmd_StdIO_fromFile(fs_File file))(proc_cmd_StdIO));
T_alias$((proc_cmd_ArgExpsn)(enum_((proc_cmd_ArgExpsn $fits($packed))(
    proc_cmd_ArgExpsn_no_expand = 0,
    proc_cmd_ArgExpsn_expand
))));

/*--- Process-operation errors ---------------------------------------------*/

errset_((proc_FileBusyE)(proc_FileBusy));
errset_((proc_FileSysE)(proc_FileSys));
errset_((proc_FileNotFoundE)(proc_FileNotFound));
errset_((proc_InvalidExeE)(proc_InvalidExe));
errset_((proc_InvalidNameE)(proc_InvalidName));
errset_((proc_IsDirE)(proc_IsDir));
errset_((proc_NameTooLongE)(proc_NameTooLong));
errset_((proc_NoDeviceE)(proc_NoDevice));
errset_((proc_NotDirE)(proc_NotDir));
errset_((proc_OperationUnsupportedE)(proc_OperationUnsupported));
errset_((proc_PermissionDeniedE)(proc_PermissionDenied));
errset_((proc_FDQuotaExceededE)(proc_FdQuotaExceeded));
errset_((proc_SymLinkLoopE)(proc_SymLinkLoop));
errset_((proc_SysFDQuotaExceededE)(proc_SysFDQuotaExceeded));

/*--- Self capability -------------------------------------------------------*/

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

/*--- Executable and current-directory operations -------------------------*/

$attr($must_check)
$extern fn_((proc_openExe(proc_Self self, fs_File_OpenFlags flags))(E$fs_File));
errset_((proc_ExePath_E)() $union_errset_(
    proc_AccessDeniedE,
    proc_FileSysE,
    proc_FileNotFoundE,
    proc_OperationUnsupportedE,
    proc_ResrcLimitReachedE,
    proc_SymLinkLoopE,
    proc_SysResrcsE
));
T_use_E$($set(proc_ExePath_E)(S$u8));
$attr($must_check)
$extern fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExePath_E$S$u8));
errset_((proc_ExePathAlloc_E)() $union_errset_(
    proc_ExePath_E,
    mem_E
));
T_use_E$($set(proc_ExePathAlloc_E)(S$u8));
$attr($must_check)
$extern fn_((proc_exePathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExePathAlloc_E$S$u8));
$attr($must_check)
$extern fn_((proc_exeDirPath(proc_Self self, S$u8 out_buf))(proc_ExePath_E$S$u8));
$attr($must_check)
$extern fn_((proc_exeDirPathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExePathAlloc_E$S$u8));
errset_((proc_CurrPath_E)() $union_errset_(
    proc_AccessDeniedE,
    proc_FileNotFoundE,
    proc_OperationUnsupportedE,
    proc_ResrcLimitReachedE,
    proc_SysResrcsE
));
T_use_E$($set(proc_CurrPath_E)(S$u8));
$attr($must_check)
$extern fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrPath_E$S$u8));
errset_((proc_CurrPathAlloc_E)() $union_errset_(
    proc_CurrPath_E,
    mem_E
));
T_use_E$($set(proc_CurrPathAlloc_E)(S$u8));
$attr($must_check)
$extern fn_((proc_currPathAlloc(proc_Self self, mem_Alctr gpa))(proc_CurrPathAlloc_E$S$u8));
errset_((proc_SetCurrPath_E)() $union_errset_(
    mem_E,
    proc_AccessDeniedE,
    proc_FileNotFoundE,
    proc_FileSysE,
    proc_InvalidNameE,
    proc_NameTooLongE,
    proc_NotDirE,
    proc_OperationUnsupportedE,
    proc_ResrcLimitReachedE,
    proc_SymLinkLoopE,
    proc_SysResrcsE
));
$attr($must_check)
$extern fn_((proc_setCurrPath(proc_Self self, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void));
errset_((proc_SetCurrDir_E)() $union_errset_(
    mem_E,
    proc_AccessDeniedE,
    proc_FileSysE,
    proc_FileNotFoundE,
    proc_NotDirE,
    proc_OperationUnsupportedE,
    proc_ResrcLimitReachedE,
    proc_SysResrcsE
));
$attr($must_check)
$extern fn_((proc_setCurrDir(proc_Self self, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void));

/*--- Replace contract -----------------------------------------------------*/

errset_((proc_Replace_E)() $union_errset_(
    mem_E,
    proc_AccessDeniedE,
    proc_FileBusyE,
    proc_FileSysE,
    proc_FileNotFoundE,
    proc_InvalidExeE,
    proc_InvalidNameE,
    proc_IsDirE,
    proc_NameTooLongE,
    proc_NotDirE,
    proc_OperationUnsupportedE,
    proc_PermissionDeniedE,
    proc_FDQuotaExceededE,
    proc_ResrcLimitReachedE,
    proc_SysFDQuotaExceededE,
    proc_SysResrcsE
));
T_alias$((proc_Replace_Opts)(struct proc_Replace_Opts {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_cmd_Env);
    var_(expand_arg0, proc_cmd_ArgExpsn);
}));
$attr($inline_always)
$static fn_((proc_Replace_Opts_default(S$S_const$u8 argv))(proc_Replace_Opts));
$attr($must_check)
$extern fn_((proc_replace(
    proc_Self self, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts
))(proc_Replace_E$void));
$attr($must_check)
$extern fn_((proc_replacePath(
    proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts
))(proc_Replace_E$void));

/*--- Child-process operations ---------------------------------------------*/

errset_((proc_Spawn_E)() $union_errset_(
    proc_SetCurrPath_E,
    mem_E,
    proc_FileBusyE,
    proc_FileSysE,
    proc_InvalidExeE,
    proc_IsDirE,
    proc_NoDeviceE,
    proc_PermissionDeniedE,
    proc_FDQuotaExceededE,
    proc_SysFDQuotaExceededE
));
T_use_E$($set(proc_Spawn_E)(proc_Child));
T_alias$((proc_Spawn_Opts)(struct proc_Spawn_Opts {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_cmd_Env);
    var_(cwd, proc_cmd_CWD);
    var_(std_in, proc_cmd_StdIO);
    var_(std_out, proc_cmd_StdIO);
    var_(std_err, proc_cmd_StdIO);
    var_(expand_arg0, proc_cmd_ArgExpsn);
    var_(start_suspended, bool);
    var_(create_no_window, bool);
}));
$attr($inline_always)
$static fn_((proc_Spawn_Opts_default(S$S_const$u8 argv))(proc_Spawn_Opts));
$attr($must_check)
$extern fn_((proc_spawn(
    proc_Self self, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(
    proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child));

/*--- Run contract ----------------------------------------------------------*/

errset_((proc_Run_Local_E)(
    proc_Run_StreamTooLong,
    proc_Run_ReadFailed,
    proc_Run_ThreadFailed
));
errset_((proc_Run_E)() $union_errset_(
    proc_Spawn_E,
    proc_Child_Wait_E,
    proc_Run_Local_E,
    mem_E
));
T_alias$((proc_Run_Opts)(struct proc_Run_Opts {
    var_(spawn, proc_Spawn_Opts);
    var_(stdout_limit, usize);
    var_(stderr_limit, usize);
    var_(reserve_amount, usize);
}));
T_alias$((proc_Run_Res)(struct proc_Run_Res {
    var_(term, proc_Child_Trm);
    /// Caller-owned allocation from the allocator supplied to `proc_run`.
    var_(out, S$u8);
    /// Caller-owned allocation from the allocator supplied to `proc_run`.
    var_(err, S$u8);
}));
T_use_E$($set(proc_Run_E)(proc_Run_Res));
$attr($inline_always)
$static fn_((proc_Run_Opts_default(S$S_const$u8 argv))(proc_Run_Opts));
$attr($must_check)
$extern fn_((proc_run(
    proc_Self self, mem_Alctr gpa, proc_Env env, proc_Run_Opts opts
))(proc_Run_E$proc_Run_Res));

/*--- Process information --------------------------------------------------*/

errset_((proc_UserInfo_E)(
    proc_UserInfo_Unsupported,
    proc_UserInfo_UserNotFound,
    proc_UserInfo_CorruptDatabase,
    proc_UserInfo_SysResrcs
));
T_alias$((proc_UserInfo)(struct proc_UserInfo {
    var_(uid, u32);
    var_(gid, u32);
}));
T_use_prl$(proc_UserInfo);
T_use_E$($set(proc_UserInfo_E)(proc_UserInfo));
$attr($must_check)
$extern fn_((proc_userInfo(proc_Self self, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo));
errset_((proc_BaseAddr_E)(
    proc_BaseAddr_Unsupported,
    proc_BaseAddr_SysResrcs,
    proc_BaseAddr_CorruptImage
));
T_use_E$($set(proc_BaseAddr_E)(usize));
$attr($must_check)
$extern fn_((proc_baseAddr(proc_Self self))(proc_BaseAddr_E$usize));

/*--- Process termination --------------------------------------------------*/

$attr($no_return)
$extern fn_((proc_exit(proc_Self self, u8 status))(void));
/// In debug builds this returns so normal cleanup and leak checks execute.
/// In non-debug builds this terminates successfully through `self`.
$attr(debug_unless($no_return))
$extern fn_((proc_cleanExit(proc_Self self))(void));
$attr($no_return $branch_cold)
$extern fn_((proc_abort(proc_Self self))(void));

struct proc_Self_VTbl {
    /*--- Executable and current-directory operations ---------------------*/

    $attr($must_check)
    fn_(((*openExeFn)(P$raw ctx, fs_File_OpenFlags flags))(E$fs_File));
    $attr($must_check)
    fn_(((*exePathFn)(P$raw ctx, S$u8 out_buf))(proc_ExePath_E$S$u8));
    $attr($must_check)
    fn_(((*currPathFn)(P$raw ctx, S$u8 out_buf))(proc_CurrPath_E$S$u8));
    $attr($must_check)
    fn_(((*setCurrPathFn)(P$raw ctx, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void));
    $attr($must_check)
    fn_(((*setCurrDirFn)(P$raw ctx, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void));
    $attr($must_check)
    fn_(((*replaceFn)(
        P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts
    ))(proc_Replace_E$void));
    $attr($must_check)
    fn_(((*replacePathFn)(
        P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts
    ))(proc_Replace_E$void));

    /*--- Child-process operations -----------------------------------------*/

    $attr($must_check)
    fn_(((*spawnFn)(
        P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    $attr($must_check)
    fn_(((*spawnPathFn)(
        P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    var_(child, struct proc_Self_VTbl_Child {
        $attr($must_check)
        fn_(((*waitFn)(P$raw ctx, P$$(proc_Child) child))(proc_Child_Wait_E$proc_Child_Trm));
        fn_(((*killFn)(P$raw ctx, P$$(proc_Child) child))(void));
    });

    /*--- Process information ----------------------------------------------*/

    $attr($must_check)
    fn_(((*userInfoFn)(P$raw ctx, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo));
    $attr($must_check)
    fn_(((*baseAddrFn)(P$raw ctx))(proc_BaseAddr_E$usize));

    /*--- Process termination ----------------------------------------------*/

    $attr($no_return)
    fn_(((*exitFn)(P$raw ctx, u8 status))(void));
    $attr($no_return)
    fn_(((*abortFn)(P$raw ctx))(void));
};

$attr($must_check)
$extern fn_((proc_VTbl_failingOpenExe(P$raw ctx, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((proc_VTbl_failingExePath(P$raw ctx, S$u8 out_buf))(proc_ExePath_E$S$u8));
$attr($must_check)
$extern fn_((proc_VTbl_failingCurrPath(P$raw ctx, S$u8 out_buf))(proc_CurrPath_E$S$u8));
$attr($must_check)
$extern fn_((proc_VTbl_failingSetCurrPath(P$raw ctx, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void));
$attr($must_check)
$extern fn_((proc_VTbl_failingSetCurrDir(P$raw ctx, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void));
$attr($must_check)
$extern fn_((proc_VTbl_failingReplace(P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts))(proc_Replace_E$void));
$attr($must_check)
$extern fn_((proc_VTbl_failingReplacePath(P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts))(proc_Replace_E$void));

$attr($must_check)
$extern fn_((proc_VTbl_failingSpawn(P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_VTbl_failingSpawnPath(P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts cmd))(proc_Spawn_E$proc_Child));
$attr($must_check)
$extern fn_((proc_VTbl_Child_failingWait(P$raw ctx, P$$(proc_Child) child))(proc_Child_Wait_E$proc_Child_Trm));
$extern fn_((proc_VTbl_Child_noKill(P$raw ctx, P$$(proc_Child) child))(void));
$extern fn_((proc_VTbl_Child_unreachableKill(P$raw ctx, P$$(proc_Child) child))(void));

$attr($must_check)
$extern fn_((proc_VTbl_failingUserInfo(P$raw ctx, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo));
$attr($must_check)
$extern fn_((proc_VTbl_failingBaseAddr(P$raw ctx))(proc_BaseAddr_E$usize));

$attr($no_return)
$extern fn_((proc_VTbl_noExit(P$raw ctx, u8 status))(void));
$attr($no_return)
$extern fn_((proc_VTbl_unreachableExit(P$raw ctx, u8 status))(void));
$attr($no_return)
$extern fn_((proc_VTbl_noAbort(P$raw ctx))(void));
$attr($no_return)
$extern fn_((proc_VTbl_unreachableAbort(P$raw ctx))(void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_cmd_StdIO_fromFile(fs_File file))(proc_cmd_StdIO)) {
    return union_of$((proc_cmd_StdIO)(proc_cmd_StdIO_file)(file));
};
fn_((proc_Replace_Opts_default(S$S_const$u8 argv))(proc_Replace_Opts)) {
    return (proc_Replace_Opts){
        .argv = argv,
        .env = none$((O$proc_cmd_Env)),
        .expand_arg0 = proc_cmd_ArgExpsn_no_expand,
    };
};
fn_((proc_Spawn_Opts_default(S$S_const$u8 argv))(proc_Spawn_Opts)) {
    return (proc_Spawn_Opts){
        .argv = argv,
        .env = none$((O$proc_cmd_Env)),
        .cwd = union_of((proc_cmd_CWD_inherit){}),
        .std_in = union_of((proc_cmd_StdIO_inherit){}),
        .std_out = union_of((proc_cmd_StdIO_inherit){}),
        .std_err = union_of((proc_cmd_StdIO_inherit){}),
        .expand_arg0 = proc_cmd_ArgExpsn_no_expand,
        .start_suspended = false,
        .create_no_window = false,
    };
};
fn_((proc_Run_Opts_default(S$S_const$u8 argv))(proc_Run_Opts)) {
    return (proc_Run_Opts){
        .spawn = proc_Spawn_Opts_default(argv),
        .stdout_limit = usize_limit_max,
        .stderr_limit = usize_limit_max,
        .reserve_amount = usize_(64),
    };
};
fn_((proc_isValid(proc_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->openExeFn)
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
        && isNonnull(self.vtbl->userInfoFn)
        && isNonnull(self.vtbl->baseAddrFn)
        && isNonnull(self.vtbl->exitFn)
        && isNonnull(self.vtbl->abortFn);
};
fn_((proc_assertValid(P$raw ctx, P_const$$(proc_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->openExeFn);
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
    claim_assert_nonnull(vtbl->userInfoFn);
    claim_assert_nonnull(vtbl->baseAddrFn);
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
