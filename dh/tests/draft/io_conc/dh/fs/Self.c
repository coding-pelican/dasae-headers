#include "Self.h"
#include "Dir.h"
#include "File/self.h"
#include "../exec/Coop.h"

#if plat_is_windows
#include "dh/os/windows/handle.h"
#include "dh/os/windows/file.h"
#endif /* plat_is_windows */
#if plat_is_posix
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif /* plat_is_posix */

/*========== Internal Declarations ==========================================*/

$static fn_((fs_direct__Op_cwd(P$raw ctx))(E$fs_Dir));
$static fn_((fs_direct__Op_openDir(P$raw ctx, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$static fn_((fs_direct__Op_createDir(P$raw ctx, S_const$u8 path))(E$void));
$static fn_((fs_direct__Op_deleteDir(P$raw ctx, S_const$u8 path))(E$void));
$static fn_((fs_direct__Op_openFile(P$raw ctx, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File));
$static fn_((fs_direct__Op_createFile(P$raw ctx, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File));
$static fn_((fs_direct__Op_deleteFile(P$raw ctx, S_const$u8 path))(E$void));
$static fn_((fs_direct__Op_rename(P$raw ctx, S_const$u8 old_path, S_const$u8 new_path))(E$void));
$static fn_((fs_direct__Op_realpath(P$raw ctx, S_const$u8 path, S$u8 out_buf))(E$S$u8));
$static fn_((fs_direct__Dir_close(P$raw ctx, fs_Dir dir))(void));
$static fn_((fs_direct__Dir_stat(P$raw ctx, fs_Dir dir))(fs_E$fs_Dir_Stat));
$static fn_((fs_direct__File_close(P$raw ctx, fs_File file))(void));
$static fn_((fs_direct__File_read(P$raw ctx, fs_File file, S$u8 buf))(E$usize));
$static fn_((fs_direct__File_write(P$raw ctx, fs_File file, S_const$u8 bytes))(E$usize));
$static fn_((fs_direct__File_stat(P$raw ctx, fs_File file))(fs_E$fs_File_Stat));
$static fn_((fs_direct__File_sync(P$raw ctx, fs_File file))(E$void));
$static fn_((fs_direct__File_isTty(P$raw ctx, fs_File file))(E$bool));
$static fn_((fs_direct__File_setLen(P$raw ctx, fs_File file, u64 new_len))(E$void));
$static fn_((fs_direct__File_setPerms(P$raw ctx, fs_File file, fs_File_Mode perms))(E$void));
$static fn_((fs_direct__File_readPos(P$raw ctx, fs_File file, S$u8 buf, u64 offset))(E$usize));
$static fn_((fs_direct__File_writePos(P$raw ctx, fs_File file, S_const$u8 buf, u64 offset))(E$usize));
$static fn_((fs_direct__File_seekBy(P$raw ctx, fs_File file, i64 rel_offset))(E$void));
$static fn_((fs_direct__File_seekTo(P$raw ctx, fs_File file, u64 abs_offset))(E$void));

/*========== External Definitions ===========================================*/

let_(fs_VTbl_noop, fs_Self_VTbl) = {
    .op = {
        .cwdFn = fs_VTbl_Op_failingCWD,
        .openDirFn = fs_VTbl_Op_failingOpenDir,
        .createDirFn = fs_VTbl_Op_failingCreateDir,
        .deleteDirFn = fs_VTbl_Op_failingDeleteDir,
        .openFileFn = fs_VTbl_Op_failingOpenFile,
        .createFileFn = fs_VTbl_Op_failingCreateFile,
        .deleteFileFn = fs_VTbl_Op_failingDeleteFile,
        .renameFn = fs_VTbl_Op_failingRename,
        .realpathFn = fs_VTbl_Op_failingRealpath,
    },
    .dir = {
        .closeFn = fs_VTbl_Dir_noClose,
        .accessFn = fs_VTbl_Dir_failingAccess,
        .statFn = fs_VTbl_Dir_failingStat,
        .statFileFn = fs_VTbl_Dir_failingStatFile,
        .renameFn = fs_VTbl_Dir_failingRename,
        .renamePreserveFn = fs_VTbl_Dir_failingRenamePreserve,
        .makePathFn = fs_VTbl_Dir_failingMakePath,
        .makePathStatusFn = fs_VTbl_Dir_failingMakePathStatus,
        .realpathFn = fs_VTbl_Dir_failingRealpath,
        .realpathAllocFn = fs_VTbl_Dir_failingRealpathAlloc,
        .realpathFileFn = fs_VTbl_Dir_failingRealpathFile,
        .readLinkFn = fs_VTbl_Dir_failingReadLink,
        .makeDirFn = fs_VTbl_Dir_failingMakeDir,
        .deleteDirFn = fs_VTbl_Dir_failingDeleteDir,
        .openDirFn = fs_VTbl_Dir_failingOpenDir,
        .openPathFn = fs_VTbl_Dir_failingOpenPath,
        .hardLinkFn = fs_VTbl_Dir_failingHardLink,
        .symLinkFn = fs_VTbl_Dir_failingSymLink,
        .createFileFn = fs_VTbl_Dir_failingCreateFile,
        .openFileFn = fs_VTbl_Dir_failingOpenFile,
        .deleteFileFn = fs_VTbl_Dir_failingDeleteFile,
        .readFileFn = fs_VTbl_Dir_failingReadFile,
        .readFileAllocFn = fs_VTbl_Dir_failingReadFileAlloc,
    },
    .file = {
        .closeFn = fs_VTbl_File_noClose,
        .readFn = fs_VTbl_File_failingRead,
        .writeFn = fs_VTbl_File_failingWrite,
        .statFn = fs_VTbl_File_failingStat,
        .syncFn = fs_VTbl_File_failingSync,
        .isTtyFn = fs_VTbl_File_failingIsTty,
        .setLenFn = fs_VTbl_File_failingSetLen,
        .setPermsFn = fs_VTbl_File_failingSetPerms,
        .readPosFn = fs_VTbl_File_failingReadPos,
        .writePosFn = fs_VTbl_File_failingWritePos,
        .seekByFn = fs_VTbl_File_failingSeekBy,
        .seekToFn = fs_VTbl_File_failingSeekTo,
        .lockFn = fs_VTbl_File_failingLock,
        .tryLockFn = fs_VTbl_File_failingTryLock,
        .unlockFn = fs_VTbl_File_noUnlock,
        .downgradeLockFn = fs_VTbl_File_failingDowngradeLock,
        .realpathFn = fs_VTbl_File_failingRealpath,
    },
};
let_(fs_VTbl_failing, fs_Self_VTbl) = {
    .op = {
        .cwdFn = fs_VTbl_Op_failingCWD,
        .openDirFn = fs_VTbl_Op_failingOpenDir,
        .createDirFn = fs_VTbl_Op_failingCreateDir,
        .deleteDirFn = fs_VTbl_Op_failingDeleteDir,
        .openFileFn = fs_VTbl_Op_failingOpenFile,
        .createFileFn = fs_VTbl_Op_failingCreateFile,
        .deleteFileFn = fs_VTbl_Op_failingDeleteFile,
        .renameFn = fs_VTbl_Op_failingRename,
        .realpathFn = fs_VTbl_Op_failingRealpath,
    },
    .dir = {
        .closeFn = fs_VTbl_Dir_unreachableClose,
        .accessFn = fs_VTbl_Dir_failingAccess,
        .statFn = fs_VTbl_Dir_failingStat,
        .statFileFn = fs_VTbl_Dir_failingStatFile,
        .renameFn = fs_VTbl_Dir_failingRename,
        .renamePreserveFn = fs_VTbl_Dir_failingRenamePreserve,
        .makePathFn = fs_VTbl_Dir_failingMakePath,
        .makePathStatusFn = fs_VTbl_Dir_failingMakePathStatus,
        .realpathFn = fs_VTbl_Dir_failingRealpath,
        .realpathAllocFn = fs_VTbl_Dir_failingRealpathAlloc,
        .realpathFileFn = fs_VTbl_Dir_failingRealpathFile,
        .readLinkFn = fs_VTbl_Dir_failingReadLink,
        .makeDirFn = fs_VTbl_Dir_failingMakeDir,
        .deleteDirFn = fs_VTbl_Dir_failingDeleteDir,
        .openDirFn = fs_VTbl_Dir_failingOpenDir,
        .openPathFn = fs_VTbl_Dir_failingOpenPath,
        .hardLinkFn = fs_VTbl_Dir_failingHardLink,
        .symLinkFn = fs_VTbl_Dir_failingSymLink,
        .createFileFn = fs_VTbl_Dir_failingCreateFile,
        .openFileFn = fs_VTbl_Dir_failingOpenFile,
        .deleteFileFn = fs_VTbl_Dir_failingDeleteFile,
        .readFileFn = fs_VTbl_Dir_failingReadFile,
        .readFileAllocFn = fs_VTbl_Dir_failingReadFileAlloc,
    },
    .file = {
        .closeFn = fs_VTbl_File_unreachableClose,
        .readFn = fs_VTbl_File_failingRead,
        .writeFn = fs_VTbl_File_failingWrite,
        .statFn = fs_VTbl_File_failingStat,
        .syncFn = fs_VTbl_File_failingSync,
        .isTtyFn = fs_VTbl_File_failingIsTty,
        .setLenFn = fs_VTbl_File_failingSetLen,
        .setPermsFn = fs_VTbl_File_failingSetPerms,
        .readPosFn = fs_VTbl_File_failingReadPos,
        .writePosFn = fs_VTbl_File_failingWritePos,
        .seekByFn = fs_VTbl_File_failingSeekBy,
        .seekToFn = fs_VTbl_File_failingSeekTo,
        .lockFn = fs_VTbl_File_failingLock,
        .tryLockFn = fs_VTbl_File_failingTryLock,
        .unlockFn = fs_VTbl_File_unreachableUnlock,
        .downgradeLockFn = fs_VTbl_File_failingDowngradeLock,
        .realpathFn = fs_VTbl_File_failingRealpath,
    },
};

fn_((fs_cwd(fs_Self self))(E$fs_Dir)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.cwdFn(self.ctx);
}

fn_((fs_openDir(fs_Self self, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.openDirFn(self.ctx, path, opts);
}

fn_((fs_createDir(fs_Self self, S_const$u8 path))(E$void)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.createDirFn(self.ctx, path);
}

fn_((fs_deleteDir(fs_Self self, S_const$u8 path))(E$void)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.deleteDirFn(self.ctx, path);
}

fn_((fs_openFile(fs_Self self, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.openFileFn(self.ctx, path, flags);
}

fn_((fs_createFile(fs_Self self, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.createFileFn(self.ctx, path, flags);
}

fn_((fs_deleteFile(fs_Self self, S_const$u8 path))(E$void)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.deleteFileFn(self.ctx, path);
}

fn_((fs_rename(fs_Self self, S_const$u8 old_path, S_const$u8 new_path))(E$void)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.renameFn(self.ctx, old_path, new_path);
}

fn_((fs_realpath(fs_Self self, S_const$u8 path, S$u8 out_buf))(E$S$u8)) {
    self = fs_ensureValid(self);
    return self.vtbl->op.realpathFn(self.ctx, path, out_buf);
}

$static var_(fs_noop__ctx, Void) = cleared();
let_(fs_noop, fs_Self) = {
    .ctx = &fs_noop__ctx,
    .vtbl = &fs_VTbl_noop,
};

$static var_(fs_failing__ctx, Void) = cleared();
let_(fs_failing, fs_Self) = {
    .ctx = &fs_failing__ctx,
    .vtbl = &fs_VTbl_failing,
};

fn_((fs_direct(void))(fs_direct_E$fs_Self) $scope) {
    pp_if_(fs_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) = cleared();
            $static let_(vtbl, fs_Self_VTbl) = {
                .op = {
                    .cwdFn = fs_direct__Op_cwd,
                    .openDirFn = fs_direct__Op_openDir,
                    .createDirFn = fs_direct__Op_createDir,
                    .deleteDirFn = fs_direct__Op_deleteDir,
                    .openFileFn = fs_direct__Op_openFile,
                    .createFileFn = fs_direct__Op_createFile,
                    .deleteFileFn = fs_direct__Op_deleteFile,
                    .renameFn = fs_direct__Op_rename,
                    .realpathFn = fs_direct__Op_realpath,
                },
                .file = {
                    .closeFn = fs_direct__File_close,
                    .readFn = fs_direct__File_read,
                    .writeFn = fs_direct__File_write,
                    .statFn = fs_direct__File_stat,
                    .syncFn = fs_direct__File_sync,
                    .isTtyFn = fs_direct__File_isTty,
                    .setLenFn = fs_direct__File_setLen,
                    .setPermsFn = fs_direct__File_setPerms,
                    .readPosFn = fs_direct__File_readPos,
                    .writePosFn = fs_direct__File_writePos,
                    .seekByFn = fs_direct__File_seekBy,
                    .seekToFn = fs_direct__File_seekTo,
                    .lockFn = fs_VTbl_File_failingLock,
                    .tryLockFn = fs_VTbl_File_failingTryLock,
                    .unlockFn = fs_VTbl_File_noUnlock,
                    .downgradeLockFn = fs_VTbl_File_failingDowngradeLock,
                    .realpathFn = fs_VTbl_File_failingRealpath,
                },
                .dir = {
                    .closeFn = fs_direct__Dir_close,
                    .accessFn = fs_VTbl_Dir_failingAccess,
                    .statFn = fs_direct__Dir_stat,
                    .statFileFn = fs_VTbl_Dir_failingStatFile,
                    .renameFn = fs_VTbl_Dir_failingRename,
                    .renamePreserveFn = fs_VTbl_Dir_failingRenamePreserve,
                    .makePathFn = fs_VTbl_Dir_failingMakePath,
                    .makePathStatusFn = fs_VTbl_Dir_failingMakePathStatus,
                    .realpathFn = fs_VTbl_Dir_failingRealpath,
                    .realpathAllocFn = fs_VTbl_Dir_failingRealpathAlloc,
                    .realpathFileFn = fs_VTbl_Dir_failingRealpathFile,
                    .readLinkFn = fs_VTbl_Dir_failingReadLink,
                    .makeDirFn = fs_VTbl_Dir_failingMakeDir,
                    .deleteDirFn = fs_VTbl_Dir_failingDeleteDir,
                    .openDirFn = fs_VTbl_Dir_failingOpenDir,
                    .openPathFn = fs_VTbl_Dir_failingOpenPath,
                    .hardLinkFn = fs_VTbl_Dir_failingHardLink,
                    .symLinkFn = fs_VTbl_Dir_failingSymLink,
                    .createFileFn = fs_VTbl_Dir_failingCreateFile,
                    .openFileFn = fs_VTbl_Dir_failingOpenFile,
                    .deleteFileFn = fs_VTbl_Dir_failingDeleteFile,
                    .readFileFn = fs_VTbl_Dir_failingReadFile,
                    .readFileAllocFn = fs_VTbl_Dir_failingReadFileAlloc,
                },
            };
            return_ok(fs_ensureValid((fs_Self){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(fs_direct_E_Unsupported());
        })
    );
} $unscoped(fn);

fn_((fs_evented(exec_Coop* coop))(fs_Self)) {
    claim_assert_nonnull(coop);
    let_ignore = exec_Coop_evented(coop);
    return fs_failing;
}

/*========== Internal Definitions ===========================================*/

fn_((fs_VTbl_Op_failingCWD(P$raw ctx))(E$fs_Dir) $scope) {
    let_ignore = ctx;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingOpenDir(P$raw ctx, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    let_ignore = opts;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingCreateDir(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingDeleteDir(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingOpenFile(P$raw ctx, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingCreateFile(P$raw ctx, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingDeleteFile(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingRename(P$raw ctx, S_const$u8 old_path, S_const$u8 new_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = old_path;
    let_ignore = new_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Op_failingRealpath(P$raw ctx, S_const$u8 path, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = path;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_VTbl_File_noClose(P$raw ctx, fs_File file))(void)) {
    let_ignore = ctx;
    let_ignore = file;
}
fn_((fs_VTbl_File_unreachableClose(P$raw ctx, fs_File file))(void)) {
    let_ignore = ctx;
    let_ignore = file;
    claim_unreachable_msg("");
};
fn_((fs_VTbl_File_failingRead(P$raw ctx, fs_File file, S$u8 buf))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingWrite(P$raw ctx, fs_File file, S_const$u8 bytes))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = bytes;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingStat(P$raw ctx, fs_File file))(fs_E$fs_File_Stat) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingSync(P$raw ctx, fs_File file))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingIsTty(P$raw ctx, fs_File file))(E$bool) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingSetLen(P$raw ctx, fs_File file, u64 new_len))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = new_len;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingSetPerms(P$raw ctx, fs_File file, fs_File_Mode perms))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = perms;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingReadPos(P$raw ctx, fs_File file, S$u8 buf, u64 offset))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = buf;
    let_ignore = offset;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingWritePos(P$raw ctx, fs_File file, S_const$u8 buf, u64 offset))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = buf;
    let_ignore = offset;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingSeekBy(P$raw ctx, fs_File file, i64 rel_offset))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = rel_offset;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingSeekTo(P$raw ctx, fs_File file, u64 abs_offset))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = abs_offset;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingLock(P$raw ctx, fs_File file, fs_Lock lock))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = lock;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingTryLock(P$raw ctx, fs_File file, fs_Lock lock))(E$bool) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = lock;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_noUnlock(P$raw ctx, fs_File file))(void)) {
    let_ignore = ctx;
    let_ignore = file;
}
fn_((fs_VTbl_File_unreachableUnlock(P$raw ctx, fs_File file))(void)) {
    let_ignore = ctx;
    let_ignore = file;
    claim_unreachable_msg("");
};
fn_((fs_VTbl_File_failingDowngradeLock(P$raw ctx, fs_File file))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_File_failingRealpath(P$raw ctx, fs_File file, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = file;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

fn_((fs_VTbl_Dir_noClose(P$raw ctx, fs_Dir dir))(void)) {
    let_ignore = ctx;
    let_ignore = dir;
}
fn_((fs_VTbl_Dir_unreachableClose(P$raw ctx, fs_Dir dir))(void)) {
    let_ignore = ctx;
    let_ignore = dir;
    claim_unreachable_msg("");
};
fn_((fs_VTbl_Dir_failingAccess(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_AccessOpts opts))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = opts;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingStat(P$raw ctx, fs_Dir dir))(fs_E$fs_Dir_Stat) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingStatFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_StatFileOpts opts))(fs_E$fs_File_Stat) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = opts;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingRename(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = old_sub_path;
    let_ignore = new_sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingRenamePreserve(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = old_sub_path;
    let_ignore = new_sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingMakePath(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingMakePathStatus(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$fs_Dir_CreatePathStatus) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingRealpath(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = pathname;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingRealpathAlloc(P$raw ctx, fs_Dir dir, S_const$u8 pathname, mem_Alctr gpa))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = pathname;
    let_ignore = gpa;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingRealpathFile(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = pathname;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingReadLink(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingMakeDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingDeleteDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingOpenDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = opts;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingOpenPath(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = opts;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingHardLink(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, fs_Dir new_dir, S_const$u8 new_sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = old_sub_path;
    let_ignore = new_dir;
    let_ignore = new_sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingSymLink(P$raw ctx, fs_Dir dir, S_const$u8 target_path, S_const$u8 sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = target_path;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingCreateFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_CreateFileOpts flags))(E$fs_File) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingOpenFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenFileOpts flags))(E$fs_File) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingDeleteFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = sub_path;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingReadFile(P$raw ctx, fs_Dir dir, S_const$u8 file_path, S$u8 buf))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = file_path;
    let_ignore = buf;
    return_err(fs_E_Unsupported());
} $unscoped(fn);
fn_((fs_VTbl_Dir_failingReadFileAlloc(P$raw ctx, fs_Dir dir, S_const$u8 file_path, mem_Alctr gpa, usize max_bytes))(E$S$u8) $scope) {
    let_ignore = ctx;
    let_ignore = dir;
    let_ignore = file_path;
    let_ignore = gpa;
    let_ignore = max_bytes;
    return_err(fs_E_Unsupported());
} $unscoped(fn);

#if plat_is_posix
$static fn_((fs__direct_posixKind(mode_t mode))(fs_Kind)) {
    if (S_ISREG(mode)) return fs_Kind_file;
    if (S_ISDIR(mode)) return fs_Kind_directory;
    if (S_ISCHR(mode)) return fs_Kind_character_device;
    if (S_ISBLK(mode)) return fs_Kind_block_device;
    if (S_ISFIFO(mode)) return fs_Kind_named_pipe;
    if (S_ISLNK(mode)) return fs_Kind_sym_link;
    if (S_ISSOCK(mode)) return fs_Kind_unix_domain_socket;
    return fs_Kind_unknown;
}
#endif /* plat_is_posix */

#if plat_is_windows
$static fn_((fs__direct_pathZ(S_const$u8 path, u8* out, usize out_len))(bool)) {
    if (path.len + 1 > out_len) return false;
    mem_copyBytes((S$u8){ .ptr = out, .len = path.len }, path);
    out[path.len] = 0;
    return true;
}

$static fn_((fs__direct_windowsAccess(fs_OpenFlags flags))(DWORD)) {
    var_(access, DWORD) = 0;
    if (fs_OpenFlags_isRead(flags)) access |= GENERIC_READ;
    if (fs_OpenFlags_isWrite(flags)) access |= GENERIC_WRITE;
    return access;
}

$static fn_((fs__direct_windowsCreateAccess(fs_CreateFlags flags))(DWORD)) {
    return GENERIC_WRITE | (flags.read ? GENERIC_READ : 0);
}

$static fn_((fs__direct_windowsFlags(void))(DWORD)) {
    return FILE_ATTRIBUTE_NORMAL;
}

$static fn_((fs__direct_windowsTime(FILETIME ft))(time_Real_Inst)) {
    var_(raw, ULARGE_INTEGER) = {};
    raw.LowPart = ft.dwLowDateTime;
    raw.HighPart = ft.dwHighDateTime;
    let secs = raw.QuadPart / 10000000;
    return time_Real_Inst_fromUnixEpoch(secs < 11644473600 ? 0 : secs - 11644473600);
}

$static fn_((fs__direct_windowsKind(DWORD attrs))(fs_Kind)) {
    if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0) return fs_Kind_directory;
    return fs_Kind_file;
}
#endif /* plat_is_windows */

fn_((fs_direct__Op_cwd(P$raw ctx))(E$fs_Dir) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if_(let handle = open(".", O_RDONLY), handle < 0) {
        return_err(fs_E_OpenFailed());
    } else {
        return_ok(fs_Dir_Handle_promote(handle));
    }
#elif plat_is_windows
    let handle = CreateFileA(
        ".",
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        null,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
        null
    );
    if (handle == INVALID_HANDLE_VALUE) return_err(fs_E_OpenFailed());
    return_ok(fs_Dir_Handle_promote(handle));
#else
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_openDir(P$raw ctx, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir) $scope) {
    let_ignore = ctx;
    let_ignore = opts;
#if plat_is_posix
    if_(let handle = open(as$(const char*)(path.ptr), O_RDONLY), handle < 0) {
        return_err(fs_E_OpenFailed());
    } else {
        return_ok(fs_Dir_Handle_promote(handle));
    }
#elif plat_is_windows
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    let handle = CreateFileA(
        as$(LPCSTR)(ptr$A(path_z)), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(fs_E_OpenFailed());
    return_ok(fs_Dir_Handle_promote(handle));
#else
    let_ignore = path;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_createDir(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (mkdir(as$(const char*)(path.ptr), fs_Dir_Mode_default) != 0) return_err(fs_E_WriteFailed());
    return_ok({});
#elif plat_is_windows
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    if (!CreateDirectoryA(as$(LPCSTR)(ptr$A(path_z)), null)) {
        let err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS) return_err(fs_E_PathAlreadyExists());
        return_err(fs_E_WriteFailed());
    }
    return_ok({});
#else
    let_ignore = path;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_deleteDir(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (rmdir(as$(const char*)(path.ptr)) != 0) return_err(fs_E_NotFound());
    return_ok({});
#elif plat_is_windows
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    if (!RemoveDirectoryA(as$(LPCSTR)(ptr$A(path_z)))) return_err(fs_E_NotFound());
    return_ok({});
#else
    let_ignore = path;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_openFile(P$raw ctx, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    var_(open_flags, i32) = 0;
    if (flags.mode == fs_OpenMode_read_only) open_flags |= O_RDONLY;
    if (flags.mode == fs_OpenMode_write_only) open_flags |= O_WRONLY;
    if (flags.mode == fs_OpenMode_read_write) open_flags |= O_RDWR;
    if (flags.nonblocking) open_flags |= O_NONBLOCK;
    if_(let handle = open(as$(const char*)(path.ptr), open_flags), handle < 0) {
        return_err(fs_E_OpenFailed());
    } else {
        return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#elif plat_is_windows
    if (flags.nonblocking) return_err(fs_E_Unsupported());
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    let handle = CreateFileA(
        as$(LPCSTR)(ptr$A(path_z)), fs__direct_windowsAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, fs__direct_windowsFlags(), null);
    if (handle == INVALID_HANDLE_VALUE) {
        return_err(fs_E_OpenFailed());
    }
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#else
    let_ignore = path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_createFile(P$raw ctx, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    var_(open_flags, i32) = O_CREAT | O_WRONLY;
    if (flags.read) open_flags = O_CREAT | O_RDWR;
    if (flags.truncate) open_flags |= O_TRUNC;
    if (flags.exclusive) open_flags |= O_EXCL;
    if (flags.nonblocking) open_flags |= O_NONBLOCK;
    if_(let handle = open(as$(const char*)(path.ptr), open_flags, flags.mode), handle < 0) {
        return_err(fs_E_OpenFailed());
    } else {
        return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#elif plat_is_windows
    if (flags.nonblocking) return_err(fs_E_Unsupported());
    let disposition = flags.exclusive
                        ? CREATE_NEW
                        : (flags.truncate ? CREATE_ALWAYS : OPEN_ALWAYS);
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    let handle = CreateFileA(
        as$(LPCSTR)(ptr$A(path_z)), fs__direct_windowsCreateAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null,
        as$(DWORD)(disposition), fs__direct_windowsFlags(), null);
    if (handle == INVALID_HANDLE_VALUE) {
        return_err(fs_E_OpenFailed());
    }
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#else
    let_ignore = path;
    let_ignore = flags;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_deleteFile(P$raw ctx, S_const$u8 path))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (unlink(as$(const char*)(path.ptr)) != 0) return_err(fs_E_NotFound());
    return_ok({});
#elif plat_is_windows
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    if (!DeleteFileA(as$(LPCSTR)(ptr$A(path_z)))) return_err(fs_E_NotFound());
    return_ok({});
#else
    let_ignore = path;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_rename(P$raw ctx, S_const$u8 old_path, S_const$u8 new_path))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (rename(as$(const char*)(old_path.ptr), as$(const char*)(new_path.ptr)) != 0) return_err(fs_E_NotFound());
    return_ok({});
#elif plat_is_windows
    var_(old_path_z, A$$(32768, u8)) = A_zero();
    var_(new_path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(old_path, ptr$A(old_path_z), len$A(old_path_z))) return_err(fs_E_FileTooBig());
    if (!fs__direct_pathZ(new_path, ptr$A(new_path_z), len$A(new_path_z))) return_err(fs_E_FileTooBig());
    if (!MoveFileExA(as$(LPCSTR)(ptr$A(old_path_z)), as$(LPCSTR)(ptr$A(new_path_z)), MOVEFILE_REPLACE_EXISTING)) return_err(fs_E_WriteFailed());
    return_ok({});
#else
    let_ignore = old_path;
    let_ignore = new_path;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Op_realpath(P$raw ctx, S_const$u8 path, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    let_ignore = path;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
#elif plat_is_windows
    var_(path_z, A$$(32768, u8)) = A_zero();
    if (!fs__direct_pathZ(path, ptr$A(path_z), len$A(path_z))) return_err(fs_E_FileTooBig());
    let len = GetFullPathNameA(as$(LPCSTR)(ptr$A(path_z)), as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr), null);
    if (len == 0) return_err(fs_E_NotFound());
    if (out_buf.len < as$(usize)(len)) return_err(fs_E_FileTooBig());
    return_ok(S_slice((out_buf)$r(0, as$(usize)(len))));
#else
    let_ignore = path;
    let_ignore = out_buf;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__Dir_close(P$raw ctx, fs_Dir dir))(void)) {
    let_ignore = ctx;
#if plat_is_posix
    close(dir.handle);
#elif plat_is_windows
    CloseHandle(dir.handle);
#else
    let_ignore = dir;
#endif
}

fn_((fs_direct__Dir_stat(P$raw ctx, fs_Dir dir))(fs_E$fs_Dir_Stat)) {
    return fs_direct__File_stat(ctx, fs_File_Handle_promote(dir.handle, fs_File_Flags_default));
}

fn_((fs_direct__File_close(P$raw ctx, fs_File file))(void)) {
    let_ignore = ctx;
#if plat_is_posix
    close(file.handle);
#elif plat_is_windows
    CloseHandle(file.handle);
#else
    let_ignore = file;
#endif
}

fn_((fs_direct__File_read(P$raw ctx, fs_File file, S$u8 buf))(E$usize) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if_(let len = read(file.handle, buf.ptr, buf.len), len < 0) {
        return_err(fs_E_ReadFailed());
    } else {
        return_ok(as$(usize)(len));
    }
#elif plat_is_windows
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(file.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(fs_E_ReadFailed());
    }
    return_ok(as$(usize)(bytes_read));
#else
    let_ignore = file;
    let_ignore = buf;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_write(P$raw ctx, fs_File file, S_const$u8 bytes))(E$usize) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if_(let len = write(file.handle, bytes.ptr, bytes.len), len < 0) {
        return_err(fs_E_WriteFailed());
    } else {
        return_ok(as$(usize)(len));
    }
#elif plat_is_windows
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(file.handle, bytes.ptr, as$(DWORD)(bytes.len), &bytes_written, null)) {
        return_err(fs_E_WriteFailed());
    }
    return_ok(as$(usize)(bytes_written));
#else
    let_ignore = file;
    let_ignore = bytes;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_stat(P$raw ctx, fs_File file))(fs_E$fs_File_Stat) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    var_(st, struct stat) = {};
    if (fstat(file.handle, &st) != 0) return_err(fs_E_ReadFailed());
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(st.st_ino),
        .nlink = as$(fs_NLink)(st.st_nlink),
        .size = as$(u64)(st.st_size),
        .permissions = as$(fs_Mode)(st.st_mode),
        .kind = fs__direct_posixKind(st.st_mode),
        .atime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.st_atime)),
        .mtime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.st_mtime)),
        .ctime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.st_ctime)),
        .block_size = as$(fs_BlockSize)(st.st_blksize),
    });
