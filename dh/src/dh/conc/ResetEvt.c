#include "dh/conc/ResetEvt.h"
#include "dh/Sched.h"
#include "dh/time/self/Awake.h"

$static fn_((conc_ResetEvt__wakeAll(O$P$conc_AwaitLink waiters))(void));
$static fn_((conc_ResetEvt__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void));
$static fn_((conc_ResetEvt__poll(P$raw ctx, u_P$raw out))(bool));
$static fn_((conc_ResetEvt__isSetRaw(P$raw ctx))(bool));
$static fn_((conc_ResetEvt__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool));
$static fn_((conc_ResetEvt__unlink(P$raw ctx, conc_AwaitLink* link))(void));
$static fn_((conc_ResetEvt__cancel(P$raw ctx, Sched sched))(void));
$static fn_((conc_ResetEvt__waitUntil(
    fn_(((*isSetFn)(P$raw ctx))(bool)),
    P$raw ctx,
    Sched sched,
    O$time_Inst deadline
))(Sched_TimedE$void));

$static let_(conc_ResetEvt__vtbl, conc_Awakeable_VTbl) = {
    .pollFn = conc_ResetEvt__poll,
    .linkFn = conc_ResetEvt__link,
    .unlinkFn = conc_ResetEvt__unlink,
    .cancelFn = conc_ResetEvt__cancel,
};

fn_((conc_ResetEvt_init(void))(conc_ResetEvt)) {
    return conc_ResetEvt_init_static();
};

fn_((conc_ResetEvt_tok(conc_ResetEvt* self))(conc_ResetEvt_Tok)) {
    return (conc_ResetEvt_Tok){ .evt = self };
};

fn_((conc_ResetEvt_sig(conc_ResetEvt* self))(conc_ResetEvt_Sig)) {
    return (conc_ResetEvt_Sig){ .evt = self };
};

