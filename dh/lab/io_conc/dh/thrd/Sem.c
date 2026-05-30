#include "Sem.h"
#include "../time/self/Awake.h"

fn_((thrd_Sem_init(void))(thrd_Sem)) {
    return thrd_Sem_init_static();
};

fn_((thrd_Sem_fini(thrd_Sem* self))(void)) {
    self->permits = 0;
    thrd_Cond_fini(&self->cond);
    thrd_Mtx_fini(&self->mtx);
};

fn_((thrd_Sem_wait(thrd_Sem* self))(void) $guard) {
    thrd_Mtx_lock(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));

    while (self->permits == 0) {
        thrd_Cond_wait(&self->cond, &self->mtx);
    }

    if (0 < --self->permits) {
        thrd_Cond_signal(&self->cond);
    }
} $unguarded(fn);

fn_((thrd_Sem_timedWait(thrd_Sem* self, time_Dur timeout))(Sched_TimeoutE$void) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    thrd_Mtx_lock(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));

    while (self->permits == 0) {
        let elapsed = time_Awake_Inst_elapsed(started, clock);
        if (time_Dur_gt(elapsed, timeout)) {
            return_err(E_cause$Sched_Timeout());
        }
        let remaining = time_Dur_sub(timeout, elapsed);
        try_(thrd_Cond_timedWait(&self->cond, &self->mtx, remaining));
    }

    if (0 < --self->permits) {
        thrd_Cond_signal(&self->cond);
    }
    return_ok({});
} $unguarded(fn);

fn_((thrd_Sem_post(thrd_Sem* self))(void) $guard) {
    thrd_Mtx_lock(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));

    self->permits++;
    thrd_Cond_signal(&self->cond);
} $unguarded(fn);
