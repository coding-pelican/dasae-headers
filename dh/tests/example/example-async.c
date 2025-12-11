#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/time/common.h"

#if UNUSED_CODE
static fn_((io_writeFmt(S$u8 stream, S_const$u8 format, ...))(E$usize)) $must_check;
static fn_((io_writeFmt(S$u8 stream, S_const$u8 format, ...))(E$usize) $guard) {
    va_list args = {};
    va_start(args, format);
    defer_(va_end(args));
    let written = vsnprintf(
        as$(char*)(stream.ptr), stream.len,
        as$(const char*)(format.ptr), args);
    if (written < 0) { return_err(E_Unexpected()); }
    return_ok(written);
} $unguarded_(fn);

static fn_((time_Duration_fmt(time_Duration self, S$u8 buf))(E$S_const$u8)) $must_check;
static fn_((time_Duration_fmt(time_Duration self, S$u8 buf))(E$S_const$u8) $scope) {
    usize written = 0;
    let total_nanos = (self.secs * time_nanos_per_sec) + self.nanos;
    if (total_nanos < time_nanos_per_micro) { // < 1 μs
        written = try_(io_writeFmt(
            buf, u8_l("%u ns"),
            self.nanos
        ));
    } else if (total_nanos < time_nanos_per_milli) { // < 1 ms
        written = try_(io_writeFmt(
            buf, u8_l("%g μs"),
            as$(f64)(total_nanos) / time_nanos_per_micro
        ));
    } else if (total_nanos < time_nanos_per_sec) { // < 1 s
        written = try_(io_writeFmt(
            buf, u8_l("%g ms"),
            as$(f64)(total_nanos) / time_nanos_per_milli
        ));
    } else if (self.secs < time_secs_per_min) { // < 1 min
        written = try_(io_writeFmt(
            buf, u8_l("%g s"),
            as$(f64)(total_nanos) / time_nanos_per_sec
        ));
    } else if (self.secs < time_secs_per_min * time_mins_per_hour) { // < 1 hour
        let mins = self.secs / time_secs_per_min;
        let secs = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu min %g s"),
            mins, as$(f64)(secs) + as$(f64)(self.nanos) / time_nanos_per_sec
        ));
    } else if (self.secs < time_secs_per_min * time_mins_per_hour * time_hours_per_day) { // < 1 day
        let hours = self.secs / (time_secs_per_min * time_mins_per_hour);
        let mins = (self.secs % (time_secs_per_min * time_mins_per_hour)) / time_secs_per_min;
        let secs = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu h %llu min %g s"),
            hours, mins, as$(f64)(secs) + as$(f64)(self.nanos) / time_nanos_per_sec
        ));
    } else {
        let days = self.secs / (time_secs_per_min * time_mins_per_hour * time_hours_per_day);
        let hours = (self.secs % (time_secs_per_min * time_mins_per_hour * time_hours_per_day))
                  / (time_secs_per_min * time_mins_per_hour);
        let mins = (self.secs % (time_secs_per_min * time_mins_per_hour)) / time_secs_per_min;
        let secs = self.secs % time_secs_per_min;

        written = try_(io_writeFmt(
            buf,
            u8_l("%llu d %llu h %llu min %g s"),
            days, hours, mins, as$(f64)(secs) + as$(f64)(self.nanos) / time_nanos_per_sec
        ));
    }

    if (buf.len <= written) {
        return_err(E_Unexpected());
    }
    return_ok(Str_slice(buf.as_const, 0, written));
} $unscoped_(fn);

static fn_((time_Instant_fmt(time_Instant self, S$u8 buf))(E$S_const$u8) $scope) {
    // Get Unix timestamp in seconds
    let unix_secs = time_Instant_toUnixEpoch(self);

    // Get the duration since Unix epoch to extract sub-second precision
    let unix_epoch_instant = time_Instant_fromUnixEpoch(unix_secs);
    let subsec_duration = time_Instant_durationSince(self, unix_epoch_instant);
    let subsec_nanos = subsec_duration.nanos;

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
        return_err(E_Unexpected());
    }
    return_ok(Str_slice(buf.as_const, 0, written));
} $unscoped_(fn);
#endif /* UNUSED_CODE */

#include "dh/ArrPQue.h"
#include "dh/sort.h"

#include "dh/async.h"
#include "dh/time.h"
#include "dh/io/stream.h"

// Delay type for scheduling tasks
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
T_use$((Task)(P, S, O));
T_use$((Task)(
    ArrPQue,
    ArrPQue_init,
    ArrPQue_fini,
    ArrPQue_enqueWithin,
    ArrPQue_deque,
));
$static ArrPQue$Task timer_queue = { 0 };

