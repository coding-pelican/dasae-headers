#include "ResetEvent.h"

/*========== Internal Declarations ==========================================*/

#define thrd_ResetEvent__unset 0u
#define thrd_ResetEvent__waiting 1u
#define thrd_ResetEvent__is_set 2u

$attr($must_check)
$static fn_((thrd_ResetEvent__wait(thrd_ResetEvent* self, O$time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$static fn_((thrd_ResetEvent__waitUntilSet(thrd_ResetEvent* self, O$time_Dur timeout))(thrd_ftx_E$void));

/*========== External Definitions ===========================================*/

fn_((thrd_ResetEvent_init(void))(thrd_ResetEvent)) {
    return thrd_ResetEvent_init_static();
};

fn_((thrd_ResetEvent_fini(thrd_ResetEvent* self))(void)) {
    atom_V_store(&self->state, thrd_ResetEvent__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvent_wait(thrd_ResetEvent* self))(void) $scope) {
    return_void(catch_((thrd_ResetEvent__wait(self, none$((O$time_Dur))))($ignore, claim_unreachable)));
} $unscoped(fn);

fn_((thrd_ResetEvent_timedWait(thrd_ResetEvent* self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_ResetEvent__wait(self, some$((O$time_Dur)(timeout)));
};

fn_((thrd_ResetEvent_set(thrd_ResetEvent* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_ResetEvent__is_set) {
        return;
    }
    if (atom_V_fetchXchg(&self->state, thrd_ResetEvent__is_set, atom_MemOrd_release) == thrd_ResetEvent__waiting) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
};

fn_((thrd_ResetEvent_reset(thrd_ResetEvent* self))(void)) {
    atom_V_store(&self->state, thrd_ResetEvent__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvent_isSet(const thrd_ResetEvent* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_ResetEvent__is_set;
};

/*========== Internal Definitions ===========================================*/

fn_((thrd_ResetEvent__wait(thrd_ResetEvent* self, O$time_Dur timeout))(thrd_ftx_E$void)) {
    return expr_(thrd_ftx_E$void $scope)(if (!thrd_ResetEvent_isSet(self)) {
        $break_(thrd_ResetEvent__waitUntilSet(self, timeout));
    } else {
        $break_(ok({}));
    }) $unscoped(expr);
};

$attr($branch_cold)
fn_((thrd_ResetEvent__waitUntilSet(thrd_ResetEvent* self, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    var state = atom_V_load(&self->state, atom_MemOrd_acquire);
    if (state == thrd_ResetEvent__unset) {
        state = orelse_((atom_V_cmpXchgStrong$(
            O$u32, &self->state, state, thrd_ResetEvent__waiting, atom_MemOrd_acquire, atom_MemOrd_acquire
        ))(thrd_ResetEvent__waiting));
    }
    if (state == thrd_ResetEvent__waiting) {
        var deadline = thrd_ftx_Deadline_init(timeout);
        while (true) {
            let waiting = thrd_ftx_Deadline_wait(&deadline, &self->state, thrd_ResetEvent__waiting);
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != thrd_ResetEvent__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == thrd_ResetEvent__is_set);
    return_ok({});
} $unscoped(fn);
