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

TEST_fn_("thrd/Group: spawn closure" $guard) {
    var sys_heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys_heap));

    var thrd_safe_heap = (heap_ThrdSafe){
        .child_alctr = heap_Sys_alctr(&sys_heap),
        .mtx = thrd_Mtx_init(),
    };
    defer_(thrd_Mtx_fini(&thrd_safe_heap.mtx));

    var wg = thrd_Group_init();
    defer_(thrd_Group_fini(&wg));

    var done_count = atom_V_init$(atom_V$usize, 0);
    var clsr = clsr_((test__markDone)(&done_count));

    try_(thrd_Group_spawn(&wg, heap_ThrdSafe_alctr(&thrd_safe_heap), clsr.as_base));
    thrd_Group_waitProtcd(&wg);

    try_(TEST_expect(atom_V_load(&done_count, atom_MemOrd_acquire) == 1));
    return_ok({});
} $unguarded(TEST_fn);
