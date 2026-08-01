#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Coop.h"
#include "dh/exec/Lane.h"
#include "dh/exec/LaneTimed.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"
#include "dh/time.h"

/*
 * An application-owned event loop. exec_LaneTimed supplies task and timer
 * primitives; this type owns the policy that pumps them.
 */
T_alias$((sample_EventLoop)(struct sample_EventLoop {
    var_(exec, exec_Coop);
}));
$static fn_((sample_EventLoop_init(mem_Alctr gpa, time_Awake clock))(sample_EventLoop)) {
    return (sample_EventLoop){
        .exec = exec_Coop_init(gpa, clock),
    };
};
$static fn_((sample_EventLoop_fini(sample_EventLoop* self))(void)) {
    exec_Coop_fini(&self->exec);
    asg_l((self)(cleared()));
};
$static fn_((sample_EventLoop_sched(sample_EventLoop* self))(Sched)) {
    return Sched_coop(&self->exec);
};
$static fn_((sample_EventLoop_clock(sample_EventLoop* self))(time_Awake)) {
    return time_Awake_evented(&self->exec);
};
$static fn_((sample_EventLoop_run(sample_EventLoop* self))(void)) {
    let timed = &self->exec.timed;
    while (true) {
        exec_LaneTimed_wakeDueTimers(timed);
        if (exec_Lane_runOneReady(&timed->lane)) continue;

        let deadline = orelse_((exec_LaneTimed_nextTimerDeadline(timed))(break));
        let remaining = exec_LaneTimed_remaining(timed, deadline);
        if (time_Dur_isZero(remaining)) continue;
        catch_((time_Awake_sleep(timed->clock_pump, remaining))($ignore, $do_nothing));
    }
};

T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((u32)(Future, Future_await, Future_cancel, Sched_async));

$static fn_((waitPair(time_Awake clock, S_const$u8 name, u64 first_ms, u64 second_ms))(u32)) {
    io_stream_println(u8_l("{:s}: start"), name);
    catch_((time_Awake_sleepMillis(clock, first_ms))($ignore, $do_nothing));
    io_stream_println(u8_l("{:s}: resumed after {:u} ms"), name, first_ms);
    catch_((time_Awake_sleepMillis(clock, second_ms))($ignore, $do_nothing));
    io_stream_println(u8_l("{:s}: resumed after {:u} ms"), name, second_ms);
    return as$(u32)(first_ms + second_ms);
};
fn_use_Clsr_((waitPair)(time_Awake, S_const$u8, u64, u64)(u32));

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var loop = sample_EventLoop_init(heap_Sys_alctr(&heap), try_(time_Awake_direct()));
    defer_(sample_EventLoop_fini(&loop));
    let sched = sample_EventLoop_sched(&loop);
    let clock = sample_EventLoop_clock(&loop);

    var task_a = Sched_async$u32(sched, clsr_((waitPair)(clock, u8_l("task a"), 100, 120)).as_base);
    defer_(let_ignore = Future_cancel$u32(&task_a, sched));
    var task_b = Sched_async$u32(sched, clsr_((waitPair)(clock, u8_l("task b"), 50, 130)).as_base);
    defer_(let_ignore = Future_cancel$u32(&task_b, sched));

    sample_EventLoop_run(&loop);

    io_stream_println(
        u8_l("completed: task a={:u} ms, task b={:u} ms"),
        Future_await$u32(&task_a, sched), Future_await$u32(&task_b, sched)
    );
    return_ok({});
} $unguarded(fn);