#elif plat_is_windows
    var_(info, BY_HANDLE_FILE_INFORMATION) = {};
    if (!GetFileInformationByHandle(file.handle, &info)) return_err(fs_E_ReadFailed());
    var_(size, ULARGE_INTEGER) = {};
    size.LowPart = info.nFileSizeLow;
    size.HighPart = info.nFileSizeHigh;
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(((u64)info.nFileIndexHigh << 32) | info.nFileIndexLow),
        .nlink = as$(fs_NLink)(info.nNumberOfLinks),
        .size = as$(u64)(size.QuadPart),
        .permissions = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0 ? 0444 : 0666,
        .kind = fs__direct_windowsKind(info.dwFileAttributes),
        .atime = fs__direct_windowsTime(info.ftLastAccessTime),
        .mtime = fs__direct_windowsTime(info.ftLastWriteTime),
        .ctime = fs__direct_windowsTime(info.ftCreationTime),
        .block_size = 4096,
    });
#else
    let_ignore = file;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_sync(P$raw ctx, fs_File file))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (fsync(file.handle) != 0) return_err(fs_E_WriteFailed());
    return_ok({});
#elif plat_is_windows
    if (!FlushFileBuffers(file.handle)) return_err(fs_E_WriteFailed());
    return_ok({});
#else
    let_ignore = file;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_isTty(P$raw ctx, fs_File file))(E$bool) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    return_ok(isatty(file.handle) != 0);
