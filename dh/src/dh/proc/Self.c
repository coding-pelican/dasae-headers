#include "dh/proc/Self.h"
#include "dh/proc/Cmd.h"
#include "dh/proc/Child.h"

fn_((proc_executablePath(proc_Self self, S$u8 out_buf))(proc_Path_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->executablePathFn(self.ctx, out_buf);
};
fn_((proc_currentPath(proc_Self self, S$u8 out_buf))(proc_Path_E$S$u8)) {
    self = proc_ensureValid(self);
    return self.vtbl->currentPathFn(self.ctx, out_buf);
};
fn_((proc_setCurrentPath(proc_Self self, S_const$u8 path))(proc_Path_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->setCurrentPathFn(self.ctx, path);
};
fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnFn(self.ctx, cmd);
};
fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child)) {
    self = proc_ensureValid(self);
    return self.vtbl->spawnPathFn(self.ctx, dir, cmd);
};
fn_((proc_replace(proc_Self self, proc_Cmd cmd))(proc_Spawn_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replaceFn(self.ctx, cmd);
};
fn_((proc_replacePath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$void)) {
    self = proc_ensureValid(self);
    return self.vtbl->replacePathFn(self.ctx, dir, cmd);
};
