#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/heap/Sys.h"

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

typedef variant_((test_thrd_SelectU8 $fits($packed))(
    (test_thrd_SelectU8_recv, u8),
    (test_thrd_SelectU8_send, Void)
)) test_thrd_SelectU8;
T_use_thrd_Select_Arm$(test_thrd_SelectU8);
T_use$((test_thrd_SelectU8)(
    thrd_Select_Arm_result,
    thrd_Select_fixed,
    thrd_Select_watchWithin,
    thrd_Select_poll
));
T_use$((u8)(
    thrd_SPSC,
    thrd_SPSC_Tx,
    thrd_SPSC_Rx,
    thrd_SPSC_init,
    thrd_SPSC_fini,
    thrd_SPSC_isFull,
    thrd_SPSC_isEmpty,
    thrd_SPSC_isOpen,
    thrd_SPSC_tx,
    thrd_SPSC_rx,
    thrd_SPSC_Tx_trySend,
    thrd_SPSC_Tx_send,
    thrd_SPSC_Tx_sendFor,
    thrd_SPSC_Tx_sendOp,
    thrd_SPSC_Tx_SendOp_op,
    thrd_SPSC_Rx_tryRecv,
    thrd_SPSC_Rx_recv,
    thrd_SPSC_Rx_recvFor,
    thrd_SPSC_Rx_recvOp
));
TEST_fn_("thrd: channel - typed SPSC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));

    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    try_(TEST_expect(thrd_SPSC_isOpen$u8(&spsc)));
    try_(TEST_expect(thrd_SPSC_isEmpty$u8(&spsc)));
    try_(TEST_expect(!thrd_SPSC_isFull$u8(&spsc)));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
    try_(thrd_SPSC_Tx_trySend$u8(spsc_tx, 11));
    try_(TEST_expect(try_(thrd_SPSC_Rx_tryRecv$u8(spsc_rx)) == 11));
    try_(thrd_SPSC_Tx_send$u8(spsc_tx, 12, chan_cancel_src));
    try_(TEST_expect(try_(thrd_SPSC_Rx_recv$u8(spsc_rx, chan_cancel_src)) == 12));
    try_(thrd_SPSC_Tx_sendFor$u8(spsc_tx, 13, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_SPSC_Rx_recvFor$u8(spsc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 13));
    var spsc_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU8)));
    var spsc_select = thrd_Select_fixed$test_thrd_SelectU8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU8)(spsc_arms)));
    var spsc_recv_op = thrd_SPSC_Rx_recvOp$u8(spsc_rx);
    thrd_Select_watchWithin$test_thrd_SelectU8(
        &spsc_select, test_thrd_SelectU8_recv, thrd_SPSC_Rx_RecvOp_op(&spsc_recv_op)
    );
    try_(thrd_SPSC_Tx_trySend$u8(spsc_tx, 14));
    let spsc_done = thrd_Select_poll$test_thrd_SelectU8(&spsc_select);
    try_(TEST_expect(isSome(spsc_done)));
    var spsc_recv_case = unwrap_(spsc_done);
    let spsc_recv_result = *thrd_Select_Arm_result$test_thrd_SelectU8(&spsc_recv_case);
    try_(TEST_expect(matches(spsc_recv_result, test_thrd_SelectU8_recv)));
    try_(TEST_expect(union_to((spsc_recv_result)(test_thrd_SelectU8_recv)) == 14));
    var spsc_send_op = thrd_SPSC_Tx_sendOp$u8(spsc_tx, 15);
    var spsc_send_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU8)));
    var spsc_send_select = thrd_Select_fixed$test_thrd_SelectU8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU8)(spsc_send_arms)));
    thrd_Select_watchWithin$test_thrd_SelectU8(
        &spsc_send_select, test_thrd_SelectU8_send, thrd_SPSC_Tx_SendOp_op$u8(&spsc_send_op)
    );
    let spsc_send_done = thrd_Select_poll$test_thrd_SelectU8(&spsc_send_select);
    try_(TEST_expect(isSome(spsc_send_done)));
    var spsc_send_case = unwrap_(spsc_send_done);
    try_(TEST_expect(matches(*thrd_Select_Arm_result$test_thrd_SelectU8(&spsc_send_case), test_thrd_SelectU8_send)));
    try_(TEST_expect(try_(thrd_SPSC_Rx_tryRecv$u8(spsc_rx)) == 15));

    return_ok({});
} $unguarded(TEST_fn)

