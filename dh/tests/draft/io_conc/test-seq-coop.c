#include "dh/TEST.h"
#include "dh/ArrList.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/heap/Page.h"

T_alias$((Event)(u8));
T_use$((Event)(P, S));
T_use$((Event)(
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
    var_(time, time_Self);
    var_(log, EventLog*);
}));
$static fn_((countFn(Sys sys, usize n, time_Dur interval, Event base))(Event)) {
    EventLog_push(sys.log, base);
    for_(($rt(n))(i)) {
        catch_((time_sleep(sys.time, interval))($ignore, $do_nothing));
        EventLog_push(sys.log, base + as$(Event)(i + 1));
    } $end(for);
    EventLog_push(sys.log, base + 9);
    return base + 9;
};
T_use$((Event)(Closure_Ctx, Closure_Rtn, Closure));
fn_use_Closure_((countFn)(Sys, usize, time_Dur, Event)(Event));

T_use$((Event)(Co_Ctx, Co_Rtn, Co_Frame));
co_fn_(countCo, (Sys sys; usize n; time_Dur interval; Event base), Event);
co_fn_scope(
    countCo,
    co_locals_({}),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(slept, Void);
    })
) {
    EventLog_push($co_arg(sys).log, $co_arg(base));
    for (co_var_(i, usize) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((slept)(catch_((time_sleep($co_arg(sys).time, $co_arg(interval)))($ignore, $do_nothing))));
        EventLog_push($co_arg(sys).log, $co_arg(base) + as$(u8)($co_mut(i) + 1));
    };
    EventLog_push($co_arg(sys).log, $co_arg(base) + 9);
    co_return_($co_arg(base) + 9);
} $unscoped(co_fn);
co_use_Closure_((countCo)(Sys, usize, time_Dur, Event)(Event));

T_use$((Event)(
    Future,
    Future_await,
    Future_cancel,
    Sched_async
));

$static fn_((runExpectedOrder(Sched sched, time_Self time, S_const$u8 expected))(E$void) $guard) {
    var log = l0$((EventLog));
    let_(sys, Sys) = {
        .time = time,
        .log = &log,
    };
    let async = Sched_async$Event;
    let cancel = Future_cancel$Event;
    let await = Future_await$Event;
    let countFn = closure_(countFn);
    let countCo = closure_(countCo);

    var task_a = async(sched, countFn(sys, 2, time_Dur_fromMillis(100), 10).as_base);
    defer_(let_ignore = cancel(&task_a, sched));
    var task_b = async(sched, countCo(sys, 3, time_Dur_fromMillis(60), 20).as_base);
    defer_(let_ignore = cancel(&task_b, sched));
    try_(TEST_expect(await(&task_a, sched) == 19));
    try_(TEST_expect(await(&task_b, sched) == 29));

    try_(EventLog_expect(&log, expected));
    return_ok({});
} $unguarded(fn);

TEST_fn_("exec_Seq runs fiber and stackless tasks without timed suspension" $guard) {
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    var exec = exec_Seq_init(gpa);
    defer_(exec_Seq_fini(&exec));
    let expected = A_from$((u8){ 10, 11, 12, 19, 20, 21, 22, 23, 29 });
    try_(runExpectedOrder(Sched_seq(&exec), time_direct(), A_ref$((S_const$u8)(expected))));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec_Coop runs evented stackless and fiber tasks in deadline order" $guard) {
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    var exec = exec_Coop_init(gpa, time_direct());
    defer_(exec_Coop_fini(&exec));
    let expected = A_from$((u8){ 10, 20, 21, 11, 22, 23, 29, 12, 19 });
    try_(runExpectedOrder(Sched_coop(&exec), time_evented(&exec), A_ref$((S_const$u8)(expected))));
    return_ok({});
} $unguarded(TEST_fn);
