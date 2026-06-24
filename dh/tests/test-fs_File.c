#include "dh-main.h"
#include "dh/fs/File.h"
#include "dh/fs/path.h"
#include "dh/mem/common.h"

$static fn_((test__pathInThisDir(S_const$u8 name, S$u8 out))(E$S$u8)) {
    return fs_path_join2(fs_path_dirname(mem_spanZ0$u8(src_loc_fileName())), name, out);
};

TEST_fn_("fs/File: create, positioned IO, stat, rename, delete" $guard) {
    var_(path_mem, A$$(1024, u8)) $undefined;
    var_(renamed_path_mem, A$$(1024, u8)) $undefined;
    let path = try_(test__pathInThisDir(u8_l("test-fs_File-direct.tmp"), A_ref$((S$u8)(path_mem))));
    let renamed_path = try_(test__pathInThisDir(u8_l("test-fs_File-direct-renamed.tmp"), A_ref$((S$u8)(renamed_path_mem))));

    let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing));
    let_ignore = catch_((fs_File_delete(renamed_path.as_const))($ignore, $do_nothing));
    defer_(let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing)));
    defer_(let_ignore = catch_((fs_File_delete(renamed_path.as_const))($ignore, $do_nothing)));

    using_(
        let create_flags = with_((fs_File_CreateFlags_default)(
            (.read)(true)
        ))
    ) blk_defer {
        var file = try_(fs_File_create(path.as_const, create_flags));
        defer_(fs_File_close(file));

        try_(TEST_expect(try_(fs_File_writePos(file, u8_l("abcdef"), 0)) == 6));
        try_(fs_File_sync(file));

        let stat = try_(fs_File_stat(file));
        try_(TEST_expect(stat.size == 6));
        try_(TEST_expect(stat.kind == fs_Kind_file));

        var_(buf, A$$(3, u8)) $undefined;
        try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(buf)), 2)) == 3));
        try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("cde"))));

        try_(fs_File_setLen(file, 3));
    } blk_deferral;

    try_(fs_File_rename(path.as_const, renamed_path.as_const));

    using_(
        let open_flags = with_((fs_File_OpenFlags_default)(
            (.mode)(fs_OpenMode_read_only)
        ))
    ) blk_defer {
        var file = try_(fs_File_open(renamed_path.as_const, open_flags));
        defer_(fs_File_close(file));

        var_(truncated, A$$(3, u8)) $undefined;
        try_(TEST_expect(try_(fs_File_readPos(file, A_ref$((S$u8)(truncated)), 0)) == 3));
        try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(truncated)), u8_l("abc"))));
    } blk_deferral;

    try_(fs_File_delete(renamed_path.as_const));
} $unguarded(TEST_fn);