#elif plat_is_windows
    return_ok(GetFileType(file.handle) == FILE_TYPE_CHAR);
#else
    let_ignore = file;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_setLen(P$raw ctx, fs_File file, u64 new_len))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (ftruncate(file.handle, as$(off_t)(new_len)) != 0) return_err(fs_E_WriteFailed());
    return_ok({});
#elif plat_is_windows
    var_(pos, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(new_len) };
    if (!SetFilePointerEx(file.handle, pos, null, FILE_BEGIN)) return_err(fs_E_WriteFailed());
    if (!SetEndOfFile(file.handle)) return_err(fs_E_WriteFailed());
    return_ok({});
#else
    let_ignore = file;
    let_ignore = new_len;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_setPerms(P$raw ctx, fs_File file, fs_File_Mode perms))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (fchmod(file.handle, perms) != 0) return_err(fs_E_PermissionDenied());
    return_ok({});
#else
    let_ignore = file;
    let_ignore = perms;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_readPos(P$raw ctx, fs_File file, S$u8 buf, u64 offset))(E$usize) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if_(let len = pread(file.handle, buf.ptr, buf.len, as$(off_t)(offset)), len < 0) {
        return_err(fs_E_ReadFailed());
    } else {
        return_ok(as$(usize)(len));
    }
