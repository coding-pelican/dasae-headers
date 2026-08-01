#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Rand.h"
#include "dh/Sched.h"
#include "dh/exec/Coop.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"
#include "dh/time.h"

$static fn_((terminalMove(u32 x, u32 y))(void)) {
    io_stream_print(u8_l("\x1b[{:u};{:u}H"), y + 1, x + 1);
};
$static fn_((terminalPut(u32 x, u32 y, u8 byte))(void)) {
    terminalMove(x, y);
    io_stream_print(u8_l("{:c}"), byte);
};

$static fn_((sleepMillis(time_Awake clock, u64 millis))(void)) {
    catch_((time_Awake_sleepMillis(clock, millis))($ignore, $do_nothing));
};

T_use$((Void)(Future, Future_await, Future_cancel, Sched_async));

$static fn_((typeFixed(time_Awake clock, S_const$u8 text, u64 interval_ms, u32 x, u32 y))(Void)) {
    for_(($rf(0), $s(text))(idx, byte)) {
        terminalPut(x + as$(u32)(idx), y, *byte);
        sleepMillis(clock, interval_ms);
    } $end(for);
    return (Void){};
};
fn_use_Clsr_((typeFixed)(time_Awake, S_const$u8, u64, u32, u32)(Void));

$static fn_((typeOverDuration(time_Awake clock, S_const$u8 text, u64 duration_ms, u32 x, u32 y))(Void)) {
    if (text.len == 0) return (Void){};
    return typeFixed(clock, text, int_max(duration_ms / text.len, u64_(1)), x, y);
};
fn_use_Clsr_((typeOverDuration)(time_Awake, S_const$u8, u64, u32, u32)(Void));

$static fn_((typeRealistic(time_Awake clock, S_const$u8 text, u64 base_interval_ms, u32 x, u32 y))(Void)) {
    var random = Rand_init();
    for_(($rf(0), $s(text))(idx, byte)) {
        terminalPut(x + as$(u32)(idx), y, *byte);

        var delay_ms = base_interval_ms + Rand_lessThan$u64(&random, 9);
        switch (*byte) {
        case '.':
        case '!':
        case '?':
            delay_ms += 40;
            break;
        case ',':
        case ';':
            delay_ms += 20;
            break;
        case ' ':
            delay_ms += 8;
            break;
        }
        sleepMillis(clock, delay_ms);
    } $end(for);
    return (Void){};
};
fn_use_Clsr_((typeRealistic)(time_Awake, S_const$u8, u64, u32, u32)(Void));

fn_((main(proc_Entry entry))(E$void) $guard) {
    var_(arg_scratch, A$$(256, u8)) $undefined;
    var args = proc_Args_iter(entry.args);
    let_ignore = try_(proc_Args_Iter_skip(&args));
    let text = orelse_((try_(proc_Args_Iter_next(&args, A_ref$((S$u8)arg_scratch))))(
        u8_l("Hello, world! This is asynchronous typing.")
    ));

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var loop = exec_Coop_init(heap_Sys_alctr(&heap), try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&loop));
    let sched = Sched_coop(&loop);
    let clock = time_Awake_evented(&loop);

    io_stream_print(u8_l("\x1b[2J\x1b[H"));
    io_stream_println(u8_l("fixed interval"));
    terminalMove(0, 2);
    io_stream_print(u8_l("fixed duration"));
    terminalMove(0, 4);
    io_stream_print(u8_l("realistic timing"));

    var fixed = Sched_async$Void(sched,clsr_((typeFixed)(clock, text, 12, 0, 1)).as_base);
    defer_(let_ignore = Future_cancel$Void(&fixed, sched));
    var duration = Sched_async$Void(sched,clsr_((typeOverDuration)(clock, text, 600, 0, 3)).as_base);
    defer_(let_ignore = Future_cancel$Void(&duration, sched));
    var realistic = Sched_async$Void(sched,clsr_((typeRealistic)(clock, text, 8, 0, 5)).as_base);
    defer_(let_ignore = Future_cancel$Void(&realistic, sched));

    let_ignore = Future_await$Void(&fixed, sched);
    let_ignore = Future_await$Void(&duration, sched);
    let_ignore = Future_await$Void(&realistic, sched);
    terminalMove(0, 7);
    io_stream_println(u8_l("all typing tasks completed"));
    return_ok({});
} $unguarded(fn);
