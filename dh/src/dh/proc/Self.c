#include "dh/proc/Self.h"
#include "dh/fs/path.h"

/*========== External Definitions ===========================================*/

#define proc__path_alloc_initial usize_(256)

fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExecutablePath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->exePathFn(self.ctx, out_buf);
};

fn_((proc_exePathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_ExecutablePathAlloc_E$S$u8) $guard) {
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
))(proc_ExecutablePath_E$S$u8) $scope) {
    let path = try_(proc_exePath(self, out_buf));
    let dir = fs_path_dirname(path.as_const);
    return_ok(S_prefix((out_buf)(dir.len)));
} $unscoped(fn);

fn_((proc_exeDirPathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_ExecutablePathAlloc_E$S$u8) $guard) {
    var path = try_(proc_exePathAlloc(self, gpa));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path));
    let dir = fs_path_dirname(path.as_const);
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path, dir.len)));
} $unguarded(fn);

fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrentPath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->currPathFn(self.ctx, out_buf);
};

fn_((proc_currPathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_CurrentPathAlloc_E$S$u8) $guard) {
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
))(proc_SetCurrentDir_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrDirFn(self.ctx, gpa, dir);
};

fn_((proc_setCurrPath(
    proc_Self self,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrentPath_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrPathFn(self.ctx, gpa, path);
};

fn_((proc_spawn(
    proc_Self self,
    mem_Alctr gpa,
    proc_Env env,
    proc_Cmd cmd
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
    proc_Cmd cmd
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
    proc_Cmd_Replace_Opts opts
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
    proc_Cmd_Replace_Opts opts
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

fn_((proc_exit(proc_Self self, u8 status))(void)) {
    self = proc_ensureValid(self);
    self.vtbl->exitFn(self.ctx, status);
    claim_unreachable;
};

fn_((proc_abort(proc_Self self))(void)) {
    self = proc_ensureValid(self);
    self.vtbl->abortFn(self.ctx);
    claim_unreachable;
};
