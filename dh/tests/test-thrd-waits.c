#include "test-thrd/main.h"

TEST_fn_("thrd waits: try cancel timeout" $guard) {
    var cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel_src));
    let cancel = thrd_CancelTok_Src_tok(&cancel_src);
    thrd_CancelTok_Src_cancel(&cancel_src);

    var evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&evt));
    try_(TEST_expect(isErr(thrd_ResetEvt_wait(&evt, thrd_CancelTok_wakeable(cancel)))));
    try_(TEST_expect(isErr(thrd_ResetEvt_waitFor(&evt, thrd_CancelTok_wakeable(cancel), time_Dur_fromMillis(1)))));

    var sem = thrd_Sem_init();
    defer_(thrd_Sem_fini(&sem));
    try_(TEST_expect(!thrd_Sem_tryWait(&sem)));
    try_(TEST_expect(isErr(thrd_Sem_wait(&sem, thrd_CancelTok_wakeable(cancel)))));

    var open_cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&open_cancel_src));
    let open_cancel = thrd_CancelTok_Src_tok(&open_cancel_src);
    try_(TEST_expect(isErr(thrd_Sem_waitFor(&sem, thrd_CancelTok_wakeable(open_cancel), time_Dur_zero))));

    var rw = thrd_RWLock_init();
    defer_(thrd_RWLock_fini(&rw));
    try_(thrd_RWLock_lock(&rw, thrd_CancelTok_wakeable(open_cancel)));
    thrd_RWLock_unlock(&rw);
    try_(thrd_RWLock_lockShared(&rw, thrd_CancelTok_wakeable(open_cancel)));
    thrd_RWLock_unlockShared(&rw);

    return_ok({});
} $unguarded(TEST_fn);
