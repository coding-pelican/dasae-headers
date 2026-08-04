#include "test-thrd-channel/main.h"

T_use$((u8)(
    Clsr_Ctx,
    Clsr_Rtn,
    Clsr,
    thrd_Batch,
    thrd_Batch_Done_from,
    thrd_Batch_Done_result,
    thrd_Batch_Done_resultMut,
    thrd_Batch_Done_into,
    thrd_Batch_Done_take,
    thrd_Batch_init,
    thrd_Batch_fini,
    thrd_Batch_spawn,
    thrd_Batch_nextProtcd,
    thrd_Batch_wait
));

$static fn_((test_thrd_Batch__wait(thrd_ResetEvt* evt))(u8)) {
    thrd_ResetEvt_waitProtcd(evt);
    return 42;
};
fn_use_Clsr_((test_thrd_Batch__wait)(thrd_ResetEvt*)(u8));

$attr($suppressing_(frame_larger_than))
TEST_fn_("thrd/Batch: typed completion and fixed capacity" $guard) {
    var_(done, thrd_Batch_Done$u8) = thrd_Batch_Done_from$u8(0);
    *P_at((thrd_Batch_Done_resultMut$u8(&done))[0]) = 3;
    try_(TEST_expect(*P_at((thrd_Batch_Done_result$u8(&done))[0]) == 3));
    try_(TEST_expect(thrd_Batch_Done_into$u8(done) == 3));
    try_(TEST_expect(*P_at((thrd_Batch_Done_result$u8(&done))[0]) == 3));
    try_(TEST_expect(thrd_Batch_Done_take$u8(&done) == 3));
    try_(TEST_expect(*P_at((thrd_Batch_Done_result$u8(&done))[0]) == 0));

    var done_buf = A_zero$((A$$(2, thrd_Batch_Done$u8)));
    var batch = thrd_Batch_init$u8(A_ref$((S$thrd_Batch_Done$u8)(done_buf)));
    var cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel));
    try_(thrd_Batch_wait$u8(&batch, thrd_CancelTok_Src_tok(&cancel)));
    thrd_Batch_fini$u8(&batch);

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&evt));
    var cap_buf = A_zero$((A$$(1, thrd_Batch_Done$u8)));
    var cap = thrd_Batch_init$u8(A_ref$((S$thrd_Batch_Done$u8)(cap_buf)));
    defer_(thrd_Batch_fini$u8(&cap));
    var task = clsr_((test_thrd_Batch__wait)(&evt));
    try_(thrd_Batch_spawn$u8(&cap, gpa, task.as_base));
    var overflow_task = clsr_((test_thrd_Batch__wait)(&evt));
    try_(TEST_expect(isErr(thrd_Batch_spawn$u8(
        &cap, gpa, overflow_task.as_base
    ))));
    thrd_ResetEvt_set(&evt);
    let ready = unwrap_(thrd_Batch_nextProtcd$u8(&cap));
    try_(TEST_expect(*P_at((thrd_Batch_Done_result$u8(&ready))[0]) == 42));
    return_ok({});
} $unguarded(TEST_fn) $suppressed;
