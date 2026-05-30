#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/time/Dur.h"
#include "dh/time/self/Awake.h"
#include "dh/thrd/Self.h"
#include "dh/exec/Seq.h"
#include "dh/exec/Coop.h"
#include "dh/io/stream.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), thrd_currId(), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
$static fn_((demoThrd(time_Awake time, time_Dur wait))(i32)) {
    var_(cnt, i32) = 0;
    report(u8_l("demoThrd"), u8_l("starting"));
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        catch_((time_Awake_sleepMillis(time, 1))($ignore, $do_nothing));
        cnt++;
    };
    report(u8_l("demoThrd"), u8_l("cnt: {:d}"), cnt);
    return cnt;
};
fn_use_Clsr_((demoThrd)(time_Awake, time_Dur)(i32));
$static fn_((demoThrdDefer(time_Awake time, time_Dur wait))(i32) $guard) {
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 2")));
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 1")));
    var_(cnt, i32) = 0;
    report(u8_l("demoThrdDefer"), u8_l("starting"));
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        catch_((time_Awake_sleepMillis(time, 1))($ignore, $do_nothing));
        cnt++;
    };
    report(u8_l("demoThrdDefer"), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unguarded(fn);
fn_use_Clsr_((demoThrdDefer)(time_Awake, time_Dur)(i32));

#define sample_co__enable_coop pp_true
$static let sample_co_exec_init = pp_if_(sample_co__enable_coop)(
    pp_then_(exec_Coop_init),
    pp_else_(exec_Seq_init));
$static let sample_co_exec_fini = pp_if_(sample_co__enable_coop)(
    pp_then_(exec_Coop_fini),
    pp_else_(exec_Seq_fini));
$static let sample_co_exec_sched = pp_if_(sample_co__enable_coop)(
    pp_then_(Sched_coop),
    pp_else_(Sched_seq));

T_use$((i32)(Future, Future_await, Future_cancel, Sched_spawn));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    var exec = sample_co_exec_init(pp_if_(sample_co__enable_coop)(
        pp_then_(gpa, try_(time_Awake_direct())),
        pp_else_(gpa)
    ));
    defer_(sample_co_exec_fini(&exec));
    let sched = sample_co_exec_sched(&exec);
    let clock = pp_if_(sample_co__enable_coop)(
        pp_then_(time_Awake_evented(as$(exec_Coop*)(&exec))),
        pp_else_(catch_((time_Awake_direct())($ignore, time_Awake_noop)))
    );

    var demo_thrd = clsr_((demoThrd)(clock, time_Dur_sec));
    var thrd = try_(Sched_spawn$i32(sched, demo_thrd.as_base));
    defer_(Future_cancel$i32(&thrd, sched));

    var demo_thrd_defer = clsr_((demoThrdDefer)(clock, time_Dur_sec));
    var thrd_defer = try_(Sched_spawn$i32(sched, demo_thrd_defer.as_base));
    defer_(Future_cancel$i32(&thrd_defer, sched));

    for_(($r(0, 20))(i)) {
        report(u8_l("main"), u8_l("current: {:uz}"), i);
        try_(time_Awake_sleep(clock, time_Dur_fromSecs$f64(0.1)));
    } $end(for);

    let joined_defer = Future_await$i32(&thrd_defer, sched);
    report(u8_l("main"), u8_l("ret: {:d}"), joined_defer);

    let joined = Future_await$i32(&thrd, sched);
    report(u8_l("main"), u8_l("ret: {:d}"), joined);

    return_ok({});
} $unguarded(fn);
