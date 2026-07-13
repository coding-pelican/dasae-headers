#include "dh/thrd/Latch.h"

fn_((thrd_Latch_init(usize count))(thrd_Latch)) {
    return thrd_Latch_init_static(count);
};
fn_((thrd_Latch_initDone(void))(thrd_Latch)) {
    return thrd_Latch_initDone_static();
};
fn_((thrd_Latch_initPending(usize count))(thrd_Latch)) {
    return thrd_Latch_initPending_static(count);
};
fn_((thrd_Latch_fini(thrd_Latch* self))(void)) {
    thrd_OnceEvt_fini(&self->done_evt);
    atom_V_store(&self->state, 0, atom_MemOrd_monotonic);
};

fn_((thrd_Latch_tok(thrd_Latch* self))(thrd_OnceEvt_Tok)) {
    return thrd_OnceEvt_tok(&self->done_evt);
};

fn_((thrd_Latch_start(thrd_Latch* self))(void)) {
    thrd_Latch_startOn(&self->state);
};
fn_((thrd_Latch_startOn(atom_V$usize* state))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, 1, atom_MemOrd_monotonic);
    claim_assert(prev_state < usize_limit_max);
};
fn_((thrd_Latch_startN(thrd_Latch* self, usize n))(void)) {
    thrd_Latch_startNOn(&self->state, n);
};
fn_((thrd_Latch_startNOn(atom_V$usize* state, usize n))(void)) {
    if (n == 0) return;
    let prev_state = atom_V_pri_fetchAdd(state, n, atom_MemOrd_monotonic);
    claim_assert(prev_state <= usize_limit_max - n);
};
fn_((thrd_Latch_finish(thrd_Latch* self))(void)) {
    thrd_Latch_finishOn(&self->state, &self->done_evt);
};
fn_((thrd_Latch_finishOn(atom_V$usize* state, thrd_OnceEvt* event))(void)) {
    let prev_state = atom_V_pri_fetchSub(state, 1, atom_MemOrd_acq_rel);
    claim_assert(prev_state > 0);
    if (prev_state == 1) {
        thrd_OnceEvt_set(event);
    }
};

fn_((thrd_Latch_isDone(thrd_Latch* self))(bool)) {
    return thrd_Latch_isDoneOn(&self->state);
};
fn_((thrd_Latch_isDoneOn(atom_V$usize* state))(bool)) {
    return atom_V_load(state, atom_MemOrd_acquire) == 0;
};
fn_((thrd_Latch_value(thrd_Latch* self))(usize)) {
    return thrd_Latch_valueOn(&self->state);
};
fn_((thrd_Latch_valueOn(atom_V$usize* state))(usize)) {
    return atom_V_load(state, atom_MemOrd_monotonic);
};

fn_((thrd_Latch_tryWait(thrd_Latch* self))(bool)) {
    return thrd_Latch_tryWaitOn(&self->state);
};
fn_((thrd_Latch_tryWaitOn(atom_V$usize* state))(bool)) {
    return thrd_Latch_isDoneOn(state);
};
fn_((thrd_Latch_wait(
    thrd_Latch* self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void)) {
    return thrd_Latch_waitOn(&self->state, &self->done_evt, cancel_src);
};
fn_((thrd_Latch_waitOn(
    atom_V$usize* state, thrd_OnceEvt* event, thrd_Wakeable cancel_src
))(Sched_Cancelable$void) $scope) {
    if (atom_V_load(state, atom_MemOrd_acquire) == 0) {
        return_ok({});
    }
    return_(thrd_OnceEvt_wait(event, cancel_src));
} $unscoped(fn);
fn_((thrd_Latch_waitFor(
    thrd_Latch* self, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void)) {
    return thrd_Latch_waitForOn(&self->state, &self->done_evt, cancel_src, timeout);
};
fn_((thrd_Latch_waitForOn(
    atom_V$usize* state, thrd_OnceEvt* event, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void) $scope) {
    if (atom_V_load(state, atom_MemOrd_acquire) == 0) {
        return_ok({});
    }
    return_(thrd_OnceEvt_waitFor(event, cancel_src, timeout));
} $unscoped(fn);
fn_((thrd_Latch_waitProtcd(thrd_Latch* self))(void)) {
    thrd_Latch_waitOnProtcd(&self->state, &self->done_evt);
};
fn_((thrd_Latch_waitOnProtcd(atom_V$usize* state, thrd_OnceEvt* event))(void)) {
    if (atom_V_load(state, atom_MemOrd_acquire) > 0) {
        thrd_OnceEvt_waitProtcd(event);
    }
};
