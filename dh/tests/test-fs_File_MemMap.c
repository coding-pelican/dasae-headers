#include "dh-main.h"
#include "dh/fs.h"
#include "dh/mem/common.h"

TEST_fn_("fs/File/MemMap: write, flush, shrink, and reopen" $scope) {
    let path = u8_l("build/dev/test-fs_File-MemMap-direct.tmp");
    let_ignore = catch_((fs_File_delete(path))($ignore, $do_nothing));

    var create_flags = fs_File_CreateFlags_default;
    create_flags.read = true;
    var file = try_(fs_File_create(path, create_flags));

    var protection = fs_File_MemMap_Protection_default;
    var map = try_(fs_File_MemMap_init(file, 6, protection, false, false, 0));
    mem_copyBytes(map.mem, u8_l("abcdef"));
    try_(fs_File_MemMap_write(&map));
    try_(TEST_expect(mem_eqlBytes(map.mem.as_const, u8_l("abcdef"))));

    try_(fs_File_MemMap_setLen(&map, 3));
    try_(TEST_expect(map.mem.len == 3));
    try_(TEST_expect(mem_eqlBytes(map.mem.as_const, u8_l("abc"))));

    fs_File_MemMap_fini(&map);
    fs_File_close(file);

    var open_flags = fs_File_OpenFlags_default;
    open_flags.mode = fs_OpenMode_read_only;
    file = try_(fs_File_open(path, open_flags));

    var_(buf, A$$(3, u8)) = A_zero();
    try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(buf)), 0)) == 3));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("abc"))));

    fs_File_close(file);
    try_(fs_File_delete(path));
} $unscoped(TEST_fn);
