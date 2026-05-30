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

#include "Self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((fs_Dir_Mode)(fs_Mode));
$static let_(fs_Dir_Mode_default, fs_Dir_Mode) = 0755;

T_alias$((fs_Dir_OpenOpts)(struct fs_Dir_OpenOpts {
    var_(iterate, bool);
}));
$static let_(fs_Dir_OpenOpts_default, fs_Dir_OpenOpts) = {
    .iterate = false,
};

T_alias$((fs_Dir_OpenFileOpts)(fs_OpenFlags));
$static let_(fs_Dir_OpenFileOpts_default, fs_Dir_OpenFileOpts) = fs_OpenFlags_default;
$static let fs_Dir_OpenFileOpts_isRead = fs_OpenFlags_isRead;
$static let fs_Dir_OpenFileOpts_isWrite = fs_OpenFlags_isWrite;

T_alias$((fs_Dir_CreateFileOpts)(fs_CreateFlags));
$static let_(fs_Dir_CreateFileOpts_default, fs_Dir_CreateFileOpts) = {
    .read = false,
    .truncate = true,
    .exclusive = false,
    .lock = fs_Lock_none,
    .lock_nonblocking = false,
    .nonblocking = false,
    .mode = fs_Dir_Mode_default,
};

T_alias$((fs_Dir_AccessOpts)(struct fs_Dir_AccessOpts {
    var_(read, bool);
    var_(write, bool);
    var_(execute, bool);
}));
$static let_(fs_Dir_AccessOpts_default, fs_Dir_AccessOpts) = {
    .read = true,
    .write = false,
    .execute = false,
};

T_alias$((fs_Dir_Stat)(fs_Stat));
T_alias$((fs_E$fs_Dir_Stat)(fs_E$fs_Stat));

T_alias$((fs_Dir_StatFileOpts)(struct fs_Dir_StatFileOpts {
    var_(follow_symlink, bool);
}));
$static let_(fs_Dir_StatFileOpts_default, fs_Dir_StatFileOpts) = {
    .follow_symlink = true,
};

T_alias$((fs_Dir_Entry)(struct fs_Dir_Entry {
    var_(name, S_const$u8);
    var_(kind, fs_Kind);
    var_(inode, fs_INode);
}));
T_use_S$(fs_Dir_Entry);
T_use_O$(fs_Dir_Entry);
T_use_E$(O$fs_Dir_Entry);

T_alias$((fs_Dir_Handle)(fs_Handle));
T_alias$((fs_Dir)(struct fs_Dir {
    var_(handle, fs_Dir_Handle);
}));
T_use_prl$(fs_Dir);
$attr($inline_always)
$static fn_((fs_Dir_Handle_promote(fs_Dir_Handle handle))(fs_Dir));
$attr($inline_always)
$static fn_((fs_Dir_handle(fs_Dir self))(fs_Dir_Handle));

