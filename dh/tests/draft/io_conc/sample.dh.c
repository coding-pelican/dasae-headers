#include "dh-main.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/io/stream.h"
#include "dh/heap/Page.h"

$static fn_((report(io_Self io, S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(io, u8_l("[{:s}] "), label);
    using_(var args = l0$((va_list))) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(io, fmt, args);
    }
    io_stream_nl(io);
};

T_alias$((Sys)(struct Sys {
    var_(time, time_Self);
    var_(io, io_Self);
}));
$static fn_((countFn(Sys sys, usize n, time_Dur interval, S_const$u8 label))(f64)) {
    let instant = time_Inst_now(sys.time);
    report(sys.io, label, u8_l("before loop {:fl}"), interval);

    for_(($rt(n))(i)) {
        catch_((time_sleep(sys.time, interval))($ignore, $do_nothing));
        report(sys.io, label, u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), interval, i, n);
    } $end(for);

    let elapsed = pipe_((instant)(
        (t)(time_Inst_elapsed(t, sys.time)),
        (t)(time_Dur_asSecs$f64(t))
    ));
    report(sys.io, label, u8_l("after loop {:fl}"), elapsed);
    return elapsed;
};
T_use$((f64)(Closure_Ctx, Closure_Rtn, Closure));
T_use$((f64)(Future, Future_await, Future_cancel, Sched_async));
fn_use_Closure_((countFn)(Sys, usize, time_Dur, S_const$u8)(f64));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    var loop = exec_Coop_init(gpa, time_direct());
    defer_(exec_Coop_fini(&loop));
    let sched = Sched_coop(&loop);
    let_(sys, Sys) = {
        .time = time_evented(&loop),
        .io = io_direct(),
    };
    var total = 0.0;
    blk_defer_({
        io_stream_println(sys.io, u8_l("begin - evented async execution"));
        var task_a = Sched_async$f64(sched, closure_(countFn)(sys, 2, time_Dur_fromSecs$f64(1.0), u8_l("task a")).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_a, sched));
        var task_b = Sched_async$f64(sched, closure_(countFn)(sys, 3, time_Dur_fromSecs$f64(0.6), u8_l("task b")).as_base);
        defer_(let_ignore = Future_cancel$f64(&task_b, sched));
        total += Future_await$f64(&task_a, sched);
        total += Future_await$f64(&task_b, sched);
        io_stream_println(sys.io, u8_l("end - evented async execution"));
    }) blk_deferral;
    io_stream_println(sys.io, u8_l("total: {:fl}"), total);
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