fn_((conc_ResetEvt_isSet(const conc_ResetEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == conc_ResetEvt_State_set;
};

fn_((conc_ResetEvt_set(conc_ResetEvt* self))(void)) {
    var_(waiters, O$P$conc_AwaitLink) = none();
    atom_SpinLock_lock(&self->lock);
    atom_V_store(&self->state, conc_ResetEvt_State_set, atom_MemOrd_release);
    waiters = self->waiters;
    self->waiters = none$((O$P$conc_AwaitLink));
    atom_SpinLock_unlock(&self->lock);
    conc_ResetEvt__wakeAll(waiters);
};

fn_((conc_ResetEvt_reset(conc_ResetEvt* self))(void)) {
    atom_SpinLock_lock(&self->lock);
    atom_V_store(&self->state, conc_ResetEvt_State_unset, atom_MemOrd_release);
    atom_SpinLock_unlock(&self->lock);
};

fn_((conc_ResetEvt_wait(conc_ResetEvt* self, Sched sched))(Sched_Cancelable$void) $scope) {
    try_(conc_ResetEvt__waitUntil(conc_ResetEvt__isSetRaw, self, sched, none$((O$time_Inst))));
    return_ok({});
} $unscoped(fn);

fn_((conc_ResetEvt_waitFor(conc_ResetEvt* self, Sched sched, time_Dur dur))(Sched_TimedE$void) $scope) {
    if (conc_ResetEvt_isSet(self)) return_ok({});
    if (time_Dur_isZero(dur)) return_err(E_cause$Sched_Timeout());
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let now = time_Awake_now(clock);
    let deadline = orelse_((time_Inst_addChkdDur(now.raw, dur))(
        time_Inst_from(u64_limit_max, time_nanos_per_sec - 1)
    ));
    return_(conc_ResetEvt__waitUntil(conc_ResetEvt__isSetRaw, self, sched, some$((O$time_Inst)(deadline))));
} $unscoped(fn);

fn_((conc_ResetEvt_waitUntil(conc_ResetEvt* self, Sched sched, time_Inst deadline))(Sched_TimedE$void)) {
    return conc_ResetEvt__waitUntil(conc_ResetEvt__isSetRaw, self, sched, some$((O$time_Inst)(deadline)));
};

fn_((conc_ResetEvt_Tok_isSet(conc_ResetEvt_Tok self))(bool)) {
    return conc_ResetEvt_isSet(self.evt);
};

fn_((conc_ResetEvt_Tok_wait(conc_ResetEvt_Tok self, Sched sched))(Sched_Cancelable$void)) {
    return conc_ResetEvt_wait(self.evt, sched);
};

fn_((conc_ResetEvt_Tok_waitFor(conc_ResetEvt_Tok self, Sched sched, time_Dur dur))(Sched_TimedE$void)) {
    return conc_ResetEvt_waitFor(self.evt, sched, dur);
};

fn_((conc_ResetEvt_Tok_waitUntil(conc_ResetEvt_Tok self, Sched sched, time_Inst deadline))(Sched_TimedE$void)) {
    return conc_ResetEvt_waitUntil(self.evt, sched, deadline);
};

fn_((conc_ResetEvt_Sig_set(conc_ResetEvt_Sig self))(void)) {
    return conc_ResetEvt_set(self.evt);
};

fn_((conc_ResetEvt_Sig_reset(conc_ResetEvt_Sig self))(void)) {
    return conc_ResetEvt_reset(self.evt);
};

fn_((conc_ResetEvt_asAwakeable(conc_ResetEvt* self))(conc_Awakeable)) {
    return conc_Awakeable_init(self, &conc_ResetEvt__vtbl);
};

fn_((conc_ResetEvt__wakeAll(O$P$conc_AwaitLink waiters))(void)) {
    while (isSome(waiters)) {
        let link = unwrap_(waiters);
        waiters = link->next;
        let prev = atom_V_fetchXchg(&link->state, conc_AwaitLink_State_woken, atom_MemOrd_acq_rel);
        if (prev == conc_AwaitLink_State_linked) {
            link->wakeFn(link->wake_ctx);
        }
    }
};

fn_((conc_ResetEvt__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void)) {
    let prev = atom_V_fetchXchg(&link->state, conc_AwaitLink_State_unlinked, atom_MemOrd_acq_rel);
    if (prev != conc_AwaitLink_State_linked) {
        return;
    }
    var cursor = waiters;
    while (isSome(*cursor)) {
        let curr = unwrap_(*cursor);
        if (curr == link) {
            *cursor = curr->next;
            curr->next = none$((O$P$conc_AwaitLink));
            return;
        }
        cursor = &curr->next;
    }
};

fn_((conc_ResetEvt__poll(P$raw ctx, u_P$raw out))(bool)) {
    let_ignore = out;
    return conc_ResetEvt_isSet(ptrAlignCast$((conc_ResetEvt*)(ctx)));
};

fn_((conc_ResetEvt__isSetRaw(P$raw ctx))(bool)) {
    return conc_ResetEvt_isSet(ptrAlignCast$((conc_ResetEvt*)(ctx)));
};

fn_((conc_ResetEvt__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool)) {
    let_ignore = sched;
    let self = ptrAlignCast$((conc_ResetEvt*)(ctx));
    atom_SpinLock_lock(&self->lock);
    if (conc_ResetEvt_isSet(self)) {
        atom_SpinLock_unlock(&self->lock);
        return true;
    }
    claim_assert(atom_V_load(&link->state, atom_MemOrd_acquire) == conc_AwaitLink_State_idle);
    atom_V_store(&link->state, conc_AwaitLink_State_linked, atom_MemOrd_release);
    link->next = self->waiters;
    self->waiters = some$((O$P$conc_AwaitLink)(link));
    atom_SpinLock_unlock(&self->lock);
    return false;
};

fn_((conc_ResetEvt__unlink(P$raw ctx, conc_AwaitLink* link))(void)) {
    let self = ptrAlignCast$((conc_ResetEvt*)(ctx));
    atom_SpinLock_lock(&self->lock);
    conc_ResetEvt__unlinkWaiter(&self->waiters, link);
    atom_SpinLock_unlock(&self->lock);
};

fn_((conc_ResetEvt__cancel(P$raw ctx, Sched sched))(void)) {
    let_ignore = ctx;
    let_ignore = sched;
};

fn_((conc_ResetEvt__waitUntil(
    fn_(((*isSetFn)(P$raw ctx))(bool)),
    P$raw ctx,
    Sched sched,
    O$time_Inst deadline
))(Sched_TimedE$void) $scope) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    while (!isSetFn(ctx)) {
        if_some((deadline)(limit)) {
            if (cmp_ge$(time_Inst)(time_Awake_now(clock).raw, limit)) {
                return_err(E_cause$Sched_Timeout());
            }
        }
        try_(Sched_idle(sched));
    }
    return_ok({});
} $unscoped(fn);
