#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/heap/Sys.h"
#include "dh/heap/ThrdSafe.h"
#include "dh/time/Dur.h"
#include "dh/time/self/Awake.h"
#include "dh/io/stream.h"

$static var_(g_clock, time_Awake) = {};

$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), thrd_currId(), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};

$static fn_((sleepMillis(u64 millis))(void)) {
    catch_((time_Awake_sleepMillis(g_clock, millis))($ignore, $do_nothing));
};

T_use$((Void)(thrd_spawn, thrd_join));
T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(thrd_spawn, thrd_join));

$static var_(g_reset_evt, thrd_ResetEvt) = {};

$static fn_((resetEvtWorker(Void unused))(Void) $scope) {
    let_ignore = unused;
    report(u8_l("resetEvtWorker"), u8_l("waiting for signal..."));
    thrd_ResetEvt_waitProtcd(&g_reset_evt);
    report(u8_l("resetEvtWorker"), u8_l("signal received!"));
    return_void();
} $unscoped(fn);
fn_use_Clsr_((resetEvtWorker)(Void)(Void));

$static fn_((exampleResetEvt(thrd_SpawnCfg spawn_cfg))(void) $guard) {
    report(u8_l("example"), u8_l("=== ResetEvt Demo ==="));

    g_reset_evt = thrd_ResetEvt_init();
    defer_(thrd_ResetEvt_fini(&g_reset_evt));

    var worker = clsr_((resetEvtWorker)(Void_()));
    let thrd = catch_((thrd_spawn$Void(spawn_cfg, worker.as_base))($ignore, return));

    sleepMillis(100);
    report(u8_l("example"), u8_l("sending signal..."));
    thrd_ResetEvt_set(&g_reset_evt);

    let_ignore = thrd_join$Void(thrd);
    io_stream_nl();
} $unguarded(fn);

$static fn_((waitGroupTask(i32 task_id))(Void) $scope) {
    report(u8_l("waitGroupTask"), u8_l("task {:d} started"), task_id);
    sleepMillis(as$(u64)(task_id) * 50);
    report(u8_l("waitGroupTask"), u8_l("task {:d} finished"), task_id);
    return_void();
} $unscoped(fn);
fn_use_Clsr_((waitGroupTask)(i32)(Void));

$static fn_((exampleGroup(mem_Alctr gpa))(void) $guard) {
    report(u8_l("example"), u8_l("=== Group Demo ==="));

    var group = thrd_Group_init();
    defer_(thrd_Group_fini(&group));

    A$$(4, Clsr_(waitGroupTask)) tasks = A_zero();
    for_(($r(1, 5), $s(A_ref(tasks)))(i, task)) {
        *task = clsr_((waitGroupTask)(as$(i32)(i)));
        thrd_Group_spawn(&group, gpa, task->as_base);
    } $end(for);

    report(u8_l("example"), u8_l("waiting for all tasks..."));
    thrd_Group_waitProtcd(&group);
    report(u8_l("example"), u8_l("all tasks completed!"));
    io_stream_nl();
} $unguarded(fn);

$static var_(g_sem, thrd_Sem) = {};

$static fn_((semWorker(i32 worker_id))(Void) $guard) {
    report(u8_l("semWorker"), u8_l("worker {:d} waiting for permit..."), worker_id);
    thrd_Sem_waitProtcd(&g_sem);
    defer_(thrd_Sem_post(&g_sem));

    report(u8_l("semWorker"), u8_l("worker {:d} acquired permit, working..."), worker_id);
    sleepMillis(200);
    report(u8_l("semWorker"), u8_l("worker {:d} done, releasing permit"), worker_id);
    return_void();
} $unguarded(fn);
fn_use_Clsr_((semWorker)(i32)(Void));

$static fn_((exampleSemaphore(mem_Alctr gpa))(void) $guard) {
    report(u8_l("example"), u8_l("=== Semaphore Demo (max 2 concurrent) ==="));

    g_sem = thrd_Sem_init();
    defer_(thrd_Sem_fini(&g_sem));

    thrd_Sem_post(&g_sem);
    thrd_Sem_post(&g_sem);

    var group = thrd_Group_init();
    defer_(thrd_Group_fini(&group));

    A$$(4, Clsr_(semWorker)) workers = A_zero();
    for_(($r(1, 5), $s(A_ref(workers)))(i, worker)) {
        *worker = clsr_((semWorker)(as$(i32)(i)));
        thrd_Group_spawn(&group, gpa, worker->as_base);
    } $end(for);

    thrd_Group_waitProtcd(&group);
    report(u8_l("example"), u8_l("all workers completed!"));
    io_stream_nl();
} $unguarded(fn);

