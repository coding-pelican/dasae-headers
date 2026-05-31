#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Seq.h"
#include "dh/exec/Coop.h"
#include "dh/time/self/Awake.h"
#include "dh/time/Dur.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/io/stream.h"

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(Future, Future_await, Future_cancel, Sched_spawn, Sched_async));

$static fn_((test_exec_Fiber_Stack_report(S_const$u8 label, i32 cnt))(void)) {
    io_stream_println(u8_l("[test_exec_Fiber_Stack: {:s}] cnt: {:d}"), label, cnt);
};

$static fn_((test_exec_Fiber_Stack_count(time_Awake time, time_Dur wait))(i32)) {
    var_(cnt, i32) = 0;
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        catch_((time_Awake_sleepMillis(time, 1))($ignore, $do_nothing));
        cnt++;
    };
    return cnt;
};
fn_use_Clsr_((test_exec_Fiber_Stack_count)(time_Awake, time_Dur)(i32));

$static fn_((test_exec_Fiber_Stack_countReport(S_const$u8 label, time_Awake time, time_Dur wait))(i32)) {
    let cnt = test_exec_Fiber_Stack_count(time, wait);
    test_exec_Fiber_Stack_report(label, cnt);
    return cnt;
};
fn_use_Clsr_((test_exec_Fiber_Stack_countReport)(S_const$u8, time_Awake, time_Dur)(i32));

$static fn_((test_exec_Fiber_Stack_asyncReport(S_const$u8 label, time_Awake time, time_Dur wait))(i32)) {
    return test_exec_Fiber_Stack_countReport(label, time, wait);
};
fn_use_Clsr_((test_exec_Fiber_Stack_asyncReport)(S_const$u8, time_Awake, time_Dur)(i32));

TEST_fn_("exec/Fiber - stack: stackful async await with io from fiber stack" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Seq_init(gpa);
    defer_(exec_Seq_fini(&exec));
    let sched = Sched_seq(&exec);
    let clock = try_(time_Awake_direct());

    var clsr = clsr_((test_exec_Fiber_Stack_asyncReport)(
        u8_l("async"), clock, time_Dur_fromMillis(50)
    ));
    var task = Sched_async$i32(sched, clsr.as_base);
    defer_(Future_cancel$i32(&task, sched));
    try_(TEST_expect(Future_await$i32(&task, sched) >= 0));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Fiber - stack: stackful spawn await returns task result" $guard) {
    pp_if_(pp_not(co_Fiber_supported))(pp_then_(try_(TEST_skip())));
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&exec));
    let sched = Sched_coop(&exec);
    let clock = time_Awake_evented(&exec);

    var clsr = clsr_((test_exec_Fiber_Stack_count)(clock, time_Dur_fromMillis(50)));
    var task = try_(Sched_spawn$i32(sched, clsr.as_base));
    defer_(Future_cancel$i32(&task, sched));
    try_(TEST_expect(Future_await$i32(&task, sched) >= 0));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Fiber - stack: stackful spawns await with io from fiber stack" $guard) {
    pp_if_(pp_not(co_Fiber_supported))(pp_then_(try_(TEST_skip())));
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    var exec = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&exec));
    let sched = Sched_coop(&exec);
    let clock = time_Awake_evented(&exec);
    let wait = time_Dur_fromMillis(50);

    var clsr_a = clsr_((test_exec_Fiber_Stack_countReport)(u8_l("a"), clock, wait));
    var task_a = try_(Sched_spawn$i32(sched, clsr_a.as_base));
    defer_(Future_cancel$i32(&task_a, sched));
    var clsr_b = clsr_((test_exec_Fiber_Stack_countReport)(u8_l("b"), clock, wait));
    var task_b = try_(Sched_spawn$i32(sched, clsr_b.as_base));
    defer_(Future_cancel$i32(&task_b, sched));

    try_(TEST_expect(Future_await$i32(&task_b, sched) >= 0));
    try_(TEST_expect(Future_await$i32(&task_a, sched) >= 0));
    return_ok({});
} $unguarded(TEST_fn);
