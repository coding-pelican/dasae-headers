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

/* --- 16 / 24 / 32-byte struct args across evented fiber yield (exec_Lane_yield path) --- */

T_alias$((test_exec_Fiber_Stack_Probe16)(struct test_exec_Fiber_Stack_Probe16 {
    var_(w0, u64);
    var_(w1, u64);
}));
claim_assert_static(sizeOf$(test_exec_Fiber_Stack_Probe16) == usize_(16));

T_alias$((test_exec_Fiber_Stack_Probe24)(struct test_exec_Fiber_Stack_Probe24 {
    var_(w0, u64);
    var_(w1, u64);
    var_(w2, u64);
}));
claim_assert_static(sizeOf$(test_exec_Fiber_Stack_Probe24) == usize_(24));

T_alias$((test_exec_Fiber_Stack_Probe32)(struct test_exec_Fiber_Stack_Probe32 {
    var_(w0, u64);
    var_(w1, u64);
    var_(w2, u64);
    var_(w3, u64);
}));
claim_assert_static(sizeOf$(test_exec_Fiber_Stack_Probe32) == usize_(32));

#define test_exec_Fiber_Stack_argProbe_yield_count u32_(32)

$static fn_((test_exec_Fiber_Stack_argProbe_expect(
    test_exec_Fiber_Stack_Probe16 p16,
    test_exec_Fiber_Stack_Probe24 p24,
    test_exec_Fiber_Stack_Probe32 p32
))(bool)) {
    return p16.w0 == u64_(0xA16A16A16A16A160) && p16.w1 == u64_(0xA16A16A16A16A161)
        && p24.w0 == u64_(0xB24B24B24B24B240) && p24.w1 == u64_(0xB24B24B24B24B241)
        && p24.w2 == u64_(0xB24B24B24B24B242)
        && p32.w0 == u64_(0xC32C32C32C32C320) && p32.w1 == u64_(0xC32C32C32C32C321)
        && p32.w2 == u64_(0xC32C32C32C32C322) && p32.w3 == u64_(0xC32C32C32C32C323);
};

$static fn_((test_exec_Fiber_Stack_argProbeYield(
    test_exec_Fiber_Stack_Probe16 p16,
    test_exec_Fiber_Stack_Probe24 p24,
    test_exec_Fiber_Stack_Probe32 p32,
    time_Awake time
))(bool)) {
    while_(
        var_(i, u32) = 0,
        i < test_exec_Fiber_Stack_argProbe_yield_count,
        ++i
    ) {
        if (!test_exec_Fiber_Stack_argProbe_expect(p16, p24, p32)) return false;
        catch_((time_Awake_sleepMillis(time, 1))($ignore, return false));
    };
    return test_exec_Fiber_Stack_argProbe_expect(p16, p24, p32);
};

$static fn_((test_exec_Fiber_Stack_argProbeTask(
    test_exec_Fiber_Stack_Probe16 p16,
    test_exec_Fiber_Stack_Probe24 p24,
    test_exec_Fiber_Stack_Probe32 p32,
    time_Awake time
))(i32)) {
    return test_exec_Fiber_Stack_argProbeYield(p16, p24, p32, time) ? 1 : 0;
};
fn_use_Clsr_((
    test_exec_Fiber_Stack_argProbeTask
)(test_exec_Fiber_Stack_Probe16, test_exec_Fiber_Stack_Probe24, test_exec_Fiber_Stack_Probe32, time_Awake)(i32));

TEST_fn_("exec/Fiber - stack: evented yield preserves 16-, 24-, and 32-byte struct arguments" $guard) {
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

    var clsr = clsr_((test_exec_Fiber_Stack_argProbeTask)(
        (test_exec_Fiber_Stack_Probe16){
            .w0 = u64_(0xA16A16A16A16A160),
            .w1 = u64_(0xA16A16A16A16A161),
        },
        (test_exec_Fiber_Stack_Probe24){
            .w0 = u64_(0xB24B24B24B24B240),
            .w1 = u64_(0xB24B24B24B24B241),
            .w2 = u64_(0xB24B24B24B24B242),
        },
        (test_exec_Fiber_Stack_Probe32){
            .w0 = u64_(0xC32C32C32C32C320),
            .w1 = u64_(0xC32C32C32C32C321),
            .w2 = u64_(0xC32C32C32C32C322),
            .w3 = u64_(0xC32C32C32C32C323),
        },
        clock
    ));
    var task = try_(Sched_spawn$i32(sched, clsr.as_base));
    defer_(Future_cancel$i32(&task, sched));
    try_(TEST_expect(Future_await$i32(&task, sched) == 1));
    return_ok({});
} $unguarded(TEST_fn);

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
