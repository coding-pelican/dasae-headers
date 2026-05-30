#include "dh/exec/common.h"
#include "dh/exec/Fiber-growable.h"

fn_((exec_callFiber(exec_Fiber_Starter* start, const co_Fiber* first_switch))(void)) {
    claim_assert_nonnull(start), claim_assert_nonnull(first_switch);
    let_ignore = first_switch;
    start->workFn(start->owner, start->task);
    claim_unreachable;
};

fn_((exec_switchToFiber(co_Fiber_Context* sched_context, exec_Fiber* fiber))(void)) {
    claim_assert_nonnull(sched_context), claim_assert_nonnull(fiber);
    let prev = exec_Fiber_enter(fiber);
    exec_Fiber__ensureStackHeadroom(
        fiber,
        co_Fiber_Context_stackPtr(&fiber->context),
        exec_Fiber_stack_switch_headroom
    );
    let_(switch_to, co_Fiber) = {
        .old = sched_context,
        .new = &fiber->context,
    };
    let_ignore = co_Fiber_contextSwitch(&switch_to);
    exec_Fiber_restore(prev);
};

fn_((exec_switchFromFiber(co_Fiber_Context* fiber_context, co_Fiber_Context* sched_context))(void)) {
    claim_assert_nonnull(fiber_context), claim_assert_nonnull(sched_context);
    let_(switch_back, co_Fiber) = {
        .old = fiber_context,
        .new = sched_context,
    };
    let_ignore = co_Fiber_contextSwitch(&switch_back);
};
