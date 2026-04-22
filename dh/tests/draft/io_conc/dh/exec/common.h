#ifndef exec_common__included
#define exec_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl.h"
#include "dh/mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Task_Kind)(enum_((exec_Task_Kind $fits($packed))(
    exec_Task_Kind_stackless = 0,
    exec_Task_Kind_fiber,
))));
claim_assert_static(eqlType$(exec_Task_Kind, u8));
$attr($inline_always)
$static fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind));
$attr($inline_always)
$static fn_((exec_invokeStep(P$$(Closure$raw) closure, u_P$raw result))(bool));
$attr($inline_always)
$static fn_((exec_invokeToCompletion(P$$(Closure$raw) closure, u_P$raw result))(void));

T_alias$((exec_Fiber)(struct exec_Fiber {
    var_(stk, S$u8);
    var_(ctx, Co_Fiber_Context);
}));
T_use_prl$(exec_Fiber);
T_use_E$($set(mem_E)(P$exec_Fiber));
#define exec_Fiber_stk_size (usize_(1) * 1024 * 1024)
T_alias$((exec_Fiber_WorkFn)(fn_(((*)(P$raw owner, P$raw task))(void) $T)));
T_alias$((exec_Fiber_Starter)(struct exec_Fiber_Starter {
    var_(owner, P$raw);
    var_(task, P$raw);
    var_(workFn, exec_Fiber_WorkFn);
}));

$attr($no_return)
$extern fn_((exec_callFiber(exec_Fiber_Starter* starter, const Co_Fiber* first_switch))(void));
$attr($must_check)
$extern fn_((exec_createFiber(mem_Alctr gpa, P$raw owner, P$raw task, exec_Fiber_WorkFn workFn))(mem_E$P$exec_Fiber));
$extern fn_((exec_destroyFiber(mem_Alctr gpa, exec_Fiber* fiber))(void));
$extern fn_((exec_switchToFiber(Co_Fiber_Context* sched_ctx, exec_Fiber* fiber))(void));
$extern fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_ctx, Co_Fiber_Context* sched_ctx))(void));

/*========== Macros and Definitions =========================================*/

fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind)) {
    claim_assert_nonnull(closure), claim_assert(closure->kind != Closure_Kind_undefined);
    return closure->kind == Closure_Kind_co
             ? exec_Task_Kind_stackless
             : exec_Task_Kind_fiber;
};

fn_((exec_invokeStep(P$$(Closure$raw) closure, u_P$raw result))(bool)) {
    claim_assert_nonnull(closure), claim_assert_nonnull(result.raw);
    let ret = P_meta((result.type)(as$(P$raw)(invoke_(closure)->ret_))).as_const;
    if (exec_kind(closure) == exec_Task_Kind_stackless) {
        let frame = ptrAlignCast$((Co_Frame$raw*)(closure->ctx_));
        if (frame->ctx_->ctrl.state != Co_State_ready) return false;
    }
    u_memcpy(result, ret);
    return true;
};

fn_((exec_invokeToCompletion(P$$(Closure$raw) closure, u_P$raw result))(void)) {
    claim_assert_nonnull(closure), claim_assert_nonnull(result.raw);
    while (!exec_invokeStep(closure, result)) $do_nothing;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_common__included */
