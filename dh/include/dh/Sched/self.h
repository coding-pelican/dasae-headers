#pragma once
#ifndef Sched_self__included
#define Sched_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../exec/base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Sched_VTbl)(struct Sched_VTbl));
$extern let_(Sched_VTbl_noop, Sched_VTbl);
$extern let_(Sched_VTbl_failing, Sched_VTbl);

struct Sched {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(Sched_VTbl));
};
$extern let_(Sched_noop, Sched);
$extern let_(Sched_failing, Sched);

$attr($inline_always)
$static fn_((Sched_isValid(Sched self))(bool));
$attr($inline_always)
$static fn_((Sched_assertValid(P$raw ctx, P_const$$(Sched_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((Sched_ensureValid(Sched self))(Sched));

$extern fn_((Sched_async(Sched self, Clsr$raw* clsr, TypeInfo ret_ty, V$Future$raw ret_mem))(V$Future$raw));
#define T_use_Sched_async$(_T...) __stmt__T_use_Sched_async$(_T)
$attr($must_check)
$extern fn_((Sched_spawn(Sched self, Clsr$raw* clsr, TypeInfo ret_ty, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw));
#define T_use_Sched_spawn$(_T...) __stmt__T_use_Sched_spawn$(_T)

$extern fn_((Sched_recancel(Sched self))(void));
$extern fn_((Sched_swapCancelProtcn(Sched self, Sched_CancelProtcn new_protect))(Sched_CancelProtcn));
$attr($must_check)
$extern fn_((Sched_idle(Sched self))(Sched_Cancelable$void));

$extern fn_((Sched_seq(exec_Seq* self))(Sched));
$extern fn_((Sched_coop(exec_Coop* self))(Sched));
$extern fn_((Sched_para(exec_Para* self))(Sched));
$extern fn_((Sched_preem(exec_Preem* self))(Sched));

struct Sched_VTbl {
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
        /// Borrowed clsr state. The implementation must not copy it.
        P$$(Clsr$raw) inner
    ))(O$P$FutureAny));
    /// Request a concurrency unit. Failure is reported instead of falling back
    /// to eager or await-time execution.
    /// Thread-safe.
    $attr($must_check)
    fn_(((*spawnFn)(
        P$raw ctx,
        u_P$raw result,
        P$$(Clsr$raw) inner
    ))(Sched_ConcE$P$FutureAny));

    /// This function is only called when `async` or `spawn` returns `some`.
    ///
    /// Thread-safe.
    fn_(((*awaitFn)(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
    /// Request cooperative cancel for `any_future`, like `std.Io.Future.cancel`.
    ///
    /// This function is only called when `async` or `spawn` returns `some`.
    ///
    /// Thread-safe.
    fn_(((*cancelFn)(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));

    /// Re-arm cancel after a prior `Sched_Canceled` from `idleFn` or another
    /// cancellation point, like `std.Io.recancel`.
    ///
    /// Not thread-safe.
    fn_(((*recancelFn)(P$raw ctx))(void));
    /// Toggle whether `idleFn` and other cancellation points may return
    /// `Sched_Canceled`, like `std.Io.swapCancelProtcn`.
    ///
    /// Not thread-safe.
    fn_(((*swapCancelProtcnFn)(P$raw ctx, Sched_CancelProtcn new_protect))(Sched_CancelProtcn));
    /// Cooperative no-op for the current routine; may return `Sched_Canceled`.
    /// Same role as `std.Io.checkCancel`: one outstanding request is consumed
    /// per successful `Sched_Canceled` return. Use `try_`/`catch_` like
    /// `time_Awake_sleep`, not as a boolean test.
    ///
    /// Not thread-safe.
    $attr($must_check)
    fn_(((*idleFn)(P$raw ctx))(Sched_Cancelable$void));
};
$extern fn_((Sched_VTbl_noAsync(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny));
$attr($must_check)
$extern fn_((Sched_VTbl_failingSpawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny));
$extern fn_((Sched_VTbl_noAwait(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$extern fn_((Sched_VTbl_unreachableAwait(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$extern fn_((Sched_VTbl_noCancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$extern fn_((Sched_VTbl_unreachableCancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$extern fn_((Sched_VTbl_noRecancel(P$raw ctx))(void));
$extern fn_((Sched_VTbl_unreachableRecancel(P$raw ctx))(void));
$extern fn_((Sched_VTbl_noSwapCancelProtcn(P$raw ctx, Sched_CancelProtcn new_protect))(Sched_CancelProtcn));
$extern fn_((Sched_VTbl_unreachableSwapCancelProtcn(P$raw ctx, Sched_CancelProtcn new_protect))(Sched_CancelProtcn));
$attr($must_check)
$extern fn_((Sched_VTbl_failingIdle(P$raw ctx))(Sched_Cancelable$void));

/*========== Macro and Definitions ==========================================*/

#if on_analysis_active_only || on_comptime
fn_((Sched_isValid(Sched self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->asyncFn)
        && isNonnull(self.vtbl->spawnFn)
        && isNonnull(self.vtbl->awaitFn)
        && isNonnull(self.vtbl->cancelFn)
        && isNonnull(self.vtbl->recancelFn)
        && isNonnull(self.vtbl->swapCancelProtcnFn)
        && isNonnull(self.vtbl->idleFn);
};
fn_((Sched_assertValid(P$raw ctx, P_const$$(Sched_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->asyncFn);
    claim_assert_nonnull(vtbl->spawnFn);
    claim_assert_nonnull(vtbl->awaitFn);
    claim_assert_nonnull(vtbl->cancelFn);
    claim_assert_nonnull(vtbl->recancelFn);
    claim_assert_nonnull(vtbl->swapCancelProtcnFn);
    claim_assert_nonnull(vtbl->idleFn);
};
fn_((Sched_ensureValid(Sched self))(Sched)) {
    return Sched_assertValid(self.ctx, self.vtbl), self;
};
#endif /* on_analysis_active_only || on_comptime */

#define __stmt__T_use_Sched_async$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(Sched_async, _T)(Sched self, Clsr$(_T)* clsr))(Future$(_T))) { \
        return *ptrAlignCast$((Future$(_T)*)( \
            Sched_async(self, clsr->as_raw, typeInfo$(_T), l0$((Future$(_T))).as_raw) \
        )); \
    } /* clang-format on */
#define __stmt__T_use_Sched_spawn$(_T...) /* clang-format off */ \
    T_use_E$($set(Sched_ConcE)(Future$(_T))); \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(Sched_spawn, _T)(Sched self, Clsr$(_T)* clsr))(E$($set(Sched_ConcE)(Future$(_T)))) $scope) { \
        return_ok(*ptrAlignCast$((Future$(_T)*)( \
            try_(Sched_spawn(self, clsr->as_raw, typeInfo$(_T), l0$((Future$(_T))).as_raw)) \
        ))); \
    } $unscoped(fn) /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched_self__included */
