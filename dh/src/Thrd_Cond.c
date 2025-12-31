#include "dh/Thrd/Cond.h"
#include "dh/Thrd/Mtx.h"
#include "dh/time/Duration.h"
#include "dh/math/common.h"

/*========== Internal Declarations ==========================================*/

pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(
        $attr($inline_always)
        $static fn_((Thrd_Cond__pthread_init(void))(Thrd_Cond));
        $attr($inline_always)
        $static fn_((Thrd_Cond__pthread_fini(P$$(Thrd_Cond) self))(void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__pthread_wait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx))(void));
        $attr($inline_always $must_check)
        $static fn_((Thrd_Cond__pthread_timedWait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx, time_Duration duration))(Thrd_Cond_Err$void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__pthread_signal(P$$(Thrd_Cond) self))(void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__pthread_broadcast(P$$(Thrd_Cond) self))(void));
    ),
    pp_else_(
        $attr($inline_always)
        $static fn_((Thrd_Cond__common_init(void))(Thrd_Cond));
        $attr($inline_always)
        $static fn_((Thrd_Cond__common_fini(P$$(Thrd_Cond) self))(void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__common_wait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx))(void));
        $attr($inline_always $must_check)
        $static fn_((Thrd_Cond__common_timedWait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx, time_Duration duration))(Thrd_Cond_Err$void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__common_signal(P$$(Thrd_Cond) self))(void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__common_broadcast(P$$(Thrd_Cond) self))(void));
    ));

$static let Thrd_Cond__init = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_init),
    pp_else_(Thrd_Cond__common_init));
$static let Thrd_Cond__fini = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_fini),
    pp_else_(Thrd_Cond__common_fini));
$static let Thrd_Cond__wait = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_wait),
    pp_else_(Thrd_Cond__common_wait));
$static let Thrd_Cond__timedWait = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_timedWait),
    pp_else_(Thrd_Cond__common_timedWait));
$static let Thrd_Cond__signal = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_signal),
    pp_else_(Thrd_Cond__common_signal));
$static let Thrd_Cond__broadcast = pp_if_(Thrd_Cond_use_pthread)(
    pp_then_(Thrd_Cond__pthread_broadcast),
    pp_else_(Thrd_Cond__common_broadcast));

/*========== External Definitions ===========================================*/

fn_((Thrd_Cond_init(void))(Thrd_Cond)) {
    return Thrd_Cond__init();
};

fn_((Thrd_Cond_fini(Thrd_Cond* self))(void)) {
    Thrd_Cond__fini(self);
};

fn_((Thrd_Cond_wait(Thrd_Cond* self, Thrd_Mtx* mtx))(void)) {
    Thrd_Cond__wait(self, mtx);
};

fn_((Thrd_Cond_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration))(Thrd_Cond_Err$void)) {
    return Thrd_Cond__timedWait(self, mtx, duration);
};

fn_((Thrd_Cond_signal(Thrd_Cond* self))(void)) {
    Thrd_Cond__signal(self);
};

fn_((Thrd_Cond_broadcast(Thrd_Cond* self))(void)) {
    Thrd_Cond__broadcast(self);
};

/*========== Internal Definitions ===========================================*/

/*========== Pthread ==========*/

#if Thrd_Cond_use_pthread
fn_((Thrd_Cond__pthread_init(void))(Thrd_Cond)) {
    return (Thrd_Cond){ .impl = PTHREAD_COND_INITIALIZER };
};

fn_((Thrd_Cond__pthread_fini(Thrd_Cond* self))(void)) {
    pthread_cond_destroy(&self->impl);
};

fn_((Thrd_Cond__pthread_wait(Thrd_Cond* self, Thrd_Mtx* mtx))(void)) {
    pthread_cond_wait(&self->impl, &mtx->impl);
};

fn_((Thrd_Cond__pthread_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration))(Thrd_Cond_Err$void) $scope) {
    const struct timespec ts = {
        .tv_sec = as$(time_t)(duration.secs),
        .tv_nsec = as$(long)(duration.nanos),
    };
    switch ((pthread_cond_timedwait(&self->impl, &mtx->impl, &ts))) {
    case_((/*SUCCESS*/ 0))          return_ok({}) $end(case);
    case_((/*TIMEDOUT*/ ETIMEDOUT)) return_err(Thrd_Cond_Err_Timeout()) $end(case);
    default_() claim_unreachable $end(default);
    }
} $unscoped_(fn);

fn_((Thrd_Cond__pthread_signal(Thrd_Cond* self))(void)) {
    pthread_cond_signal(&self->impl);
};

