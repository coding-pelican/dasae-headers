#include "common.h"

fn_((exec_callFiber(exec_Fiber_Starter* start, const Co_Fiber* first_switch))(void)) {
    claim_assert_nonnull(start), claim_assert_nonnull(first_switch);
    let_ignore = first_switch;
    start->workFn(start->owner, start->task);
    claim_unreachable;
};

fn_((exec_switchToFiber(Co_Fiber_Context* sched_ctx, exec_Fiber* fiber))(void)) {
    claim_assert_nonnull(sched_ctx), claim_assert_nonnull(fiber);
    let_(switch_to, Co_Fiber) = {
        .old = sched_ctx,
        .new = &fiber->ctx,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_to);
};

fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_ctx, Co_Fiber_Context* sched_ctx))(void)) {
    claim_assert_nonnull(fiber_ctx), claim_assert_nonnull(sched_ctx);
    let_(switch_back, Co_Fiber) = {
        .old = fiber_ctx,
        .new = sched_ctx,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_back);
};
