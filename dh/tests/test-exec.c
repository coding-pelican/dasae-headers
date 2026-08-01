#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/ArrList.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/io/stream.h"

T_alias$((test_exec_Event)(u8));
T_use$((test_exec_Event)(
    P, S,
    ArrList,
    ArrList_Grip,
    ArrList_grip,
    ArrList_Grip_release,
    ArrList_appendWithin
));

T_alias$((test_exec_EventLog)(struct test_exec_EventLog {
    var_(items, A$$(24, test_exec_Event));
    var_(len, usize);
}));
$static fn_((test_exec_EventLog_push(test_exec_EventLog* self, u8 event))(void) $guard) {
    var list = ArrList_grip$test_exec_Event(A_ref$((S$test_exec_Event)(self->items)), &self->len);
    defer_(ArrList_Grip_release$test_exec_Event(&list));
    ArrList_appendWithin$test_exec_Event(&list.ctx, event);
} $unguarded(fn);
$static fn_((test_exec_EventLog_dumpMismatch(test_exec_EventLog* self, S_const$u8 expected))(void)) {
    let actual = A_prefix((self->items)(self->len));
    io_stream_println(u8_l("    event log len: actual={:uz} expected={:uz}"), self->len, expected.len);
    let span = pri_max(self->len, expected.len);
    for_(($rt(span))(i)) {
        let actual_event = i < self->len ? *S_at((actual)[i]) : u8_(0xFF);
        let expected_event = i < expected.len ? *S_at((expected)[i]) : u8_(0xFF);
        if (actual_event == expected_event) continue;
        io_stream_println(u8_l("    [{:uz}] actual={:d} expected={:d}"), i, as$(u32)(actual_event), as$(u32)(expected_event));
    } $end(for);
    io_stream_print(u8_l("    actual:   "));
    for_(($s(actual))(event)) { io_stream_print(u8_l("{:d} "), as$(u32)(*event)); } $end(for);
    io_stream_nl();
    io_stream_print(u8_l("    expected: "));
    for_(($s(expected))(event)) { io_stream_print(u8_l("{:d} "), as$(u32)(*event)); } $end(for);
    io_stream_nl();
}
$static fn_((test_exec_EventLog_dumpInterleavingMismatch(
    test_exec_EventLog* self, S_const$u8 expected_a, S_const$u8 expected_b
))(void)) {
    let actual = A_prefix((self->items)(self->len));
    io_stream_println(u8_l("    event log len: actual={:uz} expected={:uz}"), self->len, expected_a.len + expected_b.len);
    io_stream_print(u8_l("    actual:     "));
    for_(($s(actual))(event)) { io_stream_print(u8_l("{:d} "), as$(u32)(*event)); } $end(for);
    io_stream_nl();
    io_stream_print(u8_l("    expected a: "));
    for_(($s(expected_a))(event)) { io_stream_print(u8_l("{:d} "), as$(u32)(*event)); } $end(for);
    io_stream_nl();
    io_stream_print(u8_l("    expected b: "));
    for_(($s(expected_b))(event)) { io_stream_print(u8_l("{:d} "), as$(u32)(*event)); } $end(for);
    io_stream_nl();
}
$attr($must_check)
$static fn_((test_exec_EventLog_expect(test_exec_EventLog* self, S_const$u8 expected))(E$void) $scope) {
    if (self->len != expected.len) test_exec_EventLog_dumpMismatch(self, expected);
    try_(TEST_expect(self->len == expected.len));
    for_(($s(A_prefix((self->items)(self->len))), $s(expected))(event, expected_event)) {
        if (*event != *expected_event) test_exec_EventLog_dumpMismatch(self, expected);
        try_(TEST_expect(*event == *expected_event));
    } $end(for);
    return_ok({});
} $unscoped(fn);
$attr($must_check)
$static fn_((test_exec_EventLog_expectInterleaving(
    test_exec_EventLog* self, S_const$u8 expected_a, S_const$u8 expected_b
))(E$void) $scope) {
    if (self->len != expected_a.len + expected_b.len) {
        test_exec_EventLog_dumpInterleavingMismatch(self, expected_a, expected_b);
    }
    try_(TEST_expect(self->len == expected_a.len + expected_b.len));

    var_(next_a, usize) = 0;
    var_(next_b, usize) = 0;
    for_(($s(A_prefix((self->items)(self->len))))(event)) {
        if (next_a < expected_a.len && *event == *S_at((expected_a)[next_a])) {
            next_a += 1;
            continue;
        }
        if (next_b < expected_b.len && *event == *S_at((expected_b)[next_b])) {
            next_b += 1;
            continue;
        }
        test_exec_EventLog_dumpInterleavingMismatch(self, expected_a, expected_b);
        return_err(E_cause$TEST_Fail());
    } $end(for);

    if (next_a != expected_a.len || next_b != expected_b.len) {
        test_exec_EventLog_dumpInterleavingMismatch(self, expected_a, expected_b);
    }
    try_(TEST_expect(next_a == expected_a.len));
    try_(TEST_expect(next_b == expected_b.len));
    return_ok({});
} $unscoped(fn);

