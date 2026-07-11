#include "dh/conc/OnceEvt.h"
#include "dh/Sched.h"
#include "dh/time/self/Awake.h"

$static fn_((conc_OnceEvt__wakeAll(O$P$conc_AwaitLink waiters))(void));
$static fn_((conc_OnceEvt__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void));
$static fn_((conc_OnceEvt__poll(P$raw ctx, u_P$raw out))(bool));
$static fn_((conc_OnceEvt__isSetRaw(P$raw ctx))(bool));
$static fn_((conc_OnceEvt__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool));
$static fn_((conc_OnceEvt__unlink(P$raw ctx, conc_AwaitLink* link))(void));
$static fn_((conc_OnceEvt__cancel(P$raw ctx, Sched sched))(void));
$static fn_((conc_OnceEvt__waitUntil(
    fn_(((*isSetFn)(P$raw ctx))(bool)),
    P$raw ctx,
    Sched sched,
    O$time_Inst deadline
))(Sched_TimedE$void));

$static let_(conc_OnceEvt__vtbl, conc_AwaitSrc_VTbl) = {
    .pollFn = conc_OnceEvt__poll,
    .linkFn = conc_OnceEvt__link,
    .unlinkFn = conc_OnceEvt__unlink,
    .cancelFn = conc_OnceEvt__cancel,
};

fn_((conc_OnceEvt_init(void))(conc_OnceEvt)) {
    return conc_OnceEvt_init_static();
};

fn_((conc_OnceEvt_tok(conc_OnceEvt* self))(conc_OnceEvt_Tok)) {
    return (conc_OnceEvt_Tok){ .evt = self };
};

fn_((conc_OnceEvt_sig(conc_OnceEvt* self))(conc_OnceEvt_Sig)) {
    return (conc_OnceEvt_Sig){ .evt = self };
};

fn_((conc_OnceEvt_isSet(const conc_OnceEvt* self))(bool)) {
    return atom_V_load(&self->state, atom_MemOrd_acquire) == conc_OnceEvt_State_set;
};

fn_((conc_OnceEvt_set(conc_OnceEvt* self))(void)) {
    var_(waiters, O$P$conc_AwaitLink) = none();
    atom_SpinLock_lock(&self->lock);
    atom_V_store(&self->state, conc_OnceEvt_State_set, atom_MemOrd_release);
    waiters = self->waiters;
    self->waiters = none$((O$P$conc_AwaitLink));
    atom_SpinLock_unlock(&self->lock);
    conc_OnceEvt__wakeAll(waiters);
};

fn_((conc_OnceEvt_wait(conc_OnceEvt* self, Sched sched))(Sched_Cancelable$void) $scope) {
    try_(conc_OnceEvt__waitUntil(conc_OnceEvt__isSetRaw, self, sched, none$((O$time_Inst))));
    return_ok({});
} $unscoped(fn);

fn_((conc_OnceEvt_waitFor(conc_OnceEvt* self, Sched sched, time_Dur dur))(Sched_TimedE$void) $scope) {
    if (conc_OnceEvt_isSet(self)) return_ok({});
    if (time_Dur_isZero(dur)) return_err(E_cause$Sched_Timeout());
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let now = time_Awake_now(clock);
    let deadline = orelse_((time_Inst_addChkdDur(now.raw, dur))(
        time_Inst_from(u64_limit_max, time_nanos_per_sec - 1)
    ));
    return_(conc_OnceEvt__waitUntil(conc_OnceEvt__isSetRaw, self, sched, some$((O$time_Inst)(deadline))));
} $unscoped(fn);

fn_((conc_OnceEvt_waitUntil(conc_OnceEvt* self, Sched sched, time_Inst deadline))(Sched_TimedE$void)) {
    return conc_OnceEvt__waitUntil(conc_OnceEvt__isSetRaw, self, sched, some$((O$time_Inst)(deadline)));
};

fn_((conc_OnceEvt_Tok_isSet(conc_OnceEvt_Tok self))(bool)) {
    return conc_OnceEvt_isSet(self.evt);
};

fn_((conc_OnceEvt_Tok_wait(conc_OnceEvt_Tok self, Sched sched))(Sched_Cancelable$void)) {
    return conc_OnceEvt_wait(self.evt, sched);
};

fn_((conc_OnceEvt_Tok_waitFor(conc_OnceEvt_Tok self, Sched sched, time_Dur dur))(Sched_TimedE$void)) {
    return conc_OnceEvt_waitFor(self.evt, sched, dur);
};

fn_((conc_OnceEvt_Tok_waitUntil(conc_OnceEvt_Tok self, Sched sched, time_Inst deadline))(Sched_TimedE$void)) {
    return conc_OnceEvt_waitUntil(self.evt, sched, deadline);
};

fn_((conc_OnceEvt_Sig_set(conc_OnceEvt_Sig self))(void)) {
    return conc_OnceEvt_set(self.evt);
};

fn_((conc_OnceEvt_asAwaitSrc(conc_OnceEvt* self))(conc_AwaitSrc)) {
    return conc_AwaitSrc_init(self, &conc_OnceEvt__vtbl);
};

fn_((conc_OnceEvt__wakeAll(O$P$conc_AwaitLink waiters))(void)) {
    while (isSome(waiters)) {
        let link = unwrap_(waiters);
        waiters = link->next;
        let prev = atom_V_fetchXchg(&link->state, conc_AwaitLink_State_woken, atom_MemOrd_acq_rel);
        if (prev == conc_AwaitLink_State_linked) {
            link->wakeFn(link->wake_ctx);
        }
    }
};

fn_((conc_OnceEvt__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void)) {
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

fn_((conc_OnceEvt__poll(P$raw ctx, u_P$raw out))(bool)) {
    let_ignore = out;
    return conc_OnceEvt_isSet(ptrAlignCast$((conc_OnceEvt*)(ctx)));
};

fn_((conc_OnceEvt__isSetRaw(P$raw ctx))(bool)) {
    return conc_OnceEvt_isSet(ptrAlignCast$((conc_OnceEvt*)(ctx)));
};

fn_((conc_OnceEvt__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool)) {
    let_ignore = sched;
    let self = ptrAlignCast$((conc_OnceEvt*)(ctx));
    atom_SpinLock_lock(&self->lock);
    if (conc_OnceEvt_isSet(self)) {
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

fn_((conc_OnceEvt__unlink(P$raw ctx, conc_AwaitLink* link))(void)) {
    let self = ptrAlignCast$((conc_OnceEvt*)(ctx));
    atom_SpinLock_lock(&self->lock);
    conc_OnceEvt__unlinkWaiter(&self->waiters, link);
    atom_SpinLock_unlock(&self->lock);
};

fn_((conc_OnceEvt__cancel(P$raw ctx, Sched sched))(void)) {
    let_ignore = ctx;
    let_ignore = sched;
};

fn_((conc_OnceEvt__waitUntil(
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