typedef variant_((test_thrd_SelectU16 $fits($packed))(
    (test_thrd_SelectU16_recv, u16),
    (test_thrd_SelectU16_send, Void)
)) test_thrd_SelectU16;
T_use_thrd_Select_Arm$(test_thrd_SelectU16);
T_use$((test_thrd_SelectU16)(
    thrd_Select_Arm_result,
    thrd_Select_fixed,
    thrd_Select_watchWithin,
    thrd_Select_poll
));
T_use$((u16)(
    thrd_MPSC,
    thrd_MPSC_Tx,
    thrd_MPSC_Rx,
    thrd_MPSC_initFixed,
    thrd_MPSC_finiFixed,
    thrd_MPSC_isFull,
    thrd_MPSC_isEmpty,
    thrd_MPSC_isOpen,
    thrd_MPSC_tx,
    thrd_MPSC_rx,
    thrd_MPSC_Tx_trySend,
    thrd_MPSC_Tx_send,
    thrd_MPSC_Tx_sendFor,
    thrd_MPSC_Tx_sendOp,
    thrd_MPSC_Tx_SendOp_op,
    thrd_MPSC_Rx_tryRecv,
    thrd_MPSC_Rx_recv,
    thrd_MPSC_Rx_recvFor,
    thrd_MPSC_Rx_recvOp
));
TEST_fn_("thrd: channel - typed MPSC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));

    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    try_(TEST_expect(thrd_MPSC_isOpen$u16(&mpsc)));
    try_(TEST_expect(thrd_MPSC_isEmpty$u16(&mpsc)));
    try_(TEST_expect(!thrd_MPSC_isFull$u16(&mpsc)));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
    try_(thrd_MPSC_Tx_trySend$u16(mpsc_tx, 22));
    try_(TEST_expect(try_(thrd_MPSC_Rx_tryRecv$u16(mpsc_rx)) == 22));
    try_(thrd_MPSC_Tx_send$u16(mpsc_tx, 23, chan_cancel_src));
    try_(TEST_expect(try_(thrd_MPSC_Rx_recv$u16(mpsc_rx, chan_cancel_src)) == 23));
    try_(thrd_MPSC_Tx_sendFor$u16(mpsc_tx, 24, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_MPSC_Rx_recvFor$u16(mpsc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 24));
    var mpsc_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU16)));
    var mpsc_select = thrd_Select_fixed$test_thrd_SelectU16(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU16)(mpsc_arms)));
    var mpsc_recv_op = thrd_MPSC_Rx_recvOp$u16(mpsc_rx);
    thrd_Select_watchWithin$test_thrd_SelectU16(
        &mpsc_select, test_thrd_SelectU16_recv, thrd_MPSC_Rx_RecvOp_op(&mpsc_recv_op)
    );
    try_(thrd_MPSC_Tx_trySend$u16(mpsc_tx, 25));
    let mpsc_done = thrd_Select_poll$test_thrd_SelectU16(&mpsc_select);
    try_(TEST_expect(isSome(mpsc_done)));
    var mpsc_recv_case = unwrap_(mpsc_done);
    let mpsc_recv_result = *thrd_Select_Arm_result$test_thrd_SelectU16(&mpsc_recv_case);
    try_(TEST_expect(matches(mpsc_recv_result, test_thrd_SelectU16_recv)));
    try_(TEST_expect(union_to((mpsc_recv_result)(test_thrd_SelectU16_recv)) == 25));
    var mpsc_send_op = thrd_MPSC_Tx_sendOp$u16(mpsc_tx, 26);
    var mpsc_send_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU16)));
    var mpsc_send_select = thrd_Select_fixed$test_thrd_SelectU16(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU16)(mpsc_send_arms)));
    thrd_Select_watchWithin$test_thrd_SelectU16(
        &mpsc_send_select, test_thrd_SelectU16_send, thrd_MPSC_Tx_SendOp_op$u16(&mpsc_send_op)
    );
    let mpsc_send_done = thrd_Select_poll$test_thrd_SelectU16(&mpsc_send_select);
    try_(TEST_expect(isSome(mpsc_send_done)));
    var mpsc_send_case = unwrap_(mpsc_send_done);
    try_(TEST_expect(matches(*thrd_Select_Arm_result$test_thrd_SelectU16(&mpsc_send_case), test_thrd_SelectU16_send)));
    try_(TEST_expect(try_(thrd_MPSC_Rx_tryRecv$u16(mpsc_rx)) == 26));

    return_ok({});
} $unguarded(TEST_fn)

