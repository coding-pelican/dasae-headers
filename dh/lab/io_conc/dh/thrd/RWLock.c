#include "RWLock.h"

/*--- Pthreads ---*/

#if thrd_RWLock_use_pthread
fn_((thrd_RWLock_init(void))(thrd_RWLock)) {
    return (thrd_RWLock)thrd_RWLock_init_static();
};

fn_((thrd_RWLock_fini(thrd_RWLock* self))(void)) {
    pthread_rwlock_destroy(&self->impl);
};

fn_((thrd_RWLock_lock(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_wrlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool)) {
    return pthread_rwlock_trywrlock(&self->impl) == (/*SUCCESS*/ 0);
};

fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((thrd_RWLock_lockShared(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_rdlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool)) {
    return pthread_rwlock_tryrdlock(&self->impl) == (/*SUCCESS*/ 0);
};

fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

#else /*--- Common ---*/

/// State bit layout for 64-bit usize:
/// | bit 63 (unused) | bits 62-32 (reader count, 31 bits) | bits 31-1 (writer count, 31 bits) | bit 0 (is_writing) |
///
/// For 32-bit usize:
/// | bits 31-17 (reader count, 15 bits) | bits 16-1 (writer count, 15 bits) | bit 0 (is_writing) |
#define thrd_RWLock__count_bits ((int_bits$(usize) - 1) / 2)
#define thrd_RWLock__count_max ((as$(usize)(1) << thrd_RWLock__count_bits) - 1)

#define thrd_RWLock__is_writing as$(usize)(1)
#define thrd_RWLock__writer (as$(usize)(1) << 1)
#define thrd_RWLock__reader (as$(usize)(1) << (1 + thrd_RWLock__count_bits))
#define thrd_RWLock__writer_mask (thrd_RWLock__count_max << 1)
#define thrd_RWLock__reader_mask (thrd_RWLock__count_max << (1 + thrd_RWLock__count_bits))

fn_((thrd_RWLock_init(void))(thrd_RWLock)) {
#if !thrd_Cond_use_pthread
    return (thrd_RWLock)thrd_RWLock_init_static();
#else
    return (thrd_RWLock){
        .impl = {
            .state = 0,
            .mtx = thrd_Mtx_init(),
            .sem = thrd_Sem_init(),
        }
    };
#endif
};

fn_((thrd_RWLock_fini(thrd_RWLock* self))(void)) {
    self->impl.state = 0;
    thrd_Sem_fini(&self->impl.sem);
    thrd_Mtx_fini(&self->impl.mtx);
};

fn_((thrd_RWLock_lock(thrd_RWLock* self))(void)) {
    let_ignore = atom_pri_fetchAdd(&self->impl.state, thrd_RWLock__writer, atom_MemOrd_seq_cst);
    thrd_Mtx_lock(&self->impl.mtx);
    // Add IS_WRITING and subtract WRITER atomically: IS_WRITING - WRITER
    // Note: This uses wrapping subtraction semantics
    let state = atom_pri_fetchAdd(
        &self->impl.state,
        thrd_RWLock__is_writing - thrd_RWLock__writer,
        atom_MemOrd_seq_cst
    );
    if ((state & thrd_RWLock__reader_mask) != 0) {
        thrd_Sem_wait(&self->impl.sem);
    }
};

fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool)) {
    if (thrd_Mtx_tryLock(&self->impl.mtx)) {
        let state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
        if ((state & thrd_RWLock__reader_mask) == 0) {
            let_ignore = atom_int_fetchOr(&self->impl.state, thrd_RWLock__is_writing, atom_MemOrd_seq_cst);
            return true;
        }
        thrd_Mtx_unlock(&self->impl.mtx);
    }
    return false;
};

fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void)) {
    let_ignore = atom_int_fetchAnd(&self->impl.state, ~thrd_RWLock__is_writing, atom_MemOrd_seq_cst);
    thrd_Mtx_unlock(&self->impl.mtx);
};

fn_((thrd_RWLock_lockShared(thrd_RWLock* self))(void)) {
    var state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
    // Fast path: try to acquire read lock without mutex if no writers
    while ((state & (thrd_RWLock__is_writing | thrd_RWLock__writer_mask)) == 0) {
        let result = atom_cmpXchgWeak(
            &self->impl.state,
            state,
            state + thrd_RWLock__reader,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        );
        if (isNone(result)) {
            return;
        }
        state = unwrap_(result);
    }
    // Slow path: acquire mutex to add reader (waits for writer to finish)
    thrd_Mtx_lock(&self->impl.mtx);
    let_ignore = atom_pri_fetchAdd(&self->impl.state, thrd_RWLock__reader, atom_MemOrd_seq_cst);
    thrd_Mtx_unlock(&self->impl.mtx);
};

fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool)) {
    let state = atom_load(&self->impl.state, atom_MemOrd_seq_cst);
    // Fast path: no writers waiting or writing
    if ((state & (thrd_RWLock__is_writing | thrd_RWLock__writer_mask)) == 0) {
        let result = atom_cmpXchgStrong(
            &self->impl.state,
            state,
            state + thrd_RWLock__reader,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        );
        if (isNone(result)) {
            return true;
        }
    }
    // Slow path: acquire mutex to add reader
    if (thrd_Mtx_tryLock(&self->impl.mtx)) {
        let_ignore = atom_pri_fetchAdd(&self->impl.state, thrd_RWLock__reader, atom_MemOrd_seq_cst);
        thrd_Mtx_unlock(&self->impl.mtx);
        return true;
    }
    return false;
};

fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void)) {
    let state = atom_pri_fetchSub(&self->impl.state, thrd_RWLock__reader, atom_MemOrd_seq_cst);
    // If we were the last reader and a writer is waiting, signal the semaphore
    let was_last_reader = (state & thrd_RWLock__reader_mask) == thrd_RWLock__reader;
    let writer_is_waiting = (state & thrd_RWLock__is_writing) != 0;
    if (was_last_reader && writer_is_waiting) {
        thrd_Sem_post(&self->impl.sem);
    }
};
#endif
