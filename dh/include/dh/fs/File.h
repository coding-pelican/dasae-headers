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
#ifndef fs_File__included
#define fs_File__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/fs/common.h"
#include "dh/io/Reader.h"
#include "dh/io/Writer.h"

/*========== Macros and Declarations ========================================*/

typedef posix_fd_t fs_File_Handle;
typedef posix_mode_t fs_File_Mode;

/// This is the default mode given to POSIX operating systems for creating
/// files. `0o666` is "-rw-rw-rw-" which is counter-intuitive at first,
/// since most people would expect "-rw-r--r--", for example, when using
/// the `touch` command, which would correspond to `0o644`. However, POSIX
/// libc implementations use `0666` inside `fopen` and then rely on the
/// process-scoped "umask" setting to adjust this number for file creation.
static const fs_File_Mode fs_file_default_mode = pp_if_(plat_is_posix)(
    pp_then_(0666),
    pp_else_(0)
);

typedef enum fs_File_OpenMode {
    fs_File_OpenMode_read_only = 0,
    fs_File_OpenMode_write_only,
    fs_File_OpenMode_read_write,
} fs_File_OpenMode;

typedef enum fs_File_Lock {
    fs_File_Lock_none = 0,
    fs_File_Lock_shared,
    fs_File_Lock_exclusive,
} fs_File_Lock;

typedef struct fs_File_OpenFlags {
    fs_File_OpenMode mode;
    fs_File_Lock lock;
    bool lock_nonblocking;
    bool allow_ctty;
} fs_File_OpenFlags;
static const fs_File_OpenFlags fs_File_OpenFlags_default = {
    .mode = fs_File_OpenMode_read_only,
    .lock = fs_File_Lock_none,
    .lock_nonblocking = false,
    .allow_ctty = false,
};
$static $inline_always
fn_((fs_File_OpenFlags_isRead(fs_File_OpenFlags self))(bool)) { return self.mode != fs_File_OpenMode_write_only; }
$static $inline_always
fn_((fs_File_OpenFlags_isWrite(fs_File_OpenFlags self))(bool)) { return self.mode != fs_File_OpenMode_read_only; }

typedef struct fs_File_CreateFlags {
    bool read;
    bool truncate;
    bool exclusive;
    bool lock;
    bool lock_nonblocking;
    fs_File_Mode mode;
} fs_File_CreateFlags;
static const fs_File_CreateFlags fs_File_CreateFlags_default = {
    .read = false,
    .truncate = true,
    .exclusive = false,
    .lock = fs_File_Lock_none,
    .lock_nonblocking = false,
    .mode = fs_file_default_mode,
};

typedef struct fs_File {
    fs_File_Handle handle;
} fs_File;
T_use_E$(fs_File);
extern fn_((fs_File_close(fs_File file))(void));
extern fn_((fs_File_reader(fs_File file))(io_Reader));
extern fn_((fs_File_writer(fs_File file))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* FS_DIR_INCLUDED */
