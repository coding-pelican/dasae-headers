#include "test-thrd/main.h"

TEST_fn_("thrd/OnceEvt: signal permanently releases event and token" $guard) {
    var evt = thrd_OnceEvt_init();
    defer_(thrd_OnceEvt_fini(&evt));
    let tok = thrd_OnceEvt_tok(&evt);
    let sig = thrd_OnceEvt_sig(&evt);

    try_(TEST_expect(!thrd_OnceEvt_tryWait(&evt)));
    try_(TEST_expect(!thrd_OnceEvt_Tok_tryWait(tok)));
    thrd_OnceEvt_Sig_set(sig);
    try_(TEST_expect(thrd_OnceEvt_isSet(&evt)));
    try_(TEST_expect(thrd_OnceEvt_tryWait(&evt)));
    try_(TEST_expect(thrd_OnceEvt_Tok_tryWait(tok)));
} $unguarded(TEST_fn);
