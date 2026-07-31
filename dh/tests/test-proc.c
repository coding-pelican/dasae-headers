#include "dh-main.h"
#include "dh/proc.h"
#include "dh/start/Entry.h"
#include "dh/mem/common.h"
#include "dh/fs/File.h"
#include "dh/fs/Dir.h"
#include "dh/io/Reader.h"
#include "dh/fs/path.h"
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

T_alias$((test_proc__Argv)(A$$(1, P_const$u8)));
T_alias$((test_proc__Fixture)(struct test_proc__Fixture {
    var_(start, start_Entry);
    var_(direct, proc_Direct);
    var_(argv, test_proc__Argv);
}));

$static fn_((test_proc__Fixture_init(test_proc__Fixture* self))(proc_Self)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    start_Entry_initWin32(&self->start);
#else
    self->argv = (test_proc__Argv)A_init({
        [0] = as$(P_const$u8)("test-proc"),
    });
    start_Entry_initClassic(
        &self->start,
        A_len(self->argv),
        as$(P_const$(P_const$u8))(A_ptr(self->argv))
    );
#endif
    self->direct = proc_Direct_init(start_Entry_env(&self->start));
    let proc = catch_((proc_Direct_self(&self->direct))($ignore, claim_unreachable));
    return start_Entry_entry(&self->start, proc).proc;
};

$static fn_((test_proc__Fixture_fini(test_proc__Fixture* self))(void)) {
    claim_assert_nonnull(self);
    start_Entry_fini(&self->start);
};


T_alias$((test_proc__Injected)(struct test_proc__Injected {
    var_(marker, u8);
}));

