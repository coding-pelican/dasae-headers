#include "dh/Thrd/Cond.h"
#include "dh/Thrd/Mtx.h"
#include "dh/time/Duration.h"

fn_((Thrd_Cond_init(void))(Thrd_Cond)) {
    return (Thrd_Cond){
        .impl = PTHREAD_COND_INITIALIZER
    };
};

fn_((Thrd_Cond_fini(Thrd_Cond* self))(void)) {
    pthread_cond_destroy(&self->impl);
};

fn_((Thrd_Cond_wait(Thrd_Cond* self, Thrd_Mtx* mtx))(void)) {
    pthread_cond_wait(&self->impl, &mtx->impl);
};

fn_((Thrd_Cond_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration))(Thrd_Cond_Err$void) $scope) {
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

fn_((Thrd_Cond_signal(Thrd_Cond* self))(void)) {
    pthread_cond_signal(&self->impl);
};

fn_((Thrd_Cond_broadcast(Thrd_Cond* self))(void)) {
    pthread_cond_broadcast(&self->impl);
};
