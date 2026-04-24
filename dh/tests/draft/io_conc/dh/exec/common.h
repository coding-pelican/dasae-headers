#ifndef exec_common__included
#define exec_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../Future.h"
#include "../Sched.h"

/*========== Macros and Declarations ========================================*/

$attr($no_return)
$extern fn_((exec_callFiber(exec_Fiber_Starter* starter, const Co_Fiber* first_switch))(void));
$extern fn_((exec_switchToFiber(Co_Fiber_Context* sched_ctx, exec_Fiber* fiber))(void));
$extern fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_ctx, Co_Fiber_Context* sched_ctx))(void));

$attr($inline_always)
$static fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind));
#define T_use_exec_kind$(_T...) __stmt__T_use_exec_kind$(_T)
$attr($inline_always)
$static fn_((exec_invokeToStep(P$$(Closure$raw) closure, TypeInfo type))(O$u_P_const$raw));
#define T_use_exec_invokeToStep$(_T...) __stmt__T_use_exec_invokeToStep$(_T)
$attr($inline_always)
$static fn_((exec_invokeToCompletion(P$$(Closure$raw) closure, TypeInfo type))(u_P_const$raw));
#define T_use_exec_invokeToCompletion$(_T...) __stmt__T_use_exec_invokeToCompletion$(_T)

/*========== Macros and Definitions =========================================*/

#include "dh/meta.h"

fn_((exec_kind(P_const$$(Closure$raw) closure))(exec_Task_Kind)) {
    claim_assert_nonnull(closure), claim_assert(closure->kind != Closure_Kind_undefined);
    return closure->kind == Closure_Kind_co
             ? exec_Task_Kind_stackless
             : exec_Task_Kind_fiber;
};
fn_((exec_invokeToStep(P$$(Closure$raw) closure, TypeInfo type))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(closure);
    let ret = P_meta((type)(as$(P$raw)(invoke_(closure)->ret_))).as_const;
    if (exec_kind(closure) == exec_Task_Kind_stackless) {
        let frame = local_({
            let fields = typeInfosFrom(type, typeInfo$(Co_Frame$raw));
            let record_type = u_typeInfoRecord(fields);
            let record = P_meta((record_type)(as$(P$raw)(closure->ctx_))).as_const;
            local_return_(u_castP$((const Co_Frame$raw*)(u_fieldPtr(record, fields, 1))));
        });
        if (frame->ctx_->ctrl.state != Co_State_ready) return_none();
    }
    return_some(ret);
} $unscoped(fn);
fn_((exec_invokeToCompletion(P$$(Closure$raw) closure, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(closure);
    var_(step, O$u_P_const$raw) = none();
    while_none(step = exec_invokeToStep(closure, type)) $do_nothing;
    return unwrap_(step);
};

#define __stmt__T_use_exec_kind$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id$T(exec_kind, _T)(P_const$$(Closure$(_T)) closure))(exec_Task_Kind)) { \
        return exec_kind(closure->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_exec_invokeToStep$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id$T(exec_invokeToStep, _T)(P$$(Closure$(_T)) closure))(O$(P_const$(_T))) $scope) { \
        if_some((exec_invokeToStep(closure->as_raw, typeInfo$(_T)))(ret)) { \
            return_some(u_castP$((const _T*)(ret))); \
        } else_none { \
            return_none(); \
        } \
        claim_unreachable; \
    } /* clang-format on */
#define __stmt__T_use_exec_invokeToCompletion$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id$T(exec_invokeToCompletion, _T)(P$$(Closure$(_T)) closure))(P_const$(_T))) { \
        return u_castP$((const _T*)(exec_invokeToCompletion(closure->as_raw, typeInfo$(_T)))); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_common__included */
