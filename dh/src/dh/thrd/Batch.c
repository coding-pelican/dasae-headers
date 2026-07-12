#include "dh/thrd/Batch.h"
#include "dh/meta.h"

$static fn_((thrd_Batch__wake(thrd_Batch* self))(void));
$static fn_((thrd_Batch__finish(thrd_Batch* self))(void));
$static fn_((thrd_Batch__drainReady(thrd_Batch* self, TypeInfo done_type))(void));
$static fn_((thrd_Batch__entry(thrd_Batch* self, thrd_Batch_Id id, TypeInfo result_type, Clsr$raw* clsr))(Void));
fn_use_Clsr_((thrd_Batch__entry)(thrd_Batch*, thrd_Batch_Id, TypeInfo, Clsr$raw*)(Void));

fn_((thrd_Batch_init(u_S$raw done_buf))(thrd_Batch)) {
    return (thrd_Batch){
        .group = thrd_Group_init(),
        .done = thrd_Que_init(done_buf),
        .next_id = atom_V_init(0),
        .type = $typing(done_buf.type),
        .lock = thrd_Mtx_init(),
        .waiters = thrd_wait_List_init(),
    };
};
fn_((thrd_Batch_fini(thrd_Batch* self, TypeInfo done_type))(void)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), done_type, TypeInfo_eql);
    thrd_Batch_waitProtcd(self, done_type);
    thrd_Mtx_fini(&self->lock);
    thrd_Que_fini(&self->done);
    thrd_Group_fini(&self->group);
};

fn_((thrd_Batch_spawn(thrd_Batch* self, TypeInfo result_type, mem_Alctr gpa, Clsr$raw* clsr))(thrd_spawn_E$thrd_Batch_Id) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(clsr);
    debug_assert_eqBy($typed(self->type), thrd_Batch_Done_typeInfo(result_type), TypeInfo_eql);

    let id = atom_V_pri_fetchAdd(&self->next_id, 1, atom_MemOrd_acq_rel);
    let thrd_clsr = u_castP$((Clsr_(thrd_Batch__entry)*)(try_(mem_Alctr_create($trace gpa, typeInfo$(Clsr_(thrd_Batch__entry))))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(thrd_clsr)));
    *thrd_clsr = clsr_((thrd_Batch__entry)(self, id, result_type, clsr));
    let join_node = u_castP$((thrd_Group_Node*)(try_(mem_Alctr_create($trace gpa, typeInfo$(thrd_Group_Node)))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(join_node)));
    *join_node = (thrd_Group_Node){
        .next = none$((O$P$thrd_Group_Node)),
        .gpa = mem_Alctr_ensureValid(gpa),
        .self = cleared(),
    };

    thrd_Group_start(&self->group);
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

fn_((thrd_Batch_tryNext(thrd_Batch* self, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw) $scope) {
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
fn_((thrd_Batch_next(thrd_Batch* self, thrd_CancelTok cancel, u_V$thrd_Batch_Done$raw ret_mem))(Sched_Cancelable$O$u_V$thrd_Batch_Done$raw) $guard) {
    debug_assert_eqBy($typed(self->type), ret_mem.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    let cancel_src = thrd_CancelTok_waitSrc(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Batch_tryNext(self, ret_mem))(done)) {
            return_ok(some(done));
        }
        if (thrd_Group_isDone(&self->group)) {
            return_ok(none());
        }

        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_List_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self->lock);
                thrd_wait_List_unlink(&self->waiters, &link);
                thrd_Mtx_unlock(&self->lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);
fn_((thrd_Batch_nextProtcd(thrd_Batch* self, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw) $guard) {
    debug_assert_eqBy($typed(self->type), ret_mem.inner_type, TypeInfo_eql);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (true) {
        if_some((thrd_Batch_tryNext(self, ret_mem))(done)) {
            return_some(done);
        }
        if (thrd_Group_isDone(&self->group)) {
            return_none();
        }

        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_List_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            thrd_Waiter_waitProtcd(&waiter);
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
} $unguarded(fn);
fn_((thrd_Batch_wait(thrd_Batch* self, TypeInfo done_type, thrd_CancelTok cancel))(Sched_Cancelable$void) $guard) {
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    let cancel_src = thrd_CancelTok_waitSrc(cancel);
    while (!thrd_Group_isDone(&self->group)) {
        try_(thrd_CancelTok_check(cancel));
        thrd_Batch__drainReady(self, done_type);
        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_List_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            catch_((thrd_Waiter_wait(&waiter, cancel_src))(err, {
                thrd_Mtx_lockProtcd(&self->lock);
                thrd_wait_List_unlink(&self->waiters, &link);
                thrd_Mtx_unlock(&self->lock);
                return_err(err);
            }));
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
    thrd_Batch__drainReady(self, done_type);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Batch_waitProtcd(thrd_Batch* self, TypeInfo done_type))(void) $guard) {
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));
    var link = thrd_Waiter_link(&waiter, 0);
    while (!thrd_Group_isDone(&self->group)) {
        thrd_Batch__drainReady(self, done_type);
        thrd_Mtx_lockProtcd(&self->lock);
        if (thrd_Que_len(&self->done) == 0 && !thrd_Group_isDone(&self->group)) {
            thrd_wait_List_prepend(&self->waiters, &link);
            thrd_Mtx_unlock(&self->lock);
            thrd_Waiter_waitProtcd(&waiter);
            thrd_Mtx_lockProtcd(&self->lock);
            thrd_wait_List_unlink(&self->waiters, &link);
        }
        thrd_Mtx_unlock(&self->lock);
    }
    thrd_Batch__drainReady(self, done_type);
    return_void();
} $unguarded(fn);

fn_((thrd_Batch__wake(thrd_Batch* self))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    thrd_wait_List_wakeAll(&self->waiters);
    thrd_Mtx_unlock(&self->lock);
};
fn_((thrd_Batch__finish(thrd_Batch* self))(void)) {
    thrd_Group_finish(&self->group);
    thrd_Batch__wake(self);
};
fn_((thrd_Batch__drainReady(thrd_Batch* self, TypeInfo done_type))(void)) {
    while_some((thrd_Batch_tryNext(self, u_asV$((u_V$thrd_Batch_Done$raw)(u_allocV(done_type))))), ignored) {
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
