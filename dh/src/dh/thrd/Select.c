#include "dh/thrd/Select.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_Select__linkAll(thrd_Select* self, thrd_Waiter* waiter))(void));
$static fn_((thrd_Select__unlinkAll(thrd_Select* self))(void));

fn_((thrd_Select_init(TypeInfo result_ty, S$thrd_Select_Case cases))(thrd_Select)) {
    return (thrd_Select){
        .cases = cases,
        .len = 0,
        .result_type = $typing(result_ty),
    };
}

fn_((thrd_Select_watch(thrd_Select* self, usize field_idx, thrd_Select_Op op, u_P$raw result))(void)) {
    claim_assert_nonnull(self);
    claim_assert(self->len < self->cases.len);
    debug_assert_eqBy($typed(self->result_type), result.type, TypeInfo_eql);
    self->cases.ptr[self->len] = (thrd_Select_Case){
        .field_idx = field_idx,
        .op = op,
        .result = result,
        .link = cleared(),
    };
    self->len += 1;
}
fn_((thrd_Select_poll(thrd_Select* self, u_V$thrd_Select_Done$raw ret_mem))(O$u_V$thrd_Select_Done$raw) $scope) {
    claim_assert_nonnull(self);
    let result_type = $typed(self->result_type);
    debug_assert_eqBy($typed(self->result_type), result_type, TypeInfo_eql);
    debug_assert_eqBy(ret_mem.type, thrd_Select_Done_typeInfo(result_type), TypeInfo_eql);

    for_(($r(0, self->len))(i)) {
        let case_ptr = &self->cases.ptr[i];
        if (!case_ptr->op.vtbl->pollFn(case_ptr->op.ctx, case_ptr->result)) {
            continue;
        } else {
            let done = as$(thrd_Select_Done$raw*)(ret_mem.inner);
            done->field_idx = case_ptr->field_idx;
            done->type = $typing(result_type);
            let result = thrd_Select_Done_resultMut(done, result_type);
            if (result.raw != case_ptr->result.raw) {
                u_memcpy(result, case_ptr->result.as_const);
            }
            return_some({
                .inner = done,
                .inner_type = ret_mem.inner_type,
            });
        }
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((thrd_Select_wait(thrd_Select* self, thrd_CancelTok cancel, u_V$thrd_Select_Done$raw ret_mem))(Sched_Cancelable$u_V$thrd_Select_Done$raw) $guard) {
    if_some((thrd_Select_poll(self, ret_mem))(done)) {
        return_ok(done);
    }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, &waiter);
    defer_(thrd_Select__unlinkAll(self));

    let cancel_src = thrd_CancelTok_waitSrc(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_poll(self, ret_mem))(done)) {
            return_ok(done);
        }
        try_(thrd_Waiter_wait(&waiter, cancel_src));
    }
} $unguarded(fn);
fn_((thrd_Select_waitFor(thrd_Select* self, thrd_CancelTok cancel, time_Dur dur, u_V$thrd_Select_Done$raw ret_mem))(Sched_TimedE$u_V$thrd_Select_Done$raw) $guard) {
    if_some((thrd_Select_poll(self, ret_mem))(done)) {
        return_ok(done);
    }

    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, &waiter);
    defer_(thrd_Select__unlinkAll(self));

    let cancel_src = thrd_CancelTok_waitSrc(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_poll(self, ret_mem))(done)) {
            return_ok(done);
        }

        let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }

        let wait_remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(wait_remaining)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(thrd_Waiter_waitFor(&waiter, cancel_src, wait_remaining));
    }
} $unguarded(fn);
fn_((thrd_Select_waitProtcd(thrd_Select* self, u_V$thrd_Select_Done$raw ret_mem))(u_V$thrd_Select_Done$raw) $guard) {
    if_some((thrd_Select_poll(self, ret_mem))(done)) {
        return done;
    }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, &waiter);
    defer_(thrd_Select__unlinkAll(self));

    while (true) {
        if_some((thrd_Select_poll(self, ret_mem))(done)) {
            return_(done);
        }
        thrd_Waiter_waitProtcd(&waiter);
    }
} $unguarded(fn);

fn_((thrd_Select__linkAll(thrd_Select* self, thrd_Waiter* waiter))(void)) {
    for_(($r(0, self->len))(i)) {
        let case_ptr = &self->cases.ptr[i];
        case_ptr->link = thrd_Waiter_link(waiter, i);
        let src = thrd_wait_Src_ensureValid(case_ptr->op.src);
        if (src.vtbl->linkFn(src.ctx, &case_ptr->link)) {
            thrd_Waiter_wake(waiter);
        }
    } $end(for);
};
fn_((thrd_Select__unlinkAll(thrd_Select* self))(void)) {
    for_(($r(0, self->len))(i)) {
        let case_ptr = &self->cases.ptr[i];
        let src = thrd_wait_Src_ensureValid(case_ptr->op.src);
        src.vtbl->unlinkFn(src.ctx, &case_ptr->link);
    } $end(for);
};
