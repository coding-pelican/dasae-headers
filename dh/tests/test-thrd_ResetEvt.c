#include "test-thrd/main.h"

TEST_fn_("thrd/ResetEvt: reset closes event after successful waits" $guard) {
    var evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&evt));
    let tok = thrd_ResetEvt_tok(&evt);
    let sig = thrd_ResetEvt_sig(&evt);

    try_(TEST_expect(!thrd_ResetEvt_tryWait(&evt)));
    thrd_ResetEvt_Sig_set(sig);
    try_(TEST_expect(thrd_ResetEvt_tryWait(&evt)));
    try_(TEST_expect(thrd_ResetEvt_Tok_tryWait(tok)));
    thrd_ResetEvt_Sig_reset(sig);
    try_(TEST_expect(!thrd_ResetEvt_isSet(&evt)));
    try_(TEST_expect(!thrd_ResetEvt_Tok_tryWait(tok)));
} $unguarded(TEST_fn);
