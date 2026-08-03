#include "test-thrd/main.h"

TEST_fn_("thrd/CancelTok: source cancellation is observed by token and check" $guard) {
    var src = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&src));
    let tok = thrd_CancelTok_Src_tok(&src);

    try_(TEST_expect(!thrd_CancelTok_Src_isCanceled(&src)));
    try_(TEST_expect(!thrd_CancelTok_isCanceled(tok)));
    thrd_CancelTok_Src_cancel(&src);
    try_(TEST_expect(thrd_CancelTok_Src_isCanceled(&src)));
    try_(TEST_expect(thrd_CancelTok_isCanceled(tok)));
    try_(TEST_expect(thrd_CancelTok_tryWait(tok)));
    try_(TEST_expect(isErr(thrd_CancelTok_check(tok))));
} $unguarded(TEST_fn);
