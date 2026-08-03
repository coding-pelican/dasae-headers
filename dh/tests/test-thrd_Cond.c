#include "test-thrd/main.h"

TEST_fn_("thrd/Cond: timed wait reacquires mutex after timeout" $guard) {
    var cond = thrd_Cond_init();
    defer_(thrd_Cond_fini(&cond));
    var mtx = thrd_Mtx_init();
    defer_(thrd_Mtx_fini(&mtx));
    var cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel));

    thrd_Mtx_lockProtcd(&mtx);
    let result = thrd_Cond_waitFor(
        &cond,
        &mtx,
        thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&cancel)),
        time_Dur_zero
    );
    thrd_Mtx_unlock(&mtx);
    try_(TEST_expect(isErr(result)));
} $unguarded(TEST_fn);
