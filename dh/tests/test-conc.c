#include "dh-main.h"
#include "dh/conc.h"
#include "dh/Sched.h"

T_use$((u32)(u_V, u_P, u_S));

TEST_fn_("conc/OnceEvt: set releases waiters immediately" $scope) {
    var evt = conc_OnceEvt_init();
    try_(TEST_expect(!conc_OnceEvt_isSet(&evt)));

    conc_OnceEvt_set(&evt);

    try_(TEST_expect(conc_OnceEvt_isSet(&evt)));
    try_(conc_OnceEvt_wait(&evt, Sched_noop));
    try_(conc_OnceEvt_Tok_wait(conc_OnceEvt_tok(&evt), Sched_noop));
} $unscoped(TEST_fn);

TEST_fn_("conc/OnceEvt: zero wait times out while unset" $scope) {
    var evt = conc_OnceEvt_init();
    let caught = eval_(bool $scope)(catch_((conc_OnceEvt_waitFor(&evt, Sched_noop, time_Dur_zero))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Timeout().as_any)));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(caught));
} $unscoped(TEST_fn);

TEST_fn_("conc/ResetEvt: reset affects later waits" $scope) {
    var evt = conc_ResetEvt_init();

    conc_ResetEvt_set(&evt);
    try_(conc_ResetEvt_wait(&evt, Sched_noop));

    conc_ResetEvt_reset(&evt);
    try_(TEST_expect(!conc_ResetEvt_isSet(&evt)));

    let caught = eval_(bool $scope)(catch_((conc_ResetEvt_waitFor(&evt, Sched_noop, time_Dur_zero))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Timeout().as_any)));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(caught));
} $unscoped(TEST_fn);

TEST_fn_("conc/Latch: count reaches done event" $scope) {
    var latch = conc_Latch_init(2);

    try_(TEST_expect(!conc_Latch_isDone(&latch)));
    try_(TEST_expect(conc_Latch_value(&latch) == 2));

    conc_Latch_done(&latch);
    try_(TEST_expect(!conc_Latch_isDone(&latch)));

    conc_Latch_done(&latch);
    try_(TEST_expect(conc_Latch_isDone(&latch)));
    try_(conc_Latch_wait(&latch, Sched_noop));
} $unscoped(TEST_fn);

TEST_fn_("conc/Que: sends and receives buffered values" $scope) {
    var storage = A_from$((u32){ 0, 0 });
    let storage_s = A_ref$((S$u32)(storage));
    var que = conc_Que_fixed(
        typeInfo$(u32),
        (u_S$raw){ .raw = storage_s.as_raw, .type = typeInfo$(u32) }
    );

    try_(conc_Que_trySend(&que, typeInfo$(u32), u_anyV(as$(u32)(11))));
    try_(TEST_expect(conc_Que_len(&que) == 1));

    var out = as$(u32)(0);
    let recv = try_(conc_Que_tryRecv(
        &que,
        typeInfo$(u32),
        (u_V$raw){ .inner = &out, .type = typeInfo$(u32) }
    ));
    try_(TEST_expect(recv.inner == &out));
    try_(TEST_expect(out == 11));
    try_(TEST_expect(conc_Que_len(&que) == 0));
} $unscoped(TEST_fn);

TEST_fn_("conc/Que: zero duration waits time out" $scope) {
    var storage = A_from$((u32){ 0 });
    let storage_s = A_ref$((S$u32)(storage));
    var que = conc_Que_fixed(
        typeInfo$(u32),
        (u_S$raw){ .raw = storage_s.as_raw, .type = typeInfo$(u32) }
    );

    try_(conc_Que_trySend(&que, typeInfo$(u32), u_anyV(as$(u32)(1))));

    let send_timed_out = eval_(bool $scope)(catch_((conc_Que_sendFor(
        &que,
        typeInfo$(u32),
        Sched_noop,
        time_Dur_zero,
        u_anyV(as$(u32)(2))
    ))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Timeout().as_any)));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(send_timed_out));

    var out = as$(u32)(0);
    try_(conc_Que_tryRecv(&que, typeInfo$(u32), (u_V$raw){ .inner = &out, .type = typeInfo$(u32) }));

    let recv_timed_out = eval_(bool $scope)(catch_((conc_Que_recvFor(
        &que,
        typeInfo$(u32),
        Sched_noop,
        time_Dur_zero,
        (u_V$raw){ .inner = &out, .type = typeInfo$(u32) }
    ))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Timeout().as_any)));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(recv_timed_out));
} $unscoped(TEST_fn);
