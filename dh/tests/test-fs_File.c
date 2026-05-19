#include "dh-main.h"
#include "dh/fs/File.h"
#include "dh/mem/common.h"

TEST_fn_("fs/File: create, positioned IO, stat, rename, delete" $scope) {
    let path = u8_l("build/dev/test-fs_File-direct.tmp");
    let renamed_path = u8_l("build/dev/test-fs_File-direct-renamed.tmp");

    let_ignore = catch_((fs_File_delete(path))($ignore, $do_nothing));
    let_ignore = catch_((fs_File_delete(renamed_path))($ignore, $do_nothing));

    var create_flags = fs_File_CreateFlags_default;
    create_flags.read = true;
    var file = try_(fs_File_create(path, create_flags));

    try_(TEST_expect(try_(fs_File_writePos(file, u8_l("abcdef"), 0)) == 6));
    try_(fs_File_sync(file));

    let stat = try_(fs_File_stat(file));
    try_(TEST_expect(stat.size == 6));
    try_(TEST_expect(stat.kind == fs_Kind_file));

    var_(buf, A$$(3, u8)) = A_zero();
    try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(buf)), 2)) == 3));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("cde"))));

    try_(fs_File_setLen(file, 3));
    fs_File_close(file);

    try_(fs_File_rename(path, renamed_path));

    var open_flags = fs_File_OpenFlags_default;
    open_flags.mode = fs_OpenMode_read_only;
    file = try_(fs_File_open(renamed_path, open_flags));

    var_(truncated, A$$(3, u8)) = A_zero();
    try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(truncated)), 0)) == 3));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(truncated)), u8_l("abc"))));
    fs_File_close(file);

    try_(fs_File_delete(renamed_path));
} $unscoped(TEST_fn);
