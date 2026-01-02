#include "dh/main.h"
#include "dh/Thrd/common.h"
#include "dh/Thrd/Ftx.h"
#include "dh/Thrd/Mtx.h"
#include "dh/Thrd/Sem.h"
#include "dh/Thrd/Cond.h"
#include "dh/Thrd/ResetEvent.h"
#include "dh/Thrd/WaitGroup.h"
#include "dh/heap/Page.h"
#include "dh/time/Duration.h"
#include "dh/io/stream.h"

$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currentId(), label);
    va_list args = {};
    with_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
}

//=============================================================================
// Example 1: ResetEvent - Signal between threads
//=============================================================================

$static var_(g_reset_event, Thrd_ResetEvent) = {};

$static Thrd_fn_(resetEventWorker, (Void, Void), ($ignore, $ignore)$scope) {
    report(u8_l("resetEventWorker"), u8_l("waiting for signal..."));
    Thrd_ResetEvent_wait(&g_reset_event);
    report(u8_l("resetEventWorker"), u8_l("signal received!"));
    return_({});
} $unscoped_(Thrd_fn);

$static fn_((exampleResetEvent(void))(void) $guard) {
    report(u8_l("example"), u8_l("=== ResetEvent Demo ==="));

    g_reset_event = Thrd_ResetEvent_init();
    defer_(Thrd_ResetEvent_fini(&g_reset_event));

    var thrd = catch_((Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((resetEventWorker)()).as_raw))($ignore, return));

    time_sleep(time_Duration_fromMillis(100));
    report(u8_l("example"), u8_l("sending signal..."));
    Thrd_ResetEvent_set(&g_reset_event);

    let_ignore = Thrd_join(thrd);
    io_stream_nl();
} $unguarded_(fn);

//=============================================================================
// Example 2: WaitGroup - Wait for multiple tasks
//=============================================================================

$static var_(g_wait_group, Thrd_WaitGroup) = {};

$static Thrd_fn_(waitGroupTask, ({ i32 task_id; }, Void), ($ignore, args)$scope) {
    report(u8_l("waitGroupTask"), u8_l("task {:d} started"), args->task_id);
    time_sleep(time_Duration_fromMillis(as$(u64)(args->task_id) * 50));
    report(u8_l("waitGroupTask"), u8_l("task {:d} finished"), args->task_id);
    return_({});
} $unscoped_(Thrd_fn);

$static fn_((exampleWaitGroup(void))(void) $guard) {
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);
    report(u8_l("example"), u8_l("=== WaitGroup Demo ==="));

    g_wait_group = Thrd_WaitGroup_init();
    defer_(Thrd_WaitGroup_fini(&g_wait_group));

    // Spawn multiple tasks
    A$$(4, Thrd_FnCtx$(waitGroupTask)) task_ctxs = A_zero();
    for_(($r(1, 5), $s(A_ref(task_ctxs)))(i, task_ctx) {
        *task_ctx = Thrd_FnCtx_from$((waitGroupTask)(as$(i32)(i)));
        Thrd_WaitGroup_spawn(&g_wait_group, gpa, task_ctx->as_raw);
    });

    report(u8_l("example"), u8_l("waiting for all tasks..."));
    Thrd_WaitGroup_wait(&g_wait_group);
    report(u8_l("example"), u8_l("all tasks completed!"));
    io_stream_nl();
} $unguarded_(fn);

//=============================================================================
// Example 3: Semaphore - Resource limiting (max 2 concurrent)
//=============================================================================

$static var_(g_sem, Thrd_Sem) = {};

$static Thrd_fn_(semWorker, ({ i32 worker_id; }, Void), ($ignore, args)$guard) {
    report(u8_l("semWorker"), u8_l("worker {:d} waiting for permit..."), args->worker_id);
    Thrd_Sem_wait(&g_sem);
    defer_(Thrd_Sem_post(&g_sem));

    report(u8_l("semWorker"), u8_l("worker {:d} acquired permit, working..."), args->worker_id);
    time_sleep(time_Duration_fromMillis(200));
    report(u8_l("semWorker"), u8_l("worker {:d} done, releasing permit"), args->worker_id);
    return_({});
} $unguarded_(Thrd_fn);

