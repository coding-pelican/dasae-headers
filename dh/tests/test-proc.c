#include "dh-main.h"
#include "dh/proc.h"
#include "dh/fs/File.h"
#include "dh/fs/Dir.h"
#include "dh/io/Reader.h"
#include "dh/fs/path.h"

#if plat_is_windows
#include "dh/os/windows/file.h"
#include "dh/os/windows/handle.h"

$static fn_((test__openDirZ(P_const$u8 path_z))(E$fs_Dir) $scope) {
    let handle = CreateFileA(
        as$(LPCSTR)(path_z), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$OpenFailedFS());
    return_ok((fs_Dir){ .handle = handle });
} $unscoped(fn);

$static fn_((test__readPipeAll(fs_File file, S$u8 out))(E$S$u8) $scope) {
    let reader = fs_File_reader(file);
    let read = try_(io_Reader_read(reader, out));
    return_ok(S_slice((out)$r(0, read)));
} $unscoped(fn);

$static fn_((test__fullPathZ(P_const$u8 path_z, S$u8 out))(E$S$u8) $scope) {
    let wrote = GetFullPathNameA(as$(LPCSTR)(path_z), as$(DWORD)(out.len), as$(LPSTR)(out.ptr), null);
    if (wrote == 0) return_err(E_cause$OpenFailedFS());
    if (wrote >= out.len) return_err(E_cause$FileTooBigFS());
    return_ok(S_slice((out)$r(0, as$(usize)(wrote))));
} $unscoped(fn);
#endif

TEST_fn_("proc: current path is available" $scope) {
    var_(buf, A$$(512, u8)) = A_zero();
    let path = try_(proc_currentPath(A_ref$((S$u8)(buf))));

    try_(TEST_expect(path.len != 0));
} $unscoped(TEST_fn);

TEST_fn_("proc: spawn and wait exit code" $scope) {
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("exit"),
        [3] = u8_l("7"),
    });
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = { 0 },
        .cwd = null,
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_ignore,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));
    let term = try_(proc_Child_wait(&child));

    try_(TEST_expect(term.tag == proc_Term_Tag_exited));
    try_(TEST_expect(term.code == 7));
} $unscoped(TEST_fn);

TEST_fn_("proc: stdout pipe captures child output" $scope) {
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("echo"),
        [3] = u8_l("hello"),
    });
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = { 0 },
        .cwd = null,
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));
    try_(TEST_expect(child.std_out.is_present));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.tag == proc_Term_Tag_exited));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(7, u8)) = A_zero();
    let reader = fs_File_reader(child.std_out.file);
    try_(io_Reader_readExact(reader, A_ref$((S$u8)(buf))));
    fs_File_close(child.std_out.file);

    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("hello\r\n"))));
} $unscoped(TEST_fn);

#if plat_is_windows
TEST_fn_("proc: custom environment block is passed to child" $scope) {
    var_(comspec_buf, A$$(512, u8)) = A_zero();
    let comspec_len = GetEnvironmentVariableA("ComSpec", as$(LPSTR)(A_ptr(comspec_buf)), as$(DWORD)(A_len(comspec_buf)));
    try_(TEST_expect(comspec_len != 0));

    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = S_slice(((S_const$u8)A_ref$((S_const$u8)(comspec_buf)))$r(0, as$(usize)(comspec_len))),
        [1] = u8_l("/c"),
        [2] = u8_l("echo"),
        [3] = u8_l("%PROC_TEST_VAR%"),
    });
    var_(env, A$$(1, S_const$u8)) = A_init({
        [0] = u8_l("PROC_TEST_VAR=from-env"),
    });
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = A_ref$((S$S_const$u8)(env)),
        .cwd = null,
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(64, u8)) = A_zero();
    let out = try_(test__readPipeAll(child.std_out.file, A_ref$((S$u8)(buf))));
    fs_File_close(child.std_out.file);
    try_(TEST_expect(mem_eqlBytes(out.as_const, u8_l("from-env\r\n"))));
} $unscoped(TEST_fn);

TEST_fn_("proc: cwd handle is passed to child" $guard) {
    let dir = fs_path_dirname(u8_l(__FILE__));
    var_(dir_z, A$$(1024, u8)) = A_zero();
    mem_copyBytes(S_prefix((A_ref$((S$u8)(dir_z)))(dir.len)), dir);

    var dir_handle = try_(test__openDirZ(A_ptr(dir_z)));
    defer_(fs_Dir_close(&dir_handle));

    var_(expected_buf, A$$(512, u8)) = A_zero();
    let expected = try_(test__fullPathZ(A_ptr(dir_z), A_ref$((S$u8)(expected_buf))));

    var_(argv, A$$(3, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("cd"),
    });
    var child = try_(proc_spawn((proc_Cmd){
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = { 0 },
        .cwd = &dir_handle,
        .std_in = proc_StdIO_ignore,
        .std_out = proc_StdIO_pipe,
        .std_err = proc_StdIO_ignore,
        .expand_arg0 = proc_ArgExpansion_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    }));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(term.code == 0));

    var_(buf, A$$(640, u8)) = A_zero();
    let out = try_(test__readPipeAll(child.std_out.file, A_ref$((S$u8)(buf))));
    fs_File_close(child.std_out.file);

    var_(expected_line, A$$(640, u8)) = A_zero();
    mem_copyBytes(S_slice(((S$u8)A_ref$((S$u8)(expected_line)))$r(0, expected.len)), expected.as_const);
    expected_line.val[expected.len] = '\r';
    expected_line.val[expected.len + 1] = '\n';
    try_(TEST_expect(mem_eqlBytes(out.as_const, S_slice(((S_const$u8)A_ref$((S_const$u8)(expected_line)))$r(0, expected.len + 2)))));
} $unguarded(TEST_fn);

TEST_fn_("proc: spawnPath resolves executable relative to dir handle" $guard) {
    var_(comspec_buf, A$$(512, u8)) = A_zero();
    let comspec_len = GetEnvironmentVariableA("ComSpec", as$(LPSTR)(A_ptr(comspec_buf)), as$(DWORD)(A_len(comspec_buf)));
    try_(TEST_expect(comspec_len != 0));
    let comspec = S_slice(((S_const$u8)A_ref$((S_const$u8)(comspec_buf)))$r(0, as$(usize)(comspec_len)));
    let comspec_dir = fs_path_dirname(comspec);
    let comspec_base = fs_path_basename(comspec);

    var_(dir_buf, A$$(512, u8)) = A_zero();
    mem_copyBytes(S_slice(((S$u8)A_ref$((S$u8)(dir_buf)))$r(0, comspec_dir.len)), comspec_dir);
    dir_buf.val[comspec_dir.len] = 0;

    var dir = try_(test__openDirZ(A_ptr(dir_buf)));
    defer_(fs_Dir_close(&dir));

    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = comspec_base,
        [1] = u8_l("/c"),
        [2] = u8_l("exit"),
        [3] = u8_l("9"),
    });
    var child = try_(proc_spawnPath(
        dir,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = { 0 },
            .cwd = null,
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
#endif
