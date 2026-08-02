#include "dh/proc/Self.h"
#include "dh/fs/path.h"
#include "dh/ArrList.h"
#include "dh/clsr.h"
#include "dh/thrd.h"
#include "dh/io/Reader.h"

/*========== External Definitions ===========================================*/

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

$static fn_((proc__runRead(proc__RunReadCtx* ctx))(i32) $guard) {
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
fn_use_Clsr_((proc__runRead)(proc__RunReadCtx*)(i32));

$static fn_((proc__runTakeBytes(proc__RunReadCtx* ctx))(S$u8)) {
    claim_assert_nonnull(ctx);
    let len = ArrList_len(ctx->bytes);
    ArrList_shrinkAndFree(&ctx->bytes, typeInfo$(u8), ctx->gpa, len);
    let bytes = u_castS$((S$u8)(ArrList_itemsMut(ctx->bytes, typeInfo$(u8))));
    ctx->bytes = ArrList_empty(typeInfo$(u8));
    return bytes;
};

$static fn_((proc__runReadStatusErr(proc__RunReadStatus status))(proc_Run_E$void) $scope) {
    switch (status) {
    case_((proc__RunReadStatus_ok))
        return_ok({}); $end(case)
        ;
    case_((proc__RunReadStatus_too_long))
        return_err(E_cause$proc_Run_StreamTooLong()); $end(case)
        ;
    case_((proc__RunReadStatus_read_failed))
        return_err(E_cause$proc_Run_ReadFailed()); $end(case)
        ;
    case_((proc__RunReadStatus_out_of_memory))
        return_err(E_cause$OutOfMemory()); $end(case)
        ;
    default_()
        claim_unreachable; $end(default)
        ;
    }
} $unscoped(fn);

fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExePath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->exePathFn(self.ctx, out_buf);
};

fn_((proc_exePathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_ExePathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_alloc_initial));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    while (true) {
        let path = catch_((proc_exePath(self, path_buf))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResourceLimitReached().as_any)) {
                return_err(err);
            }
            let next_len = orelse_((usize_mulChkd(path_buf.len, 2))(
                return_err(E_cause$OutOfMemory())
            ));
            path_buf = try_(mem_Alctr_reallocBytes($trace gpa, path_buf, next_len));
            continue;
        }));
        return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
    }
} $unguarded(fn);

fn_((proc_exeDirPath(
    proc_Self self,
    S$u8 out_buf
))(proc_ExePath_E$S$u8) $scope) {
    let path = try_(proc_exePath(self, out_buf));
    let dir = fs_path_dirname(path.as_const);
    return_ok(S_prefix((out_buf)(dir.len)));
} $unscoped(fn);

fn_((proc_exeDirPathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_ExePathAlloc_E$S$u8) $guard) {
    var path = try_(proc_exePathAlloc(self, gpa));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path));
    let dir = fs_path_dirname(path.as_const);
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path, dir.len)));
} $unguarded(fn);

fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrPath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->currPathFn(self.ctx, out_buf);
};

fn_((proc_currPathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_CurrPathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_alloc_initial));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    while (true) {
        let path = catch_((proc_currPath(self, path_buf))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResourceLimitReached().as_any)) {
                return_err(err);
            }
            let next_len = orelse_((usize_mulChkd(path_buf.len, 2))(
                return_err(E_cause$OutOfMemory())
            ));
            path_buf = try_(mem_Alctr_reallocBytes($trace gpa, path_buf, next_len));
            continue;
        }));
        return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
    }
} $unguarded(fn);

fn_((proc_setCurrDir(
    proc_Self self,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrDirFn(self.ctx, gpa, dir);
};

fn_((proc_setCurrPath(
    proc_Self self,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrPathFn(self.ctx, gpa, path);
};

fn_((proc_spawn(
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
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
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
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

fn_((proc_replace(
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    proc_Replace_Opts opts
))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replaceFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        opts
    );
};

fn_((proc_replacePath(
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replacePathFn(
        self.ctx,
        mem_Alctr_ensureValid(gpa),
        proc_Env_ensureValid(env),
        dir,
        opts
    );
};

fn_((proc_getUserInfo(
    proc_Self self,
    S_const$u8 name
))(proc_GetUserInfo_E$proc_UserInfo)) {
    self = proc_ensureValid(self);
    return self.vtbl->getUserInfoFn(self.ctx, name);
};

fn_((proc_getBaseAddress(
    proc_Self self
))(proc_GetBaseAddress_E$usize)) {
    self = proc_ensureValid(self);
    return self.vtbl->getBaseAddressFn(self.ctx);
};

fn_((proc_run(
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    proc_Run_Opts opts
))(proc_Run_E$proc_Run_Res) $guard) {
    self = proc_ensureValid(self);
    gpa = mem_Alctr_ensureValid(gpa);
    env = proc_Env_ensureValid(env);

    let_(stdio_ignore, proc_Spawn_StdIO) = union_of((proc_Spawn_StdIO_ignore){});
    let_(stdio_pipe, proc_Spawn_StdIO) = union_of((proc_Spawn_StdIO_pipe){});
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

fn_((proc_Run_Res_fini(
    proc_Run_Res* self,
    mem_Alctr gpa
))(void)) {
    claim_assert_nonnull(self);
    gpa = mem_Alctr_ensureValid(gpa);
    if (self->out.len != 0) mem_Alctr_freeBytes($trace gpa, self->out);
    if (self->err.len != 0) mem_Alctr_freeBytes($trace gpa, self->err);
    self->out = (S$u8){ 0 };
    self->err = (S$u8){ 0 };
};

$attr($no_return)
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

$attr($no_return $branch_cold)
fn_((proc_abort(proc_Self self))(void)) {
    self = proc_ensureValid(self);
    self.vtbl->abortFn(self.ctx);
    claim_unreachable;
};
