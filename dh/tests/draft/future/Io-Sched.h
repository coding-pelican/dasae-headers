/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Io-Sched.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-03-28 (date of creation)
 * @updated 2026-03-28 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Io_Sched
 *
 * @brief   Single-threaded event loop Io backend
 * @details Stackless coroutine scheduler. Multiplexes hundreds to
 *          thousands of coroutines on one thread via timer-based
 *          cooperative scheduling. Suspend data protocol enables
 *          coroutines to express scheduling requirements without
 *          coupling to the event loop.
 */
#ifndef Io_Sched__included
#define Io_Sched__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Io.h"
#include "Io-Synced.h" /* Io_SuspendOp, Io_SuspendReq, platform_sleep */
#include "dh/time.h"
#include "dh/time/Instant.h"

/*========== EvLoop Task ====================================================*/

#define Io_Sched_max_tasks (256)

typedef struct Io_Sched_Task {
    var_(handle, P$Io_AnyFuture); /* AnyFuture identity — address is the handle */
    var_(frame, Co_Ctx*); /* coroutine frame (null for plain fn tasks)   */
    var_(start, Io_StartFn); /* plain fn entry (null for coroutine tasks)   */
    var_(context, P$raw); /* copied args for plain fn tasks              */
    var_(context_size, usize);
    var_(result, u_P$raw); /* points into caller's Io_Future$(T).result   */
    var_(expires, time_Instant);
    var_(active, bool);
} Io_Sched_Task;

typedef struct Io_Sched_Loop {
    var_(tasks, A$$(Io_Sched_max_tasks, Io_Sched_Task));
    var_(task_count, usize);
} Io_Sched_Loop;

/*========== EvLoop Internals ===============================================*/

$static fn_((Io_Sched_loop(void))(Io_Sched_Loop)) {
    var_(loop, Io_Sched_Loop) = cleared();
    loop.task_count = 0;
    for_(($s(A_ref(loop.tasks)))(task)) task->active = false $end(for);
    return loop;
};

$static fn_((Io_Sched_Loop_findSlot(Io_Sched_Loop* self))(Io_Sched_Task*)) {
    for_(($s(A_ref(self->tasks)), $rf(0))(task, i)) {
        if (task->active) continue;
        if (self->task_count <= i) self->task_count = i + 1;
        return task;
    } $end(for);
    claim_unreachable;
};

/// Handle a suspended coroutine's suspend_data, registering it
/// with the event loop for later resumption.
$static fn_((Io_Sched_Loop_handleSuspend(
    Io_Sched_Loop* self,
    Co_Ctx* frame,
    Io_Sched_Task* task
))(void)) {
    let_ignore = self;
    let now = time_Instant_now();
    if (frame->suspend_data == null) {
        /* yield — resume next tick */
        task->expires = now;
        return;
    }
    let req = as$(const Io_SuspendReq*)(frame->suspend_data);
    switch (req->op) {
    case Io_SuspendOp_yield:
        task->expires = now;
        break;
    case Io_SuspendOp_sleep: {
        let sleep_req = ptrAlignCast$((const Io_SuspendReq_sleep*)(req));
        task->expires = time_Instant_addDuration(
            now, time_Duration_fromNanos(sleep_req->ns)
        );
    } break;
    }
};

/// Run one tick of the event loop. Returns true if any task remains active.
$static fn_((Io_Sched_Loop_tick(Io_Sched_Loop* self))(bool)) {
    let now = time_Instant_now();
    var any = false;
    for_(($s(A_ref(self->tasks)))(task)) {
        if (!task->active) continue;
        any = true;
        if (!time_Instant_le(task->expires, now)) continue;
        if (task->frame != null) {
            /* Coroutine task */
            let s = resume_(task->frame);
            if (s == Co_State_ready) {
                /* Completed — copy return value to caller's result buffer */
                raw_memcpy(
                    task->result.raw,
                    &task->frame->ret->data,
                    task->result.type.size
                );
                task->active = false;
            } else {
                /* Still suspended — re-register based on suspend_data */
                Io_Sched_Loop_handleSuspend(self, task->frame, task);
            }
        } else {
            /* Plain function task — execute to completion */
            task->start(task->context, task->result.raw);
            task->active = false;
        }
    } $end(for);
    return any;
};

