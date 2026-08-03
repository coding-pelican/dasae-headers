#include "dh-main.h"
#include "dh/proc/Self.h"
#include "dh/heap/Sys.h"
#include "dh/io/Reader.h"
#include "dh/mem/common.h"

TEST_fn_("proc/Self: direct capability exposes process paths" $guard) {
    let self = try_(proc_direct());
    try_(TEST_expect(proc_isValid(self)));

    let exe_file = try_(proc_openExe(self, fs_File_OpenFlags_default));
    defer_(fs_File_close(exe_file));
    let exe_stat = try_(fs_File_stat(exe_file));
    try_(TEST_expect(exe_stat.kind == fs_Kind_file));

    var_(path_mem, A$$(2048, u8)) $undefined;
    let exe = try_(proc_exePath(self, A_ref$((S$u8)(path_mem))));
    try_(TEST_expect(exe.len != 0));

    let cwd = try_(proc_currPath(self, A_ref$((S$u8)(path_mem))));
    try_(TEST_expect(cwd.len != 0));
} $unguarded(TEST_fn);

TEST_fn_("proc/Self: spawn and replace options expose operation defaults" $scope) {
    var argv = A_from$((S_const$u8){ u8_l("program") });
    let spawn = proc_Spawn_Opts_default(A_ref$((S$S_const$u8)(argv)));
    let replace = proc_Replace_Opts_default(A_ref$((S$S_const$u8)(argv)));

    try_(TEST_expect(spawn.argv.len == 1));
    try_(TEST_expect(isNone(spawn.env)));
    try_(TEST_expect(matches(spawn.cwd, proc_cmd_CWD_inherit)));
    try_(TEST_expect(matches(spawn.std_in, proc_cmd_StdIO_inherit)));
    try_(TEST_expect(matches(spawn.std_out, proc_cmd_StdIO_inherit)));
    try_(TEST_expect(matches(spawn.std_err, proc_cmd_StdIO_inherit)));
    try_(TEST_expect(spawn.expand_arg0 == proc_cmd_ArgExpsn_no_expand));
    try_(TEST_expect(!spawn.start_suspended));
    try_(TEST_expect(!spawn.create_no_window));

    try_(TEST_expect(replace.argv.len == 1));
    try_(TEST_expect(isNone(replace.env)));
    try_(TEST_expect(replace.expand_arg0 == proc_cmd_ArgExpsn_no_expand));
} $unscoped(TEST_fn);

TEST_fn_("proc/Self: direct user lookup resolves root on POSIX systems" $scope) {
    pp_if_(plat_is_linux)(
        pp_then_(
            let self = try_(proc_direct());
            let info = try_(proc_userInfo(self, u8_l("root")));
            try_(TEST_expect(info.uid == 0));
            try_(TEST_expect(info.gid == 0));
        ),
        pp_else_(
            try_(TEST_skipMsg(u8_l("direct user lookup is not supported on this target")));
        ));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("proc/Self: direct base address identifies the current image" $scope) {
    let self = try_(proc_direct());
    let address = catch_((proc_baseAddr(self))(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("base address is unavailable"))))
    ));
    try_(TEST_expect(address != 0));
} $unscoped(TEST_fn);

TEST_fn_("proc/Self: clean exit returns in debug builds" $scope) {
    pp_if_(debug_enabled)(
        pp_then_(
            let self = try_(proc_direct());
            proc_cleanExit(self);
        ),
        pp_else_(
            try_(TEST_skipMsg(u8_l("cleanExit terminates in non-debug builds")));
        ));
    return_ok({});
} $unscoped(TEST_fn);

$static fn_((test_proc__cmd(S$S_const$u8 argv, proc_cmd_StdIO std_out))(proc_Spawn_Opts)) {
    return (proc_Spawn_Opts){
        .argv = argv,
        .env = none(),
        .cwd = union_of((proc_cmd_CWD_inherit){}),
        .std_in = union_of((proc_cmd_StdIO_ignore){}),
        .std_out = std_out,
        .std_err = union_of((proc_cmd_StdIO_ignore){}),
        .expand_arg0 = proc_cmd_ArgExpsn_no_expand,
        .start_suspended = false,
        .create_no_window = true,
    };
};
TEST_fn_("proc: spawn and wait report child exit code" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let self = try_(proc_direct());
    var argv = A_from$((S_const$u8){
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)(
                u8_l("cmd.exe"),
                u8_l("/D"),
                u8_l("/C")
            )),
            pp_case_((plat_type_linux)(
                u8_l("/bin/sh"),
                u8_l("-c")
            )),
            pp_default_(()(local_({
                try_(TEST_skipMsg(u8_l("native process spawning is not supported")));
                local_return_(u8_l(""));
            })))
        )),
        u8_l("exit 7"),
    });
    let_(std_out, proc_cmd_StdIO) = union_of((proc_cmd_StdIO_ignore){});
    var child = try_(proc_spawn(
        self, heap_Sys_alctr(&heap),
        proc_Env_empty, test_proc__cmd(A_ref$((S$S_const$u8)(argv)), std_out)
    ));
    let trm = try_(proc_Child_wait(&child, self));
    try_(TEST_expect(matches(trm, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((trm)(proc_Child_Trm_exited)) == 7));
    try_(TEST_expect(isNone(child.handle)));
    try_(TEST_expect(isNone(child.io.in)));
    try_(TEST_expect(isNone(child.io.out)));
    try_(TEST_expect(isNone(child.io.err)));
} $unguarded(TEST_fn);