$static var_(g_mtx, thrd_Mtx) = {};
$static var_(g_cond, thrd_Cond) = {};
$static var_(g_queue_count, i32) = 0;
$static var_(g_done, bool) = false;

$static fn_((producer(i32 items_to_produce))(Void) $scope) {
    for_(($r(0, intCast$((usize)(items_to_produce))))($ignore)) {
        sleepMillis(50);

        thrd_Mtx_lockProtcd(&g_mtx);
        g_queue_count++;
        report(u8_l("producer"), u8_l("produced item, queue size: {:d}"), g_queue_count);
        thrd_Cond_signal(&g_cond);
        thrd_Mtx_unlock(&g_mtx);
    } $end(for);

    thrd_Mtx_lockProtcd(&g_mtx);
    g_done = true;
    thrd_Cond_broadcast(&g_cond);
    thrd_Mtx_unlock(&g_mtx);

    report(u8_l("producer"), u8_l("finished producing"));
    return_void();
} $unscoped(fn);
fn_use_Clsr_((producer)(i32)(Void));

$static fn_((consumer(i32 consumer_id))(i32) $scope) {
    i32 consumed = 0;
    while (true) {
        thrd_Mtx_lockProtcd(&g_mtx);

        while (g_queue_count == 0 && !g_done) {
            thrd_Cond_waitProtcd(&g_cond, &g_mtx);
        }

        if (g_queue_count > 0) {
            g_queue_count--;
            consumed++;
            report(u8_l("consumer"), u8_l("consumer {:d} consumed item, remaining: {:d}"), consumer_id, g_queue_count);
        } else if (g_done) {
            thrd_Mtx_unlock(&g_mtx);
            break;
        }

        thrd_Mtx_unlock(&g_mtx);
    }

    report(u8_l("consumer"), u8_l("consumer {:d} finished, consumed {:d} items"), consumer_id, consumed);
    return consumed;
} $unscoped(fn);
fn_use_Clsr_((consumer)(i32)(i32));

$static fn_((exampleProducerConsumer(thrd_SpawnCfg spawn_cfg))(void) $guard) {
    report(u8_l("example"), u8_l("=== Producer/Consumer Demo ==="));

    g_mtx = thrd_Mtx_init();
    defer_(thrd_Mtx_fini(&g_mtx));
    g_cond = thrd_Cond_init();
    defer_(thrd_Cond_fini(&g_cond));
    g_queue_count = 0;
    g_done = false;

    var prod_work = clsr_((producer)(5));
    let prod = catch_((thrd_spawn$Void(spawn_cfg, prod_work.as_base))($ignore, return));

    var cons1_work = clsr_((consumer)(1));
    var cons2_work = clsr_((consumer)(2));
    let cons1 = catch_((thrd_spawn$i32(spawn_cfg, cons1_work.as_base))($ignore, return));
    let cons2 = catch_((thrd_spawn$i32(spawn_cfg, cons2_work.as_base))($ignore, return));

    let_ignore = thrd_join$Void(prod);
    let c1 = thrd_join$i32(cons1)->ctx.ret;
    let c2 = thrd_join$i32(cons2)->ctx.ret;

    report(u8_l("example"), u8_l("total consumed: {:d}"), c1 + c2);
    io_stream_nl();
} $unguarded(fn);

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var sys_heap = heap_Sys_init();
    defer_(heap_Sys_fini(&sys_heap));

    var thrd_safe_heap = (heap_ThrdSafe){
        .child_alctr = heap_Sys_alctr(&sys_heap),
        .mtx = thrd_Mtx_init(),
    };
    defer_(thrd_Mtx_fini(&thrd_safe_heap.mtx));

    g_clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let gpa = heap_ThrdSafe_alctr(&thrd_safe_heap);
    let spawn_cfg = thrd_SpawnCfg_default(gpa);

    exampleResetEvt(spawn_cfg);
    exampleGroup(gpa);
    exampleSemaphore(gpa);
    exampleProducerConsumer(spawn_cfg);

    report(u8_l("main"), u8_l("All examples completed!"));
    return_ok({});
} $unguarded(fn);
