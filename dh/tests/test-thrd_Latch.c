#include "test-thrd/main.h"

TEST_fn_("thrd/Latch: zero count is done for wait and token" $guard) {
    var cancel_src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel_src));
    let wait_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&cancel_src));

    var latch = thrd_Latch_init(0);
    defer_(thrd_Latch_fini(&latch));

    try_(TEST_expect(thrd_Latch_isDone(&latch)));
    try_(TEST_expect(thrd_Latch_tryWait(&latch)));
    try_(thrd_Latch_wait(&latch, wait_src));
    try_(TEST_expect(thrd_Latch_isDone(&latch)));
    try_(TEST_expect(thrd_Latch_value(&latch) == 0));

    let tok = thrd_Latch_tok(&latch);
    try_(TEST_expect(thrd_OnceEvt_Tok_tryWait(tok)));
    try_(thrd_OnceEvt_Tok_wait(tok, wait_src));
    return_ok({});
} $unguarded(TEST_fn);
