#include "test-thrd-channel/main.h"

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

TEST_fn_("thrd/MPSC: starts open and empty" $guard) {
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    try_(TEST_expect(thrd_MPSC_isOpen$u16(&mpsc)));
    try_(TEST_expect(thrd_MPSC_isEmpty$u16(&mpsc)));
    try_(TEST_expect(!thrd_MPSC_isFull$u16(&mpsc)));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPSC: typed try send and receive" $guard) {
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
    try_(thrd_MPSC_Tx_trySend$u16(mpsc_tx, 22));
    try_(TEST_expect(try_(thrd_MPSC_Rx_tryRecv$u16(mpsc_rx)) == 22));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPSC: typed blocking send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
    try_(thrd_MPSC_Tx_send$u16(mpsc_tx, 23, chan_cancel_src));
    try_(TEST_expect(try_(thrd_MPSC_Rx_recv$u16(mpsc_rx, chan_cancel_src)) == 23));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPSC: typed timed send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
    try_(thrd_MPSC_Tx_sendFor$u16(mpsc_tx, 24, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_MPSC_Rx_recvFor$u16(mpsc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 24));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPSC: select receives typed value" $guard) {
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
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

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPSC: select sends typed value" $guard) {
    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_initFixed$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_finiFixed$u16(&mpsc));
    let mpsc_tx = thrd_MPSC_tx$u16(&mpsc);
    let mpsc_rx = thrd_MPSC_rx$u16(&mpsc);
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