#elif plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(file.handle, distance, null, FILE_BEGIN)) return_err(fs_E_ReadFailed());
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(file.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(fs_E_ReadFailed());
    }
    return_ok(as$(usize)(bytes_read));
#else
    let_ignore = file;
    let_ignore = buf;
    let_ignore = offset;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_writePos(P$raw ctx, fs_File file, S_const$u8 buf, u64 offset))(E$usize) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if_(let len = pwrite(file.handle, buf.ptr, buf.len, as$(off_t)(offset)), len < 0) {
        return_err(fs_E_WriteFailed());
    } else {
        return_ok(as$(usize)(len));
    }
#elif plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(file.handle, distance, null, FILE_BEGIN)) return_err(fs_E_WriteFailed());
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(file.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_written, null)) {
        return_err(fs_E_WriteFailed());
    }
    return_ok(as$(usize)(bytes_written));
#else
    let_ignore = file;
    let_ignore = buf;
    let_ignore = offset;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_seekBy(P$raw ctx, fs_File file, i64 rel_offset))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (lseek(file.handle, as$(off_t)(rel_offset), SEEK_CUR) < 0) return_err(fs_E_ReadFailed());
    return_ok({});
#elif plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(rel_offset) };
    if (!SetFilePointerEx(file.handle, distance, null, FILE_CURRENT)) return_err(fs_E_ReadFailed());
    return_ok({});
#else
    let_ignore = file;
    let_ignore = rel_offset;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_direct__File_seekTo(P$raw ctx, fs_File file, u64 abs_offset))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_posix
    if (lseek(file.handle, as$(off_t)(abs_offset), SEEK_SET) < 0) return_err(fs_E_ReadFailed());
    return_ok({});
#elif plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(abs_offset) };
    if (!SetFilePointerEx(file.handle, distance, null, FILE_BEGIN)) return_err(fs_E_ReadFailed());
    return_ok({});
#else
    let_ignore = file;
    let_ignore = abs_offset;
    return_err(fs_E_Unsupported());
#endif
} $unscoped(fn);
