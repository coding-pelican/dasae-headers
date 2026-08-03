#include "dh/proc/Self.h"
#include "private/share.h"
#include "dh/ascii.h"
#include "dh/fmt/common.h"
#include "dh/fs/path.h"
#include "dh/io/Fixed.h"
#include "dh/io/Reader.h"
#include "dh/mem/common.h"
#include "dh/start.h"
#include "dh/unicode.h"
#include "dh/ArrList.h"
#include "dh/clsr.h"
#include "dh/thrd/Self.h"

/*========== Internal Declarations ==========================================*/

#define proc__path_alloc_initial usize_(256)

T_use$((i32)(
    Clsr_Ctx, Clsr_Rtn, Clsr,
    thrd_spawn, thrd_join
));

typedef enum proc__RunReadStatus {
    proc__RunReadStatus_ok = 0,
    proc__RunReadStatus_too_long,
    proc__RunReadStatus_read_failed,
    proc__RunReadStatus_out_of_memory,
} proc__RunReadStatus;

typedef struct proc__RunReadCtx {
    var_(reader, io_Reader);
    var_(gpa, mem_Alctr);
    var_(limit, usize);
    var_(reserve_amount, usize);
    var_(bytes, ArrList);
    var_(status, proc__RunReadStatus);
} proc__RunReadCtx;

$static fn_((proc__runRead(proc__RunReadCtx* ctx))(i32));
fn_use_Clsr_((proc__runRead)(proc__RunReadCtx*)(i32));
$static fn_((proc__runTakeBytes(proc__RunReadCtx* ctx))(S$u8));
$attr($must_check)
$static fn_((proc__runReadStatusErr(proc__RunReadStatus status))(proc_Run_E$void));

/*--- Windows target leaves -------------------------------------------------*/

#if plat_is_windows
$attr($must_check)
$static fn_((proc__windows_openExe(P$raw ctx, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$static fn_((proc__windows_exePath(P$raw ctx, S$u8 out_buf))(proc_ExePath_E$S$u8));
$attr($must_check)
$static fn_((proc__windows_currPath(P$raw ctx, S$u8 out_buf))(proc_CurrPath_E$S$u8));
$attr($must_check)
$static fn_((proc__windows_setCurrPath(P$raw ctx, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void));
$attr($must_check)
$static fn_((proc__windows_setCurrDir(P$raw ctx, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void));
$attr($must_check)
$static fn_((proc__windows_spawn(
    P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$static fn_((proc__windows_spawnPath(
    P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$static fn_((proc__windows_child_wait(
    P$raw ctx, P$$(proc_Child) child
))(proc_Child_Wait_E$proc_Child_Trm));
$static fn_((proc__windows_child_kill(P$raw ctx, P$$(proc_Child) child))(void));
$attr($must_check)
$static fn_((proc__windows_baseAddr(P$raw ctx))(proc_BaseAddr_E$usize));
$attr($no_return $branch_cold)
$static fn_((proc__windows_abort(P$raw ctx))(void));
#endif /* plat_is_windows */

/*--- Linux target leaves ---------------------------------------------------*/

#if plat_is_linux
$attr($must_check)
$static fn_((proc__linux_openExe(P$raw ctx, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$static fn_((proc__linux_exePath(P$raw ctx, S$u8 out_buf))(proc_ExePath_E$S$u8));
$attr($must_check)
$static fn_((proc__linux_currPath(P$raw ctx, S$u8 out_buf))(proc_CurrPath_E$S$u8));
$attr($must_check)
$static fn_((proc__linux_setCurrPath(P$raw ctx, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void));
$attr($must_check)
$static fn_((proc__linux_setCurrDir(P$raw ctx, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void));
$attr($must_check)
$static fn_((proc__linux_replace(
    P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts
))(proc_Replace_E$void));
$attr($must_check)
$static fn_((proc__linux_replacePath(
    P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts
))(proc_Replace_E$void));
$attr($must_check)
$static fn_((proc__linux_spawn(
    P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$static fn_((proc__linux_spawnPath(
    P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$static fn_((proc__linux_child_wait(
    P$raw ctx, P$$(proc_Child) child
))(proc_Child_Wait_E$proc_Child_Trm));
$static fn_((proc__linux_child_kill(P$raw ctx, P$$(proc_Child) child))(void));
$attr($must_check)
$static fn_((proc__linux_userInfo(P$raw ctx, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo));
$attr($must_check)
$static fn_((proc__linux_baseAddr(P$raw ctx))(proc_BaseAddr_E$usize));
$attr($no_return $branch_cold)
$static fn_((proc__linux_abort(P$raw ctx))(void));
#endif /* plat_is_linux */

$static fn_((proc__childClear(proc_Child* self))(void));

$static let proc_direct__openExe = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_openExe)),
    pp_case_((plat_type_linux)(proc__linux_openExe)),
    pp_default_(()(proc_VTbl_failingOpenExe))
));
$static let proc_direct__exePath = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_exePath)),
    pp_case_((plat_type_linux)(proc__linux_exePath)),
    pp_default_(()(proc_VTbl_failingExePath))
));
$static let proc_direct__currPath = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_currPath)),
    pp_case_((plat_type_linux)(proc__linux_currPath)),
    pp_default_(()(proc_VTbl_failingCurrPath))
));
$static let proc_direct__setCurrPath = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_setCurrPath)),
    pp_case_((plat_type_linux)(proc__linux_setCurrPath)),
    pp_default_(()(proc_VTbl_failingSetCurrPath))
));
$static let proc_direct__setCurrDir = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_setCurrDir)),
    pp_case_((plat_type_linux)(proc__linux_setCurrDir)),
    pp_default_(()(proc_VTbl_failingSetCurrDir))
));
$static let proc_direct__replace = pp_switch_((plat_type)(
    pp_case_((plat_type_linux)(proc__linux_replace)),
    pp_default_(()(proc_VTbl_failingReplace))
));
$static let proc_direct__replacePath = pp_switch_((plat_type)(
    pp_case_((plat_type_linux)(proc__linux_replacePath)),
    pp_default_(()(proc_VTbl_failingReplacePath))
));
$static let proc_direct__spawn = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_spawn)),
    pp_case_((plat_type_linux)(proc__linux_spawn)),
    pp_default_(()(proc_VTbl_failingSpawn))
));
$static let proc_direct__spawnPath = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_spawnPath)),
    pp_case_((plat_type_linux)(proc__linux_spawnPath)),
    pp_default_(()(proc_VTbl_failingSpawnPath))
));
$static let proc_direct__child_wait = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_child_wait)),
    pp_case_((plat_type_linux)(proc__linux_child_wait)),
    pp_default_(()(proc_VTbl_Child_failingWait))
));
$static let proc_direct__child_kill = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_child_kill)),
    pp_case_((plat_type_linux)(proc__linux_child_kill)),
    pp_default_(()(proc_VTbl_Child_noKill))
));
$static let proc_direct__userInfo = pp_switch_((plat_type)(
    pp_case_((plat_type_linux)(proc__linux_userInfo)),
    pp_default_(()(proc_VTbl_failingUserInfo))
));
$static let proc_direct__baseAddr = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_baseAddr)),
    pp_case_((plat_type_linux)(proc__linux_baseAddr)),
    pp_default_(()(proc_VTbl_failingBaseAddr))
));
$attr($no_return)
$static fn_((proc_direct__exit(P$raw ctx, u8 status))(void));
$static let proc_direct__abort = pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(proc__windows_abort)),
    pp_case_((plat_type_linux)(proc__linux_abort)),
    pp_default_(()(proc_VTbl_noAbort))
));

/*========== External Definitions ===========================================*/

$static var_(proc__ctx_noop, Void) $undefined_static;
$static let_(proc__vtbl_noop, proc_Self_VTbl) = {
    .openExeFn = proc_VTbl_failingOpenExe,
    .exePathFn = proc_VTbl_failingExePath,
    .currPathFn = proc_VTbl_failingCurrPath,
    .setCurrPathFn = proc_VTbl_failingSetCurrPath,
    .setCurrDirFn = proc_VTbl_failingSetCurrDir,
    .replaceFn = proc_VTbl_failingReplace,
    .replacePathFn = proc_VTbl_failingReplacePath,
    .spawnFn = proc_VTbl_failingSpawn,
    .spawnPathFn = proc_VTbl_failingSpawnPath,
    .child = {
        .waitFn = proc_VTbl_Child_failingWait,
        .killFn = proc_VTbl_Child_noKill,
    },
    .userInfoFn = proc_VTbl_failingUserInfo,
    .baseAddrFn = proc_VTbl_failingBaseAddr,
    .exitFn = proc_VTbl_noExit,
    .abortFn = proc_VTbl_noAbort,
};
let_(proc_noop, proc_Self) = {
    .ctx = &proc__ctx_noop,
    .vtbl = &proc__vtbl_noop,
};

$static var_(proc__ctx_failing, Void) $undefined_static;
$static let_(proc__vtbl_failing, proc_Self_VTbl) = {
    .openExeFn = proc_VTbl_failingOpenExe,
    .exePathFn = proc_VTbl_failingExePath,
    .currPathFn = proc_VTbl_failingCurrPath,
    .setCurrPathFn = proc_VTbl_failingSetCurrPath,
    .setCurrDirFn = proc_VTbl_failingSetCurrDir,
    .replaceFn = proc_VTbl_failingReplace,
    .replacePathFn = proc_VTbl_failingReplacePath,
    .spawnFn = proc_VTbl_failingSpawn,
    .spawnPathFn = proc_VTbl_failingSpawnPath,
    .child = {
        .waitFn = proc_VTbl_Child_failingWait,
        .killFn = proc_VTbl_Child_unreachableKill,
    },
    .userInfoFn = proc_VTbl_failingUserInfo,
    .baseAddrFn = proc_VTbl_failingBaseAddr,
    .exitFn = proc_VTbl_unreachableExit,
    .abortFn = proc_VTbl_unreachableAbort,
};
let_(proc_failing, proc_Self) = {
    .ctx = &proc__ctx_failing,
    .vtbl = &proc__vtbl_failing,
};

