#include "dh-main.h"
#include "dh/io/stream.h"
#include "dh/thrd/Self.h"
#include "dh/time.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/exec/Coop.h"

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

T_alias$((Sys)(struct Sys {
    var_(time, time_Awake);
}));
$static fn_((countFn(Sys sys, usize n, f64 interval_secs, S_const$u8 label))(f64)) {
    let instant = time_Awake_now(sys.time);
    report(label, u8_l("before loop {:.1fl}"), interval_secs);

    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, time_Dur_fromSecs$f64(interval_secs)))($ignore, $do_nothing));
        report(label, u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"), interval_secs, i, n);
    } $end(for);

    let elapsed_secs = pipe_((instant)(
        (t)(time_Awake_Inst_elapsed(t, sys.time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report(label, u8_l("after loop {:.1fl}"), elapsed_secs);
    return elapsed_secs;
};
T_use$((f64)(Clsr_Ctx, Clsr_Rtn, Clsr));
fn_use_Clsr_((countFn)(Sys, usize, f64, S_const$u8)(f64));

T_use$((f64)(Co_Ctx, Co_Rtn, Co_Frame));
$static co_fn_(countCo, (Sys sys; usize n; f64 interval_secs; S_const$u8 label), f64);
co_fn_frame_scope(
    countCo,
    co_locals_({
        var_(instant, time_Awake_Inst);
        var_(elapsed_secs, f64);
    }),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(sleeping, Void);
    })
);
co_fn_scope(countCo) {
    co_let_(instant) = time_Awake_now($co_arg(sys).time);
    report($co_arg(label), u8_l("before loop {:.1fl}"), $co_arg(interval_secs));

    for (co_var_(i) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep(
            $co_arg(sys).time, time_Dur_fromSecs$f64($co_arg(interval_secs))
        ))($ignore, $do_nothing))));
        report(
            $co_arg(label), u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"),
            $co_arg(interval_secs), $co_mut(i), $co_arg(n)
        );
    }

    co_let_(elapsed_secs) = pipe_(($co(instant))(
        (t)(time_Awake_Inst_elapsed(t, $co_arg(sys).time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report($co_arg(label), u8_l("after loop {:.1fl}"), $co(elapsed_secs));
    co_return_($co(elapsed_secs));
} $unscoped(co_fn);
co_use_Clsr_((countCo)(Sys, usize, f64, S_const$u8)(f64));

/*
begin
[ThrdId(0): task a] before loop 1.0
[ThrdId(0): task b] before loop 0.6
[ThrdId(0): task b] slept 0.6 | i: 0 < n: 3
[ThrdId(0): task a] slept 1.0 | i: 0 < n: 2
[ThrdId(0): task b] slept 0.6 | i: 1 < n: 3
[ThrdId(0): task b] slept 0.6 | i: 2 < n: 3
[ThrdId(0): task b] after loop 1.8
[ThrdId(0): task a] slept 1.0 | i: 1 < n: 2
[ThrdId(0): task a] after loop 2.0
end
task elapsed sum: 3.8
wall elapsed: 2.0
*/
T_use$((f64)(Future, Future_await, Future_cancel, Sched_async));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    let direct_clock = try_(time_Awake_direct());
    var loop = exec_Coop_init(gpa, direct_clock);
    defer_(exec_Coop_fini(&loop));
    let sched = Sched_coop(&loop);
    let_(sys, Sys) = {
        .time = time_Awake_evented(&loop),
    };

    var task_elapsed_sum = 0.0;
    let run_start = time_Awake_now(direct_clock);
    using_() blk_defer {
        io_stream_println(u8_l("begin - evented async execution"));
        var task_a = Sched_async$f64(sched, clsr_((countFn)(sys, 2, 1.0, u8_l("task a"))).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_a, sched));
        var task_b = Sched_async$f64(sched, clsr_((countCo)(sys, 3, 0.6, u8_l("task b"))).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_b, sched));
        task_elapsed_sum += Future_await$f64(&task_a, sched);
        task_elapsed_sum += Future_await$f64(&task_b, sched);
        io_stream_println(u8_l("end - evented async execution"));
    } blk_deferral;
    let wall_elapsed_secs = pipe_((run_start)(
        (t)(time_Awake_Inst_elapsed(t, direct_clock)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    io_stream_println(u8_l("task elapsed sum: {:.1fl}"), task_elapsed_sum);
    io_stream_println(u8_l("wall elapsed: {:.1fl}"), wall_elapsed_secs);

    return_ok({});
} $unguarded(fn);
