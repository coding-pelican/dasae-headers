#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Lane.h"
#include "dh/exec/Seq.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"

/*
 * A minimal application event loop: each call to runOneReady advances one
 * stackless task by one step. No timer or platform implementation is hidden
 * inside this loop.
 */
T_alias$((sample_Exec)(struct sample_Exec {
    var_(seq, exec_Seq);
}));
$static fn_((sample_Exec_init(mem_Alctr gpa))(sample_Exec)) {
    return (sample_Exec){
        .seq = exec_Seq_init(gpa),
    };
};
$static fn_((sample_Exec_fini(sample_Exec* self))(void)) {
    exec_Seq_fini(&self->seq);
    asg_l((self)(cleared()));
};
$static fn_((sample_Exec_sched(sample_Exec* self))(Sched)) {
    return Sched_seq(&self->seq);
};
$static fn_((sample_Exec_run(sample_Exec* self))(void)) {
    while (exec_Lane_runOneReady(&self->seq.lane)) {}
};

T_use$((u32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
$static co_fn_(count, (S_const$u8 label; u32 count;), u32);
co_fn_frame_scope(
    count,
    co_locals_({}),
    co_locals_mut_({
        var_(i, u32);
    }),
    co_suspended_({
        var_(next_tick, Void);
    })
);
co_fn_scope(count) {
    for (co_var_(i) = 0; $co_mut(i) < $co_arg(count); ++$co_mut(i)) {
        io_stream_println(
            u8_l("{:s}: step {:u}"),
            $co_arg(label), $co_mut(i) + 1
        );
        suspend_((next_tick)((Void){}));
    }
    co_return_($co_arg(count));
} $unscoped(co_fn);
co_use_Clsr_((count)(S_const$u8, u32)(u32));

T_use$((u32)(Future, Future_await, Future_cancel, Sched_async));
fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var exec = sample_Exec_init(heap_Sys_alctr(&heap));
    defer_(sample_Exec_fini(&exec));
    let sched = sample_Exec_sched(&exec);

    var task_a = Sched_async$u32(sched,clsr_((count)(u8_l("task a"), 2)).as_base);
    defer_(let_ignore = Future_cancel$u32(&task_a, sched));
    var task_b = Sched_async$u32(sched, clsr_((count)(u8_l("task b"), 3)).as_base);
    defer_(let_ignore = Future_cancel$u32(&task_b, sched));

    sample_Exec_run(&exec);

    io_stream_println(
        u8_l("completed steps: {:u} + {:u}"),
        Future_await$u32(&task_a, sched), Future_await$u32(&task_b, sched)
    );
    return_ok({});
} $unguarded(fn);
