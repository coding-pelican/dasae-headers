#include "dh-main.h"
#include "dh/thrd.h"

T_use$((Void)(
    thrd_Select,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll
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
    thrd_Select,
    thrd_Select_Done_init,
    thrd_Select_Done_result,
    thrd_Select_Done_resultMut,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll,
    thrd_Select_wait
));
T_use$((u8)(
    thrd_Batch,
    thrd_Batch_Done_init,
    thrd_Batch_Done_result,
    thrd_Batch_Done_resultMut,
    thrd_Batch_init,
    thrd_Batch_fini,
    thrd_Batch_wait
));
TEST_fn_("thrd: channel - typed batch queue and select templates" $guard) {
    var_(batch_anon, thrd_Batch$$(u8)) = cleared();
    var_(batch_done_anon, thrd_Batch_Done$$(u8)) = cleared();
    var_(batch_done, thrd_Batch_Done$u8) = thrd_Batch_Done_init$u8(0);
    (void)&batch_anon;
    (void)&batch_done_anon;
    *thrd_Batch_Done_resultMut$u8(&batch_done) = 3;
    try_(TEST_expect(*thrd_Batch_Done_result$u8(&batch_done) == 3));
    var batch_done_buf = A_zero$((A$$(2, thrd_Batch_Done$u8)));
    var batch = thrd_Batch_init$u8(A_ref$((S$thrd_Batch_Done$u8)(batch_done_buf)));
    var batch_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&batch_cancel));
    try_(thrd_Batch_wait$u8(&batch, thrd_CancelTok_Src_tok(&batch_cancel)));
    thrd_Batch_fini$u8(&batch);

    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_waitSrc(thrd_CancelTok_Src_tok(&chan_cancel));

    var_(select_anon, thrd_Select$$(i8)) = cleared();
    var_(select_done_anon, thrd_Select_Done$$(i8)) = cleared();
    var_(select_done_value, thrd_Select_Done$i8) = thrd_Select_Done_init$i8(0);
    (void)&select_anon;
    (void)&select_done_anon;
    *thrd_Select_Done_resultMut$i8(&select_done_value) = -3;
    try_(TEST_expect(*thrd_Select_Done_result$i8(&select_done_value) == -3));

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
    var select_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var select_result = i8_(0);
    var select = thrd_Select_init$i8(A_ref$((S$thrd_Select_Case)(select_cases)));
    var select_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&select_cancel));
    thrd_CancelTok_Src_cancel(&select_cancel);
    let canceled_select = thrd_Select_wait$i8(&select, thrd_CancelTok_Src_tok(&select_cancel));
    try_(TEST_expect(isErr(canceled_select)));
    var que_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watch$i8(&select, 0, thrd_Que_RecvOp_op(&que_recv_op), &select_result);
    try_(thrd_Que_trySend$i8(&que, 9));
    let select_done = thrd_Select_poll$i8(&select);
    try_(TEST_expect(isSome(select_done)));
    try_(TEST_expect(unwrap_(select_done).field_idx == 0));
    try_(TEST_expect(unwrap_(select_done).result == 9));
    try_(TEST_expect(select_result == 9));
    var que_send_item = i8_(10);
    var que_send_op = thrd_Que_sendOp$i8(&que, &que_send_item);
    var_(que_send_result, Void) = Void_();
    var que_send_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var que_send_select = thrd_Select_init$Void(A_ref$((S$thrd_Select_Case)(que_send_cases)));
    thrd_Select_watch$Void(&que_send_select, 0, thrd_Que_SendOp_op(&que_send_op), &que_send_result);
    let que_send_done = thrd_Select_poll$Void(&que_send_select);
    try_(TEST_expect(isSome(que_send_done)));
    try_(TEST_expect(unwrap_(que_send_done).field_idx == 0));
    try_(TEST_expect(try_(thrd_Que_tryRecv$i8(&que)) == 10));

    return_ok({});
} $unguarded(TEST_fn)

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
    thrd_SPSC_Rx_tryRecv,
    thrd_SPSC_Rx_recv,
    thrd_SPSC_Rx_recvFor,
    thrd_SPSC_Rx_recvOp,
    thrd_Select,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll
));
TEST_fn_("thrd: channel - typed SPSC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_waitSrc(thrd_CancelTok_Src_tok(&chan_cancel));

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
    var spsc_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var spsc_result = u8_(0);
    var spsc_select = thrd_Select_init$u8(A_ref$((S$thrd_Select_Case)(spsc_cases)));
    var spsc_recv_op = thrd_SPSC_Rx_recvOp$u8(spsc_rx);
    thrd_Select_watch$u8(&spsc_select, 0, thrd_SPSC_Rx_RecvOp_op(&spsc_recv_op), &spsc_result);
    try_(thrd_SPSC_Tx_trySend$u8(spsc_tx, 14));
    let spsc_done = thrd_Select_poll$u8(&spsc_select);
    try_(TEST_expect(isSome(spsc_done)));
    try_(TEST_expect(unwrap_(spsc_done).field_idx == 0));
    try_(TEST_expect(unwrap_(spsc_done).result == 14));
    try_(TEST_expect(spsc_result == 14));
    var spsc_send_item = u8_(15);
    var spsc_send_op = thrd_SPSC_Tx_sendOp$u8(spsc_tx, &spsc_send_item);
    var_(spsc_send_result, Void) = Void_();
    var spsc_send_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var spsc_send_select = thrd_Select_init$Void(A_ref$((S$thrd_Select_Case)(spsc_send_cases)));
    thrd_Select_watch$Void(&spsc_send_select, 0, thrd_SPSC_Tx_SendOp_op(&spsc_send_op), &spsc_send_result);
    let spsc_send_done = thrd_Select_poll$Void(&spsc_send_select);
    try_(TEST_expect(isSome(spsc_send_done)));
    try_(TEST_expect(unwrap_(spsc_send_done).field_idx == 0));
    try_(TEST_expect(try_(thrd_SPSC_Rx_tryRecv$u8(spsc_rx)) == 15));

    return_ok({});
} $unguarded(TEST_fn)

