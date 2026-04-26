#include "Sem.h"
#include "../time/self/Awake.h"

fn_((Thrd_Sem_init(void))(Thrd_Sem)) {
    return (Thrd_Sem){
        .mtx = Thrd_Mtx_init(),
        .cond = Thrd_Cond_init(),
        .permits = 0,
    };
};

fn_((Thrd_Sem_fini(Thrd_Sem* self))(void)) {
    self->permits = 0;
    Thrd_Cond_fini(&self->cond);
    Thrd_Mtx_fini(&self->mtx);
};

fn_((Thrd_Sem_wait(Thrd_Sem* self))(void) $guard) {
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));

    while (self->permits == 0) {
        Thrd_Cond_wait(&self->cond, &self->mtx);
    }

    if (0 < --self->permits) {
        Thrd_Cond_signal(&self->cond);
    }
} $unguarded(fn);

fn_((Thrd_Sem_timedWait(Thrd_Sem* self, time_Dur timeout))(Thrd_Sem_E$void) $guard) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);

    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));

    while (self->permits == 0) {
        let elapsed = time_Awake_Inst_elapsed(started, clock);
        if (time_Dur_gt(elapsed, timeout)) {
            return_err(Thrd_Sem_E_Timeout());
        }
        let remaining = time_Dur_sub(timeout, elapsed);
        try_(Thrd_Cond_timedWait(&self->cond, &self->mtx, remaining));
    }

    if (0 < --self->permits) {
        Thrd_Cond_signal(&self->cond);
    }
    return_ok({});
} $unguarded(fn);

fn_((Thrd_Sem_post(Thrd_Sem* self))(void) $guard) {
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));

    self->permits++;
    Thrd_Cond_signal(&self->cond);
} $unguarded(fn);
