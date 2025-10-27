/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dir.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_dir
 *
 * @brief   Directory utilities
 * @details Provides utilities for directory operations:
 *          - Directory creation
 *          - Directory listing
 *          - Directory traversal
 *          - Directory deletion
 *          - Directory utilities
 */

#ifndef fs_Dir__included
#define fs_Dir__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/fs/common.h"
#include "dh/fs/File.h"
#include "dh/mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

typedef posix_fd_t fs_Dir_Handle;
typedef struct fs_Dir {
    fs_Dir_Handle handle;
} fs_Dir;
T_use_E$(fs_Dir);
static const fs_File_Mode fs_Dir_default_mode = 0755;

extern fn_((fs_Dir_create(S_const$u8 path))(E$void)) $must_check;

extern fn_((fs_Dir_close(fs_Dir* self))(void));
extern fn_((fs_Dir_rename(fs_Dir self, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void)) $must_check;
extern fn_((fs_Dir_makePath(fs_Dir self, S_const$u8 sub_path))(E$void)) $must_check;
extern fn_((fs_Dir_realpath(fs_Dir self, S_const$u8 pathname, S$u8 out_buffer))(E$S$u8)) $must_check;
extern fn_((fs_Dir_realpathAlloc(fs_Dir self, S_const$u8 pathname, mem_Allocator allocator))(E$S$u8)) $must_check;

extern fn_((fs_Dir_makeDir(fs_Dir self, S_const$u8 sub_path))(E$void)) $must_check;
extern fn_((fs_Dir_deleteDir(fs_Dir self, S_const$u8 sub_path))(E$void)) $must_check;
extern fn_((fs_Dir_openDir(fs_Dir self, S_const$u8 sub_path, fs_File_OpenFlags flags))(E$fs_Dir)) $must_check;

extern fn_((fs_Dir_createFile(fs_Dir self, S_const$u8 sub_path, fs_File_CreateFlags flags))(E$fs_File));
extern fn_((fs_Dir_openFile(fs_Dir self, S_const$u8 sub_path, fs_File_OpenFlags flags))(E$fs_File)) $must_check;
extern fn_((fs_Dir_deleteFile(fs_Dir self, S_const$u8 sub_path))(E$void)) $must_check;
extern fn_((fs_Dir_readFile(fs_Dir self, S_const$u8 file_path, S$u8 buffer))(E$S$u8)) $must_check;
extern fn_((fs_Dir_readFileAlloc(fs_Dir self, S_const$u8 file_path, mem_Allocator allocator, usize max_bytes))(E$S$u8)) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_Dir__included */
