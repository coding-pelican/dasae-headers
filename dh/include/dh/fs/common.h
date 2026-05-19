/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2026-05-17 (date of last update)
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs
 *
 * @brief   Common file system utilities
 * @details Provides common file system utilities and functions for:
 *          - File operations
 *          - Directory operations
 *          - Path operations
 *          - File system operations
 *          - File system utilities
 */
#ifndef fs_common__included
#define fs_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../time/SysTime.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_File_E)(
    FSNotFound,
    FSAccessDenied,
    FSOpenFailed,
    FSReadFailed,
    FSWriteFailed
));

errset_((fs_E)(
    NotFoundFS,
    AccessDeniedFS,
    PermissionDeniedFS,
    OpenFailedFS,
    ReadFailedFS,
    WriteFailedFS,
    WouldBlockFS,
    IsDirFS,
    NotDirFS,
    PathAlreadyExistsFS,
    ReadOnlyFileSystemFS,
    NoSpaceLeftFS,
    FileTooBigFS,
    SymLinkLoopFS,
    BusyFS,
    UnsupportedFS
));

typedef fs_Mode fs_Dir_Mode;
typedef fs_Mode fs_File_Mode;

typedef enum_((fs_OpenMode $fits($packed))(
    fs_OpenMode_read_only = 0,
    fs_OpenMode_write_only = 1,
    fs_OpenMode_read_write = 2,
)) fs_OpenMode;

typedef enum_((fs_Lock $fits($packed))(
    fs_Lock_none = 0,
    fs_Lock_shared = 1,
    fs_Lock_exclusive = 2,
)) fs_Lock;

typedef struct fs_OpenFlags {
    fs_OpenMode mode;
    fs_Lock lock;
    bool lock_nonblocking;
    bool nonblocking;
    bool allow_ctty;
} fs_OpenFlags;

static const fs_OpenFlags fs_OpenFlags_default = {
    .mode = fs_OpenMode_read_only,
    .lock = fs_Lock_none,
    .lock_nonblocking = false,
    .nonblocking = false,
    .allow_ctty = false,
};

$attr($inline_always)
$static fn_((fs_OpenFlags_isRead(fs_OpenFlags self))(bool)) { return self.mode != fs_OpenMode_write_only; };
$attr($inline_always)
$static fn_((fs_OpenFlags_isWrite(fs_OpenFlags self))(bool)) { return self.mode != fs_OpenMode_read_only; };

typedef struct fs_CreateFlags {
    bool read;
    bool truncate;
    bool exclusive;
    fs_Lock lock;
    bool lock_nonblocking;
    bool nonblocking;
    fs_Mode mode;
} fs_CreateFlags;

typedef enum_((fs_Kind $fits($packed))(
    fs_Kind_block_device = 0,
    fs_Kind_character_device = 1,
    fs_Kind_directory = 2,
    fs_Kind_named_pipe = 3,
    fs_Kind_sym_link = 4,
    fs_Kind_file = 5,
    fs_Kind_unix_domain_socket = 6,
    fs_Kind_unknown = 7,
)) fs_Kind;

typedef struct fs_Stat {
    fs_INode inode;
    fs_NLink nlink;
    u64 size;
    fs_Mode permissions;
    fs_Kind kind;
    time_SysTime atime;
    time_SysTime mtime;
    time_SysTime ctime;
    fs_BlockSize block_size;
} fs_Stat;
T_use_E$($set(fs_E)(fs_Stat));

typedef fs_Stat fs_Dir_Stat;
typedef fs_E$fs_Stat fs_E$fs_Dir_Stat;
typedef fs_Stat fs_File_Stat;
typedef fs_E$fs_Stat fs_E$fs_File_Stat;

typedef struct fs_Dir_OpenOpts fs_Dir_OpenOpts;
typedef fs_OpenFlags fs_Dir_OpenFileOpts;
typedef struct fs_Dir_AccessOpts fs_Dir_AccessOpts;
typedef struct fs_Dir_StatFileOpts fs_Dir_StatFileOpts;
typedef fs_CreateFlags fs_Dir_CreateFileOpts;
typedef enum_((fs_Dir_CreatePathStatus $fits($packed))(
    fs_Dir_CreatePathStatus_existing = 0,
    fs_Dir_CreatePathStatus_created = 1,
)) fs_Dir_CreatePathStatus;
T_use_E$(fs_Dir_CreatePathStatus);

typedef fs_OpenFlags fs_File_OpenFlags;
typedef fs_CreateFlags fs_File_CreateFlags;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_common__included */
