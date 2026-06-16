#pragma once
#ifndef exec_Task__included
#define exec_Task__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Fiber.h"
#include "../Future/base.h"
#include "../Sched/base.h"
#include "../clsr.h"
#include "../ArrList.h"
#include "../ArrQue.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Task_Kind)(enum_((exec_Task_Kind $fits($packed))(
    exec_Task_Kind_stackless = 0,
    exec_Task_Kind_stackful
))));
claim_assert_static(eqlType$(exec_Task_Kind, u8));

T_alias$((exec_Task_State)(enum_((exec_Task_State $fits($packed))(
    exec_Task_State_pending = 0,
    exec_Task_State_deferred,
    exec_Task_State_ready,
    exec_Task_State_running,
    exec_Task_State_waiting,
    exec_Task_State_done,
    /// Used by `exec_Preem` only; lane tasks use `exec_Task_Cancel` instead.
    exec_Task_State_canceled,
))));
claim_assert_static(eqlType$(exec_Task_State, u8));

T_alias$((exec_Task_Cancel)(enum_((exec_Task_Cancel $fits($packed))(
    exec_Task_Cancel_none = 0,
    exec_Task_Cancel_requested,
    exec_Task_Cancel_acknowledged,
))));
claim_assert_static(eqlType$(exec_Task_Cancel, u8));

T_alias$((exec_Task)(union exec_Task {
    T_embed$(struct {
        var_(state, exec_Task_State);
        var_(cancel, exec_Task_Cancel);
        var_(cancel_protection, Sched_CancelProtn);
        var_(result, u_P$raw);
        var_(inner, P$$(Clsr$raw));
        var_(fiber, O$P$exec_Fiber);
    });
    var_(as_any, FutureAny) $flexible;
}));
T_use_prl$(exec_Task);
T_use_ArrList$(P$exec_Task);
T_use_ArrQue$(P$exec_Task);

/* --- Clsr mapping --- */

$attr($inline_always)
$static fn_((exec_Task_kind(P_const$$(Clsr$raw) clsr))(exec_Task_Kind));
#define T_use_exec_Task_kind$(_T...) __stmt__T_use_exec_Task_kind$(_T)

/* --- Completion --- */

$extern fn_((exec_Task_isDone(P_const$exec_Task task))(bool));
$extern fn_((exec_Task_copyToResult(P_const$exec_Task task, u_P$raw result))(void));

/* --- Cooperative cancel --- */

$extern fn_((exec_Task_hasCancelRequest(P_const$exec_Task task))(bool));
$extern fn_((exec_Task_requestCancel(exec_Task* task))(void));
$extern fn_((exec_Task_recancel(exec_Task* task))(void));
$extern fn_((exec_Task_swapCancelProtn(exec_Task* task, Sched_CancelProtn new_protection))(Sched_CancelProtn));
/// May return `Sched_Canceled` and consume one outstanding request (`exec_Task_idle`).
$attr($must_check)
$extern fn_((exec_Task_idle(exec_Task* task))(Sched_Cancelable$void));

/*========== Macros and Definitions =========================================*/

#include "../meta.h"

fn_((exec_Task_kind(P_const$$(Clsr$raw) clsr))(exec_Task_Kind)) {
    claim_assert_nonnull(clsr), claim_assert(clsr->kind != Clsr_Kind_undefined);
    return clsr->kind == Clsr_Kind_co
             ? exec_Task_Kind_stackless
             : exec_Task_Kind_stackful;
};

#define __stmt__T_use_exec_Task_kind$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(exec_Task_kind, _T)(P_const$$(Clsr$(_T)) clsr))(exec_Task_Kind)) { \
        return exec_Task_kind(clsr->as_raw); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Task__included */
