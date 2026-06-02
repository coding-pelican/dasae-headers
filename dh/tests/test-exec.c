#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/ArrList.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"

T_alias$((Event)(u8));
T_use$((Event)(
    P, S,
    ArrList,
    ArrList_Grip,
    ArrList_grip,
    ArrList_Grip_release,
    ArrList_appendWithin
));

T_alias$((EventLog)(struct EventLog {
    var_(items, A$$(24, Event));
    var_(len, usize);
}));
$static fn_((EventLog_push(EventLog* self, u8 event))(void) $guard) {
    var list = ArrList_grip$Event(A_ref$((S$Event)(self->items)), &self->len);
    defer_(ArrList_Grip_release$Event(&list));
    ArrList_appendWithin$Event(&list.ctx, event);
} $unguarded(fn);
$attr($must_check)
$static fn_((EventLog_expect(EventLog* self, S_const$u8 expected))(E$void) $scope) {
    try_(TEST_expect(self->len == expected.len));
    for_(($s(A_prefix((self->items)(self->len))), $s(expected))(event, expected_event)) {
        try_(TEST_expect(*event == *expected_event));
    } $end(for);
    return_ok({});
} $unscoped(fn);

T_alias$((Sys)(struct Sys {
    var_(sched, Sched);
    var_(time, time_Awake);
}));
T_alias$((SysLogged)(struct SysLogged {
    T_embed$(Sys);
    var_(log, EventLog*);
}));
$static fn_((countFn(SysLogged sys, usize n, time_Dur interval, Event base))(Event)) {
    EventLog_push(sys.log, base);
    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, interval))($ignore, $do_nothing));
        EventLog_push(sys.log, base + as$(Event)(i + 1));
    } $end(for);
    EventLog_push(sys.log, base + 9);
    return base + 9;
};
T_use$((Event)(Clsr_Ctx, Clsr_Rtn, Clsr));
fn_use_Clsr_((countFn)(SysLogged, usize, time_Dur, Event)(Event));

T_use$((Event)(Co_Ctx, Co_Rtn, Co_Frame));
$static co_fn_(countCo, (SysLogged sys; usize n; time_Dur interval; Event base), Event);
co_fn_frame_scope(
    countCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(countCo) {
    EventLog_push($co_arg(sys).log, $co_arg(base));
    for (co_var_(i, usize) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, $co_arg(interval)))($ignore, $do_nothing))));
        EventLog_push($co_arg(sys).log, $co_arg(base) + as$(u8)($co_mut(i) + 1));
    };
    EventLog_push($co_arg(sys).log, $co_arg(base) + 9);
    co_return_($co_arg(base) + 9);
} $unscoped(co_fn);
co_use_Clsr_((countCo)(SysLogged, usize, time_Dur, Event)(Event));

T_use$((u32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((u32)(Future, Future_await, Future_cancel, Sched_async));
$static co_fn_(cancelOnSleepCo, (Sys sys), u32);
co_fn_frame_scope(
    cancelOnSleepCo,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(cancelOnSleepCo) {
    suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, time_Dur_fromMillis(5000)))(
        $ignore, $do_nothing
    ))));
    catch_((Sched_idle($co_arg(sys).sched))(err, switch (E_tag$Sched_Cancelable(err)) {
        case_((E_Tag$Sched_Canceled)) co_return_(99) $end(case);
        case (E_Tag$Any): claim_unreachable;
    }));
    co_return_(77);
} $unscoped(co_fn);
co_use_Clsr_((cancelOnSleepCo)(Sys)(u32));

$static co_fn_(cancelPollSeqCo, (Sys sys), u32);
co_fn_frame_scope(
    cancelPollSeqCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(cancelPollSeqCo) {
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
co_use_Clsr_((cancelPollSeqCo)(Sys)(u32));

TEST_fn_("exec/Task: idle consumes request; recancel re-arms" $guard) {
    var_(task, exec_Task) = {
        .state = exec_Task_State_running,
        .cancel = exec_Task_Cancel_none,
        .cancel_protection = Sched_CancelProtn_unblocked,
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

T_use$((Event)(Future, Future_await, Future_cancel, Sched_async));
$static fn_((runExpectedOrder(Sched sched, time_Awake time, S_const$u8 expected))(E$void) $guard) {
    var_(log, EventLog) = {
        .items = A_zero(),
        .len = 0,
    };
    let_(sys, SysLogged) = {
        .sched = sched,
        .time = time,
        .log = &log,
    };
    let async = Sched_async$Event;
    let cancel = Future_cancel$Event;
    let await = Future_await$Event;

    var task_a = async(sched, clsr_((countFn)(sys, 2, time_Dur_fromMillis(100), 10)).as_base);
    defer_(let_ignore = cancel(&task_a, sched));
    var task_b = async(sched, clsr_((countCo)(sys, 3, time_Dur_fromMillis(60), 20)).as_base);
    defer_(let_ignore = cancel(&task_b, sched));
    try_(TEST_expect(await(&task_a, sched) == 19));
    try_(TEST_expect(await(&task_b, sched) == 29));

    try_(EventLog_expect(&log, expected));
    return_ok({});
} $unguarded(fn);

TEST_fn_("exec/Seq: cooperative cancel reaches co step after direct sleep" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var seq = exec_Seq_init(gpa);
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    let_(sys, Sys) = {
        .sched = sched,
        .time = try_(time_Awake_direct()),
    };
    var task = Sched_async$u32(sched, clsr_((cancelPollSeqCo)(sys)).as_base);
    let_ignore = Future_cancel$u32(&task, sched);
    try_(TEST_expect(Future_await$u32(&task, sched) == 88));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Seq: runs fiber and stackless tasks without timed suspension" $guard) {
    var heap = heap_Sys_init();
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
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var coop = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    let_(sys, Sys) = {
        .sched = sched,
        .time = time_Awake_evented(&coop),
    };
    var clsr = clsr_((cancelOnSleepCo)(sys));
    var task = Sched_async$u32(sched, clsr.as_base);
    let child = ptrAlignCast$((exec_Task*)(unwrap_(task.any_future)));
    try_(TEST_expect(!exec_Task_hasCancelRequest(child)));
    try_(TEST_expect(Future_cancel$u32(&task, sched) == 99));
    try_(TEST_expect(Future_await$u32(&task, sched) == 99));
    try_(TEST_expect(child->cancel == exec_Task_Cancel_acknowledged));
    try_(TEST_expect(exec_Task_isDone(child)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Coop: runs evented stackless and fiber tasks in deadline order" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&exec));
    let expected = A_from$((u8){ 10, 20, 21, 11, 22, 23, 29, 12, 19 });
    try_(runExpectedOrder(Sched_coop(&exec), time_Awake_evented(&exec), A_ref$((S_const$u8)(expected))));
    return_ok({});
} $unguarded(TEST_fn);
