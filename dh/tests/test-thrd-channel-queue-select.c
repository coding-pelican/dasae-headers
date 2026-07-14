#include "test-thrd-channel/main.h"

typedef variant_((test_thrd_SelectI8 $fits($packed))(
    (test_thrd_SelectI8_recv, i8),
    (test_thrd_SelectI8_send, Void)
)) test_thrd_SelectI8;
T_use_thrd_Select_Arm$(test_thrd_SelectI8);
T_use$((test_thrd_SelectI8)(
    thrd_Select_Arm_result,
    thrd_Select_Arm_into,
    thrd_Select_Arm_take,
    thrd_Select_fixed,
    thrd_Select_watchWithin,
    thrd_Select_poll,
    thrd_Select_pollMut,
    thrd_Select_wait,
    thrd_Select_waitMut,
    thrd_Select_waitMutFor,
    thrd_Select_waitMutProtcd
));

typedef variant_((test_thrd_SelectI8Pair $fits($packed))(
    (test_thrd_SelectI8Pair_left, i8),
    (test_thrd_SelectI8Pair_right, i8)
)) test_thrd_SelectI8Pair;
T_use_thrd_Select_Arm$(test_thrd_SelectI8Pair);
T_use$((test_thrd_SelectI8Pair)(
    thrd_Select_Arm_from,
    thrd_Select_Arm_result,
    thrd_Select_init,
    thrd_Select_fini,
    thrd_Select_fixed,
    thrd_Select_from,
    thrd_Select_watch,
    thrd_Select_watchWithin,
    thrd_Select_pollMut
));

T_use$((i8)(
    thrd_Que,
    thrd_Que_init,
    thrd_Que_fini,
    thrd_Que_isFull,
    thrd_Que_isEmpty,
    thrd_Que_isOpen,
    thrd_Que_trySend,
    thrd_Que_send,
    thrd_Que_sendFor,
    thrd_Que_tryRecv,
    thrd_Que_recv,
    thrd_Que_recvFor,
    thrd_Que_recvOp,
    thrd_Que_sendOp,
    thrd_Que_SendOp_op
));
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

$static fn_((test_thrd_channel__batchWait(thrd_ResetEvt* evt))(u8)) {
    thrd_ResetEvt_waitProtcd(evt);
    return 42;
};
fn_use_Clsr_((test_thrd_channel__batchWait)(thrd_ResetEvt*)(u8));

