#include "self.h"

fn_((fs_File_close(fs_File self, fs_Self fs))(void)) {
    fs = fs_ensureValid(fs);
    fs.vtbl->file.closeFn(fs.ctx, self);
};

fn_((fs_File_stat(fs_File self, fs_Self fs))(fs_E$fs_File_Stat)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.statFn(fs.ctx, self);
};

fn_((fs_File_sync(fs_File self, fs_Self fs))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.syncFn(fs.ctx, self);
};

fn_((fs_File_isTty(fs_File self, fs_Self fs))(E$bool)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.isTtyFn(fs.ctx, self);
};

fn_((fs_File_setLen(fs_File self, fs_Self fs, u64 new_len))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.setLenFn(fs.ctx, self, new_len);
};

fn_((fs_File_setPerms(fs_File self, fs_Self fs, fs_File_Mode perms))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.setPermsFn(fs.ctx, self, perms);
};

fn_((fs_File_readPos(fs_File self, fs_Self fs, S$u8 buf, u64 offset))(E$usize)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.readPosFn(fs.ctx, self, buf, offset);
};

fn_((fs_File_writePos(fs_File self, fs_Self fs, S_const$u8 buf, u64 offset))(E$usize)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.writePosFn(fs.ctx, self, buf, offset);
};

fn_((fs_File_seekBy(fs_File self, fs_Self fs, i64 rel_offset))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.seekByFn(fs.ctx, self, rel_offset);
};

fn_((fs_File_seekTo(fs_File self, fs_Self fs, u64 abs_offset))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.seekToFn(fs.ctx, self, abs_offset);
};

fn_((fs_File_lock(fs_File self, fs_Self fs, fs_Lock lock))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.lockFn(fs.ctx, self, lock);
};

fn_((fs_File_tryLock(fs_File self, fs_Self fs, fs_Lock lock))(E$bool)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.tryLockFn(fs.ctx, self, lock);
};

fn_((fs_File_unlock(fs_File self, fs_Self fs))(void)) {
    fs = fs_ensureValid(fs);
    fs.vtbl->file.unlockFn(fs.ctx, self);
};

fn_((fs_File_downgradeLock(fs_File self, fs_Self fs))(E$void)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.downgradeLockFn(fs.ctx, self);
};

fn_((fs_File_realpath(fs_File self, fs_Self fs, S$u8 out_buf))(E$S$u8)) {
    fs = fs_ensureValid(fs);
    return fs.vtbl->file.realpathFn(fs.ctx, self, out_buf);
};

fn_((fs_File_realpathAlloc(fs_File self, fs_Self fs, mem_Alctr gpa))(E$S$u8) $scope) {
    let_ignore = self;
    let_ignore = fs;
    let_ignore = gpa;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_File_io(fs_File self, fs_Self fs))(fs_File_IO)) {
    fs = fs_ensureValid(fs);
    return (fs_File_IO){
        .fs = fs,
        .file = self,
    };
};

$attr($must_check)
$static fn_((fs_File_IO__read(P$raw ctx, S$u8 buf))(E$usize));
fn_((fs_File_IO_reader(fs_File_IO* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return (io_Reader){
        .ctx = self,
        .read = fs_File_IO__read,
    };
};

$attr($must_check)
$static fn_((fs_File_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize));
fn_((fs_File_IO_writer(fs_File_IO* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return (io_Writer){
        .ctx = self,
        .write = fs_File_IO__write,
    };
};

fn_((fs_File_IO__read(P$raw ctx, S$u8 buf))(E$usize)) {
    let self = ptrCast$((fs_File_IO*)(ensureNonnull(ctx)));
    let fs = fs_ensureValid(self->fs);
    return fs.vtbl->file.readFn(fs.ctx, self->file, buf);
};

fn_((fs_File_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize)) {
    let self = ptrCast$((fs_File_IO*)(ensureNonnull(ctx)));
    let fs = fs_ensureValid(self->fs);
    return fs.vtbl->file.writeFn(fs.ctx, self->file, bytes);
};
