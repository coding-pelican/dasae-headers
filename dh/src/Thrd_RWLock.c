#include "dh/Thrd/RWLock.h"

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
