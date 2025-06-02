#include "mockup-async_ex.h"

#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/time.h"

#include <stdio.h>

/// \brief Task to be executed
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
use_Ptr$(Task);
use_Sli$(Task);
use_Opt$(Task);

/// \brief List of tasks to be executed
static var_(exec_s_task_list, Arr$$(10, Opt$Task)) = {};
/// \brief Run the event loop
/// \param endless Whether to run the loop endlessly
static fn_(exec_runLoop(bool endless), void) {
    use_Sli$(Opt$Task);
    use_Opt$(Co_Ctx);

    printf("looping events y'all\n");
    while (true) {
        let  now   = time_Instant_now();
        bool any   = false;
        var  frame = none$(Opt$Ptr$Co_Ctx);
        for_slice (Sli_arr$(Sli$Opt$Task, exec_s_task_list), task_remaining) {
            if_some(Opt_asPtr(task_remaining), task) {
                any = true;
                if (time_Instant_le(task->expires, now)) {
                    printf("sleep over y'all\n");
                    Opt_asg(&frame, some(task->frame));
                    Opt_asg(task_remaining, none());
                    break;
                }
            }
        }
        if_some(frame, ctx) {
            resume_(ctx);
        }
        if (!(endless || any)) { break; }
    }
}

/// \brief Find a slot for a task
/// \return The slot for the task
static fn_(exec_findSlot(void), Opt$Task*) {
    use_Sli$(Opt$Task);

    Opt$Task* slot = null;
    for_slice (Sli_arr$(Sli$Opt$Task, exec_s_task_list), task) {
        if_none(*task) {
            slot = task;
            break;
        }
    }
    if (slot == null) { claim_unreachable; }
    return slot;
}

use_Co_Ctx$(Void);
/// \brief Sleep for a specified duration
/// \param caller The caller context
/// \param ms The duration to sleep in milliseconds
async_fn_(exec_sleep, (var_(caller, Opt$$(Co_Ctx*)); var_(ms, u64);), Void);
async_fn_scope(exec_sleep, {}) {
    $ignore = locals;
    suspend_({
        let slot = exec_findSlot();
        let time = eval({
            static let fromMs = time_Duration_fromMillis;
            static let addDur = time_Instant_addDuration;
            static let now    = time_Instant_now;
            eval_return addDur(now(), fromMs(args->ms));
        });
        Opt_asg(slot, some({ .frame = orelse(args->caller, ctx->anyraw), .expires = time }));
    });
    areturn_({});
} $unscoped_async_fn;

#include "dh/main.h"
#include "dh/Thrd.h"
#include "dh/callback.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
fn_(report(Str_const label, const char* fmt, ...), void) {
    printf("[ThrdId(%zu): %*s] ", Thrd_getCurrentId(), as$(i32, label.len), label.ptr);
    va_list args = {};
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

use_Co_Ctx$(f64);
async_fn_(count, (var_(caller, Opt$$(Co_Ctx*)); var_(n, usize); var_(interval, f64); var_(label, Str_const);), f64);
async_fn_scope(count, {
    var_(start, time_Instant);
    var_(wait_ms, u64);
    var_(iter, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
    var_(total, f64);
}) {
    locals->start = time_Instant_now();
    report(args->label, "before loop %f\n", args->interval);
    locals->wait_ms = as$(u64, (args->interval * time_millis_per_sec));

    locals->iter = 0;
    while (locals->iter < args->n) {
        // locals->sleep_ctx = *async_ctx((exec_sleep)(ctx->anyraw, locals->wait_ms));
        // while (resume_(&locals->sleep_ctx)->state == Co_State_suspended) { suspend_(); }
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(ctx->anyraw), locals->wait_ms));
        report(args->label, "slept %f | i: %zu < n: %zu\n", args->interval, locals->iter, args->n);
        locals->iter++;
    }

    locals->total = eval({
        static let asSecs   = time_Duration_asSecs_f64;
        static let durSince = time_Instant_durationSince;
        static let now      = time_Instant_now;
        eval_return asSecs(durSince(now(), locals->start));
    });
    report(args->label, "after loop %f\n", locals->total);
    areturn_(locals->total);
} $unscoped_async_fn;

/// \brief Run the main function
/// \param args The arguments to the main function
async_fn_(runMain, (Sli$Str_const args;), f64);
async_fn_scope(runMain, {
    var_(tasks, Arr$$(2, Co_CtxFn$(count)));
    var_(total, f64);
    var_(await_idx, usize);
    var_(await_curr, Co_CtxFn$(count)*);
}) {
    $ignore = args;
    printf("begin\n");

    // clang-format off
    Arr_asg(locals->tasks, Arr_init$(Arr$$(2, Co_CtxFn$(count)), {
        [0] = *async_ctx((count)(none(), 2, 1.0, u8_l("task a"))),
        [1] = *async_ctx((count)(none(), 3, 0.6, u8_l("task b"))),
    }));
    for_array (locals->tasks, task) { resume_(task); }
    // clang-format on

    printf("count size: %zu\n", sizeOf(Arr_getAt(locals->tasks, 0)));

    locals->total = 0.0;
    for (locals->await_idx = 0; locals->await_idx < Arr_len(locals->tasks); ++locals->await_idx) {
        locals->await_curr = Arr_at(locals->tasks, locals->await_idx);
        await_(locals->await_curr);
        locals->total += locals->await_curr->ret->value;
    }

    printf("end\n");
    areturn_(locals->total);
} $unscoped_async_fn;

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    var task = async_((runMain)(args));
    printf("run size: %zu\n", sizeOf(*task));
    exec_runLoop(false);
    nosuspend_(await_(resume_(task)));
    let total = task->ret->value;
    printf("total: %f\n", total);
    return_ok({});
} $unscoped;
