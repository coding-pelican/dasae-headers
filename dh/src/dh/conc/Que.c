#include "dh/conc/Que.h"
#include "dh/ArrQue.h"
#include "dh/Sched.h"
#include "dh/time/self/Awake.h"

$static fn_((conc_Que__grip(conc_Que$raw* self, TypeInfo type))(ArrQue_Grip));
$static fn_((conc_Que__wakeAll(O$P$conc_AwaitLink waiters))(void));
$static fn_((conc_Que__wakeOne(O$P$conc_AwaitLink* waiters))(void));
$static fn_((conc_Que__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void));
$static fn_((conc_Que__poll(P$raw ctx, u_P$raw out))(bool));
$static fn_((conc_Que__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool));
$static fn_((conc_Que__unlink(P$raw ctx, conc_AwaitLink* link))(void));
$static fn_((conc_Que__cancel(P$raw ctx, Sched sched))(void));

$static let_(conc_Que__recv_vtbl, conc_AwaitSrc_VTbl) = {
    .pollFn = conc_Que__poll,
    .linkFn = conc_Que__link,
    .unlinkFn = conc_Que__unlink,
    .cancelFn = conc_Que__cancel,
};

fn_((conc_Que_fixed(TypeInfo type, u_S$raw buf))(conc_Que$raw)) {
    return (conc_Que$raw){
        .buf = buf.raw,
        .head = 0,
        .len = 0,
        .closed = false,
        .lock = atom_SpinLock_init(),
        .send_waiters = none(),
        .recv_waiters = none(),
        .type = $typing(type),
    };
};

fn_((conc_Que_len(const conc_Que$raw* self))(usize) $guard) {
    atom_SpinLock_lock(ptrQualCast$((atom_SpinLock*)(&self->lock)));
    defer_(atom_SpinLock_unlock(ptrQualCast$((atom_SpinLock*)(&self->lock))));
    return_(self->len);
} $unguarded(fn);

fn_((conc_Que_cap(const conc_Que$raw* self))(usize)) {
    return self->buf.len;
};

fn_((conc_Que_close(conc_Que$raw* self))(void)) {
    var_(send_waiters, O$P$conc_AwaitLink) = none();
    var_(recv_waiters, O$P$conc_AwaitLink) = none();
    atom_SpinLock_lock(&self->lock);
    self->closed = true;
    send_waiters = self->send_waiters;
    recv_waiters = self->recv_waiters;
    self->send_waiters = none$((O$P$conc_AwaitLink));
    self->recv_waiters = none$((O$P$conc_AwaitLink));
    atom_SpinLock_unlock(&self->lock);
    conc_Que__wakeAll(send_waiters);
    conc_Que__wakeAll(recv_waiters);
};

fn_((conc_Que_isClosed(const conc_Que$raw* self))(bool) $guard) {
    atom_SpinLock_lock(ptrQualCast$((atom_SpinLock*)(&self->lock)));
    defer_(atom_SpinLock_unlock(ptrQualCast$((atom_SpinLock*)(&self->lock))));
    return_(self->closed);
} $unguarded(fn);

fn_((conc_Que_trySend(conc_Que$raw* self, TypeInfo type, u_V$raw item))(conc_chan_E$void) $guard) {
    let_ignore = type;
    atom_SpinLock_lock(&self->lock);
    defer_(atom_SpinLock_unlock(&self->lock));

    if (self->closed) {
        return_err(E_cause$conc_chan_Closed());
    }
    if (self->len == self->buf.len) {
        return_err(E_cause$conc_chan_Full());
    }
    var que = conc_Que__grip(self, type);
    ArrQue_enqueWithin(&que.ctx, item);
    ArrQue_Grip_release(&que, type);
    conc_Que__wakeOne(&self->recv_waiters);
    return_ok({});
} $unguarded(fn);

fn_((conc_Que_tryRecv(conc_Que$raw* self, TypeInfo type, u_V$raw ret_mem))(conc_chan_E$u_V$raw) $guard) {
    let_ignore = type;
    atom_SpinLock_lock(&self->lock);
    defer_(atom_SpinLock_unlock(&self->lock));

    if (self->len == 0) {
        if (self->closed) {
            return_err(E_cause$conc_chan_Closed());
        }
        return_err(E_cause$conc_chan_Empty());
    }
    var que = conc_Que__grip(self, type);
    let_ignore = unwrap_(ArrQue_deque(&que.ctx, ret_mem));
    ArrQue_Grip_release(&que, type);
    conc_Que__wakeOne(&self->send_waiters);
    return_ok(ret_mem);
} $unguarded(fn);

fn_((conc_Que_send(conc_Que$raw* self, TypeInfo type, Sched sched, u_V$raw item))(conc_chan_WaitE$void) $scope) {
    while (true) {
        catch_((conc_Que_trySend(self, type, item))(err, {
            if (E_eql(err.as_any, E_cause$conc_chan_Closed().as_any)) {
                return_err(E_cause$conc_chan_Closed());
            }
            try_(Sched_idle(sched));
            continue;
        }));
        return_ok({});
    }
} $unscoped(fn);

fn_((conc_Que_sendFor(
    conc_Que$raw* self,
    TypeInfo type,
    Sched sched,
    time_Dur dur,
    u_V$raw item
))(conc_chan_TimedE$void) $scope) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let now = time_Awake_now(clock);
    let deadline = orelse_((time_Inst_addChkdDur(now.raw, dur))(
        time_Inst_from(u64_limit_max, time_nanos_per_sec - 1)
    ));
    return_(conc_Que_sendUntil(self, type, sched, deadline, item));
} $unscoped(fn);

