/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-25 (date of creation)
 * @updated 2026-04-25 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs
 */
#ifndef fs_Self__included
#define fs_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/mem/Alctr.h"
#include "../exec/base.h"

/*========== Macros and Declarations ========================================*/

#define fs_direct_supported __bool__fs_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__fs_direct_supported pp_true
#else
#define __bool__fs_direct_supported pp_false
#endif

T_alias$((fs_Self_VTbl)(struct fs_Self_VTbl));
$extern let_(fs_VTbl_noop, fs_Self_VTbl);
$extern let_(fs_VTbl_failing, fs_Self_VTbl);

T_alias$((fs_Self)(struct fs_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(fs_Self_VTbl));
}));
T_impl_E$($set(fs_direct_E)(fs_Self));
$extern let_(fs_noop, fs_Self);
$extern let_(fs_failing, fs_Self);

$attr($inline_always)
$static fn_((fs_isValid(fs_Self self))(bool));
$attr($inline_always)
$static fn_((fs_assertValid(P$raw ctx, P_const$$(fs_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((fs_ensureValid(fs_Self self))(fs_Self));

$attr($must_check)
$extern fn_((fs_cwd(fs_Self self))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_openDir(fs_Self self, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_createDir(fs_Self self, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_deleteDir(fs_Self self, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_openFile(fs_Self self, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_createFile(fs_Self self, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_deleteFile(fs_Self self, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_rename(fs_Self self, S_const$u8 old_path, S_const$u8 new_path))(E$void));
$attr($must_check)
$extern fn_((fs_realpath(fs_Self self, S_const$u8 path, S$u8 out_buf))(E$S$u8));

$attr($must_check)
$extern fn_((fs_direct(void))(fs_direct_E$fs_Self));
$extern fn_((fs_evented(exec_Coop* coop))(fs_Self));

T_alias$((fs_Self_VTbl_Op)(struct fs_Self_VTbl_Op {
    fn_(((*cwdFn)(P$raw ctx))(E$fs_Dir));
    fn_(((*openDirFn)(P$raw ctx, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir));
    fn_(((*createDirFn)(P$raw ctx, S_const$u8 path))(E$void));
    fn_(((*deleteDirFn)(P$raw ctx, S_const$u8 path))(E$void));
    fn_(((*openFileFn)(P$raw ctx, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File));
    fn_(((*createFileFn)(P$raw ctx, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File));
    fn_(((*deleteFileFn)(P$raw ctx, S_const$u8 path))(E$void));
    fn_(((*renameFn)(P$raw ctx, S_const$u8 old_path, S_const$u8 new_path))(E$void));
    fn_(((*realpathFn)(P$raw ctx, S_const$u8 path, S$u8 out_buf))(E$S$u8));
}));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingCWD(P$raw ctx))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingOpenDir(P$raw ctx, S_const$u8 path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingCreateDir(P$raw ctx, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingDeleteDir(P$raw ctx, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingOpenFile(P$raw ctx, S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingCreateFile(P$raw ctx, S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingDeleteFile(P$raw ctx, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingRename(P$raw ctx, S_const$u8 old_path, S_const$u8 new_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Op_failingRealpath(P$raw ctx, S_const$u8 path, S$u8 out_buf))(E$S$u8));
T_alias$((fs_Self_VTbl_Dir)(struct fs_Self_VTbl_Dir {
    fn_(((*closeFn)(P$raw ctx, fs_Dir dir))(void));
    fn_(((*accessFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_AccessOpts opts))(E$void));
    fn_(((*statFn)(P$raw ctx, fs_Dir dir))(fs_E$fs_Dir_Stat));
    fn_(((*statFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_StatFileOpts opts))(fs_E$fs_File_Stat));
    fn_(((*renameFn)(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
    fn_(((*renamePreserveFn)(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
    fn_(((*makePathFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
    fn_(((*makePathStatusFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$fs_Dir_CreatePathStatus));
    fn_(((*realpathFn)(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
    fn_(((*realpathAllocFn)(P$raw ctx, fs_Dir dir, S_const$u8 pathname, mem_Alctr gpa))(E$S$u8));
    fn_(((*realpathFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
    fn_(((*readLinkFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8));
    fn_(((*makeDirFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
    fn_(((*deleteDirFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
    fn_(((*openDirFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
    fn_(((*openPathFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
    fn_(((*hardLinkFn)(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, fs_Dir new_dir, S_const$u8 new_sub_path))(E$void));
    fn_(((*symLinkFn)(P$raw ctx, fs_Dir dir, S_const$u8 target_path, S_const$u8 sub_path))(E$void));
    fn_(((*createFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_CreateFileOpts flags))(E$fs_File));
    fn_(((*openFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenFileOpts flags))(E$fs_File));
    fn_(((*deleteFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
    fn_(((*readFileFn)(P$raw ctx, fs_Dir dir, S_const$u8 file_path, S$u8 buf))(E$S$u8));
    fn_(((*readFileAllocFn)(P$raw ctx, fs_Dir dir, S_const$u8 file_path, mem_Alctr gpa, usize max_bytes))(E$S$u8));
}));
$extern fn_((fs_VTbl_Dir_noClose(P$raw ctx, fs_Dir dir))(void));
$extern fn_((fs_VTbl_Dir_unreachableClose(P$raw ctx, fs_Dir dir))(void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingAccess(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_AccessOpts opts))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingStat(P$raw ctx, fs_Dir dir))(fs_E$fs_Dir_Stat));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingStatFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_StatFileOpts opts))(fs_E$fs_File_Stat));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingRename(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingRenamePreserve(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingMakePath(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingMakePathStatus(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$fs_Dir_CreatePathStatus));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingRealpath(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingRealpathAlloc(P$raw ctx, fs_Dir dir, S_const$u8 pathname, mem_Alctr gpa))(E$S$u8));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingRealpathFile(P$raw ctx, fs_Dir dir, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingReadLink(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingMakeDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingDeleteDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingOpenDir(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingOpenPath(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingHardLink(P$raw ctx, fs_Dir dir, S_const$u8 old_sub_path, fs_Dir new_dir, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingSymLink(P$raw ctx, fs_Dir dir, S_const$u8 target_path, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingCreateFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_CreateFileOpts flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingOpenFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path, fs_Dir_OpenFileOpts flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingDeleteFile(P$raw ctx, fs_Dir dir, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingReadFile(P$raw ctx, fs_Dir dir, S_const$u8 file_path, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_VTbl_Dir_failingReadFileAlloc(P$raw ctx, fs_Dir dir, S_const$u8 file_path, mem_Alctr gpa, usize max_bytes))(E$S$u8));
T_alias$((fs_Self_VTbl_File)(struct fs_Self_VTbl_File {
    fn_(((*closeFn)(P$raw ctx, fs_File file))(void));
    fn_(((*readFn)(P$raw ctx, fs_File file, S$u8 buf))(E$usize));
    fn_(((*writeFn)(P$raw ctx, fs_File file, S_const$u8 bytes))(E$usize));
    fn_(((*statFn)(P$raw ctx, fs_File file))(fs_E$fs_File_Stat));
    fn_(((*syncFn)(P$raw ctx, fs_File file))(E$void));
    fn_(((*isTtyFn)(P$raw ctx, fs_File file))(E$bool));
    fn_(((*setLenFn)(P$raw ctx, fs_File file, u64 new_len))(E$void));
    fn_(((*setPermsFn)(P$raw ctx, fs_File file, fs_File_Mode perms))(E$void));
    fn_(((*readPosFn)(P$raw ctx, fs_File file, S$u8 buf, u64 offset))(E$usize));
    fn_(((*writePosFn)(P$raw ctx, fs_File file, S_const$u8 buf, u64 offset))(E$usize));
    fn_(((*seekByFn)(P$raw ctx, fs_File file, i64 rel_offset))(E$void));
    fn_(((*seekToFn)(P$raw ctx, fs_File file, u64 abs_offset))(E$void));
    fn_(((*lockFn)(P$raw ctx, fs_File file, fs_Lock lock))(E$void));
    fn_(((*tryLockFn)(P$raw ctx, fs_File file, fs_Lock lock))(E$bool));
    fn_(((*unlockFn)(P$raw ctx, fs_File file))(void));
    fn_(((*downgradeLockFn)(P$raw ctx, fs_File file))(E$void));
    fn_(((*realpathFn)(P$raw ctx, fs_File file, S$u8 out_buf))(E$S$u8));
}));
$extern fn_((fs_VTbl_File_noClose(P$raw ctx, fs_File file))(void));
$extern fn_((fs_VTbl_File_unreachableClose(P$raw ctx, fs_File file))(void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingRead(P$raw ctx, fs_File file, S$u8 buf))(E$usize));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingWrite(P$raw ctx, fs_File file, S_const$u8 bytes))(E$usize));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingStat(P$raw ctx, fs_File file))(fs_E$fs_File_Stat));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingSync(P$raw ctx, fs_File file))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingIsTty(P$raw ctx, fs_File file))(E$bool));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingSetLen(P$raw ctx, fs_File file, u64 new_len))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingSetPerms(P$raw ctx, fs_File file, fs_File_Mode perms))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingReadPos(P$raw ctx, fs_File file, S$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingWritePos(P$raw ctx, fs_File file, S_const$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingSeekBy(P$raw ctx, fs_File file, i64 rel_offset))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingSeekTo(P$raw ctx, fs_File file, u64 abs_offset))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingLock(P$raw ctx, fs_File file, fs_Lock lock))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingTryLock(P$raw ctx, fs_File file, fs_Lock lock))(E$bool));
$extern fn_((fs_VTbl_File_noUnlock(P$raw ctx, fs_File file))(void));
$extern fn_((fs_VTbl_File_unreachableUnlock(P$raw ctx, fs_File file))(void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingDowngradeLock(P$raw ctx, fs_File file))(E$void));
$attr($must_check)
$extern fn_((fs_VTbl_File_failingRealpath(P$raw ctx, fs_File file, S$u8 out_buf))(E$S$u8));

struct fs_Self_VTbl {
    var_(op, fs_Self_VTbl_Op);
    var_(dir, fs_Self_VTbl_Dir);
    var_(file, fs_Self_VTbl_File);
};

/*========== Macros and Definitions =========================================*/

fn_((fs_isValid(fs_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->op.cwdFn)
        && isNonnull(self.vtbl->op.openDirFn)
        && isNonnull(self.vtbl->op.createDirFn)
        && isNonnull(self.vtbl->op.deleteDirFn)
        && isNonnull(self.vtbl->op.openFileFn)
        && isNonnull(self.vtbl->op.createFileFn)
        && isNonnull(self.vtbl->op.deleteFileFn)
        && isNonnull(self.vtbl->op.renameFn)
        && isNonnull(self.vtbl->op.realpathFn)
        && isNonnull(self.vtbl->dir.closeFn)
        && isNonnull(self.vtbl->dir.accessFn)
        && isNonnull(self.vtbl->dir.statFn)
        && isNonnull(self.vtbl->dir.statFileFn)
        && isNonnull(self.vtbl->dir.renameFn)
        && isNonnull(self.vtbl->dir.renamePreserveFn)
        && isNonnull(self.vtbl->dir.makePathFn)
        && isNonnull(self.vtbl->dir.makePathStatusFn)
        && isNonnull(self.vtbl->dir.realpathFn)
        && isNonnull(self.vtbl->dir.realpathAllocFn)
        && isNonnull(self.vtbl->dir.realpathFileFn)
        && isNonnull(self.vtbl->dir.readLinkFn)
        && isNonnull(self.vtbl->dir.makeDirFn)
        && isNonnull(self.vtbl->dir.deleteDirFn)
        && isNonnull(self.vtbl->dir.openDirFn)
        && isNonnull(self.vtbl->dir.openPathFn)
        && isNonnull(self.vtbl->dir.hardLinkFn)
        && isNonnull(self.vtbl->dir.symLinkFn)
        && isNonnull(self.vtbl->dir.createFileFn)
        && isNonnull(self.vtbl->dir.openFileFn)
        && isNonnull(self.vtbl->dir.deleteFileFn)
        && isNonnull(self.vtbl->dir.readFileFn)
        && isNonnull(self.vtbl->dir.readFileAllocFn)
        && isNonnull(self.vtbl->file.closeFn)
        && isNonnull(self.vtbl->file.readFn)
        && isNonnull(self.vtbl->file.writeFn)
        && isNonnull(self.vtbl->file.statFn)
        && isNonnull(self.vtbl->file.syncFn)
        && isNonnull(self.vtbl->file.isTtyFn)
        && isNonnull(self.vtbl->file.setLenFn)
        && isNonnull(self.vtbl->file.setPermsFn)
        && isNonnull(self.vtbl->file.readPosFn)
        && isNonnull(self.vtbl->file.writePosFn)
        && isNonnull(self.vtbl->file.seekByFn)
        && isNonnull(self.vtbl->file.seekToFn)
        && isNonnull(self.vtbl->file.lockFn)
        && isNonnull(self.vtbl->file.tryLockFn)
        && isNonnull(self.vtbl->file.unlockFn)
        && isNonnull(self.vtbl->file.downgradeLockFn)
        && isNonnull(self.vtbl->file.realpathFn);
};
fn_((fs_assertValid(P$raw ctx, P_const$$(fs_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->op.cwdFn);
    claim_assert_nonnull(vtbl->op.openDirFn);
    claim_assert_nonnull(vtbl->op.createDirFn);
    claim_assert_nonnull(vtbl->op.deleteDirFn);
    claim_assert_nonnull(vtbl->op.openFileFn);
    claim_assert_nonnull(vtbl->op.createFileFn);
    claim_assert_nonnull(vtbl->op.deleteFileFn);
    claim_assert_nonnull(vtbl->op.renameFn);
    claim_assert_nonnull(vtbl->op.realpathFn);
    claim_assert_nonnull(vtbl->dir.closeFn);
    claim_assert_nonnull(vtbl->dir.accessFn);
    claim_assert_nonnull(vtbl->dir.statFn);
    claim_assert_nonnull(vtbl->dir.statFileFn);
    claim_assert_nonnull(vtbl->dir.renameFn);
    claim_assert_nonnull(vtbl->dir.renamePreserveFn);
    claim_assert_nonnull(vtbl->dir.makePathFn);
    claim_assert_nonnull(vtbl->dir.makePathStatusFn);
    claim_assert_nonnull(vtbl->dir.realpathFn);
    claim_assert_nonnull(vtbl->dir.realpathAllocFn);
    claim_assert_nonnull(vtbl->dir.realpathFileFn);
    claim_assert_nonnull(vtbl->dir.readLinkFn);
    claim_assert_nonnull(vtbl->dir.makeDirFn);
    claim_assert_nonnull(vtbl->dir.deleteDirFn);
    claim_assert_nonnull(vtbl->dir.openDirFn);
    claim_assert_nonnull(vtbl->dir.openPathFn);
    claim_assert_nonnull(vtbl->dir.hardLinkFn);
    claim_assert_nonnull(vtbl->dir.symLinkFn);
    claim_assert_nonnull(vtbl->dir.createFileFn);
    claim_assert_nonnull(vtbl->dir.openFileFn);
    claim_assert_nonnull(vtbl->dir.deleteFileFn);
    claim_assert_nonnull(vtbl->dir.readFileFn);
    claim_assert_nonnull(vtbl->dir.readFileAllocFn);
    claim_assert_nonnull(vtbl->file.closeFn);
    claim_assert_nonnull(vtbl->file.readFn);
    claim_assert_nonnull(vtbl->file.writeFn);
    claim_assert_nonnull(vtbl->file.statFn);
    claim_assert_nonnull(vtbl->file.syncFn);
    claim_assert_nonnull(vtbl->file.isTtyFn);
    claim_assert_nonnull(vtbl->file.setLenFn);
    claim_assert_nonnull(vtbl->file.setPermsFn);
    claim_assert_nonnull(vtbl->file.readPosFn);
    claim_assert_nonnull(vtbl->file.writePosFn);
    claim_assert_nonnull(vtbl->file.seekByFn);
    claim_assert_nonnull(vtbl->file.seekToFn);
    claim_assert_nonnull(vtbl->file.lockFn);
    claim_assert_nonnull(vtbl->file.tryLockFn);
    claim_assert_nonnull(vtbl->file.unlockFn);
    claim_assert_nonnull(vtbl->file.downgradeLockFn);
    claim_assert_nonnull(vtbl->file.realpathFn);
};
fn_((fs_ensureValid(fs_Self self))(fs_Self)) {
    return fs_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_Self__included */
