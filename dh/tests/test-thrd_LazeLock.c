#include "test-thrd/main.h"

TEST_fn_("thrd/LazeLock: initializes once lazily" $guard) {
    var lock = thrd_LazeLock_init$(u32);
    defer_(thrd_LazeLock_fini$u32(&lock));

    u32 init_count = 0;
    var init = clsr_((test__nextLazyValue)(&init_count));
    let first = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);
    let second = thrd_LazeLock_getOrInit$u32(&lock, init.as_base);

    try_(TEST_expect(first == second));
    try_(TEST_expect(*P_at((first)[0]) == 101));
    try_(TEST_expect(init_count == 1));
    try_(TEST_expect(thrd_LazeLock_isSet$u32(&lock)));
    return_ok({});
} $unguarded(TEST_fn);