TEST_fn_("proc: pipe output contains child bytes" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let self = try_(proc_direct());
    var argv = A_from$((S_const$u8){
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)(
                u8_l("cmd.exe"),
                u8_l("/D"),
                u8_l("/C"),
                u8_l("echo proc-pipe")
            )),
            pp_case_((plat_type_linux)(
                u8_l("/bin/sh"),
                u8_l("-c"),
                u8_l("printf proc-pipe")
            )),
            pp_default_(()(local_({
                try_(TEST_skipMsg(u8_l("native process spawning is not supported")));
                local_return_(u8_l(""));
            })))
        )),
    });
    let_(std_out, proc_cmd_StdIO) = union_of((proc_cmd_StdIO_pipe){});
    var child = try_(proc_spawn(
        self,
        heap_Sys_alctr(&heap),
        proc_Env_empty,
        test_proc__cmd(
            A_ref$((S$S_const$u8)(argv)),
            std_out
        )
    ));
    let out_file = unwrap_(child.io.out);
    var_(out_mem, A$$(32, u8)) $undefined;
    let read_len = try_(io_Reader_read(
        fs_File_reader(out_file),
        A_ref$((S$u8)(out_mem))
    ));
    let trm = try_(proc_Child_wait(&child, self));

    try_(TEST_expect(matches(trm, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((trm)(proc_Child_Trm_exited)) == 0));
    try_(TEST_expect(isNone(child.handle)));
    try_(TEST_expect(isNone(child.io.out)));
    try_(TEST_expect(read_len >= u8_l("proc-pipe").len));
    try_(TEST_expect(mem_eqlBytes(
        A_prefix$((S_const$u8)(out_mem)(u8_l("proc-pipe").len)),
        u8_l("proc-pipe")
    )));
} $unguarded(TEST_fn);

TEST_fn_("proc: empty command is rejected before spawn" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let self = try_(proc_direct());
    var_(argv, A$$(0, S_const$u8)) = {};
    let_(std_out, proc_cmd_StdIO) = union_of((proc_cmd_StdIO_ignore){});
    let rejected = eval_(bool $scope)(catch_((proc_spawn(
        self, heap_Sys_alctr(&heap),
        proc_Env_empty, test_proc__cmd(A_ref$((S$S_const$u8)(argv)), std_out)
    ))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$proc_InvalidName().as_any)));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(rejected));
} $unguarded(TEST_fn);

TEST_fn_("proc: run collects stdout and stderr concurrently" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let self = try_(proc_direct());
    var argv = A_from$((S_const$u8){
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)(
                u8_l("cmd.exe"),
                u8_l("/D"),
                u8_l("/C"),
                u8_l("<nul set /p=proc-out&1>&2<nul set /p=proc-err&exit /b 0")
            )),
            pp_case_((plat_type_linux)(
                u8_l("/bin/sh"),
                u8_l("-c"),
                u8_l("printf proc-out; printf proc-err >&2")
            )),
            pp_default_(()(local_({
                try_(TEST_skipMsg(u8_l("native process running is not supported")));
                local_return_(u8_l(""));
            })))
        )),
    });
    var opts = proc_Run_Opts_default(A_ref$((S$S_const$u8)(argv)));
    opts.stdout_limit = 64;
    opts.stderr_limit = 64;
    let result = try_(proc_run(self, gpa, proc_Env_empty, opts));
    defer_(mem_Alctr_freeBytes($trace gpa, result.out));
    defer_(mem_Alctr_freeBytes($trace gpa, result.err));

    try_(TEST_expect(matches(result.term, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((result.term)(proc_Child_Trm_exited)) == 0));
    try_(TEST_expect(mem_eqlBytes(result.out.as_const, u8_l("proc-out"))));
    try_(TEST_expect(mem_eqlBytes(result.err.as_const, u8_l("proc-err"))));
} $unguarded(TEST_fn);
