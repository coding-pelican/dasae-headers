#include "dh/thrd/SPSC.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_SPSC__grip(thrd_SPSC* self))(ArrQue_Grip));
$static fn_((thrd_SPSC__remaining(time_Awake clock, time_Awake_Inst started, time_Dur timeout))(time_Dur));

fn_((thrd_SPSC_init(u_S$raw buf))(thrd_SPSC)) {
    return (thrd_SPSC){
        .buf = buf.raw,
        .head = 0,
        .len = 0,
        .closed = false,
        .lock = thrd_Mtx_init(),
        .not_empty = thrd_Cond_init(),
        .not_full = thrd_Cond_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_SPSC_fini(thrd_SPSC* self, TypeInfo type))(void) $guard) {
    thrd_Mtx_lock(&self->lock);
    self->closed = true;
    var grip = thrd_SPSC__grip(self);
    ArrQue_clearRetainingCap(&grip.ctx);
    let_ignore = type;
    ArrQue_Grip_release(&grip, $typed(self->type));
    thrd_Mtx_unlock(&self->lock);
    thrd_Cond_fini(&self->not_full);
    thrd_Cond_fini(&self->not_empty);
    thrd_Mtx_fini(&self->lock);
} $unguarded(fn);

fn_((thrd_SPSC_tx(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Tx)) {
    return (thrd_SPSC_Tx){ .chan = self, .type = $typing(type) };
};

fn_((thrd_SPSC_Tx_send(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_WaitE$void) $guard) {
    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    while (!self.chan->closed && self.chan->len == self.chan->buf.len) {
        thrd_Cond_wait(&self.chan->not_full, &self.chan->lock);
    }
    if (self.chan->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    var grip = thrd_SPSC__grip(self.chan);
    ArrQue_enqueWithin(&grip.ctx, item);
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_SPSC_Tx_trySend(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_E$void) $guard) {
    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    if (self.chan->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    if (self.chan->len == self.chan->buf.len) {
        return_err(E_cause$thrd_chan_Full());
    }
    var grip = thrd_SPSC__grip(self.chan);
    ArrQue_enqueWithin(&grip.ctx, item);
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_SPSC_Tx_sendFor(thrd_SPSC_Tx self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    while (!self.chan->closed && self.chan->len == self.chan->buf.len) {
        let remaining = thrd_SPSC__remaining(clock, started, dur);
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(thrd_Cond_timedWait(&self.chan->not_full, &self.chan->lock, remaining));
    }
    if (self.chan->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    var grip = thrd_SPSC__grip(self.chan);
    ArrQue_enqueWithin(&grip.ctx, item);
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_empty);
    return_ok({});
} $unguarded(fn);

fn_((thrd_SPSC_rx(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Rx)) {
    return (thrd_SPSC_Rx){ .chan = self, .type = $typing(type) };
};

fn_((thrd_SPSC_Rx_tryRecv(thrd_SPSC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    if (self.chan->len == 0) {
        if (self.chan->closed) {
            return_err(E_cause$thrd_chan_Closed());
        }
        return_err(E_cause$thrd_chan_Empty());
    }
    var grip = thrd_SPSC__grip(self.chan);
    let_ignore = unwrap_(ArrQue_deque(&grip.ctx, out));
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_SPSC_Rx_recv(thrd_SPSC_Rx self, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    while (!self.chan->closed && self.chan->len == 0) {
        thrd_Cond_wait(&self.chan->not_empty, &self.chan->lock);
    }
    if (self.chan->len == 0) {
        return_err(E_cause$thrd_chan_Closed());
    }
    var grip = thrd_SPSC__grip(self.chan);
    let_ignore = unwrap_(ArrQue_deque(&grip.ctx, out));
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_SPSC_Rx_recvFor(thrd_SPSC_Rx self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lock(&self.chan->lock);
    defer_(thrd_Mtx_unlock(&self.chan->lock));

    while (!self.chan->closed && self.chan->len == 0) {
        let remaining = thrd_SPSC__remaining(clock, started, dur);
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(thrd_Cond_timedWait(&self.chan->not_empty, &self.chan->lock, remaining));
    }
    if (self.chan->len == 0) {
        return_err(E_cause$thrd_chan_Closed());
    }
    var grip = thrd_SPSC__grip(self.chan);
    let_ignore = unwrap_(ArrQue_deque(&grip.ctx, out));
    ArrQue_Grip_release(&grip, $typed(self.chan->type));
    thrd_Cond_signal(&self.chan->not_full);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_SPSC_open(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Chans)) {
    return (thrd_SPSC_Chans)tie_(
        (thrd_SPSC_tx(self, type)),
        (thrd_SPSC_rx(self, type))
    );
};

fn_((thrd_SPSC_close(thrd_SPSC* self, TypeInfo type))(void) $guard) {
    let_ignore = type;
    thrd_Mtx_lock(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    self->closed = true;
    thrd_Cond_broadcast(&self->not_empty);
    thrd_Cond_broadcast(&self->not_full);
} $unguarded(fn);

fn_((thrd_SPSC__grip(thrd_SPSC* self))(ArrQue_Grip)) {
    return ArrQue_grip((u_S$raw){ .raw = self->buf, .type = $typed(self->type) }, &self->head, &self->len);
};

fn_((thrd_SPSC__remaining(time_Awake clock, time_Awake_Inst started, time_Dur timeout))(time_Dur)) {
    let elapsed = time_Awake_Inst_elapsed(started, clock);
    return orelse_((time_Dur_subChkd(timeout, elapsed))(time_Dur_zero));
};