typedef variant_((test_thrd_SelectU32 $fits($packed))(
    (test_thrd_SelectU32_recv, u32),
    (test_thrd_SelectU32_send, Void)
)) test_thrd_SelectU32;
T_use_thrd_Select_Arm$(test_thrd_SelectU32);
T_use$((test_thrd_SelectU32)(
    thrd_Select_Arm_result,
    thrd_Select_fixed,
    thrd_Select_watchWithin,
    thrd_Select_poll
));
T_use$((u32)(
    thrd_SPMC,
    thrd_SPMC_Tx,
    thrd_SPMC_Rx,
    thrd_SPMC_initFixed,
    thrd_SPMC_finiFixed,
    thrd_SPMC_isFull,
    thrd_SPMC_isEmpty,
    thrd_SPMC_isOpen,
    thrd_SPMC_tx,
    thrd_SPMC_rx,
    thrd_SPMC_Tx_trySend,
    thrd_SPMC_Tx_send,
    thrd_SPMC_Tx_sendFor,
    thrd_SPMC_Tx_sendOp,
    thrd_SPMC_Tx_SendOp_op,
    thrd_SPMC_Rx_tryRecv,
    thrd_SPMC_Rx_recv,
    thrd_SPMC_Rx_recvFor,
    thrd_SPMC_Rx_recvOp
));
TEST_fn_("thrd: channel - typed SPMC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));

    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    try_(TEST_expect(thrd_SPMC_isOpen$u32(&spmc)));
    try_(TEST_expect(thrd_SPMC_isEmpty$u32(&spmc)));
    try_(TEST_expect(!thrd_SPMC_isFull$u32(&spmc)));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
    try_(thrd_SPMC_Tx_trySend$u32(spmc_tx, 33));
    try_(TEST_expect(try_(thrd_SPMC_Rx_tryRecv$u32(spmc_rx)) == 33));
    try_(thrd_SPMC_Tx_send$u32(spmc_tx, 34, chan_cancel_src));
    try_(TEST_expect(try_(thrd_SPMC_Rx_recv$u32(spmc_rx, chan_cancel_src)) == 34));
    try_(thrd_SPMC_Tx_sendFor$u32(spmc_tx, 35, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_SPMC_Rx_recvFor$u32(spmc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 35));
    var spmc_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU32)));
    var spmc_select = thrd_Select_fixed$test_thrd_SelectU32(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU32)(spmc_arms)));
    var spmc_recv_op = thrd_SPMC_Rx_recvOp$u32(spmc_rx);
    thrd_Select_watchWithin$test_thrd_SelectU32(
        &spmc_select, test_thrd_SelectU32_recv, thrd_SPMC_Rx_RecvOp_op(&spmc_recv_op)
    );
    try_(thrd_SPMC_Tx_trySend$u32(spmc_tx, 36));
    let spmc_done = thrd_Select_poll$test_thrd_SelectU32(&spmc_select);
    try_(TEST_expect(isSome(spmc_done)));
    var spmc_recv_case = unwrap_(spmc_done);
    let spmc_recv_result = *thrd_Select_Arm_result$test_thrd_SelectU32(&spmc_recv_case);
    try_(TEST_expect(matches(spmc_recv_result, test_thrd_SelectU32_recv)));
    try_(TEST_expect(union_to((spmc_recv_result)(test_thrd_SelectU32_recv)) == 36));
    var spmc_send_op = thrd_SPMC_Tx_sendOp$u32(spmc_tx, 37);
    var spmc_send_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU32)));
    var spmc_send_select = thrd_Select_fixed$test_thrd_SelectU32(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU32)(spmc_send_arms)));
    thrd_Select_watchWithin$test_thrd_SelectU32(
        &spmc_send_select, test_thrd_SelectU32_send, thrd_SPMC_Tx_SendOp_op$u32(&spmc_send_op)
    );
    let spmc_send_done = thrd_Select_poll$test_thrd_SelectU32(&spmc_send_select);
    try_(TEST_expect(isSome(spmc_send_done)));
    var spmc_send_case = unwrap_(spmc_send_done);
    try_(TEST_expect(matches(*thrd_Select_Arm_result$test_thrd_SelectU32(&spmc_send_case), test_thrd_SelectU32_send)));
    try_(TEST_expect(try_(thrd_SPMC_Rx_tryRecv$u32(spmc_rx)) == 37));

    return_ok({});
} $unguarded(TEST_fn)

