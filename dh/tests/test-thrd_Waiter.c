#include "test-thrd/main.h"

TEST_fn_("thrd/Waiter: linked waker releases one protected wait" $guard) {
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    let link = thrd_Waiter_link(&waiter, 0);

    thrd_Waker_wake(thrd_wait_Link_data(&link)->waker);
    thrd_Waiter_waitProtcd(&waiter);
} $unguarded(TEST_fn);