fn_((Thrd_Cond__pthread_broadcast(Thrd_Cond* self))(void)) {
    pthread_cond_broadcast(&self->impl);
};

#else /*========== Common ==========*/

typedef enum_(Thrd_Cond__Notify $bits(8)) {
    Thrd_Cond__Notify_one = 0, // wake up only one thread
    Thrd_Cond__Notify_all = 1, // wake up all threads
} Thrd_Cond__Notify;

/* === Declarations === */

pp_if_(Thrd_Cond_has_specialized)(
    pp_then_(pp_expand(
        pp_switch_ pp_begin(plat_type)(
            pp_case_((plat_type_windows)(
                $attr($inline_always)
                $static fn_((Thrd_Cond__windows_impl_init(void))(Thrd_Cond));
                $attr($inline_always)
                $static fn_((Thrd_Cond__windows_impl_fini(P$$(Thrd_Cond) self))(void));
                $attr($inline_always $must_check)
                $static fn_((Thrd_Cond__windows_impl_wait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx, O$time_Duration timeout))(Thrd_Cond_Err$void));
                $attr($inline_always)
                $static fn_((Thrd_Cond__windows_impl_wake(P$$(Thrd_Cond) self, Thrd_Cond__Notify notify))(void))
            ))
        ) pp_end
    )),
    pp_else_(
        $attr($inline_always)
        $static fn_((Thrd_Cond__default_impl_init(void))(Thrd_Cond));
        $attr($inline_always)
        $static fn_((Thrd_Cond__default_impl_fini(P$$(Thrd_Cond) self))(void));
        $attr($inline_always $must_check)
        $static fn_((Thrd_Cond__default_impl_wait(P$$(Thrd_Cond) self, P$$(Thrd_Mtx) mtx, O$time_Duration timeout))(Thrd_Cond_Err$void));
        $attr($inline_always)
        $static fn_((Thrd_Cond__default_impl_wake(P$$(Thrd_Cond) self, Thrd_Cond__Notify notify))(void))
    ));

$static let Thrd_Cond__impl_init = pp_if_(Thrd_Cond_has_specialized)(
    pp_then_(pp_expand(
        pp_switch_ pp_begin(plat_type)(
            pp_case_((plat_type_windows)(
                Thrd_Cond__windows_impl_init
            ))
        ) pp_end
    )),
    pp_else_(Thrd_Cond__default_impl_init));
$static let Thrd_Cond__impl_fini = pp_if_(Thrd_Cond_has_specialized)(
    pp_then_(pp_expand(
        pp_switch_ pp_begin(plat_type)(
            pp_case_((plat_type_windows)(
                Thrd_Cond__windows_impl_fini
            ))
        ) pp_end
    )),
    pp_else_(Thrd_Cond__default_impl_fini));
$static let Thrd_Cond__impl_wait = pp_if_(Thrd_Cond_has_specialized)(
    pp_then_(pp_expand(
        pp_switch_ pp_begin(plat_type)(
            pp_case_((plat_type_windows)(
                Thrd_Cond__windows_impl_wait
            ))
        ) pp_end
    )),
    pp_else_(Thrd_Cond__default_impl_wait));
$static let Thrd_Cond__impl_wake = pp_if_(Thrd_Cond_has_specialized)(
    pp_then_(pp_expand(
        pp_switch_ pp_begin(plat_type)(
            pp_case_((plat_type_windows)(
                Thrd_Cond__windows_impl_wake
            ))
        ) pp_end
    )),
    pp_else_(Thrd_Cond__default_impl_wake));

/* === Internal Definitions === */

fn_((Thrd_Cond__common_init(void))(Thrd_Cond)) {
    return Thrd_Cond__impl_init();
};

fn_((Thrd_Cond__common_fini(Thrd_Cond* self))(void)) {
    Thrd_Cond__impl_fini(self);
};

fn_((Thrd_Cond__common_wait(Thrd_Cond* self, Thrd_Mtx* mtx))(void) $scope) {
    return_void(catch_((Thrd_Cond__impl_wait(self, mtx, none$((O$time_Duration))))($ignore, claim_unreachable)));
} $unscoped_(fn);

fn_((Thrd_Cond__common_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration))(Thrd_Cond_Err$void)) {
    return Thrd_Cond__impl_wait(self, mtx, some$((O$time_Duration)(duration)));
};

fn_((Thrd_Cond__common_signal(Thrd_Cond* self))(void)) {
    Thrd_Cond__impl_wake(self, Thrd_Cond__Notify_one);
};

