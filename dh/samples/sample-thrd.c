#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/time/Dur.h"
#include "dh/time/self/Awake.h"
#include "dh/thrd/CancelTok.h"
#include "dh/thrd/Self.h"
#include "dh/io/stream.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), thrd_currId(), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
$static fn_((demoThrd(time_Awake time, time_Dur wait))(i32)) {
    var_(cnt, i32) = 0;
    report(u8_l(nameOf(demoThrd)), u8_l("starting"));
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        catch_((time_Awake_sleepMillis(time, 1))($ignore, $do_nothing));
        cnt++;
    };
    report(u8_l(nameOf(demoThrd)), u8_l("cnt: {:d}"), cnt);
    return cnt;
};
fn_use_Clsr_((demoThrd)(time_Awake, time_Dur)(i32));
$static fn_((demoThrdDefer(time_Awake time, time_Dur wait))(i32) $guard) {
    defer_(report(u8_l(nameOf(demoThrdDefer)), u8_l("deferred 2")));
    defer_(report(u8_l(nameOf(demoThrdDefer)), u8_l("deferred 1")));
    var_(cnt, i32) = 0;
    report(u8_l(nameOf(demoThrdDefer)), u8_l("starting"));
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        catch_((time_Awake_sleepMillis(time, 1))($ignore, $do_nothing));
        cnt++;
    };
    report(u8_l(nameOf(demoThrdDefer)), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unguarded(fn);
fn_use_Clsr_((demoThrdDefer)(time_Awake, time_Dur)(i32));
$static fn_((demoThrdDeferCheck(time_Awake time, time_Dur wait, thrd_CancelTok cancel))(i32) $guard) {
    defer_(report(u8_l(nameOf(demoThrdDeferCheck)), u8_l("deferred 2")));
    defer_(report(u8_l(nameOf(demoThrdDeferCheck)), u8_l("deferred 1")));
    var_(cnt, i32) = 0;
    report(u8_l(nameOf(demoThrdDeferCheck)), u8_l("starting"));
    while_(
        let start = time_Awake_now(time),
        time_Dur_lt(time_Awake_Inst_elapsed(start, time), wait)
    ) {
        if (thrd_CancelTok_tryWait(cancel)) {
            report(u8_l(nameOf(demoThrdDeferCheck)), u8_l("canceled"));
            return_(cnt);
        }
        catch_((time_Awake_sleep(time, time_Dur_fromMillis(1)))($ignore, $do_nothing));
        cnt++;
    }
    report(u8_l(nameOf(demoThrdDeferCheck)), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unguarded(fn);
fn_use_Clsr_((demoThrdDeferCheck)(time_Awake, time_Dur, thrd_CancelTok)(i32));

T_use$((i32)(thrd_spawn, thrd_join));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    let spawn_cfg = thrd_SpawnCfg_default(gpa);

    var demo_thrd = clsr_((demoThrd)(clock, time_Dur_sec));
    let thrd = try_(thrd_spawn$i32(spawn_cfg, demo_thrd.as_base));

    var demo_thrd_defer = clsr_((demoThrdDefer)(clock, time_Dur_sec));
    let thrd_defer = try_(thrd_spawn$i32(spawn_cfg, demo_thrd_defer.as_base));

    var cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel));
    var demo_thrd_defer_check = clsr_((demoThrdDeferCheck)(clock, time_Dur_mul$u32(time_Dur_sec, 10), thrd_CancelTok_Src_tok(&cancel)));
    let thrd_defer_check = try_(thrd_spawn$i32(spawn_cfg, demo_thrd_defer_check.as_base));

    for_(($r(0, 20))(i)) {
        report(u8_l(nameOf(main)), u8_l("current: {:uz}"), i);
        if (i == 10) {
            thrd_CancelTok_Src_cancel(&cancel);
            report(u8_l(nameOf(main)), u8_l("requested cancel"));
        }
        try_(time_Awake_sleep(clock, time_Dur_fromSecs$f64(0.1)));
    } $end(for);

    let joined_defer_check = thrd_join$i32(thrd_defer_check);
    report(u8_l(nameOf(main)), u8_l(nameOf(joined_defer_check) " ret: {:d}"), joined_defer_check->ctx.ret);

    let joined_defer = thrd_join$i32(thrd_defer);
    report(u8_l(nameOf(main)), u8_l(nameOf(joined_defer) " ret: {:d}"), joined_defer->ctx.ret);

    let joined = thrd_join$i32(thrd);
    report(u8_l(nameOf(main)), u8_l(nameOf(joined) " ret: {:d}"), joined->ctx.ret);

    return_ok({});
} $unguarded(fn);
