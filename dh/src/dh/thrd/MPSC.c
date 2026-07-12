#include "dh/thrd/MPSC.h"
#include "dh/mem/ring.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_MPSC__idx(thrd_MPSC* self, usize pos))(usize));
$static fn_((thrd_MPSC__slotMut(thrd_MPSC* self, TypeInfo type, usize pos))(u_P$raw));
$static fn_((thrd_MPSC__claimTail(thrd_MPSC* self))(O$usize));
$static fn_((thrd_MPSC__publishTail(thrd_MPSC* self, usize pos))(void));
$static fn_((thrd_MPSC__wakeNotEmpty(thrd_MPSC* self))(void));
$static fn_((thrd_MPSC__wakeNotFull(thrd_MPSC* self))(void));
$static fn_((thrd_MPSC__broadcast(thrd_MPSC* self))(void));
$static fn_((thrd_MPSC__sendReady(P$raw ctx))(bool));
$static fn_((thrd_MPSC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_MPSC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_MPSC__recvReady(P$raw ctx))(bool));
$static fn_((thrd_MPSC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_MPSC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_MPSC__sendOpPoll(P$raw ctx, u_P$raw result))(bool));
$static fn_((thrd_MPSC__recvOpPoll(P$raw ctx, u_P$raw result))(bool));

$static let_(thrd_MPSC__send_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_MPSC__sendReady,
    .linkFn = thrd_MPSC__sendLink,
    .unlinkFn = thrd_MPSC__sendUnlink,
};

$static let_(thrd_MPSC__recv_vtbl, thrd_wait_Src_VTbl) = {
    .readyFn = thrd_MPSC__recvReady,
    .linkFn = thrd_MPSC__recvLink,
    .unlinkFn = thrd_MPSC__recvUnlink,
};

$static let_(thrd_MPSC__send_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_MPSC__sendOpPoll,
};

$static let_(thrd_MPSC__recv_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_MPSC__recvOpPoll,
};

fn_((thrd_MPSC_init(u_S$raw buf))(thrd_MPSC)) {
    return (thrd_MPSC){
        .buf = buf.raw,
        .head = atom_V_init(0),
        .tail_claim = atom_V_init(0),
        .tail_commit = atom_V_init(0),
        .closed = atom_V_init(false),
        .wait_lock = thrd_Mtx_init(),
        .send_waiters = thrd_wait_List_init(),
        .recv_waiters = thrd_wait_List_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_MPSC_fini(thrd_MPSC* self))(void)) {
    claim_assert_nonnull(self);
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_MPSC__broadcast(self);
    thrd_Mtx_fini(&self->wait_lock);
};

fn_((thrd_MPSC_len(thrd_MPSC* self))(usize)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_commit, atom_MemOrd_acquire);
    return tail - head;
};

fn_((thrd_MPSC_cap(thrd_MPSC* self))(usize)) {
    return self->buf.len;
};

fn_((thrd_MPSC_isFull(thrd_MPSC* self))(bool)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_claim, atom_MemOrd_acquire);
    return tail - head == self->buf.len;
};

fn_((thrd_MPSC_isEmpty(thrd_MPSC* self))(bool)) {
    let head = atom_V_load(&self->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_commit, atom_MemOrd_acquire);
    return head == tail;
};

fn_((thrd_MPSC_isOpen(thrd_MPSC* self))(bool)) {
    return !thrd_MPSC_isClosed(self);
};

fn_((thrd_MPSC_isClosed(thrd_MPSC* self))(bool)) {
    return atom_V_load(&self->closed, atom_MemOrd_acquire);
};

fn_((thrd_MPSC_tx(thrd_MPSC* self))(thrd_MPSC_Tx)) {
    return (thrd_MPSC_Tx){ .chan = self };
};

