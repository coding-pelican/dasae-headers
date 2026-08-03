#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Seq.h"
#include "dh/time.h"
#include "dh/ArrList.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"

T_alias$((test_exec_Seq_Event)(u8));
T_use$((test_exec_Seq_Event)(
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

T_alias$((test_exec_Seq_EventLog)(struct test_exec_Seq_EventLog {
    var_(items, A$$(24, test_exec_Seq_Event));
    var_(len, usize);
}));
$static fn_((test_exec_Seq__push(test_exec_Seq_EventLog* self, test_exec_Seq_Event event))(void) $guard) {
    var list = ArrList_grip$test_exec_Seq_Event(
        A_ref$((S$test_exec_Seq_Event)(self->items)),
        &self->len
    );
    defer_(ArrList_Grip_release$test_exec_Seq_Event(&list));
    ArrList_appendWithin$test_exec_Seq_Event(&list.ctx, event);
} $unguarded(fn);

T_alias$((test_exec_Seq_Sys)(struct test_exec_Seq_Sys {
    var_(sched, Sched);
    var_(time, time_Awake);
}));
T_alias$((test_exec_Seq_SysLogged)(struct test_exec_Seq_SysLogged {
    T_embed$(test_exec_Seq_Sys);
    var_(log, test_exec_Seq_EventLog*);
}));
$static fn_((test_exec_Seq__countFn(
    test_exec_Seq_SysLogged sys, usize n, time_Dur interval, test_exec_Seq_Event base
))(test_exec_Seq_Event)) {
    test_exec_Seq__push(sys.log, base);
    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, interval))($ignore, $do_nothing));
        test_exec_Seq__push(sys.log, base + as$(test_exec_Seq_Event)(i + 1));
    } $end(for);
    test_exec_Seq__push(sys.log, base + 9);
    return base + 9;
};
fn_use_Clsr_((test_exec_Seq__countFn)(
    test_exec_Seq_SysLogged, usize, time_Dur, test_exec_Seq_Event
)(test_exec_Seq_Event));

$static co_fn_(test_exec_Seq__countCo, (
    test_exec_Seq_SysLogged sys;
    usize n;
    time_Dur interval;
    test_exec_Seq_Event base
), test_exec_Seq_Event);
co_fn_frame_scope(
    test_exec_Seq__countCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_Seq__countCo) {
    test_exec_Seq__push($co_arg(sys).log, $co_arg(base));
    for (co_var_(i, usize) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, $co_arg(interval)))(
            $ignore, $do_nothing
        ))));
        test_exec_Seq__push(
            $co_arg(sys).log,
            $co_arg(base) + as$(test_exec_Seq_Event)($co_mut(i) + 1)
        );
    };
    test_exec_Seq__push($co_arg(sys).log, $co_arg(base) + 9);
    co_return_($co_arg(base) + 9);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_Seq__countCo)(
    test_exec_Seq_SysLogged, usize, time_Dur, test_exec_Seq_Event
)(test_exec_Seq_Event));

T_use$((u32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((u32)(Future, Future_await, Future_cancel, Sched_async));
$static co_fn_(test_exec_Seq__cancelPollCo, (test_exec_Seq_Sys sys), u32);
co_fn_frame_scope(
    test_exec_Seq__cancelPollCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_Seq__cancelPollCo) {
    for (co_var_(i, usize) = 0; $co_mut(i) < 8; ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep(
            $co_arg(sys).time,
            time_Dur_fromMillis(50)
        ))($ignore, $do_nothing))));
        catch_((Sched_idle($co_arg(sys).sched))(err, switch (E_tag$Sched_Cancelable(err)) {
            case_((E_Tag$Sched_Canceled)) co_return_(88) $end(case);
            case (E_Tag$Any): claim_unreachable;
        }));
    }
    co_return_(44);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_Seq__cancelPollCo)(test_exec_Seq_Sys)(u32));

T_alias$((test_exec_Seq_Probe)(struct test_exec_Seq_Probe {
    var_(seq, exec_Seq*);
    var_(observed_running_task, bool*);
}));

$static fn_((test_exec_Seq__probeTask(test_exec_Seq_Probe probe))(u32)) {
    if_some((exec_Seq_task(probe.seq))(task)) {
        *P_at((probe.observed_running_task)[0]) = task->state == exec_Task_State_running;
    }
    return 42;
};
fn_use_Clsr_((test_exec_Seq__probeTask)(test_exec_Seq_Probe)(u32));

TEST_fn_("exec/Seq: await drives ready task with scoped current-task introspection" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var seq = exec_Seq_init(heap_Sys_alctr(&heap));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    var_(observed_running_task, bool) = false;

    try_(TEST_expect(isNone(exec_Seq_task(&seq))));
    var future = Sched_async$u32(sched, clsr_((test_exec_Seq__probeTask)((test_exec_Seq_Probe){
        .seq = &seq,
        .observed_running_task = &observed_running_task,
    })).as_base);
    try_(TEST_expect(isSome(future.any_future)));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
    try_(TEST_expect(observed_running_task));
    try_(TEST_expect(isNone(exec_Seq_task(&seq))));
    try_(TEST_expect(isNone(future.any_future)));
} $unguarded(TEST_fn);

TEST_fn_("exec/Seq: cooperative cancel reaches co step after direct sleep" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    var seq = exec_Seq_init(heap_Arena_alctr(&arena));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    let_(sys, test_exec_Seq_Sys) = {
        .sched = sched,
        .time = try_(time_Awake_direct()),
    };
    var task = Sched_async$u32(sched, clsr_((test_exec_Seq__cancelPollCo)(sys)).as_base);
    let_ignore = Future_cancel$u32(&task, sched);
    try_(TEST_expect(Future_await$u32(&task, sched) == 88));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Seq: runs fiber and stackless tasks without timed suspension" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    var seq = exec_Seq_init(heap_Arena_alctr(&arena));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    let time = try_(time_Awake_direct());
    var_(log, test_exec_Seq_EventLog) = {
        .items = A_zero(),
        .len = 0,
    };
    let_(sys, test_exec_Seq_SysLogged) = {
        .sched = sched,
        .time = time,
        .log = &log,
    };

    var task_a = Sched_async$test_exec_Seq_Event(
        sched,
        clsr_((test_exec_Seq__countFn)(sys, 2, time_Dur_fromMillis(100), 10)).as_base
    );
    defer_(let_ignore = Future_cancel$test_exec_Seq_Event(&task_a, sched));
    var task_b = Sched_async$test_exec_Seq_Event(
        sched,
        clsr_((test_exec_Seq__countCo)(sys, 3, time_Dur_fromMillis(60), 20)).as_base
    );
    defer_(let_ignore = Future_cancel$test_exec_Seq_Event(&task_b, sched));
    try_(TEST_expect(Future_await$test_exec_Seq_Event(&task_a, sched) == 19));
    try_(TEST_expect(Future_await$test_exec_Seq_Event(&task_b, sched) == 29));

    let expected_arr = A_from$((u8){ 10, 11, 12, 19, 20, 21, 22, 23, 29 });
    let expected = A_ref$((S_const$u8)(expected_arr));
    try_(TEST_expect(log.len == expected.len));
    for_((
        $s(A_prefix((log.items)(log.len))),
        $s(expected)
    )(event, expected_event)) {
        try_(TEST_expect(*event == *expected_event));
    } $end(for);
    return_ok({});
} $unguarded(TEST_fn);