T_alias$((test_exec_Sys)(struct test_exec_Sys {
    var_(sched, Sched);
    var_(time, time_Awake);
}));
T_alias$((test_exec_SysLogged)(struct test_exec_SysLogged {
    T_embed$(test_exec_Sys);
    var_(log, test_exec_EventLog*);
}));
$static fn_((test_exec_countFn(test_exec_SysLogged sys, usize n, time_Dur interval, test_exec_Event base))(test_exec_Event)) {
    test_exec_EventLog_push(sys.log, base);
    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, interval))($ignore, $do_nothing));
        test_exec_EventLog_push(sys.log, base + as$(test_exec_Event)(i + 1));
    } $end(for);
    test_exec_EventLog_push(sys.log, base + 9);
    return base + 9;
};
T_use$((test_exec_Event)(Clsr_Ctx, Clsr_Rtn, Clsr));
fn_use_Clsr_((test_exec_countFn)(test_exec_SysLogged, usize, time_Dur, test_exec_Event)(test_exec_Event));

T_use$((test_exec_Event)(Co_Ctx, Co_Rtn, Co_Frame));
$static co_fn_(test_exec_countCo, (test_exec_SysLogged sys; usize n; time_Dur interval; test_exec_Event base), test_exec_Event);
co_fn_frame_scope(
    test_exec_countCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_countCo) {
    test_exec_EventLog_push($co_arg(sys).log, $co_arg(base));
    for (co_var_(i, usize) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, $co_arg(interval)))($ignore, $do_nothing))));
        test_exec_EventLog_push($co_arg(sys).log, $co_arg(base) + as$(u8)($co_mut(i) + 1));
    };
    test_exec_EventLog_push($co_arg(sys).log, $co_arg(base) + 9);
    co_return_($co_arg(base) + 9);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_countCo)(test_exec_SysLogged, usize, time_Dur, test_exec_Event)(test_exec_Event));

T_use$((u32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((u32)(Future, Future_await, Future_cancel, Sched_async, Sched_spawn));
$static fn_((test_exec_addOneFn(u32 value))(u32)) {
    return value + 1;
};
fn_use_Clsr_((test_exec_addOneFn)(u32)(u32));

$static fn_((test_exec_sleepThenReturnFn(time_Awake time, time_Dur dur, u32 value))(u32)) {
    catch_((time_Awake_sleep(time, dur))($ignore, $do_nothing));
    return value;
};
fn_use_Clsr_((test_exec_sleepThenReturnFn)(time_Awake, time_Dur, u32)(u32));

$static co_fn_(test_exec_cancelOnSleepCo, (test_exec_Sys sys), u32);
co_fn_frame_scope(
    test_exec_cancelOnSleepCo,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_cancelOnSleepCo) {
    suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, time_Dur_fromMillis(5000)))(
        $ignore, $do_nothing
    ))));
    catch_((Sched_idle($co_arg(sys).sched))(err, switch (E_tag$Sched_Cancelable(err)) {
        case_((E_Tag$Sched_Canceled)) co_return_(99) $end(case);
        case (E_Tag$Any): claim_unreachable;
    }));
    co_return_(77);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_cancelOnSleepCo)(test_exec_Sys)(u32));

