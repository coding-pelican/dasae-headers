#include "dh/thrd/MPMC.h"
#include "dh/mem/ring.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_MPMC__idx(thrd_MPMC* self, usize pos))(usize));
$static fn_((thrd_MPMC__slotMut(thrd_MPMC* self, TypeInfo type, usize pos))(u_P$raw));
$static fn_((thrd_MPMC__claimTail(thrd_MPMC* self))(O$usize));
$static fn_((thrd_MPMC__publishTail(thrd_MPMC* self, usize pos))(void));
$static fn_((thrd_MPMC__claimHead(thrd_MPMC* self))(O$usize));
$static fn_((thrd_MPMC__releaseHead(thrd_MPMC* self, usize pos))(void));
$static fn_((thrd_MPMC__wakeNotEmpty(thrd_MPMC* self))(void));
$static fn_((thrd_MPMC__wakeNotFull(thrd_MPMC* self))(void));
$static fn_((thrd_MPMC__broadcast(thrd_MPMC* self))(void));
$static fn_((thrd_MPMC__sendReady(P$raw ctx))(bool));
$static fn_((thrd_MPMC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_MPMC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_MPMC__recvReady(P$raw ctx))(bool));
$static fn_((thrd_MPMC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool));
$static fn_((thrd_MPMC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void));
$static fn_((thrd_MPMC__sendOpPoll(P$raw ctx))(bool));
$static fn_((thrd_MPMC__sendOpCommit(P$raw ctx, u_P$raw data, u_P$raw result))(bool));
$static fn_((thrd_MPMC__recvOpPoll(P$raw ctx))(bool));
$static fn_((thrd_MPMC__recvOpCommit(P$raw ctx, u_P$raw data, u_P$raw result))(bool));

$static let_(thrd_MPMC__send_vtbl, thrd_Wakeable_VTbl) = {
    .readyFn = thrd_MPMC__sendReady,
    .linkFn = thrd_MPMC__sendLink,
    .unlinkFn = thrd_MPMC__sendUnlink,
};

$static let_(thrd_MPMC__recv_vtbl, thrd_Wakeable_VTbl) = {
    .readyFn = thrd_MPMC__recvReady,
    .linkFn = thrd_MPMC__recvLink,
    .unlinkFn = thrd_MPMC__recvUnlink,
};

$static let_(thrd_MPMC__send_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_MPMC__sendOpPoll,
    .commitFn = thrd_MPMC__sendOpCommit,
};

$static let_(thrd_MPMC__recv_op_vtbl, thrd_Select_Op_VTbl) = {
    .pollFn = thrd_MPMC__recvOpPoll,
    .commitFn = thrd_MPMC__recvOpCommit,
};

fn_((thrd_MPMC_initFixed(u_S$raw buf))(thrd_MPMC)) {
    claim_assert(buf.len != 0);
    return (thrd_MPMC){
        .buf = buf.raw,
        .head_claim = atom_V_init(0),
        .head_release = atom_V_init(0),
        .tail_claim = atom_V_init(0),
        .tail_commit = atom_V_init(0),
        .closed = atom_V_init(false),
        .wait_lock = thrd_Mtx_init(),
        .send_waiters = thrd_wait_Chain_init(),
        .recv_waiters = thrd_wait_Chain_init(),
        .type = $typing(buf.type),
    };
};

fn_((thrd_MPMC_finiFixed(thrd_MPMC* self))(void)) {
    claim_assert_nonnull(self);
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_MPMC__broadcast(self);
    thrd_Mtx_fini(&self->wait_lock);
};

fn_((thrd_MPMC_len(thrd_MPMC* self))(usize)) {
    let head = atom_V_load(&self->head_release, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_commit, atom_MemOrd_acquire);
    return tail - head;
};

fn_((thrd_MPMC_cap(thrd_MPMC* self))(usize)) {
    return self->buf.len;
};

fn_((thrd_MPMC_isFull(thrd_MPMC* self))(bool)) {
    let head = atom_V_load(&self->head_release, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_claim, atom_MemOrd_acquire);
    return tail - head == self->buf.len;
};

fn_((thrd_MPMC_isEmpty(thrd_MPMC* self))(bool)) {
    let head = atom_V_load(&self->head_claim, atom_MemOrd_acquire);
    let tail = atom_V_load(&self->tail_commit, atom_MemOrd_acquire);
    return head == tail;
};

