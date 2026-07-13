#include "dh/thrd/Sem.h"
#include "dh/time/self/Awake.h"

fn_((thrd_Sem_init(void))(thrd_Sem)) {
    return (thrd_Sem){
        .mtx = thrd_Mtx_init(),
        .cond = thrd_Cond_init(),
        .permits = 0,
    };
};
fn_((thrd_Sem_fini(thrd_Sem* self))(void)) {
    self->permits = 0;
    thrd_Cond_fini(&self->cond);
    thrd_Mtx_fini(&self->mtx);
};

fn_((thrd_Sem_tryWait(thrd_Sem* self))(bool)) {
    thrd_Mtx_lockProtcd(&self->mtx);
    if (self->permits == 0) {
        thrd_Mtx_unlock(&self->mtx);
        return false;
    }
    self->permits -= 1;
    thrd_Mtx_unlock(&self->mtx);
    return true;
};
fn_((thrd_Sem_wait(
    thrd_Sem* self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void) $guard) {
    thrd_Mtx_lockProtcd(&self->mtx);
    while (self->permits == 0) {
        catch_((thrd_Cond_wait(&self->cond, &self->mtx, cancel_src))(err, {
            thrd_Mtx_unlock(&self->mtx);
            return_err(err);
        }));
    }
    self->permits -= 1;
    if (self->permits != 0) {
        thrd_Cond_signal(&self->cond);
    }
    thrd_Mtx_unlock(&self->mtx);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Sem_waitFor(
    thrd_Sem* self, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lockProtcd(&self->mtx);
    while (self->permits == 0) {
        if (time_Dur_isZero(timeout)) {
            thrd_Mtx_unlock(&self->mtx);
            return_err(E_cause$Sched_Timeout());
        }
        let elapsed = time_Awake_Inst_elapsed(started, clock);
        if (cmp_ge$(time_Dur)(elapsed, timeout)) {
            thrd_Mtx_unlock(&self->mtx);
            return_err(E_cause$Sched_Timeout());
        }
        let remaining = time_Dur_sub(timeout, elapsed);

        catch_((thrd_Cond_waitFor(&self->cond, &self->mtx, cancel_src, remaining))(err, {
            thrd_Mtx_unlock(&self->mtx);
            return_err(err);
        }));
    }
    self->permits -= 1;
    if (self->permits != 0) {
        thrd_Cond_signal(&self->cond);
    }
    thrd_Mtx_unlock(&self->mtx);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Sem_waitProtcd(thrd_Sem* self))(void) $guard) {
    thrd_Mtx_lockProtcd(&self->mtx);
    while (self->permits == 0) {
        thrd_Cond_waitProtcd(&self->cond, &self->mtx);
    }
    self->permits -= 1;
    if (self->permits != 0) {
        thrd_Cond_signal(&self->cond);
    }
    thrd_Mtx_unlock(&self->mtx);
} $unguarded(fn);

fn_((thrd_Sem_post(thrd_Sem* self))(void) $guard) {
    thrd_Mtx_lockProtcd(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));

    self->permits++;
    thrd_Cond_signal(&self->cond);
} $unguarded(fn);