typedef variant_((test_thrd_SelectU64 $fits($packed))(
    (test_thrd_SelectU64_recv, u64),
    (test_thrd_SelectU64_send, Void)
)) test_thrd_SelectU64;
T_use_thrd_Select_Arm$(test_thrd_SelectU64);
T_use$((test_thrd_SelectU64)(
    thrd_Select_Arm_result,
    thrd_Select_fixed,
    thrd_Select_watchWithin,
    thrd_Select_poll
));
T_use$((u64)(
    thrd_MPMC,
    thrd_MPMC_Tx,
    thrd_MPMC_Rx,
    thrd_MPMC_initFixed,
    thrd_MPMC_finiFixed,
    thrd_MPMC_isFull,
    thrd_MPMC_isEmpty,
    thrd_MPMC_isOpen,
    thrd_MPMC_tx,
    thrd_MPMC_rx,
    thrd_MPMC_Tx_trySend,
    thrd_MPMC_Tx_send,
    thrd_MPMC_Tx_sendFor,
    thrd_MPMC_Tx_sendOp,
    thrd_MPMC_Tx_SendOp_op,
    thrd_MPMC_Rx_tryRecv,
    thrd_MPMC_Rx_recv,
    thrd_MPMC_Rx_recvFor,
    thrd_MPMC_Rx_recvOp
));
TEST_fn_("thrd: channel - typed MPMC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));

    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    try_(TEST_expect(thrd_MPMC_isOpen$u64(&mpmc)));
    try_(TEST_expect(thrd_MPMC_isEmpty$u64(&mpmc)));
    try_(TEST_expect(!thrd_MPMC_isFull$u64(&mpmc)));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
    try_(thrd_MPMC_Tx_trySend$u64(mpmc_tx, 44));
    try_(TEST_expect(try_(thrd_MPMC_Rx_tryRecv$u64(mpmc_rx)) == 44));
    try_(thrd_MPMC_Tx_send$u64(mpmc_tx, 45, chan_cancel_src));
    try_(TEST_expect(try_(thrd_MPMC_Rx_recv$u64(mpmc_rx, chan_cancel_src)) == 45));
    try_(thrd_MPMC_Tx_sendFor$u64(mpmc_tx, 46, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_MPMC_Rx_recvFor$u64(mpmc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 46));
    var mpmc_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU64)));
    var mpmc_select = thrd_Select_fixed$test_thrd_SelectU64(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU64)(mpmc_arms)));
    var mpmc_recv_op = thrd_MPMC_Rx_recvOp$u64(mpmc_rx);
    thrd_Select_watchWithin$test_thrd_SelectU64(
        &mpmc_select, test_thrd_SelectU64_recv, thrd_MPMC_Rx_RecvOp_op(&mpmc_recv_op)
    );
    try_(thrd_MPMC_Tx_trySend$u64(mpmc_tx, 47));
    let mpmc_done = thrd_Select_poll$test_thrd_SelectU64(&mpmc_select);
    try_(TEST_expect(isSome(mpmc_done)));
    var mpmc_recv_case = unwrap_(mpmc_done);
    let mpmc_recv_result = *thrd_Select_Arm_result$test_thrd_SelectU64(&mpmc_recv_case);
    try_(TEST_expect(matches(mpmc_recv_result, test_thrd_SelectU64_recv)));
    try_(TEST_expect(union_to((mpmc_recv_result)(test_thrd_SelectU64_recv)) == 47));
    var mpmc_send_op = thrd_MPMC_Tx_sendOp$u64(mpmc_tx, 48);
    var mpmc_send_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectU64)));
    var mpmc_send_select = thrd_Select_fixed$test_thrd_SelectU64(A_ref$((S$thrd_Select_Arm$test_thrd_SelectU64)(mpmc_send_arms)));
    thrd_Select_watchWithin$test_thrd_SelectU64(
        &mpmc_send_select, test_thrd_SelectU64_send, thrd_MPMC_Tx_SendOp_op$u64(&mpmc_send_op)
    );
    let mpmc_send_done = thrd_Select_poll$test_thrd_SelectU64(&mpmc_send_select);
    try_(TEST_expect(isSome(mpmc_send_done)));
    var mpmc_send_case = unwrap_(mpmc_send_done);
    try_(TEST_expect(matches(*thrd_Select_Arm_result$test_thrd_SelectU64(&mpmc_send_case), test_thrd_SelectU64_send)));
    try_(TEST_expect(try_(thrd_MPMC_Rx_tryRecv$u64(mpmc_rx)) == 48));

    return_ok({});
} $unguarded(TEST_fn)
