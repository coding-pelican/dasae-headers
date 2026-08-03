#include "test-thrd-channel/main.h"

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

TEST_fn_("thrd/SPMC: starts open and empty" $guard) {
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    try_(TEST_expect(thrd_SPMC_isOpen$u32(&spmc)));
    try_(TEST_expect(thrd_SPMC_isEmpty$u32(&spmc)));
    try_(TEST_expect(!thrd_SPMC_isFull$u32(&spmc)));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/SPMC: typed try send and receive" $guard) {
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
    try_(thrd_SPMC_Tx_trySend$u32(spmc_tx, 33));
    try_(TEST_expect(try_(thrd_SPMC_Rx_tryRecv$u32(spmc_rx)) == 33));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/SPMC: typed blocking send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
    try_(thrd_SPMC_Tx_send$u32(spmc_tx, 34, chan_cancel_src));
    try_(TEST_expect(try_(thrd_SPMC_Rx_recv$u32(spmc_rx, chan_cancel_src)) == 34));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/SPMC: typed timed send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
    try_(thrd_SPMC_Tx_sendFor$u32(spmc_tx, 35, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_SPMC_Rx_recvFor$u32(spmc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 35));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/SPMC: select receives typed value" $guard) {
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
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

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/SPMC: select sends typed value" $guard) {
    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_initFixed$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_finiFixed$u32(&spmc));
    let spmc_tx = thrd_SPMC_tx$u32(&spmc);
    let spmc_rx = thrd_SPMC_rx$u32(&spmc);
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