// Suspend for time in milliseconds
T_use_P$(Co_Ctx);
T_use_O$(P$Co_Ctx);
use_Co_Ctx$(Void);
async_fn_(waitForTime, (var_(caller, O$P$Co_Ctx); var_(name, S_const$u8); var_(ms, u64);), Void);
async_fn_scope(waitForTime, {}) {
    let_ignore = locals;
    io_stream_println(
        u8_l("debug: [waitForTime({:xz})] starting <- [{:s}({:xz})]"),
        intFromPtr(ctx->base), args->name, intFromPtr(orelse_((args->caller)(ctx->anyraw)))
    );
    suspend_({
        let addDur = time_Instant_addDuration;
        let now = time_Instant_now;
        let fromMs = time_Duration_fromMillis;
        ArrPQue_enqueWithin$Task(
            &timer_queue,
            lit$((Task){
                .frame = orelse_((args->caller)(ctx->anyraw)),
                .expires = addDur(now(), fromMs(args->ms)),
            })
        );
        io_stream_println(
            u8_l("debug: [waitForTime({:xz})] suspending -> [{:s}({:xz})]"),
            intFromPtr(ctx->base), args->name, intFromPtr(orelse_((args->caller)(ctx->anyraw)))
        );
    });
    io_stream_println(
        u8_l("debug: [waitForTime({:xz})] returning -> [{:s}({:xz})]"),
        intFromPtr(ctx->base), args->name, intFromPtr(orelse_((args->caller)(ctx->anyraw)))
    );
    areturn_({});
} $unscoped_(async_fn);

// Coroutine: waitUntilAndPrint
async_fn_(waitUntilAndPrint, (var_(caller, O$$(Co_Ctx*)); var_(time1, u64); var_(time2, u64); var_(name, S_const$u8);), Void);
async_fn_scope(waitUntilAndPrint, {
    time_Instant start;
    Co_CtxFn$(waitForTime) wait_ctx;
}) {
    locals->start = time_Instant_now();
    io_stream_println(
        u8_l("debug: [{:s}({:xz})] starting <- [asyncMain]"),
        args->name, intFromPtr(ctx->base)
    );

    // locals->wait_ctx = *async_ctx((waitForTime)(orelse_(args->caller, ctx->anyraw), args->name, args->time1));
    // while (resume_(&locals->wait_ctx)->state == Co_State_suspended) {
    //     suspend_(io_stream_println(u8_l("debug: [{:s}({:xz})] suspending -> [asyncMain]\n"), args->name, intFromPtr(ctx->base)));
    // }
    callAsync(&locals->wait_ctx, (waitForTime)(some(orelse_((args->caller)(ctx->anyraw))), args->name, args->time1));
    debug_assert(locals->wait_ctx.state == Co_State_ready);
    io_stream_println(
        u8_l("debug: [{:s}({:xz})] suspending until {:uz} ms"),
        args->name, intFromPtr(ctx->base), args->time1
    );
    {
        let asSecs = time_Duration_asSecs$f64;
        let durSince = time_Instant_durationSince;
        let now = time_Instant_now;
        io_stream_println(
            u8_l("debug: [{:s}({:xz})] it is now {:uz} ms since start!"),
            args->name, intFromPtr(ctx->base), as$(u64)((asSecs(durSince(now(), locals->start)) * 1000.0))
        );
    }

    // locals->wait_ctx = *async_ctx((waitForTime)(orelse_(args->caller, ctx->anyraw), args->name, args->time2));
    // while (resume_(&locals->wait_ctx)->state == Co_State_suspended) {
    //     suspend_(printf("debug: [%*s(%xz)] suspending -> [asyncMain]\n", as$(i32, args->name.len), args->name.ptr, as$(u64, ctx->base)));
    // }
    callAsync(&locals->wait_ctx, (waitForTime)(some(orelse_((args->caller)(ctx->anyraw))), args->name, args->time1));
    debug_assert(locals->wait_ctx.state == Co_State_ready);
    io_stream_println(
        u8_l("debug: [{:s}({:xz})] suspending until {:uz} ms"),
        args->name, intFromPtr(ctx->base), args->time2
    );
    {
        let asSecs = time_Duration_asSecs$f64;
        let durSince = time_Instant_durationSince;
        let now = time_Instant_now;
        io_stream_println(
            u8_l("debug: [{:s}({:xz})] it is now {:uz} ms since start!"),
            args->name, intFromPtr(ctx->base), as$(u64)((asSecs(durSince(now(), locals->start)) * 1000.0))
        );
    }

    io_stream_println(
        u8_l("debug: [{:s}({:xz})] returning -> [asyncMain]"),
        args->name, intFromPtr(ctx->base)
    );
    areturn_({});
} $unscoped_(async_fn);

#include "dh/main.h"
#include "dh/heap/Page.h"