fn_((thrd_MPMC_isOpen(thrd_MPMC* self))(bool)) {
    return !thrd_MPMC_isClosed(self);
};

fn_((thrd_MPMC_isClosed(thrd_MPMC* self))(bool)) {
    return atom_V_load(&self->closed, atom_MemOrd_acquire);
};

fn_((thrd_MPMC_tx(thrd_MPMC* self))(thrd_MPMC_Tx)) {
    return (thrd_MPMC_Tx){ .chan = self };
};

fn_((thrd_MPMC_Tx_trySend(thrd_MPMC_Tx self, u_V$raw item))(thrd_chan_E$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    if (thrd_MPMC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    if_some((thrd_MPMC__claimTail(self.chan))(pos)) {
        u_memcpy(thrd_MPMC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
        thrd_MPMC__publishTail(self.chan, pos);
        thrd_MPMC__wakeNotEmpty(self.chan);
        return_ok({});
    }
    return_err(E_cause$thrd_chan_Full());
} $unguarded(fn);

fn_((thrd_MPMC_Tx_send(thrd_MPMC_Tx self, u_V$raw item, thrd_Wakeable cancel_src))(thrd_chan_WaitE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_MPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        if_some((thrd_MPMC__claimTail(self.chan))(pos)) {
            u_memcpy(thrd_MPMC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
            thrd_MPMC__publishTail(self.chan, pos);
            thrd_MPMC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPMC_isClosed(self.chan) && thrd_MPMC_isFull(self.chan)) {
            thrd_wait_Chain_prepend(&self.chan->send_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_Chain_unlink(&self.chan->send_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_Chain_unlink(&self.chan->send_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_MPMC_Tx_sendFor(thrd_MPMC_Tx self, u_V$raw item, thrd_Wakeable cancel_src, time_Dur dur))(thrd_chan_TimedE$void) $guard) {
    debug_assert_eqBy($typed(self.chan->type), item.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if (thrd_MPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }
        if_some((thrd_MPMC__claimTail(self.chan))(pos)) {
            u_memcpy(thrd_MPMC__slotMut(self.chan, item.inner_type, pos), item.ref.as_const);
            thrd_MPMC__publishTail(self.chan, pos);
            thrd_MPMC__wakeNotEmpty(self.chan);
            return_ok({});
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPMC_isClosed(self.chan) && thrd_MPMC_isFull(self.chan)) {
            let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
            if (time_Dur_isZero(remaining)) {
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(E_cause$Sched_Timeout());
            }
            thrd_wait_Chain_prepend(&self.chan->send_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_Chain_unlink(&self.chan->send_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_Chain_unlink(&self.chan->send_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_MPMC_rx(thrd_MPMC* self))(thrd_MPMC_Rx)) {
    return (thrd_MPMC_Rx){ .chan = self };
};

fn_((thrd_MPMC_Rx_tryRecv(thrd_MPMC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    if_some((thrd_MPMC__claimHead(self.chan))(pos)) {
        u_memcpy(out.ref, thrd_MPMC__slotMut(self.chan, out.inner_type, pos).as_const);
        thrd_MPMC__releaseHead(self.chan, pos);
        thrd_MPMC__wakeNotFull(self.chan);
        return_ok(out);
    }
    if (thrd_MPMC_isClosed(self.chan)) {
        return_err(E_cause$thrd_chan_Closed());
    }
    return_err(E_cause$thrd_chan_Empty());
} $unguarded(fn);

fn_((thrd_MPMC_Rx_recv(thrd_MPMC_Rx self, thrd_Wakeable cancel_src, u_V$raw out))(thrd_chan_WaitE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_MPMC__claimHead(self.chan))(pos)) {
            u_memcpy(out.ref, thrd_MPMC__slotMut(self.chan, out.inner_type, pos).as_const);
            thrd_MPMC__releaseHead(self.chan, pos);
            thrd_MPMC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_MPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPMC_isClosed(self.chan) && thrd_MPMC_isEmpty(self.chan)) {
            thrd_wait_Chain_prepend(&self.chan->recv_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_Chain_unlink(&self.chan->recv_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_Chain_unlink(&self.chan->recv_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_MPMC_Rx_recvFor(thrd_MPMC_Rx self, thrd_Wakeable cancel_src, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw) $guard) {
    debug_assert_eqBy($typed(self.chan->type), out.inner_type, TypeInfo_eql);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_MPMC__claimHead(self.chan))(pos)) {
            u_memcpy(out.ref, thrd_MPMC__slotMut(self.chan, out.inner_type, pos).as_const);
            thrd_MPMC__releaseHead(self.chan, pos);
            thrd_MPMC__wakeNotFull(self.chan);
            return_ok(out);
        }
        if (thrd_MPMC_isClosed(self.chan)) {
            return_err(E_cause$thrd_chan_Closed());
        }

        thrd_Mtx_lockProtcd(&self.chan->wait_lock);
        if (!thrd_MPMC_isClosed(self.chan) && thrd_MPMC_isEmpty(self.chan)) {
            let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
            if (time_Dur_isZero(remaining)) {
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(E_cause$Sched_Timeout());
            }
            thrd_wait_Chain_prepend(&self.chan->recv_waiters, &link);
            thrd_Mtx_unlock(&self.chan->wait_lock);
            catch_((thrd_Waiter_waitFor(&waiter, cancel_src, remaining))(err, {
                thrd_Mtx_lockProtcd(&self.chan->wait_lock);
                thrd_wait_Chain_unlink(&self.chan->recv_waiters, &link);
                thrd_Mtx_unlock(&self.chan->wait_lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self.chan->wait_lock);
            thrd_wait_Chain_unlink(&self.chan->recv_waiters, &link);
        }
        thrd_Mtx_unlock(&self.chan->wait_lock);
    }
} $unguarded(fn);

fn_((thrd_MPMC_Rx_recvOp(thrd_MPMC_Rx self, TypeInfo type))(thrd_MPMC_Rx_RecvOp)) {
    debug_assert_eqBy($typed(self.chan->type), type, TypeInfo_eql);
    return (thrd_MPMC_Rx_RecvOp){ .rx = self, .type = type };
};

fn_((thrd_MPMC_Rx_RecvOp_op(thrd_MPMC_Rx_RecvOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self->rx.chan,
        .data = u_anyP(&self->type),
        .result_type = self->type,
        .vtbl = &thrd_MPMC__recv_op_vtbl,
        .src = thrd_Wakeable_ensureValid((thrd_Wakeable){
            .ctx = self->rx.chan,
            .vtbl = &thrd_MPMC__recv_vtbl,
        }),
    });
};

fn_((thrd_MPMC_Tx_sendOp(thrd_MPMC_Tx self, u_P$raw item))(thrd_MPMC_Tx_SendOp)) {
    debug_assert_eqBy($typed(self.chan->type), item.type, TypeInfo_eql);
    return (thrd_MPMC_Tx_SendOp){ .tx = self, .item = item };
};

fn_((thrd_MPMC_Tx_SendOp_op(thrd_MPMC_Tx_SendOp* self))(thrd_Select_Op)) {
    claim_assert_nonnull(self);
    return thrd_Select_Op_ensureValid((thrd_Select_Op){
        .ctx = self->tx.chan,
        .data = self->item,
        .result_type = typeInfo$(Void),
        .vtbl = &thrd_MPMC__send_op_vtbl,
        .src = thrd_Wakeable_ensureValid((thrd_Wakeable){
            .ctx = self->tx.chan,
            .vtbl = &thrd_MPMC__send_vtbl,
        }),
    });
};

fn_((thrd_MPMC_open(thrd_MPMC* self))(thrd_MPMC_Chans)) {
    return (thrd_MPMC_Chans)tie_(
        (thrd_MPMC_tx(self)),
        (thrd_MPMC_rx(self))
    );
};

fn_((thrd_MPMC_close(thrd_MPMC* self))(void)) {
    atom_V_store(&self->closed, true, atom_MemOrd_release);
    thrd_MPMC__broadcast(self);
};

fn_((thrd_MPMC__idx(thrd_MPMC* self, usize pos))(usize)) {
    return mem_ring_idx(self->buf.len, pos);
};

fn_((thrd_MPMC__slotMut(thrd_MPMC* self, TypeInfo type, usize pos))(u_P$raw)) {
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return u_atS(u_from$S((type)(self->buf)), thrd_MPMC__idx(self, pos));
};

fn_((thrd_MPMC__claimTail(thrd_MPMC* self))(O$usize) $scope) {
    var tail = atom_V_load(&self->tail_claim, atom_MemOrd_acquire);
    while (true) {
        let head = atom_V_load(&self->head_release, atom_MemOrd_acquire);
        if (tail - head == self->buf.len) {
            return_none();
        }
        tail = orelse_((atom_V_cmpXchgWeak(
            &self->tail_claim, tail, tail + 1,
            atom_MemOrd_acq_rel, atom_MemOrd_acquire
        ))(return_some(tail)));
    }
} $unscoped(fn);

fn_((thrd_MPMC__publishTail(thrd_MPMC* self, usize pos))(void)) {
    while (atom_V_load(&self->tail_commit, atom_MemOrd_acquire) != pos) {
        atom_spinLoopHint();
    }
    atom_V_store(&self->tail_commit, pos + 1, atom_MemOrd_release);
};

fn_((thrd_MPMC__claimHead(thrd_MPMC* self))(O$usize) $scope) {
    var head = atom_V_load(&self->head_claim, atom_MemOrd_acquire);
    while (true) {
        let tail = atom_V_load(&self->tail_commit, atom_MemOrd_acquire);
        if (head == tail) {
            return_none();
        }
        head = orelse_((atom_V_cmpXchgWeak(
            &self->head_claim, head, head + 1,
            atom_MemOrd_acq_rel, atom_MemOrd_acquire
        ))(return_some(head)));
    }
} $unscoped(fn);

fn_((thrd_MPMC__releaseHead(thrd_MPMC* self, usize pos))(void)) {
    while (atom_V_load(&self->head_release, atom_MemOrd_acquire) != pos) {
        atom_spinLoopHint();
    }
    atom_V_store(&self->head_release, pos + 1, atom_MemOrd_release);
};

fn_((thrd_MPMC__wakeNotEmpty(thrd_MPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_Chain_wakeOne(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPMC__wakeNotFull(thrd_MPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_Chain_wakeOne(&self->send_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPMC__broadcast(thrd_MPMC* self))(void)) {
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_Chain_wakeAll(&self->send_waiters);
    thrd_wait_Chain_wakeAll(&self->recv_waiters);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPMC__recvOpPoll(P$raw ctx))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    return !thrd_MPMC_isEmpty(self);
} $unscoped(fn);

fn_((thrd_MPMC__recvOpCommit(P$raw ctx, u_P$raw data, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    let_ignore = data;
    debug_assert_eqBy($typed(self->type), result.type, TypeInfo_eql);
    return isOk(thrd_MPMC_Rx_tryRecv((thrd_MPMC_Rx){ .chan = self }, (u_V$raw){ .inner = result.raw, .type = result.type }));
} $unscoped(fn);

fn_((thrd_MPMC__sendOpPoll(P$raw ctx))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    return !thrd_MPMC_isFull(self);
} $unscoped(fn);

fn_((thrd_MPMC__sendOpCommit(P$raw ctx, u_P$raw data, u_P$raw result))(bool) $scope) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    let_ignore = result;
    return isOk(thrd_MPMC_Tx_trySend((thrd_MPMC_Tx){ .chan = self }, (u_V$raw){ .inner = data.raw, .type = data.type }));
} $unscoped(fn);

fn_((thrd_MPMC__recvReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    return !thrd_MPMC_isEmpty(self) || thrd_MPMC_isClosed(self);
};

fn_((thrd_MPMC__sendReady(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    return !thrd_MPMC_isFull(self) || thrd_MPMC_isClosed(self);
};

fn_((thrd_MPMC__sendLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_MPMC_isFull(self) || thrd_MPMC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_Chain_prepend(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_MPMC__sendUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_Chain_unlink(&self->send_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};

fn_((thrd_MPMC__recvLink(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    if (!thrd_MPMC_isEmpty(self) || thrd_MPMC_isClosed(self)) {
        thrd_Mtx_unlock(&self->wait_lock);
        return true;
    }
    thrd_wait_Chain_prepend(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
    return false;
};

fn_((thrd_MPMC__recvUnlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrAlignCast$((thrd_MPMC*)(ctx));
    thrd_Mtx_lockProtcd(&self->wait_lock);
    thrd_wait_Chain_unlink(&self->recv_waiters, link);
    thrd_Mtx_unlock(&self->wait_lock);
};
