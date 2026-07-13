#include "dh/thrd/RWLock.h"

/* --- Pthreads --- */

#if thrd_RWLock_use_pthread
fn_((thrd_RWLock_init(void))(thrd_RWLock)) {
    return (thrd_RWLock){
        .impl = PTHREAD_RWLOCK_INITIALIZER
    };
};
fn_((thrd_RWLock_fini(thrd_RWLock* self))(void)) {
    pthread_rwlock_destroy(&self->impl);
};

fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool)) {
    return pthread_rwlock_trywrlock(&self->impl) == (/*SUCCESS*/ 0);
};
fn_((thrd_RWLock_lock(thrd_RWLock* self, thrd_Wakeable cancel_src))(Sched_Cancelable$void)) {
    let_ignore = self;
    let_ignore = cancel_src;
    claim_unreachable;
};
fn_((thrd_RWLock_lockProtcd(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_wrlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};
fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool)) {
    return pthread_rwlock_tryrdlock(&self->impl) == (/*SUCCESS*/ 0);
};
fn_((thrd_RWLock_lockShared(thrd_RWLock* self, thrd_Wakeable cancel_src))(Sched_Cancelable$void)) {
    let_ignore = self;
    let_ignore = cancel_src;
    claim_unreachable;
};
fn_((thrd_RWLock_lockSharedProtcd(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_rdlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};
fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void)) {
    let rc = pthread_rwlock_unlock(&self->impl);
    claim_assert(rc == (/*SUCCESS*/ 0));
};

#else /* --- Common --- */

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
    return (thrd_RWLock){
        .impl = {
            .state = 0,
            .mtx = thrd_Mtx_init(),
            .cond = thrd_Cond_init(),
        }
    };
};
fn_((thrd_RWLock_fini(thrd_RWLock* self))(void)) {
    self->impl.state = 0;
    thrd_Cond_fini(&self->impl.cond);
    thrd_Mtx_fini(&self->impl.mtx);
};

fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool)) {
    if (thrd_Mtx_tryLock(&self->impl.mtx)) {
        if ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__reader_mask)) == 0) {
            self->impl.state |= thrd_RWLock__is_writing;
            thrd_Mtx_unlock(&self->impl.mtx);
            return true;
        }
        thrd_Mtx_unlock(&self->impl.mtx);
    }
    return false;
};
fn_((thrd_RWLock_lock(thrd_RWLock* self, thrd_Wakeable cancel_src))(Sched_Cancelable$void) $guard) {
    try_(thrd_Mtx_lock(&self->impl.mtx, cancel_src));
    self->impl.state += thrd_RWLock__writer;
    while ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__reader_mask)) != 0) {
        catch_((thrd_Cond_wait(&self->impl.cond, &self->impl.mtx, cancel_src))(err, {
            self->impl.state -= thrd_RWLock__writer;
            thrd_Cond_broadcast(&self->impl.cond);
            thrd_Mtx_unlock(&self->impl.mtx);
            return_err(err);
        }));
    }
    self->impl.state += thrd_RWLock__is_writing - thrd_RWLock__writer;
    thrd_Mtx_unlock(&self->impl.mtx);
    return_ok({});
} $unguarded(fn);
fn_((thrd_RWLock_lockProtcd(thrd_RWLock* self))(void)) {
    thrd_Mtx_lockProtcd(&self->impl.mtx);
    self->impl.state += thrd_RWLock__writer;
    while ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__reader_mask)) != 0) {
        thrd_Cond_waitProtcd(&self->impl.cond, &self->impl.mtx);
    }
    self->impl.state += thrd_RWLock__is_writing - thrd_RWLock__writer;
    thrd_Mtx_unlock(&self->impl.mtx);
};
fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void)) {
    thrd_Mtx_lockProtcd(&self->impl.mtx);
    self->impl.state &= ~thrd_RWLock__is_writing;
    thrd_Cond_broadcast(&self->impl.cond);
    thrd_Mtx_unlock(&self->impl.mtx);
};

fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool)) {
    if (thrd_Mtx_tryLock(&self->impl.mtx)) {
        if ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__writer_mask)) == 0) {
            self->impl.state += thrd_RWLock__reader;
            thrd_Mtx_unlock(&self->impl.mtx);
            return true;
        }
        thrd_Mtx_unlock(&self->impl.mtx);
    }
    return false;
};
fn_((thrd_RWLock_lockShared(thrd_RWLock* self, thrd_Wakeable cancel_src))(Sched_Cancelable$void) $guard) {
    try_(thrd_Mtx_lock(&self->impl.mtx, cancel_src));
    while ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__writer_mask)) != 0) {
        catch_((thrd_Cond_wait(&self->impl.cond, &self->impl.mtx, cancel_src))(err, {
            thrd_Mtx_unlock(&self->impl.mtx);
            return_err(err);
        }));
    }
    self->impl.state += thrd_RWLock__reader;
    thrd_Mtx_unlock(&self->impl.mtx);
    return_ok({});
} $unguarded(fn);
fn_((thrd_RWLock_lockSharedProtcd(thrd_RWLock* self))(void)) {
    thrd_Mtx_lockProtcd(&self->impl.mtx);
    while ((self->impl.state & (thrd_RWLock__is_writing | thrd_RWLock__writer_mask)) != 0) {
        thrd_Cond_waitProtcd(&self->impl.cond, &self->impl.mtx);
    }
    self->impl.state += thrd_RWLock__reader;
    thrd_Mtx_unlock(&self->impl.mtx);
};
fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void)) {
    thrd_Mtx_lockProtcd(&self->impl.mtx);
    self->impl.state -= thrd_RWLock__reader;
    if ((self->impl.state & thrd_RWLock__reader_mask) == 0) {
        thrd_Cond_broadcast(&self->impl.cond);
    }
    thrd_Mtx_unlock(&self->impl.mtx);
};
#endif
