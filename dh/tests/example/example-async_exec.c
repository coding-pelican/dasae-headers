#include "dh/async.h"

#include "dh/time.h"
#include "dh/time/Instant.h"
#include "dh/io/stream.h"

/// \brief Task to be executed
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
T_use$((Task)(P, S, O));
T_use$((Co_Ctx)(P, S, O));
T_use$((P$Co_Ctx)(O));

/// \brief List of tasks to be executed
$static var_(exec_s_task_list, A$$(10, O$Task)) = {};
/// \brief Run the event loop
/// \param endless Whether to run the loop endlessly
$static fn_((exec_runLoop(bool endless))(void)) {
    io_stream_println(u8_l("looping events y'all"));
    while (true) {
        let now = time_Instant_now();
        var any = false;
        var frame = eval_(O$P$Co_Ctx $scope)(for_(($s(A_ref(exec_s_task_list)))(task_remaining) {
            let task = orelse_((O_asP(task_remaining))(continue));
            any = true;
            if (time_Instant_le(task->expires, now)) {
                io_stream_println(u8_l("sleep over y'all"));
                let frame = task->frame;
                asg_lit((task_remaining)(none()));
                $break_(some(frame));
            }
        })) eval_(else)($break_(none())) $unscoped_(eval);
        if_some((frame)(ctx)) {
            resume_(ctx);
        }
        if (!(endless || any)) { break; }
    }
}

/// \brief Find a slot for a task
/// \return The slot for the task
$static fn_((exec_findSlot(void))(O$Task*)) {
    let slot = eval_(P$$(O$Task) $scope)(
        for_(($s(A_ref(exec_s_task_list)))(task) {
            if_none(*task)
                $break_(task);
        })
    ) eval_(else)(claim_unreachable) $unscoped_(eval);
    return slot;
}

use_Co_Ctx$(Void);
/// \brief Sleep for a specified duration
/// \param caller The caller context
/// \param ms The duration to sleep in milliseconds
async_fn_(exec_sleep, (var_(caller, O$$(Co_Ctx*)); var_(ms, u64);), Void);
async_fn_scope(exec_sleep, {}) {
    let_ignore = locals;
    suspend_({
        let slot = exec_findSlot();
        let time = blk({
            $static let fromMs = time_Duration_fromMillis;
            $static let addDur = time_Instant_addDuration;
            $static let now = time_Instant_now;
            blk_return_(addDur(now(), fromMs(args->ms)));
        });
        asg_lit((slot)(some({ .frame = orelse_((args->caller)(ctx->anyraw)), .expires = time })));
    });
    areturn_({});
} $unscoped_(async_fn);

#include "dh/main.h"
#include "dh/Thrd.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currentId(), label);
    va_list args = {};
    with_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
}

use_Co_Ctx$(f64);
async_fn_(count, (var_(caller, O$$(Co_Ctx*)); var_(n, usize); var_(interval, f64); var_(label, S_const$u8);), f64);
async_fn_scope(count, {
    var_(start, time_Instant);
    var_(wait_ms, u64);
    var_(iter, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
    var_(total, f64);
}) {
    locals->start = time_Instant_now();
    report(args->label, u8_l("before loop {:fl}"), args->interval);
    locals->wait_ms = as$(u64)(args->interval * time_millis_per_sec);

    locals->iter = 0;
    while (locals->iter < args->n) {
        // locals->sleep_ctx = *async_ctx((exec_sleep)(ctx->anyraw, locals->wait_ms));
        // while (resume_(&locals->sleep_ctx)->state == Co_State_suspended) { suspend_(); }
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(ctx->anyraw), locals->wait_ms));
        report(args->label, u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), args->interval, locals->iter, args->n);
        locals->iter++;
    }

    locals->total = blk({
        $static let asSecs = time_Duration_asSecs$f64;
        $static let durSince = time_Instant_durationSince;
        $static let now = time_Instant_now;
        blk_return_(asSecs(durSince(now(), locals->start)));
    });
    report(args->label, u8_l("after loop {:fl}"), locals->total);
    areturn_(locals->total);
} $unscoped_(async_fn);

/// \brief Run the main function
/// \param args The arguments to the main function
async_fn_(runMain, (S$S_const$u8 args;), f64);
async_fn_scope(runMain, {
    var_(tasks, A$$(2, Co_CtxFn$(count)));
    var_(total, f64);
    var_(await_idx, usize);
    var_(await_curr, Co_CtxFn$(count)*);
}) {
    let_ignore = args;
    io_stream_println(u8_l("begin"));

    asg_lit((&locals->tasks)(A_init({
        [0] = *async_ctx((count)(none(), 2, 1.0, u8_l("task a"))),
        [1] = *async_ctx((count)(none(), 3, 0.6, u8_l("task b"))),
    })));
    for_(($s(A_ref(locals->tasks)))(task) { resume_(task); });

    io_stream_println(u8_l("count size: {:uz}"), sizeOf$(A_InnerT$(TypeOf(locals->tasks))));

    locals->total = 0.0;
    for (locals->await_idx = 0; locals->await_idx < A_len(locals->tasks); ++locals->await_idx) {
        locals->await_curr = A_at((locals->tasks)[locals->await_idx]);
        await_(locals->await_curr);
        locals->total += Co_Ctx_returned(locals->await_curr);
    }

    io_stream_println(u8_l("end"));
    areturn_(locals->total);
} $unscoped_(async_fn);

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    var task = async_((runMain)(args));
    io_stream_println(u8_l("run size: {:uz}"), sizeOf$(TypeOf(*task)));
    exec_runLoop(false);
    nosuspend_(await_(resume_(task)));
    let total = task->ret->value;
    io_stream_println(u8_l("total: {:fl}"), total);
    return_ok({});
} $unscoped_(fn);
