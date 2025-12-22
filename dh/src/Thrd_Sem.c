#include "dh/Thrd/Sem.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

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
} $unguarded_(fn);

fn_((Thrd_Sem_timedWait(Thrd_Sem* self, time_Duration timeout))(Thrd_Sem_Err$void) $guard) {
    let instant = time_Instant_now();

    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));

    while (self->permits == 0) {
        let elapsed = time_Instant_elapsed(instant);
        if (time_Duration_gt(elapsed, timeout)) {
            return_err(Thrd_Sem_Err_Timeout());
        }
        let remaining = time_Duration_sub(timeout, elapsed);
        try_(Thrd_Cond_timedWait(&self->cond, &self->mtx, remaining));
    }

    if (0 < --self->permits) {
        Thrd_Cond_signal(&self->cond);
    }
    return_ok({});
} $unguarded_(fn);

fn_((Thrd_Sem_post(Thrd_Sem* self))(void) $guard) {
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));

    self->permits++;
    Thrd_Cond_signal(&self->cond);
} $unguarded_(fn);
