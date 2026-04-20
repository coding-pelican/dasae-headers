#include "dh-main.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/time/Dur.h"
#include "dh/io/stream.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
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
fn_use_Closure$((countFn)(Sys, usize, time_Dur, S_const$u8)(f64));
/* Coroutines enforce the use of closures. (co_use_Closure) is mandatory. */
#pragma region coroutine_goals_syntax
#if UNUSED_CODE
$static co_fn_(((countCo)(Sys sys, usize n, time_Dur interval, S_const$u8 label))(f64)$scope(
    $co_locals({
        var_(instant, time_Inst);
        var_(i, usize);
        var_(elapsed, f64);
    }),
    $co_locals_mut({})
)) {
    co_let_((instant)(time_Inst_now($co_arg(sys).time)));
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    co_for_(($rt($co_arg(n)))(i)) {
        catch_((time_sleep($co_arg(sys).time, interval))($ignore, $do_nothing));
        report($co_arg(sys).io, $co_arg(label), u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), $co_arg(interval), $co(i), $co_arg(n));
    } $end(co_for);

    co_let_((elapsed)(pipe_(($co(instant))(
        (t)(time_Inst_elapsed(t, $co_arg(sys).time)),
        (t)(time_Dur_asSecs$f64(t))
    ))));
    report($co_arg(sys).io, $co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
co_use_Closure$((countCo)(Sys, usize, time_Dur, S_const$u8)(f64));
#endif /* UNUSED_CODE */
#pragma endregion coroutine_goals_syntax
T_use$((f64)(
    Future,
    Future_await,
    Future_cancel,
    Sched_async,
    Sched_spawn
));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    {
        let_(sys, Sys) = {
            .time = time_seq(),
            .io = io_seq(),
        };
        let count = closure$(countFn);
        var closure = count(sys, 10, time_Dur_fromMillis(100), u8_l("count"));
        io_stream_print(sys.io, u8_l("fn_ret: {:fl}\n"), invoke(&closure)->ret);
    }
    var_(exec, exec_Para) = {};
    let sched = exec_Para_sched(&exec);
    let_(sys, Sys) = {
        .time = time_para(),
        .io = io_para(),
    };
    {
        let async = Sched_spawn$f64;
        let cancel = Future_cancel$f64;
        let await = Future_await$f64;
        let count = closure$(countFn);
        let intervalSecs = time_Dur_fromSecs$f64;

        var total = 0.0;
        blk_defer_({
            io_stream_println(sys.io, u8_l("begin"));
            var task_a = catch_((async(sched, count(sys, 2, intervalSecs(1.0), u8_l("task a")).as_raw))(
                $ignore, claim_unreachable
            ));
            defer_(let_ignore = cancel(&task_a, sched));
            var task_b = catch_((async(sched, count(sys, 3, intervalSecs(0.6), u8_l("task b")).as_raw))(
                $ignore, claim_unreachable
            ));
            defer_(let_ignore = cancel(&task_b, sched));
            total += await(&task_a, sched);
            total += await(&task_b, sched);
            io_stream_println(sys.io, u8_l("end"));
        }) blk_deferral;
        io_stream_println(sys.io, u8_l("total: {:fl}"), total);
    };
    return_ok({});
} $unguarded(fn);

/*
begin
[task a] before loop 1.0
[task b] before loop 0.6
[task a] slept 1.0 | i: 0 < n: 2
[task b] slept 0.6 | i: 0 < n: 3
[task b] slept 0.6 | i: 1 < n: 3
[task a] slept 1.0 | i: 1 < n: 2
[task b] slept 0.6 | i: 2 < n: 3
[task a] after loop 1.0
[task b] after loop 1.8
end
total: 2.8
*/