#if UNUSED_CODE
TEST_fn_("Test time_Duration sort" $guard) {
    T_use_S$(time_Duration);
    T_use_ArrList$(time_Duration);
    var times = typeCast$((ArrList$time_Duration)(ArrList_init(
        typeInfo$(time_Duration),
        heap_Page_allocator(&(heap_Page){})
    )));
    defer_(ArrList_fini(times.base));
    {
        let start = time_Instant_now();
        try_(ArrList_append(times.base, meta_create$((time_Duration)({ .secs = 10 }))));
        try_(ArrList_append(times.base, meta_create$((time_Duration)({ .secs = 50 }))));
        try_(ArrList_append(times.base, meta_create$((time_Duration)({ .secs = 20 }))));
        try_(ArrList_append(times.base, meta_create$((time_Duration)({ .secs = 40 }))));
        try_(ArrList_append(times.base, meta_create$((time_Duration)({ .secs = 30 }))));

        var mem = (A$$(128, u8)){};
        var buf = A_slice$(S$u8, mem, $r(0, 128));
        for_(($s(times.items))(time) {
            let point = time_Instant_addDuration(start, *time);
            io_stream_print(u8_l("{:s}\n"), try_(time_Instant_fmt(point, buf)));
            let dur = time_Instant_durationSince(point, start);
            io_stream_print(u8_l("{:s}\n"), try_(time_Duration_fmt(dur, buf)));
        });
        io_stream_print(u8_l("\n"));

        try_(sort_stableSort(
            times.allocator, times.base->items, wrapLa$(sort_CmpFn, lam_((P_const$raw lhs, P_const$raw rhs), cmp_Ord) {
                let dur_lhs = as$(const time_Duration*)((lhs));
                let dur_rhs = as$(const time_Duration*)((rhs));
                return time_Duration_cmp(*dur_lhs, *dur_rhs);
            })
        ));
        for_(($s(times.items))(time) {
            let point = time_Instant_addDuration(start, *time);
            io_stream_print(u8_l("{:s}\n"), try_(time_Instant_fmt(point, buf)));
            let dur = time_Instant_durationSince(point, start);
            io_stream_print(u8_l("{:s}\n"), try_(time_Duration_fmt(dur, buf)));
        });
        io_stream_print(u8_l("\n"));
    }
    return_ok({});
} $unguarded_(TEST_fn);
#endif /* UNUSED_CODE */

// asyncMain wrapper
async_fn_(asyncMain, (S$S_const$u8 args;), Void);
async_fn_scope(asyncMain, {
    var_(tasks, A$$(2, Co_CtxFn$(waitUntilAndPrint)));
    var_(iter_resume, usize);
    var_(iter_await, usize);
}) {
    let_ignore = args;
    asg_lit((&locals->tasks)(A_init({
        [0] = *async_ctx((waitUntilAndPrint)(none(), 1000, 1200, u8_l("task-pair a"))),
        [1] = *async_ctx((waitUntilAndPrint)(none(), 500, 1300, u8_l("task-pair b"))),
    })));
    for (locals->iter_resume = 0; locals->iter_resume < A_len(locals->tasks); ++locals->iter_resume) {
        resume_(A_at((locals->tasks)[locals->iter_resume]));
    }
    for (locals->iter_await = 0; locals->iter_await < A_len(locals->tasks); ++locals->iter_await) {
        await_(A_at((locals->tasks)[locals->iter_await]));
    }
    io_stream_println(u8_l("debug: [asyncMain] all tasks completed"));
    areturn_({});
} $unscoped_(async_fn);


$static cmp_fn_ord$((Task)(lhs, rhs)) { return cmp_ord$(time_Instant)(lhs.expires, rhs.expires); }
$static cmp_fn_ordCtx$((Task)(lhs, rhs, ctx)) { return $unused(ctx), cmp_ord$(Task)(lhs, rhs); }
$static cmp_fn_u_ordCtx_default$((Task)(lhs, rhs, ctx));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    for_(($rf(0), $s(args))(idx, arg) {
        io_stream_print(u8_l("args[{:uz}]: {:s}\n"), idx, arg);
    });
    io_stream_print(u8_l("\n"));

    var page = lit0$((heap_Page));
    let gpa = heap_Page_allocator(&page);
    let ctx = lit$((ArrPQue_Ctx){
        .inner = u_anyP(&lit0$((const Void))),
        .ordFn = Task_u_ordCtx,
    });
    timer_queue = try_(ArrPQue_init$Task(gpa, 32, &ctx));
    defer_(ArrPQue_fini$Task(&timer_queue, gpa));

    var main_task = async_((asyncMain)());
    {
#if UNUSED_CODE
        var mem = (A$$(128, u8)){};
        var buf = A_ref$((S$u8)mem);
        for_(($s(timer_queue.items))(time) {
            io_stream_print(u8_l("{:s}\n"), try_(time_Instant_fmt(time->expires, buf)));
        });
#endif /* UNUSED_CODE */
    }

    while_some(ArrPQue_deque$Task(&timer_queue), delay) {
        let now = time_Instant_now();
        if (time_Instant_lt(now, delay.expires)) {
            time_sleep(time_Instant_durationSince(delay.expires, now));
        }
        io_stream_print(u8_l("debug: [event loop] resuming task ({:xz})\n"), as$(u64)(delay.frame));
        resume_(delay.frame);
    }

    nosuspend_(await_(resume_(main_task)));
    return_ok({});
} $unguarded_(fn);
