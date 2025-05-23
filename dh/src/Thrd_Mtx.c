#include "dh/Thrd/Mtx.h"

fn_(Thrd_Mtx_init(void), Thrd_Mtx) {
    return (Thrd_Mtx){
        .impl = PTHREAD_MUTEX_INITIALIZER
    };
}

fn_(Thrd_Mtx_fini(Thrd_Mtx* self), void) {
    pthread_mutex_destroy(&self->impl);
}

fn_(Thrd_Mtx_lock(Thrd_Mtx* self), void) {
    pthread_mutex_lock(&self->impl);
}

fn_(Thrd_Mtx_tryLock(Thrd_Mtx* self), bool) {
    return pthread_mutex_trylock(&self->impl) == 0;
}

fn_(Thrd_Mtx_unlock(Thrd_Mtx* self), void) {
    pthread_mutex_unlock(&self->impl);
}
