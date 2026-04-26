#include "Dir.h"
#include "File/self.h"

fn_((fs_Dir_close(fs_Dir* self, fs_Self fs))(void)) {
    claim_assert_nonnull(self);
    fs = fs_ensureValid(fs);
    fs.vtbl->dir.closeFn(fs.ctx, *self);
};

fn_((fs_Dir_access(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_AccessOpts opts))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.accessFn(fs.ctx, self, sub_path, opts);
};

fn_((fs_Dir_stat(fs_Dir self, fs_Self fs))(fs_E$fs_Dir_Stat)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.statFn(fs.ctx, self);
};

fn_((fs_Dir_statFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_StatFileOpts opts))(fs_E$fs_File_Stat)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.statFileFn(fs.ctx, self, sub_path, opts);
};

fn_((fs_Dir_rename(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.renameFn(fs.ctx, self, old_sub_path, new_sub_path);
};

fn_((fs_Dir_renamePreserve(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.renamePreserveFn(fs.ctx, self, old_sub_path, new_sub_path);
};

fn_((fs_Dir_makePath(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.makePathFn(fs.ctx, self, sub_path);
};

fn_((fs_Dir_makePathStatus(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$fs_Dir_CreatePathStatus)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.makePathStatusFn(fs.ctx, self, sub_path);
};

fn_((fs_Dir_realpath(fs_Dir self, fs_Self fs, S_const$u8 pathname, S$u8 out_buf))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.realpathFn(fs.ctx, self, pathname, out_buf);
};

fn_((fs_Dir_realpathAlloc(fs_Dir self, fs_Self fs, S_const$u8 pathname, mem_Alctr gpa))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.realpathAllocFn(fs.ctx, self, pathname, gpa);
};

fn_((fs_Dir_realpathFile(fs_Dir self, fs_Self fs, S_const$u8 pathname, S$u8 out_buf))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.realpathFileFn(fs.ctx, self, pathname, out_buf);
};

fn_((fs_Dir_readLink(fs_Dir self, fs_Self fs, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.readLinkFn(fs.ctx, self, sub_path, out_buf);
};

fn_((fs_Dir_makeDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.makeDirFn(fs.ctx, self, sub_path);
};

fn_((fs_Dir_deleteDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.deleteDirFn(fs.ctx, self, sub_path);
};

fn_((fs_Dir_openDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.openDirFn(fs.ctx, self, sub_path, opts);
};

fn_((fs_Dir_openPath(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.openPathFn(fs.ctx, self, sub_path, opts);
};

fn_((fs_Dir_hardLink(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, fs_Dir new_dir, S_const$u8 new_sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.hardLinkFn(fs.ctx, self, old_sub_path, new_dir, new_sub_path);
};

fn_((fs_Dir_symLink(fs_Dir self, fs_Self fs, S_const$u8 target_path, S_const$u8 sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.symLinkFn(fs.ctx, self, target_path, sub_path);
};

fn_((fs_Dir_createFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_CreateFileOpts flags))(E$fs_File)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.createFileFn(fs.ctx, self, sub_path, flags);
};

fn_((fs_Dir_openFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenFileOpts flags))(E$fs_File)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.openFileFn(fs.ctx, self, sub_path, flags);
};

fn_((fs_Dir_deleteFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.deleteFileFn(fs.ctx, self, sub_path);
};

fn_((fs_Dir_readFile(fs_Dir self, fs_Self fs, S_const$u8 file_path, S$u8 buf))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.readFileFn(fs.ctx, self, file_path, buf);
};

fn_((fs_Dir_readFileAlloc(fs_Dir self, fs_Self fs, S_const$u8 file_path, mem_Alctr gpa, usize max_bytes))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->dir.readFileAllocFn(fs.ctx, self, file_path, gpa, max_bytes);
};

#if UNUSED_CODE
fn_((fs_Dir_Reader_init(fs_Dir self, S$u8 buf))(fs_Dir_Reader)) {
    return (fs_Dir_Reader){
        .dir = self,
        .state = fs_Dir_Reader_State_reset,
        .buf = buf,
        .index = 0,
        .end = 0,
    };
};

fn_((fs_Dir_Reader_reset(fs_Dir_Reader* self))(void)) {
    claim_assert_nonnull(self);
    self->state = fs_Dir_Reader_State_reset;
    self->index = 0;
    self->end = 0;
};

fn_((fs_Dir_Reader_read(fs_Dir_Reader* self, S$fs_Dir_Entry out))(E$usize) $scope) {
    claim_assert_nonnull(self);
    let_ignore = out;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_Iterator_init(fs_Dir self, fs_Dir_Reader_State initial_state))(fs_Dir_Iterator)) {
    var_(self, fs_Dir_Iterator) = {
        .reader_buf = cleared(),
    };
    self.reader = fs_Dir_Reader_init(self, A_ref$((S$u8)self.reader_buf));
    self.reader.state = initial_state;
    return self;
};

fn_((fs_Dir_iterate(fs_Dir self))(fs_Dir_Iterator)) {
    return fs_Dir_Iterator_init(dir, fs_Dir_Reader_State_reset);
};

fn_((fs_Dir_iterateAssumeFirstIteration(fs_Dir self))(fs_Dir_Iterator)) {
    return fs_Dir_Iterator_init(dir, fs_Dir_Reader_State_reading);
};

fn_((fs_Dir_Iterator_next(fs_Dir_Iterator* self))(E$O$fs_Dir_Entry) $scope) {
    claim_assert_nonnull(self);
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_walkSelectively(fs_Dir self, mem_Alctr gpa))(E$fs_Dir_SelectiveWalker) $scope) {
    let_ignore = dir;
    let_ignore = gpa;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_SelectiveWalker_fini(fs_Dir_SelectiveWalker* self))(void)) {
    let_ignore = self;
};

fn_((fs_Dir_SelectiveWalker_next(fs_Dir_SelectiveWalker* self))(E$O$fs_Dir_Walker_Entry) $scope) {
    let_ignore = self;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_SelectiveWalker_enter(fs_Dir_SelectiveWalker* self, fs_Dir_Walker_Entry entry))(E$void) $scope) {
    let_ignore = self;
    let_ignore = entry;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_SelectiveWalker_leave(fs_Dir_SelectiveWalker* self))(void)) {
    let_ignore = self;
};

fn_((fs_Dir_walk(fs_Dir self, mem_Alctr gpa))(E$fs_Dir_Walker) $scope) {
    let_ignore = dir;
    let_ignore = gpa;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_Dir_Walker_fini(fs_Dir_Walker* self))(void)) {
    if (isNonnull(self)) fs_Dir_SelectiveWalker_fini(&self->inner);
};

fn_((fs_Dir_Walker_next(fs_Dir_Walker* self))(E$O$fs_Dir_Walker_Entry) $scope) {
    claim_assert_nonnull(self);
    return fs_Dir_SelectiveWalker_next(&self->inner);
} $unscoped(fn);

fn_((fs_Dir_Walker_leave(fs_Dir_Walker* self))(void)) {
    if (isNonnull(self)) fs_Dir_SelectiveWalker_leave(&self->inner);
};

#endif /* UNUSED_CODE */
