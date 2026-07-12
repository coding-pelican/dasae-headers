#include "dh/thrd/SPMC.h"
#include "dh/mem/ring.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_SPMC__idx(thrd_SPMC* self, usize pos))(usize));
$static fn_((thrd_SPMC__slotMut(thrd_SPMC* self, TypeInfo type, usize pos))(u_P$raw));
$static fn_((thrd_SPMC__claimHead(thrd_SPMC* self))(O$usize));
$static fn_((thrd_SPMC__releaseHead(thrd_SPMC* self, usize pos))(void));
$static fn_((thrd_SPMC__wakeNotEmpty(thrd_SPMC* self))(void));
$static fn_((thrd_SPMC__wakeNotFull(thrd_SPMC* self))(void));
$static fn_((thrd_SPMC__broadcast(thrd_SPMC* self))(void));
$static fn_((thrd_SPMC__sendReady(P$raw ctx))(bool));
$static fn_((thrd_SPMC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_SPMC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_SPMC__recvReady(P$raw ctx))(bool));
$static fn_((thrd_SPMC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_SPMC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_SPMC__sendOpPoll(P$raw ctx, u_P$raw result))(bool));
$static fn_((thrd_SPMC__recvOpPoll(P$raw ctx, u_P$raw result))(bool));

$static let_(thrd_SPMC__send_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_SPMC__sendReady,
    .linkFn = thrd_SPMC__sendLink,
    .unlinkFn = thrd_SPMC__sendUnlink,
};

$static let_(thrd_SPMC__recv_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_SPMC__recvReady,
    .linkFn = thrd_SPMC__recvLink,
    .unlinkFn = thrd_SPMC__recvUnlink,
};

$static let_(thrd_SPMC__send_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_SPMC__sendOpPoll,
};

$static let_(thrd_SPMC__recv_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_SPMC__recvOpPoll,
};

fn_((thrd_SPMC_init(u_S$raw buf))(thrd_SPMC)) {
    return (thrd_SPMC){
        .buf = buf.raw,
        .head_claim = atom_V_init(0),
        .head_release = atom_V_init(0),
        .tail = atom_V_init(0),
        .closed = atom_V_init(false),
        .wait_lock = thrd_Mtx_init(),
        .send_waiters = thrd_wait_List_init(),
        .recv_waiters = thrd_wait_List_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_SPMC_fini(thrd_SPMC* self))(void)) {
    claim_assert_nonnull(self);
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_SPMC__broadcast(self);
    thrd_Mtx_fini(&self->wait_lock);
};

