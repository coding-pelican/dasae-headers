#include "dh/Thrd/RWLock.h"

#if Thrd_RWLock__use_pthread
fn_((Thrd_RWLock_init(void))(Thrd_RWLock)) {
    return (Thrd_RWLock){
        .impl = PTHREAD_RWLOCK_INITIALIZER
    };
};

fn_((Thrd_RWLock_fini(Thrd_RWLock* self))(void)) {
    pthread_rwlock_destroy(&self->impl);
};

fn_((Thrd_RWLock_lock(Thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_wrlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((Thrd_RWLock_tryLock(Thrd_RWLock* self))(bool)) {
    return pthread_rwlock_trywrlock(&self->impl) == (/*SUCCESS*/ 0);
};

fn_((Thrd_RWLock_unlock(Thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((Thrd_RWLock_lockShared(Thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_rdlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((Thrd_RWLock_tryLockShared(Thrd_RWLock* self))(bool)) {
    return pthread_rwlock_tryrdlock(&self->impl) == (/*SUCCESS*/ 0);
};

fn_((Thrd_RWLock_unlockShared(Thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

#else
/// State bit layout for 64-bit usize:
/// | bit 63 (unused) | bits 62-32 (reader count, 31 bits) | bits 31-1 (writer count, 31 bits) | bit 0 (is_writing) |
///
/// For 32-bit usize:
/// | bits 31-17 (reader count, 15 bits) | bits 16-1 (writer count, 15 bits) | bit 0 (is_writing) |
#define Thrd_RWLock__count_bits ((int_bits$(usize) - 1) / 2)
#define Thrd_RWLock__count_max ((as$(usize)(1) << Thrd_RWLock__count_bits) - 1)

#define Thrd_RWLock__is_writing as$(usize)(1)
#define Thrd_RWLock__writer (as$(usize)(1) << 1)
#define Thrd_RWLock__reader (as$(usize)(1) << (1 + Thrd_RWLock__count_bits))
#define Thrd_RWLock__writer_mask (Thrd_RWLock__count_max << 1)
#define Thrd_RWLock__reader_mask (Thrd_RWLock__count_max << (1 + Thrd_RWLock__count_bits))

fn_((Thrd_RWLock_init(void))(Thrd_RWLock)) {
    return (Thrd_RWLock){
        .impl = {
            .state = 0,
            .mtx = Thrd_Mtx_init(),
            .sem = Thrd_Sem_init(),
        }
    };
};

fn_((Thrd_RWLock_fini(Thrd_RWLock* self))(void)) {
    self->impl.state = 0;
    Thrd_Sem_fini(&self->impl.sem);
    Thrd_Mtx_fini(&self->impl.mtx);
};

fn_((Thrd_RWLock_lock(Thrd_RWLock* self))(void)) {
    let_ignore = atom_fetchAdd(&self->impl.state, Thrd_RWLock__writer, atom_MemOrd_seq_cst);
    Thrd_Mtx_lock(&self->impl.mtx);
    // Add IS_WRITING and subtract WRITER atomically: IS_WRITING - WRITER
    // Note: This uses wrapping subtraction semantics
    let state = atom_fetchAdd(
        &self->impl.state,
        Thrd_RWLock__is_writing - Thrd_RWLock__writer,
        atom_MemOrd_seq_cst
    );
    if ((state & Thrd_RWLock__reader_mask) != 0) {
        Thrd_Sem_wait(&self->impl.sem);
    }
};

fn_((Thrd_RWLock_tryLock(Thrd_RWLock* self))(bool)) {
    if (Thrd_Mtx_tryLock(&self->impl.mtx)) {
        let state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
        if ((state & Thrd_RWLock__reader_mask) == 0) {
            let_ignore = atom_fetchOr(&self->impl.state, Thrd_RWLock__is_writing, atom_MemOrd_seq_cst);
            return true;
        }
        Thrd_Mtx_unlock(&self->impl.mtx);
    }
    return false;
};

fn_((Thrd_RWLock_unlock(Thrd_RWLock* self))(void)) {
    let_ignore = atom_fetchAnd(&self->impl.state, ~Thrd_RWLock__is_writing, atom_MemOrd_seq_cst);
    Thrd_Mtx_unlock(&self->impl.mtx);
};

fn_((Thrd_RWLock_lockShared(Thrd_RWLock* self))(void)) {
    var state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
    // Fast path: try to acquire read lock without mutex if no writers
    while ((state & (Thrd_RWLock__is_writing | Thrd_RWLock__writer_mask)) == 0) {
        let result = atom_cmpXchgWeak(
            &self->impl.state,
            state,
            state + Thrd_RWLock__reader,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        );
        if (isNone(result)) {
            return;
        }
        state = unwrap_(result);
    }
    // Slow path: acquire mutex to add reader (waits for writer to finish)
    Thrd_Mtx_lock(&self->impl.mtx);
    let_ignore = atom_fetchAdd(&self->impl.state, Thrd_RWLock__reader, atom_MemOrd_seq_cst);
    Thrd_Mtx_unlock(&self->impl.mtx);
};

fn_((Thrd_RWLock_tryLockShared(Thrd_RWLock* self))(bool)) {
    let state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
    // Fast path: no writers waiting or writing
    if ((state & (Thrd_RWLock__is_writing | Thrd_RWLock__writer_mask)) == 0) {
        let result = atom_cmpXchgStrong(
            &self->impl.state,
            state,
            state + Thrd_RWLock__reader,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        );
        if (isNone(result)) {
            return true;
        }
    }
    // Slow path: acquire mutex to add reader
    if (Thrd_Mtx_tryLock(&self->impl.mtx)) {
        let_ignore = atom_fetchAdd(&self->impl.state, Thrd_RWLock__reader, atom_MemOrd_seq_cst);
        Thrd_Mtx_unlock(&self->impl.mtx);
        return true;
    }
    return false;
};

fn_((Thrd_RWLock_unlockShared(Thrd_RWLock* self))(void)) {
    let state = atom_fetchSub(&self->impl.state, Thrd_RWLock__reader, atom_MemOrd_seq_cst);
    // If we were the last reader and a writer is waiting, signal the semaphore
    let was_last_reader = (state & Thrd_RWLock__reader_mask) == Thrd_RWLock__reader;
    let writer_is_waiting = (state & Thrd_RWLock__is_writing) != 0;
    if (was_last_reader && writer_is_waiting) {
        Thrd_Sem_post(&self->impl.sem);
    }
};
#endif
