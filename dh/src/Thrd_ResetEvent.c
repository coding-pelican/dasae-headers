#include "dh/Thrd/ResetEvent.h"
#include "dh/time/Duration.h"

#define Thrd_ResetEvent__unset 0u
#define Thrd_ResetEvent__waiting 1u
#define Thrd_ResetEvent__is_set 2u

$attr($must_check)
$static fn_((Thrd_ResetEvent__wait(Thrd_ResetEvent* self, O$time_Duration timeout))(Thrd_ResetEvent_Err$void));
$attr($must_check)
$static fn_((Thrd_ResetEvent__waitUntilSet(Thrd_ResetEvent* self, O$time_Duration timeout))(Thrd_ResetEvent_Err$void));

fn_((Thrd_ResetEvent_init(void))(Thrd_ResetEvent)) {
    return (Thrd_ResetEvent){
        .state = atom_V_init(Thrd_ResetEvent__unset),
    };
};

fn_((Thrd_ResetEvent_fini(Thrd_ResetEvent* self))(void)) {
    atom_V_store(&self->state, Thrd_ResetEvent__unset, atom_MemOrd_monotonic);
};

fn_((Thrd_ResetEvent_wait(Thrd_ResetEvent* self))(void) $scope) {
    return_void(catch_((Thrd_ResetEvent__wait(self, none$((O$time_Duration))))($ignore, claim_unreachable)));
} $unscoped_(fn);

fn_((Thrd_ResetEvent_timedWait(Thrd_ResetEvent* self, time_Duration timeout))(Thrd_ResetEvent_Err$void)) {
    return Thrd_ResetEvent__wait(self, some$((O$time_Duration)(timeout)));
};

fn_((Thrd_ResetEvent_set(Thrd_ResetEvent* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == Thrd_ResetEvent__is_set) {
        return;
    }
    if (atom_V_fetchXchg(&self->state, Thrd_ResetEvent__is_set, atom_MemOrd_release) == Thrd_ResetEvent__waiting) {
        Thrd_Ftx_wake(&self->state, u32_limit_max);
    }
};

fn_((Thrd_ResetEvent_reset(Thrd_ResetEvent* self))(void)) {
    atom_V_store(&self->state, Thrd_ResetEvent__unset, atom_MemOrd_monotonic);
};

fn_((Thrd_ResetEvent_isSet(const Thrd_ResetEvent* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == Thrd_ResetEvent__is_set;
};

fn_((Thrd_ResetEvent__wait(Thrd_ResetEvent* self, O$time_Duration timeout))(Thrd_ResetEvent_Err$void)) {
    return expr_(Thrd_ResetEvent_Err$void $scope)(if (!Thrd_ResetEvent_isSet(self)) {
        $break_(Thrd_ResetEvent__waitUntilSet(self, timeout));
    } else {
        $break_(ok({}));
    }) $unscoped_(expr);
};

$attr($branch_cold)
fn_((Thrd_ResetEvent__waitUntilSet(Thrd_ResetEvent* self, O$time_Duration timeout))(Thrd_ResetEvent_Err$void) $scope) {
    var state = atom_V_load(&self->state, atom_MemOrd_acquire);
    if (state == Thrd_ResetEvent__unset) {
        state = orelse_((atom_V_cmpXchgStrong$(
            O$u32, &self->state, state, Thrd_ResetEvent__waiting, atom_MemOrd_acquire, atom_MemOrd_acquire
        ))(Thrd_ResetEvent__waiting));
    }
    if (state == Thrd_ResetEvent__waiting) {
        var deadline = Thrd_Ftx_Deadline_init(timeout);
        while (true) {
            let waiting = Thrd_Ftx_Deadline_wait(&deadline, &self->state, Thrd_ResetEvent__waiting);
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != Thrd_ResetEvent__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == Thrd_ResetEvent__is_set);
    return_ok({});
} $unscoped_(fn);