fn_((proc_direct(void))(proc_direct_E$proc_Self) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_linux))(
        pp_then_({
            $static var_(ctx, Void) $undefined_static;
            $static let_(vtbl, proc_Self_VTbl) = {
                .openExeFn = proc_direct__openExe,
                .exePathFn = proc_direct__exePath,
                .currPathFn = proc_direct__currPath,
                .setCurrPathFn = proc_direct__setCurrPath,
                .setCurrDirFn = proc_direct__setCurrDir,
                .replaceFn = proc_direct__replace,
                .replacePathFn = proc_direct__replacePath,
                .spawnFn = proc_direct__spawn,
                .spawnPathFn = proc_direct__spawnPath,
                .child = {
                    .waitFn = proc_direct__child_wait,
                    .killFn = proc_direct__child_kill,
                },
                .userInfoFn = proc_direct__userInfo,
                .baseAddrFn = proc_direct__baseAddr,
                .exitFn = proc_direct__exit,
                .abortFn = proc_direct__abort,
            };
            return_ok(proc_ensureValid((proc_Self){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$proc_direct_Unsupported());
        })
    );
} $unscoped(fn);

/*--- Executable and current-directory operations -------------------------*/

fn_((proc_openExe(proc_Self self, fs_File_OpenFlags flags))(E$fs_File)) {
    self = proc_ensureValid(self);
    return self.vtbl->openExeFn(self.ctx, flags);
};
fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExePath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->exePathFn(self.ctx, out_buf);
};
fn_((proc_exePathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExePathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_alloc_initial));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    while (true) {
        let path = catch_((proc_exePath(self, path_buf))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResrcLimitReached().as_any)) {
                return_err(err);
            }
            try_(proc__growScratch(&path_buf, gpa));
            continue;
        }));
        return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
    }
} $unguarded(fn);
fn_((proc_exeDirPath(proc_Self self, S$u8 out_buf))(proc_ExePath_E$S$u8) $scope) {
    let path = try_(proc_exePath(self, out_buf));
    let dir = fs_path_dirname(path.as_const);
    return_ok(S_prefix((out_buf)(dir.len)));
} $unscoped(fn);
fn_((proc_exeDirPathAlloc(proc_Self self, mem_Alctr gpa))(proc_ExePathAlloc_E$S$u8) $guard) {
    var path = try_(proc_exePathAlloc(self, gpa));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path));
    let dir = fs_path_dirname(path.as_const);
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path, dir.len)));
} $unguarded(fn);
fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrPath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->currPathFn(self.ctx, out_buf);
};
fn_((proc_currPathAlloc(proc_Self self, mem_Alctr gpa))(proc_CurrPathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_alloc_initial));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    while (true) {
        let path = catch_((proc_currPath(self, path_buf))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResrcLimitReached().as_any)) {
                return_err(err);
            }
            try_(proc__growScratch(&path_buf, gpa));
            continue;
        }));
        return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
    }
} $unguarded(fn);
fn_((proc_setCurrPath(proc_Self self, mem_Alctr gpa, S_const$u8 path))(proc_SetCurrPath_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrPathFn(self.ctx, gpa, path);
};
fn_((proc_setCurrDir(proc_Self self, mem_Alctr gpa, fs_Dir dir))(proc_SetCurrDir_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrDirFn(self.ctx, gpa, dir);
};
fn_((proc_replace(proc_Self self, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replaceFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        opts
    );
};
fn_((proc_replacePath(proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replacePathFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        dir,
        opts
    );
};

/*--- Child-process operations ---------------------------------------------*/

fn_((proc_spawn(
    proc_Self self, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        cmd
    );
};
fn_((proc_spawnPath(
    proc_Self self, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnPathFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        dir,
        cmd
    );
};
fn_((proc_run(
    proc_Self self, mem_Alctr gpa, proc_Env env, proc_Run_Opts opts
))(proc_Run_E$proc_Run_Res) $guard) {
    self = proc_ensureValid(self);
    gpa = mem_Alctr_ensureValid(gpa);
    env = proc_Env_ensureValid(env);

    let_(stdio_ignore, proc_cmd_StdIO) = union_of((proc_cmd_StdIO_ignore){});
    let_(stdio_pipe, proc_cmd_StdIO) = union_of((proc_cmd_StdIO_pipe){});
    opts.spawn.std_in = stdio_ignore;
    opts.spawn.std_out = stdio_pipe;
    opts.spawn.std_err = stdio_pipe;

    var child = try_(proc_spawn(self, gpa, env, opts.spawn));
    errdefer_($ignore, proc_Child_kill(&child, self));

    var_(out_ctx, proc__RunReadCtx) = {
        .reader = fs_File_reader(unwrap_(child.io.out)),
        .gpa = gpa,
        .limit = opts.stdout_limit,
        .reserve_amount = opts.reserve_amount,
        .bytes = ArrList_empty(typeInfo$(u8)),
        .status = proc__RunReadStatus_ok,
    };
    var_(err_ctx, proc__RunReadCtx) = {
        .reader = fs_File_reader(unwrap_(child.io.err)),
        .gpa = gpa,
        .limit = opts.stderr_limit,
        .reserve_amount = opts.reserve_amount,
        .bytes = ArrList_empty(typeInfo$(u8)),
        .status = proc__RunReadStatus_ok,
    };
    defer_(ArrList_clearAndFree(&out_ctx.bytes, typeInfo$(u8), gpa));
    defer_(ArrList_clearAndFree(&err_ctx.bytes, typeInfo$(u8), gpa));

    var out_clsr = clsr_((proc__runRead)(&out_ctx));
    var err_clsr = clsr_((proc__runRead)(&err_ctx));
    let cfg = (thrd_SpawnCfg){
        .gpa = gpa,
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
    let out_worker = catch_((thrd_spawn$i32(cfg, out_clsr.as_base))(
        $ignore, return_err(E_cause$proc_Run_ThreadFailed())
    ));
    let err_worker = catch_((thrd_spawn$i32(cfg, err_clsr.as_base))($ignore, {
        proc_Child_kill(&child, self);
        let_ignore = thrd_join$i32(out_worker);
        return_err(E_cause$proc_Run_ThreadFailed());
    }));
    let_ignore = thrd_join$i32(out_worker);
    let_ignore = thrd_join$i32(err_worker);

    try_(proc__runReadStatusErr(out_ctx.status));
    try_(proc__runReadStatusErr(err_ctx.status));
    let term = try_(proc_Child_wait(&child, self));

    return_ok((proc_Run_Res){
        .term = term,
        .out = proc__runTakeBytes(&out_ctx),
        .err = proc__runTakeBytes(&err_ctx),
    });
} $unguarded(fn);

/*--- Process information --------------------------------------------------*/

fn_((proc_userInfo(proc_Self self, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo)) {
    self = proc_ensureValid(self);
    return self.vtbl->userInfoFn(self.ctx, name);
};
fn_((proc_baseAddr(proc_Self self))(proc_BaseAddr_E$usize)) {
    self = proc_ensureValid(self);
    return self.vtbl->baseAddrFn(self.ctx);
};

/*--- Process termination --------------------------------------------------*/

fn_((proc_exit(proc_Self self, u8 status))(void)) {
    self = proc_ensureValid(self);
    self.vtbl->exitFn(self.ctx, status);
    claim_unreachable;
};
fn_((proc_cleanExit(proc_Self self))(void)) {
    pp_if_(debug_enabled)(
        pp_then_(let_ignore = self; return;),
        pp_else_(proc_exit(self, 0)));
};
fn_((proc_abort(proc_Self self))(void)) {
    self = proc_ensureValid(self);
    self.vtbl->abortFn(self.ctx);
    claim_unreachable;
};

/*========== Internal Definitions ===========================================*/

/*--- Run and collect -------------------------------------------------------*/

fn_((proc__runRead(proc__RunReadCtx* ctx))(i32) $guard) {
    claim_assert_nonnull(ctx);
    ctx->bytes = ArrList_empty(typeInfo$(u8));
    ctx->status = proc__RunReadStatus_ok;
    if (ctx->reserve_amount != 0) {
        catch_((ArrList_ensureCapPrecise(
            &ctx->bytes, typeInfo$(u8), ctx->gpa, ctx->reserve_amount
        ))($ignore, {
            ctx->status = proc__RunReadStatus_out_of_memory;
        }));
    }

    var_(scratch, A$$(2048, u8)) $undefined;
    while (true) {
        let n = catch_((io_Reader_read(
            ctx->reader, A_ref$((S$u8)(scratch))
        ))($ignore, {
            ctx->status = proc__RunReadStatus_read_failed;
            return 1;
        }));
        if (n == 0) break;
        if (ctx->status != proc__RunReadStatus_ok) continue;
        let next_len = orelse_((usize_addChkd(ArrList_len(ctx->bytes), n))({
            ctx->status = proc__RunReadStatus_out_of_memory;
            continue;
        }));
        if (next_len > ctx->limit) {
            ctx->status = proc__RunReadStatus_too_long;
            continue;
        }
        let chunk = A_prefix$((S_const$u8)(scratch)(n));
        catch_((ArrList_appendS(
            &ctx->bytes, ctx->gpa, u_anyS(chunk)
        ))($ignore, {
            ctx->status = proc__RunReadStatus_out_of_memory;
        }));
    }
    return 0;
} $unguarded(fn);
fn_((proc__runTakeBytes(proc__RunReadCtx* ctx))(S$u8)) {
    claim_assert_nonnull(ctx);
    let len = ArrList_len(ctx->bytes);
    ArrList_shrinkAndFree(&ctx->bytes, typeInfo$(u8), ctx->gpa, len);
    let bytes = u_castS$((S$u8)(ArrList_itemsMut(ctx->bytes, typeInfo$(u8))));
    ctx->bytes = ArrList_empty(typeInfo$(u8));
    return bytes;
};
fn_((proc__runReadStatusErr(proc__RunReadStatus status))(proc_Run_E$void) $scope) {
    switch (status) {
    case_((proc__RunReadStatus_ok)) return_ok({}) $end(case);
    case_((proc__RunReadStatus_too_long)) return_err(E_cause$proc_Run_StreamTooLong()) $end(case);
    case_((proc__RunReadStatus_read_failed)) return_err(E_cause$proc_Run_ReadFailed()) $end(case);
    case_((proc__RunReadStatus_out_of_memory)) return_err(E_cause$OutOfMemory()) $end(case);
    default_() claim_unreachable $end(default);
    }
} $unscoped(fn);

/*--- Direct capability adapters -------------------------------------------*/

fn_((proc_direct__exit(P$raw ctx, u8 status))(void)) {
    let_ignore = ensureNonnull(ctx);
    start_exit(status);
};
/*--- Common child state ----------------------------------------------------*/

fn_((proc__childClear(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->io.in)(file)) fs_File_close(file);
    if_some((self->io.out)(file)) fs_File_close(file);
    if_some((self->io.err)(file)) fs_File_close(file);
    asg_l((&self->io.in)(none()));
    asg_l((&self->io.out)(none()));
    asg_l((&self->io.err)(none()));
    asg_l((&self->handle)(none()));
    self->id = 0;
};

/*--- Default vtable implementations ---------------------------------------*/

fn_((proc_VTbl_failingOpenExe(
    P$raw ctx,
    fs_File_OpenFlags flags
))(E$fs_File) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = flags;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingExePath(
    P$raw ctx,
    S$u8 out_buf
))(proc_ExePath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingCurrPath(
    P$raw ctx,
    S$u8 out_buf
))(proc_CurrPath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingSetCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = path;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingSetCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = dir;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingReplace(
    P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Replace_Opts opts
))(proc_Replace_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = proc_Env_ensureValid(env);
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingReplacePath(
    P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Replace_Opts opts
))(proc_Replace_E$void) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = proc_Env_ensureValid(env);
    let_ignore = dir;
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingSpawn(
    P$raw ctx, mem_Alctr gpa, proc_Env env, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = proc_Env_ensureValid(env);
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingSpawnPath(
    P$raw ctx, mem_Alctr gpa, proc_Env env, fs_Dir dir, proc_Spawn_Opts opts
))(proc_Spawn_E$proc_Child) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = mem_Alctr_ensureValid(gpa);
    let_ignore = proc_Env_ensureValid(env);
    let_ignore = dir;
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_VTbl_Child_failingWait(
    P$raw ctx,
    P$$(proc_Child) child
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(child);
    return_err(E_cause$proc_SysResrcs());
} $unscoped(fn);

fn_((proc_VTbl_Child_noKill(P$raw ctx, P$$(proc_Child) child))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(child);
};

fn_((proc_VTbl_Child_unreachableKill(
    P$raw ctx,
    P$$(proc_Child) child
))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(child);
    claim_unreachable;
};

fn_((proc_VTbl_failingUserInfo(
    P$raw ctx,
    S_const$u8 name
))(proc_UserInfo_E$proc_UserInfo) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = name;
    return_err(E_cause$proc_UserInfo_Unsupported());
} $unscoped(fn);

fn_((proc_VTbl_failingBaseAddr(
    P$raw ctx
))(proc_BaseAddr_E$usize) $scope) {
    let_ignore = ensureNonnull(ctx);
    return_err(E_cause$proc_BaseAddr_Unsupported());
} $unscoped(fn);

fn_((proc_VTbl_noExit(P$raw ctx, u8 status))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = status;
    claim_unreachable;
};
fn_((proc_VTbl_unreachableExit(P$raw ctx, u8 status))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = status;
    claim_unreachable;
};
fn_((proc_VTbl_noAbort(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((proc_VTbl_unreachableAbort(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

/*--- Windows ---*/

#if plat_is_windows
#include "dh/sys/api/windows/dll.h"
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/nt.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/sync.h"

#define proc__windows_path_max usize_(32768)
#define proc__windows_path_prefix_len usize_(4)

typedef struct proc__windows_ResolvedStdIO {
    var_(child, O$proc_Handle);
    var_(parent_pipe, O$fs_File);
    var_(needs_close_child, bool);
} proc__windows_ResolvedStdIO;
T_use_E$(proc__windows_ResolvedStdIO);

typedef struct proc__windows_OwnedBuf {
    var_(gpa, mem_Alctr);
    var_(ptr, O$P$u8);
    var_(len, usize);
    var_(alloc_len, usize);
} proc__windows_OwnedBuf;
T_use_E$(proc__windows_OwnedBuf);

typedef struct proc__windows_OwnedWideBuf {
    var_(gpa, mem_Alctr);
    var_(ptr, O$P$u16);
    var_(len, usize);
    var_(alloc_len, usize);
} proc__windows_OwnedWideBuf;
T_use_E$(proc__windows_OwnedWideBuf);

/*--- Declarations ---*/

$attr($must_check)
$static fn_((proc__windows_alloc(mem_Alctr gpa, usize len))(O$P$u8));
$static fn_((proc__windows_free(mem_Alctr gpa, P$u8 ptr, usize len))(void));
$attr($must_check)
$static fn_((proc__windows_mapError(DWORD err))(EAny));
$attr($must_check)
$static fn_((proc__windows_mapPathQueryError(DWORD err))(EAny));
$attr($must_check)
$static fn_((proc__windows_mapSetPathError(DWORD err))(EAny));
$attr($must_check)
$static fn_((proc__windows_dupInheritable(HANDLE src))(E$proc_Handle));
$attr($must_check)
$static fn_((proc__windows_stdioNull(bool for_read))(E$proc_Handle));
$static fn_((proc__windows_ResolvedStdIO_fini(proc__windows_ResolvedStdIO* self))(void));
$static fn_((proc__windows_OwnedBuf_fini(proc__windows_OwnedBuf* self))(void));
$static fn_((proc__windows_OwnedWideBuf_fini(proc__windows_OwnedWideBuf* self))(void));
$attr($must_check)
$static fn_((proc__windows_wideAlloc(mem_Alctr gpa, usize len))(O$P$u16));
$attr($must_check)
$static fn_((proc__windows_wtf8ZAlloc(
    mem_Alctr gpa, S_const$u8 src
))(E$proc__windows_OwnedWideBuf));
$attr($must_check)
$static fn_((proc__windows_wtf8OptZAlloc(
    mem_Alctr gpa, O$S_const$u8 src
))(E$proc__windows_OwnedWideBuf));
$attr($must_check)
$static fn_((proc__windows_dupSliceZ(
    mem_Alctr gpa, S_const$u8 src
))(E$proc__windows_OwnedBuf));
$static fn_((proc__windows_trimNtPrefix(proc__windows_OwnedBuf* self))(void));
$attr($must_check)
$static fn_((proc__windows_dirPathAlloc(
    mem_Alctr gpa, fs_Dir dir
))(E$proc__windows_OwnedBuf));
$attr($must_check)
$static fn_((proc__windows_resolvePathAlloc(
    mem_Alctr gpa, S_const$u8 base, S_const$u8 sub_path
))(E$proc__windows_OwnedBuf));
$attr($must_check)
$static fn_((proc__windows_resolveStdIO(
    proc_cmd_StdIO spec, O$fs_File inherited, DWORD std_id
))(E$proc__windows_ResolvedStdIO));
$attr($must_check)
$static fn_((proc__windows_stdFile(DWORD std_id))(O$fs_File));
$attr($must_check)
$static fn_((proc__windows_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 application_name,
    O$S_const$u8 current_dir
))(proc_Spawn_E$proc_Child));

/*--- Definitions ---*/

fn_((proc__windows_alloc(mem_Alctr gpa, usize len))(O$P$u8)) {
    return mem_Alctr_rawAlloc($trace gpa, len, alignOfLog2$(u16));
};

fn_((proc__windows_free(mem_Alctr gpa, P$u8 ptr, usize len))(void)) {
    claim_assert_nonnull(ptr);
    mem_Alctr_rawFree(
        $trace gpa,
        P_prefix$((S$u8)(ptr)(len)),
        alignOfLog2$(u16)
    );
};

fn_((proc__windows_mapError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_BAD_FORMAT: $fallthrough;
    case ERROR_BAD_EXE_FORMAT: $fallthrough;
    case ERROR_EXE_MACHINE_TYPE_MISMATCH: return E_cause$proc_InvalidExe().any;
    case ERROR_SHARING_VIOLATION: return E_cause$proc_FileBusy().any;
    case ERROR_INVALID_NAME: return E_cause$proc_InvalidName().any;
    case ERROR_DIRECTORY: return E_cause$proc_NotDir().any;
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$proc_SysResrcs().any;
    case ERROR_TOO_MANY_OPEN_FILES: return E_cause$proc_FdQuotaExceeded().any;
    case ERROR_SHARING_BUFFER_EXCEEDED: return E_cause$proc_ResrcLimitReached().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__windows_mapPathQueryError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_INSUFFICIENT_BUFFER: return E_cause$proc_ResrcLimitReached().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__windows_mapSetPathError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_INVALID_NAME: return E_cause$proc_InvalidName().any;
    case ERROR_DIRECTORY: return E_cause$proc_NotDir().any;
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$proc_SysResrcs().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__windows_dupInheritable(HANDLE src))(E$proc_Handle) $scope) {
    var_(dup, HANDLE) = INVALID_HANDLE_VALUE;
    if (!DuplicateHandle(
            GetCurrentProcess(), src,
            GetCurrentProcess(), &dup,
            0, TRUE, DUPLICATE_SAME_ACCESS
        )) {
        return_err(proc__windows_mapError(GetLastError()));
    }
    return_ok(as$(proc_Handle)(dup));
} $unscoped(fn);

fn_((proc__windows_stdioNull(bool for_read))(E$proc_Handle) $guard) {
    let path = for_read ? "NUL" : "NUL";
    let access = for_read ? GENERIC_READ : GENERIC_WRITE;
    let handle = CreateFileA(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE, null, OPEN_EXISTING, 0, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(proc__windows_mapError(GetLastError()));
    defer_(claim_assert(CloseHandle(handle)));
    let inheritable = try_(proc__windows_dupInheritable(handle));
    return_ok(inheritable);
} $unguarded(fn);

fn_((proc__windows_ResolvedStdIO_fini(proc__windows_ResolvedStdIO* self))(void)) {
    claim_assert_nonnull(self);
    if (self->needs_close_child) {
        if_some((self->child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
        asg_l((&self->child)(none()));
    }
    if_some((self->parent_pipe)(file)) fs_File_close(file);
    asg_l((&self->parent_pipe)(none()));
};

fn_((proc__windows_OwnedBuf_fini(proc__windows_OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->ptr)(ptr)) proc__windows_free(self->gpa, ptr, self->alloc_len);
    asg_l((&self->ptr)(none()));
    self->len = 0;
    self->alloc_len = 0;
};

fn_((proc__windows_OwnedWideBuf_fini(proc__windows_OwnedWideBuf* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->ptr)(ptr)) {
        proc__windows_free(self->gpa, ptrCast$((P$u8)(ptr)), self->alloc_len);
    }
    asg_l((&self->ptr)(none()));
    self->len = 0;
    self->alloc_len = 0;
};

fn_((proc__windows_wideAlloc(mem_Alctr gpa, usize len))(O$P$u16) $scope) {
    let byte_len = orelse_((usize_mulChkd(len, sizeOf$(u16)))(return_none()));
    let ptr = proc__windows_alloc(gpa, byte_len);
    if_none(ptr) return_none();
    return_some(ptrAlignCast$((P$u16)(unwrap_(ptr))));
} $unscoped(fn);

fn_((proc__windows_wtf8ZAlloc(mem_Alctr gpa, S_const$u8 src))(E$proc__windows_OwnedWideBuf) $scope) {
    let text_len = unicode_wtf8ToWTF16Len(src);
    let alloc_len = orelse_((usize_addChkd(text_len, 1))(return_err(E_cause$proc_SysResrcs())));
    let alloc_bytes = orelse_((usize_mulChkd(alloc_len, sizeOf$(u16)))(return_err(E_cause$proc_SysResrcs())));
    let buf = orelse_((proc__windows_wideAlloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    let out = P_prefix$((S$u16)(buf)(alloc_len));
    let converted = catch_((unicode_wtf8ToWTF16Within(src, S_prefix((out)(text_len))))(
        $ignore,
        {
            proc__windows_free(gpa, ptrCast$((P$u8)(buf)), alloc_bytes);
            return_err(E_cause$proc_InvalidName());
        }
    ));
    claim_assert(converted.len == text_len);
    *S_at((out)[text_len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = text_len,
        .alloc_len = alloc_bytes,
    });
} $unscoped(fn);

fn_((proc__windows_wtf8OptZAlloc(mem_Alctr gpa, O$S_const$u8 src))(E$proc__windows_OwnedWideBuf) $scope) {
    if_none(src) {
        return_ok({
            .gpa = gpa,
            .ptr = none(),
            .len = 0,
            .alloc_len = 0,
        });
    }
    return_(proc__windows_wtf8ZAlloc(gpa, unwrap_(src)));
} $unscoped(fn);

fn_((proc__windows_dupSliceZ(mem_Alctr gpa, S_const$u8 src))(E$proc__windows_OwnedBuf) $scope) {
    let alloc_len = src.len + 1;
    let buf = orelse_((proc__windows_alloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    let out = P_prefix$((S$u8)(buf)(alloc_len));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = src.len,
        .alloc_len = alloc_len,
    });
} $unscoped(fn);

fn_((proc__windows_trimNtPrefix(proc__windows_OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if (self->len < proc__windows_path_prefix_len) return;
    let path = P_prefix$((S$u8)(unwrap_(self->ptr))(self->len + 1));
    if (
        *S_at((path)[0]) == u8_c('\\') && *S_at((path)[1]) == u8_c('\\') && *S_at((path)[2]) == u8_c('?') && *S_at((path)[3]) == u8_c('\\')
    ) {
        mem_moveBytes(
            S_prefix((path)(self->len + 1 - proc__windows_path_prefix_len)),
            S_slice((path.as_const)$r(proc__windows_path_prefix_len, self->len + 1))
        );
        self->len -= proc__windows_path_prefix_len;
    }
};

fn_((proc__windows_dirPathAlloc(mem_Alctr gpa, fs_Dir dir))(E$proc__windows_OwnedBuf) $guard) {
    let need = GetFinalPathNameByHandleA(dir.handle, null, 0, 0);
    if (need == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    let alloc_len = as$(usize)(need) + 1;
    let buf = orelse_((proc__windows_alloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, proc__windows_free(gpa, buf, alloc_len));
    let wrote = GetFinalPathNameByHandleA(dir.handle, as$(LPSTR)(buf), need + 1, 0);
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    var_(owned, proc__windows_OwnedBuf) = {
        .gpa = gpa,
        .ptr = some(buf),
        .len = as$(usize)(wrote),
        .alloc_len = alloc_len,
    };
    proc__windows_trimNtPrefix(&owned);
    return_ok(owned);
} $unguarded(fn);

fn_((proc__windows_resolvePathAlloc(mem_Alctr gpa, S_const$u8 base, S_const$u8 sub_path))(E$proc__windows_OwnedBuf) $guard) {
    if (fs_path_isAbs(sub_path)) return_(proc__windows_dupSliceZ(gpa, sub_path));
    let base_sub_len = orelse_((usize_addChkd(base.len, sub_path.len))(return_err(E_cause$proc_SysResrcs())));
    let cap = orelse_((usize_addChkd(base_sub_len, usize_(2)))(return_err(E_cause$proc_SysResrcs())));
    let buf = orelse_((proc__windows_alloc(gpa, cap))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, proc__windows_free(gpa, buf, cap));
    let out = P_prefix$((S$u8)(buf)(cap));
    let joined = catch_((fs_path_join2(base, sub_path, S_prefix((out)(cap - 1))))($ignore, {
        return_err(E_cause$proc_SysResrcs());
    }));
    *S_at((out)[joined.len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = joined.len,
        .alloc_len = cap,
    });
} $unguarded(fn);

fn_((proc__windows_resolveStdIO(proc_cmd_StdIO spec, O$fs_File inherited, DWORD std_id))(E$proc__windows_ResolvedStdIO) $scope) {
    let for_read = std_id == STD_INPUT_HANDLE;
    if (matches(spec, proc_cmd_StdIO_inherit)) {
        if_none(inherited) {
            return_ok({
                .child = none(),
                .parent_pipe = none(),
                .needs_close_child = false,
            });
        }
        let child = try_(proc__windows_dupInheritable(fs_File_handle(unwrap_(inherited))));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (matches(spec, proc_cmd_StdIO_file)) {
        let child = try_(proc__windows_dupInheritable(fs_File_handle(union_to((spec)(proc_cmd_StdIO_file)))));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (matches(spec, proc_cmd_StdIO_close)) {
        return_ok({
            .child = none(),
            .parent_pipe = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_cmd_StdIO_ignore)) {
        let child = try_(proc__windows_stdioNull(for_read));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (!matches(spec, proc_cmd_StdIO_pipe)) return_err(E_cause$proc_OperationUnsupported());

    SECURITY_ATTRIBUTES sa = {
        .nLength = sizeOf$(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = null,
        .bInheritHandle = TRUE,
    };
    var_(read_end, HANDLE) = INVALID_HANDLE_VALUE;
    var_(write_end, HANDLE) = INVALID_HANDLE_VALUE;
    if (!CreatePipe(&read_end, &write_end, &sa, 0)) {
        return_err(proc__windows_mapError(GetLastError()));
    }
    if (for_read) {
        claim_assert(SetHandleInformation(write_end, HANDLE_FLAG_INHERIT, 0));
        return_ok({
            .child = some(as$(proc_Handle)(read_end)),
            .parent_pipe = some(fs_File_Handle_promote(write_end, fs_File_Flags_default)),
            .needs_close_child = true,
        });
    }
    claim_assert(SetHandleInformation(read_end, HANDLE_FLAG_INHERIT, 0));
    return_ok({
        .child = some(as$(proc_Handle)(write_end)),
        .parent_pipe = some(fs_File_Handle_promote(read_end, fs_File_Flags_default)),
        .needs_close_child = true,
    });
} $unscoped(fn);

fn_((proc__windows_stdFile(DWORD std_id))(O$fs_File)) {
    let handle = GetStdHandle(std_id);
    if (handle == null || handle == INVALID_HANDLE_VALUE) {
        return none$((O$fs_File));
    }
    return some$((O$fs_File)(fs_File_Handle_promote(
        handle,
        fs_File_Flags_default
    )));
};

fn_((proc__windows_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 application_name,
    O$S_const$u8 current_dir
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var cmdline_wide = try_(proc__cmdLineWTF16(gpa, cmd.argv));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(cmdline_wide)));
    var env_block = try_(proc__envWTF16(
        gpa,
        env,
        cmd.env
    ));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(env_block)));
    var_(application_name_wide, proc__windows_OwnedWideBuf) = try_(
        proc__windows_wtf8OptZAlloc(gpa, application_name)
    );
    errdefer_($ignore, proc__windows_OwnedWideBuf_fini(&application_name_wide));
    var_(current_dir_wide, proc__windows_OwnedWideBuf) = try_(
        proc__windows_wtf8OptZAlloc(gpa, current_dir)
    );
    errdefer_($ignore, proc__windows_OwnedWideBuf_fini(&current_dir_wide));

    var_(std_in, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_in,
        proc__windows_stdFile(STD_INPUT_HANDLE),
        STD_INPUT_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_in));
    var_(std_out, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_out,
        proc__windows_stdFile(STD_OUTPUT_HANDLE),
        STD_OUTPUT_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_out));
    var_(std_err, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_err,
        proc__windows_stdFile(STD_ERROR_HANDLE),
        STD_ERROR_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_err));

    STARTUPINFOW startup = cleared();
    startup.cb = sizeOf$(STARTUPINFOW);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = as$(HANDLE)(orelse_((std_in.child)(as$(proc_Handle)(null))));
    startup.hStdOutput = as$(HANDLE)(orelse_((std_out.child)(as$(proc_Handle)(null))));
    startup.hStdError = as$(HANDLE)(orelse_((std_err.child)(as$(proc_Handle)(null))));

    PROCESS_INFORMATION proc_info = cleared();
    var_(flags, DWORD) = CREATE_UNICODE_ENVIRONMENT;
    if (cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessW(
            isSome(application_name_wide.ptr) ? as$(LPCWSTR)(unwrap_(application_name_wide.ptr)) : null,
            as$(LPWSTR)(cmdline_wide.ptr),
            null,
            null,
            TRUE,
            flags,
            env_block.ptr,
            isSome(current_dir_wide.ptr) ? as$(LPCWSTR)(unwrap_(current_dir_wide.ptr)) : null,
            &startup,
            &proc_info
        )) {
        return_err(proc__windows_mapError(GetLastError()));
    }

    claim_assert(CloseHandle(proc_info.hThread));
    mem_Alctr_free($trace gpa, u_anyS(cmdline_wide));
    mem_Alctr_free($trace gpa, u_anyS(env_block));
    var cleanup_application_name = application_name_wide;
    proc__windows_OwnedWideBuf_fini(&cleanup_application_name);
    var cleanup_current_dir = current_dir_wide;
    proc__windows_OwnedWideBuf_fini(&cleanup_current_dir);
    if (std_in.needs_close_child) if_some((std_in.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_out.needs_close_child) if_some((std_out.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_err.needs_close_child) if_some((std_err.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));

    return_ok({
        .handle = some(as$(proc_Child_Handle)(proc_info.hProcess)),
        .id = as$(proc_Child_Id)(proc_info.dwProcessId),
        .io = {
            .in = std_in.parent_pipe,
            .out = std_out.parent_pipe,
            .err = std_err.parent_pipe,
        },
    });
} $unguarded(fn);

fn_((proc__windows_openExe(
    P$raw ctx,
    fs_File_OpenFlags flags
))(E$fs_File) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (flags.nonblocking) return_err(E_cause$fs_Unsupported());

    let teb = ensureNonnull(NtCurrentTeb());
    let peb = ensureNonnull(teb->ProcessEnvironmentBlock);
    let params = ensureNonnull(peb->ProcessParameters);
    let image_path = params->ImagePathName;
    let path = ensureNonnull(image_path.Buffer);
    claim_assert(image_path.Length % sizeOf$(u16) == 0);
    claim_assert(image_path.MaximumLength >= image_path.Length + sizeOf$(u16));
    let path_len = as$(usize)(image_path.Length) / sizeOf$(u16);
    claim_assert(*P_at((path)[path_len]) == u16_c('\0'));

    var_(access, DWORD) = 0;
    if (fs_File_OpenFlags_isRead(flags)) access |= GENERIC_READ;
    if (fs_File_OpenFlags_isWrite(flags)) access |= GENERIC_WRITE;
    let handle = CreateFileW(
        path,
        access,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        null,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        null
    );
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$fs_OpenFailed());
    return_ok(fs_File_Handle_promote(
        handle,
        (fs_File_Flags){
            .nonblocking = flags.nonblocking,
        }
    ));
} $unscoped(fn);
fn_((proc__windows_exePath(
    P$raw ctx,
    S$u8 out_buf
))(proc_ExePath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    if (out_buf.len == 0) return_err(E_cause$proc_ResrcLimitReached());
    let wrote = GetModuleFileNameA(null, as$(LPSTR)(out_buf.ptr), as$(DWORD)(out_buf.len));
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResrcLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

fn_((proc__windows_currPath(
    P$raw ctx,
    S$u8 out_buf
))(proc_CurrPath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let wrote = GetCurrentDirectoryA(as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr));
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResrcLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

fn_((proc__windows_setCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $guard) {
    claim_assert_nonnull(ctx);
    var_(path_z, proc__windows_OwnedBuf) = try_(proc__windows_dupSliceZ(gpa, path));
    defer_(proc__windows_OwnedBuf_fini(&path_z));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(unwrap_(path_z.ptr)))) {
        return_err(proc__windows_mapSetPathError(GetLastError()));
    }
    return_ok({});
} $unguarded(fn);

fn_((proc__windows_setCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $guard) {
    claim_assert_nonnull(ctx);
    var_(path, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(gpa, dir));
    defer_(proc__windows_OwnedBuf_fini(&path));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(unwrap_(path.ptr)))) {
        return_err(proc__windows_mapSetPathError(GetLastError()));
    }
    return_ok({});
} $unguarded(fn);

fn_((proc__windows_spawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    if (matches(cmd.cwd, proc_cmd_CWD_inherit)) {
        return_(proc__windows_spawnImpl(
            ctx,
            gpa,
            env,
            cmd,
            none$((O$S_const$u8)),
            none$((O$S_const$u8))
        ));
    }
    if (matches(cmd.cwd, proc_cmd_CWD_path)) {
        return_(proc__windows_spawnImpl(
            ctx,
            gpa,
            env,
            cmd,
            none$((O$S_const$u8)),
            some$((O$S_const$u8)(union_to((cmd.cwd)(proc_cmd_CWD_path))))
        ));
    }
    var_(cwd, proc__windows_OwnedBuf) = try_(
        proc__windows_dirPathAlloc(
            gpa,
            union_to((cmd.cwd)(proc_cmd_CWD_dir))
        )
    );
    defer_(proc__windows_OwnedBuf_fini(&cwd));
    let child = try_(proc__windows_spawnImpl(
        ctx,
        gpa,
        env,
        cmd,
        none$((O$S_const$u8)),
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(cwd.ptr), .len = cwd.len }))
    ));
    return_ok(child);
} $unguarded(fn);

fn_((proc__windows_spawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    var_(base, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(gpa, dir));
    defer_(proc__windows_OwnedBuf_fini(&base));
    var_(exe_path, proc__windows_OwnedBuf) = try_(proc__windows_resolvePathAlloc(
        gpa,
        P_prefix$((S$u8)(unwrap_(base.ptr))(base.len)).as_const,
        *S_at((cmd.argv)[0])
    ));
    defer_(proc__windows_OwnedBuf_fini(&exe_path));
    let child = try_(proc__windows_spawnImpl(
        ctx,
        gpa,
        env,
        cmd,
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(exe_path.ptr), .len = exe_path.len })),
        none$((O$S_const$u8))
    ));
    return_ok(child);
} $unguarded(fn);

fn_((proc__windows_child_wait(P$raw ctx, P$$(proc_Child) child))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    let_ignore = ensureNonnull(ctx);
    child = ensureNonnull(child);
    let handle = unwrap_(child->handle);
    switch (WaitForSingleObject(as$(HANDLE)(handle), INFINITE)) {
    case WAIT_OBJECT_0: break;
    case WAIT_ABANDONED: $fallthrough;
    case WAIT_TIMEOUT: $fallthrough;
    case WAIT_FAILED: {
        if (GetLastError() == ERROR_ACCESS_DENIED) {
            return_err(E_cause$proc_AccessDenied());
        }
        return_err(E_cause$proc_SysResrcs());
    };
    default_() return_err(E_cause$proc_SysResrcs()) $end(default);
    }
    var_(exit_code, DWORD) = 0;
    if (!GetExitCodeProcess(as$(HANDLE)(handle), &exit_code)) {
        return_err(E_cause$proc_SysResrcs());
    }
    claim_assert(CloseHandle(as$(HANDLE)(handle)));
    proc__childClear(child);
    return_ok(union_of$(
        (proc_Child_Trm)(proc_Child_Trm_exited)(as$(u8)(exit_code))
    ));
} $unscoped(fn);

fn_((proc__windows_child_kill(
    P$raw ctx,
    P$$(proc_Child) child
))(void)) {
    let_ignore = ensureNonnull(ctx);
    child = ensureNonnull(child);
    if_some((child->handle)(handle)) {
        let_ignore = TerminateProcess(as$(HANDLE)(handle), 1);
        let_ignore = catch_((proc__windows_child_wait(ctx, child))(
            $ignore,
            $do_nothing
        ));
    };
};

fn_((proc__windows_baseAddr(P$raw ctx))(proc_BaseAddr_E$usize) $scope) {
    claim_assert_nonnull(ctx);
    let module = GetModuleHandleW(null);
    if (isNull(module)) {
        return_err(E_cause$proc_BaseAddr_SysResrcs());
    }
    return_ok(as$(usize)(module));
} $unscoped(fn);

fn_((proc__windows_abort(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = TerminateProcess(GetCurrentProcess(), 3);
    start_exit(134);
};
#endif /* plat_is_windows */

/*--- Linux ---*/

#if plat_is_linux
#include "dh/sys/call/linux.h"
#include "dh/sys/posix.h"

#define proc__linux_default_path u8_l("/usr/local/bin:/bin:/usr/bin")

typedef struct proc__linux_StdIO {
    var_(child_fd, sys_call_linux_fd_t);
    var_(parent, O$fs_File);
    var_(needs_close_child, bool);
} proc__linux_StdIO;
T_use_E$(proc__linux_StdIO);

T_use_E$($set(proc_Spawn_E)(O$S$u8));
T_use_E$($set(proc_Spawn_E)(O$S_const$u8));

/*--- Declarations ---*/

$attr($must_check)
$static fn_((proc__linux_mapErr(sys_call_linux_word err))(EAny));
$attr($must_check)
$static fn_((proc__linux_mapExePathErr(sys_call_linux_word err))(EAny));
$attr($must_check)
$static fn_((proc__linux_mapCurrPathErr(sys_call_linux_word err))(EAny));
$attr($must_check)
$static fn_((proc__linux_mapSetPathErr(sys_call_linux_word err))(EAny));
$static fn_((proc__linux_pipeFile(sys_call_linux_fd_t fd))(fs_File));
$attr($must_check)
$static fn_((proc__linux_dupeZ(
    mem_Alctr gpa,
    S_const$u8 src
))(mem_E$S$u8));
$attr($must_check)
$static fn_((proc__linux_envByOwned(
    mem_Alctr gpa,
    proc_Env env,
    S_const$u8 name
))(proc_Spawn_E$O$S$u8));
$static fn_((proc__linux_closeIf(sys_call_linux_fd_t fd))(void));
$static fn_((proc__linux_discardStdIO(proc__linux_StdIO std_io))(void));
$attr($must_check)
$static fn_((proc__linux_readExecErr(
    sys_call_linux_fd_t fd,
    sys_call_linux_word* err
))(sys_call_linux_word));
$attr($must_check)
$static fn_((proc__linux_resolveStdIO(
    proc_cmd_StdIO spec,
    O$fs_File inherited,
    sys_call_linux_fd_t std_fd
))(E$proc__linux_StdIO));
$attr($must_check)
$static fn_((proc__linux_dupTo(
    sys_call_linux_fd_t src,
    sys_call_linux_fd_t dst
))(sys_call_linux_word));
$attr($must_check)
$static fn_((proc__linux_execPath(
    const char* path,
    char* const* argv,
    char* const* envp
))(sys_call_linux_word));
$attr($must_check)
$static fn_((proc__linux_exec(
    proc_cmd_ArgExpsn expand_arg0,
    const char* file,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf
))(sys_call_linux_word));
$attr($no_return)
$static fn_((proc__linux_childFail(
    sys_call_linux_fd_t err_fd,
    sys_call_linux_word err
))(void));
$attr($no_return)
$static fn_((proc__linux_childExec(
    proc_Spawn_Opts cmd,
    O$S_const$u8 cwd_path,
    const char* exe,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf,
    proc__linux_StdIO std_in,
    proc__linux_StdIO std_out,
    proc__linux_StdIO std_err,
    sys_call_linux_fd_t err_fd
))(void));
$attr($must_check)
$static fn_((proc__linux_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 exe_path
))(proc_Spawn_E$proc_Child));
$attr($must_check)
$static fn_((proc__linux_dirExePath(
    fs_Dir dir,
    S_const$u8 arg0,
    S$u8 out
))(E$S_const$u8));
$attr($must_check)
$static fn_((proc__linux_replaceImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env inherited_env,
    proc_Replace_Opts opts,
    O$S_const$u8 exe_path
))(proc_Replace_E$void));

/*--- Definitions ---*/

fn_((proc__linux_mapErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_EPERM: return E_cause$proc_PermissionDenied().any;
    case sys_call_linux_ENOTDIR: return E_cause$proc_NotDir().any;
    case sys_call_linux_EIO: $fallthrough;
    case sys_call_linux_ELOOP: return E_cause$proc_FileSys().any;
    case sys_call_linux_ENOEXEC: $fallthrough;
    case sys_call_linux_EINVAL: $fallthrough;
    case sys_call_linux_ELIBBAD: return E_cause$proc_InvalidExe().any;
    case sys_call_linux_EISDIR: return E_cause$proc_IsDir().any;
    case sys_call_linux_ETXTBSY: return E_cause$proc_FileBusy().any;
    case sys_call_linux_EMFILE: return E_cause$proc_FdQuotaExceeded().any;
    case sys_call_linux_ENFILE: return E_cause$proc_SysFDQuotaExceeded().any;
    case sys_call_linux_ENODEV: return E_cause$proc_NoDevice().any;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_NameTooLong().any;
    case sys_call_linux_E2BIG: $fallthrough;
    case sys_call_linux_ENOMEM: return E_cause$proc_SysResrcs().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__linux_mapExePathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: $fallthrough;
    case sys_call_linux_ENOTDIR: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_EIO: return E_cause$proc_FileSys().any;
    case sys_call_linux_ELOOP: return E_cause$proc_SymLinkLoop().any;
    case sys_call_linux_ERANGE: $fallthrough;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_ResrcLimitReached().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__linux_mapCurrPathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: $fallthrough;
    case sys_call_linux_ENOTDIR: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_ERANGE: $fallthrough;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_ResrcLimitReached().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__linux_mapSetPathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_ENOTDIR: return E_cause$proc_NotDir().any;
    case sys_call_linux_ELOOP: return E_cause$proc_SymLinkLoop().any;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_NameTooLong().any;
    case sys_call_linux_EIO: return E_cause$proc_FileSys().any;
    default_() return E_cause$proc_SysResrcs().any $end(default);
    }
};

fn_((proc__linux_pipeFile(sys_call_linux_fd_t fd))(fs_File)) {
    return fs_File_Handle_promote(as$(fs_File_Handle)(fd), fs_File_Flags_default);
};

fn_((proc__linux_dupeZ(mem_Alctr gpa, S_const$u8 src))(mem_E$S$u8) $scope) {
    let len = orelse_((usize_addChkd(src.len, 1))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes($trace gpa, len));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok(out);
} $unscoped(fn);

fn_((proc__linux_envByOwned(
    mem_Alctr gpa,
    proc_Env env,
    S_const$u8 name
))(proc_Spawn_E$O$S$u8) $guard) {
    var scratch = try_(mem_Alctr_allocBytes($trace gpa, usize_(256)));
    defer_(mem_Alctr_freeBytes($trace gpa, scratch));
    while (true) {
        let value = catch_((proc_Env_by(env, name, scratch))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResrcLimitReached().as_any)) {
                return_err(err);
            }
            try_(proc__growScratch(&scratch, gpa));
            continue;
        }));
        if_none(value) return_ok(none());
        return_ok(some(try_(mem_Alctr_dupeBytes($trace gpa, unwrap_(value)))));
    }
} $unguarded(fn);

fn_((proc__linux_closeIf(sys_call_linux_fd_t fd))(void)) {
    if (fd >= 0) {
        let_ignore = sys_call_linux_close(fd);
    }
};

fn_((proc__linux_discardStdIO(proc__linux_StdIO std_io))(void)) {
    if (std_io.needs_close_child) proc__linux_closeIf(std_io.child_fd);
    if_some((std_io.parent)(file)) fs_File_close(file);
};

fn_((proc__linux_readExecErr(
    sys_call_linux_fd_t fd,
    sys_call_linux_word* err
))(sys_call_linux_word)) {
    claim_assert_nonnull(err);
    var_(read, sys_call_linux_word) = 0;
    do {
        read = sys_call_linux_read(fd, err, sizeOf$(sys_call_linux_word));
    } while (
        sys_call_linux_syscall_isErr(read)
        && sys_call_linux_syscall_err(read) == sys_call_linux_EINTR);
    return read;
};

fn_((proc__linux_resolveStdIO(proc_cmd_StdIO spec, O$fs_File inherited, sys_call_linux_fd_t std_fd))(E$proc__linux_StdIO) $scope) {
    let for_read = std_fd == 0;
    if (matches(spec, proc_cmd_StdIO_inherit)) {
        if_some((inherited)(file)) return_ok({
            .child_fd = as$(sys_call_linux_fd_t)(fs_File_handle(file)),
            .parent = none(),
            .needs_close_child = false,
        });
        let flags = for_read ? sys_call_linux_O_RDONLY : sys_call_linux_O_WRONLY;
        let fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/null", flags, 0);
        if (sys_call_linux_syscall_isErr(fd)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(fd)));
        return_ok({ .child_fd = fd, .parent = none(), .needs_close_child = true });
    }
    if (matches(spec, proc_cmd_StdIO_file)) {
        return_ok({
            .child_fd = as$(sys_call_linux_fd_t)(fs_File_handle(union_to((spec)(proc_cmd_StdIO_file)))),
            .parent = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_cmd_StdIO_close)) {
        return_ok({
            .child_fd = -1,
            .parent = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_cmd_StdIO_ignore)) {
        let flags = for_read ? sys_call_linux_O_RDONLY : sys_call_linux_O_WRONLY;
        let fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/null", flags, 0);
        if (sys_call_linux_syscall_isErr(fd)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(fd)));
        return_ok({
            .child_fd = fd,
            .parent = none(),
            .needs_close_child = true,
        });
    }
    if (!matches(spec, proc_cmd_StdIO_pipe)) return_err(E_cause$proc_OperationUnsupported());

    int fds[2] = { -1, -1 };
    let rc = sys_call_linux_pipe2(fds, sys_call_linux_O_CLOEXEC);
    if (sys_call_linux_syscall_isErr(rc)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
    if (for_read) {
        return_ok({
            .child_fd = fds[0],
            .parent = some(proc__linux_pipeFile(fds[1])),
            .needs_close_child = true,
        });
    }
    return_ok({
        .child_fd = fds[1],
        .parent = some(proc__linux_pipeFile(fds[0])),
        .needs_close_child = true,
    });
} $unscoped(fn);

fn_((proc__linux_dupTo(
    sys_call_linux_fd_t src,
    sys_call_linux_fd_t dst
))(sys_call_linux_word)) {
    if (src < 0) {
        proc__linux_closeIf(dst);
        return 0;
    }
    if (src == dst) return 0;
    let rc = sys_call_linux_dup3(src, dst, 0);
    if (sys_call_linux_syscall_isErr(rc)) {
        return sys_call_linux_syscall_err(rc);
    }
    proc__linux_closeIf(src);
    return 0;
};

fn_((proc__linux_execPath(
    const char* path,
    char* const* argv,
    char* const* envp
))(sys_call_linux_word)) {
    let rc = sys_call_linux_execve(path, argv, envp);
    claim_assert(sys_call_linux_syscall_isErr(rc));
    return sys_call_linux_syscall_err(rc);
};

fn_((proc__linux_exec(
    proc_cmd_ArgExpsn expand_arg0,
    const char* file,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf
))(sys_call_linux_word)) {
    let file_s = mem_spanZ0Bytes(as$(P_const$u8)(file));
    if (mem_containsUnitBytes(file_s, u8_c('/'))) {
        return proc__linux_execPath(file, argv, envp);
    }

    var paths = mem_tokzUnitBytes(path_env, u8_c(':'));
    var_(saw_access_denied, bool) = false;
    while_some((mem_TokzIter_nextBytes(&paths)), path) {
        let required = path.len + 1 + file_s.len + 1;
        if (required > path_buf.len) return sys_call_linux_ENAMETOOLONG;
        mem_copyBytes(S_prefix((path_buf)(path.len)), path);
        *S_at((path_buf)[path.len]) = u8_c('/');
        mem_copyBytes(
            S_slice((path_buf)$r(path.len + 1, required - 1)),
            file_s
        );
        *S_at((path_buf)[required - 1]) = 0;

        let previous_arg0 = argv[0];
        if (expand_arg0 == proc_cmd_ArgExpsn_expand) {
            argv[0] = as$(char*)(path_buf.ptr);
        }
        let err = proc__linux_execPath(as$(const char*)(path_buf.ptr), argv, envp);
        argv[0] = previous_arg0;

        if (err == sys_call_linux_EACCES) {
            saw_access_denied = true;
            continue;
        }
        if (err == sys_call_linux_ENOENT || err == sys_call_linux_ENOTDIR) {
            continue;
        }
        return err;
    }
    return saw_access_denied ? sys_call_linux_EACCES : sys_call_linux_ENOENT;
};

fn_((proc__linux_childFail(
    sys_call_linux_fd_t err_fd,
    sys_call_linux_word err
))(void)) {
    var_(wrote, sys_call_linux_word) = 0;
    do {
        wrote = sys_call_linux_write(err_fd, &err, sizeOf$(sys_call_linux_word));
    } while (
        sys_call_linux_syscall_isErr(wrote)
        && sys_call_linux_syscall_err(wrote) == sys_call_linux_EINTR);
    sys_call_linux_exit(127);
};

fn_((proc__linux_childExec(
    proc_Spawn_Opts cmd,
    O$S_const$u8 cwd_path,
    const char* exe,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf,
    proc__linux_StdIO std_in,
    proc__linux_StdIO std_out,
    proc__linux_StdIO std_err,
    sys_call_linux_fd_t err_fd
))(void)) {
    if (matches(cmd.cwd, proc_cmd_CWD_dir)) {
        let rc = sys_call_linux_fchdir(
            as$(sys_call_linux_fd_t)(fs_Dir_handle(union_to((cmd.cwd)(proc_cmd_CWD_dir)))));
        if (sys_call_linux_syscall_isErr(rc)) {
            proc__linux_childFail(err_fd, sys_call_linux_syscall_err(rc));
        }
    } else if (matches(cmd.cwd, proc_cmd_CWD_path)) {
        let path = unwrap_(cwd_path);
        let rc = sys_call_linux_chdir(as$(const char*)(path.ptr));
        if (sys_call_linux_syscall_isErr(rc)) {
            proc__linux_childFail(err_fd, sys_call_linux_syscall_err(rc));
        }
    }
    let err_in = proc__linux_dupTo(std_in.child_fd, 0);
    if (err_in != 0) proc__linux_childFail(err_fd, err_in);
    let err_out = proc__linux_dupTo(std_out.child_fd, 1);
    if (err_out != 0) proc__linux_childFail(err_fd, err_out);
    let err_err = proc__linux_dupTo(std_err.child_fd, 2);
    if (err_err != 0) proc__linux_childFail(err_fd, err_err);

    proc__linux_childFail(
        err_fd,
        proc__linux_exec(
            cmd.expand_arg0,
            exe,
            argv,
            envp,
            path_env,
            path_buf
        )
    );
};

fn_((proc__linux_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 exe_path
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    if (cmd.start_suspended) return_err(E_cause$proc_OperationUnsupported());
    let_ignore = cmd.create_no_window;

    var argv = try_(proc_NativeStrList_from(gpa, cmd.argv));
    defer_(proc_NativeStrList_fini(&argv, gpa));
    var child_env = try_(proc_NativeStrList_fromEnv(gpa, env, cmd.env));
    defer_(proc_NativeStrList_fini(&child_env, gpa));

    var path_env_mem = try_(proc__linux_envByOwned(
        gpa, env, u8_l("PATH")
    ));
    defer_(if_some((path_env_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    let path_env = isSome(path_env_mem)
                     ? unwrap_(path_env_mem).as_const
                     : proc__linux_default_path;

    let arg0 = proc_NativeStrList_at(argv, usize_(0));
    let path_buf_len = orelse_((usize_addChkd(path_env.len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let path_buf = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(path_buf_len, 2))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, path_buf));

    var exe_mem = none$((O$S$u8));
    defer_(if_some((exe_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    if_some((exe_path)(path)) {
        asg_l((&exe_mem)(some(try_(proc__linux_dupeZ(gpa, path)))));
    }
    let exe = isSome(exe_mem)
                ? as$(const char*)(unwrap_(exe_mem).ptr)
                : as$(const char*)(arg0.ptr);

    var cwd_mem = none$((O$S$u8));
    defer_(if_some((cwd_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    var_(cwd_path, O$S_const$u8) = none();
    if (matches(cmd.cwd, proc_cmd_CWD_path)) {
        asg_l((&cwd_mem)(some(try_(proc__linux_dupeZ(
            gpa, union_to((cmd.cwd)(proc_cmd_CWD_path))
        )))));
        asg_l((&cwd_path)(some(unwrap_(cwd_mem).as_const)));
    }

    let_(std_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDIN_FILENO,
        fs_File_Flags_default
    ));
    let_(std_out_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDOUT_FILENO,
        fs_File_Flags_default
    ));
    let_(std_err_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDERR_FILENO,
        fs_File_Flags_default
    ));
    var std_in = try_(proc__linux_resolveStdIO(
        cmd.std_in, std_inherited, as$(sys_call_linux_fd_t)(sys_posix_STDIN_FILENO)
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_in));
    var std_out = try_(proc__linux_resolveStdIO(
        cmd.std_out, std_out_inherited, as$(sys_call_linux_fd_t)(sys_posix_STDOUT_FILENO)
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_out));
    var std_err = try_(proc__linux_resolveStdIO(
        cmd.std_err, std_err_inherited, as$(sys_call_linux_fd_t)(sys_posix_STDERR_FILENO)
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_err));

    int err_fds[2] = { -1, -1 };
    let err_pipe_rc = sys_call_linux_pipe2(err_fds, sys_call_linux_O_CLOEXEC);
    if (sys_call_linux_syscall_isErr(err_pipe_rc)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(err_pipe_rc)));
    }
    defer_(proc__linux_closeIf(err_fds[0]));
    defer_(proc__linux_closeIf(err_fds[1]));

    let pid = sys_call_linux_fork();
    if (sys_call_linux_syscall_isErr(pid)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(pid)));
    }
    if (pid == 0) {
        proc__linux_closeIf(err_fds[0]);
        proc__linux_childExec(
            cmd,
            cwd_path,
            exe,
            proc_NativeStrList_raw(argv),
            proc_NativeStrList_raw(child_env),
            path_env,
            path_buf,
            std_in,
            std_out,
            std_err,
            err_fds[1]
        );
    }
    errdefer_($ignore, {
        let_ignore = sys_call_linux_kill(as$(sys_call_linux_pid_t)(pid), 9);
        var_(status, i32) = 0;
        let_ignore = sys_call_linux_wait4(
            as$(sys_call_linux_pid_t)(pid), &status, 0, null);
    });

    proc__linux_closeIf(err_fds[1]);
    err_fds[1] = -1;
    if (std_in.needs_close_child) {
        proc__linux_closeIf(std_in.child_fd);
        std_in.needs_close_child = false;
    }
    if (std_out.needs_close_child) {
        proc__linux_closeIf(std_out.child_fd);
        std_out.needs_close_child = false;
    }
    if (std_err.needs_close_child) {
        proc__linux_closeIf(std_err.child_fd);
        std_err.needs_close_child = false;
    }

    var_(exec_err, sys_call_linux_word) = 0;
    let exec_err_read = proc__linux_readExecErr(err_fds[0], &exec_err);
    proc__linux_closeIf(err_fds[0]);
    err_fds[0] = -1;
    if (sys_call_linux_syscall_isErr(exec_err_read)) {
        return_err(E_cause$proc_SysResrcs());
    }
    if (exec_err_read != 0) {
        claim_assert(as$(usize)(exec_err_read) == sizeOf$(sys_call_linux_word));
        return_err(proc__linux_mapErr(exec_err));
    }

    return_ok({
        .handle = some(as$(proc_Child_Handle)(pid)),
        .id = as$(proc_Child_Id)(pid),
        .io = {
            .in = std_in.parent,
            .out = std_out.parent,
            .err = std_err.parent,
        },
    });
} $unguarded(fn);

fn_((proc__linux_openExe(
    P$raw ctx,
    fs_File_OpenFlags flags
))(E$fs_File)) {
    let_ignore = ensureNonnull(ctx);
    return fs_File_open(u8_l("/proc/self/exe"), flags);
};
fn_((proc__linux_exePath(
    P$raw ctx,
    S$u8 out_buf
))(proc_ExePath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let read = sys_call_linux_readlinkat(sys_call_linux_AT_FDCWD, "/proc/self/exe", as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(read)) {
        return_err(proc__linux_mapExePathErr(sys_call_linux_syscall_err(read)));
    }
    if (as$(usize)(read) >= out_buf.len) return_err(E_cause$proc_ResrcLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(read))));
} $unscoped(fn);

fn_((proc__linux_currPath(
    P$raw ctx,
    S$u8 out_buf
))(proc_CurrPath_E$S$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let rc = sys_call_linux_getcwd(as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapCurrPathErr(sys_call_linux_syscall_err(rc)));
    }
    let len = mem_lenZ0Bytes(out_buf.ptr);
    return_ok(S_prefix((out_buf)(len)));
} $unscoped(fn);

fn_((proc__linux_setCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $guard) {
    claim_assert_nonnull(ctx);
    let path_z = try_(proc__linux_dupeZ(gpa, path));
    defer_(mem_Alctr_freeBytes($trace gpa, path_z));
    let rc = sys_call_linux_chdir(as$(const char*)(path_z.ptr));
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapSetPathErr(sys_call_linux_syscall_err(rc)));
    }
    return_ok({});
} $unguarded(fn);

fn_((proc__linux_setCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let rc = sys_call_linux_fchdir(as$(sys_call_linux_fd_t)(fs_Dir_handle(dir)));
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
    }
    return_ok({});
} $unscoped(fn);

fn_((proc__linux_dirExePath(
    fs_Dir dir,
    S_const$u8 arg0,
    S$u8 out
))(E$S_const$u8) $scope) {
    var fixed = io_Fixed_Writer_from(io_Fixed_writing(out));
    catch_((fmt_format(
        io_Fixed_writer(&fixed),
        u8_l("/proc/self/fd/{:ul}/{:s}"),
        as$(u64)(fs_Dir_handle(dir)),
        arg0
    ))($ignore, return_err(E_cause$proc_ResrcLimitReached())));
    catch_((io_Writer_writeByte(io_Fixed_writer(&fixed), u8_c('\0')))(
        $ignore, return_err(E_cause$proc_ResrcLimitReached())
    ));
    let written = io_Fixed_written(fixed.stream).as_const;
    return_ok(S_prefix((written)(written.len - usize_(1))));
} $unscoped(fn);

fn_((proc__linux_replaceImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env inherited_env,
    proc_Replace_Opts opts,
    O$S_const$u8 exe_path
))(proc_Replace_E$void) $guard) {
    claim_assert_nonnull(ctx);
    if (opts.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var argv = try_(proc_NativeStrList_from(gpa, opts.argv));
    defer_(proc_NativeStrList_fini(&argv, gpa));
    var env = try_(proc_NativeStrList_fromEnv(gpa, inherited_env, opts.env));
    defer_(proc_NativeStrList_fini(&env, gpa));

    var path_env_mem = try_(proc__linux_envByOwned(
        gpa, inherited_env, u8_l("PATH")
    ));
    defer_(if_some((path_env_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    let path_env = isSome(path_env_mem)
                     ? unwrap_(path_env_mem).as_const
                     : proc__linux_default_path;

    let arg0 = proc_NativeStrList_at(argv, usize_(0));
    let path_buf_len = orelse_((usize_addChkd(path_env.len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let path_buf = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(path_buf_len, 2))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, path_buf));

    var exe_mem = none$((O$S$u8));
    defer_(if_some((exe_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    if_some((exe_path)(path)) {
        asg_l((&exe_mem)(some(try_(proc__linux_dupeZ(gpa, path)))));
    }
    let exe = isSome(exe_mem)
                ? as$(const char*)(unwrap_(exe_mem).ptr)
                : as$(const char*)(arg0.ptr);

    return_err(proc__linux_mapErr(proc__linux_exec(
        opts.expand_arg0,
        exe,
        proc_NativeStrList_raw(argv),
        proc_NativeStrList_raw(env),
        path_env,
        path_buf
    )));
} $unguarded(fn);

fn_((proc__linux_replace(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Replace_Opts opts
))(proc_Replace_E$void)) {
    return proc__linux_replaceImpl(ctx, gpa, env, opts, none$((O$S_const$u8)));
};

fn_((proc__linux_replacePath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void) $guard) {
    claim_assert_nonnull(ctx);
    if (opts.argv.len == 0) return_err(E_cause$proc_InvalidName());
    let arg0 = *S_at((opts.argv)[0]);
    let base_len = orelse_((usize_addChkd(u8_l("/proc/self/fd/").len, 21))(return_err(E_cause$OutOfMemory())));
    let out_len = orelse_((usize_addChkd(base_len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(out_len, 1))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, out));
    let exe_path = try_(proc__linux_dirExePath(dir, arg0, out));
    return proc__linux_replaceImpl(
        ctx,
        gpa,
        env,
        opts,
        some$((O$S_const$u8)(exe_path))
    );
} $unguarded(fn);

fn_((proc__linux_spawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child)) {
    return proc__linux_spawnImpl(ctx, gpa, env, cmd, none$((O$S_const$u8)));
};

fn_((proc__linux_spawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    let arg0 = *S_at((cmd.argv)[0]);
    let base_len = orelse_((usize_addChkd(u8_l("/proc/self/fd/").len, 21))(return_err(E_cause$OutOfMemory())));
    let out_len = orelse_((usize_addChkd(base_len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(out_len, 1))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, out));
    let exe_path = try_(proc__linux_dirExePath(dir, arg0, out));
    return proc__linux_spawnImpl(ctx, gpa, env, cmd, some$((O$S_const$u8)(exe_path)));
} $unguarded(fn);

fn_((proc__linux_child_wait(
    P$raw ctx,
    P$$(proc_Child) child
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    let_ignore = ensureNonnull(ctx);
    child = ensureNonnull(child);
    claim_assert(child->id != 0);
    var_(status, int) = 0;
    var_(waited, sys_call_linux_word) = 0;
    do {
        waited = sys_call_linux_wait4(
            as$(sys_call_linux_pid_t)(child->id), &status, 0, null);
    } while (
        sys_call_linux_syscall_isErr(waited)
        && sys_call_linux_syscall_err(waited) == sys_call_linux_EINTR);
    if (sys_call_linux_syscall_isErr(waited)) {
        if (sys_call_linux_syscall_err(waited) == sys_call_linux_ECHILD) {
            proc__childClear(child);
            return_err(E_cause$proc_SysResrcs());
        }
        return_err(E_cause$proc_SysResrcs());
    }
    proc__childClear(child);
    if ((status & 0x7f) == 0) {
        return_ok(union_of$(
            (proc_Child_Trm)(proc_Child_Trm_exited)(as$(u8)((status >> 8) & 0xff))
        ));
    }
    if ((status & 0x7f) != 0x7f) {
        return_ok(union_of$(
            (proc_Child_Trm)(proc_Child_Trm_signal)(as$(proc_Child_Sig)(status & 0x7f))
        ));
    }
    return_ok(union_of$(
        (proc_Child_Trm)(proc_Child_Trm_stopped)(as$(proc_Child_Sig)((status >> 8) & 0xff))
    ));
} $unscoped(fn);

fn_((proc__linux_child_kill(
    P$raw ctx,
    P$$(proc_Child) child
))(void)) {
    let_ignore = ensureNonnull(ctx);
    child = ensureNonnull(child);
    if (child->id != 0) {
        let_ignore = sys_call_linux_kill(
            as$(sys_call_linux_pid_t)(child->id), 9);
        let_ignore = catch_((proc__linux_child_wait(ctx, child))(
            $ignore,
            $do_nothing
        ));
    }
};

fn_((proc__linux_userInfo(P$raw ctx, S_const$u8 name))(proc_UserInfo_E$proc_UserInfo) $guard) {
    claim_assert_nonnull(ctx);
    let file = catch_((fs_File_open(u8_l("/etc/passwd"), fs_File_OpenFlags_default))(
        $ignore, return_err(E_cause$proc_UserInfo_SysResrcs())
    ));
    defer_(fs_File_close(file));

    typedef enum proc__PasswdState {
        proc__PasswdState_name,
        proc__PasswdState_wait_line,
        proc__PasswdState_password,
        proc__PasswdState_uid,
        proc__PasswdState_gid,
    } proc__PasswdState;
    var_(state, proc__PasswdState) = proc__PasswdState_name;
    var_(name_idx, usize) = 0;
    var_(uid_buf, A$$(10, u8)) $undefined;
    var_(uid_len, usize) = 0;
    var_(gid_buf, A$$(10, u8)) $undefined;
    var_(gid_len, usize) = 0;
    var_(uid, u32) = 0;
    var_(offset, u64) = 0;
    var_(buf, A$$(2048, u8)) $undefined;
    while (true) {
        let n = catch_((fs_File_readPos(file, A_ref$((S$u8)(buf)), offset))(
            $ignore, return_err(E_cause$proc_UserInfo_SysResrcs())
        ));
        if (n == 0) break;
        offset += n;
        for_(($s(A_prefix$((S_const$u8)(buf)(n))))(byte_ptr)) {
            let byte = *byte_ptr;
            switch (state) {
            case_((proc__PasswdState_name)) {
                if (byte == ':') {
                    state = name_idx == name.len
                              ? proc__PasswdState_password
                              : proc__PasswdState_wait_line;
                } else if (byte == '\n') {
                    name_idx = 0;
                } else if (name_idx >= name.len || *S_at((name)[name_idx]) != byte) {
                    state = proc__PasswdState_wait_line;
                } else {
                    ++name_idx;
                }
            } $end(case);
            case_((proc__PasswdState_wait_line)) {
                if (byte == '\n') {
                    state = proc__PasswdState_name;
                    name_idx = 0;
                }
            } $end(case);
            case_((proc__PasswdState_password)) {
                if (byte == ':') {
                    state = proc__PasswdState_uid;
                } else if (byte == '\n') {
                    return_err(E_cause$proc_UserInfo_CorruptDatabase());
                }
            } $end(case);
            case_((proc__PasswdState_uid)) {
                if (byte == ':') {
                    uid = catch_((fmt_parse$u32(
                        A_prefix$((S_const$u8)(uid_buf)(uid_len)),
                        u8_(10)
                    ))($ignore, return_err(E_cause$proc_UserInfo_CorruptDatabase())));
                    state = proc__PasswdState_gid;
                } else if (ascii_isDigit(byte)) {
                    if (uid_len == A_len(uid_buf)) {
                        return_err(E_cause$proc_UserInfo_CorruptDatabase());
                    }
                    *A_at((uid_buf)[uid_len++]) = byte;
                } else {
                    return_err(E_cause$proc_UserInfo_CorruptDatabase());
                }
            } $end(case);
            case_((proc__PasswdState_gid)) {
                if (byte == ':') {
                    let gid = catch_((fmt_parse$u32(
                        A_prefix$((S_const$u8)(gid_buf)(gid_len)),
                        u8_(10)
                    ))($ignore, return_err(E_cause$proc_UserInfo_CorruptDatabase())));
                    return_ok((proc_UserInfo){ .uid = uid, .gid = gid });
                } else if (ascii_isDigit(byte)) {
                    if (gid_len == A_len(gid_buf)) {
                        return_err(E_cause$proc_UserInfo_CorruptDatabase());
                    }
                    *A_at((gid_buf)[gid_len++]) = byte;
                } else {
                    return_err(E_cause$proc_UserInfo_CorruptDatabase());
                }
            } $end(case);
            default_() claim_unreachable $end(default);
            }
        } $end(for);
    }
    return_err(E_cause$proc_UserInfo_UserNotFound());
} $unguarded(fn);
fn_((proc__linux_baseAddr(P$raw ctx))(proc_BaseAddr_E$usize) $guard) {
    claim_assert_nonnull(ctx);
    let file = catch_((fs_File_open(u8_l("/proc/self/maps"), fs_File_OpenFlags_default))(
        $ignore, return_err(E_cause$proc_BaseAddr_SysResrcs())
    ));
    defer_(fs_File_close(file));

    var_(buf, A$$(256, u8)) $undefined;
    let n = catch_((fs_File_readPos(file, A_ref$((S$u8)(buf)), 0))(
        $ignore, return_err(E_cause$proc_BaseAddr_SysResrcs())
    ));
    if (n == 0) return_err(E_cause$proc_BaseAddr_CorruptImage());

    let bytes = A_prefix$((S_const$u8)(buf)(n));
    let separator = orelse_((mem_findFirstUnitBytes(bytes, u8_c('-')))(
        return_err(E_cause$proc_BaseAddr_CorruptImage())
    ));
    return_ok(catch_((fmt_parse$usize(S_prefix((bytes)(separator)), u8_(16)))(
        $ignore, return_err(E_cause$proc_BaseAddr_CorruptImage())
    )));
} $unguarded(fn);

fn_((proc__linux_abort(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = sys_posix_raise(sys_posix_SIGABRT);
    start_exit(134);
};
#endif /* plat_is_linux */
