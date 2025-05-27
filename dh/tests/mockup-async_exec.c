#include "mockup-async.h"

#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/time.h"
#define u8_s Str_m
#define u8_l Str_l

#include <stdio.h>

// Delay type for scheduling tasks
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
use_Ptr$(Task);
use_Sli$(Task);
use_Opt$(Task);

static var_(exec_s_task_list, Arr$$(10, Opt$Task)) = {};
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

use_Co_Ctx$(exec_sleep, (var_(caller, Co_Ctx*); var_(ms, u64);), Void, {});
async_fn_scope(exec_sleep) {
    $ignore = locals;
    suspend_({
        let slot = exec_findSlot();
        let time = eval({
            static let fromMs = time_Duration_fromMillis;
            static let addDur = time_Instant_addDuration;
            static let now    = time_Instant_now;
            eval_return addDur(now(), fromMs(args->ms));
        });
        Opt_asg(slot, some({ .frame = args->caller, .expires = time }));
    });
    async_return_({});
} async_unscoped;

#include "dh/main.h"
#include "dh/Thrd.h"
#include "dh/callback.h"

fn_(report(Str_const label, const char* fmt, ...), void) {
    printf("[ThrdId(%zu): %*s] ", Thrd_getCurrentId(), as$(i32, label.len), label.ptr);
    va_list args = {};
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

use_Co_Ctx$(count, (var_(n, usize); var_(interval, f64); var_(label, Str_const);), f64, {
    var_(start, time_Instant);
    var_(wait_ms, u64);
    var_(i, usize);
    var_(sleep_ctx, Co_Ctx$(exec_sleep));
    var_(total, f64);
});
async_fn_scope(count) {
    locals->start = time_Instant_now();
    report(args->label, "before loop %f\n", args->interval);
    locals->wait_ms = as$(u64, (args->interval * time_millis_per_sec));

    locals->i = 0;
    while (locals->i < args->n) {
        // suspend_(locals->sleep_ctx = *async_(exec_sleep, self->base, locals->wait_ms));
        async_with(locals->sleep_ctx, exec_sleep, (self->base, locals->wait_ms));
        report(args->label, "slept %f | i: %zu < n: %zu\n", args->interval, locals->i, args->n);
        locals->i++;
    }

    locals->total = eval({
        static let asSecs   = time_Duration_asSecs_f64;
        static let durSince = time_Instant_durationSince;
        static let now      = time_Instant_now;
        eval_return asSecs(durSince(now(), locals->start));
    });
    report(args->label, "after loop %f\n", locals->total);
    async_return_(locals->total);
} async_unscoped;

use_Co_Ctx$(runMain, (), f64, {
    var_(tasks, Arr$$(2, Co_Ctx$(count)));
    var_(total, f64);
    var_(await_idx, usize);
    var_(await_curr, Co_Ctx$(count)*);
});
async_fn_scope(runMain) {
    $ignore = args;
    printf("begin\n");

    // clang-format off
    Arr_asg(locals->tasks, Arr_init$(Arr$$(2, Co_Ctx$(count)), {
        [0] = async_ctx(count, (2, 1.0, u8_l("task a"))),
        [1] = async_ctx(count, (3, 0.6, u8_l("task b"))),
    }));
    for_array (locals->tasks, task) { resume_(task); }
    // clang-format on

    printf("count size: %zu\n", sizeOf(Arr_getAt(locals->tasks, 0)));

    locals->total = 0.0;
    for (locals->await_idx = 0; locals->await_idx < Arr_len(locals->tasks); ++locals->await_idx) {
        locals->await_curr = as$(Co_Ctx$(count)*, Arr_at(locals->tasks, locals->await_idx));
        await_(locals->await_curr);
        locals->total += locals->await_curr->ret.value;
    }

    printf("end\n");
    async_return_(locals->total);
} async_unscoped;

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    $ignore = args;

    var task = async_(runMain, ());
    printf("run size: %zu\n", sizeOf(*task));
    exec_runLoop(false);
    let total = nosuspend_await_(task).value;
    printf("total: %f\n", total);

    return_ok({});
} $unscoped;

/*
clang -std=c17 -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\include -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\libs\BlocksRuntime\include -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\ -DCOMP -Wall -Wextra -funsigned-char -fblocks -DBlocksRuntime_STATIC -g3 -O0 -Wall -Werror C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\mockup-async_exec.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ArrList.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\claim_assert.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\debug_assert.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\debug_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Err.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ErrTrace.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\fs_dir.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Arena.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Classic.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Fixed.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Page.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_ThrdSafe.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\io_Writer.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\list.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\log.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\mem_Allocator.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\mem_Tracker.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\meta_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ptr.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Random.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\sort.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Str.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\TEST.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_Cond.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_Mtx.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_Duration.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_Instant.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_SysTime.c -o C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\mockup-async_exec.exe "C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\libs\BlocksRuntime\lib\BlocksRuntime.lib" -static -pthread
 */