T_use$((u16)(
    thrd_MPSC,
    thrd_MPSC_Tx,
    thrd_MPSC_Rx,
    thrd_MPSC_init,
    thrd_MPSC_fini,
    thrd_MPSC_isFull,
    thrd_MPSC_isEmpty,
    thrd_MPSC_isOpen,
    thrd_MPSC_tx,
    thrd_MPSC_rx,
    thrd_MPSC_Tx_trySend,
    thrd_MPSC_Tx_send,
    thrd_MPSC_Tx_sendFor,
    thrd_MPSC_Tx_sendOp,
    thrd_MPSC_Rx_tryRecv,
    thrd_MPSC_Rx_recv,
    thrd_MPSC_Rx_recvFor,
    thrd_MPSC_Rx_recvOp,
    thrd_Select,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll
));
TEST_fn_("thrd: channel - typed MPSC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_waitSrc(thrd_CancelTok_Src_tok(&chan_cancel));

    var mpsc_buf = A_zero$((A$$(2, u16)));
    var mpsc = thrd_MPSC_init$u16(A_ref$((S$u16)(mpsc_buf)));
    defer_(thrd_MPSC_fini$u16(&mpsc));
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
    var mpsc_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var mpsc_result = u16_(0);
    var mpsc_select = thrd_Select_init$u16(A_ref$((S$thrd_Select_Case)(mpsc_cases)));
    var mpsc_recv_op = thrd_MPSC_Rx_recvOp$u16(mpsc_rx);
    thrd_Select_watch$u16(&mpsc_select, 0, thrd_MPSC_Rx_RecvOp_op(&mpsc_recv_op), &mpsc_result);
    try_(thrd_MPSC_Tx_trySend$u16(mpsc_tx, 25));
    let mpsc_done = thrd_Select_poll$u16(&mpsc_select);
    try_(TEST_expect(isSome(mpsc_done)));
    try_(TEST_expect(unwrap_(mpsc_done).field_idx == 0));
    try_(TEST_expect(unwrap_(mpsc_done).result == 25));
    try_(TEST_expect(mpsc_result == 25));
    var mpsc_send_item = u16_(26);
    var mpsc_send_op = thrd_MPSC_Tx_sendOp$u16(mpsc_tx, &mpsc_send_item);
    var_(mpsc_send_result, Void) = Void_();
    var mpsc_send_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var mpsc_send_select = thrd_Select_init$Void(A_ref$((S$thrd_Select_Case)(mpsc_send_cases)));
    thrd_Select_watch$Void(&mpsc_send_select, 0, thrd_MPSC_Tx_SendOp_op(&mpsc_send_op), &mpsc_send_result);
    let mpsc_send_done = thrd_Select_poll$Void(&mpsc_send_select);
    try_(TEST_expect(isSome(mpsc_send_done)));
    try_(TEST_expect(unwrap_(mpsc_send_done).field_idx == 0));
    try_(TEST_expect(try_(thrd_MPSC_Rx_tryRecv$u16(mpsc_rx)) == 26));

    return_ok({});
} $unguarded(TEST_fn)