/// Run the event loop until all tasks complete.
$static fn_((Io_Sched_Loop_run(Io_Sched_Loop* self))(void)) {
    while (Io_Sched_Loop_tick(self)) $do_nothing;
};

/// Run the event loop until a specific task completes.
$static fn_((Io_Sched_Loop_runUntil(Io_Sched_Loop* self, Io_Sched_Task* target))(void)) {
    while (target->active) Io_Sched_Loop_tick(self);
};

/*========== EvLoop: VTable Implementation ==================================*/

$static fn_((Io_Sched_async(
    void* userdata,
    Io_Future* future,
    u_P$raw result,
    P_const$raw context,
    TypeInfo context_ty,
    Io_StartFn start
))(void)) {
    let loop = as$(Io_Sched_Loop*)(userdata);
    let task = Io_Sched_Loop_findSlot(loop);

    /* For plain functions in the event loop, we store the context pointer
     * directly. The caller must ensure the context remains valid until
     * the task executes. For stack-allocated args, this means the
     * dispatching scope must outlive the event loop run. */
    *task = (Io_Sched_Task){
        .start = start,
        .frame = null,
        .context = ptrQualCast$((P$raw)(context)), /* NOTE: not copied — caller owns lifetime */
        .context_size = context_ty.size,
        .result = result,
        .expires = time_Instant_now(),
        .active = true,
    };
    asg_l((&future->any_future)(some(task->handle)));
};

$static fn_((Io_Sched_asyncCo(
    void* userdata,
    Io_Future* future,
    u_P$raw result,
    Co_Ctx* frame
))(void)) {
    let loop = as$(Io_Sched_Loop*)(userdata);
    let task = Io_Sched_Loop_findSlot(loop);

    *task = (Io_Sched_Task){
        .start = null,
        .frame = frame,
        .context = null,
        .context_size = 0,
        .result = result,
        .expires = time_Instant_now(),
        .active = true,
    };

    /* First resume — run until first suspend point. */
    let s = resume_(frame);
    if (s == Co_State_ready) {
        /* Completed immediately — no event loop registration needed. */
        raw_memcpy(result.raw, &frame->ret->data, result.type.size);
        task->active = false;
        asg_l((&future->any_future)(none()));
    } else {
        /* Register based on first suspend_data. */
        Io_Sched_Loop_handleSuspend(loop, frame, task);
        asg_l((&future->any_future)(some(task->handle)));
    }
};

$static fn_((Io_Sched_await(
    void* userdata,
    P$Io_AnyFuture any_future,
    u_P$raw result
))(void)) {
    let_ignore = result;
    let loop = as$(Io_Sched_Loop*)(userdata);
    /* Find the task by AnyFuture handle address (container_of). */
    let task = recordPtr(any_future, Io_Sched_Task, handle);
    /* Run the event loop until this specific task completes. */
    Io_Sched_Loop_runUntil(loop, task);
    /* result was already written by the tick that completed the task. */
};

$static fn_((Io_Sched_cancel(
    void* userdata,
    P$Io_AnyFuture any_future,
    u_P$raw result
))(void)) {
    let_ignore = result;
    let loop = as$(Io_Sched_Loop*)(userdata);
    let task = recordPtr(any_future, Io_Sched_Task, handle);
    /* Simple cancellation: deactivate without running. */
    task->active = false;
};

$static fn_((Io_Sched_sleep(
    void* userdata,
    u64 duration_ns
))(void)) {
    let_ignore = userdata;
    /* Fallback: blocking sleep. In a real implementation this would
     * register a timer on the current coroutine's suspend_data. */
    Io_platform_sleep_ns(duration_ns);
};

/*========== EvLoop: VTable Instance ========================================*/

$static const Io_VTbl Io_Sched_vtbl = {
    .asyncFn = Io_Sched_async,
    .asyncCoFn = Io_Sched_asyncCo,
    .awaitFn = Io_Sched_await,
    .cancelFn = Io_Sched_cancel,
    .sleepFn = Io_Sched_sleep,
};

/*========== EvLoop: Constructor ============================================*/

/// Create an event loop Io instance.
/// The Io_Sched must outlive all futures created through this Io.
$attr($inline_always)
$static fn_((Io_sched(Io_Sched_Loop* loop))(Io)) {
    return (Io){
        .userdata = loop,
        .vtbl = &Io_Sched_vtbl,
    };
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Io_Sched__included */
