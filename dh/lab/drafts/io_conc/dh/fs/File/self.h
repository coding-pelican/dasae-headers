/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    File.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_File
 */
#ifndef fs_File_self__included
#define fs_File_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Self.h"
#include "../../io/Reader.h"
#include "../../io/Writer.h"

/*========== Macros and Declarations ========================================*/

T_alias$((fs_File_Mode)(fs_Mode));
/// This is the default mode given to POSIX operating systems for creating
/// files. `0o666` is "-rw-rw-rw-" which is counter-intuitive at first,
/// since most people would expect "-rw-r--r--", for example, when using
/// the `touch` command, which would correspond to `0o644`. However, POSIX
/// libc implementations use `0666` inside `fopen` and then rely on the
/// process-scoped "umask" setting to adjust this number for file creation.
$static let_(fs_File_Mode_default, fs_File_Mode) = pp_if_(plat_is_posix)(pp_then_(0666), pp_else_(0));

T_alias$((fs_File_OpenFlags)(fs_OpenFlags));
$static let_(fs_File_OpenFlags_default, fs_File_OpenFlags) = fs_OpenFlags_default;
$static let fs_File_OpenFlags_isRead = fs_OpenFlags_isRead;
$static let fs_File_OpenFlags_isWrite = fs_OpenFlags_isWrite;

T_alias$((fs_File_CreateFlags)(fs_CreateFlags));
$static let_(fs_File_CreateFlags_default, fs_File_CreateFlags) = {
    .read = false,
    .truncate = true,
    .exclusive = false,
    .lock = fs_Lock_none,
    .lock_nonblocking = false,
    .nonblocking = false,
    .mode = fs_File_Mode_default,
};

T_alias$((fs_File_Stat)(fs_Stat));
T_alias$((fs_E$fs_File_Stat)(fs_E$fs_Stat));

T_alias$((fs_File_Handle)(fs_Handle));
T_alias$((fs_File_Flags)(struct fs_File_Flags {
    var_(nonblocking, bool);
}));
$static let_(fs_File_Flags_default, fs_File_Flags) = {
    .nonblocking = false,
};
T_alias$((fs_File)(struct fs_File {
    var_(handle, fs_File_Handle);
    var_(flags, fs_File_Flags);
}));
T_use_prl$(fs_File);
$attr($inline_always)
$static fn_((fs_File_Handle_promote(fs_File_Handle handle, fs_File_Flags flags))(fs_File));
$attr($inline_always)
$static fn_((fs_File_handle(fs_File self))(fs_File_Handle));
$attr($inline_always)
$static fn_((fs_File_flags(fs_File self))(fs_File_Flags));

$extern fn_((fs_File_close(fs_File self, fs_Self fs))(void));
$attr($must_check)
$extern fn_((fs_File_stat(fs_File self, fs_Self fs))(fs_E$fs_File_Stat));
$attr($must_check)
$extern fn_((fs_File_sync(fs_File self, fs_Self fs))(E$void));
$attr($must_check)
$extern fn_((fs_File_isTty(fs_File self, fs_Self fs))(E$bool));
$attr($must_check)
$extern fn_((fs_File_setLen(fs_File self, fs_Self fs, u64 new_len))(E$void));
$attr($must_check)
$extern fn_((fs_File_setPerms(fs_File self, fs_Self fs, fs_File_Mode perms))(E$void));

$attr($must_check)
$extern fn_((fs_File_readPos(fs_File self, fs_Self fs, S$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_File_writePos(fs_File self, fs_Self fs, S_const$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_File_seekBy(fs_File self, fs_Self fs, i64 rel_offset))(E$void));
$attr($must_check)
$extern fn_((fs_File_seekTo(fs_File self, fs_Self fs, u64 abs_offset))(E$void));

$attr($must_check)
$extern fn_((fs_File_lock(fs_File self, fs_Self fs, fs_Lock lock))(E$void));
$attr($must_check)
$extern fn_((fs_File_tryLock(fs_File self, fs_Self fs, fs_Lock lock))(E$bool));
$extern fn_((fs_File_unlock(fs_File self, fs_Self fs))(void));
$attr($must_check)
$extern fn_((fs_File_downgradeLock(fs_File self, fs_Self fs))(E$void));

$attr($must_check)
$extern fn_((fs_File_realpath(fs_File self, fs_Self fs, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_File_realpathAlloc(fs_File self, fs_Self fs, mem_Alctr gpa))(E$S$u8));

T_alias$((fs_File_IO)(struct fs_File_IO {
    var_(fs, fs_Self);
    var_(file, fs_File);
}));
$extern fn_((fs_File_io(fs_File file, fs_Self fs))(fs_File_IO));
$extern fn_((fs_File_IO_reader(fs_File_IO* self))(io_Reader));
$extern fn_((fs_File_IO_writer(fs_File_IO* self))(io_Writer));

/*========== Macros and Definitions =========================================*/

fn_((fs_File_Handle_promote(fs_File_Handle handle, fs_File_Flags flags))(fs_File)) {
    return l$((fs_File){
        .handle = handle,
        .flags = flags,
    });
};
fn_((fs_File_handle(fs_File self))(fs_File_Handle)) {
    return self.handle;
};
fn_((fs_File_flags(fs_File self))(fs_File_Flags)) {
    return self.flags;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_File_self__included */
