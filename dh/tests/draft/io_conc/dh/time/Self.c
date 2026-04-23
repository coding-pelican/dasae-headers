#include "Inst.h"
#include "Clock.h"
#include "../exec/Coop.h"

#if plat_is_windows
#include "dh/os/windows/sync.h"
#endif /* plat_is_windows */
#if plat_based_unix
#include <time.h>
#endif /* plat_based_unix */

/*========== Internal Declarations ==========================================*/

$static fn_((time_direct__nowClock(P$raw ctx))(time_Clock));
$static fn_((time_direct__nowInst(P$raw ctx))(time_Inst));
$static fn_((time_direct__freqInst(P$raw ctx))(time_Inst));
$static fn_((time_direct__freqInvInst(P$raw ctx))(f64));
$static fn_((time_direct__offsetInst(P$raw ctx))(time_Inst));
$attr($must_check)
$static fn_((time_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));

$static fn_((time_evented__nowClock(P$raw ctx))(time_Clock));
$static fn_((time_evented__nowInst(P$raw ctx))(time_Inst));
$static fn_((time_evented__freqInst(P$raw ctx))(time_Inst));
$static fn_((time_evented__freqInstInv(P$raw ctx))(f64));
$static fn_((time_evented__offsetInst(P$raw ctx))(time_Inst));
$attr($must_check)
$static fn_((time_evented__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));

T_use$((exec_Timer)(ArrPQue_enque));

/*========== External Definitions ===========================================*/

fn_((time_direct(void))(time_Self)) {
    $static var_(ctx, Void) $like_ref = cleared();
    $static let_(vtbl, time_Self_Vtbl) $like_ref = { {
        .nowClockFn = time_direct__nowClock,
        .nowInstFn = time_direct__nowInst,
        .freqInstFn = time_direct__freqInst,
        .freqInstInvFn = time_direct__freqInvInst,
        .offsetInstFn = time_direct__offsetInst,
        .sleepFn = time_direct__sleep,
    } };
    return (time_Self){
        .ctx = &ctx,
        .vtbl = vtbl,
    };
};

fn_((time_evented(exec_Coop* coop))(time_Self)) {
    $static let_(vtbl, time_Self_Vtbl) $like_ref = { {
        .nowClockFn = time_evented__nowClock,
        .nowInstFn = time_evented__nowInst,
        .freqInstFn = time_evented__freqInst,
        .freqInstInvFn = time_evented__freqInstInv,
        .offsetInstFn = time_evented__offsetInst,
        .sleepFn = time_evented__sleep,
    } };
    return (time_Self){
        .ctx = coop,
        .vtbl = vtbl,
    };
};

fn_((time_sleep(time_Self self, time_Dur dur))(Sched_Cancelable$void)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->sleepFn);
    return self.vtbl->sleepFn(self.ctx, dur);
};

fn_((time_sleepSecs(time_Self self, u64 secs))(Sched_Cancelable$void)) {
    return time_sleep(self, time_Dur_fromSecs(secs));
};

fn_((time_sleepMillis(time_Self self, u64 millis))(Sched_Cancelable$void)) {
    return time_sleep(self, time_Dur_fromMillis(millis));
};

fn_((time_sleepMicros(time_Self self, u64 micros))(Sched_Cancelable$void)) {
    return time_sleep(self, time_Dur_fromMicros(micros));
};

fn_((time_sleepNanos(time_Self self, u32 nanos))(Sched_Cancelable$void)) {
    return time_sleep(self, time_Dur_fromNanos(nanos));
};

/*========== Internal Definitions ===========================================*/

