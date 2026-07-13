#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/time/self.h"
#include "dh/heap/Sys.h"
#include "dh/heap/ThrdSafe.h"

T_use$((i32)(
    Co_Ctx, Co_Rtn, Co_Frame,
    Clsr_Ctx, Clsr_Rtn, Clsr,
    clsr_kind, thrd_spawn, thrd_join
));
T_use$((u32)(
    Clsr_Ctx, Clsr_Rtn, Clsr,
    thrd_OnceLock,
    thrd_OnceLock_fini,
    thrd_OnceLock_isSet,
    thrd_OnceLock_trySet,
    thrd_OnceLock_wait,
    thrd_OnceLock_get,
    thrd_LazeLock,
    thrd_LazeLock_fini,
    thrd_LazeLock_isSet,
    thrd_LazeLock_getOrInit
));
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

$static fn_((test__nextLazyValue(u32* counter))(u32)) {
    *counter += 1;
    return 100 + *counter;
};
fn_use_Clsr_((test__nextLazyValue)(u32*)(u32));

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
            .gpa = heap_Sys_alctr(&heap),
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
            .gpa = heap_Sys_alctr(&heap),
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

TEST_fn_("thrd waits: try cancel timeout" $guard) {
    var cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel_src));
    let cancel = thrd_CancelTok_Src_tok(&cancel_src);
    thrd_CancelTok_Src_cancel(&cancel_src);

    var evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&evt));
    try_(TEST_expect(isErr(thrd_ResetEvt_wait(&evt, thrd_CancelTok_wakeable(cancel)))));
    try_(TEST_expect(isErr(thrd_ResetEvt_waitFor(&evt, thrd_CancelTok_wakeable(cancel), time_Dur_fromMillis(1)))));

    var sem = thrd_Sem_init();
    defer_(thrd_Sem_fini(&sem));
    try_(TEST_expect(!thrd_Sem_tryWait(&sem)));
    try_(TEST_expect(isErr(thrd_Sem_wait(&sem, thrd_CancelTok_wakeable(cancel)))));

    var open_cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&open_cancel_src));
    let open_cancel = thrd_CancelTok_Src_tok(&open_cancel_src);
    try_(TEST_expect(isErr(thrd_Sem_waitFor(&sem, thrd_CancelTok_wakeable(open_cancel), time_Dur_zero))));

    var rw = thrd_RWLock_init();
    defer_(thrd_RWLock_fini(&rw));
    try_(thrd_RWLock_lock(&rw, thrd_CancelTok_wakeable(open_cancel)));
    thrd_RWLock_unlock(&rw);
    try_(thrd_RWLock_lockShared(&rw, thrd_CancelTok_wakeable(open_cancel)));
    thrd_RWLock_unlockShared(&rw);

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/OnceLock: sets once and returns typed value" $guard) {
    var lock = thrd_OnceLock_init_static$(u32);
    defer_(thrd_OnceLock_fini$u32(&lock));

    try_(TEST_expect(!thrd_OnceLock_isSet$u32(&lock)));
    try_(TEST_expect(thrd_OnceLock_trySet$u32(&lock, 42)));
    try_(TEST_expect(!thrd_OnceLock_trySet$u32(&lock, 99)));
    thrd_OnceLock_wait$u32(&lock);

    try_(TEST_expect(thrd_OnceLock_isSet$u32(&lock)));
    try_(TEST_expect(thrd_OnceLock_get$u32(&lock) == 42));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/LazeLock: initializes once lazily" $guard) {
    var lock = thrd_LazeLock_init$(u32);
    defer_(thrd_LazeLock_fini$u32(&lock));

    u32 init_count = 0;
    var init = clsr_((test__nextLazyValue)(&init_count));
    let first = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);
    let second = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);

    try_(TEST_expect(first == second));
    try_(TEST_expect(*first == 101));
    try_(TEST_expect(init_count == 1));
    try_(TEST_expect(thrd_LazeLock_isSet$u32(&lock)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Latch: zero count is done for wait and token" $guard) {
    var cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel_src));
    let wait_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&cancel_src));

    var latch = thrd_Latch_init(0);
    defer_(thrd_Latch_fini(&latch));

    try_(TEST_expect(thrd_Latch_isDone(&latch)));
    try_(TEST_expect(thrd_Latch_tryWait(&latch)));
    try_(thrd_Latch_wait(&latch, wait_src));
    try_(TEST_expect(thrd_Latch_isDone(&latch)));
    try_(TEST_expect(thrd_Latch_value(&latch) == 0));

    let tok = thrd_Latch_tok(&latch);
    try_(TEST_expect(thrd_OnceEvt_Tok_tryWait(tok)));
    try_(thrd_OnceEvt_Tok_wait(tok, wait_src));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Group: spawn closure" $guard) {
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

    try_(thrd_Group_spawn(&wg, heap_ThrdSafe_alctr(&thrd_safe_heap), clsr.as_base));
    thrd_Group_waitProtcd(&wg);

    try_(TEST_expect(atom_V_load(&done_count, atom_MemOrd_acquire) == 1));
    return_ok({});
} $unguarded(TEST_fn);
