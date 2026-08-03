#include "test-thrd/main.h"

TEST_fn_("thrd/OnceLock: sets once and returns typed value" $guard) {
    var lock = thrd_OnceLock_init_static$(u32);
    defer_(thrd_OnceLock_fini$u32(&lock));

    try_(TEST_expect(!thrd_OnceLock_isSet$u32(&lock)));
    try_(TEST_expect(thrd_OnceLock_trySet$u32(&lock, 42)));
    try_(TEST_expect(!thrd_OnceLock_trySet$u32(&lock, 99)));
    thrd_OnceLock_wait$u32(&lock);

    try_(TEST_expect(thrd_OnceLock_isSet$u32(&lock)));
    try_(TEST_expect(thrd_OnceLock_get$u32(&lock) == 42));
    return_ok({});
} $unguarded(TEST_fn);