fn_((Thrd_Cond__common_broadcast(Thrd_Cond* self))(void)) {
    Thrd_Cond__impl_wake(self, Thrd_Cond__Notify_all);
};

/* === Definitions === */

/* --- Default --- */

#if !Thrd_Cond_has_specialized
#define Thrd_Cond__default_one_waiter 1
#define Thrd_Cond__default_waiter_mask 0xffff

#define Thrd_Cond__default_one_signal 1 << 16
#define Thrd_Cond__default_signal_mask 0xffff << 16

$attr($inline_always)
$static fn_((Thrd_Cond__default_impl_init(void))(Thrd_Cond)) {
    return (Thrd_Cond){
        .impl = {
            .state = atom_V_init(0),
            .epoch = atom_V_init(0),
        }
    };
};

$attr($inline_always)
$static fn_((Thrd_Cond__default_impl_fini(Thrd_Cond* self))(void)) {
    atom_V_store(&self->impl.epoch, 0, atom_MemOrd_monotonic);
    atom_V_store(&self->impl.state, 0, atom_MemOrd_monotonic);
};

// Observe the epoch, then check the state again to see if we should wake up.
// The epoch must be observed before we check the state or we could potentially miss a wake() and deadlock:
//
// - T1: s = LOAD(&state)
// - T2: UPDATE(&s, signal)
// - T2: UPDATE(&epoch, 1) + FUTEX_WAKE(&epoch)
// - T1: e = LOAD(&epoch) (was reordered after the state load)
// - T1: s & signals == 0 -> FUTEX_WAIT(&epoch, e) (missed the state update + the epoch change)
//
// Acquire barrier to ensure the epoch load happens before the state load.
fn_((Thrd_Cond__default_impl_wait(Thrd_Cond* self, Thrd_Mtx* mtx, O$time_Duration timeout))(Thrd_Cond_Err$void)) {
    var epoch = atom_V_load(&self->impl.epoch, atom_MemOrd_acquire);
    var state = atom_V_fetchAdd(&self->impl.state, Thrd_Cond__default_one_waiter, atom_MemOrd_monotonic);
    debug_assert(state & Thrd_Cond__default_waiter_mask != Thrd_Cond__default_waiter_mask);
    state += Thrd_Cond__default_one_waiter;

    Thrd_Mtx_unlock(mtx);
    defer_(Thrd_Mtx_lock(mtx));

    var deadline = Thrd_Ftx_Deadline_init(timeout);
    while (true) {
        catch_((Thrd_Ftx_Deadline_wait(&deadline, &self->impl.epoch, epoch))(err, {
            // On timeout, we must decrement the waiter we added above.
            while (true) {
                // If there's a signal when we're timing out, consume it and report being woken up instead.
                // Acquire barrier ensures code before the wake() which added the signal happens before we decrement it and return.
                while (state & Thrd_Cond__default_signal_mask != 0) {
                    let new_state = state - Thrd_Cond__default_one_waiter - Thrd_Cond__default_one_signal;
                    state = orelse_((atom_V_cmpXchgWeak(&self->impl.state, state, new_state, atom_MemOrd_acquire, atom_MemOrd_monotonic))(return_ok({})));
                }

                // Remove the waiter we added and officially return timed out.
                let new_state = state - Thrd_Cond__default_one_waiter;
                state = orelse_((atom_V_cmpXchgWeak(&self->impl.state, state, new_state, atom_MemOrd_monotonic, atom_MemOrd_monotonic))(return_err(err)));
            }
        }));

        epoch = atom_V_load(&self->impl.epoch, atom_MemOrd_acquire);
        state = atom_V_load(&self->impl.state, atom_MemOrd_monotonic);

        // Try to wake up by consuming a signal and decremented the waiter we added previously.
        // Acquire barrier ensures code before the wake() which added the signal happens before we decrement it and return.
        while (state & Thrd_Cond__default_signal_mask != 0) {
            let new_state = state - Thrd_Cond__default_one_waiter - Thrd_Cond__default_one_signal;
            state = orelse_((atom_V_cmpXchgWeak(&self->impl.state, state, new_state, atom_MemOrd_acquire, atom_MemOrd_monotonic))(return_ok({})));
        }
    }
    return_ok({});
} $unguarded_(fn);