T_use$((u32)(
    thrd_SPMC,
    thrd_SPMC_Tx,
    thrd_SPMC_Rx,
    thrd_SPMC_init,
    thrd_SPMC_fini,
    thrd_SPMC_isFull,
    thrd_SPMC_isEmpty,
    thrd_SPMC_isOpen,
    thrd_SPMC_tx,
    thrd_SPMC_rx,
    thrd_SPMC_Tx_trySend,
    thrd_SPMC_Tx_send,
    thrd_SPMC_Tx_sendFor,
    thrd_SPMC_Tx_sendOp,
    thrd_SPMC_Rx_tryRecv,
    thrd_SPMC_Rx_recv,
    thrd_SPMC_Rx_recvFor,
    thrd_SPMC_Rx_recvOp,
    thrd_Select,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll
));
TEST_fn_("thrd: channel - typed SPMC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_waitSrc(thrd_CancelTok_Src_tok(&chan_cancel));

    var spmc_buf = A_zero$((A$$(2, u32)));
    var spmc = thrd_SPMC_init$u32(A_ref$((S$u32)(spmc_buf)));
    defer_(thrd_SPMC_fini$u32(&spmc));
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
    var spmc_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var spmc_result = u32_(0);
    var spmc_select = thrd_Select_init$u32(A_ref$((S$thrd_Select_Case)(spmc_cases)));
    var spmc_recv_op = thrd_SPMC_Rx_recvOp$u32(spmc_rx);
    thrd_Select_watch$u32(&spmc_select, 0, thrd_SPMC_Rx_RecvOp_op(&spmc_recv_op), &spmc_result);
    try_(thrd_SPMC_Tx_trySend$u32(spmc_tx, 36));
    let spmc_done = thrd_Select_poll$u32(&spmc_select);
    try_(TEST_expect(isSome(spmc_done)));
    try_(TEST_expect(unwrap_(spmc_done).field_idx == 0));
    try_(TEST_expect(unwrap_(spmc_done).result == 36));
    try_(TEST_expect(spmc_result == 36));
    var spmc_send_item = u32_(37);
    var spmc_send_op = thrd_SPMC_Tx_sendOp$u32(spmc_tx, &spmc_send_item);
    var_(spmc_send_result, Void) = Void_();
    var spmc_send_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var spmc_send_select = thrd_Select_init$Void(A_ref$((S$thrd_Select_Case)(spmc_send_cases)));
    thrd_Select_watch$Void(&spmc_send_select, 0, thrd_SPMC_Tx_SendOp_op(&spmc_send_op), &spmc_send_result);
    let spmc_send_done = thrd_Select_poll$Void(&spmc_send_select);
    try_(TEST_expect(isSome(spmc_send_done)));
    try_(TEST_expect(unwrap_(spmc_send_done).field_idx == 0));
    try_(TEST_expect(try_(thrd_SPMC_Rx_tryRecv$u32(spmc_rx)) == 37));

    return_ok({});
} $unguarded(TEST_fn)

