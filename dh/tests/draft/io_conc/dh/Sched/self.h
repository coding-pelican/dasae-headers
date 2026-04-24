#ifndef Sched_self__included
#define Sched_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../exec/base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Sched_VTbl)(struct Sched_VTbl {
    /// Split a call from its result storage and use available asynchronous
    /// progress. If that is unavailable, the implementation may complete the
    /// call eagerly or defer it to `await`.
    ///
    /// If it returns `none`, `result` has already been populated and `await`
    /// will be a no-op.
    /// Thread-safe.
    fn_(((*asyncFn)(
        /// Corresponds to `Sched.ctx`.
        P$raw ctx,
        /// Points to the caller-owned `Future.result_` field.
        /// The pointer remains valid for the lifetime of that Future value.
        u_P$raw result,
        /// Borrowed closure state. The implementation must not copy it.
        P$$(Closure$raw) inner
    ))(O$P$FutureAny));

    /// Request a concurrency unit. Failure is reported instead of falling back
    /// to eager or await-time execution.
    /// Thread-safe.
    $attr($must_check)
    fn_(((*spawnFn)(
        P$raw ctx,
        u_P$raw result,
        P$$(Closure$raw) inner
    ))(Sched_ConcE$P$FutureAny));

    /// This function is only called when `async` or `spawn` returns `some`.
    ///
    /// Thread-safe.
    fn_(((*awaitFn)(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));

    /// Equivalent to `await` but initiates cancel request.
    ///
    /// This function is only called when `async` or `spawn` returns `some`.
    ///
    /// Thread-safe.
    fn_(((*cancelFn)(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
}));
$extern fn_((Sched_VTbl_noAsync(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$FutureAny));
$attr($must_check)
$extern fn_((Sched_VTbl_noSpawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$FutureAny));
$extern fn_((Sched_VTbl_noAwait(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$extern fn_((Sched_VTbl_noCancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));

struct Sched {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(Sched_VTbl));
};
$extern fn_((Sched_async(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(V$Future$raw));
#define T_use_Sched_async$(_T...) __stmt__T_use_Sched_async$(_T)
$attr($must_check)
$extern fn_((Sched_spawn(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw));
#define T_use_Sched_spawn$(_T...) __stmt__T_use_Sched_spawn$(_T)

$extern fn_((Sched_seq(exec_Seq* self))(Sched));
$extern fn_((Sched_coop(exec_Coop* self))(Sched));
$extern fn_((Sched_para(exec_Para* self))(Sched));
$extern fn_((Sched_preem(exec_Preem* self))(Sched));

T_alias$((Sched_Group)(struct Sched_Group{}));
T_alias$((Sched_Select)(struct Sched_Select{}));

/*========== Macro and Definitions ==========================================*/

#define __stmt__T_use_Sched_async$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id(Sched_async, _T)(Sched self, Closure$(_T)* closure))(Future$(_T))) { \
        return *ptrAlignCast$((Future$(_T)*)( \
            Sched_async(self, closure->as_raw, typeInfo$(_T), l0$((Future$(_T))).as_raw) \
        )); \
    } /* clang-format on */
#define __stmt__T_use_Sched_spawn$(_T...) /* clang-format off */ \
    T_use_E$($set(Sched_ConcE)(Future$(_T))); \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(Sched_spawn, _T)(Sched self, Closure$(_T)* closure))(E$($set(Sched_ConcE)(Future$(_T)))) $scope) { \
        return_ok(*ptrAlignCast$((Future$(_T)*)( \
            try_(Sched_spawn(self, closure->as_raw, typeInfo$(_T), l0$((Future$(_T))).as_raw)) \
        ))); \
    } $unscoped(fn) /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched_self__included */
