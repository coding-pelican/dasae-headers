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

TEST_fn_("thrd/LazeLock: initializes once lazily" $guard) {
    var lock = thrd_LazeLock_init$(u32);
    defer_(thrd_LazeLock_fini$u32(&lock));

    u32 init_count = 0;
    var init = clsr_((test__nextLazyValue)(&init_count));
    let first = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);
    let second = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);

    try_(TEST_expect(first == second));
    try_(TEST_expect(*first == 101));
    try_(TEST_expect(init_count == 1));
    try_(TEST_expect(thrd_LazeLock_isSet$u32(&lock)));
    return_ok({});
} $unguarded(TEST_fn);