fn_((Thrd_Cond__default_impl_wake(Thrd_Cond* self, Thrd_Cond__Notify notify))(void)) {
    var state = atom_V_load(&self->impl.state, atom_MemOrd_monotonic);
    while (true) {
        let waiters = (state & Thrd_Cond__default_waiter_mask) / Thrd_Cond__default_one_waiter;
        let signals = (state & Thrd_Cond__default_signal_mask) / Thrd_Cond__default_one_signal;

        // Reserves which waiters to wake up by incrementing the signals count.
        // Therefore, the signals count is always less than or equal to the waiters count.
        // We don't need to Futex.wake if there's nothing to wake up or if other wake() threads have reserved to wake up the current waiters.
        let wakeable = waiters - signals;
        if (wakeable == 0) { return; }

        let to_wake = expr_(u32 $scope)(switch (notify) {
            case_((Thrd_Cond__Notify_one)) $break_(1) $end(case);
            case_((Thrd_Cond__Notify_all)) $break_(wakeable) $end(case);
        }) $unscoped_(expr);

        // Reserve the amount of waiters to wake by incrementing the signals count.
        // Release barrier ensures code before the wake() happens before the signal it posted and consumed by the wait() threads.
        let new_state = state + (Thrd_Cond__default_one_signal * to_wake);
        state = orelse_((atom_V_cmpXchgWeak(&self->impl.state, state, new_state, atom_MemOrd_release, atom_MemOrd_monotonic))({
            // Wake up the waiting threads we reserved above by changing the epoch value.
            // NOTE: a waiting thread could miss a wake up if *exactly* ((1<<32)-1) wake()s happen between it observing the epoch and sleeping on it.
            // This is very unlikely due to how many precise amount of Futex.wake() calls that would be between the waiting thread's potential preemption.
            //
            // Release barrier ensures the signal being added to the state happens before the epoch is changed.
            // If not, the waiting thread could potentially deadlock from missing both the state and epoch change:
            //
            // - T2: UPDATE(&epoch, 1) (reordered before the state change)
            // - T1: e = LOAD(&epoch)
            // - T1: s = LOAD(&state)
            // - T2: UPDATE(&state, signal) + FUTEX_WAKE(&epoch)
            // - T1: s & signals == 0 -> FUTEX_WAIT(&epoch, e) (missed both epoch change and state change)
            let_ignore = atom_V_fetchAdd(&self->impl.epoch, 1, atom_MemOrd_release);
            return Thrd_Ftx_wake(&self->impl.epoch, to_wake);
        }));
    }
};
#endif /* !Thrd_Cond_has_specialized */

/* --- Windows --- */

#if Thrd_Cond_has_specialized && plat_is_windows
fn_((Thrd_Cond__windows_impl_init(void))(Thrd_Cond)) {
    return (Thrd_Cond){ .impl.inner = CONDITION_VARIABLE_INIT };
};

fn_((Thrd_Cond__windows_impl_fini(Thrd_Cond* self))(void)) {
    let_ignore = self;
};

fn_((Thrd_Cond__windows_impl_wait(Thrd_Cond* self, Thrd_Mtx* mtx, O$time_Duration timeout))(Thrd_Cond_Err$void) $scope) {
    claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(DWORD), packTypeInfo$(u32)));
    var timeout_overflowed = false;
    var timeout_ms = u32_limit_max;
    if_some((timeout)(duration)) {
        // Round the nanoseconds to the nearest millisecond,
        // then saturating cast it to windows DWORD for use in kernel32 call.
        let secs_as_ms = u64_mulSat(duration.secs, time_millis_per_sec);
        let nanos_as_ms = u32_addSat(duration.nanos, time_nanos_per_milli / 2) / time_nanos_per_milli;
        let total_ms = u64_addSat(secs_as_ms, nanos_as_ms);
        timeout_ms = orelse_((math_intCast$((O$$(u32))(total_ms)))(u32_limit_max));

        if (timeout_ms == u32_limit_max) {
            timeout_overflowed = true;
            timeout_ms -= 1;
        }
    }
    let rc = SleepConditionVariableSRW(
        &self->impl.inner,
        &mtx->impl.inner,
        timeout_ms,
        0 // the srwlock was assumed to acquired in exclusive mode not shared
    );
    if (!rc) {
        debug_assert(GetLastError() == ERROR_TIMEOUT);
        if (!timeout_overflowed) { return_err(Thrd_Cond_Err_Timeout()); }
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_Cond__windows_impl_wake(Thrd_Cond* self, Thrd_Cond__Notify notify))(void)) {
    switch (notify) {
    case_((Thrd_Cond__Notify_one)) WakeConditionVariable(&self->impl.inner) $end(case);
    case_((Thrd_Cond__Notify_all)) WakeAllConditionVariable(&self->impl.inner) $end(case);
    }
};
#endif /* Thrd_Cond_has_specialized && plat_is_windows */
#endif
