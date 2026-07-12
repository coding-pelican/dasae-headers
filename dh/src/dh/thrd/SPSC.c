#include "dh/thrd/SPSC.h"
#include "dh/mem/ring.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_SPSC__idx(thrd_SPSC* self, usize pos))(usize));
$static fn_((thrd_SPSC__slotMut(thrd_SPSC* self, TypeInfo type, usize pos))(u_P$raw));
$static fn_((thrd_SPSC__wakeNotEmpty(thrd_SPSC* self))(void));
$static fn_((thrd_SPSC__wakeNotFull(thrd_SPSC* self))(void));
$static fn_((thrd_SPSC__broadcast(thrd_SPSC* self))(void));
$static fn_((thrd_SPSC__sendReady(P$raw ctx))(bool));
$static fn_((thrd_SPSC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_SPSC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_SPSC__recvReady(P$raw ctx))(bool));
$static fn_((thrd_SPSC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_SPSC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_SPSC__sendOpPoll(P$raw ctx, u_P$raw result))(bool));
$static fn_((thrd_SPSC__recvOpPoll(P$raw ctx, u_P$raw result))(bool));

$static let_(thrd_SPSC__send_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_SPSC__sendReady,
    .linkFn = thrd_SPSC__sendLink,
    .unlinkFn = thrd_SPSC__sendUnlink,
};

$static let_(thrd_SPSC__recv_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_SPSC__recvReady,
    .linkFn = thrd_SPSC__recvLink,
    .unlinkFn = thrd_SPSC__recvUnlink,
};

$static let_(thrd_SPSC__send_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_SPSC__sendOpPoll,
};

$static let_(thrd_SPSC__recv_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_SPSC__recvOpPoll,
};

fn_((thrd_SPSC_init(u_S$raw buf))(thrd_SPSC)) {
    return (thrd_SPSC){
        .buf = buf.raw,
        .head = atom_V_init(0),
        .tail = atom_V_init(0),
        .closed = atom_V_init(false),
        .wait_lock = thrd_Mtx_init(),
        .send_waiters = thrd_wait_List_init(),
        .recv_waiters = thrd_wait_List_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_SPSC_fini(thrd_SPSC* self))(void)) {
    claim_assert_nonnull(self);
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_SPSC__broadcast(self);
    thrd_Mtx_fini(&self->wait_lock);
};

fn_((thrd_SPSC_len(thrd_SPSC* self))(usize)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return tail - head;
};

fn_((thrd_SPSC_cap(thrd_SPSC* self))(usize)) {
    return self->buf.len;
};

fn_((thrd_SPSC_isFull(thrd_SPSC* self))(bool)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return tail - head == self->buf.len;
};

fn_((thrd_SPSC_isEmpty(thrd_SPSC* self))(bool)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return head == tail;
};

fn_((thrd_SPSC_isOpen(thrd_SPSC* self))(bool)) {
    return !thrd_SPSC_isClosed(self);
};

fn_((thrd_SPSC_isClosed(thrd_SPSC* self))(bool)) {
    return atom_V_load(&self->closed, atom_MemOrd_acquire);
};

fn_((thrd_SPSC_tx(thrd_SPSC* self))(thrd_SPSC_Tx)) {
    return (thrd_SPSC_Tx){ .chan = self };
};

