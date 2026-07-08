#include "dh/thrd/ResetEvt.h"
#include "dh/time/Dur.h"

#define thrd_ResetEvt__unset 0u
#define thrd_ResetEvt__waiting 1u
#define thrd_ResetEvt__is_set 2u

$attr($must_check)
$static fn_((thrd_ResetEvt__wait(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$static fn_((thrd_ResetEvt__waitUntilSet(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));

fn_((thrd_ResetEvt_init(void))(thrd_ResetEvt)) {
    return (thrd_ResetEvt){
        .state = atom_V_init(thrd_ResetEvt__unset),
    };
};

fn_((thrd_ResetEvt_fini(thrd_ResetEvt* self))(void)) {
    atom_V_store(&self->state, thrd_ResetEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvt_tok(thrd_ResetEvt* self))(thrd_ResetEvt_Tok)) {
    return (thrd_ResetEvt_Tok){ .event = self };
};

fn_((thrd_ResetEvt_sig(thrd_ResetEvt* self))(thrd_ResetEvt_Sig)) {
    return (thrd_ResetEvt_Sig){ .event = self };
};

fn_((thrd_ResetEvt_wait(thrd_ResetEvt* self))(void) $scope) {
    return_void(catch_((thrd_ResetEvt__wait(self, none$((O$time_Dur))))($ignore, claim_unreachable)));
} $unscoped(fn);

fn_((thrd_ResetEvt_timedWait(thrd_ResetEvt* self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_ResetEvt__wait(self, some$((O$time_Dur)(timeout)));
};

fn_((thrd_ResetEvt_set(thrd_ResetEvt* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_ResetEvt__is_set) {
        return;
    }
    if (atom_V_fetchXchg(&self->state, thrd_ResetEvt__is_set, atom_MemOrd_release) == thrd_ResetEvt__waiting) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
};

fn_((thrd_ResetEvt_reset(thrd_ResetEvt* self))(void)) {
    atom_V_store(&self->state, thrd_ResetEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvt_isSet(const thrd_ResetEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_ResetEvt__is_set;
};

fn_((thrd_ResetEvt__wait(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void)) {
    return expr_(thrd_ftx_E$void $scope)(if (!thrd_ResetEvt_isSet(self)) {
        $break_(thrd_ResetEvt__waitUntilSet(self, timeout));
    } else {
        $break_(ok({}));
    }) $unscoped(expr);
};

$attr($branch_cold)
fn_((thrd_ResetEvt__waitUntilSet(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    var state = atom_V_load(&self->state, atom_MemOrd_acquire);
    if (state == thrd_ResetEvt__unset) {
        state = orelse_((atom_V_cmpXchgStrong$(
            O$u32, &self->state, state, thrd_ResetEvt__waiting, atom_MemOrd_acquire, atom_MemOrd_acquire
        ))(thrd_ResetEvt__waiting));
    }
    if (state == thrd_ResetEvt__waiting) {
        var deadline = thrd_ftx_Deadline_init(timeout);
        while (true) {
            let waiting = thrd_ftx_Deadline_wait(&deadline, &self->state, thrd_ResetEvt__waiting);
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != thrd_ResetEvt__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == thrd_ResetEvt__is_set);
    return_ok({});
} $unscoped(fn);

fn_((thrd_ResetEvt_Tok_wait(thrd_ResetEvt_Tok self))(void)) {
    thrd_ResetEvt_wait(self.event);
};

fn_((thrd_ResetEvt_Tok_timedWait(thrd_ResetEvt_Tok self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_ResetEvt_timedWait(self.event, timeout);
};

fn_((thrd_ResetEvt_Tok_isSet(thrd_ResetEvt_Tok self))(bool)) {
    return thrd_ResetEvt_isSet(self.event);
};

fn_((thrd_ResetEvt_Sig_set(thrd_ResetEvt_Sig self))(void)) {
    thrd_ResetEvt_set(self.event);
};

fn_((thrd_ResetEvt_Sig_reset(thrd_ResetEvt_Sig self))(void)) {
    thrd_ResetEvt_reset(self.event);
};