$static fn_((test_proc__Injected_executablePath(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8));
$static fn_((test_proc__Injected_currentPath(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8));
$static fn_((test_proc__Injected_setCurrentPath(P$raw ctx, S_const$u8 path))(proc_Path_E$void));
$static fn_((test_proc__Injected_spawn(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$static fn_((test_proc__Injected_spawnPath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child));
$static fn_((test_proc__Injected_replace(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$void));
$static fn_((test_proc__Injected_replacePath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$void));

$static let_(test_proc__Injected_self_vtbl, proc_Self_VTbl) = {
    .executablePathFn = test_proc__Injected_executablePath,
    .currentPathFn = test_proc__Injected_currentPath,
    .setCurrentPathFn = test_proc__Injected_setCurrentPath,
    .spawnFn = test_proc__Injected_spawn,
    .spawnPathFn = test_proc__Injected_spawnPath,
    .replaceFn = test_proc__Injected_replace,
    .replacePathFn = test_proc__Injected_replacePath,
};

fn_((test_proc__Injected_executablePath(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_currentPath(P$raw ctx, S$u8 out_buf))(proc_Path_E$S$u8) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_setCurrentPath(P$raw ctx, S_const$u8 path))(proc_Path_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = path;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_spawn(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$proc_Child) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_spawnPath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$proc_Child) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_replace(P$raw ctx, proc_Cmd cmd))(proc_Spawn_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((test_proc__Injected_replacePath(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(proc_Spawn_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$static fn_((test_proc__Injected_self(test_proc__Injected* ctx))(proc_Self)) {
    claim_assert_nonnull(ctx);
    return proc_ensureValid((proc_Self){
        .ctx = ctx,
        .vtbl = &test_proc__Injected_self_vtbl,
    });
};

TEST_fn_("proc: current path is available" $guard) {
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
    var_(buf, A$$(512, u8)) $undefined;
    let path = try_(proc_currentPath(self, A_ref$((S$u8)(buf))));

    try_(TEST_expect(path.len != 0));
} $unguarded(TEST_fn);

TEST_fn_("proc: spawn and wait exit code" $guard) {
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
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
    var child = try_(proc_spawn(
        self,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = none(),
            .std_in = union_of((proc_std_IO_ignore){}),
            .std_out = union_of((proc_std_IO_ignore){}),
            .std_err = union_of((proc_std_IO_ignore){}),
            .expand_arg0 = proc_ArgExpsn_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    ));
    let term = try_(proc_Child_wait(&child));

    try_(TEST_expect(matches(term, proc_Child_Ter_Tag_exited)));
    try_(TEST_expect(union_to((term)(proc_Child_Ter_Tag_exited)) == 7));
} $unguarded(TEST_fn);

TEST_fn_("proc: stdout pipe captures child output" $guard) {
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
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
    var child = try_(proc_spawn(
        self,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = none(),
            .std_in = union_of((proc_std_IO_ignore){}),
            .std_out = union_of((proc_std_IO_pipe){}),
            .std_err = union_of((proc_std_IO_ignore){}),
            .expand_arg0 = proc_ArgExpsn_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    ));
    try_(TEST_expect(isSome(child.io.out)));
    let std_out = unwrap_(child.io.out);
    defer_(fs_File_close(std_out));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(matches(term, proc_Child_Ter_Tag_exited)));
    try_(TEST_expect(union_to((term)(proc_Child_Ter_Tag_exited)) == 0));

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
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
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
    var child = try_(proc_spawn(self, (proc_Cmd){
                                          .argv = A_ref$((S$S_const$u8)(argv)),
                                          .env = some(A_ref$((S$S_const$u8)(env))),
                                          .cwd = none(),
                                          .std_in = union_of((proc_std_IO_ignore){}),
                                          .std_out = union_of((proc_std_IO_pipe){}),
                                          .std_err = union_of((proc_std_IO_ignore){}),
                                          .expand_arg0 = proc_ArgExpsn_no_expand,
                                          .start_suspended = false,
                                          .create_no_window = true,
                                      }));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(union_to((term)(proc_Child_Ter_Tag_exited)) == 0));

    var_(buf, A$$(64, u8)) $undefined;
    let std_out = unwrap_(child.io.out);
    defer_(fs_File_close(std_out));

    let out = try_(test__readPipeAll(std_out, A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(out.as_const, expected)));
} $unguarded(TEST_fn);

TEST_fn_("proc: cwd handle is passed to child" $guard) {
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
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
    var child = try_(proc_spawn(
        self,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = some(dir_handle),
            .std_in = union_of((proc_std_IO_ignore){}),
            .std_out = union_of((proc_std_IO_pipe){}),
            .std_err = union_of((proc_std_IO_ignore){}),
            .expand_arg0 = proc_ArgExpsn_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    ));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(union_to((term)(proc_Child_Ter_Tag_exited)) == 0));

    var_(buf, A$$(640, u8)) $undefined;
    let std_out = unwrap_(child.io.out);
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
    var_(fixture, test_proc__Fixture) $undefined;
    let self = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));
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
        self,
        dir,
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = none(),
            .std_in = union_of((proc_std_IO_ignore){}),
            .std_out = union_of((proc_std_IO_ignore){}),
            .std_err = union_of((proc_std_IO_ignore){}),
            .expand_arg0 = proc_ArgExpsn_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    ));

    let term = try_(proc_Child_wait(&child));
    try_(TEST_expect(union_to((term)(proc_Child_Ter_Tag_exited)) == 9));
} $unguarded(TEST_fn);
TEST_fn_("proc: Self accepts a user-provided implementation" $guard) {
    var_(ctx, test_proc__Injected) = { .marker = 1 };
    let self = test_proc__Injected_self(&ctx);
    try_(TEST_expect(proc_isValid(self)));
} $unguarded(TEST_fn);

TEST_fn_("proc: Entry exposes copyable startup facts" $guard) {
    var_(fixture, test_proc__Fixture) $undefined;
    let_ignore = test_proc__Fixture_init(&fixture);
    defer_(test_proc__Fixture_fini(&fixture));

    let proc = catch_((proc_Direct_self(&fixture.direct))($ignore, claim_unreachable));
    let entry = start_Entry_entry(&fixture.start, proc);
    let copied = entry;
    try_(TEST_expect(proc_isValid(copied.proc)));

    var args = proc_Args_iter(copied.args);
    var_(scratch, A$$(256, u8)) $undefined;
    let arg0 = try_(proc_Args_Iter_next(&args, A_ref$((S$u8)(scratch))));
    try_(TEST_expect(isSome(arg0)));
    try_(TEST_expect(isSome(proc_Preopens_by(copied.preopens, u8_l("stdin")))));
} $unguarded(TEST_fn);