$static co_fn_(test_exec_cancelPollSeqCo, (test_exec_Sys sys), u32);
co_fn_frame_scope(
    test_exec_cancelPollSeqCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(test_exec_cancelPollSeqCo) {
    for (co_var_(i, usize) = 0; $co_mut(i) < 8; ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, time_Dur_fromMillis(50)))(
            $ignore, $do_nothing
        ))));
        catch_((Sched_idle($co_arg(sys).sched))(err, switch (E_tag$Sched_Cancelable(err)) {
            case_((E_Tag$Sched_Canceled)) co_return_(88) $end(case);
            case (E_Tag$Any): claim_unreachable;
        }));
    }
    co_return_(44);
} $unscoped(co_fn);
co_use_Clsr_((test_exec_cancelPollSeqCo)(test_exec_Sys)(u32));

TEST_fn_("exec/Task: idle consumes request; recancel re-arms" $guard) {
    var_(task, exec_Task) = {
        .state = exec_Task_State_running,
        .cancel = exec_Task_Cancel_none,
        .cancel_protection = Sched_CancelProtcn_unblocked,
        .result = cleared(),
        .inner = cleared(),
        .fiber = none(),
    };

    try_(TEST_expect(!exec_Task_hasCancelRequest(&task)));
    exec_Task_requestCancel(&task);
    try_(TEST_expect(exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(isErr(exec_Task_idle(&task))));
    try_(TEST_expect(!exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(task.cancel == exec_Task_Cancel_acknowledged));
    try_(TEST_expect(isOk(exec_Task_idle(&task))));
    exec_Task_recancel(&task);
    try_(TEST_expect(exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(isErr(exec_Task_idle(&task))));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("Sched/Future: seq async owns result until await consumes future handle" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var seq = exec_Seq_init(heap_Sys_alctr(&heap));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);

    var future = Sched_async$u32(sched, clsr_((test_exec_addOneFn)(41)).as_base);
    try_(TEST_expect(isSome(future.any_future)));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
    try_(TEST_expect(isNone(future.any_future)));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
} $unguarded(TEST_fn);

TEST_fn_("Sched/Future: cancel consumes future handle and yields task result storage" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var seq = exec_Seq_init(heap_Sys_alctr(&heap));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);

    var future = Sched_async$u32(sched, clsr_((test_exec_addOneFn)(9)).as_base);
    try_(TEST_expect(isSome(future.any_future)));
    let_ignore = Future_cancel$u32(&future, sched);
    try_(TEST_expect(isNone(future.any_future)));
    let_ignore = Future_cancel$u32(&future, sched);
} $unguarded(TEST_fn);

TEST_fn_("Sched: failing scheduler reports unavailable spawn and canceled idle" $guard) {
    if_err((Sched_spawn$u32(Sched_failing, clsr_((test_exec_addOneFn)(1)).as_base))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_ConcUnavailable().as_any)));
    } else_ok(future) {
        let_ignore = future;
        try_(TEST_expect(false));
    }

    if_err((Sched_idle(Sched_failing))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Canceled().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unguarded(TEST_fn);

TEST_fn_("exec/Preem: await joins spawned thread and copies result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);

    var clsr = clsr_((test_exec_addOneFn)(41));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Preem: cancel joins running thread and discards task result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);
    let time = try_(time_Awake_direct());

    var clsr = clsr_((test_exec_sleepThenReturnFn)(time, time_Dur_fromMillis(25), 123));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    try_(TEST_expect(Future_cancel$u32(&future, sched) == 0));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Preem: cancel after completion joins thread and preserves result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);
    let time = try_(time_Awake_direct());

    var clsr = clsr_((test_exec_sleepThenReturnFn)(time, time_Dur_fromMillis(1), 321));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    catch_((time_Awake_sleep(time, time_Dur_fromMillis(25)))($ignore, $do_nothing));
    try_(TEST_expect(Future_cancel$u32(&future, sched) == 321));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);

