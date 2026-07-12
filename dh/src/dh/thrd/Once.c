#include "dh/thrd/Once.h"

#define thrd_Once__init 0u
#define thrd_Once__running 1u
#define thrd_Once__done 2u

T_use$((Void)(clsr_invokeToComplete));

fn_((thrd_Once_init(void))(thrd_Once)) {
    return thrd_Once_init_static();
};
fn_((thrd_Once_fini(thrd_Once* self))(void)) {
    atom_V_store(&self->state, thrd_Once__init, atom_MemOrd_monotonic);
};

fn_((thrd_Once_tryBegin(thrd_Once* self))(bool)) {
    return isNone(atom_V_cmpXchgStrong$(
        O$u32, &self->state, thrd_Once__init, thrd_Once__running,
        atom_MemOrd_acq_rel, atom_MemOrd_acquire
    ));
};
fn_((thrd_Once_finish(thrd_Once* self))(void)) {
    let prev = atom_V_fetchXchg(&self->state, thrd_Once__done, atom_MemOrd_release);
    claim_assert(prev == thrd_Once__running || prev == thrd_Once__done);
    if (prev == thrd_Once__running) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
};
fn_((thrd_Once_run(thrd_Once* self, Clsr$Void* clsr))(void)) {
    if (thrd_Once_tryBegin(self)) {
        clsr_invokeToComplete$Void(clsr);
        thrd_Once_finish(self);
        return;
    }
    thrd_Once_wait(self);
};

fn_((thrd_Once_isDone(const thrd_Once* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_Once__done;
};
fn_((thrd_Once_wait(thrd_Once* self))(void)) {
    while (atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_Once__running) {
        thrd_ftx_waitProtcd(&self->state, thrd_Once__running);
    }
    claim_assert(atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_Once__done);
};