$attr($suppressing_(frame_larger_than))
TEST_fn_("thrd: channel - typed batch queue and select templates" $guard) {
    var_(batch_done, thrd_Batch_Done$u8) = thrd_Batch_Done_from$u8(0);
    *thrd_Batch_Done_resultMut$u8(&batch_done) = 3;
    try_(TEST_expect(*thrd_Batch_Done_result$u8(&batch_done) == 3));
    try_(TEST_expect(thrd_Batch_Done_into$u8(batch_done) == 3));
    try_(TEST_expect(thrd_Batch_Done_take$u8(&batch_done) == 3));
    try_(TEST_expect(*thrd_Batch_Done_result$u8(&batch_done) == 0));
    var batch_done_buf = A_zero$((A$$(2, thrd_Batch_Done$u8)));
    var batch = thrd_Batch_init$u8(A_ref$((S$thrd_Batch_Done$u8)(batch_done_buf)));
    var batch_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&batch_cancel));
    try_(thrd_Batch_wait$u8(&batch, thrd_CancelTok_Src_tok(&batch_cancel)));
    thrd_Batch_fini$u8(&batch);

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var batch_evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&batch_evt));
    var batch_cap_buf = A_zero$((A$$(1, thrd_Batch_Done$u8)));
    var batch_cap = thrd_Batch_init$u8(A_ref$((S$thrd_Batch_Done$u8)(batch_cap_buf)));
    defer_(thrd_Batch_fini$u8(&batch_cap));
    var batch_task = clsr_((test_thrd_channel__batchWait)(&batch_evt));
    try_(thrd_Batch_spawn$u8(&batch_cap, gpa, batch_task.as_base));
    var batch_overflow_task = clsr_((test_thrd_channel__batchWait)(&batch_evt));
    let batch_overflow = thrd_Batch_spawn$u8(&batch_cap, gpa, batch_overflow_task.as_base);
    try_(TEST_expect(isErr(batch_overflow)));
    thrd_ResetEvt_set(&batch_evt);
    let batch_done_ready = thrd_Batch_nextProtcd$u8(&batch_cap);
    try_(TEST_expect(isSome(batch_done_ready)));
    let batch_done_value = unwrap_(batch_done_ready);
    try_(TEST_expect(*thrd_Batch_Done_result$u8(&batch_done_value) == 42));

    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));

    var que_buf = A_zero$((A$$(2, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
    try_(TEST_expect(thrd_Que_isOpen$i8(&que)));
    try_(TEST_expect(thrd_Que_isEmpty$i8(&que)));
    try_(TEST_expect(!thrd_Que_isFull$i8(&que)));
    try_(thrd_Que_trySend$i8(&que, 7));
    try_(TEST_expect(try_(thrd_Que_tryRecv$i8(&que)) == 7));
    try_(thrd_Que_send$i8(&que, 8, chan_cancel_src));
    try_(TEST_expect(try_(thrd_Que_recv$i8(&que, chan_cancel_src)) == 8));
    try_(thrd_Que_sendFor$i8(&que, 9, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_Que_recvFor$i8(&que, chan_cancel_src, time_Dur_fromMillis(1))) == 9));
    var select_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(select_arms)));
    var select_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&select_cancel));
    thrd_CancelTok_Src_cancel(&select_cancel);
    let canceled_select = thrd_Select_wait$test_thrd_SelectI8(
        &select, thrd_CancelTok_Src_tok(&select_cancel)
    );
    try_(TEST_expect(isErr(canceled_select)));
    var que_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 9));
    let select_done = thrd_Select_poll$test_thrd_SelectI8(&select);
    try_(TEST_expect(isSome(select_done)));
    var que_recv_case = unwrap_(select_done);
    let que_recv_result = thrd_Select_Arm_into$test_thrd_SelectI8(que_recv_case);
    try_(TEST_expect(matches(que_recv_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_recv_result)(test_thrd_SelectI8_recv)) == 9));

    var left_buf = A_zero$((A$$(1, i8)));
    var left_que = thrd_Que_init$i8(A_ref$((S$i8)(left_buf)));
    defer_(thrd_Que_fini$i8(&left_que));
    var right_buf = A_zero$((A$$(1, i8)));
    var right_que = thrd_Que_init$i8(A_ref$((S$i8)(right_buf)));
    defer_(thrd_Que_fini$i8(&right_que));
    var pair_arms = A_zero$((A$$(2, thrd_Select_Arm$test_thrd_SelectI8Pair)));
    var pair_select = thrd_Select_fixed$test_thrd_SelectI8Pair(
        A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8Pair)(pair_arms))
    );
    var left_recv_op = thrd_Que_recvOp$i8(&left_que);
    thrd_Select_watchWithin$test_thrd_SelectI8Pair(
        &pair_select, test_thrd_SelectI8Pair_left, thrd_Que_RecvOp_op(&left_recv_op)
    );
    var right_recv_op = thrd_Que_recvOp$i8(&right_que);
    thrd_Select_watchWithin$test_thrd_SelectI8Pair(
        &pair_select, test_thrd_SelectI8Pair_right, thrd_Que_RecvOp_op(&right_recv_op)
    );
    try_(thrd_Que_trySend$i8(&right_que, 31));
    let pair_done = thrd_Select_pollMut$test_thrd_SelectI8Pair(&pair_select);
    try_(TEST_expect(isSome(pair_done)));
    let pair_case = unwrap_(pair_done);
    let pair_result = *thrd_Select_Arm_result$test_thrd_SelectI8Pair(pair_case);
    try_(TEST_expect(matches(pair_result, test_thrd_SelectI8Pair_right)));
    try_(TEST_expect(union_to((pair_result)(test_thrd_SelectI8Pair_right)) == 31));

    var grow_select = try_(thrd_Select_init$test_thrd_SelectI8Pair(gpa, 1));
    defer_(thrd_Select_fini$test_thrd_SelectI8Pair(&grow_select, gpa));
    var grow_left_recv_op = thrd_Que_recvOp$i8(&left_que);
    try_(thrd_Select_watch$test_thrd_SelectI8Pair(
        &grow_select, gpa, test_thrd_SelectI8Pair_left, thrd_Que_RecvOp_op(&grow_left_recv_op)
    ));
    var grow_right_recv_op = thrd_Que_recvOp$i8(&right_que);
    try_(thrd_Select_watch$test_thrd_SelectI8Pair(
        &grow_select, gpa, test_thrd_SelectI8Pair_right, thrd_Que_RecvOp_op(&grow_right_recv_op)
    ));
    try_(thrd_Que_trySend$i8(&left_que, 32));
    let grow_done = thrd_Select_pollMut$test_thrd_SelectI8Pair(&grow_select);
    try_(TEST_expect(isSome(grow_done)));
    let grow_case = unwrap_(grow_done);
    let grow_result = *thrd_Select_Arm_result$test_thrd_SelectI8Pair(grow_case);
    try_(TEST_expect(matches(grow_result, test_thrd_SelectI8Pair_left)));
    try_(TEST_expect(union_to((grow_result)(test_thrd_SelectI8Pair_left)) == 32));

    var from_left_recv_op = thrd_Que_recvOp$i8(&left_que);
    var from_right_recv_op = thrd_Que_recvOp$i8(&right_que);
    var from_select = thrd_Select_from$test_thrd_SelectI8Pair(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8Pair)(A_from$((thrd_Select_Arm$test_thrd_SelectI8Pair){
        thrd_Select_Arm_from$test_thrd_SelectI8Pair(
            test_thrd_SelectI8Pair_left, thrd_Que_RecvOp_op(&from_left_recv_op)
        ),
        thrd_Select_Arm_from$test_thrd_SelectI8Pair(
            test_thrd_SelectI8Pair_right, thrd_Que_RecvOp_op(&from_right_recv_op)
        ),
    }))));
    try_(thrd_Que_trySend$i8(&right_que, 33));
    let from_done = thrd_Select_pollMut$test_thrd_SelectI8Pair(&from_select);
    try_(TEST_expect(isSome(from_done)));
    let from_case = unwrap_(from_done);
    let from_result = *thrd_Select_Arm_result$test_thrd_SelectI8Pair(from_case);
    try_(TEST_expect(matches(from_result, test_thrd_SelectI8Pair_right)));
    try_(TEST_expect(union_to((from_result)(test_thrd_SelectI8Pair_right)) == 33));

    var que_send_op = thrd_Que_sendOp$i8(&que, 10);
    var que_send_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_send_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_send_arms)));
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_send_select, test_thrd_SelectI8_send, thrd_Que_SendOp_op$i8(&que_send_op)
    );
    let que_send_done = thrd_Select_poll$test_thrd_SelectI8(&que_send_select);
    try_(TEST_expect(isSome(que_send_done)));
    var que_send_case = unwrap_(que_send_done);
    let que_send_result = thrd_Select_Arm_take$test_thrd_SelectI8(&que_send_case);
    try_(TEST_expect(matches(que_send_result, test_thrd_SelectI8_send)));
    try_(TEST_expect(!matches(*thrd_Select_Arm_result$test_thrd_SelectI8(&que_send_case), test_thrd_SelectI8_send)));
    try_(TEST_expect(try_(thrd_Que_tryRecv$i8(&que)) == 10));

    var que_poll_mut_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_poll_mut_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_poll_mut_arms)));
    var que_poll_mut_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_poll_mut_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_poll_mut_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 11));
    let que_poll_mut_done = thrd_Select_pollMut$test_thrd_SelectI8(&que_poll_mut_select);
    try_(TEST_expect(isSome(que_poll_mut_done)));
    let que_poll_mut_case = unwrap_(que_poll_mut_done);
    let que_poll_mut_result = *thrd_Select_Arm_result$test_thrd_SelectI8(que_poll_mut_case);
    try_(TEST_expect(matches(que_poll_mut_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_poll_mut_result)(test_thrd_SelectI8_recv)) == 11));

    var que_wait_mut_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_wait_mut_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_wait_mut_arms)));
    var que_wait_mut_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_wait_mut_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_wait_mut_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 12));
    let que_wait_mut_case = try_(thrd_Select_waitMut$test_thrd_SelectI8(&que_wait_mut_select, thrd_CancelTok_Src_tok(&chan_cancel)));
    let que_wait_mut_result = *thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_case);
    try_(TEST_expect(matches(que_wait_mut_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_result)(test_thrd_SelectI8_recv)) == 12));

    var que_wait_mut_for_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_wait_mut_for_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_wait_mut_for_arms)));
    var que_wait_mut_for_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_wait_mut_for_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_wait_mut_for_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 13));
    let que_wait_mut_for_case = try_(thrd_Select_waitMutFor$test_thrd_SelectI8(
        &que_wait_mut_for_select,
        thrd_CancelTok_Src_tok(&chan_cancel),
        time_Dur_fromMillis(1)
    ));
    let que_wait_mut_for_result = *thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_for_case);
    try_(TEST_expect(matches(que_wait_mut_for_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_for_result)(test_thrd_SelectI8_recv)) == 13));

    var que_wait_mut_protcd_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_wait_mut_protcd_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_wait_mut_protcd_arms)));
    var que_wait_mut_protcd_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_wait_mut_protcd_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_wait_mut_protcd_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 14));
    let que_wait_mut_protcd_case = thrd_Select_waitMutProtcd$test_thrd_SelectI8(&que_wait_mut_protcd_select);
    let que_wait_mut_protcd_result = *thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_protcd_case);
    try_(TEST_expect(matches(que_wait_mut_protcd_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_protcd_result)(test_thrd_SelectI8_recv)) == 14));

    return_ok({});
} $unguarded(TEST_fn) $suppressed;
