#include "dh/thrd/Que.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_Que__remaining(time_Awake clock, time_Awake_Inst started, time_Dur timeout))(time_Dur));

fn_((thrd_Que_init(u_S$raw buf))(thrd_Que)) {
    return (thrd_Que){
        .que = ArrQue_fixed(buf),
        .closed = false,
        .lock = thrd_Mtx_init(),
        .not_empty = thrd_Cond_init(),
        .not_full = thrd_Cond_init(),
    };
};

fn_((thrd_Que_fini(thrd_Que* self, TypeInfo type))(void)) {
    let_ignore = type;
    self->closed = true;
    ArrQue_clearRetainingCap(&self->que);
    thrd_Cond_fini(&self->not_full);
    thrd_Cond_fini(&self->not_empty);
    thrd_Mtx_fini(&self->lock);
};

fn_((thrd_Que_len(thrd_Que* self))(usize) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_len(self->que));
} $unguarded(fn);

fn_((thrd_Que_cap(thrd_Que* self))(usize) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_cap(self->que));
} $unguarded(fn);

fn_((thrd_Que_close(thrd_Que* self))(void) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    self->closed = true;
    thrd_Cond_broadcast(&self->not_empty);
    thrd_Cond_broadcast(&self->not_full);
} $unguarded(fn);

fn_((thrd_Que_isClosed(thrd_Que* self))(bool) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(self->closed);
} $unguarded(fn);

fn_((thrd_Que_send(thrd_Que* self, u_V$raw item))(thrd_chan_WaitE$void) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    while (!self->closed && ArrQue_isFull(self->que)) {
        thrd_Cond_wait(&self->not_full, &self->lock);
    }
    if (self->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    ArrQue_enqueWithin(&self->que, item);
    thrd_Cond_signal(&self->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_Que_trySend(thrd_Que* self, u_V$raw item))(thrd_chan_E$void) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    if (self->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    if (ArrQue_isFull(self->que)) {
        return_err(E_cause$thrd_chan_Full());
    }
    ArrQue_enqueWithin(&self->que, item);
    thrd_Cond_signal(&self->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_Que_sendFor(thrd_Que* self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    while (!self->closed && ArrQue_isFull(self->que)) {
        let remaining = thrd_Que__remaining(clock, started, dur);
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(thrd_Cond_timedWait(&self->not_full, &self->lock, remaining));
    }
    if (self->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    ArrQue_enqueWithin(&self->que, item);
    thrd_Cond_signal(&self->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_Que_recv(thrd_Que* self, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    while (!self->closed && ArrQue_isEmpty(self->que)) {
        thrd_Cond_wait(&self->not_empty, &self->lock);
    }
    if (ArrQue_isEmpty(self->que)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    let_ignore = unwrap_(ArrQue_deque(&self->que, out));
    thrd_Cond_signal(&self->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_Que_tryRecv(thrd_Que* self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    if (ArrQue_isEmpty(self->que)) {
        if (self->closed) {
            return_err(E_cause$thrd_chan_Closed());
        }
        return_err(E_cause$thrd_chan_Empty());
    }
    let_ignore = unwrap_(ArrQue_deque(&self->que, out));
    thrd_Cond_signal(&self->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_Que_recvFor(thrd_Que* self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    while (!self->closed && ArrQue_isEmpty(self->que)) {
        let remaining = thrd_Que__remaining(clock, started, dur);
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(thrd_Cond_timedWait(&self->not_empty, &self->lock, remaining));
    }
    if (ArrQue_isEmpty(self->que)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    let_ignore = unwrap_(ArrQue_deque(&self->que, out));
    thrd_Cond_signal(&self->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_Que__remaining(time_Awake clock, time_Awake_Inst started, time_Dur timeout))(time_Dur)) {
    let elapsed = time_Awake_Inst_elapsed(started, clock);
    return orelse_((time_Dur_subChkd(timeout, elapsed))(time_Dur_zero));
};
