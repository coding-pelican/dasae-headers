#include "dh-main.h"
#include "dh/proc.h"
#include "dh/heap/Sys.h"
#include "dh/io/Reader.h"
#include "dh/mem/common.h"

TEST_fn_("proc: direct capability exposes process paths" $guard) {
    let self = try_(proc_direct());
    try_(TEST_expect(proc_isValid(self)));

    var_(path_mem, A$$(2048, u8)) $undefined;
    let exe = try_(proc_exePath(self, A_ref$((S$u8)(path_mem))));
    try_(TEST_expect(exe.len != 0));

    let cwd = try_(proc_currPath(self, A_ref$((S$u8)(path_mem))));
    try_(TEST_expect(cwd.len != 0));
} $unguarded(TEST_fn);

$static fn_((test_proc__fakeWait(P$raw ctx, proc_Child* self))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    claim_assert_nonnull(self);
    ++*calls;
    asg_l((&self->handle)(none()));
    self->id = 0;
    return_ok(union_of$((proc_Child_Trm)(proc_Child_Trm_exited)(u8_(42))));
} $unscoped(fn);
$static fn_((test_proc__fakeKill(P$raw ctx, proc_Child* self))(void)) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    claim_assert_nonnull(self);
    ++*calls;
    asg_l((&self->handle)(none()));
    self->id = 0;
};
TEST_fn_("proc: child operations dispatch through proc self" $guard) {
    let direct = try_(proc_direct());
    let vtbl = with_((*direct.vtbl)(
        (.child)({
            .waitFn = test_proc__fakeWait,
            .killFn = test_proc__fakeKill,
        })
    ));
    var_(calls, usize) = 0;
    let proc = proc_ensureValid((proc_Self){
        .ctx = &calls,
        .vtbl = &vtbl,
    });
    var_(child, proc_Child) = {
        .handle = some(9),
        .id = 9,
        .io = {
            .in = none(),
            .out = none(),
            .err = none(),
        },
    };
    let trm = try_(proc_Child_wait(&child, proc));
    try_(TEST_expect(matches(trm, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((trm)(proc_Child_Trm_exited)) == 42));
    asg_l((&child.handle)(some(10)));
    child.id = 10;
    proc_Child_kill(&child, proc);
    try_(TEST_expect(calls == 2));
    try_(TEST_expect(child.id == 0));
    proc_Child_kill(&child, proc);
    try_(TEST_expect(calls == 2));
} $unguarded(TEST_fn);

$static fn_((test_proc__cmd(S$S_const$u8 argv, proc_Stream std_out))(proc_Cmd)) {
    return (proc_Cmd){
        .argv = argv,
        .env = none(),
        .cwd = union_of((proc_Cmd_CWD_inherit){}),
        .std_in = union_of((proc_Stream_ignore){}),
        .std_out = std_out,
        .std_err = union_of((proc_Stream_ignore){}),
        .expand_arg0 = proc_Cmd_ArgExpsn_no_expand,
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
            pp_default_(local_({
                try_(TEST_skipMsg(u8_l("native process spawning is not supported")));
                local_return_(u8_l(""));
            }))
        )),
        u8_l("exit 7"),
    });
    let_(std_out, proc_Stream) = union_of((proc_Stream_ignore){});
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
            pp_default_(local_({
                try_(TEST_skipMsg(u8_l("native process spawning is not supported")));
                local_return_(u8_l(""));
            }))
        )),
    });
    let_(std_out, proc_Stream) = union_of((proc_Stream_pipe){});
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
    let_(std_out, proc_Stream) = union_of((proc_Stream_ignore){});
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
