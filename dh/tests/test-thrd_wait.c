#include "test-thrd/main.h"

$static fn_((test_thrd_wait__wake(P$raw ctx))(void)) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    *P_at((calls)[0]) += 1;
};

TEST_fn_("thrd/wait: chain wakes and unlinks registered links" $scope) {
    var_(first_calls, usize) = 0;
    var_(second_calls, usize) = 0;
    let first = thrd_Waker_ensureValid((thrd_Waker){
        .ctx = &first_calls,
        .wakeFn = test_thrd_wait__wake,
    });
    let second = thrd_Waker_ensureValid((thrd_Waker){
        .ctx = &second_calls,
        .wakeFn = test_thrd_wait__wake,
    });
    var chain = thrd_wait_Chain_init();
    var first_link = thrd_wait_Link_from(first, 3);
    var second_link = thrd_wait_Link_from(second, 5);

    thrd_wait_Chain_prepend(&chain, &first_link);
    thrd_wait_Chain_prepend(&chain, &second_link);
    thrd_wait_Chain_wakeOne(&chain);
    try_(TEST_expect(first_calls + second_calls == 1));
    thrd_wait_Chain_unlink(&chain, &first_link);
    thrd_wait_Chain_wakeAll(&chain);
    try_(TEST_expect(first_calls + second_calls == 1));
} $unscoped(TEST_fn);
