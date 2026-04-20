/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Io.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-03-28 (date of creation)
 * @updated 2026-03-28 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Io
 *
 * @brief   I/O execution model interface
 * @details Allocator-pattern dependency injection for execution models.
 *          Supports blocking, event loop, and thread pool backends
 *          through a unified vtbl interface.
 *          Io_async dispatches plain functions.
 *          Io_asyncCo dispatches stackless coroutines (Co_Ctx).
 */
#ifndef Io__included
#define Io__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Co.h"

/*========== Io: The Interface ==============================================*/

typedef struct Io_VTbl Io_VTbl;
typedef struct Io {
    var_(userdata, void*);
    var_(vtbl, const Io_VTbl*);
} Io;

/*========== Io_AnyFuture: Opaque Handle ====================================*/

/// Opaque handle to a backend-managed future.
/// Backend defines the concrete type internally.
/// Blocking backend never produces this (always none()).
typedef struct Io_AnyFuture Io_AnyFuture;
T_use_P$(Io_AnyFuture);
T_use_O$(P$Io_AnyFuture);

/*========== Io_Future: Raw Future ==========================================*/

/// Raw future header. Result data follows immediately after this struct
/// in the typed Io_Future$(T) union overlay.
typedef struct Io_Future {
    struct {
        var_(any_future, O$P$Io_AnyFuture);
        debug_only(var_(result_type, TypeInfo);)
    };
    var_(result, u8) $flexible;
} Io_Future;
T_use_P$(Io_Future);
$extern fn_((Io_Future_result(const Io_Future* self, TypeInfo type))(u_P_const$raw));
$extern fn_((Io_Future_resultMut(Io_Future* self, TypeInfo type))(u_P$raw));
/// Await a raw future. Idempotent — if any_future is none(), result is
/// already populated and this is a no-op.
$extern fn_((Io_Future_await(Io_Future* self, Io io, u_V$raw ret_mem))(u_V$raw));
/// Cancel a raw future. Idempotent — same semantics as await but
/// places a cancellation request on the backend.
$extern fn_((Io_Future_cancel(Io_Future* self, Io io, u_V$raw ret_mem))(u_V$raw));

/*========== Io_Future$(T): Typed Future ====================================*/

#define Io_Future$$(_T...) __comp_anon__Io_Future$$(_T)
#define Io_Future$(_T...) __comp_alias__Io_Future$(_T)
#define T_decl_Io_Future$(_T...) __comp_gen__T_decl_Io_Future$(_T)
#define T_impl_Io_Future$(_T...) __comp_gen__T_impl_Io_Future$(_T)
#define T_use_Io_Future$(_T...) __comp_gen__T_use_Io_Future$(_T)

#define __comp_anon__Io_Future$$(_T...) \
    union { \
        struct { \
            var_(any_future, O$P$Io_AnyFuture); \
            debug_only(var_(result_type, TypeInfo);) var_(result, _T); \
        }; \
        var_(as_raw, Io_Future) $like_ref; \
    }
#define __comp_alias__Io_Future$(_T...) \
    pp_join($, Io_Future, _T)
#define __comp_gen__T_decl_Io_Future$(_T...) \
    $maybe_unused typedef union Io_Future$(_T) Io_Future$(_T)
#define __comp_gen__T_impl_Io_Future$(_T...) \
    union Io_Future$(_T) { \
        struct { \
            var_(any_future, O$P$Io_AnyFuture); \
            debug_only(var_(result_type, TypeInfo);) var_(result, _T); \
        }; \
        var_(as_raw, Io_Future) $like_ref; \
    }
#define __comp_gen__T_use_Io_Future$(_T...) \
    T_decl_Io_Future$(_T); \
    T_impl_Io_Future$(_T)

/*========== Io_Future$(T) Typed Wrappers ===================================*/

/* clang-format off */
#define T_use_Io_Future_result$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(Io_Future_result, _T)(const Io_Future$(_T)* self))(const _T*)) { \
        return u_castP$((_T)(Io_Future_result(self->as_raw, typeInfo$(_T)))); \
    }

#define T_use_Io_Future_resultMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(Io_Future_resultMut, _T)(Io_Future$(_T)* self))(_T*)) { \
        return u_castP$((_T)(Io_Future_resultMut(self->as_raw, typeInfo$(_T)))); \
    }

