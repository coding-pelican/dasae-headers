#include "dh/thrd/Batch.h"
#include "dh/meta.h"

$static fn_((thrd_Batch__wake(thrd_Batch* self))(void));
$static fn_((thrd_Batch__finish(thrd_Batch* self))(void));
$static fn_((thrd_Batch__drainReady(thrd_Batch* self, TypeInfo result_type))(void));
$static fn_((thrd_Batch__entry(thrd_Batch* self, thrd_Batch_Id id, TypeInfo result_type, Clsr$raw* clsr))(Void));
fn_use_Clsr_((thrd_Batch__entry)(thrd_Batch*, thrd_Batch_Id, TypeInfo, Clsr$raw*)(Void));

fn_((thrd_Batch_Done_from(thrd_Batch_Id id, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(u_V$thrd_Batch_Done$raw)) {
    ret_mem.type = thrd_Batch_Done_typeInfo(result_type);
    claim_assert_nonnull(ret_mem.inner);
    ret_mem.inner->id = id;
    ret_mem.inner->type = $typing(result_type);
    mem_set0P(thrd_Batch_Done_resultMut(ret_mem.inner, result_type));
    return ret_mem;
};
fn_((thrd_Batch_Done_into(const thrd_Batch_Done$raw* self, TypeInfo result_type, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(ret_mem.inner);
    debug_assert_eqBy(ret_mem.type, result_type, TypeInfo_eql);
    u_memcpy(ret_mem.ref, thrd_Batch_Done_result(self, result_type));
    return ret_mem;
};
fn_((thrd_Batch_Done_take(thrd_Batch_Done$raw* self, TypeInfo result_type, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(ret_mem.inner);
    debug_assert_eqBy(ret_mem.type, result_type, TypeInfo_eql);
    u_memcpy(ret_mem.ref, thrd_Batch_Done_result(self, result_type));
    mem_set0P(thrd_Batch_Done_resultMut(self, result_type));
    return ret_mem;
};

fn_((thrd_Batch_init(TypeInfo result_type, u_S$raw done_buf))(thrd_Batch)) {
    done_buf.type = thrd_Batch_Done_typeInfo(result_type);
    return (thrd_Batch){
        .group = thrd_Group_init(),
        .done = thrd_Que_init(done_buf),
        .next_id = atom_V_init(0),
        .type = $typing(done_buf.type),
        .lock = thrd_Mtx_init(),
        .waiters = thrd_wait_Chain_init(),
    };
};
fn_((thrd_Batch_fini(thrd_Batch* self, TypeInfo result_type))(void)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), thrd_Batch_Done_typeInfo(result_type), TypeInfo_eql);
    thrd_Batch_waitProtcd(self, result_type);
    thrd_Mtx_fini(&self->lock);
    thrd_Que_fini(&self->done);
    thrd_Group_fini(&self->group);
};

fn_((thrd_Batch_spawn(thrd_Batch* self, TypeInfo result_type, mem_Alctr gpa, Clsr$raw* clsr))(thrd_Batch_SpawnE$thrd_Batch_Id) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(clsr);
    debug_assert_eqBy($typed(self->type), thrd_Batch_Done_typeInfo(result_type), TypeInfo_eql);

    thrd_Mtx_lockProtcd(&self->lock);
    let done_len = thrd_Que_len(&self->done);
    let done_cap = thrd_Que_cap(&self->done);
    let pending_len = thrd_Group_value(&self->group);
    if (done_len >= done_cap || pending_len >= done_cap - done_len) {
        thrd_Mtx_unlock(&self->lock);
        return_err(E_cause$thrd_chan_Full());
    }
    thrd_Group_start(&self->group);
    thrd_Mtx_unlock(&self->lock);

    let id = atom_V_pri_fetchAdd(&self->next_id, 1, atom_MemOrd_acq_rel);
    let thrd_clsr = u_castP$((Clsr_(thrd_Batch__entry)*)(catch_((mem_Alctr_create($trace gpa, typeInfo$(Clsr_(thrd_Batch__entry))))(err, {
        thrd_Group_finish(&self->group);
        thrd_Batch__wake(self);
        return_err(err);
    }))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(thrd_clsr)));
    *thrd_clsr = clsr_((thrd_Batch__entry)(self, id, result_type, clsr));
    let join_node = u_castP$((thrd_Group_Node*)(catch_((mem_Alctr_create($trace gpa, typeInfo$(thrd_Group_Node)))(err, {
        thrd_Group_finish(&self->group);
        thrd_Batch__wake(self);
        return_err(err);
    }))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(join_node)));
    *join_node = (thrd_Group_Node){
        .next = none$((O$P$thrd_Group_Node)),
        .gpa = mem_Alctr_ensureValid(gpa),
        .self = cleared(),
    };

    let instance = catch_((thrd_spawnOwned(
        thrd_SpawnCfg_default(gpa),
        thrd_clsr->as_raw,
        typeInfo$(Void),
        u_anyP(thrd_clsr)
    ))(err, {
        thrd_Group_finish(&self->group);
        thrd_Batch__wake(self);
        return_err(err);
    }));
    thrd_Group_adopt(&self->group, join_node, instance);
    thrd_Batch__wake(self);
    return_ok(id);
} $unguarded(fn);

fn_((thrd_Batch_tryNext(thrd_Batch* self, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw) $scope) {
    ret_mem.type = thrd_Batch_Done_typeInfo(result_type);
    debug_assert_eqBy($typed(self->type), ret_mem.inner_type, TypeInfo_eql);
    let done = catch_((thrd_Que_tryRecv(&self->done, ret_mem.as_raw))(err, {
        let_ignore = err;
        return_none();
    }));
    return_some({
        .inner = as$(thrd_Batch_Done$raw*)(done.inner),
        .inner_type = done.inner_type,
    });
} $unscoped(fn);
fn_((thrd_Batch_next(thrd_Batch* self, TypeInfo result_type, thrd_CancelTok cancel, u_V$thrd_Batch_Done$raw ret_mem))(Sched_Cancelable$O$u_V$thrd_Batch_Done$raw) $guard) {
    ret_mem.type = thrd_Batch_Done_typeInfo(result_type);
    debug_assert_eqBy($typed(self->type), ret_mem.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Batch_tryNext(self, result_type, ret_mem))(done)) {
            return_ok(some(done));
        }
        if (thrd_Group_isDone(&self->group)) {
            return_ok(none());
        }

        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_Chain_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self->lock);
                thrd_wait_Chain_unlink(&self->waiters, &link);
                thrd_Mtx_unlock(&self->lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_Chain_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);
fn_((thrd_Batch_nextProtcd(thrd_Batch* self, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw) $guard) {
    ret_mem.type = thrd_Batch_Done_typeInfo(result_type);
    debug_assert_eqBy($typed(self->type), ret_mem.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_Batch_tryNext(self, result_type, ret_mem))(done)) {
            return_some(done);
        }
        if (thrd_Group_isDone(&self->group)) {
            return_none();
        }

        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_Chain_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            thrd_Waiter_waitProtcd(&waiter);
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_Chain_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);
fn_((thrd_Batch_wait(thrd_Batch* self, TypeInfo result_type, thrd_CancelTok cancel))(Sched_Cancelable$void) $guard) {
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (!thrd_Group_isDone(&self->group)) {
        try_(thrd_CancelTok_check(cancel));
        thrd_Batch__drainReady(self, result_type);
        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_Chain_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self->lock);
                thrd_wait_Chain_unlink(&self->waiters, &link);
                thrd_Mtx_unlock(&self->lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_Chain_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
    thrd_Batch__drainReady(self, result_type);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Batch_waitProtcd(thrd_Batch* self, TypeInfo result_type))(void) $guard) {
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (!thrd_Group_isDone(&self->group)) {
        thrd_Batch__drainReady(self, result_type);
        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_Chain_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            thrd_Waiter_waitProtcd(&waiter);
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_Chain_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
    thrd_Batch__drainReady(self, result_type);
    return_void();
} $unguarded(fn);

fn_((thrd_Batch__wake(thrd_Batch* self))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_Chain_wakeAll(&self->waiters);
    thrd_Mtx_unlock(&self->lock);
};
fn_((thrd_Batch__finish(thrd_Batch* self))(void)) {
    thrd_Group_finish(&self->group);
    thrd_Batch__wake(self);
};
fn_((thrd_Batch__drainReady(thrd_Batch* self, TypeInfo result_type))(void)) {
    while_some((thrd_Batch_tryNext(self, result_type, u_asV$((u_V$thrd_Batch_Done$raw)(u_allocV(thrd_Batch_Done_typeInfo(result_type)))))), ignored) {
        let_ignore = ignored;
    };
};
fn_((thrd_Batch__entry(thrd_Batch* self, thrd_Batch_Id id, TypeInfo result_type, Clsr$raw* clsr))(Void) $guard) {
    defer_(thrd_Batch__finish(self));
    let done_type = thrd_Batch_Done_typeInfo(result_type);
    debug_assert_eqBy($typed(self->type), done_type, TypeInfo_eql);

    let done = u_allocV(done_type);
    let done_ptr = as$(thrd_Batch_Done$raw*)(done.inner);
    done_ptr->id = id;
    done_ptr->type = $typing(result_type);
    u_memcpy(thrd_Batch_Done_resultMut(done_ptr, result_type), clsr_invokeToComplete(clsr, result_type));

    catch_((thrd_Que_trySend(&self->done, done))(err, {
        let_ignore = err;
        claim_unreachable;
    }));
    thrd_Batch__wake(self);
    return_void();
} $unguarded(fn);