T_use$((test_exec_Event)(Future, Future_await, Future_cancel, Sched_async));
$static fn_((runLoggedTasks(Sched sched, time_Awake time, test_exec_EventLog* log))(E$void) $guard) {
    let_(sys, test_exec_SysLogged) = {
        .sched = sched,
        .time = time,
        .log = log,
    };
    let async = Sched_async$test_exec_Event;
    let cancel = Future_cancel$test_exec_Event;
    let await = Future_await$test_exec_Event;

    var task_a = async(sched, clsr_((test_exec_countFn)(sys, 2, time_Dur_fromMillis(100), 10)).as_base);
    defer_(let_ignore = cancel(&task_a, sched));
    var task_b = async(sched, clsr_((test_exec_countCo)(sys, 3, time_Dur_fromMillis(60), 20)).as_base);
    defer_(let_ignore = cancel(&task_b, sched));
    try_(TEST_expect(await(&task_a, sched) == 19));
    try_(TEST_expect(await(&task_b, sched) == 29));

    return_ok({});
} $unguarded(fn);
$static fn_((runExpectedOrder(Sched sched, time_Awake time, S_const$u8 expected))(E$void) $guard) {
    var_(log, test_exec_EventLog) = {
        .items = A_zero(),
        .len = 0,
    };
    try_(runLoggedTasks(sched, time, &log));

    try_(test_exec_EventLog_expect(&log, expected));
    return_ok({});
} $unguarded(fn);

TEST_fn_("exec/Seq: cooperative cancel reaches co step after direct sleep" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var seq = exec_Seq_init(gpa);
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    let_(sys, test_exec_Sys) = {
        .sched = sched,
        .time = try_(time_Awake_direct()),
    };
    var task = Sched_async$u32(sched, clsr_((test_exec_cancelPollSeqCo)(sys)).as_base);
    let_ignore = Future_cancel$u32(&task, sched);
    try_(TEST_expect(Future_await$u32(&task, sched) == 88));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Seq: runs fiber and stackless tasks without timed suspension" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Seq_init(gpa);
    defer_(exec_Seq_fini(&exec));
    let expected = A_from$((u8){ 10, 11, 12, 19, 20, 21, 22, 23, 29 });
    try_(runExpectedOrder(Sched_seq(&exec), try_(time_Awake_direct()), A_ref$((S_const$u8)(expected))));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Coop: cooperative cancel reaches sleep cancel point" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var coop = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    let_(sys, test_exec_Sys) = {
        .sched = sched,
        .time = time_Awake_evented(&coop),
    };
    var clsr = clsr_((test_exec_cancelOnSleepCo)(sys));
    var task = Sched_async$u32(sched, clsr.as_base);
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
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&exec));
    var_(log, test_exec_EventLog) = {
        .items = A_zero(),
        .len = 0,
    };
    let sched = Sched_coop(&exec);
    try_(runLoggedTasks(sched, time_Awake_evented(&exec), &log));

    let expected_a = A_from$((u8){ 10, 11, 12, 19 });
    let expected_b = A_from$((u8){ 20, 21, 22, 23, 29 });
    try_(test_exec_EventLog_expectInterleaving(
        &log,
        A_ref$((S_const$u8)(expected_a)),
        A_ref$((S_const$u8)(expected_b))
    ));
    return_ok({});
} $unguarded(TEST_fn);
