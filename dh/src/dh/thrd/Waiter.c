#include "dh/thrd/Waiter.h"

fn_((thrd_Waiter_init(void))(thrd_Waiter)) {
    return (thrd_Waiter){
        .lock = thrd_Mtx_init(),
        .cond = thrd_Cond_init(),
        .woken = false,
    };
};
fn_((thrd_Waiter_fini(thrd_Waiter* self))(void)) {
    thrd_Cond_fini(&self->cond);
    thrd_Mtx_fini(&self->lock);
};

fn_((thrd_Waiter_link(thrd_Waiter* self, usize case_idx))(thrd_wait_Link)) {
    return thrd_wait_Link_from(
        thrd_Waker_ensureValid((thrd_Waker){
            .ctx = self,
            .wakeFn = thrd_Waiter_wake,
        }),
        case_idx
    );
};
fn_((thrd_Waiter_wait(
    thrd_Waiter* self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    while (!self->woken) {
        catch_((thrd_Cond_wait(&self->cond, &self->lock, cancel_src))(err, {
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
    }
    self->woken = false;
    thrd_Mtx_unlock(&self->lock);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Waiter_waitFor(
    thrd_Waiter* self, thrd_Wakeable cancel_src, time_Dur dur
))(Sched_TimedE$void) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    while (!self->woken) {
        catch_((thrd_Cond_waitFor(&self->cond, &self->lock, cancel_src, dur))(err, {
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
    }
    self->woken = false;
    thrd_Mtx_unlock(&self->lock);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Waiter_waitProtcd(thrd_Waiter* self))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    while (!self->woken) {
        thrd_Cond_waitProtcd(&self->cond, &self->lock);
    }
    self->woken = false;
    thrd_Mtx_unlock(&self->lock);
};
fn_((thrd_Waiter_wake(P$raw ctx))(void)) {
    let self = ptrAlignCast$((thrd_Waiter*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    self->woken = true;
    thrd_Cond_signal(&self->cond);
    thrd_Mtx_unlock(&self->lock);
};
