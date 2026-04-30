#ifndef exec_common__included
#define exec_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../Closure.h"
#include "../Future.h"
#include "../Sched.h"

/*========== Macros and Declarations ========================================*/

$attr($no_return)
$extern fn_((exec_callFiber(exec_Fiber_Starter* starter, const Co_Fiber* first_switch))(void));
$extern fn_((exec_switchToFiber(Co_Fiber_Context* sched_context, exec_Fiber* fiber))(void));
$extern fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_context, Co_Fiber_Context* sched_context))(void));

$attr($inline_always)
$static fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind));
#define T_use_exec_kind$(_T...) __stmt__T_use_exec_kind$(_T)

/*========== Macros and Definitions =========================================*/

#include "dh/meta.h"

fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind)) {
    claim_assert_nonnull(closure), claim_assert(closure->kind != Closure_Kind_undefined);
    return closure->kind == Closure_Kind_co
             ? exec_Task_Kind_stackless
             : exec_Task_Kind_fiber;
};


#define __stmt__T_use_exec_kind$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(exec_kind, _T)(P_const$$(Closure$(_T)) closure))(exec_Task_Kind)) { \
        return exec_kind(closure->as_raw); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_common__included */
