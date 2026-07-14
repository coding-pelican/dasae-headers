#include "dh-main.h"
#include "dh/proc.h"
#include "dh/fs/File.h"
#include "dh/fs/Dir.h"
#include "dh/io/Reader.h"
#include "dh/fs/path.h"
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

TEST_fn_("proc: current path is available" $scope) {
    var_(buf, A$$(512, u8)) $undefined;
    let path = try_(proc_currentPath(A_ref$((S$u8)(buf))));

    try_(TEST_expect(path.len != 0));
} $unscoped(TEST_fn);

TEST_fn_("proc: spawn and wait exit code" $scope) {
#if plat_is_windows
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("exit"),
        [3] = u8_l("7"),
    });
#elif plat_is_linux
    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("/bin/sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("exit 7"),
    });
#endif /* plat_is_windows, plat_is_linux */
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = none(),
        .cwd = none(),
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_ignore,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));
    let term = try_(proc_Child_wait(&child));

    try_(TEST_expect(term.tag == proc_Ter_Tag_exited));
    try_(TEST_expect(term.code == 7));
} $unscoped(TEST_fn);

TEST_fn_("proc: stdout pipe captures child output" $guard) {
#if plat_is_windows
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("echo"),
        [3] = u8_l("hello"),
    });
    let expected = u8_l("hello" io_crlf);
#elif plat_is_linux
    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("/bin/sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("printf 'hello\\n'"),
    });
    let expected = u8_l("hello\n");
#endif /* plat_is_windows, plat_is_linux */
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = none(),
        .cwd = none(),
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));
    try_(TEST_expect(isSome(child.std_out)));
    let std_out = unwrap_(child.std_out);
    defer_(fs_File_close(std_out));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.tag == proc_Ter_Tag_exited));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(7, u8)) $undefined;
    let reader = fs_File_reader(std_out);
    try_(io_Reader_readExact(reader, S_prefix((A_ref$((S$u8)(buf)))(expected.len))));

    try_(TEST_expect(mem_eqlBytes(S_prefix((A_ref$((S_const$u8)(buf)))(expected.len)), expected)));
} $unguarded(TEST_fn);

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"

$static fn_((test__openDirZ(P_const$u8 path_z))(E$fs_Dir) $scope) {
    let handle = CreateFileA(
        (as$(LPCSTR)(path_z)),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        null,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
        null
    );
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$fs_OpenFailed());
    return_ok((fs_Dir){ .handle = handle });
} $unscoped(fn);

$static fn_((test__readPipeAll(fs_File file, S$u8 out))(E$S$u8) $scope) {
    let reader = fs_File_reader(file);
    let read = try_(io_Reader_read(reader, out));
    return_ok(S_slice((out)$r(0, read)));
} $unscoped(fn);

$static fn_((test__fullPathZ(P_const$u8 path_z, S$u8 out))(E$S$u8) $scope) {
    let wrote = GetFullPathNameA(as$(LPCSTR)(path_z), as$(DWORD)(out.len), as$(LPSTR)(out.ptr), null);
    if (wrote == 0) return_err(E_cause$fs_OpenFailed());
    if (wrote >= out.len) return_err(E_cause$fs_FileTooBig());
    return_ok(S_slice((out)$r(0, as$(usize)(wrote))));
} $unscoped(fn);
#endif /* plat_is_windows */

#if !plat_is_windows
$static fn_((test__readPipeAll(fs_File file, S$u8 out))(E$S$u8) $scope) {
    let reader = fs_File_reader(file);
    let read = try_(io_Reader_read(reader, out));
    return_ok(S_slice((out)$r(0, read)));
} $unscoped(fn);
#endif /* !plat_is_windows */

TEST_fn_("proc: custom environment block is passed to child" $guard) {
#if plat_is_windows
    var_(comspec_buf, A$$(512, u8)) $undefined;
    let comspec_len = GetEnvironmentVariableA(
        "ComSpec", as$(LPSTR)(A_ptr(comspec_buf)), as$(DWORD)(A_len(comspec_buf))
    );
    try_(TEST_expect(comspec_len != 0));

    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = S_slice((A_ref$((S_const$u8)(comspec_buf)))$r(0, as$(usize)(comspec_len))),
        [1] = u8_l("/c"),
        [2] = u8_l("echo"),
        [3] = u8_l("%PROC_TEST_VAR%"),
    });
    let expected = u8_l("from-env" io_crlf);
#elif plat_is_linux
    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("/bin/sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("printf '%s\\n' \"$PROC_TEST_VAR\""),
    });
    let expected = u8_l("from-env\n");
