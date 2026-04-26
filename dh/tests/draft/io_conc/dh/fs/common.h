/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2025-02-23 (date of last update)
 * @version v0.1-alpha
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
#include "../time/self/Real.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_E)(
    NotFound,
    AccessDenied,
    PermissionDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed,
    WouldBlock,
    IsDir,
    NotDir,
    PathAlreadyExists,
    ReadOnlyFileSystem,
    NoSpaceLeft,
    FileTooBig,
    SymLinkLoop,
    Busy,
    Unsupported
));

T_alias$((fs_Dir_Mode)(fs_Mode));
T_alias$((fs_File_Mode)(fs_Mode));

T_alias$((fs_OpenMode)(enum_((fs_OpenMode $fits($packed))(
    fs_OpenMode_read_only = 0,
    fs_OpenMode_write_only = 1,
    fs_OpenMode_read_write = 2,
))));
T_alias$((fs_Lock)(enum_((fs_Lock $fits($packed))(
    fs_Lock_none = 0,
    fs_Lock_shared = 1,
    fs_Lock_exclusive = 2,
))));
T_alias$((fs_OpenFlags)(struct fs_OpenFlags {
    var_(mode, fs_OpenMode);
    var_(lock, fs_Lock);
    var_(lock_nonblocking, bool);
    var_(nonblocking, bool);
    var_(allow_ctty, bool);
}));
$static let_(fs_OpenFlags_default, fs_OpenFlags) = {
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

T_alias$((fs_CreateFlags)(struct fs_CreateFlags {
    var_(read, bool);
    var_(truncate, bool);
    var_(exclusive, bool);
    var_(lock, fs_Lock);
    var_(lock_nonblocking, bool);
    var_(nonblocking, bool);
    var_(mode, fs_Mode);
}));

T_alias$((fs_Kind)(enum_((fs_Kind $fits($packed))(
    fs_Kind_block_device = 0,
    fs_Kind_character_device = 1,
    fs_Kind_directory = 2,
    fs_Kind_named_pipe = 3,
    fs_Kind_sym_link = 4,
    fs_Kind_file = 5,
    fs_Kind_unix_domain_socket = 6,
    fs_Kind_unknown = 7,
))));
T_alias$((fs_Stat)(struct fs_Stat {
    var_(inode, fs_INode);
    var_(nlink, fs_NLink);
    var_(size, u64);
    var_(permissions, fs_Mode);
    var_(kind, fs_Kind);
    var_(atime, time_Real_Inst);
    var_(mtime, time_Real_Inst);
    var_(ctime, time_Real_Inst);
    var_(block_size, fs_BlockSize);
}));
T_use_E$($set(fs_E)(fs_Stat));
T_alias$((fs_Dir_Stat)(fs_Stat));
T_alias$((fs_E$fs_Dir_Stat)(fs_E$fs_Stat));
T_alias$((fs_File_Stat)(fs_Stat));
T_alias$((fs_E$fs_File_Stat)(fs_E$fs_Stat));

T_alias$((fs_Dir_OpenOpts)(struct fs_Dir_OpenOpts));
T_alias$((fs_Dir_OpenFileOpts)(fs_OpenFlags));
T_alias$((fs_Dir_AccessOpts)(struct fs_Dir_AccessOpts));
T_alias$((fs_Dir_StatFileOpts)(struct fs_Dir_StatFileOpts));
T_alias$((fs_Dir_CreateFileOpts)(fs_CreateFlags));
T_alias$((fs_Dir_CreatePathStatus)(enum_((fs_Dir_CreatePathStatus $fits($packed))(
    fs_Dir_CreatePathStatus_existing = 0,
    fs_Dir_CreatePathStatus_created = 1,
))));
T_use_E$(fs_Dir_CreatePathStatus);

T_alias$((fs_File_OpenFlags)(fs_OpenFlags));
T_alias$((fs_File_CreateFlags)(fs_CreateFlags));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_common__included */
