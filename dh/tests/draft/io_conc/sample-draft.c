#include "dh-main.h"
#include "dh/exec.h"
#include "dh/heap/Page.h"
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
T_use$((f64)(Closure_Ctx, Closure_Rtn, Closure));
fn_use_Closure_((countFn)(Sys, usize, time_Dur, S_const$u8)(f64));
/* Coroutines enforce the use of closures. (co_use_Closure) is mandatory. */
#pragma region coroutine_goals_syntax
#if UNUSED_CODE
$static co_fn_(((countCo)(Sys sys, usize n, time_Dur interval, S_const$u8 label))(f64)$scope(
    co_locals_({
        var_(instant, time_Inst);
        var_(i, usize);
        var_(elapsed, f64);
    }),
    co_locals_mut_({}),
    co_suspended_({
        var_(slept, Void);
    })
)) {
    co_let_((instant)(time_Inst_now($co_arg(sys).time)));
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    co_for_(($rt($co_arg(n)))(i)) {
        suspend_((slept)(catch_((time_sleep($co_arg(sys).time, interval))($ignore, $do_nothing))));
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
#if UNUSED_CODE
T_alias$((Co_Ctx$f64)(union Co_Ctx$f64 {
    T_embed$(struct {
        T_embed$(union {
            var_(ret, f64);
            var_(ret_, f64) $like_ref;
        });
        var_(args_, V$raw) $flexible;
        var_(data_, V$raw) $flexible;
    });
    var_(as_raw, Co_Ctx$raw) $flexible;
}));
T_alias$((Co_Rtn$f64)(fn_(((*)(P$$(Co_Ctx$f64) ctx))(P$$(Co_Ctx$f64)) $T)));
T_alias$((Co_Frame$f64)(union Co_Frame$f64 {
    T_embed$(struct {
        var_(rtn, Co_Rtn$f64);
        T_embed$(union {
            var_(ctx, Co_Ctx$f64);
            var_(ctx_, Co_Ctx$f64) $like_ref;
        });
    });
    var_(as_raw, Co_Frame$raw) $flexible;
}));

T_alias$((Co__Ret_countFn)(f64));
T_alias$((Co__Args_countFn)(struct Co__Args_countFn {
    var_(sys, Sys);
    var_(n, usize);
    var_(interval, time_Dur);
    var_(label, S_const$u8);
}));
T_alias$((Co__Suspended_countFn)(union Co__Suspended_countFn {
    var_(data, Void);
}));
T_alias$((Co__Locals_countFn)(struct structCo__Locals_countFn {
    var_(instant, time_Inst);
    var_(i, usize);
    var_(elapsed, f64);
}));
T_alias$((Co__LocalsMut_countFn)(struct Co__LocalsMut_countFn{}));
T_alias$((Co__Data_countFn)(struct Co__Data_countFn {
    var_(suspended, Co__Suspended_countFn);
    var_(locals, Co__Locals_countFn);
    var_(locals_mut, Co__LocalsMut_countFn);
}));
T_alias$((Co__Ctx_countFn)(union Co__Ctx_countFn {
    T_embed$(struct {
        var_(ctrl, Co_Ctrl);
        var_(suspended_data, P$raw);
        T_embed$(union {
            var_(ret, Co__Ret_countFn);
            var_(ret_, Co__Ret_countFn) $like_ref;
        });
        T_embed$(union {
            var_(args, Co__Args_countFn);
            var_(args_, Co__Args_countFn) $like_ref;
        });
        T_embed$(union {
            var_(data, Co__Data_countFn);
            var_(data_, Co__Data_countFn) $like_ref;
        });
    });
    var_(as_raw, Co_Ctx$raw) $flexible;
    var_(as_base, Co_Ctx$f64) $flexible;
}));
T_alias$((Co__Rtn_countFn)(fn_(((*)(P$$(Co__Ctx_countFn) ctx))(P$$(Co__Ctx_countFn)) $T)));
T_alias$((Co__Frame_countFn)(union Co__Frame_countFn {
    T_embed$(struct {
        var_(rtn, Co__Rtn_countFn);
        T_embed$(union {
            var_(ctx, Co__Ctx_countFn);
            var_(ctx_, Co__Ctx_countFn) $like_ref;
        });
    });
    var_(as_raw, Co_Frame$raw) $flexible;
    var_(as_base, Co_Frame$f64) $flexible;
}));
$static fn_((countCo(P$$(Co__Ctx_countFn) ctx))(P$$(Co__Ctx_countFn)) $scope) {
    co_called_(ctx);

    co_let_(instant, time_Inst_now($co_arg(sys).time));
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    for_(($rt($co_arg(n)))(i)) {
        co_let_(i, i);
        suspend_(catch_((time_sleep($co_arg(sys).time, $co_arg(interval)))($ignore, $do_nothing)));
        report($co_arg(sys).io, $co_arg(label), u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), $co_arg(interval), $co(i), $co_arg(n));
    } $end(for);

    co_let_(
        elapsed,
        pipe_(($co(instant))(
            (t)(time_Inst_elapsed(t, $co_arg(sys).time)),
            (t)(time_Dur_asSecs$f64(t))
        ))
    );
    report($co_arg(sys).io, $co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));

    co_return_($co(elapsed));
} $unscoped(fn);
co_use_Closure$((countCo)(Sys, usize, time_Dur, S_const$u8)(f64));
#endif /* UNUSED_CODE */
T_use$((f64)(Co_Ctx, Co_Rtn, Co_Frame));
co_fn_(countCo, (Sys sys; usize n; time_Dur interval; S_const$u8 label), f64);
co_fn_scope(
    countCo,
    co_locals_({
        var_(instant, time_Inst);
        var_(elapsed, f64);
    }),
    co_locals_mut_({
        var_(i, usize);
    }),
    co_suspended_({
        var_(slept, Void);
    })
) {
    co_let_(instant, time_Inst_now($co_arg(sys).time));
    report($co_arg(sys).io, $co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    for (co_var_(i, 0); $co_mut(i) < $co_arg(n); ++$co_mut(i)) {
        suspend_(slept, catch_((time_sleep($co_arg(sys).time, $co_arg(interval)))($ignore, $do_nothing)));
        report(
            $co_arg(sys).io, $co_arg(label),
            u8_l("slept {:fl} | i: {:uz} < n: {:uz}"),
            $co_arg(interval), $co_mut(i), $co_arg(n)
        );
    };

    co_let_(elapsed, pipe_(($co(instant))((t)(time_Inst_elapsed(t, $co_arg(sys).time)), (t)(time_Dur_asSecs$f64(t)))));
    report($co_arg(sys).io, $co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
co_use_Closure_((countCo)(Sys, usize, time_Dur, S_const$u8)(f64));
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
        let gpa = heap_Page_alctr(&l0$((heap_Page)));
        var loop = exec_Coop_init(gpa, time_direct());
        defer_(exec_Coop_fini(&loop));
        let sched = Sched_coop(&loop);
        let_(sys, Sys) = {
            .time = time_evented(&loop),
            .io = io_direct(),
        };
        let async = Sched_async$f64;
        let cancel = Future_cancel$f64;
        let await = Future_await$f64;
        let countFn = closure_(countFn);
        let countCo = closure_(countCo);
        let intervalSecs = time_Dur_fromSecs$f64;

        var total = 0.0;
        blk_defer_({
            io_stream_println(sys.io, u8_l("begin - evented async execution"));
            var task_a = async(sched, countFn(sys, 2, intervalSecs(1.0), u8_l("task a")).as_base);
            defer_(let_ignore = cancel(&task_a, sched));
            var task_b = async(sched, countCo(sys, 3, intervalSecs(0.6), u8_l("task b")).as_base);
            defer_(let_ignore = cancel(&task_b, sched));
            total += await(&task_a, sched);
            total += await(&task_b, sched);
            io_stream_println(sys.io, u8_l("end - evented async execution"));
        }) blk_deferral;
        io_stream_println(sys.io, u8_l("total: {:fl}"), total);
    };
    {
        let gpa = heap_Page_alctr(&l0$((heap_Page)));
        var exec = exec_Preem_init(gpa);
        defer_(exec_Preem_fini(&exec));
        let sched = Sched_preem(&exec);
        let_(sys, Sys) = {
            .time = time_direct(),
            .io = io_direct(),
        };
        let spawn = Sched_spawn$f64;
        let cancel = Future_cancel$f64;
        let await = Future_await$f64;
        let countFn = closure_(countFn);
        let countCo = closure_(countCo);
        let intervalSecs = time_Dur_fromSecs$f64;

        var total = 0.0;
        blk_defer_({
            io_stream_println(sys.io, u8_l("begin - parallel execution"));
            var task_a = catch_((spawn(sched, countFn(sys, 2, intervalSecs(1.0), u8_l("task a")).as_base))(
                $ignore, claim_unreachable
            ));
            defer_(let_ignore = cancel(&task_a, sched));
            var task_b = catch_((spawn(sched, countCo(sys, 3, intervalSecs(0.6), u8_l("task b")).as_base))(
                $ignore, claim_unreachable
            ));
            defer_(let_ignore = cancel(&task_b, sched));
            total += await(&task_a, sched);
            total += await(&task_b, sched);
            io_stream_println(sys.io, u8_l("end - parallel execution"));
        }) blk_deferral;
        io_stream_println(sys.io, u8_l("total: {:fl}"), total);
    };
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