$extern fn_((fs_Dir_close(fs_Dir* self, fs_Self fs))(void));
$attr($must_check)
$extern fn_((fs_Dir_access(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_AccessOpts opts))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_stat(fs_Dir self, fs_Self fs))(fs_E$fs_Dir_Stat));
$attr($must_check)
$extern fn_((fs_Dir_statFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_StatFileOpts opts))(fs_E$fs_File_Stat));
$attr($must_check)
$extern fn_((fs_Dir_rename(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_renamePreserve(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_makePath(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_makePathStatus(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$fs_Dir_CreatePathStatus));
$attr($must_check)
$extern fn_((fs_Dir_realpath(fs_Dir self, fs_Self fs, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_Dir_realpathAlloc(fs_Dir self, fs_Self fs, S_const$u8 pathname, mem_Alctr gpa))(E$S$u8));
$attr($must_check)
$extern fn_((fs_Dir_realpathFile(fs_Dir self, fs_Self fs, S_const$u8 pathname, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_Dir_readLink(fs_Dir self, fs_Self fs, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8));

$attr($must_check)
$extern fn_((fs_Dir_makeDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_deleteDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_openDir(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_Dir_openPath(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenOpts opts))(E$fs_Dir));
$attr($must_check)
$extern fn_((fs_Dir_hardLink(fs_Dir self, fs_Self fs, S_const$u8 old_sub_path, fs_Dir new_dir, S_const$u8 new_sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_symLink(fs_Dir self, fs_Self fs, S_const$u8 target_path, S_const$u8 sub_path))(E$void));

$extern fn_((fs_Dir_createFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_CreateFileOpts flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_Dir_openFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path, fs_Dir_OpenFileOpts flags))(E$fs_File));
$attr($must_check)
$extern fn_((fs_Dir_deleteFile(fs_Dir self, fs_Self fs, S_const$u8 sub_path))(E$void));
$attr($must_check)
$extern fn_((fs_Dir_readFile(fs_Dir self, fs_Self fs, S_const$u8 file_path, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_Dir_readFileAlloc(fs_Dir self, fs_Self fs, S_const$u8 file_path, mem_Alctr gpa, usize max_bytes))(E$S$u8));

#if UNUSED_CODE
T_alias$((fs_Dir_Reader_State)(enum_((fs_Dir_Reader_State $fits($packed))(
    fs_Dir_Reader_State_reset = 0,
    fs_Dir_Reader_State_reading = 1,
    fs_Dir_Reader_State_finished = 2,
))));

typedef struct fs_Dir_Reader {
    fs_Dir dir;
    fs_Dir_Reader_State state;
    S$u8 buf;
    usize index;
    usize end;
} fs_Dir_Reader;
typedef struct fs_Dir_Iterator {
    fs_Dir_Reader reader;
    A$$(2048, u8) reader_buf;
} fs_Dir_Iterator;
typedef struct fs_Dir_Walker_Entry {
    fs_Dir dir;
    S_const$u8 basename;
    S_const$u8 path;
    fs_Kind kind;
} fs_Dir_Walker_Entry;
T_use_O$(fs_Dir_Walker_Entry);
T_use_E$(O$fs_Dir_Walker_Entry);
typedef struct fs_Dir_SelectiveWalker {
    mem_Alctr gpa;
    var_(ctx, P$raw);
} fs_Dir_SelectiveWalker;
T_use_E$(fs_Dir_SelectiveWalker);
typedef struct fs_Dir_Walker {
    fs_Dir_SelectiveWalker inner;
} fs_Dir_Walker;
T_use_E$(fs_Dir_Walker);
$extern fn_((fs_Dir_Reader_init(fs_Dir dir, S$u8 buf))(fs_Dir_Reader));
$extern fn_((fs_Dir_Reader_reset(fs_Dir_Reader* self))(void));
$attr($must_check)
$extern fn_((fs_Dir_Reader_read(fs_Dir_Reader* self, S$fs_Dir_Entry out))(E$usize));
$extern fn_((fs_Dir_Iterator_init(fs_Dir dir, fs_Dir_Reader_State initial_state))(fs_Dir_Iterator));
$extern fn_((fs_Dir_iterate(fs_Dir dir))(fs_Dir_Iterator));
$extern fn_((fs_Dir_iterateAssumeFirstIteration(fs_Dir dir))(fs_Dir_Iterator));
$attr($must_check)
$extern fn_((fs_Dir_Iterator_next(fs_Dir_Iterator* self))(E$O$fs_Dir_Entry));
$attr($must_check)
$extern fn_((fs_Dir_walkSelectively(fs_Dir dir, mem_Alctr gpa))(E$fs_Dir_SelectiveWalker));
$extern fn_((fs_Dir_SelectiveWalker_fini(fs_Dir_SelectiveWalker* self))(void));
$attr($must_check)
$extern fn_((fs_Dir_SelectiveWalker_next(fs_Dir_SelectiveWalker* self))(E$O$fs_Dir_Walker_Entry));
$attr($must_check)
$extern fn_((fs_Dir_SelectiveWalker_enter(fs_Dir_SelectiveWalker* self, fs_Dir_Walker_Entry entry))(E$void));
$extern fn_((fs_Dir_SelectiveWalker_leave(fs_Dir_SelectiveWalker* self))(void));
$attr($must_check)
$extern fn_((fs_Dir_walk(fs_Dir dir, mem_Alctr gpa))(E$fs_Dir_Walker));
$extern fn_((fs_Dir_Walker_fini(fs_Dir_Walker* self))(void));
$attr($must_check)
$extern fn_((fs_Dir_Walker_next(fs_Dir_Walker* self))(E$O$fs_Dir_Walker_Entry));
$extern fn_((fs_Dir_Walker_leave(fs_Dir_Walker* self))(void));
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

fn_((fs_Dir_Handle_promote(fs_Dir_Handle handle))(fs_Dir)) {
    return l$((fs_Dir){ .handle = handle });
};
fn_((fs_Dir_handle(fs_Dir self))(fs_Dir_Handle)) {
    return self.handle;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_Dir__included */
