#include "dh/main.h"
#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "dh/io/stream.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_getCurrentId(), label);
    va_list args = {};
    with_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
}
$static Thrd_fn_(demoThrd, ({ time_Duration wait; }, i32), ($ignore, args)$scope) {
    i32 cnt = 0;
    while_(
        let start = time_Instant_now(),
        time_Duration_lt(time_Instant_elapsed(start), args->wait)
    ) { cnt++; }
    report(u8_l("demoThrd"), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unscoped_(Thrd_fn);

$static Thrd_fn_(demoThrdDefer, ({ time_Duration wait; }, i32), ($ignore, args)$guard) {
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 2")));
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 1")));
    i32 cnt = 0;
    while_(
        let start = time_Instant_now(),
        time_Duration_lt(time_Instant_elapsed(start), args->wait)
    ) { cnt++; }
    report(u8_l("demoThrdDefer"), u8_l("cnt: {:d}"), cnt);
    return_(cnt);
} $unguarded_(Thrd_fn);

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var thrd = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from$((demoThrd)(time_Duration_sec)).as_raw));
    defer_(report(u8_l("main"), u8_l("ret: {:d}"), Thrd_FnCtx_ret$((demoThrd)(Thrd_join(thrd)))));

    var thrd_defer = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from$((demoThrdDefer)(time_Duration_sec)).as_raw));
    defer_(report(u8_l("main"), u8_l("ret: {:d}"), Thrd_FnCtx_ret$((demoThrdDefer)(Thrd_join(thrd_defer)))));

    for_(($r(0, 20))(i) {
        report(u8_l("main"), u8_l("current: {:uz}"), i);
        time_sleep(time_Duration_fromSecs$f64(0.1));
    });

    return_ok({});
} $unguarded_(fn);
