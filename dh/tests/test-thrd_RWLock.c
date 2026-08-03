#include "test-thrd/main.h"

TEST_fn_("thrd/RWLock: exclusive and shared ownership can be reacquired" $guard) {
    var lock = thrd_RWLock_init();
    defer_(thrd_RWLock_fini(&lock));

    try_(TEST_expect(thrd_RWLock_tryLock(&lock)));
    thrd_RWLock_unlock(&lock);
    try_(TEST_expect(thrd_RWLock_tryLockShared(&lock)));
    try_(TEST_expect(thrd_RWLock_tryLockShared(&lock)));
    thrd_RWLock_unlockShared(&lock);
    thrd_RWLock_unlockShared(&lock);
    try_(TEST_expect(thrd_RWLock_tryLock(&lock)));
    thrd_RWLock_unlock(&lock);
} $unguarded(TEST_fn);
