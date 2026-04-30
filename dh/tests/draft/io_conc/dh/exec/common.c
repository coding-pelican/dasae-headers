#include "common.h"

fn_((exec_callFiber(exec_Fiber_Starter* start, const Co_Fiber* first_switch))(void)) {
    claim_assert_nonnull(start), claim_assert_nonnull(first_switch);
    let_ignore = first_switch;
    start->workFn(start->owner, start->task);
    claim_unreachable;
};

fn_((exec_switchToFiber(Co_Fiber_Context* sched_context, exec_Fiber* fiber))(void)) {
    claim_assert_nonnull(sched_context), claim_assert_nonnull(fiber);
    let prev = exec_Fiber_enter(fiber);
    let_(switch_to, Co_Fiber) = {
        .old = sched_context,
        .new = &fiber->context,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_to);
    exec_Fiber_restore(prev);
};

fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_context, Co_Fiber_Context* sched_context))(void)) {
    claim_assert_nonnull(fiber_context), claim_assert_nonnull(sched_context);
    let_(switch_back, Co_Fiber) = {
        .old = fiber_context,
        .new = sched_context,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_back);
};
