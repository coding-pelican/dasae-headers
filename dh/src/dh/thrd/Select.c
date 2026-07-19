#include "dh/thrd/Select.h"
#include "dh/time/self/Awake.h"

$static fn_((thrd_Select__linkAll(thrd_Select* self, TypeInfo type, thrd_Waiter* waiter))(void));
$static fn_((thrd_Select__unlinkAll(thrd_Select* self, TypeInfo type))(void));

fn_((thrd_Select_Arm_from(
    TypeInfo type, u_V$raw tag, thrd_Select_Op op, u_V$thrd_Select_Arm$raw ret_mem
))(u_V$thrd_Select_Arm$raw)) {
    let arm_type = thrd_Select_Arm_typeInfo(type);
    ret_mem.type = arm_type;
    claim_assert_nonnull(ret_mem.inner);
    ret_mem.inner->op = thrd_Select_Op_ensureValid(op);
    ret_mem.inner->link = l0$((thrd_wait_Link));
    ret_mem.inner->type = $typing(type);
    ret_mem.inner->commit_offset = offsetTo(thrd_Select_Arm$raw, result_) + mem_alignFwd(tag.type.size, TypeInfo_align(type));
    u_memcpy((u_P$raw){ .raw = as$(P$raw)(&ret_mem.inner->result_), .type = tag.type }, tag.ref.as_const);
    return ret_mem;
};
fn_((thrd_Select_Arm_into(thrd_Select_Arm$raw* self, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(ret_mem.inner);
    u_memcpy(ret_mem.ref, (u_P_const$raw){ .raw = &self->result_, .type = ret_mem.type });
    return ret_mem;
};
fn_((thrd_Select_Arm_take(thrd_Select_Arm$raw* self, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(ret_mem.inner);
    u_memcpy(ret_mem.ref, (u_P_const$raw){ .raw = &self->result_, .type = ret_mem.type });
    mem_set0P((u_P$raw){ .raw = &self->result_, .type = ret_mem.type });
    return ret_mem;
};

fn_((thrd_Select_fixed(TypeInfo type, u_S$thrd_Select_Arm$raw arms))(thrd_Select)) {
    arms.type = thrd_Select_Arm_typeInfo(type);
    return (thrd_Select){
        .arms = ArrList_fixed(arms.as_raw),
        .next_idx = 0,
    };
};
fn_((thrd_Select_init(TypeInfo type, mem_Alctr gpa, usize cap))(mem_E$thrd_Select) $scope) {
    let arm_type = thrd_Select_Arm_typeInfo(type);
    return_ok((thrd_Select){
        .arms = try_(ArrList_init(arm_type, gpa, cap)),
        .next_idx = 0,
    });
} $unscoped(fn);
fn_((thrd_Select_fini(thrd_Select* self, TypeInfo type, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
    ArrList_fini(&self->arms, thrd_Select_Arm_typeInfo(type), gpa);
};
fn_((thrd_Select_from(TypeInfo type, u_S$thrd_Select_Arm$raw arms))(thrd_Select)) {
    arms.type = thrd_Select_Arm_typeInfo(type);
    return (thrd_Select){
        .arms = {
            .items = arms.as_raw.raw,
            .cap = arms.len,
            .type = $typing(arms.type),
        },
        .next_idx = 0,
    };
};

fn_((thrd_Select_watch(thrd_Select* self, mem_Alctr gpa, TypeInfo type, u_V$thrd_Select_Arm$raw arm))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    arm.type = thrd_Select_Arm_typeInfo(type);
    try_(ArrList_append(&self->arms, gpa, arm.as_raw));
    return_ok({});
} $unscoped(fn);
fn_((thrd_Select_watchFixed(thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw arm))(thrd_chan_CapE$void) $scope) {
    claim_assert_nonnull(self);
    arm.type = thrd_Select_Arm_typeInfo(type);
    if (ArrList_isFull(self->arms)) {
        return_err(E_cause$thrd_chan_Full());
    }
    thrd_Select_watchWithin(self, type, arm);
    return_ok({});
} $unscoped(fn);
fn_((thrd_Select_watchWithin(thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw arm))(void)) {
    arm.type = thrd_Select_Arm_typeInfo(type);
    claim_assert_nonnull(self);
    claim_assert_nonnull(arm.inner);
    ArrList_appendWithin(&self->arms, arm.as_raw);
};
fn_((thrd_Select_poll(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw ret_mem
))(O$u_V$thrd_Select_Arm$raw) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(ret_mem.inner);
    ret_mem.type = thrd_Select_Arm_typeInfo(type);

    if_some((thrd_Select_pollMut(self, type))(case_ptr)) {
        let selected = (u_P$raw){
            .raw = case_ptr,
            .type = ret_mem.type,
        };
        return_some(u_asV$((u_V$thrd_Select_Arm$raw)(u_deref(u_memcpy(
            ret_mem.ref, selected.as_const
        )))));
    }
    return_none();
} $unscoped(fn);
fn_((thrd_Select_pollMut(
    thrd_Select* self, TypeInfo type
))(O$P$thrd_Select_Arm$raw) $scope) {
    claim_assert_nonnull(self);
    let arms = ArrList_itemsMut(self->arms, thrd_Select_Arm_typeInfo(type));
    if (arms.len == 0) {
        return_none();
    }
    let start_idx = self->next_idx % arms.len;
    for_(($r(0, arms.len))(offset)) {
        let idx = (start_idx + offset) % arms.len;
        let case_ref = u_atS(arms, idx);
        let case_ptr = as$(thrd_Select_Arm$raw*)(case_ref.raw);
        let result = (u_P$raw){
            .raw = as$(P$raw)(as$(u8*)(case_ptr) + case_ptr->commit_offset),
            .type = case_ptr->op.result_type,
        };
        if (!case_ptr->op.vtbl->pollFn(case_ptr->op.ctx)) {
            continue;
        }
        if (!case_ptr->op.vtbl->commitFn(
                case_ptr->op.ctx,
                case_ptr->op.data,
                result
            )) {
            continue;
        } else {
            self->next_idx = (idx + 1) % arms.len;
            return_some(case_ptr);
        }
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((thrd_Select_wait(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, u_V$thrd_Select_Arm$raw ret_mem
))(Sched_Cancelable$u_V$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_poll(self, type, ret_mem))(done)) { return_ok(done); }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_poll(self, type, ret_mem))(done)) {
            return_ok(done);
        }
        try_(thrd_Waiter_wait(&waiter, cancel_src));
    }
} $unguarded(fn);
fn_((thrd_Select_waitMut(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel
))(Sched_Cancelable$P$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_pollMut(self, type))(done)) { return_ok(done); }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_pollMut(self, type))(done)) {
            return_ok(done);
        }
        try_(thrd_Waiter_wait(&waiter, cancel_src));
    }
} $unguarded(fn);
fn_((thrd_Select_waitFor(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, time_Dur dur, u_V$thrd_Select_Arm$raw ret_mem
))(Sched_TimedE$u_V$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_poll(self, type, ret_mem))(done)) {
        return_ok(done);
    }

    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_poll(self, type, ret_mem))(done)) {
            return_ok(done);
        }

        let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }

        try_(thrd_Waiter_waitFor(&waiter, cancel_src, remaining));
    }
} $unguarded(fn);
fn_((thrd_Select_waitMutFor(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, time_Dur dur
))(Sched_TimedE$P$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_pollMut(self, type))(done)) {
        return_ok(done);
    }

    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let started = time_Awake_now(clock);
    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    let cancel_src = thrd_CancelTok_wakeable(cancel);
    while (true) {
        try_(thrd_CancelTok_check(cancel));
        if_some((thrd_Select_pollMut(self, type))(done)) {
            return_ok(done);
        }

        let remaining = time_Dur_subSat(dur, time_Awake_Inst_elapsed(started, clock));
        if (time_Dur_isZero(remaining)) {
            return_err(E_cause$Sched_Timeout());
        }

        try_(thrd_Waiter_waitFor(&waiter, cancel_src, remaining));
    }
} $unguarded(fn);
fn_((thrd_Select_waitProtcd(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw ret_mem
))(u_V$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_poll(self, type, ret_mem))(done)) {
        return done;
    }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    while (true) {
        if_some((thrd_Select_poll(self, type, ret_mem))(done)) {
            return_(done);
        }
        thrd_Waiter_waitProtcd(&waiter);
    }
} $unguarded(fn);
fn_((thrd_Select_waitMutProtcd(
    thrd_Select* self, TypeInfo type
))(P$thrd_Select_Arm$raw) $guard) {
    if_some((thrd_Select_pollMut(self, type))(done)) {
        return done;
    }

    var waiter = thrd_Waiter_init();
    defer_(thrd_Waiter_fini(&waiter));

    thrd_Select__linkAll(self, type, &waiter);
    defer_(thrd_Select__unlinkAll(self, type));

    while (true) {
        if_some((thrd_Select_pollMut(self, type))(done)) {
            return_(done);
        }
        thrd_Waiter_waitProtcd(&waiter);
    }
} $unguarded(fn);

fn_((thrd_Select__linkAll(thrd_Select* self, TypeInfo type, thrd_Waiter* waiter))(void)) {
    let arms = ArrList_itemsMut(self->arms, thrd_Select_Arm_typeInfo(type));
    for_(($us(arms), $rf(0))(case_ref, i)) {
        let case_ptr = as$(thrd_Select_Arm$raw*)(case_ref.raw);
        case_ptr->link = thrd_Waiter_link(waiter, i);
        let src = thrd_Wakeable_ensureValid(case_ptr->op.src);
        if (thrd_Wakeable_link(src, &case_ptr->link)) {
            thrd_Waiter_wake(waiter);
        }
    } $end(for);
};
fn_((thrd_Select__unlinkAll(thrd_Select* self, TypeInfo type))(void)) {
    let arms = ArrList_itemsMut(self->arms, thrd_Select_Arm_typeInfo(type));
    for_(($us(arms))(case_ref)) {
        let case_ptr = as$(thrd_Select_Arm$raw*)(case_ref.raw);
        let src = thrd_Wakeable_ensureValid(case_ptr->op.src);
        thrd_Wakeable_unlink(src, &case_ptr->link);
    } $end(for);
};
