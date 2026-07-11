#include "dh/conc/Latch.h"
#include "dh/Sched.h"

fn_((conc_Latch_init(usize count))(conc_Latch)) {
    return conc_Latch_init_static(count);
};

fn_((conc_Latch_add(conc_Latch* self, usize n))(void)) {
    if (n == 0) return;
    let prev = atom_V_pri_fetchAdd(&self->count, n, atom_MemOrd_acq_rel);
    claim_assert(prev <= usize_limit_max - n);
};

fn_((conc_Latch_done(conc_Latch* self))(void)) {
    return conc_Latch_doneN(self, 1);
};

fn_((conc_Latch_doneN(conc_Latch* self, usize n))(void)) {
    if (n == 0) return;
    let prev = atom_V_pri_fetchSub(&self->count, n, atom_MemOrd_acq_rel);
    claim_assert(n <= prev);
    if (prev == n) conc_OnceEvt_set(&self->done_evt);
};

fn_((conc_Latch_isDone(const conc_Latch* self))(bool)) {
    return atom_V_load(&self->count, atom_MemOrd_acquire) == 0;
};

fn_((conc_Latch_value(const conc_Latch* self))(usize)) {
    return atom_V_load(&self->count, atom_MemOrd_acquire);
};

fn_((conc_Latch_wait(conc_Latch* self, Sched sched))(Sched_Cancelable$void)) {
    return conc_OnceEvt_wait(&self->done_evt, sched);
};

fn_((conc_Latch_waitFor(conc_Latch* self, Sched sched, time_Dur dur))(Sched_TimedE$void)) {
    return conc_OnceEvt_waitFor(&self->done_evt, sched, dur);
};

fn_((conc_Latch_waitUntil(conc_Latch* self, Sched sched, time_Inst deadline))(Sched_TimedE$void)) {
    return conc_OnceEvt_waitUntil(&self->done_evt, sched, deadline);
};
