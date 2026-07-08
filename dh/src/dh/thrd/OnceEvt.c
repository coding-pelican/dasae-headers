#include "dh/thrd/OnceEvt.h"
#include "dh/time/Dur.h"

#define thrd_OnceEvt__unset 0u
#define thrd_OnceEvt__waiting 1u
#define thrd_OnceEvt__is_set 2u

$attr($must_check)
$static fn_((thrd_OnceEvt__wait(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$static fn_((thrd_OnceEvt__waitUntilSet(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));

fn_((thrd_OnceEvt_init(void))(thrd_OnceEvt)) {
    return (thrd_OnceEvt){
        .state = atom_V_init(thrd_OnceEvt__unset),
    };
};

fn_((thrd_OnceEvt_fini(thrd_OnceEvt* self))(void)) {
    atom_V_store(&self->state, thrd_OnceEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_OnceEvt_tok(thrd_OnceEvt* self))(thrd_OnceEvt_Tok)) {
    return (thrd_OnceEvt_Tok){ .event = self };
};

fn_((thrd_OnceEvt_sig(thrd_OnceEvt* self))(thrd_OnceEvt_Sig)) {
    return (thrd_OnceEvt_Sig){ .event = self };
};

fn_((thrd_OnceEvt_wait(thrd_OnceEvt* self))(void) $scope) {
    return_void(catch_((thrd_OnceEvt__wait(self, none$((O$time_Dur))))($ignore, claim_unreachable)));
} $unscoped(fn);

fn_((thrd_OnceEvt_timedWait(thrd_OnceEvt* self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_OnceEvt__wait(self, some$((O$time_Dur)(timeout)));
};

fn_((thrd_OnceEvt_set(thrd_OnceEvt* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_OnceEvt__is_set) {
        return;
    }
    if (atom_V_fetchXchg(&self->state, thrd_OnceEvt__is_set, atom_MemOrd_release) == thrd_OnceEvt__waiting) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
};

fn_((thrd_OnceEvt_isSet(const thrd_OnceEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_OnceEvt__is_set;
};

fn_((thrd_OnceEvt_Tok_wait(thrd_OnceEvt_Tok self))(void)) {
    thrd_OnceEvt_wait(self.event);
};

fn_((thrd_OnceEvt_Tok_timedWait(thrd_OnceEvt_Tok self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_OnceEvt_timedWait(self.event, timeout);
};

fn_((thrd_OnceEvt_Tok_isSet(thrd_OnceEvt_Tok self))(bool)) {
    return thrd_OnceEvt_isSet(self.event);
};

fn_((thrd_OnceEvt_Sig_set(thrd_OnceEvt_Sig self))(void)) {
    thrd_OnceEvt_set(self.event);
};

fn_((thrd_OnceEvt__wait(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void)) {
    return expr_(thrd_ftx_E$void $scope)(if (!thrd_OnceEvt_isSet(self)) {
        $break_(thrd_OnceEvt__waitUntilSet(self, timeout));
    } else {
        $break_(ok({}));
    }) $unscoped(expr);
};

$attr($branch_cold)
fn_((thrd_OnceEvt__waitUntilSet(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    var state = atom_V_load(&self->state, atom_MemOrd_acquire);
    if (state == thrd_OnceEvt__unset) {
        state = orelse_((atom_V_cmpXchgStrong$(
            O$u32, &self->state, state, thrd_OnceEvt__waiting, atom_MemOrd_acquire, atom_MemOrd_acquire
        ))(thrd_OnceEvt__waiting));
    }
    if (state == thrd_OnceEvt__waiting) {
        var deadline = thrd_ftx_Deadline_init(timeout);
        while (true) {
            let waiting = thrd_ftx_Deadline_wait(&deadline, &self->state, thrd_OnceEvt__waiting);
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != thrd_OnceEvt__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == thrd_OnceEvt__is_set);
    return_ok({});
} $unscoped(fn);
