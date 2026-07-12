#include "dh/thrd/Que.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_Que__sendReady(P$raw ctx))(bool));
$static fn_((thrd_Que__sendLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_Que__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_Que__recvReady(P$raw ctx))(bool));
$static fn_((thrd_Que__recvLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_Que__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_Que__sendOpPoll(P$raw ctx, u_P$raw result))(bool));
$static fn_((thrd_Que__recvOpPoll(P$raw ctx, u_P$raw result))(bool));

$static let_(thrd_Que__send_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_Que__sendReady,
    .linkFn = thrd_Que__sendLink,
    .unlinkFn = thrd_Que__sendUnlink,
};

$static let_(thrd_Que__recv_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_Que__recvReady,
    .linkFn = thrd_Que__recvLink,
    .unlinkFn = thrd_Que__recvUnlink,
};

$static let_(thrd_Que__send_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_Que__sendOpPoll,
};

$static let_(thrd_Que__recv_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_Que__recvOpPoll,
};

fn_((thrd_Que_init(u_S$raw buf))(thrd_Que)) {
    return (thrd_Que){
        .que = ArrQue_fixed(buf),
        .closed = false,
        .lock = thrd_Mtx_init(),
        .send_waiters = thrd_wait_List_init(),
        .recv_waiters = thrd_wait_List_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_Que_fini(thrd_Que* self))(void) $guard) {
    claim_assert_nonnull(self);
    thrd_Mtx_lockProtcd(&self->lock);
    self->closed = true;
    ArrQue_clearRetainingCap(&self->que);
    thrd_wait_List_wakeAll(&self->send_waiters);
    thrd_wait_List_wakeAll(&self->recv_waiters);
    thrd_Mtx_unlock(&self->lock);
    thrd_Mtx_fini(&self->lock);
} $unguarded(fn);

fn_((thrd_Que_len(thrd_Que* self))(usize) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_len(self->que));
} $unguarded(fn);

fn_((thrd_Que_cap(thrd_Que* self))(usize) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_cap(self->que));
} $unguarded(fn);

fn_((thrd_Que_isFull(thrd_Que* self))(bool) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_isFull(self->que));
} $unguarded(fn);

fn_((thrd_Que_isEmpty(thrd_Que* self))(bool) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(ArrQue_isEmpty(self->que));
} $unguarded(fn);

fn_((thrd_Que_isOpen(thrd_Que* self))(bool) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(!self->closed);
} $unguarded(fn);

fn_((thrd_Que_close(thrd_Que* self))(void) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    self->closed = true;
    thrd_wait_List_wakeAll(&self->send_waiters);
    thrd_wait_List_wakeAll(&self->recv_waiters);
} $unguarded(fn);

fn_((thrd_Que_isClosed(thrd_Que* self))(bool) $guard) {
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));
    return_(self->closed);
} $unguarded(fn);

