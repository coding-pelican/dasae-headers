/**
 * @file    Io_example.c
 * @brief   Demonstrates identical coroutine code running on
 *          blocking and event loop backends.
 */
#include "Io.h"
#include "Io-Synced.h"
#include "Io-Sched.h"

#include "dh/time.h"
#include "dh/time/Instant.h"
#include "dh/io/stream.h"
#include "dh-main.h"

/*========== Type Registration ==============================================*/

use_Co_Ctx$(Void);
use_Co_Ctx$(f64);
T_use$((f64)(
    Io_Future,
    Io_Future_await
));

/*========== Io Sleep Coroutine =============================================*/

/// Coroutine primitive: sleep for ms milliseconds.
/// Sets suspend_data and yields to the runner.
co_fn_(Io_sleep, (var_(io, Io); var_(ms, u64);), Void);
co_fn_scope(Io_sleep, {
    var_(req, Io_SuspendReq_sleep);
}) {
    let_ignore = args->io;
    locals->req = (Io_SuspendReq_sleep){
        .op = Io_SuspendOp_sleep,
        .ns = args->ms * 1000000ULL,
    };
    suspend_(ctx->suspend_data = &locals->req);
    co_return_({});
} $unscoped(co_fn);

/*========== User Coroutine: count ==========================================*/

/// Counts n iterations with interval-second delays between each.
/// Returns total elapsed time.
co_fn_(count, (var_(io, Io); var_(n, usize); var_(interval, f64); var_(label, S_const$u8);), f64);
co_fn_scope(count, {
    var_(start, time_Instant);
    var_(wait_ms, u64);
    var_(iter, usize);
    var_(sleep_ctx, Co_CtxFn$(Io_sleep));
    var_(total, f64);
}) {
    locals->start = time_Instant_now();
    locals->wait_ms = as$(u64)(args->interval * 1000.0);

    locals->iter = 0;
    while (locals->iter < args->n) {
        co_call(&locals->sleep_ctx, (Io_sleep)(args->io, locals->wait_ms));
        io_stream_println(u8_l("[{:s}] slept {:fl}s | i: {:uz}"), args->label, args->interval, locals->iter);
        locals->iter++;
    }

    locals->total = local_({
        let asSecs = time_Duration_asSecs$f64;
        let durSince = time_Instant_durationSince;
        let now = time_Instant_now;
        local_return_(asSecs(durSince(now(), locals->start)));
    });
    io_stream_println(u8_l("[{:s}] done in {:fl}s"), args->label, locals->total);
    co_return_(locals->total);
} $unscoped(co_fn);

/*========== main ===========================================================*/

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    /* ================================================================== */
    /* Blocking Backend                                                    */
    /* ================================================================== */
    io_stream_println(u8_l("=== blocking ==="));
    {
        let io = Io_synced();

        var task_a = *co_ctx((count)(io, 2, 1.0, u8_l("task-a")));
        var task_b = *co_ctx((count)(io, 3, 0.6, u8_l("task-b")));

        /* Dispatch coroutines — blocking runs each to completion. */
        var future_a = l0$((Io_Future$(f64)));
        Io_asyncCo(io, task_a.anyraw, &future_a);

        var future_b = l0$((Io_Future$(f64)));
        Io_asyncCo(io, task_b.anyraw, &future_b);

        /* await is no-op — results already populated. */
        let a = Io_Future_await$f64(&future_a, io);
        let b = Io_Future_await$f64(&future_b, io);

        io_stream_println(u8_l("blocking total: {:fl}s"), a + b);
        /* Expected: ~3.8s (2.0 + 1.8, sequential) */
    }

    /* ================================================================== */
    /* Event Loop Backend                                                  */
    /* ================================================================== */
    io_stream_println(u8_l("=== event loop ==="));
    {
        var loop = Io_Sched_loop();
        let io = Io_sched(&loop);

        var task_a = *co_ctx((count)(io, 2, 1.0, u8_l("task-a")));
        var task_b = *co_ctx((count)(io, 3, 0.6, u8_l("task-b")));

        /* Dispatch — event loop registers both, runs first suspend. */
        var future_a = l0$((Io_Future$(f64)));
        Io_asyncCo(io, task_a.anyraw, &future_a);

        var future_b = l0$((Io_Future$(f64)));
        Io_asyncCo(io, task_b.anyraw, &future_b);

        /* await drives the event loop until each future completes. */
        let a = Io_Future_await$f64(&future_a, io);
        let b = Io_Future_await$f64(&future_b, io);

        io_stream_println(u8_l("event loop total: {:fl}s"), a + b);
        /* Expected: ~2.0s wall-clock (interleaved), ~3.8s sum */
    }

    // /* ================================================================== */
    // /* Plain function via Io_async                                         */
    // /* ================================================================== */
    // io_stream_println(u8_l("=== plain function ==="));
    // {
    //     let io = Io_synced();

    //     /* Args struct for a plain (non-coroutine) function. */
    //     typedef struct {
    //         var_(x, i32);
    //         var_(y, i32);
    //     } AddArgs;

    //     /* Start function matching Io_StartFn signature. */
    //     $static fn_((add_start(P_const$raw context, P$raw result))(void)) {
    //         let a = as$(const AddArgs*)(context);
    //         *as$(i32*)(result) = a->x + a->y;
    //     }

    //     let context = (AddArgs){ .x = 17, .y = 25 };
    //     var future = (Io_Future$(i32)){};
    //     Io_async(io, add_start, &context, &future);

    //     let sum = Io_Future_await$i32(&future, io);
    //     io_stream_println(u8_l("17 + 25 = {:d}"), sum);
    // }

    return_ok({});
} $unscoped(fn);