$static fn_((exampleSemaphore(void))(void) $guard) {
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);
    report(u8_l("example"), u8_l("=== Semaphore Demo (max 2 concurrent) ==="));

    g_sem = Thrd_Sem_init();
    defer_(Thrd_Sem_fini(&g_sem));

    // Give 2 permits
    Thrd_Sem_post(&g_sem);
    Thrd_Sem_post(&g_sem);

    var wait_group = Thrd_WaitGroup_init();
    defer_(Thrd_WaitGroup_fini(&wait_group));

    // Spawn 4 workers competing for 2 permits
    A$$(4, Thrd_FnCtx$(semWorker)) worker_ctxs = A_zero();
    for_(($r(1, 5), $s(A_ref(worker_ctxs)))(i, worker_ctx) {
        *worker_ctx = Thrd_FnCtx_from$((semWorker)(as$(i32)(i)));
        Thrd_WaitGroup_spawn(&wait_group, gpa, worker_ctx->as_raw);
    });

    Thrd_WaitGroup_wait(&wait_group);
    report(u8_l("example"), u8_l("all workers completed!"));
    io_stream_nl();
} $unguarded_(fn);

//=============================================================================
// Example 4: Mutex + Condition Variable - Producer/Consumer
//=============================================================================

$static var_(g_mtx, Thrd_Mtx) = {};
$static var_(g_cond, Thrd_Cond) = {};
$static var_(g_queue_count, i32) = 0;
$static var_(g_done, bool) = false;

$static Thrd_fn_(producer, ({ i32 items_to_produce; }, Void), ($ignore, args)$scope) {
    for_(($r(0, intCast$((usize)(args->items_to_produce))))($ignore) {
        time_sleep(time_Duration_fromMillis(50));

        Thrd_Mtx_lock(&g_mtx);
        g_queue_count++;
        report(u8_l("producer"), u8_l("produced item, queue size: {:d}"), g_queue_count);
        Thrd_Cond_signal(&g_cond);
        Thrd_Mtx_unlock(&g_mtx);
    });

    Thrd_Mtx_lock(&g_mtx);
    g_done = true;
    Thrd_Cond_broadcast(&g_cond);
    Thrd_Mtx_unlock(&g_mtx);

    report(u8_l("producer"), u8_l("finished producing"));
    return_({});
} $unscoped_(Thrd_fn);

$static Thrd_fn_(consumer, ({ i32 consumer_id; }, i32), ($ignore, args)$scope) {
    i32 consumed = 0;
    while (true) {
        Thrd_Mtx_lock(&g_mtx);

        while (g_queue_count == 0 && !g_done) {
            Thrd_Cond_wait(&g_cond, &g_mtx);
        }

        if (g_queue_count > 0) {
            g_queue_count--;
            consumed++;
            report(u8_l("consumer"), u8_l("consumer {:d} consumed item, remaining: {:d}"), args->consumer_id, g_queue_count);
        } else if (g_done) {
            Thrd_Mtx_unlock(&g_mtx);
            break;
        }

        Thrd_Mtx_unlock(&g_mtx);
    }

    report(u8_l("consumer"), u8_l("consumer {:d} finished, consumed {:d} items"), args->consumer_id, consumed);
    return_(consumed);
} $unscoped_(Thrd_fn);

$static fn_((exampleProducerConsumer(void))(void) $guard) {
    report(u8_l("example"), u8_l("=== Producer/Consumer Demo ==="));

    g_mtx = Thrd_Mtx_init();
    defer_(Thrd_Mtx_fini(&g_mtx));
    g_cond = Thrd_Cond_init();
    defer_(Thrd_Cond_fini(&g_cond));
    g_queue_count = 0;
    g_done = false;

    var prod = catch_((Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((producer)(5)).as_raw))($ignore, return));

    var cons1 = catch_((Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((consumer)(1)).as_raw))($ignore, return));
    var cons2 = catch_((Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((consumer)(2)).as_raw))($ignore, return));

    let_ignore = Thrd_join(prod);
    let c1 = Thrd_FnCtx_ret$((consumer)(Thrd_join(cons1)));
    let c2 = Thrd_FnCtx_ret$((consumer)(Thrd_join(cons2)));

    report(u8_l("example"), u8_l("total consumed: {:d}"), c1 + c2);
    io_stream_nl();
} $unguarded_(fn);

//=============================================================================
// Main
//=============================================================================

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    exampleResetEvent();
    exampleWaitGroup();
    exampleSemaphore();
    exampleProducerConsumer();

    report(u8_l("main"), u8_l("All examples completed!"));
    return_ok({});
} $unscoped_(fn);
