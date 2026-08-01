#include "dh-main.h"
#include "dh/fs.h"
#include "dh/fs/path.h"
#include "dh/mem/common.h"

$static fn_((test__pathInThisDir(S_const$u8 name, S$u8 out))(E$S$u8)) {
    return fs_path_join2(fs_path_dirname(mem_spanZ0Bytes(srcLoc().file_path)), name, out);
}

TEST_fn_("fs/File/MemMap: write, flush, shrink, and reopen" $guard) {
    var_(path_mem, A$$(1024, u8)) $undefined;
    let path = try_(test__pathInThisDir(u8_l("test-fs_File-MemMap-direct.tmp"), A_ref$((S$u8)(path_mem))));

    let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing));
    defer_(let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing)));

    using_(
        let create_flags = with_((fs_File_CreateFlags_default)(
            (.read)(true)
        ))
    ) blk_defer {
        var file = try_(fs_File_create(path.as_const, create_flags));
        defer_(fs_File_close(file));

        var protection = fs_File_MemMap_Protcn_default;
        var map = try_(fs_File_MemMap_init(file, 6, protection, false, false, 0));
        defer_(fs_File_MemMap_fini(&map));
        mem_copyBytes(map.mem, u8_l("abcdef"));
        try_(fs_File_MemMap_write(&map));
        try_(TEST_expect(mem_eqlBytes(map.mem.as_const, u8_l("abcdef"))));

        try_(fs_File_MemMap_setLen(&map, 3));
        try_(TEST_expect(map.mem.len == 3));
        try_(TEST_expect(mem_eqlBytes(map.mem.as_const, u8_l("abc"))));
    } blk_deferral;

    using_(
        let open_flags = with_((fs_File_OpenFlags_default)(
            (.mode)(fs_OpenMode_read_only)
        ))
    ) blk_defer {
        var file = try_(fs_File_open(path.as_const, open_flags));
        defer_(fs_File_close(file));

        var_(buf, A$$(3, u8)) $undefined;
        try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(buf)), 0)) == 3));
        try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("abc"))));
    } blk_deferral;

    try_(fs_File_delete(path.as_const));
} $unguarded(TEST_fn);