fn_((time_direct__freqInst(P$raw ctx))(time_Inst)) {
    let_ignore = ctx;
#if plat_is_windows
    var freq = l0$((time_InstPlatform));
    if (!QueryPerformanceFrequency(&freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    return (time_Inst){ .impl = freq };
#elif plat_based_unix
    return (time_Inst){
        .impl = {
            .tv_sec = 1,
            .tv_nsec = time_Inst_nanos_per_sec,
        },
    };
#else
    return (time_Inst){ .impl = cleared() };
#endif
};

fn_((time_direct__freqInvInst(P$raw ctx))(f64)) {
    let_ignore = ctx;
#if plat_is_windows
    var freq = l0$((time_InstPlatform));
    if (!QueryPerformanceFrequency(&freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    return 1.0 / as$(f64)(freq.QuadPart);
#elif plat_based_unix
    return 1.0 / as$(f64)(time_Inst_nanos_per_sec);
#else
    return f64_nan;
#endif
};

fn_((time_direct__offsetInst(P$raw ctx))(time_Inst)) {
    let_ignore = ctx;
    return (time_Inst){ .impl = cleared() };
};

fn_((time_direct__nowInst(P$raw ctx))(time_Inst)) {
    let_ignore = ctx;
    var inst = l0$((time_InstPlatform));
#if plat_is_windows
    QueryPerformanceCounter(&inst);
#elif plat_based_unix
    clock_gettime(CLOCK_MONOTONIC, &inst);
#else
#endif
    return (time_Inst){ .impl = inst };
};

fn_((time_direct__nowClock(P$raw ctx))(time_Clock)) {
    let_ignore = ctx;
    var clock = l0$((time_ClockPlatform));
#if plat_is_windows
    GetSystemTimeAsFileTime(&clock);
#elif plat_based_unix
    clock_gettime(CLOCK_REALTIME, &clock);
#else
#endif
    return (time_Clock){ .impl = clock };
};

fn_((time_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void) $scope) {
    let_ignore = ctx;
#if plat_is_windows
    let millis = dur.secs * time_millis_per_sec + dur.nanos / time_nanos_per_milli;
    Sleep(as$(DWORD)(millis));
#elif plat_based_unix
    struct timespec req = {
        .tv_sec = as$(time_t)(dur.secs),
        .tv_nsec = as$(long)(dur.nanos),
    };
    struct timespec rem = cleared();
    while (nanosleep(&req, &rem) == -1) req = rem;
#else
#endif
    return_ok({});
} $unscoped(fn);

/*========== Evented Definitions ============================================*/

$static fn_((time_evented__now(exec_Coop* self))(time_Inst)) {
    return self->timed.clock.vtbl->nowInstFn(self->timed.clock.ctx);
};

fn_((time_evented__nowClock(P$raw ctx))(time_Clock)) {
    let self = as$(exec_Coop*)(ctx);
    return self->timed.clock.vtbl->nowClockFn(self->timed.clock.ctx);
};

fn_((time_evented__nowInst(P$raw ctx))(time_Inst)) {
    return time_evented__now(as$(exec_Coop*)(ctx));
};

fn_((time_evented__freqInst(P$raw ctx))(time_Inst)) {
    let self = as$(exec_Coop*)(ctx);
    return self->timed.clock.vtbl->freqInstFn(self->timed.clock.ctx);
};

fn_((time_evented__freqInstInv(P$raw ctx))(f64)) {
    let self = as$(exec_Coop*)(ctx);
    return self->timed.clock.vtbl->freqInstInvFn(self->timed.clock.ctx);
};

fn_((time_evented__offsetInst(P$raw ctx))(time_Inst)) {
    let self = as$(exec_Coop*)(ctx);
    return self->timed.clock.vtbl->offsetInstFn(self->timed.clock.ctx);
};

fn_((time_evented__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void) $scope) {
    let self = as$(exec_Coop*)(ctx);
    let timed = &self->timed;
    let lane = &self->timed.lane;
    let deadline = orelse_((time_Inst_addChkdDur(time_evented__now(self), dur))(
        return_ok({})
    ));

    if_some((lane->task_curr)(task)) {
        task->state = exec_Task_State_waiting;
        let_(timer, exec_Timer) = {
            .deadline = deadline,
            .task = task,
        };
        let_ignore = catch_((ArrPQue_enque$exec_Timer(&timed->tasks_timer, lane->gpa, timer))($ignore, {
            task->state = exec_Task_State_canceled;
            return_err(Sched_Cancelable_Canceled());
        }));
        return_ok_void(exec_Lane_yield(lane));
    }

    return_ok_void(exec_LaneTimed_runUntil(timed, deadline));
} $unscoped(fn);
