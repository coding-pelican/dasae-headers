#include "dh/proc/Self.h"
#include "dh/fs/path.h"

#define proc__path_max usize_(32768)

fn_((proc_exePath(proc_Self self, S$u8 out_buf))(proc_ExecutablePath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->exePathFn(self.ctx, out_buf);
};
fn_((proc_exePathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_ExecutablePathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_max));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    let path = try_(proc_exePath(self, path_buf));
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
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
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_max));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    let path = try_(proc_exeDirPath(self, path_buf));
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
} $unguarded(fn);
fn_((proc_currPath(proc_Self self, S$u8 out_buf))(proc_CurrentPath_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->currPathFn(self.ctx, out_buf);
};
fn_((proc_currPathAlloc(
    proc_Self self,
    mem_Alctr gpa
))(proc_CurrentPathAlloc_E$S$u8) $guard) {
    var path_buf = try_(mem_Alctr_allocBytes($trace gpa, proc__path_max));
    errdefer_($ignore, mem_Alctr_freeBytes($trace gpa, path_buf));
    let path = try_(proc_currPath(self, path_buf));
    return_ok(try_(mem_Alctr_reallocBytes($trace gpa, path_buf, path.len)));
} $unguarded(fn);
fn_((proc_setCurrDir(proc_Self self, fs_Dir dir))(proc_SetCurrentDir_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrDirFn(self.ctx, dir);
};
fn_((proc_setCurrPath(proc_Self self, S_const$u8 path))(proc_SetCurrentPath_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrPathFn(self.ctx, path);
};
fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnFn(self.ctx, cmd);
};
fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnPathFn(self.ctx, dir, cmd);
};
fn_((proc_replace(proc_Self self, proc_Replace_Opts opts))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replaceFn(self.ctx, opts);
};
fn_((proc_replacePath(
    proc_Self self,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replacePathFn(self.ctx, dir, opts);
};
