#include "dh/thrd/ResetEvt.h"
#include "dh/time/Dur.h"

#define thrd_ResetEvt__unset 0u
#define thrd_ResetEvt__waiting 1u
#define thrd_ResetEvt__is_set 2u
$attr($must_check)
$static fn_((thrd_ResetEvt__wait(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$static fn_((thrd_ResetEvt__waitUntilSet(thrd_ResetEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));

$static fn_((thrd_ResetEvt__ready(P$raw ctx))(bool));
$static fn_((thrd_ResetEvt__link(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_ResetEvt__unlink(P$raw ctx, thrd_wait_Link* link))(void));
$static let_(thrd_ResetEvt__src_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_ResetEvt__ready,
    .linkFn = thrd_ResetEvt__link,
    .unlinkFn = thrd_ResetEvt__unlink,
};

fn_((thrd_ResetEvt_init(void))(thrd_ResetEvt)) {
    return (thrd_ResetEvt){
        .state = atom_V_init(thrd_ResetEvt__unset),
        .lock = thrd_Mtx_init(),
        .waiters = thrd_wait_List_init(),
    };
};
fn_((thrd_ResetEvt_fini(thrd_ResetEvt* self))(void)) {
    thrd_Mtx_fini(&self->lock);
    atom_V_store(&self->state, thrd_ResetEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvt_tok(thrd_ResetEvt* self))(thrd_ResetEvt_Tok)) {
    return (thrd_ResetEvt_Tok){ .event = self };
};
fn_((thrd_ResetEvt_sig(thrd_ResetEvt* self))(thrd_ResetEvt_Sig)) {
    return (thrd_ResetEvt_Sig){ .event = self };
};

fn_((thrd_ResetEvt_isSet(const thrd_ResetEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_ResetEvt__is_set;
};
fn_((thrd_ResetEvt_tryWait(thrd_ResetEvt* self))(bool)) {
    return thrd_ResetEvt_isSet(self);
};
fn_((thrd_ResetEvt_wait(thrd_ResetEvt* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void) $guard) {
    if (thrd_ResetEvt_isSet(self)) {
        return_ok({});
    }
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    var link = thrd_Waiter_link(&waiter, 0);
    let src = thrd_ResetEvt_waitSrc(self);
    if (src.vtbl->linkFn(src.ctx, &link)) {
        return_ok({});
    }
    defer_(src.vtbl->unlinkFn(src.ctx, &link));

    try_(thrd_Waiter_wait(&waiter, cancel_src));
    return_ok({});
} $unguarded(fn);
fn_((thrd_ResetEvt_waitFor(thrd_ResetEvt* self, thrd_wait_Src cancel_src, time_Dur timeout))(Sched_TimedE$void) $guard) {
    if (thrd_ResetEvt_isSet(self)) {
        return_ok({});
    }
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    var link = thrd_Waiter_link(&waiter, 0);
    let src = thrd_ResetEvt_waitSrc(self);
    if (src.vtbl->linkFn(src.ctx, &link)) {
        return_ok({});
    }
    defer_(src.vtbl->unlinkFn(src.ctx, &link));

    try_(thrd_Waiter_waitFor(&waiter, cancel_src, timeout));
    return_ok({});
} $unguarded(fn);
fn_((thrd_ResetEvt_waitProtcd(thrd_ResetEvt* self))(void) $scope) {
    return_void(catch_((thrd_ResetEvt__wait(self, none$((O$time_Dur))))($ignore, claim_unreachable)));
} $unscoped(fn);
fn_((thrd_ResetEvt_waitSrc(thrd_ResetEvt* self))(thrd_wait_Src)) {
    return (thrd_wait_Src){
        .ctx = self,
        .vtbl = &thrd_ResetEvt__src_vtbl,
    };
};
fn_((thrd_ResetEvt_set(thrd_ResetEvt* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_ResetEvt__is_set) {
        return;
    }
    thrd_Mtx_lockProtcd(&self->lock);
    let prev_state = atom_V_fetchXchg(&self->state, thrd_ResetEvt__is_set, atom_MemOrd_release);
    var waiters = self->waiters;
    self->waiters = thrd_wait_List_init();
    thrd_Mtx_unlock(&self->lock);
    if (prev_state == thrd_ResetEvt__waiting) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
    thrd_wait_List_wakeAll(&waiters);
};
fn_((thrd_ResetEvt_reset(thrd_ResetEvt* self))(void)) {
    atom_V_store(&self->state, thrd_ResetEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_ResetEvt_Tok_isSet(thrd_ResetEvt_Tok self))(bool)) {
    return thrd_ResetEvt_isSet(self.event);
};
fn_((thrd_ResetEvt_Tok_tryWait(thrd_ResetEvt_Tok self))(bool)) {
    return thrd_ResetEvt_tryWait(self.event);
};
fn_((thrd_ResetEvt_Tok_wait(thrd_ResetEvt_Tok self, thrd_wait_Src cancel_src))(Sched_Cancelable$void)) {
    return thrd_ResetEvt_wait(self.event, cancel_src);
};
fn_((thrd_ResetEvt_Tok_waitFor(thrd_ResetEvt_Tok self, thrd_wait_Src cancel_src, time_Dur timeout))(Sched_TimedE$void)) {
    return thrd_ResetEvt_waitFor(self.event, cancel_src, timeout);
};
fn_((thrd_ResetEvt_Tok_waitProtcd(thrd_ResetEvt_Tok self))(void)) {
    thrd_ResetEvt_waitProtcd(self.event);
};
fn_((thrd_ResetEvt_Tok_waitSrc(thrd_ResetEvt_Tok self))(thrd_wait_Src)) {
    return thrd_ResetEvt_waitSrc(self.event);
};

fn_((thrd_ResetEvt_Sig_set(thrd_ResetEvt_Sig self))(void)) {
    thrd_ResetEvt_set(self.event);
};
fn_((thrd_ResetEvt_Sig_reset(thrd_ResetEvt_Sig self))(void)) {
    thrd_ResetEvt_reset(self.event);
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
            let waiting = thrd_ftx_Deadline_wait(&deadline, &self->state, thrd_ResetEvt__waiting, none$((O$thrd_wait_Src)));
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != thrd_ResetEvt__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == thrd_ResetEvt__is_set);
    return_ok({});
} $unscoped(fn);

fn_((thrd_ResetEvt__ready(P$raw ctx))(bool)) {
    return thrd_ResetEvt_isSet(ptrAlignCast$((thrd_ResetEvt*)(ctx)));
};
fn_((thrd_ResetEvt__link(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_ResetEvt*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    if (thrd_ResetEvt_isSet(self)) {
        thrd_Mtx_unlock(&self->lock);
        return true;
    }
    thrd_wait_List_prepend(&self->waiters, link);
    thrd_Mtx_unlock(&self->lock);
    return false;
};
fn_((thrd_ResetEvt__unlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_ResetEvt*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_List_unlink(&self->waiters, link);
    thrd_Mtx_unlock(&self->lock);
};