fn_((conc_Que_sendUntil(
    conc_Que$raw* self,
    TypeInfo type,
    Sched sched,
    time_Inst deadline,
    u_V$raw item
))(conc_chan_TimedE$void) $scope) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    while (true) {
        catch_((conc_Que_trySend(self, type, item))(err, {
            if (E_eql(err.as_any, E_cause$conc_chan_Closed().as_any)) {
                return_err(E_cause$conc_chan_Closed());
            }
            if (cmp_ge$(time_Inst)(time_Awake_now(clock).raw, deadline)) {
                return_err(E_cause$Sched_Timeout());
            }
            try_(Sched_idle(sched));
            continue;
        }));
        return_ok({});
    }
} $unscoped(fn);

fn_((conc_Que_recv(conc_Que$raw* self, TypeInfo type, Sched sched, u_V$raw ret_mem))(conc_chan_WaitE$u_V$raw) $scope) {
    while (true) {
        let value = catch_((conc_Que_tryRecv(self, type, ret_mem))(err, {
            if (E_eql(err.as_any, E_cause$conc_chan_Closed().as_any)) {
                return_err(E_cause$conc_chan_Closed());
            }
            try_(Sched_idle(sched));
            continue;
        }));
        return_ok(value);
    }
} $unscoped(fn);

fn_((conc_Que_recvFor(
    conc_Que$raw* self,
    TypeInfo type,
    Sched sched,
    time_Dur dur,
    u_V$raw ret_mem
))(conc_chan_TimedE$u_V$raw) $scope) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let now = time_Awake_now(clock);
    let deadline = orelse_((time_Inst_addChkdDur(now.raw, dur))(
        time_Inst_from(u64_limit_max, time_nanos_per_sec - 1)
    ));
    return_(conc_Que_recvUntil(self, type, sched, deadline, ret_mem));
} $unscoped(fn);

fn_((conc_Que_recvUntil(
    conc_Que$raw* self,
    TypeInfo type,
    Sched sched,
    time_Inst deadline,
    u_V$raw ret_mem
))(conc_chan_TimedE$u_V$raw) $scope) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    while (true) {
        let value = catch_((conc_Que_tryRecv(self, type, ret_mem))(err, {
            if (E_eql(err.as_any, E_cause$conc_chan_Closed().as_any)) {
                return_err(E_cause$conc_chan_Closed());
            }
            if (cmp_ge$(time_Inst)(time_Awake_now(clock).raw, deadline)) {
                return_err(E_cause$Sched_Timeout());
            }
            try_(Sched_idle(sched));
            continue;
        }));
        return_ok(value);
    }
} $unscoped(fn);

fn_((conc_Que_recvSrc(conc_Que$raw* self, TypeInfo type))(conc_AwaitSrc)) {
    let_ignore = type;
    return conc_AwaitSrc_init(self, &conc_Que__recv_vtbl);
};

fn_((conc_Que__grip(conc_Que$raw* self, TypeInfo type))(ArrQue_Grip)) {
    return ArrQue_grip(
        (u_S$raw){ .raw = self->buf, .type = type },
        &self->head,
        &self->len
    );
};

fn_((conc_Que__wakeAll(O$P$conc_AwaitLink waiters))(void)) {
    while (isSome(waiters)) {
        let link = unwrap_(waiters);
        waiters = link->next;
        let prev = atom_V_fetchXchg(&link->state, conc_AwaitLink_State_woken, atom_MemOrd_acq_rel);
        if (prev == conc_AwaitLink_State_linked) {
            link->wakeFn(link->wake_ctx);
        }
    }
};

fn_((conc_Que__wakeOne(O$P$conc_AwaitLink* waiters))(void)) {
    while (isSome(*waiters)) {
        let link = unwrap_(*waiters);
        *waiters = link->next;
        link->next = none$((O$P$conc_AwaitLink));
        let prev = atom_V_fetchXchg(&link->state, conc_AwaitLink_State_woken, atom_MemOrd_acq_rel);
        if (prev == conc_AwaitLink_State_linked) {
            link->wakeFn(link->wake_ctx);
            return;
        }
    }
};

fn_((conc_Que__unlinkWaiter(O$P$conc_AwaitLink* waiters, conc_AwaitLink* link))(void)) {
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

fn_((conc_Que__poll(P$raw ctx, u_P$raw out))(bool) $scope) {
    let self = ptrAlignCast$((conc_Que$raw*)(ctx));
    return isOk(conc_Que_tryRecv(self, out.type, (u_V$raw){ .inner = out.raw, .type = out.type }));
} $unscoped(fn);

fn_((conc_Que__link(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool)) {
    let_ignore = sched;
    let self = ptrAlignCast$((conc_Que$raw*)(ctx));
    atom_SpinLock_lock(&self->lock);
    if (self->len != 0 || self->closed) {
        atom_SpinLock_unlock(&self->lock);
        return true;
    }
    claim_assert(atom_V_load(&link->state, atom_MemOrd_acquire) == conc_AwaitLink_State_idle);
    atom_V_store(&link->state, conc_AwaitLink_State_linked, atom_MemOrd_release);
    link->next = self->recv_waiters;
    self->recv_waiters = some$((O$P$conc_AwaitLink)(link));
    atom_SpinLock_unlock(&self->lock);
    return false;
};

fn_((conc_Que__unlink(P$raw ctx, conc_AwaitLink* link))(void)) {
    let self = ptrAlignCast$((conc_Que$raw*)(ctx));
    atom_SpinLock_lock(&self->lock);
    conc_Que__unlinkWaiter(&self->recv_waiters, link);
    atom_SpinLock_unlock(&self->lock);
};

fn_((conc_Que__cancel(P$raw ctx, Sched sched))(void)) {
    let_ignore = ctx;
    let_ignore = sched;
};
