#include "dh-main.h"
#include "dh/fs/Dir.h"
#include "dh/mem/common.h"

TEST_fn_("fs/Dir: opens, renames, and deletes a child directory" $guard) {
    pp_if_(pp_not(pp_or(plat_is_windows, plat_is_linux)))(
        try_(TEST_skipMsg(u8_l("directory operations are unavailable")))
    );

    var root = try_(fs_Dir_openDir(fs_Dir_cwd, u8_l("."), fs_File_OpenFlags_default));
    defer_(fs_Dir_close(&root));

    let old_name = u8_l("test-fs_Dir-old.tmp");
    let new_name = u8_l("test-fs_Dir-new.tmp");
    let file_name = u8_l("test-fs_Dir-child-file.tmp");
    let_ignore = catch_((fs_Dir_deleteDir(root, old_name))($ignore, $do_nothing));
    let_ignore = catch_((fs_Dir_deleteDir(root, new_name))($ignore, $do_nothing));
    let_ignore = catch_((fs_Dir_deleteFile(fs_Dir_cwd, file_name))($ignore, $do_nothing));
    defer_(let_ignore = catch_((fs_Dir_deleteDir(root, old_name))($ignore, $do_nothing)));
    defer_(let_ignore = catch_((fs_Dir_deleteDir(root, new_name))($ignore, $do_nothing)));
    defer_(let_ignore = catch_((fs_Dir_deleteFile(fs_Dir_cwd, file_name))($ignore, $do_nothing)));

    try_(fs_Dir_makeDir(root, old_name));
    try_(fs_Dir_rename(root, old_name, new_name));
    {
        var child = try_(fs_Dir_openDir(root, new_name, fs_File_OpenFlags_default));
        defer_(fs_Dir_close(&child));
        defer_(let_ignore = catch_((fs_Dir_deleteFile(child, file_name))($ignore, $do_nothing)));

        {
            let file = try_(fs_Dir_createFile(child, file_name, fs_File_CreateFlags_default));
            defer_(fs_File_close(file));
            let payload = u8_l("child-directory-relative");
            try_(TEST_expect(try_(fs_File_writePos(file, payload, 0)) == payload.len));
        }

        var_(buffer, A$$(32, u8)) = A_zero();
        let read = try_(fs_Dir_readFile(child, file_name, A_ref$((S$u8)(buffer))));
        try_(TEST_expect(mem_eqlBytes(read.as_const, u8_l("child-directory-relative"))));
        try_(fs_Dir_deleteFile(child, file_name));
    }
    try_(fs_Dir_deleteDir(root, new_name));
} $unguarded(TEST_fn);