T_use$((u64)(
    thrd_MPMC,
    thrd_MPMC_Tx,
    thrd_MPMC_Rx,
    thrd_MPMC_init,
    thrd_MPMC_fini,
    thrd_MPMC_isFull,
    thrd_MPMC_isEmpty,
    thrd_MPMC_isOpen,
    thrd_MPMC_tx,
    thrd_MPMC_rx,
    thrd_MPMC_Tx_trySend,
    thrd_MPMC_Tx_send,
    thrd_MPMC_Tx_sendFor,
    thrd_MPMC_Tx_sendOp,
    thrd_MPMC_Rx_tryRecv,
    thrd_MPMC_Rx_recv,
    thrd_MPMC_Rx_recvFor,
    thrd_MPMC_Rx_recvOp,
    thrd_Select,
    thrd_Select_init,
    thrd_Select_watch,
    thrd_Select_poll
));
TEST_fn_("thrd: channel - typed MPMC templates send and receive" $guard) {
    var chan_cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    let chan_cancel_src = thrd_CancelTok_waitSrc(thrd_CancelTok_Src_tok(&chan_cancel));

    var mpmc_buf = A_zero$((A$$(2, u64)));
    var mpmc = thrd_MPMC_init$u64(A_ref$((S$u64)(mpmc_buf)));
    defer_(thrd_MPMC_fini$u64(&mpmc));
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
    var mpmc_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var mpmc_result = u64_(0);
    var mpmc_select = thrd_Select_init$u64(A_ref$((S$thrd_Select_Case)(mpmc_cases)));
    var mpmc_recv_op = thrd_MPMC_Rx_recvOp$u64(mpmc_rx);
    thrd_Select_watch$u64(&mpmc_select, 0, thrd_MPMC_Rx_RecvOp_op(&mpmc_recv_op), &mpmc_result);
    try_(thrd_MPMC_Tx_trySend$u64(mpmc_tx, 47));
    let mpmc_done = thrd_Select_poll$u64(&mpmc_select);
    try_(TEST_expect(isSome(mpmc_done)));
    try_(TEST_expect(unwrap_(mpmc_done).field_idx == 0));
    try_(TEST_expect(unwrap_(mpmc_done).result == 47));
    try_(TEST_expect(mpmc_result == 47));
    var mpmc_send_item = u64_(48);
    var mpmc_send_op = thrd_MPMC_Tx_sendOp$u64(mpmc_tx, &mpmc_send_item);
    var_(mpmc_send_result, Void) = Void_();
    var mpmc_send_cases = A_zero$((A$$(1, thrd_Select_Case)));
    var mpmc_send_select = thrd_Select_init$Void(A_ref$((S$thrd_Select_Case)(mpmc_send_cases)));
    thrd_Select_watch$Void(&mpmc_send_select, 0, thrd_MPMC_Tx_SendOp_op(&mpmc_send_op), &mpmc_send_result);
    let mpmc_send_done = thrd_Select_poll$Void(&mpmc_send_select);
    try_(TEST_expect(isSome(mpmc_send_done)));
    try_(TEST_expect(unwrap_(mpmc_send_done).field_idx == 0));
    try_(TEST_expect(try_(thrd_MPMC_Rx_tryRecv$u64(mpmc_rx)) == 48));

    return_ok({});
} $unguarded(TEST_fn)