#define T_use_Io_Future_await$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(Io_Future_await, _T)(Io_Future$(_T)* self, Io io))(_T)) { \
        return u_castV$((_T)(Io_Future_await(self->as_raw, io, u_retV$(_T)))); \
    }

#define T_use_Io_Future_cancel$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(Io_Future_cancel, _T)(Io_Future$(_T)* self, Io io))(_T)) { \
        return u_castV$((_T)(Io_Future_cancel(self->as_raw, io, u_retV$(_T)))); \
    }
/* clang-format on */

/*========== Io_StartFn: Type-Erased Function Entry =========================*/

/// Start function for plain (non-coroutine) async dispatch.
/// context: pointer to copied args struct.
/// result:  pointer to result buffer in Future.
typedef fn_(((*)(P_const$raw context, P$raw result))(void) $T) Io_StartFn;

/*========== Io_VTbl: Backend Interface ===================================*/

struct Io_VTbl {
    /// Dispatch a plain function asynchronously.
    ///
    /// Blocking: calls start(context, result) immediately, sets
    ///           future->any_future = none().
    /// EventLoop: copies context, registers task, sets
    ///            future->any_future = some(handle).
    ///
    /// @param userdata   Backend state (event loop, thread pool, etc.)
    /// @param future     Raw future header
    /// @param result     Typed result pointer (u_P$raw with TypeInfo)
    /// @param context    Pointer to args struct
    /// @param context_ty TypeInfo of args struct
    /// @param start      Type-erased entry point
    fn_(((*asyncFn)(
        void* userdata,
        Io_Future* future,
        u_P$raw result,
        P_const$raw context,
        TypeInfo context_ty,
        Io_StartFn start
    ))(void));

    /// Dispatch a stackless coroutine (Co_Ctx) asynchronously.
    ///
    /// Blocking: resume loop to completion, copies return value
    ///           to result, sets future->any_future = none().
    /// EventLoop: registers frame, schedules via suspend_data,
    ///            sets future->any_future = some(handle).
    ///
    /// @param userdata   Backend state
    /// @param future     Raw future header
    /// @param result     Typed result pointer
    /// @param frame      Coroutine frame (Co_Ctx*)
    fn_(((*asyncCoFn)(
        void* userdata,
        Io_Future* future,
        u_P$raw result,
        Co_Ctx* frame
    ))(void));

    /// Await a future. Called only when any_future is non-null.
    ///
    /// @param userdata    Backend state
    /// @param any_future  Opaque handle from async_fn/async_co
    /// @param result      Where to write the result
    fn_(((*awaitFn)(
        void* userdata,
        P$Io_AnyFuture any_future,
        u_P$raw result
    ))(void));

    /// Cancel a future. Same as await but requests cancellation.
    fn_(((*cancelFn)(
        void* userdata,
        P$Io_AnyFuture any_future,
        u_P$raw result
    ))(void));

    /// Sleep for a duration.
    ///
    /// Blocking: platform_sleep_ns.
    /// EventLoop: registers timer, resumes after expiry.
    fn_(((*sleepFn)(
        void* userdata,
        u64 duration_ns
    ))(void));
};

/*========== Io Dispatch Macros =============================================*/

/// Dispatch a plain function through Io.
/// _start must be an Io_StartFn-compatible function pointer.
/// _context_ptr must point to the args struct.
/// _future_ptr must point to a typed Io_Future$(T).
#define Io_async(_io, _start, _context_ptr, _future_ptr) \
    (_io).vtbl->asyncFn( \
        (_io).userdata, \
        (_future_ptr)->as_raw, \
        u_anyP(&(_future_ptr)->result), \
        ptrCast$((P_const$raw)(_context_ptr)), \
        typeInfo$(TypeOf(*(_context_ptr))), \
        (_start) \
    )

/// Dispatch a stackless coroutine through Io.
/// _frame must be a Co_Ctx* (or &co_ctx(...)->anyraw).
/// _future_ptr must point to a typed Io_Future$(T).
#define Io_asyncCo(_io, _frame, _future_ptr) \
    (_io).vtbl->asyncCoFn( \
        (_io).userdata, \
        (_future_ptr)->as_raw, \
        u_anyP(&(_future_ptr)->result), \
        (_frame) \
    )

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Io__included */
