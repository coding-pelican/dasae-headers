/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2026-05-17 (date of last update)
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_File
 */
#pragma once
#ifndef fs_File_self__included
#define fs_File_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../common.h"
#include "../../mem/Alctr.h"
#include "../../io/Reader.h"
#include "../../io/Writer.h"

/*========== Macros and Declarations ========================================*/

/// This is the default mode given to POSIX operating systems for creating
/// files. `0o666` is "-rw-rw-rw-" which is counter-intuitive at first,
/// since most people would expect "-rw-r--r--", for example, when using
/// the `touch` command, which would correspond to `0o644`. However, POSIX
/// libc implementations use `0666` inside `fopen` and then rely on the
/// process-scoped "umask" setting to adjust this number for file creation.
$static const fs_File_Mode fs_File_Mode_default = pp_if_(plat_is_posix)(
    pp_then_(0666),
    pp_else_({}));

static const fs_File_OpenFlags fs_File_OpenFlags_default = {
    .mode = fs_OpenMode_read_only,
    .lock = fs_Lock_none,
    .lock_nonblocking = false,
    .nonblocking = false,
    .allow_ctty = false,
};
$attr($inline_always)
$static fn_((fs_File_OpenFlags_isRead(fs_File_OpenFlags self))(bool)) { return fs_OpenFlags_isRead(self); };
$attr($inline_always)
$static fn_((fs_File_OpenFlags_isWrite(fs_File_OpenFlags self))(bool)) { return fs_OpenFlags_isWrite(self); };

static const fs_File_CreateFlags fs_File_CreateFlags_default = {
    .read = false,
    .truncate = true,
    .exclusive = false,
    .lock = fs_Lock_none,
    .lock_nonblocking = false,
    .nonblocking = false,
    .mode = pp_if_(plat_is_posix)(
        pp_then_(0666),
        pp_else_({})),
};

typedef fs_Handle fs_File_Handle;
typedef struct fs_File_Flags {
    bool nonblocking;
} fs_File_Flags;
static const fs_File_Flags fs_File_Flags_default = {
    .nonblocking = false,
};

struct fs_File {
    fs_File_Handle handle;
    fs_File_Flags flags;
};
T_impl_prl$(fs_File);
$attr($inline_always)
$static fn_((fs_File_handle(fs_File self))(fs_File_Handle)) { return self.handle; };
$attr($inline_always)
$static fn_((fs_File_flags(fs_File self))(fs_File_Flags)) { return self.flags; };

$attr($must_check)
$extern fn_((fs_File_open(S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_File_create(S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_File_delete(S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((fs_File_rename(S_const$u8 old_path, S_const$u8 new_path))(E$void));

$extern fn_((fs_File_close(fs_File self))(void));

$attr($must_check)
$extern fn_((fs_File_stat(fs_File self))(fs_E$fs_File_Stat));
$attr($must_check)
$extern fn_((fs_File_sync(fs_File self))(E$void));
$attr($must_check)
$extern fn_((fs_File_isTTY(fs_File self))(E$bool));
$attr($must_check)
$extern fn_((fs_File_setLen(fs_File self, u64 new_len))(E$void));
$attr($must_check)
$extern fn_((fs_File_setPerms(fs_File self, fs_File_Mode perms))(E$void));

$attr($must_check)
$extern fn_((fs_File_readPos(fs_File self, S$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_File_writePos(fs_File self, S_const$u8 buf, u64 offset))(E$usize));
$attr($must_check)
$extern fn_((fs_File_seekBy(fs_File self, i64 rel_offset))(E$void));
$attr($must_check)
$extern fn_((fs_File_seekTo(fs_File self, u64 abs_offset))(E$void));

$attr($must_check)
$extern fn_((fs_File_lock(fs_File self, fs_Lock lock))(E$void));
$attr($must_check)
$extern fn_((fs_File_tryLock(fs_File self, fs_Lock lock))(E$bool));
$extern fn_((fs_File_unlock(fs_File self))(void));
$attr($must_check)
$extern fn_((fs_File_downgradeLock(fs_File self))(E$void));

$attr($must_check)
$extern fn_((fs_File_realpath(fs_File self, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_File_realpathAlloc(fs_File self, mem_Alctr gpa))(E$S$u8));

typedef struct fs_File_IO {
    fs_File file;
} fs_File_IO;
$extern fn_((fs_File_io(fs_File file))(fs_File_IO));
$extern fn_((fs_File_IO_reader(fs_File_IO* self))(io_Reader));
$extern fn_((fs_File_IO_writer(fs_File_IO* self))(io_Writer));

$extern fn_((fs_File_reader(fs_File self))(io_Reader));
$extern fn_((fs_File_writer(fs_File self))(io_Writer));

$attr($inline_always)
$static fn_((fs_File_Handle_promote(fs_File_Handle handle, fs_File_Flags flags))(fs_File)) {
    return l$((fs_File){
        .handle = handle,
        .flags = flags,
    });
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_File_self__included */
