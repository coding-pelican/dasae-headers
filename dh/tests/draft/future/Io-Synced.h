/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Io-Synced.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-03-28 (date of creation)
 * @updated 2026-03-28 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Io_Synced
 *
 * @brief   Blocking Io backend
 * @details Synchronous execution model. All async dispatches complete
 *          immediately. await/cancel are no-ops.
 *          Generates machine code equivalent to plain C function calls.
 */
#ifndef Io_Synced__included
#define Io_Synced__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Io.h"
#include "dh/time.h"

/*========== Suspend Data Protocol ==========================================*/

/// Suspend reason tags for coroutine-to-runner communication.
/// Coroutines set suspend_data to an Io_SuspendReq* before suspend_().
/// The runner reads the tag and handles synchronously.
typedef enum Io_SuspendOp {
    Io_SuspendOp_yield = 0,
    Io_SuspendOp_sleep,
} Io_SuspendOp;

/// Common header — runner casts suspend_data to this first.
typedef struct Io_SuspendReq {
    var_(op, Io_SuspendOp);
} Io_SuspendReq;

typedef struct Io_SuspendReq_sleep {
    var_(op, Io_SuspendOp); /* Io_SuspendOp_sleep */
    var_(ns, u64);
} Io_SuspendReq_sleep;

/*========== Blocking: Handle Suspend Data ==================================*/

$static fn_((Io_Synced_handleSuspend(Co_Ctx* frame))(void)) {
    if (frame->suspend_data == null) return;
    let req = ptrAlignCast$((const Io_SuspendReq*)(frame->suspend_data));
    switch (req->op) {
    case Io_SuspendOp_yield:
        break;
    case Io_SuspendOp_sleep: {
        let sleep_req = ptrAlignCast$((const Io_SuspendReq_sleep*)(req));
        time_sleep(time_Duration_fromNanos(sleep_req->ns));
    } break;
    }
};

/*========== Blocking: VTable Implementation ================================*/

$static fn_((Io_Synced_async(
    void* userdata,
    Io_Future* future,
    u_P$raw result,
    P_const$raw context,
    TypeInfo context_ty,
    Io_StartFn start
))(void)) {
    let_ignore = userdata;
    let_ignore = context_ty;
    /* Immediate synchronous execution. */
    start(context, result.raw);
    asg_l((&future->any_future)(none()));
};

$static fn_((Io_Synced_asyncCo(
    void* userdata,
    Io_Future* future,
    u_P$raw result,
    Co_Ctx* frame
))(void)) {
    let_ignore = userdata;
    /* Resume loop to completion. */
    while (true) {
        let s = resume_(frame);
        if (s == Co_State_ready) break;
        /* Handle suspend_data synchronously and immediately resume. */
        Io_Synced_handleSuspend(frame);
    }
    /* Copy return value to result buffer. */
    raw_memcpy(result.raw, &frame->ret->data, result.type.size);
    asg_l((&future->any_future)(none()));
};

$static fn_((Io_Synced_await(
    void* userdata,
    P$Io_AnyFuture any_future,
    u_P$raw result
))(void)) {
    let_ignore = userdata;
    let_ignore = any_future;
    let_ignore = result;
    /* Blocking backend never produces AnyFuture handles. */
    claim_unreachable;
};

$static fn_((Io_Synced_sleep(
    void* userdata,
    u64 duration_ns
))(void)) {
    let_ignore = userdata;
    time_sleep(time_Duration_fromNanos(duration_ns));
};

/*========== Blocking: VTable Instance ======================================*/

$static const Io_VTbl Io_Synced_vtbl = {
    .asyncFn = Io_Synced_async,
    .asyncCoFn = Io_Synced_asyncCo,
    .awaitFn = Io_Synced_await,
    .cancelFn = Io_Synced_await,
    .sleepFn = Io_Synced_sleep,
};

/*========== Blocking: Constructor ==========================================*/

/// Create a blocking Io instance. No heap, no event loop, no threads.
$attr($inline_always)
$static fn_((Io_synced(void))(Io)) {
    return (Io){
        .userdata = null,
        .vtbl = &Io_Synced_vtbl,
    };
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Io_Synced__included */
