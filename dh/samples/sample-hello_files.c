#include "dh-main.h"
#include "dh/fs/File.h"
#include "dh/fs/path.h"
#include "dh/io/stream.h"
#include "dh/mem/common.h"

T_use_A$(4, S_const$u8);
T_use_A$(4, fs_File);
T_use_E$($set(fs_E)(A$4$fs_File));

$static fn_((samplePathInThisDir(S_const$u8 name, S$u8 out))(E$S$u8)) {
    return fs_path_join2(fs_path_dirname(mem_spanZ0$u8(srcLoc().file_path)), name, out);
};

$attr($must_check)
$static fn_((createHelloFile(S_const$u8 path, S_const$u8 message))(E$fs_File) $guard) {
    let create_flags = with_((fs_File_CreateFlags_default)(
        (.read)(true)
    ));
    var file = try_(fs_File_create(path, create_flags));
    errdefer_($ignore, fs_File_close(file));

    let written = try_(fs_File_writePos(file, message, 0));
    debug_assert_true(written == message.len);
    try_(fs_File_sync(file));

    return_ok(file);
} $unguarded(fn);

$attr($must_check)
$static fn_((createHelloFiles(A$4$S_const$u8 paths))(fs_E$A$4$fs_File) $guard) {
    var fp1 = try_(createHelloFile(*A_at((paths)[0]), u8_l("hello 1\n")));
    errdefer_($ignore, fs_File_close(fp1));

    var fp2 = try_(createHelloFile(*A_at((paths)[1]), u8_l("hello 2\n")));
    errdefer_($ignore, fs_File_close(fp2));

    var fp3 = try_(createHelloFile(*A_at((paths)[2]), u8_l("hello 3\n")));
    errdefer_($ignore, fs_File_close(fp3));

    var fp4 = try_(createHelloFile(*A_at((paths)[3]), u8_l("hello 4\n")));
    errdefer_($ignore, fs_File_close(fp4));

    return_ok((A$4$fs_File)A_init({
        [0] = fp1,
        [1] = fp2,
        [2] = fp3,
        [3] = fp4,
    }));
} $unguarded(fn);

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;

    var_(path_mem_1, A$$(512, u8)) $undefined;
    var_(path_mem_2, A$$(512, u8)) $undefined;
    var_(path_mem_3, A$$(512, u8)) $undefined;
    var_(path_mem_4, A$$(512, u8)) $undefined;

    let paths = (A$4$S_const$u8)A_init({
        [0] = try_(samplePathInThisDir(u8_l("sample-hello_files-1.tmp"), A_ref$((S$u8)(path_mem_1)))).as_const,
        [1] = try_(samplePathInThisDir(u8_l("sample-hello_files-2.tmp"), A_ref$((S$u8)(path_mem_2)))).as_const,
        [2] = try_(samplePathInThisDir(u8_l("sample-hello_files-3.tmp"), A_ref$((S$u8)(path_mem_3)))).as_const,
        [3] = try_(samplePathInThisDir(u8_l("sample-hello_files-4.tmp"), A_ref$((S$u8)(path_mem_4)))).as_const,
    });

    let_ignore = catch_((fs_File_delete(*A_at((paths)[0])))($ignore, $do_nothing));
    let_ignore = catch_((fs_File_delete(*A_at((paths)[1])))($ignore, $do_nothing));
    let_ignore = catch_((fs_File_delete(*A_at((paths)[2])))($ignore, $do_nothing));
    let_ignore = catch_((fs_File_delete(*A_at((paths)[3])))($ignore, $do_nothing));
    defer_({
        let_ignore = catch_((fs_File_delete(*A_at((paths)[0])))($ignore, $do_nothing));
        let_ignore = catch_((fs_File_delete(*A_at((paths)[1])))($ignore, $do_nothing));
        let_ignore = catch_((fs_File_delete(*A_at((paths)[2])))($ignore, $do_nothing));
        let_ignore = catch_((fs_File_delete(*A_at((paths)[3])))($ignore, $do_nothing));
    });

    let files = try_(createHelloFiles(paths));
    defer_({
        fs_File_close(*A_at((files)[0]));
        fs_File_close(*A_at((files)[1]));
        fs_File_close(*A_at((files)[2]));
        fs_File_close(*A_at((files)[3]));
    });

    for_(($r(0, 4))(i)) {
        var_(buf, A$$(8, u8)) $undefined;
        let read = try_(fs_File_readPos(*A_at((files)[i]), A_ref$((S$u8)(buf)), 0));
        io_stream_print(u8_l("{:s}"), S_prefix((A_ref$((S_const$u8)(buf)))(read)));
    } $end(for);

    return_ok({});
} $unguarded(fn);
