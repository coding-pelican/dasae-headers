#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/Err.h"
#include "dh/Str.h"
#include "dh/time/common.h"

#define u8_s Str_m
#define u8_l Str_l
static fn_(io_writeFmt(Sli$u8 stream, Sli_const$u8 format, ...), Err$usize) $must_check;
static fn_(io_writeFmt(Sli$u8 stream, Sli_const$u8 format, ...), Err$usize, $guard) {
    va_list args = {};
    va_start(args, format);
    defer_(va_end(args));
    let written = vsnprintf(
        as$(char*, stream.ptr), stream.len,
        as$(const char*, format.ptr), args
    );
    if (written < 0) { return_err(Err_Unexpected()); }
    return_ok(written);
} $unguarded;

use_Err$(Sli_const$u8);
$maybe_unused static fn_(time_Duration_fmt(time_Duration self, Sli$u8 buf), Err$Sli_const$u8) $must_check;
static fn_(time_Duration_fmt(time_Duration self, Sli$u8 buf), Err$Sli_const$u8, $scope) {
    usize written     = 0;
    let   total_nanos = self.secs * time_nanos_per_sec + self.nanos;
    if (total_nanos < time_nanos_per_micro) { // < 1 μs
        written = try_(io_writeFmt(
            buf, u8_l("%u ns"),
            self.nanos
        ));
    } else if (total_nanos < time_nanos_per_milli) { // < 1 ms
        written = try_(io_writeFmt(
            buf, u8_l("%g μs"),
            as$(f64, total_nanos) / time_nanos_per_micro
        ));
    } else if (total_nanos < time_nanos_per_sec) { // < 1 s
        written = try_(io_writeFmt(
            buf, u8_l("%g ms"),
            as$(f64, total_nanos) / time_nanos_per_milli
        ));
    } else if (self.secs < time_secs_per_min) { // < 1 min
        written = try_(io_writeFmt(
            buf, u8_l("%g s"),
            as$(f64, total_nanos) / time_nanos_per_sec
        ));
    } else if (self.secs < time_secs_per_min * time_mins_per_hour) { // < 1 hour
        let mins = self.secs / time_secs_per_min;
        let secs = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu min %g s"),
            mins, as$(f64, secs) + as$(f64, self.nanos) / time_nanos_per_sec
        ));
    } else if (self.secs < time_secs_per_min * time_mins_per_hour * time_hours_per_day) { // < 1 day
        let hours = self.secs / (time_secs_per_min * time_mins_per_hour);
        let mins  = (self.secs % (time_secs_per_min * time_mins_per_hour)) / time_secs_per_min;
        let secs  = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu h %llu min %g s"),
            hours, mins, as$(f64, secs) + as$(f64, self.nanos) / time_nanos_per_sec
        ));
    } else {
        let days  = self.secs / (time_secs_per_min * time_mins_per_hour * time_hours_per_day);
        let hours = (self.secs % (time_secs_per_min * time_mins_per_hour * time_hours_per_day))
                  / (time_secs_per_min * time_mins_per_hour);
        let mins = (self.secs % (time_secs_per_min * time_mins_per_hour)) / time_secs_per_min;
        let secs = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu d %llu h %llu min %g s"),
            days, hours, mins, as$(f64, secs) + as$(f64, self.nanos) / time_nanos_per_sec
        ));
    }

    if (buf.len <= written) {
        return_err(Err_Unexpected());
    }
    return_ok(Str_slice(buf.as_const, 0,  written));
} $unscoped;

static fn_(time_Instant_fmt(time_Instant self, Sli$u8 buf), Err$Sli_const$u8, $scope) {
    // Get Unix timestamp in seconds
    let unix_secs = time_Instant_toUnixEpoch(self);

    // Get the duration since Unix epoch to extract sub-second precision
    let unix_epoch_instant = time_Instant_fromUnixEpoch(unix_secs);
    let subsec_duration    = time_Instant_durationSince(self, unix_epoch_instant);
    let subsec_nanos       = subsec_duration.nanos;

    usize written = 0;
    if (subsec_nanos == 0) {
        // No sub-second precision needed
        written = try_(io_writeFmt(
            buf, u8_l("%llu"),
            unix_secs
        ));
    } else if (subsec_nanos % time_nanos_per_milli == 0) {
        // Show milliseconds
        written = try_(io_writeFmt(
            buf, u8_l("%llu.%03u"),
            unix_secs, subsec_nanos / time_nanos_per_milli
        ));
    } else if (subsec_nanos % time_nanos_per_micro == 0) {
        // Show microseconds
        written = try_(io_writeFmt(
            buf, u8_l("%llu.%06u"),
            unix_secs, subsec_nanos / time_nanos_per_micro
        ));
    } else {
        // Show nanoseconds
        written = try_(io_writeFmt(
            buf, u8_l("%llu.%09u"),
            unix_secs, subsec_nanos
        ));
    }

    if (buf.len <= written) {
        return_err(Err_Unexpected());
    }
    return_ok(Str_slice(buf.as_const, 0, written));
} $unscoped;

