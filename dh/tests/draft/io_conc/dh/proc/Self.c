#include "Self.h"

/*========== Internal Definitions ===========================================*/

$static fn_((proc_VTbl_failingExecutablePath(P$raw ctx, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = out_buf;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingCurrentPath(P$raw ctx, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = out_buf;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingSetCurrentPath(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingSpawn(P$raw ctx, proc_Cmd cmd))(E$proc_Child) $scope) {
    let_ignore = ctx;
    let_ignore = cmd;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingSpawnPath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = cmd;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingReplace(P$raw ctx, proc_Cmd cmd))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = cmd;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingReplacePath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = cmd;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_failingWait(P$raw ctx, proc_Child* child))(E$proc_Term) $scope) {
    let_ignore = ctx;
    let_ignore = child;
    return_err(proc_E_Unsupported());
} $unscoped(fn);

$static fn_((proc_VTbl_noKill(P$raw ctx, proc_Child* child))(void)) {
    let_ignore = ctx;
    let_ignore = child;
}

/*========== External Definitions ===========================================*/

let_(proc_VTbl_failing, proc_Self_VTbl) = {
    .executablePathFn = proc_VTbl_failingExecutablePath,
    .currentPathFn = proc_VTbl_failingCurrentPath,
    .setCurrentPathFn = proc_VTbl_failingSetCurrentPath,
    .spawnFn = proc_VTbl_failingSpawn,
    .spawnPathFn = proc_VTbl_failingSpawnPath,
    .replaceFn = proc_VTbl_failingReplace,
    .replacePathFn = proc_VTbl_failingReplacePath,
    .waitFn = proc_VTbl_failingWait,
    .killFn = proc_VTbl_noKill,
};

$static var_(proc_failing__ctx, Void) = cleared();
let_(proc_failing, proc_Self) = {
    .ctx = &proc_failing__ctx,
    .vtbl = &proc_VTbl_failing,
};

fn_((proc_executablePath(proc_Self self, S$u8 out_buf))(E$S$u8)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->executablePathFn(self.ctx, out_buf);
}

fn_((proc_currentPath(proc_Self self, S$u8 out_buf))(E$S$u8)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->currentPathFn(self.ctx, out_buf);
}

fn_((proc_setCurrentPath(proc_Self self, S_const$u8 path))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->setCurrentPathFn(self.ctx, path);
}

fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(E$proc_Child)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->spawnFn(self.ctx, cmd);
}

fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(E$proc_Child)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->spawnPathFn(self.ctx, dir, cmd);
}

fn_((proc_replace(proc_Self self, proc_Cmd cmd))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->replaceFn(self.ctx, cmd);
}

fn_((proc_replacePath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->replacePathFn(self.ctx, dir, cmd);
}

fn_((proc_direct(void))(proc_direct_E$proc_Self) $scope) {
    return_err(proc_direct_E_Unsupported());
} $unscoped(fn);
