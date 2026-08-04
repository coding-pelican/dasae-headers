#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Coop.h"
#include "dh/ArrList.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/time.h"

T_alias$((test_exec_Coop_Event)(u8));
T_use$((test_exec_Coop_Event)(
    P, S,
    ArrList,
    ArrList_Grip,
    ArrList_grip,
    ArrList_Grip_release,
    ArrList_appendWithin,
    Clsr_Ctx, Clsr_Rtn, Clsr,
    Co_Ctx, Co_Rtn, Co_Frame,
    Future, Future_await, Future_cancel,
    Sched_async
));

T_alias$((test_exec_Coop_EventLog)(struct test_exec_Coop_EventLog {
    var_(items, A$$(24, test_exec_Coop_Event));
    var_(len, usize);
}));
$static fn_((test_exec_Coop__push(
    test_exec_Coop_EventLog* self, test_exec_Coop_Event event
))(void) $guard) {
    var list = ArrList_grip$test_exec_Coop_Event(
        A_ref$((S$test_exec_Coop_Event)(self->items)),
        &self->len
    );
    defer_(ArrList_Grip_release$test_exec_Coop_Event(&list));
    ArrList_appendWithin$test_exec_Coop_Event(&list.ctx, event);
} $unguarded(fn);

T_alias$((test_exec_Coop_Sys)(struct test_exec_Coop_Sys {
    var_(sched, Sched);
    var_(time, time_Awake);
}));
T_alias$((test_exec_Coop_SysLogged)(struct test_exec_Coop_SysLogged {
    T_embed$(test_exec_Coop_Sys);
    var_(log, test_exec_Coop_EventLog*);
}));
$static fn_((test_exec_Coop__countFn(
    test_exec_Coop_SysLogged sys, usize n, time_Dur interval, test_exec_Coop_Event base
))(test_exec_Coop_Event)) {
    test_exec_Coop__push(sys.log, base);
    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, interval))($ignore, $do_nothing));
        test_exec_Coop__push(sys.log, base + as$(test_exec_Coop_Event)(i + 1));
    } $end(for);
    test_exec_Coop__push(sys.log, base + 9);
    return base + 9;
};
fn_use_Clsr_((test_exec_Coop__countFn)(
    test_exec_Coop_SysLogged, usize, time_Dur, test_exec_Coop_Event
)(test_exec_Coop_Event));

$static co_fn_(test_exec_Coop__countCo, (
    test_exec_Coop_SysLogged sys;
    usize n;
    time_Dur interval;
    test_exec_Coop_Event base
), test_exec_Coop_Event);
co_fn_frame_scope(
    test_exec_Coop__countCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_Coop__countCo) {
    test_exec_Coop__push($co_arg(sys).log, $co_arg(base));
    for (co_var_(i, usize) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, $co_arg(interval)))(
            $ignore, $do_nothing
        ))));
        test_exec_Coop__push(
            $co_arg(sys).log,
            $co_arg(base) + as$(test_exec_Coop_Event)($co_mut(i) + 1)
        );
    };
    test_exec_Coop__push($co_arg(sys).log, $co_arg(base) + 9);
    co_return_($co_arg(base) + 9);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_Coop__countCo)(
    test_exec_Coop_SysLogged, usize, time_Dur, test_exec_Coop_Event
)(test_exec_Coop_Event));