#include "dh/ArrList.h"
#include "dh/sort.h"

// Generic PQue for fixed-size arrays using ArrList and sort utilities
typedef struct PQue {
    ArrList    list;
    sort_CmpFn cmpFn;
} PQue;
use_Err$(PQue);
#define PQue$(T) pp_join($, PQue, T)
#define PQue$$(T) \
    union { \
        PQue base[1]; \
        struct { \
            ArrList$$(T) list; \
            sort_CmpFn cmpFn; \
        }; \
    }

#define use_PQue$(T) \
    $maybe_unused typedef union PQue$(T) PQue$(T); \
    union PQue$(T) { \
        PQue base[1]; \
        struct { \
            ArrList$(T) list; \
            sort_CmpFn cmpFn; \
        }; \
    }

fn_(PQue_init(TypeInfo type, mem_Allocator allocator, sort_CmpFn cmpFn), PQue) {
    return (PQue){
        .list  = ArrList_init(type, allocator),
        .cmpFn = cmpFn
    };
}

$must_use
fn_(PQue_initCap(TypeInfo type, mem_Allocator allocator, usize cap, sort_CmpFn cmpFn), Err$PQue, $scope) {
    return_ok({
        .list  = try_(ArrList_initCap(type, allocator, cap)),
        .cmpFn = cmpFn,
    });
} $unscoped;

fn_(PQue_fini(PQue* self), void) {
    ArrList_fini(&self->list);
}

$must_use
fn_(PQue_enq(PQue* self, meta_Ptr item), Err$void, $scope) {
    try_(ArrList_append(&self->list, item));
    try_(sort_stableSort(self->list.allocator, self->list.items, self->cmpFn));
    return_ok({});
} $unscoped;

fn_(PQue_deqOrNull(PQue* self), Opt$meta_Ptr, $scope) {
    if (self->list.items.len == 0) { return_none(); }
    return_(ArrList_shiftOrNull(&self->list));
} $unscoped;



#include "mockup-async.h"
#include "dh/time.h"
#include <stdio.h>

// Delay type for scheduling tasks
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
use_Ptr$(Task);
use_Sli$(Task);
use_Opt$(Task);
use_ArrList$(Task);
use_PQue$(Task);

// Globals (in Zig these are file-scope vars)
static PQue$Task timer_queue = { 0 };

// Suspend for time in milliseconds
use_Co_Ctx$(waitForTime, (var_(frame, Co_Ctx*); var_(name, Sli_const$u8); var_(ms, u64);), Void, {});
async_fn_scope(waitForTime) {
    $ignore = locals;
    printf("debug: [waitForTime(%zx)] starting <- [%*s(%zx)]\n", as$(u64, self->base), as$(i32, args->name.len), args->name.ptr, as$(u64, args->frame));
    suspend_({
        printf("debug: [waitForTime(%zx)] suspending -> [%*s(%zx)]\n", as$(u64, self->base), as$(i32, args->name.len), args->name.ptr, as$(u64, args->frame));
        static let addDur = time_Instant_addDuration;
        static let now    = time_Instant_now;
        static let fromMs = time_Duration_fromMillis;
        catch_(PQue_enq(timer_queue.base, meta_create$(Task,
            .frame   = args->frame,
            .expires = addDur(now(), fromMs(args->ms))
        )), claim_unreachable);
    });
    printf("debug: [waitForTime(%zx)] returning -> [%*s(%zx)]\n", as$(u64, self->base), as$(i32, args->name.len), args->name.ptr, as$(u64, args->frame));
    async_return_({});
} async_unscoped;