fn_((thrd_SPSC_Tx_send(thrd_SPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_SPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (tail - head < self.chan->buf.len) {
            u_memcpy(thrd_SPSC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
            atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
            thrd_SPSC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPSC_isClosed(self.chan) && thrd_SPSC_isFull(self.chan)) {
            thrd_wait_List_prepend(&self.chan->send_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_List_unlink(&self.chan->send_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_List_unlink(&self.chan->send_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_SPSC_Tx_trySend(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_E$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    if (thrd_SPSC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
    if (tail - head == self.chan->buf.len) {
        return_err(E_cause$thrd_chan_Full());
    }
    u_memcpy(thrd_SPSC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
    atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
    thrd_SPSC__wakeNotEmpty(self.chan);
    return_ok({});
} $unguarded(fn);

fn_((thrd_SPSC_Tx_sendFor(thrd_SPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_SPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (tail - head < self.chan->buf.len) {
            u_memcpy(thrd_SPSC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
            atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
            thrd_SPSC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPSC_isClosed(self.chan) && thrd_SPSC_isFull(self.chan)) {
            let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
            if (time_Dur_isZero(remaining)) {
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(E_cause$Sched_Timeout());
            }
            thrd_wait_List_prepend(&self.chan->send_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_List_unlink(&self.chan->send_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_List_unlink(&self.chan->send_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_SPSC_rx(thrd_SPSC* self))(thrd_SPSC_Rx)) {
    return (thrd_SPSC_Rx){ .chan = self };
};

fn_((thrd_SPSC_Rx_tryRecv(thrd_SPSC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
    if (head == tail) {
        if (thrd_SPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        return_err(E_cause$thrd_chan_Empty());
    }
    u_memcpy(out.ref, thrd_SPSC__slotMut(self.chan, out.inner_type, head).as_const);
    atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
    thrd_SPSC__wakeNotFull(self.chan);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_SPSC_Rx_recv(thrd_SPSC_Rx self, thrd_wait_Src cancel_src, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (head != tail) {
            u_memcpy(out.ref, thrd_SPSC__slotMut(self.chan, out.inner_type, head).as_const);
            atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
            thrd_SPSC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_SPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPSC_isClosed(self.chan) && thrd_SPSC_isEmpty(self.chan)) {
            thrd_wait_List_prepend(&self.chan->recv_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_List_unlink(&self.chan->recv_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_List_unlink(&self.chan->recv_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_SPSC_Rx_recvFor(thrd_SPSC_Rx self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (head != tail) {
            u_memcpy(out.ref, thrd_SPSC__slotMut(self.chan, out.inner_type, head).as_const);
            atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
            thrd_SPSC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_SPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPSC_isClosed(self.chan) && thrd_SPSC_isEmpty(self.chan)) {
            let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
            if (time_Dur_isZero(remaining)) {
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(E_cause$Sched_Timeout());
            }
            thrd_wait_List_prepend(&self.chan->recv_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_List_unlink(&self.chan->recv_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_List_unlink(&self.chan->recv_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_SPSC_Rx_recvOp(thrd_SPSC_Rx self, TypeInfo type))(thrd_SPSC_Rx_RecvOp)) {
    debug_assert_eqBy($typed(self.chan->type), type, TypeInfo_eql);
    return (thrd_SPSC_Rx_RecvOp){ .rx = self, .type = $typing(type) };
};

fn_((thrd_SPSC_Rx_RecvOp_op(thrd_SPSC_Rx_RecvOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_SPSC__recv_op_vtbl,
        .src = thrd_wait_Src_ensureValid((thrd_wait_Src){
            .ctx = self->rx.chan,
            .vtbl = &thrd_SPSC__recv_vtbl,
        }),
    });
};

fn_((thrd_SPSC_Tx_sendOp(thrd_SPSC_Tx self, u_P$raw item))(thrd_SPSC_Tx_SendOp)) {
    debug_assert_eqBy($typed(self.chan->type), item.type, TypeInfo_eql);
    return (thrd_SPSC_Tx_SendOp){ .tx = self, .item = item };
};

fn_((thrd_SPSC_Tx_SendOp_op(thrd_SPSC_Tx_SendOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_SPSC__send_op_vtbl,
        .src = thrd_wait_Src_ensureValid((thrd_wait_Src){
            .ctx = self->tx.chan,
            .vtbl = &thrd_SPSC__send_vtbl,
        }),
    });
};

fn_((thrd_SPSC_open(thrd_SPSC* self))(thrd_SPSC_Chans)) {
    return (thrd_SPSC_Chans)tie_(
        (thrd_SPSC_tx(self)),
        (thrd_SPSC_rx(self))
    );
};

fn_((thrd_SPSC_close(thrd_SPSC* self))(void)) {
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_SPSC__broadcast(self);
};

fn_((thrd_SPSC__idx(thrd_SPSC* self, usize pos))(usize)) {
    return mem_ring_idx(self->buf.len, pos);
};

fn_((thrd_SPSC__slotMut(thrd_SPSC* self, TypeInfo type, usize pos))(u_P$raw)) {
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return u_atS(u_from$S((type)(self->buf)), thrd_SPSC__idx(self, pos));
};

fn_((thrd_SPSC__wakeNotEmpty(thrd_SPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPSC__wakeNotFull(thrd_SPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->send_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPSC__broadcast(thrd_SPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeAll(&self->send_waiters);
    thrd_wait_List_wakeAll(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPSC__recvOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_SPSC_Rx_RecvOp*)(ctx));
    debug_assert_eqBy($typed(self->type), result.type, TypeInfo_eql);
    return isOk(thrd_SPSC_Rx_tryRecv(self->rx, (u_V$raw){ .inner = result.raw, .type = result.type }));
} $unscoped(fn);

fn_((thrd_SPSC__sendOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_SPSC_Tx_SendOp*)(ctx));
    let_ignore = result;
    return isOk(thrd_SPSC_Tx_trySend(self->tx, (u_V$raw){ .inner = self->item.raw, .type = self->item.type }));
} $unscoped(fn);

fn_((thrd_SPSC__recvReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    return !thrd_SPSC_isEmpty(self) || thrd_SPSC_isClosed(self);
};

fn_((thrd_SPSC__sendReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    return !thrd_SPSC_isFull(self) || thrd_SPSC_isClosed(self);
};

fn_((thrd_SPSC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_SPSC_isFull(self) || thrd_SPSC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_SPSC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPSC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_SPSC_isEmpty(self) || thrd_SPSC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_SPSC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_SPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};
