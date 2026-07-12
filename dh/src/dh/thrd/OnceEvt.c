#include "dh/thrd/OnceEvt.h"
#include "dh/time/Dur.h"

#define thrd_OnceEvt__unset 0u
#define thrd_OnceEvt__waiting 1u
#define thrd_OnceEvt__is_set 2u
$attr($must_check)
$static fn_((thrd_OnceEvt__wait(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$static fn_((thrd_OnceEvt__waitUntilSet(thrd_OnceEvt* self, O$time_Dur timeout))(thrd_ftx_E$void));

$static fn_((thrd_OnceEvt__ready(P$raw ctx))(bool));
$static fn_((thrd_OnceEvt__link(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_OnceEvt__unlink(P$raw ctx, thrd_wait_Link* link))(void));
$static let_(thrd_OnceEvt__src_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_OnceEvt__ready,
    .linkFn = thrd_OnceEvt__link,
    .unlinkFn = thrd_OnceEvt__unlink,
};

fn_((thrd_OnceEvt_init(void))(thrd_OnceEvt)) {
    return (thrd_OnceEvt){
        .state = atom_V_init(thrd_OnceEvt__unset),
        .lock = thrd_Mtx_init(),
        .waiters = thrd_wait_List_init(),
    };
};
fn_((thrd_OnceEvt_fini(thrd_OnceEvt* self))(void)) {
    thrd_Mtx_fini(&self->lock);
    atom_V_store(&self->state, thrd_OnceEvt__unset, atom_MemOrd_monotonic);
};

fn_((thrd_OnceEvt_tok(thrd_OnceEvt* self))(thrd_OnceEvt_Tok)) {
    return (thrd_OnceEvt_Tok){ .event = self };
};
fn_((thrd_OnceEvt_sig(thrd_OnceEvt* self))(thrd_OnceEvt_Sig)) {
    return (thrd_OnceEvt_Sig){ .event = self };
};

fn_((thrd_OnceEvt_isSet(const thrd_OnceEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == thrd_OnceEvt__is_set;
};
fn_((thrd_OnceEvt_tryWait(thrd_OnceEvt* self))(bool)) {
    return thrd_OnceEvt_isSet(self);
};
fn_((thrd_OnceEvt_wait(thrd_OnceEvt* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void) $guard) {
    if (thrd_OnceEvt_isSet(self)) {
        return_ok({});
    }
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    var link = thrd_Waiter_link(&waiter, 0);
    let src = thrd_OnceEvt_waitSrc(self);
    if (src.vtbl->linkFn(src.ctx, &link)) {
        return_ok({});
    }
    defer_(src.vtbl->unlinkFn(src.ctx, &link));

    try_(thrd_Waiter_wait(&waiter, cancel_src));
    return_ok({});
} $unguarded(fn);
fn_((thrd_OnceEvt_waitFor(thrd_OnceEvt* self, thrd_wait_Src cancel_src, time_Dur timeout))(Sched_TimedE$void) $guard) {
    if (thrd_OnceEvt_isSet(self)) {
        return_ok({});
    }
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    var link = thrd_Waiter_link(&waiter, 0);
    let src = thrd_OnceEvt_waitSrc(self);
    if (src.vtbl->linkFn(src.ctx, &link)) {
        return_ok({});
    }
    defer_(src.vtbl->unlinkFn(src.ctx, &link));

    try_(thrd_Waiter_waitFor(&waiter, cancel_src, timeout));
    return_ok({});
} $unguarded(fn);
fn_((thrd_OnceEvt_waitProtcd(thrd_OnceEvt* self))(void) $scope) {
    return_void(catch_((thrd_OnceEvt__wait(self, none$((O$time_Dur))))($ignore, claim_unreachable)));
} $unscoped(fn);
fn_((thrd_OnceEvt_waitSrc(thrd_OnceEvt* self))(thrd_wait_Src)) {
    return (thrd_wait_Src){
        .ctx = self,
        .vtbl = &thrd_OnceEvt__src_vtbl,
    };
};
fn_((thrd_OnceEvt_set(thrd_OnceEvt* self))(void)) {
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_OnceEvt__is_set) {
        return;
    }
    thrd_Mtx_lockProtcd(&self->lock);
    let prev_state = atom_V_fetchXchg(&self->state, thrd_OnceEvt__is_set, atom_MemOrd_release);
    var waiters = self->waiters;
    self->waiters = thrd_wait_List_init();
    thrd_Mtx_unlock(&self->lock);
    if (prev_state == thrd_OnceEvt__waiting) {
        thrd_ftx_wake(&self->state, u32_limit_max);
    }
    thrd_wait_List_wakeAll(&waiters);
};

fn_((thrd_OnceEvt_Tok_isSet(thrd_OnceEvt_Tok self))(bool)) {
    return thrd_OnceEvt_isSet(self.event);
};
fn_((thrd_OnceEvt_Tok_tryWait(thrd_OnceEvt_Tok self))(bool)) {
    return thrd_OnceEvt_tryWait(self.event);
};
fn_((thrd_OnceEvt_Tok_wait(thrd_OnceEvt_Tok self, thrd_wait_Src cancel_src))(Sched_Cancelable$void)) {
    return thrd_OnceEvt_wait(self.event, cancel_src);
};
fn_((thrd_OnceEvt_Tok_waitFor(thrd_OnceEvt_Tok self, thrd_wait_Src cancel_src, time_Dur timeout))(Sched_TimedE$void)) {
    return thrd_OnceEvt_waitFor(self.event, cancel_src, timeout);
};
fn_((thrd_OnceEvt_Tok_waitProtcd(thrd_OnceEvt_Tok self))(void)) {
    thrd_OnceEvt_waitProtcd(self.event);
};
fn_((thrd_OnceEvt_Tok_waitSrc(thrd_OnceEvt_Tok self))(thrd_wait_Src)) {
    return thrd_OnceEvt_waitSrc(self.event);
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
            let waiting = thrd_ftx_Deadline_wait(&deadline, &self->state, thrd_OnceEvt__waiting, none$((O$thrd_wait_Src)));
            state = atom_V_load(&self->state, atom_MemOrd_acquire);
            if (state != thrd_OnceEvt__waiting) { break; }
            try_(waiting);
        }
    }
    claim_assert(state == thrd_OnceEvt__is_set);
    return_ok({});
} $unscoped(fn);

fn_((thrd_OnceEvt__ready(P$raw ctx))(bool)) {
    return thrd_OnceEvt_isSet(ptrAlignCast$((thrd_OnceEvt*)(ctx)));
};
fn_((thrd_OnceEvt__link(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_OnceEvt*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    if (thrd_OnceEvt_isSet(self)) {
        thrd_Mtx_unlock(&self->lock);
        return true;
    }
    thrd_wait_List_prepend(&self->waiters, link);
    thrd_Mtx_unlock(&self->lock);
    return false;
};
fn_((thrd_OnceEvt__unlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_OnceEvt*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_List_unlink(&self->waiters, link);
    thrd_Mtx_unlock(&self->lock);
};