fn_((thrd_MPSC_Tx_trySend(thrd_MPSC_Tx self, u_V$raw item))(thrd_chan_E$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    if (thrd_MPSC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    if_some((thrd_MPSC__claimTail(self.chan))(pos)) {
        u_memcpy(thrd_MPSC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
        thrd_MPSC__publishTail(self.chan, pos);
        thrd_MPSC__wakeNotEmpty(self.chan);
        return_ok({});
    }
    return_err(E_cause$thrd_chan_Full());
} $unguarded(fn);

fn_((thrd_MPSC_Tx_send(thrd_MPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_MPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        if_some((thrd_MPSC__claimTail(self.chan))(pos)) {
            u_memcpy(thrd_MPSC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
            thrd_MPSC__publishTail(self.chan, pos);
            thrd_MPSC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPSC_isClosed(self.chan) && thrd_MPSC_isFull(self.chan)) {
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

fn_((thrd_MPSC_Tx_sendFor(thrd_MPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_MPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        if_some((thrd_MPSC__claimTail(self.chan))(pos)) {
            u_memcpy(thrd_MPSC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
            thrd_MPSC__publishTail(self.chan, pos);
            thrd_MPSC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPSC_isClosed(self.chan) && thrd_MPSC_isFull(self.chan)) {
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

fn_((thrd_MPSC_rx(thrd_MPSC* self))(thrd_MPSC_Rx)) {
    return (thrd_MPSC_Rx){ .chan = self };
};

fn_((thrd_MPSC_Rx_recv(thrd_MPSC_Rx self, thrd_wait_Src cancel_src, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail_commit, atom_MemOrd_acquire);
        if (head != tail) {
            u_memcpy(out.ref, thrd_MPSC__slotMut(self.chan, out.inner_type, head).as_const);
            atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
            thrd_MPSC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_MPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPSC_isClosed(self.chan) && thrd_MPSC_isEmpty(self.chan)) {
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

fn_((thrd_MPSC_Rx_tryRecv(thrd_MPSC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
    let tail = atom_V_load(&self.chan->tail_commit, atom_MemOrd_acquire);
    if (head == tail) {
        if (thrd_MPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        return_err(E_cause$thrd_chan_Empty());
    }
    u_memcpy(out.ref, thrd_MPSC__slotMut(self.chan, out.inner_type, head).as_const);
    atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
    thrd_MPSC__wakeNotFull(self.chan);
    return_ok(out);
} $unguarded(fn);

fn_((thrd_MPSC_Rx_recvFor(thrd_MPSC_Rx self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        let head = atom_V_load(&self.chan->head, atom_MemOrd_acquire);
        let tail = atom_V_load(&self.chan->tail_commit, atom_MemOrd_acquire);
        if (head != tail) {
            u_memcpy(out.ref, thrd_MPSC__slotMut(self.chan, out.inner_type, head).as_const);
            atom_V_store(&self.chan->head, head + 1, atom_MemOrd_release);
            thrd_MPSC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_MPSC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPSC_isClosed(self.chan) && thrd_MPSC_isEmpty(self.chan)) {
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

fn_((thrd_MPSC_Rx_recvOp(thrd_MPSC_Rx self, TypeInfo type))(thrd_MPSC_Rx_RecvOp)) {
    debug_assert_eqBy($typed(self.chan->type), type, TypeInfo_eql);
    return (thrd_MPSC_Rx_RecvOp){ .rx = self, .type = $typing(type) };
};

fn_((thrd_MPSC_Rx_RecvOp_op(thrd_MPSC_Rx_RecvOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return (thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_MPSC__recv_op_vtbl,
        .src = { .ctx = self->rx.chan, .vtbl = &thrd_MPSC__recv_vtbl },
    };
};

fn_((thrd_MPSC_Tx_sendOp(thrd_MPSC_Tx self, u_P$raw item))(thrd_MPSC_Tx_SendOp)) {
    debug_assert_eqBy($typed(self.chan->type), item.type, TypeInfo_eql);
    return (thrd_MPSC_Tx_SendOp){ .tx = self, .item = item };
};

fn_((thrd_MPSC_Tx_SendOp_op(thrd_MPSC_Tx_SendOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return (thrd_Select_Op){
        .ctx = self,
        .vtbl = &thrd_MPSC__send_op_vtbl,
        .src = { .ctx = self->tx.chan, .vtbl = &thrd_MPSC__send_vtbl },
    };
};

fn_((thrd_MPSC_open(thrd_MPSC* self))(thrd_MPSC_Chans)) {
    return (thrd_MPSC_Chans)tie_(
        (thrd_MPSC_tx(self)),
        (thrd_MPSC_rx(self))
    );
};

fn_((thrd_MPSC_close(thrd_MPSC* self))(void)) {
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_MPSC__broadcast(self);
};

fn_((thrd_MPSC__idx(thrd_MPSC* self, usize pos))(usize)) {
    return mem_ring_idx(self->buf.len, pos);
};

fn_((thrd_MPSC__slotMut(thrd_MPSC* self, TypeInfo type, usize pos))(u_P$raw)) {
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return u_atS(u_from$S((type)(self->buf)), thrd_MPSC__idx(self, pos));
};

fn_((thrd_MPSC__claimTail(thrd_MPSC* self))(O$usize) $scope) {
    var tail = atom_V_load(&self->tail_claim, atom_MemOrd_acquire);
    while (true) {
        let head = atom_V_load(&self->head, atom_MemOrd_acquire);
        if (tail - head == self->buf.len) {
            return_none();
        }
        tail = orelse_((atom_V_cmpXchgWeak(
            &self->tail_claim, tail, tail + 1,
            atom_MemOrd_acq_rel, atom_MemOrd_acquire
        ))(return_some(tail)));
    }
} $unscoped(fn);

fn_((thrd_MPSC__publishTail(thrd_MPSC* self, usize pos))(void)) {
    while (atom_V_load(&self->tail_commit, atom_MemOrd_acquire) != pos) {
        atom_spinLoopHint();
    }
    atom_V_store(&self->tail_commit, pos + 1, atom_MemOrd_release);
};

fn_((thrd_MPSC__wakeNotEmpty(thrd_MPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPSC__wakeNotFull(thrd_MPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeOne(&self->send_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPSC__broadcast(thrd_MPSC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_wakeAll(&self->send_waiters);
    thrd_wait_List_wakeAll(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPSC__recvOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPSC_Rx_RecvOp*)(ctx));
    debug_assert_eqBy($typed(self->type), result.type, TypeInfo_eql);
    return isOk(thrd_MPSC_Rx_tryRecv(self->rx, (u_V$raw){ .inner = result.raw, .type = result.type }));
} $unscoped(fn);

fn_((thrd_MPSC__sendOpPoll(P$raw ctx, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPSC_Tx_SendOp*)(ctx));
    let_ignore = result;
    return isOk(thrd_MPSC_Tx_trySend(self->tx, (u_V$raw){ .inner = self->item.raw, .type = self->item.type }));
} $unscoped(fn);

fn_((thrd_MPSC__recvReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    return !thrd_MPSC_isEmpty(self) || thrd_MPSC_isClosed(self);
};

fn_((thrd_MPSC__sendReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    return !thrd_MPSC_isFull(self) || thrd_MPSC_isClosed(self);
};

fn_((thrd_MPSC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_MPSC_isFull(self) || thrd_MPSC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_MPSC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPSC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_MPSC_isEmpty(self) || thrd_MPSC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_List_prepend(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_MPSC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_MPSC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_List_unlink(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};
