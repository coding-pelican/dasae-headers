#include "test-thrd/main.h"

$static fn_((test_thrd_Waker__wake(P$raw ctx))(void)) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    *P_at((calls)[0]) += 1;
};

TEST_fn_("thrd/Waker: wake dispatches through owned context" $scope) {
    var_(calls, usize) = 0;
    let waker = thrd_Waker_ensureValid((thrd_Waker){
        .ctx = &calls,
        .wakeFn = test_thrd_Waker__wake,
    });

    thrd_Waker_wake(waker);
    thrd_Waker_wake(waker);
    try_(TEST_expect(calls == 2));
} $unscoped(TEST_fn);
