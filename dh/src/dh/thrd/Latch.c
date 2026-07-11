#include "dh/thrd/Latch.h"

#define thrd_Latch__is_waiting (as$(usize)(1) << 0)
#define thrd_Latch__one_pending (as$(usize)(1) << 1)

fn_((thrd_Latch_init(void))(thrd_Latch)) {
    return thrd_Latch_init_static();
};

fn_((thrd_Latch_fini(thrd_Latch* self))(void)) {
    thrd_OnceEvt_fini(&self->done_evt);
    atom_V_store(&self->state, 0, atom_MemOrd_monotonic);
};

fn_((thrd_Latch_start(thrd_Latch* self))(void)) {
    thrd_Latch_startOn(&self->state);
};

fn_((thrd_Latch_startOn(atom_V$usize* state))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_Latch__one_pending, atom_MemOrd_monotonic);
    claim_assert((prev_state & thrd_Latch__is_waiting) == 0);
    claim_assert((prev_state / thrd_Latch__one_pending) < (usize_limit_max / thrd_Latch__one_pending));
};

fn_((thrd_Latch_startN(thrd_Latch* self, usize n))(void)) {
    thrd_Latch_startNOn(&self->state, n);
};

fn_((thrd_Latch_startNOn(atom_V$usize* state, usize n))(void)) {
    if (n == 0) { return; }
    let prev_state = atom_V_pri_fetchAdd(state, thrd_Latch__one_pending * n, atom_MemOrd_monotonic);
    claim_assert((prev_state & thrd_Latch__is_waiting) == 0);
    claim_assert((prev_state / thrd_Latch__one_pending) <= (usize_limit_max / thrd_Latch__one_pending) - n);
};

fn_((thrd_Latch_finish(thrd_Latch* self))(void)) {
    thrd_Latch_finishOn(&self->state, &self->done_evt);
};

fn_((thrd_Latch_finishOn(atom_V$usize* state, thrd_OnceEvt* event))(void)) {
    let prev_state = atom_V_pri_fetchSub(state, thrd_Latch__one_pending, atom_MemOrd_acq_rel);
    claim_assert((prev_state / thrd_Latch__one_pending) > 0);
    if (prev_state == (thrd_Latch__one_pending | thrd_Latch__is_waiting)) {
        thrd_OnceEvt_set(event);
    }
};

fn_((thrd_Latch_wait(thrd_Latch* self))(void)) {
    thrd_Latch_waitOn(&self->state, &self->done_evt);
};

fn_((thrd_Latch_waitOn(atom_V$usize* state, thrd_OnceEvt* event))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_Latch__is_waiting, atom_MemOrd_acquire);
    claim_assert((prev_state & thrd_Latch__is_waiting) == 0);
    if ((prev_state / thrd_Latch__one_pending) > 0) {
        thrd_OnceEvt_wait(event);
    }
};

fn_((thrd_Latch_timedWait(thrd_Latch* self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_Latch_timedWaitOn(&self->state, &self->done_evt, timeout);
};

fn_((thrd_Latch_timedWaitOn(atom_V$usize* state, thrd_OnceEvt* event, time_Dur timeout))(thrd_ftx_E$void) $scope) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_Latch__is_waiting, atom_MemOrd_acquire);
    claim_assert((prev_state & thrd_Latch__is_waiting) == 0);
    if ((prev_state / thrd_Latch__one_pending) == 0) {
        return_ok({});
    }
    return thrd_OnceEvt_timedWait(event, timeout);
} $unscoped(fn);

fn_((thrd_Latch_isDone(thrd_Latch* self))(bool)) {
    return thrd_Latch_isDoneOn(&self->state);
};

fn_((thrd_Latch_isDoneOn(atom_V$usize* state))(bool)) {
    let prev_state = atom_V_load(state, atom_MemOrd_acquire);
    claim_assert((prev_state & thrd_Latch__is_waiting) == 0);
    return (prev_state / thrd_Latch__one_pending) == 0;
};

fn_((thrd_Latch_value(thrd_Latch* self))(usize)) {
    return thrd_Latch_valueOn(&self->state);
};

fn_((thrd_Latch_valueOn(atom_V$usize* state))(usize)) {
    return atom_V_load(state, atom_MemOrd_monotonic) / thrd_Latch__one_pending;
};

fn_((thrd_Latch_tok(thrd_Latch* self))(thrd_OnceEvt_Tok)) {
    return thrd_OnceEvt_tok(&self->done_evt);
};