// Coroutine: waitUntilAndPrint
use_Co_Ctx$(waitUntilAndPrint, (var_(time1, u64); var_(time2, u64); var_(name, Sli_const$u8);), Void, {
    time_Instant start;
    Co_Ctx$(waitForTime) wait_ctx1;
    Co_Ctx$(waitForTime) wait_ctx2;
});
async_fn_scope(waitUntilAndPrint) {
    locals->start = time_Instant_now();
    printf("debug: [%*s(%zx)] starting <- [asyncMain]\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base));

    locals->wait_ctx1 = ((Co_Ctx$waitForTime){ .fn = waitForTime, .args = { self->base, args->name, args->time1 }, .locals = {} });
    resume_(&locals->wait_ctx1);
    printf("debug: [%*s(%zx)] suspending -> [asyncMain]\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base));
    suspend_();
    resume_(&locals->wait_ctx1);
    printf("debug: [%*s(%zx)] suspending until %zu ms\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base), args->time1);
    {
        static let asSecs   = time_Duration_asSecs_f64;
        static let durSince = time_Instant_durationSince;
        static let now      = time_Instant_now;
        printf(
            "[%*s] it is now %zu ms since start!\n",
            as$(i32, args->name.len),
            args->name.ptr,
            as$(u64, asSecs(durSince(now(), locals->start)) * 1000.0)
        );
    }

    locals->wait_ctx2 = ((Co_Ctx$waitForTime){ .fn = waitForTime, .args = { self->base, args->name, args->time2 }, .locals = {} });
    resume_(&locals->wait_ctx2);
    printf("debug: [%*s(%zx)] suspending -> [asyncMain]\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base));
    suspend_();
    resume_(&locals->wait_ctx2);
    printf("debug: [%*s(%zx)] suspending until %zu ms\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base), args->time2);
    {
        static let asSecs   = time_Duration_asSecs_f64;
        static let durSince = time_Instant_durationSince;
        static let now      = time_Instant_now;
        printf(
            "[%*s] it is now %zu ms since start!\n",
            as$(i32, args->name.len),
            args->name.ptr,
            as$(u64, asSecs(durSince(now(), locals->start)) * 1000.0)
        );
    }

    printf("debug: [%*s(%zx)] returning -> [asyncMain]\n", as$(i32, args->name.len), args->name.ptr, as$(u64, self->base));
    async_return_({});
} async_unscoped;



#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/heap/Page.h"
#include "dh/callback.h"

static $must_check $maybe_unused
fn_(testTimeDurationSort(void), Err$void, $guard) {
    use_Sli$(time_Duration);
    use_ArrList$(time_Duration);
    var_type(times, ArrList$time_Duration, ArrList_init(typeInfo$(time_Duration), heap_Page_allocator(&(heap_Page){})));
    defer_(ArrList_fini(times.base));
    {
        let start = time_Instant_now();
        try_(ArrList_append(times.base, meta_create$(time_Duration, .secs = 10)));
        try_(ArrList_append(times.base, meta_create$(time_Duration, .secs = 50)));
        try_(ArrList_append(times.base, meta_create$(time_Duration, .secs = 20)));
        try_(ArrList_append(times.base, meta_create$(time_Duration, .secs = 40)));
        try_(ArrList_append(times.base, meta_create$(time_Duration, .secs = 30)));

        var mem = (Arr$$(128, u8)){};
        var buf = Arr_slice$(Sli$u8, mem, (0, 128));
        for_slice (times.items, time) {
            let point = time_Instant_addDuration(start, *time);
            {
                let fmt = try_(time_Instant_fmt(point, buf));
                printf("%*s\n", as$(i32, fmt.len), fmt.ptr);
            }
            let dur = time_Instant_durationSince(point, start);
            {
                let fmt = try_(time_Duration_fmt(dur, buf));
                printf("%*s\n", as$(i32, fmt.len), fmt.ptr);
            }
        }
        printf("\n");

        try_(sort_stableSort(times.allocator, times.base->items, wrapLam$(sort_CmpFn, lam_((anyptr_const lhs, anyptr_const rhs), cmp_Ord) {
            let dur_lhs = as$(const time_Duration*, lhs);
            let dur_rhs = as$(const time_Duration*, rhs);
            return time_Duration_cmp(*dur_lhs, *dur_rhs);
        })));
        for_slice (times.items, time) {
            let point = time_Instant_addDuration(start, *time);
            {
                let fmt = try_(time_Instant_fmt(point, buf));
                printf("%*s\n", as$(i32, fmt.len), fmt.ptr);
            }
            let dur = time_Instant_durationSince(point, start);
            {
                let fmt = try_(time_Duration_fmt(dur, buf));
                printf("%*s\n", as$(i32, fmt.len), fmt.ptr);
            }
        }
        printf("\n");
    }
    return_ok({});
} $unguarded;



// asyncMain wrapper
use_Co_Ctx$(asyncMain, (), Void, {
    Arr$$(2, Co_Ctx$(waitUntilAndPrint)) tasks;
    usize iter_resume;
    usize iter_await;
});
async_fn_scope(asyncMain) {
    $ignore = args;

    // clang-format off
    Arr_asg(locals->tasks, Arr_init$(Arr$$(2, Co_Ctx$(waitUntilAndPrint)), {
        [0] = async_ctx(waitUntilAndPrint, (1000, 1200, u8_l("task-pair a"))),
        [1] = async_ctx(waitUntilAndPrint, (500, 1300, u8_l("task-pair b"))),
    })); // clang-format on
    for (locals->iter_resume = 0; locals->iter_resume < Arr_len(locals->tasks); ++locals->iter_resume) {
        resume_(Arr_at(locals->tasks, locals->iter_resume));
    }

    for (locals->iter_await = 0; locals->iter_await < Arr_len(locals->tasks); ++locals->iter_await) {
        await_(Arr_at(locals->tasks, locals->iter_await));
    }

    printf("debug: [asyncMain] all tasks completed\n");
    async_return_({});
} async_unscoped;



fn_(dh_main(Sli$Str_const args), Err$void, $guard) {
    for_slice_indexed (args, arg, idx) {
        printf("args[%zu]: %*s\n", idx, as$(i32, arg->len), arg->ptr);
    }
    printf("\n");

    *timer_queue.base = try_(PQue_initCap(
        typeInfo$(Task),
        heap_Page_allocator(&(heap_Page){}),
        32,
        wrapLam$(sort_CmpFn, lam_((anyptr_const lhs, anyptr_const rhs), cmp_Ord) {
            let delay_lhs = as$(const Task*, lhs);
            let delay_rhs = as$(const Task*, rhs);
            return time_Instant_cmp(delay_lhs->expires, delay_rhs->expires);
        })
    ));
    defer_(PQue_fini(timer_queue.base));

    var main_task = async_(asyncMain, ());
    {
        var mem = (Arr$$(128, u8)){};
        var buf = Arr_slice$(Sli$u8, mem, (0, 128));
        for_slice (timer_queue.list.items, time) {
            let fmt = try_(time_Instant_fmt(time->expires, buf));
            printf("%*s\n", as$(i32, fmt.len), fmt.ptr);
        }
    }

    while_some(meta_castOpt$(Opt$Ptr$Task, PQue_deqOrNull(timer_queue.base)), delay) {
        let now = time_Instant_now();
        if (time_Instant_lt(now, delay->expires)) {
            time_sleep(time_Instant_durationSince(delay->expires, now));
        }
        printf("debug: [event loop] resuming task (%zx)\n", as$(u64, delay->frame));
        resume_(delay->frame);
    }

    nosuspend_await_(main_task);
    return_ok({});
} $unguarded;

/*
clang -std=c17 -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\include -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\libs\BlocksRuntime\include -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\ -DCOMP -Wall -Wextra -funsigned-char -fblocks -DBlocksRuntime_STATIC -g3 -O0 -Wall -Werror C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\mockup-async.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ArrList.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\claim_assert.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\debug_assert.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\debug_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Err.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ErrTrace.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\fs_dir.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Arena.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Classic.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Fixed.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_Page.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\heap_ThrdSafe.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\io_Writer.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\list.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\log.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\mem_Allocator.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\mem_Tracker.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\meta_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\ptr.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Random.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\sort.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Str.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\TEST.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_Cond.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\Thrd_Mtx.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_common.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_Duration.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_Instant.c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src\time_SysTime.c -o C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\tests\mockup-async.exe "C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\libs\BlocksRuntime\lib\BlocksRuntime.lib" -static -pthread
 */
