#include "dh-main.h"
#include "dh/Thrd/self.h"
#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/io/stream.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currId(), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
$static fn_((demoThrd(time_Duration wait))(i32)) {
    var_(cnt, i32) = 0;
    while_(
        let start = time_Instant_now(),
        time_Duration_lt(time_Instant_elapsed(start), wait)
    ) cnt++;
    report(u8_l("demoThrd"), u8_l("cnt: {:d}"), cnt);
    return cnt;
};
fn_use_Clsr_((demoThrd)(time_Duration)(i32));
$static fn_((demoThrdDefer(time_Duration wait))(i32) $guard) {
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 2")));
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 1")));
    var_(cnt, i32) = 0;
    while_(
        let start = time_Instant_now(),
        time_Duration_lt(time_Instant_elapsed(start), wait)
    ) cnt++;
    report(u8_l("demoThrdDefer"), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unguarded(fn);
fn_use_Clsr_((demoThrdDefer)(time_Duration)(i32));

T_use$((i32)(Thrd_spawn, Thrd_join));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var thrd = try_(Thrd_spawn$i32(Thrd_SpawnCfg_default, clsr_(demoThrd)(time_Duration_sec).as_base));
    defer_(report(u8_l("main"), u8_l("ret: {:d}"), Thrd_join$i32(thrd)));

    var thrd_defer = try_(Thrd_spawn$i32(Thrd_SpawnCfg_default, clsr_(demoThrdDefer)(time_Duration_sec).as_base));
    defer_(report(u8_l("main"), u8_l("ret: {:d}"), Thrd_join$i32(thrd_defer)));

    for_(($r(0, 20))(i)) {
        report(u8_l("main"), u8_l("current: {:uz}"), i);
        time_sleep(time_Duration_fromSecs$f64(0.1));
    } $end(for);

    return_ok({});
} $unguarded(fn);