T_use$((u32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((u32)(Future, Future_await, Future_cancel, Sched_async));
$static co_fn_(test_exec_Coop__cancelOnSleepCo, (test_exec_Coop_Sys sys), u32);
co_fn_frame_scope(
    test_exec_Coop__cancelOnSleepCo,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_Coop__cancelOnSleepCo) {
    suspend_((sleeping)(catch_((time_Awake_sleep(
        $co_arg(sys).time,
        time_Dur_fromMillis(5000)
    ))($ignore, $do_nothing))));
    catch_((Sched_idle($co_arg(sys).sched))(err, switch (E_tag$Sched_Cancelable(err)) {
        case_((E_Tag$Sched_Canceled)) co_return_(99) $end(case);
        case (E_Tag$Any): claim_unreachable;
    }));
    co_return_(77);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_Coop__cancelOnSleepCo)(test_exec_Coop_Sys)(u32));

T_alias$((test_exec_Coop_Probe)(struct test_exec_Coop_Probe {
    var_(coop, exec_Coop*);
    var_(time, time_Awake);
    var_(observed_running_task, bool*);
}));

$static fn_((test_exec_Coop__sleepAndProbe(test_exec_Coop_Probe probe))(u32)) {
    if_some((exec_Coop_task(probe.coop))(task)) {
        *P_at((probe.observed_running_task)[0]) = task->state == exec_Task_State_running;
    }
    catch_((time_Awake_sleep(probe.time, time_Dur_fromMillis(1)))($ignore, $do_nothing));
    return 73;
};
fn_use_Clsr_((test_exec_Coop__sleepAndProbe)(test_exec_Coop_Probe)(u32));

T_alias$((test_exec_Coop_Clock)(struct test_exec_Coop_Clock {
    var_(now, time_Inst);
}));

$static fn_((test_exec_Coop_Clock__now(P$raw ctx))(time_Awake_Inst)) {
    let self = ptrCast$((test_exec_Coop_Clock*)(ensureNonnull(ctx)));
    return (time_Awake_Inst){ .raw = self->now };
};

$static fn_((test_exec_Coop_Clock__resolution(
    P$raw ctx
))(time_ResolutionE$time_Resolution) $scope) {
    let_ignore = ensureNonnull(ctx);
    return_ok(time_Dur_fromNanos(1));
} $unscoped(fn);

$static fn_((test_exec_Coop_Clock__sleep(
    P$raw ctx, time_Dur dur
))(Sched_Cancelable$void) $scope) {
    let self = ptrCast$((test_exec_Coop_Clock*)(ensureNonnull(ctx)));
    self->now = unwrap_(time_Inst_addChkdDur(self->now, dur));
    return_ok({});
} $unscoped(fn);

$static fn_((test_exec_Coop_Clock_awake(test_exec_Coop_Clock* self))(time_Awake)) {
    $static let_(vtbl, time_Awake_VTbl) = {
        .nowFn = test_exec_Coop_Clock__now,
        .resolutionFn = test_exec_Coop_Clock__resolution,
        .sleepFn = test_exec_Coop_Clock__sleep,
    };
    return time_Awake_ensureValid((time_Awake){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

TEST_fn_("exec/Coop: run drives timed task while current-task scope remains bounded" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var_(clock_state, test_exec_Coop_Clock) = {
        .now = time_Inst_from(0, 0),
    };
    let clock = test_exec_Coop_Clock_awake(&clock_state);
    var coop = exec_Coop_init(heap_Sys_alctr(&heap), clock);
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    var_(observed_running_task, bool) = false;

    try_(TEST_expect(isNone(exec_Coop_task(&coop))));
    var future = Sched_async$u32(sched, clsr_((test_exec_Coop__sleepAndProbe)((test_exec_Coop_Probe){
        .coop = &coop,
        .time = time_Awake_evented(&coop),
        .observed_running_task = &observed_running_task,
    })).as_base);
    let task = ptrAlignCast$((exec_Task*)(unwrap_(future.any_future)));
    try_(TEST_expect(task->state == exec_Task_State_ready));

    exec_Coop_runUntil(&coop, time_Awake_now(clock));
    try_(TEST_expect(!exec_Task_isDone(task)));

    exec_Coop_run(&coop);
    try_(TEST_expect(exec_Task_isDone(task)));
    try_(TEST_expect(observed_running_task));
    try_(TEST_expect(isNone(exec_Coop_task(&coop))));
    try_(TEST_expect(Future_await$u32(&future, sched) == 73));
    try_(TEST_expect(isNone(future.any_future)));
} $unguarded(TEST_fn);

TEST_fn_("exec/Coop: cooperative cancel reaches sleep cancel point" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    var_(clock_state, test_exec_Coop_Clock) = {
        .now = time_Inst_from(0, 0),
    };
    let clock = test_exec_Coop_Clock_awake(&clock_state);
    var coop = exec_Coop_init(heap_Arena_alctr(&arena), clock);
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    let_(sys, test_exec_Coop_Sys) = {
        .sched = sched,
        .time = time_Awake_evented(&coop),
    };
    var task = Sched_async$u32(sched, clsr_((test_exec_Coop__cancelOnSleepCo)(sys)).as_base);
    let child = ptrAlignCast$((exec_Task*)(unwrap_(task.any_future)));
    try_(TEST_expect(!exec_Task_hasCancelRequest(child)));
    try_(TEST_expect(Future_cancel$u32(&task, sched) == 99));
    try_(TEST_expect(Future_await$u32(&task, sched) == 99));
    try_(TEST_expect(child->cancel == exec_Task_Cancel_acknowledged));
    try_(TEST_expect(exec_Task_isDone(child)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Coop: preserves timed progress within stackless and fiber tasks" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    var_(clock_state, test_exec_Coop_Clock) = {
        .now = time_Inst_from(0, 0),
    };
    let clock = test_exec_Coop_Clock_awake(&clock_state);
    var coop = exec_Coop_init(heap_Arena_alctr(&arena), clock);
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    var_(log, test_exec_Coop_EventLog) = {
        .items = A_zero(),
        .len = 0,
    };
    let_(sys, test_exec_Coop_SysLogged) = {
        .sched = sched,
        .time = time_Awake_evented(&coop),
        .log = &log,
    };

    var task_a = Sched_async$test_exec_Coop_Event(
        sched,
        clsr_((test_exec_Coop__countFn)(sys, 2, time_Dur_fromMillis(100), 10)).as_base
    );
    defer_(let_ignore = Future_cancel$test_exec_Coop_Event(&task_a, sched));
    var task_b = Sched_async$test_exec_Coop_Event(
        sched,
        clsr_((test_exec_Coop__countCo)(sys, 3, time_Dur_fromMillis(60), 20)).as_base
    );
    defer_(let_ignore = Future_cancel$test_exec_Coop_Event(&task_b, sched));
    try_(TEST_expect(Future_await$test_exec_Coop_Event(&task_a, sched) == 19));
    try_(TEST_expect(Future_await$test_exec_Coop_Event(&task_b, sched) == 29));

    let expected_a_arr = A_from$((u8){ 10, 11, 12, 19 });
    let expected_b_arr = A_from$((u8){ 20, 21, 22, 23, 29 });
    let expected_a = A_ref$((S_const$u8)(expected_a_arr));
    let expected_b = A_ref$((S_const$u8)(expected_b_arr));
    try_(TEST_expect(log.len == expected_a.len + expected_b.len));
    var_(next_a, usize) = 0;
    var_(next_b, usize) = 0;
    for_(($s(A_prefix((log.items)(log.len))))(event)) {
        if (next_a < expected_a.len && *event == *S_at((expected_a)[next_a])) {
            next_a += 1;
            continue;
        }
        if (next_b < expected_b.len && *event == *S_at((expected_b)[next_b])) {
            next_b += 1;
            continue;
        }
        return_err(E_cause$TEST_Fail());
    } $end(for);
    try_(TEST_expect(next_a == expected_a.len));
    try_(TEST_expect(next_b == expected_b.len));
    return_ok({});
} $unguarded(TEST_fn);
