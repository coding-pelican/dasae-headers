#ifndef Sched__included
#define Sched__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Future.h"

/*========== Macros and Declarations ========================================*/

errset_((Sched_ConcE)(Unavailable));
T_use_E$($set(Sched_ConcE)(V$Future$raw));
T_use_E$($set(Sched_ConcE)(P$Future$raw));

errset_((Sched_Cancelable)(Canceled));
T_use_E$($set(Sched_Cancelable)(V$Future$raw));
T_use_E$($set(Sched_Cancelable)(P$Future$raw));

T_alias$((Sched)(struct Sched));
T_alias$((Sched_VTbl)(struct Sched_VTbl {
    /// If it returns `null` it means `result` has been already populated and
    /// `await` will be a no-op.
    ///
    /// When this function returns non-null, the implementation guarantees that
    /// a unit of concurrency has been assigned to the returned task.
    ///
    /// Thread-safe.
    fn_(((*asyncFn)(
        /// Corresponds to `Sched.ctx`.
        O$P$raw ctx,
        /// The pointer of this slice is an "eager" result value.
        /// The length is the size in bytes of the result type.
        /// This pointer's lifetime expires directly after the call to this function.
        u_P$raw result,
        /// Copied and then passed to `entryFn`.
        u_P_const$raw inner,
        fn_(((*entryFn)(P_const$raw inner, P$raw result))(void))
    ))(O$P$Future$raw));

    /// Thread-safe.
    $attr($must_check)
    fn_(((*spawnFn)(
        O$P$raw ctx,
        u_P$raw result,
        u_P_const$raw inner,
        fn_(((*entryFn)(P_const$raw inner, P$raw result))(void))
    ))(Sched_ConcE$P$Future$raw));

    /// This function is only called when `async` returns a non-null value.
    ///
    /// Thread-safe.
    fn_(((*awaitFn)(O$P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));

    /// Equivalent to `await` but initiates cancel request.
    ///
    /// This function is only called when `async` returns a non-null value.
    ///
    /// Thread-safe.
    fn_(((*cancelFn)(O$P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));
}));
struct Sched {
    var_(ctx, O$P$raw);
    var_(vtbl, P_const$$(Sched_VTbl));
};

$extern fn_((Sched_async(Sched self, Closure$raw* closure, V$Future$raw ret_mem))(V$Future$raw));
#define T_use_Sched_async$(_T...) __stmt__T_use_Sched_async$(_T)
$attr($must_check)
$extern fn_((Sched_spawn(Sched self, Closure$raw* closure, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw));
#define T_use_Sched_spawn$(_T...) __stmt__T_use_Sched_spawn$(_T)


#define __stmt__T_use_Sched_async$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id(Sched_async, _T)(Sched self, Closure$raw* closure))(Future$(_T))) /* clang-format on */
#define __stmt__T_use_Sched_spawn$(_T...) /* clang-format off */ \
    T_use_E$($set(Sched_ConcE)(Future$(_T))); \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(Sched_spawn, _T)(Sched self, Closure$raw* closure))(E$($set(Sched_ConcE)(Future$(_T))))) /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched__included */
