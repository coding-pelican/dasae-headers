#include "dh/Thrd/Mtx.h"

fn_((Thrd_Mtx_init(void))(Thrd_Mtx)) {
    return (Thrd_Mtx){
        .impl = PTHREAD_MUTEX_INITIALIZER
    };
}

fn_((Thrd_Mtx_fini(Thrd_Mtx* self))(void)) {
    pthread_mutex_destroy(&self->impl);
}

fn_((Thrd_Mtx_lock(Thrd_Mtx* self))(void)) {
    pthread_mutex_lock(&self->impl);
}

fn_((Thrd_Mtx_tryLock(Thrd_Mtx* self))(bool)) {
    return pthread_mutex_trylock(&self->impl) == 0;
}

fn_((Thrd_Mtx_unlock(Thrd_Mtx* self))(void)) {
    pthread_mutex_unlock(&self->impl);
}

fn_((Thrd_MtxRecur_init(void))(Thrd_MtxRecur)) {
    return (Thrd_MtxRecur){
        .inner = Thrd_Mtx_init(),
        .thread_id = Thrd_invalid_id,
        .lock_count = 0,
    };
}

fn_((Thrd_MtxRecur_fini(Thrd_MtxRecur* self))(void)) {
    Thrd_Mtx_fini(&self->inner);
}

fn_((Thrd_MtxRecur_lock(Thrd_MtxRecur* self))(void)) {
    let current_id = Thrd_getCurrentId();
    if (atom_load(&self->thread_id, atom_MemOrd_unordered) != current_id) {
        Thrd_Mtx_lock(&self->inner);
        claim_assert(self->lock_count == 0);
        atom_store(&self->thread_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
}

fn_((Thrd_MtxRecur_tryLock(Thrd_MtxRecur* self))(bool)) {
    let current_id = Thrd_getCurrentId();
    if (atom_load(&self->thread_id, atom_MemOrd_unordered) != current_id) {
        if (!Thrd_Mtx_tryLock(&self->inner)) { return false; }
        claim_assert(self->lock_count == 0);
        atom_store(&self->thread_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
    return true;
}

fn_((Thrd_MtxRecur_unlock(Thrd_MtxRecur* self))(void)) {
    self->lock_count--;
    if (self->lock_count == 0) {
        atom_store(&self->thread_id, Thrd_invalid_id, atom_MemOrd_unordered);
        Thrd_Mtx_unlock(&self->inner);
    }
}
