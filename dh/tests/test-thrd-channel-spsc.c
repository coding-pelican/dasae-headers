#include "test-thrd-channel/main.h"

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

TEST_fn_("thrd: SPSC - starts open and empty" $guard) {
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    try_(TEST_expect(thrd_SPSC_isOpen$u8(&spsc)));
    try_(TEST_expect(thrd_SPSC_isEmpty$u8(&spsc)));
    try_(TEST_expect(!thrd_SPSC_isFull$u8(&spsc)));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd: SPSC - typed try send and receive" $guard) {
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
    try_(thrd_SPSC_Tx_trySend$u8(spsc_tx, 11));
    try_(TEST_expect(try_(thrd_SPSC_Rx_tryRecv$u8(spsc_rx)) == 11));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd: SPSC - typed blocking send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
    try_(thrd_SPSC_Tx_send$u8(spsc_tx, 12, chan_cancel_src));
    try_(TEST_expect(try_(thrd_SPSC_Rx_recv$u8(spsc_rx, chan_cancel_src)) == 12));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd: SPSC - typed timed send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
    try_(thrd_SPSC_Tx_sendFor$u8(spsc_tx, 13, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_SPSC_Rx_recvFor$u8(spsc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 13));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd: SPSC - select receives typed value" $guard) {
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
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

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd: SPSC - select sends typed value" $guard) {
    var spsc_buf = A_zero$((A$$(2, u8)));
    var spsc = thrd_SPSC_init$u8(A_ref$((S$u8)(spsc_buf)));
    defer_(thrd_SPSC_fini$u8(&spsc));
    let spsc_tx = thrd_SPSC_tx$u8(&spsc);
    let spsc_rx = thrd_SPSC_rx$u8(&spsc);
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