#endif /* plat_is_windows, plat_is_linux */
    var_(env, A$$(1, S_const$u8)) = A_init({
        [0] = u8_l("PROC_TEST_VAR=from-env"),
    });
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = some(A_ref$((S$S_const$u8)(env))),
        .cwd = none(),
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(64, u8)) $undefined;
    let std_out = unwrap_(child.std_out);
    defer_(fs_File_close(std_out));

    let out = try_(test__readPipeAll(std_out, A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(out.as_const, expected)));
} $unguarded(TEST_fn);

TEST_fn_("proc: cwd handle is passed to child" $guard) {
    let dir = fs_path_dirname(mem_spanZ0$u8(src_loc_filePath()));
    var_(dir_z, A$$(1024, u8)) = A_zero();
    mem_copyBytes(S_prefix((A_ref$((S$u8)(dir_z)))(dir.len)), dir);

#if plat_is_windows
    var dir_handle = try_(test__openDirZ(A_ptr(dir_z)));
    defer_(fs_Dir_close(&dir_handle));

    var_(expected_buf, A$$(512, u8)) = A_zero();
    let expected = try_(test__fullPathZ(A_ptr(dir_z), A_ref$((S$u8)(expected_buf)))).as_const;

    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("cd"),
    });
#elif plat_is_linux
    var dir_handle = try_(fs_Dir_openDir((fs_Dir){ .handle = sys_call_linux_AT_FDCWD }, dir, fs_File_OpenFlags_default));
    defer_(fs_Dir_close(&dir_handle));

    let expected = dir;

    var_(argv, A$$(1, S_const$u8)) = A_init({
        [0] = u8_l("/bin/pwd"),
    });
#endif /* plat_is_windows, plat_is_linux */
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = none(),
        .cwd = some(dir_handle),
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(640, u8)) $undefined;
    let std_out = unwrap_(child.std_out);
    defer_(fs_File_close(std_out));
    let out = try_(test__readPipeAll(std_out, A_ref$((S$u8)(buf))));

    var_(expected_line, A$$(640, u8)) $undefined;
    mem_copyBytes(S_slice(((S$u8)A_ref$((S$u8)(expected_line)))$r(0, expected.len)), expected);
#if plat_is_windows
    *A_at((expected_line)[expected.len]) = io_cr_byte;
    *A_at((expected_line)[expected.len + 1]) = io_lf_byte;
    let expected_line_len = expected.len + 2;
#else
    *A_at((expected_line)[expected.len]) = io_lf_byte;
    let expected_line_len = expected.len + 1;
#endif
    try_(TEST_expect(mem_eqlBytes(
        out.as_const, S_slice(((S_const$u8)A_ref$((S_const$u8)(expected_line)))$r(0, expected_line_len))
    )));
} $unguarded(TEST_fn);

TEST_fn_("proc: spawnPath resolves executable relative to dir handle" $guard) {
#if plat_is_windows
    var_(comspec_buf, A$$(512, u8)) $undefined;
    let comspec_len = GetEnvironmentVariableA(
        "ComSpec", as$(LPSTR)(A_ptr(comspec_buf)), as$(DWORD)(A_len(comspec_buf))
    );
    try_(TEST_expect(comspec_len != 0));
    let comspec = S_slice(((S_const$u8)A_ref$((S_const$u8)(comspec_buf)))$r(0, as$(usize)(comspec_len)));
    let comspec_dir = fs_path_dirname(comspec);
    let comspec_base = fs_path_basename(comspec);

    var_(dir_buf, A$$(512, u8)) $undefined;
    mem_copyBytes(S_slice(((S$u8)A_ref$((S$u8)(dir_buf)))$r(0, comspec_dir.len)), comspec_dir);
    *A_at((dir_buf)[comspec_dir.len]) = 0;

    var dir = try_(test__openDirZ(A_ptr(dir_buf)));
    defer_(fs_Dir_close(&dir));

    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = comspec_base,
        [1] = u8_l("/c"),
        [2] = u8_l("exit"),
        [3] = u8_l("9"),
    });
#elif plat_is_linux
    var dir = try_(fs_Dir_openDir((fs_Dir){ .handle = sys_call_linux_AT_FDCWD }, u8_l("/bin"), fs_File_OpenFlags_default));
    defer_(fs_Dir_close(&dir));

    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("exit 9"),
    });
#endif /* plat_is_windows, plat_is_linux */
    var child = try_(proc_spawnPath(
        dir,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = none(),
            .std_in = proc_StdIO_ignore,
            .std_out = proc_StdIO_ignore,
            .std_err = proc_StdIO_ignore,
            .expand_arg0 = proc_ArgExpansion_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    ));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.code == 9));
} $unguarded(TEST_fn);
