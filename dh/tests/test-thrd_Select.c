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
T_use$((i8)(
    thrd_Que,
    thrd_Que_init,
    thrd_Que_fini,
    thrd_Que_trySend,
    thrd_Que_tryRecv,
    thrd_Que_sendOp,
    thrd_Que_SendOp_op,
    thrd_Que_recvOp
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

$static fn_((test_thrd_channel__cancelWakeable(thrd_CancelTok_Src* src))(thrd_Wakeable)) {
    claim_assert_nonnull(src);
    *src = thrd_CancelTok_Src_init();
    return thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(src));
};

TEST_fn_("thrd/Select: canceled wait returns error" $guard) {
    var select_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(select_arms)));
    var_(select_cancel, thrd_CancelTok_Src) = cleared();
    let_ignore = test_thrd_channel__cancelWakeable(&select_cancel);
    defer_(thrd_CancelTok_Src_fini(&select_cancel));
    thrd_CancelTok_Src_cancel(&select_cancel);
    let canceled_select = thrd_Select_wait$test_thrd_SelectI8(
        &select, thrd_CancelTok_Src_tok(&select_cancel)
    );
    try_(TEST_expect(isErr(canceled_select)));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: poll returns copied receive arm" $guard) {
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
    var select_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(select_arms)));
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
    let preserved = *P_at((thrd_Select_Arm_result$test_thrd_SelectI8(&que_recv_case))[0]);
    try_(TEST_expect(matches(preserved, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((preserved)(test_thrd_SelectI8_recv)) == 9));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: pollMut returns ready fixed arm" $guard) {
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
    let pair_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8Pair(pair_case)
    )[0]);
    try_(TEST_expect(matches(pair_result, test_thrd_SelectI8Pair_right)));
    try_(TEST_expect(union_to((pair_result)(test_thrd_SelectI8Pair_right)) == 31));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: watch grows with allocator" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var left_buf = A_zero$((A$$(1, i8)));
    var left_que = thrd_Que_init$i8(A_ref$((S$i8)(left_buf)));
    defer_(thrd_Que_fini$i8(&left_que));
    var right_buf = A_zero$((A$$(1, i8)));
    var right_que = thrd_Que_init$i8(A_ref$((S$i8)(right_buf)));
    defer_(thrd_Que_fini$i8(&right_que));
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
    let grow_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8Pair(grow_case)
    )[0]);
    try_(TEST_expect(matches(grow_result, test_thrd_SelectI8Pair_left)));
    try_(TEST_expect(union_to((grow_result)(test_thrd_SelectI8Pair_left)) == 32));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: from uses prepared arm list" $guard) {
    var left_buf = A_zero$((A$$(1, i8)));
    var left_que = thrd_Que_init$i8(A_ref$((S$i8)(left_buf)));
    defer_(thrd_Que_fini$i8(&left_que));
    var right_buf = A_zero$((A$$(1, i8)));
    var right_que = thrd_Que_init$i8(A_ref$((S$i8)(right_buf)));
    defer_(thrd_Que_fini$i8(&right_que));
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
    let from_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8Pair(from_case)
    )[0]);
    try_(TEST_expect(matches(from_result, test_thrd_SelectI8Pair_right)));
    try_(TEST_expect(union_to((from_result)(test_thrd_SelectI8Pair_right)) == 33));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: send op commits selected value" $guard) {
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
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
    try_(TEST_expect(!matches(
        *P_at((thrd_Select_Arm_result$test_thrd_SelectI8(&que_send_case))[0]),
        test_thrd_SelectI8_send
    )));
    try_(TEST_expect(try_(thrd_Que_tryRecv$i8(&que)) == 10));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: pollMut returns internal receive arm" $guard) {
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
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
    let que_poll_mut_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8(que_poll_mut_case)
    )[0]);
    try_(TEST_expect(matches(que_poll_mut_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_poll_mut_result)(test_thrd_SelectI8_recv)) == 11));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: waitMut returns internal receive arm" $guard) {
    var_(chan_cancel, thrd_CancelTok_Src) = cleared();
    let_ignore = test_thrd_channel__cancelWakeable(&chan_cancel);
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
    var que_wait_mut_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_wait_mut_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_wait_mut_arms)));
    var que_wait_mut_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_wait_mut_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_wait_mut_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 12));
    let que_wait_mut_case = try_(thrd_Select_waitMut$test_thrd_SelectI8(&que_wait_mut_select, thrd_CancelTok_Src_tok(&chan_cancel)));
    let que_wait_mut_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_case)
    )[0]);
    try_(TEST_expect(matches(que_wait_mut_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_result)(test_thrd_SelectI8_recv)) == 12));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: waitMutFor returns internal receive arm" $guard) {
    var_(chan_cancel, thrd_CancelTok_Src) = cleared();
    let_ignore = test_thrd_channel__cancelWakeable(&chan_cancel);
    defer_(thrd_CancelTok_Src_fini(&chan_cancel));
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
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
    let que_wait_mut_for_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_for_case)
    )[0]);
    try_(TEST_expect(matches(que_wait_mut_for_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_for_result)(test_thrd_SelectI8_recv)) == 13));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: waitMutProtcd returns internal receive arm" $guard) {
    var que_buf = A_zero$((A$$(1, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(que_buf)));
    defer_(thrd_Que_fini$i8(&que));
    var que_wait_mut_protcd_arms = A_zero$((A$$(1, thrd_Select_Arm$test_thrd_SelectI8)));
    var que_wait_mut_protcd_select = thrd_Select_fixed$test_thrd_SelectI8(A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8)(que_wait_mut_protcd_arms)));
    var que_wait_mut_protcd_recv_op = thrd_Que_recvOp$i8(&que);
    thrd_Select_watchWithin$test_thrd_SelectI8(
        &que_wait_mut_protcd_select, test_thrd_SelectI8_recv, thrd_Que_RecvOp_op(&que_wait_mut_protcd_recv_op)
    );
    try_(thrd_Que_trySend$i8(&que, 14));
    let que_wait_mut_protcd_case = thrd_Select_waitMutProtcd$test_thrd_SelectI8(&que_wait_mut_protcd_select);
    let que_wait_mut_protcd_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8(que_wait_mut_protcd_case)
    )[0]);
    try_(TEST_expect(matches(que_wait_mut_protcd_result, test_thrd_SelectI8_recv)));
    try_(TEST_expect(union_to((que_wait_mut_protcd_result)(test_thrd_SelectI8_recv)) == 14));

    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd/Select: pollMut rotates ready arms" $guard) {
    var fair_left_buf = A_zero$((A$$(2, i8)));
    var fair_left_que = thrd_Que_init$i8(A_ref$((S$i8)(fair_left_buf)));
    defer_(thrd_Que_fini$i8(&fair_left_que));
    var fair_right_buf = A_zero$((A$$(2, i8)));
    var fair_right_que = thrd_Que_init$i8(A_ref$((S$i8)(fair_right_buf)));
    defer_(thrd_Que_fini$i8(&fair_right_que));
    var fair_arms = A_zero$((A$$(2, thrd_Select_Arm$test_thrd_SelectI8Pair)));
    var fair_select = thrd_Select_fixed$test_thrd_SelectI8Pair(
        A_ref$((S$thrd_Select_Arm$test_thrd_SelectI8Pair)(fair_arms))
    );
    var fair_left_recv_op = thrd_Que_recvOp$i8(&fair_left_que);
    thrd_Select_watchWithin$test_thrd_SelectI8Pair(
        &fair_select, test_thrd_SelectI8Pair_left, thrd_Que_RecvOp_op(&fair_left_recv_op)
    );
    var fair_right_recv_op = thrd_Que_recvOp$i8(&fair_right_que);
    thrd_Select_watchWithin$test_thrd_SelectI8Pair(
        &fair_select, test_thrd_SelectI8Pair_right, thrd_Que_RecvOp_op(&fair_right_recv_op)
    );
    try_(thrd_Que_trySend$i8(&fair_left_que, 41));
    try_(thrd_Que_trySend$i8(&fair_left_que, 42));
    try_(thrd_Que_trySend$i8(&fair_right_que, 51));
    try_(thrd_Que_trySend$i8(&fair_right_que, 52));
    let fair_first = unwrap_(thrd_Select_pollMut$test_thrd_SelectI8Pair(&fair_select));
    let fair_first_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8Pair(fair_first)
    )[0]);
    try_(TEST_expect(matches(fair_first_result, test_thrd_SelectI8Pair_left)));
    try_(TEST_expect(union_to((fair_first_result)(test_thrd_SelectI8Pair_left)) == 41));
    let fair_second = unwrap_(thrd_Select_pollMut$test_thrd_SelectI8Pair(&fair_select));
    let fair_second_result = *P_at((
        thrd_Select_Arm_result$test_thrd_SelectI8Pair(fair_second)
    )[0]);
    try_(TEST_expect(matches(fair_second_result, test_thrd_SelectI8Pair_right)));
    try_(TEST_expect(union_to((fair_second_result)(test_thrd_SelectI8Pair_right)) == 51));

    return_ok({});
} $unguarded(TEST_fn);