fn_((thrd_SPMC_len(thrd_SPMC* self))(usize)) {
    let head = atom_V_load(&self->head_release, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return tail - head;
};

fn_((thrd_SPMC_cap(thrd_SPMC* self))(usize)) {
    return self->buf.len;
};

fn_((thrd_SPMC_isFull(thrd_SPMC* self))(bool)) {
    let head = atom_V_load(&self->head_release, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return tail - head == self->buf.len;
};

fn_((thrd_SPMC_isEmpty(thrd_SPMC* self))(bool)) {
    let head = atom_V_load(&self->head_claim, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
    return head == tail;
};

fn_((thrd_SPMC_isOpen(thrd_SPMC* self))(bool)) {
    return !thrd_SPMC_isClosed(self);
};

fn_((thrd_SPMC_isClosed(thrd_SPMC* self))(bool)) {
    return atom_V_load(&self->closed, atom_MemOrd_acquire);
};

fn_((thrd_SPMC_tx(thrd_SPMC* self))(thrd_SPMC_Tx)) {
    return (thrd_SPMC_Tx){ .chan = self };
};

fn_((thrd_SPMC_Tx_send(thrd_SPMC_Tx self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_SPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        let head = atom_V_load(&self.chan->head_release, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (tail - head < self.chan->buf.len) {
            u_memcpy(thrd_SPMC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
            atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
            thrd_SPMC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPMC_isClosed(self.chan) && thrd_SPMC_isFull(self.chan)) {
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

fn_((thrd_SPMC_Tx_trySend(thrd_SPMC_Tx self, u_V$raw item))(thrd_chan_E$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    if (thrd_SPMC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    let head = atom_V_load(&self.chan->head_release, atom_MemOrd_acquire);
    let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
    if (tail - head == self.chan->buf.len) {
        return_err(E_cause$thrd_chan_Full());
    }
    u_memcpy(thrd_SPMC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
    atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
    thrd_SPMC__wakeNotEmpty(self.chan);
    return_ok({});
} $unguarded(fn);

fn_((thrd_SPMC_Tx_sendFor(thrd_SPMC_Tx self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_SPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        let head = atom_V_load(&self.chan->head_release, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail, atom_MemOrd_acquire);
        if (tail - head < self.chan->buf.len) {
            u_memcpy(thrd_SPMC__slotMut(self.chan, item.inner_type, tail), item.ref.as_const);
            atom_V_store(&self.chan->tail, tail + 1, atom_MemOrd_release);
            thrd_SPMC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPMC_isClosed(self.chan) && thrd_SPMC_isFull(self.chan)) {
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

fn_((thrd_SPMC_rx(thrd_SPMC* self))(thrd_SPMC_Rx)) {
    return (thrd_SPMC_Rx){ .chan = self };
};

fn_((thrd_SPMC_Rx_tryRecv(thrd_SPMC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    if_some((thrd_SPMC__claimHead(self.chan))(pos)) {
        u_memcpy(out.ref, thrd_SPMC__slotMut(self.chan, out.inner_type, pos).as_const);
        thrd_SPMC__releaseHead(self.chan, pos);
        thrd_SPMC__wakeNotFull(self.chan);
        return_ok(out);
    }
    if (thrd_SPMC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    return_err(E_cause$thrd_chan_Empty());
} $unguarded(fn);

fn_((thrd_SPMC_Rx_recv(thrd_SPMC_Rx self, thrd_wait_Src cancel_src, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_SPMC__claimHead(self.chan))(pos)) {
            u_memcpy(out.ref, thrd_SPMC__slotMut(self.chan, out.inner_type, pos).as_const);
            thrd_SPMC__releaseHead(self.chan, pos);
            thrd_SPMC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_SPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPMC_isClosed(self.chan) && thrd_SPMC_isEmpty(self.chan)) {
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

fn_((thrd_SPMC_Rx_recvFor(thrd_SPMC_Rx self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_SPMC__claimHead(self.chan))(pos)) {
            u_memcpy(out.ref, thrd_SPMC__slotMut(self.chan, out.inner_type, pos).as_const);
            thrd_SPMC__releaseHead(self.chan, pos);
            thrd_SPMC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_SPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_SPMC_isClosed(self.chan) && thrd_SPMC_isEmpty(self.chan)) {
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

fn_((thrd_SPMC_Rx_recvOp(thrd_SPMC_Rx self, TypeInfo type))(thrd_SPMC_Rx_RecvOp)) {
    debug_assert_eqBy($typed(self.chan->type), type, TypeInfo_eql);
    return (thrd_SPMC_Rx_RecvOp){ .rx = self, .type = $typing(type) };
};

fn_((thrd_SPMC_Rx_RecvOp_op(thrd_SPMC_Rx_RecvOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_SPMC__recv_op_vtbl,
        .src = thrd_wait_Src_ensureValid((thrd_wait_Src){
            .ctx = self->rx.chan,
            .vtbl = &thrd_SPMC__recv_vtbl,
        }),
    });
};

fn_((thrd_SPMC_Tx_sendOp(thrd_SPMC_Tx self, u_P$raw item))(thrd_SPMC_Tx_SendOp)) {
    debug_assert_eqBy($typed(self.chan->type), item.type, TypeInfo_eql);
    return (thrd_SPMC_Tx_SendOp){ .tx = self, .item = item };
};

fn_((thrd_SPMC_Tx_SendOp_op(thrd_SPMC_Tx_SendOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_SPMC__send_op_vtbl,
        .src = thrd_wait_Src_ensureValid((thrd_wait_Src){
            .ctx = self->tx.chan,
            .vtbl = &thrd_SPMC__send_vtbl,
        }),
    });
};

fn_((thrd_SPMC_open(thrd_SPMC* self))(thrd_SPMC_Chans)) {
    return (thrd_SPMC_Chans)tie_(
        (thrd_SPMC_tx(self)),
        (thrd_SPMC_rx(self))
    );
};

fn_((thrd_SPMC_close(thrd_SPMC* self))(void)) {
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_SPMC__broadcast(self);
};

fn_((thrd_SPMC__idx(thrd_SPMC* self, usize pos))(usize)) {
    return mem_ring_idx(self->buf.len, pos);
};

fn_((thrd_SPMC__slotMut(thrd_SPMC* self, TypeInfo type, usize pos))(u_P$raw)) {
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return u_atS(u_from$S((type)(self->buf)), thrd_SPMC__idx(self, pos));
};

fn_((thrd_SPMC__claimHead(thrd_SPMC* self))(O$usize) $scope) {
    var head = atom_V_load(&self->head_claim, atom_MemOrd_acquire);
    while (true) {
        let tail = atom_V_load(&self->tail, atom_MemOrd_acquire);
        if (head == tail) {
            return_none();
        }
        head = orelse_((atom_V_cmpXchgWeak(
            &self->head_claim, head, head + 1,
            atom_MemOrd_acq_rel, atom_MemOrd_acquire
        ))(return_some(head)));
    }
} $unscoped(fn);

fn_((thrd_SPMC__releaseHead(thrd_SPMC* self, usize pos))(void)) {
    while (atom_V_load(&self->head_release, atom_MemOrd_acquire) != pos) {
        atom_spinLoopHint();
    }
    atom_V_store(&self->head_release, pos + 1, atom_MemOrd_release);
};

fn_((thrd_SPMC__wakeNotEmpty(thrd_SPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPMC__wakeNotFull(thrd_SPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->send_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPMC__broadcast(thrd_SPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeAll(&self->send_waiters);
    thrd_wait_List_wakeAll(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPMC__recvOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_SPMC_Rx_RecvOp*)(ctx));
    debug_assert_eqBy($typed(self->type), result.type, TypeInfo_eql);
    return isOk(thrd_SPMC_Rx_tryRecv(self->rx, (u_V$raw){ .inner = result.raw, .type = result.type }));
} $unscoped(fn);

fn_((thrd_SPMC__sendOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_SPMC_Tx_SendOp*)(ctx));
    let_ignore = result;
    return isOk(thrd_SPMC_Tx_trySend(self->tx, (u_V$raw){ .inner = self->item.raw, .type = self->item.type }));
} $unscoped(fn);

fn_((thrd_SPMC__recvReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    return !thrd_SPMC_isEmpty(self) || thrd_SPMC_isClosed(self);
};

fn_((thrd_SPMC__sendReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    return !thrd_SPMC_isFull(self) || thrd_SPMC_isClosed(self);
};

fn_((thrd_SPMC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_SPMC_isFull(self) || thrd_SPMC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_SPMC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_SPMC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_SPMC_isEmpty(self) || thrd_SPMC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_SPMC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_SPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};