fn_((thrd_Que_send(thrd_Que* self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void) $guard) {
    debug_assert_eqBy($typed(self->type), item.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        thrd_Mtx_lockProtcd(&self->lock);
        if (self->closed) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$thrd_chan_Closed());
        }
        if (!ArrQue_isFull(self->que)) {
            ArrQue_enqueWithin(&self->que, item);
            thrd_wait_List_wakeOne(&self->recv_waiters);
            thrd_Mtx_unlock(&self->lock);
            return_ok({});
        }
        thrd_wait_List_prepend(&self->send_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
        catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->send_waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
        thrd_Mtx_lockProtcd(&self->lock);
        thrd_wait_List_unlink(&self->send_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);

fn_((thrd_Que_trySend(thrd_Que* self, u_V$raw item))(thrd_chan_E$void) $guard) {
    debug_assert_eqBy($typed(self->type), item.inner_type, TypeInfo_eql);
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    if (self->closed) {
        return_err(E_cause$thrd_chan_Closed());
    }
    if (ArrQue_isFull(self->que)) {
        return_err(E_cause$thrd_chan_Full());
    }
    ArrQue_enqueWithin(&self->que, item);
    thrd_wait_List_wakeOne(&self->recv_waiters);
    return_ok({});
} $unguarded(fn);

fn_((thrd_Que_sendFor(thrd_Que* self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    debug_assert_eqBy($typed(self->type), item.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        thrd_Mtx_lockProtcd(&self->lock);
        if (self->closed) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$thrd_chan_Closed());
        }
        if (!ArrQue_isFull(self->que)) {
            ArrQue_enqueWithin(&self->que, item);
            thrd_wait_List_wakeOne(&self->recv_waiters);
            thrd_Mtx_unlock(&self->lock);
            return_ok({});
        }
        let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(remaining)) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$Sched_Timeout());
        }
        thrd_wait_List_prepend(&self->send_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
        catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->send_waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
        thrd_Mtx_lockProtcd(&self->lock);
        thrd_wait_List_unlink(&self->send_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);

fn_((thrd_Que_recv(thrd_Que* self, thrd_wait_Src cancel_src, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self->type), out.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        thrd_Mtx_lockProtcd(&self->lock);
        if (!ArrQue_isEmpty(self->que)) {
            let_ignore = unwrap_(ArrQue_deque(&self->que, out));
            thrd_wait_List_wakeOne(&self->send_waiters);
            thrd_Mtx_unlock(&self->lock);
            return_ok(out);
        }
        if (self->closed) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$thrd_chan_Closed());
        }
        thrd_wait_List_prepend(&self->recv_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
        catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->recv_waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
        thrd_Mtx_lockProtcd(&self->lock);
        thrd_wait_List_unlink(&self->recv_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);

fn_((thrd_Que_recvOp(thrd_Que* self, TypeInfo type))(thrd_Que_RecvOp)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return (thrd_Que_RecvOp){ .que = self, .type = $typing(type) };
};

fn_((thrd_Que_RecvOp_op(thrd_Que_RecvOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return (thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_Que__recv_op_vtbl,
        .src = { .ctx = self->que, .vtbl = &thrd_Que__recv_vtbl },
    };
};

fn_((thrd_Que_sendOp(thrd_Que* self, u_P$raw item))(thrd_Que_SendOp)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), item.type, TypeInfo_eql);
    return (thrd_Que_SendOp){ .que = self, .item = item };
};

fn_((thrd_Que_SendOp_op(thrd_Que_SendOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return (thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_Que__send_op_vtbl,
        .src = { .ctx = self->que, .vtbl = &thrd_Que__send_vtbl },
    };
};

fn_((thrd_Que__recvOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_Que_RecvOp*)(ctx));
    debug_assert_eqBy($typed(self->type), result.type, TypeInfo_eql);
    return isOk(thrd_Que_tryRecv(self->que, (u_V$raw){ .inner = result.raw, .type = result.type }));
} $unscoped(fn);

fn_((thrd_Que__sendOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_Que_SendOp*)(ctx));
    let_ignore = result;
    return isOk(thrd_Que_trySend(self->que, (u_V$raw){ .inner = self->item.raw, .type = self->item.type }));
} $unscoped(fn);

fn_((thrd_Que__recvReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    return !thrd_Que_isEmpty(self) || thrd_Que_isClosed(self);
};

fn_((thrd_Que__sendReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    return !thrd_Que_isFull(self) || thrd_Que_isClosed(self);
};

fn_((thrd_Que__sendLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    if (!ArrQue_isFull(self->que) || self->closed) {
        thrd_Mtx_unlock(&self->lock);
        return true;
    }
    thrd_wait_List_prepend(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->lock);
    return false;
};

fn_((thrd_Que__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_List_unlink(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->lock);
};

fn_((thrd_Que__recvLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    if (!ArrQue_isEmpty(self->que) || self->closed) {
        thrd_Mtx_unlock(&self->lock);
        return true;
    }
    thrd_wait_List_prepend(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->lock);
    return false;
};

fn_((thrd_Que__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_Que*)(ctx));
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_List_unlink(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->lock);
};

fn_((thrd_Que_tryRecv(thrd_Que* self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self->type), out.inner_type, TypeInfo_eql);
    thrd_Mtx_lockProtcd(&self->lock);
    defer_(thrd_Mtx_unlock(&self->lock));

    if (ArrQue_isEmpty(self->que)) {
        if (self->closed) {
            return_err(E_cause$thrd_chan_Closed());
        }
        return_err(E_cause$thrd_chan_Empty());
    }
    let_ignore = unwrap_(ArrQue_deque(&self->que, out));
    thrd_wait_List_wakeOne(&self->send_waiters);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_Que_recvFor(thrd_Que* self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self->type), out.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        thrd_Mtx_lockProtcd(&self->lock);
        if (!ArrQue_isEmpty(self->que)) {
            let_ignore = unwrap_(ArrQue_deque(&self->que, out));
            thrd_wait_List_wakeOne(&self->send_waiters);
            thrd_Mtx_unlock(&self->lock);
            return_ok(out);
        }
        if (self->closed) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$thrd_chan_Closed());
        }
        let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(remaining)) {
            thrd_Mtx_unlock(&self->lock);
            return_err(E_cause$Sched_Timeout());
        }
        thrd_wait_List_prepend(&self->recv_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
        catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->recv_waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            return_err(err);
        }));
        thrd_Mtx_lockProtcd(&self->lock);
        thrd_wait_List_unlink(&self->recv_waiters, &link);
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);
