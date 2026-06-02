#include "dh-main.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/io/stream.h"
#include "dh/heap/Sys.h"

$static fn_((report(io_Self io, S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(io, u8_l("[{:s}] "), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(io, fmt, args);
    }
    io_stream_nl(io);
};


T_alias$((Sys)(struct Sys {
    var_(time, time_Awake);
    var_(io, io_Self);
}));

$static fn_((countFn(Sys sys, usize n, f64 interval_secs, S_const$u8 label))(f64)) {
    let instant = time_Awake_now(sys.time);
    report(sys.io, label, u8_l("before loop {:.1fl}"), interval_secs);

    for_(($rt(n))(i)) {
        catch_((time_Awake_sleep(sys.time, time_Dur_fromSecs$f64(interval_secs)))($ignore, $do_nothing));
        report(sys.io, label, u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"), interval_secs, i, n);
    } $end(for);

    let elapsed = pipe_((instant)(
        (t)(time_Awake_Inst_elapsed(t, sys.time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report(sys.io, label, u8_l("after loop {:.1fl}"), elapsed);
    return elapsed;
};
T_use$((f64)(Clsr_Ctx, Clsr_Rtn, Clsr));
fn_use_Clsr_((countFn)(Sys, usize, f64, S_const$u8)(f64));

T_use$((f64)(Co_Ctx, Co_Rtn, Co_Frame));
#if UNUSED_CODE
$static co_fn_((countCo)(Sys sys, usize n, f64 interval_secs, S_const$u8 label)(f64)$scope(
    co_locals_({
        var_(instant, time_Awake_Inst);
        var_(elapsed, f64);
        var_(i, usize);
    }),
    co_locals_mut_({}),
    co_suspended_({
        var_(sleeping, Void);
    })
)) {
    co_let_(instant) = time_Awake_now($co_arg(sys).time);
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:.1fl}"), $co_arg(interval_secs));

    co_for_(($rt($co_arg(n)))(i)) {
        suspend_((sleeping)(catch_((
            time_Awake_sleep($co_arg(sys).time, time_Dur_fromSecs$f64($co_arg(interval_secs)))
        )($ignore, $do_nothing))));
        report(
            $co_arg(sys).io, $co_arg(label), u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"),
            $co_arg(interval_secs), $co(i), $co_arg(n)
        );
    } $end(co_for);

    co_let_(elapsed) = pipe_(($co(instant))(
        (t)(time_Awake_Inst_elapsed(t, $co_arg(sys).time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report($co_arg(sys).io, $co_arg(label), u8_l("after loop {:.1fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
#endif /* UNUSED_CODE */
$static co_fn_(countCo, (Sys sys; usize n; f64 interval_secs; S_const$u8 label), f64);
co_fn_frame_scope(
    countCo,
    co_locals_({
        var_(instant, time_Awake_Inst);
        var_(elapsed, f64);
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
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:.1fl}"), $co_arg(interval_secs));

    for (co_var_(i) = 0; $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(sys).time, time_Dur_fromSecs$f64($co_arg(interval_secs))))($ignore, $do_nothing))));
        report($co_arg(sys).io, $co_arg(label), u8_l("slept {:.1fl} | i: {:uz} < n: {:uz}"), $co_arg(interval_secs), $co_mut(i), $co_arg(n));
    }

    co_let_(elapsed) = pipe_(($co(instant))(
        (t)(time_Awake_Inst_elapsed(t, $co_arg(sys).time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report($co_arg(sys).io, $co_arg(label), u8_l("after loop {:.1fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
co_use_Clsr_((countCo)(Sys, usize, f64, S_const$u8)(f64));

T_use$((f64)(Future, Future_await, Future_cancel, Sched_async));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var loop = exec_Coop_init(gpa, try_(time_Awake_direct()), exec_Evented_noop);
    defer_(exec_Coop_fini(&loop));
    let sched = Sched_coop(&loop);
    let_(sys, Sys) = {
        .time = time_Awake_evented(&loop),
        .io = try_(io_direct()),
    };
    var task_elapsed_sum = 0.0;
    let run_start = time_Awake_now(sys.time);
    using_() blk_defer {
        io_stream_println(sys.io, u8_l("begin - evented async execution"));
        var task_a = Sched_async$f64(sched, clsr_((countFn)(sys, 2, 1.0, u8_l("task a"))).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_a, sched));
        var task_b = Sched_async$f64(sched, clsr_((countCo)(sys, 3, 0.6, u8_l("task b"))).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_b, sched));
        task_elapsed_sum += Future_await$f64(&task_a, sched);
        task_elapsed_sum += Future_await$f64(&task_b, sched);
        io_stream_println(sys.io, u8_l("end - evented async execution"));
    } blk_deferral;
    let wall_elapsed_secs = pipe_((run_start)(
        (t)(time_Awake_Inst_elapsed(t, sys.time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    io_stream_println(sys.io, u8_l("task elapsed sum: {:.1fl}"), task_elapsed_sum);
    io_stream_println(sys.io, u8_l("wall elapsed: {:.1fl}"), wall_elapsed_secs);
    return_ok({});
} $unguarded(fn);

/*
begin
[task a] before loop 1.0
[task b] before loop 0.6
[task b] slept 0.6 | i: 0 < n: 3
[task a] slept 1.0 | i: 0 < n: 2
[task b] slept 0.6 | i: 1 < n: 3
[task b] slept 0.6 | i: 2 < n: 3
[task b] after loop 1.8
[task a] slept 1.0 | i: 1 < n: 2
[task a] after loop 2.0
end
total: 3.8
*/
