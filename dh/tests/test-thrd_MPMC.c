#include "test-thrd-channel/main.h"

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

TEST_fn_("thrd/MPMC: starts open and empty" $guard) {
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    try_(TEST_expect(thrd_MPMC_isOpen$u64(&mpmc)));
    try_(TEST_expect(thrd_MPMC_isEmpty$u64(&mpmc)));
    try_(TEST_expect(!thrd_MPMC_isFull$u64(&mpmc)));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPMC: typed try send and receive" $guard) {
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
    try_(thrd_MPMC_Tx_trySend$u64(mpmc_tx, 44));
    try_(TEST_expect(try_(thrd_MPMC_Rx_tryRecv$u64(mpmc_rx)) == 44));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPMC: typed blocking send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
    try_(thrd_MPMC_Tx_send$u64(mpmc_tx, 45, chan_cancel_src));
    try_(TEST_expect(try_(thrd_MPMC_Rx_recv$u64(mpmc_rx, chan_cancel_src)) == 45));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPMC: typed timed send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&chan_cancel));
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
    try_(thrd_MPMC_Tx_sendFor$u64(mpmc_tx, 46, chan_cancel_src, time_Dur_fromMillis(1)));
    try_(TEST_expect(try_(thrd_MPMC_Rx_recvFor$u64(mpmc_rx, chan_cancel_src, time_Dur_fromMillis(1))) == 46));

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPMC: select receives typed value" $guard) {
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
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

    return_ok({});
} $unguarded(TEST_fn)

TEST_fn_("thrd/MPMC: select sends typed value" $guard) {
    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_initFixed$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_finiFixed$u64(&mpmc));
    let mpmc_tx = thrd_MPMC_tx$u64(&mpmc);
    let mpmc_rx = thrd_MPMC_rx$u64(&mpmc);
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
