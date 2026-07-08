#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/time/self.h"
#include "dh/heap/Sys.h"
#include "dh/heap/ThrdSafe.h"

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use_clsr_kind$(i32);
T_use_thrd_spawn$(i32);
T_use_thrd_join$(i32);

$static fn_((test__double(i32 value))(i32)) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    catch_((time_Awake_sleepMillis(clock, 1))($ignore, $do_nothing));
    return value * 2;
};
fn_use_Clsr_((test__double)(i32)(i32));

$static fn_((test__markDone(atom_V$usize* counter))(Void) $scope) {
    let_ignore = atom_V_pri_fetchAdd(counter, 1, atom_MemOrd_acq_rel);
    return_void();
} $unscoped(fn);
fn_use_Clsr_((test__markDone)(atom_V$usize*)(Void));

$static co_fn_(test__sumAfterSuspend, (i32 lhs; i32 rhs), i32);
co_fn_frame_scope(
    test__sumAfterSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(idle, Void);
    })
);
co_fn_scope(test__sumAfterSuspend) {
    suspend_((idle)(Void_()));
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((test__sumAfterSuspend)(i32, i32)(i32));

TEST_fn_("thrd: spawn and join function closure" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));

    var clsr = clsr_((test__double)(21));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = some$((O$mem_Alctr)(heap_Sys_alctr(&heap))),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr_kind$i32(joined) == Clsr_Kind_fn));
    try_(TEST_expect(clsr.ctx.ret == 42));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: spawn and join coroutine closure" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));

    var clsr = clsr_((test__sumAfterSuspend)(19, 23));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = some$((O$mem_Alctr)(heap_Sys_alctr(&heap))),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr_kind$i32(joined) == Clsr_Kind_co));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ret == 42));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Group: spawn detached closure" $guard) {
    var sys_heap = heap_Sys_init();
    defer_(heap_Sys_fini(&sys_heap));

    var thrd_safe_heap = (heap_ThrdSafe){
        .child_alctr = heap_Sys_alctr(&sys_heap),
        .mtx = thrd_Mtx_init(),
    };
    defer_(thrd_Mtx_fini(&thrd_safe_heap.mtx));

    var wg = thrd_Group_init();
    defer_(thrd_Group_fini(&wg));

    var done_count = atom_V_init$(atom_V$usize, 0);
    var clsr = clsr_((test__markDone)(&done_count));

    thrd_Group_spawn(&wg, heap_ThrdSafe_alctr(&thrd_safe_heap), clsr.as_base);
    thrd_Group_wait(&wg);

    try_(TEST_expect(atom_V_load(&done_count, atom_MemOrd_acquire) == 1));
    return_ok({});
} $unguarded(TEST_fn);
