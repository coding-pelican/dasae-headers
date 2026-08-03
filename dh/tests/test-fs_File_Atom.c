#include "dh-main.h"
#include "dh/fs/File/Atom.h"
#include "dh/fs/path.h"
#include "dh/mem/common.h"

$static fn_((test_fs_File_Atom__pathInThisDir(S_const$u8 name, S$u8 out))(E$S$u8)) {
    return fs_path_join2(fs_path_dirname(mem_spanZ0Bytes(src_loc_fileName())), name, out);
};

TEST_fn_("fs/File/Atom: finalization closes an owned temporary file" $guard) {
    var_(path_mem, A$$(1024, u8)) $undefined;
    let path = try_(test_fs_File_Atom__pathInThisDir(
        u8_l("test-fs_File_Atom.tmp"), A_ref$((S$u8)(path_mem))
    ));
    let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing));
    defer_(let_ignore = catch_((fs_File_delete(path.as_const))($ignore, $do_nothing)));

    var file = try_(fs_File_create(path.as_const, fs_File_CreateFlags_default));
    var atom = (fs_File_Atom){
        .file = file,
        .file_basename_hex = 0,
        .file_open = true,
        .file_exists = true,
        .dir = fs_Dir_Handle_promote(fs_File_handle(file)),
        .close_dir_on_fini = false,
        .dest_sub_path = u8_l(""),
    };
    fs_File_Atom_fini(&atom);

    try_(TEST_expect(!atom.file_open));
    try_(fs_File_delete(path.as_const));
} $unguarded(TEST_fn);
