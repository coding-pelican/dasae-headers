#include "dh/main.h"
#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/time/Instant.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
fn_((report(Sli_const$u8 label, Sli_const$u8 fmt, ...))(void)) {
    printf("[ThrdId(%zu): %*s] ", Thrd_getCurrentId(), as$(i32, label.len), label.ptr);
    va_list args = {};
    va_start(args, fmt);
    vprintf(as$(const char*, fmt.ptr), args);
    va_end(args);
}

// clang-format off
static Thrd_fn_(demoThrd, ({ time_Duration wait; }, i32), ($ignore, args) $scope) {
    i32 cnt   = 0;
    let start = time_Instant_now();
    while (time_Duration_lt(time_Instant_elapsed(start), args->wait)) { cnt++; }
    report(u8_l("demoThrd"), u8_l("cnt: %d\n"), cnt);
    return_(cnt);
} $unscoped_Thrd_fn;
// clang-format on

static Thrd_fn_(demoThrdDefer, ({ time_Duration wait; }, i32), ($ignore, args)$guard) {
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 2\n")));
    defer_(report(u8_l("demoThrdDefer"), u8_l("deferred 1\n")));
    i32 cnt   = 0;
    let start = time_Instant_now();
    while (time_Duration_lt(time_Instant_elapsed(start), args->wait)) { cnt++; }
    report(u8_l("demoThrdDefer"), u8_l("cnt: %d\n"), cnt);
    return_(cnt);
} $unguarded_Thrd_fn;

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $guard) {
    let_ignore = args;

    var thrd = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from(demoThrd, { time_Duration_secs }).base));
    defer_(report(u8_l("main"), u8_l("ret: %d\n"), Thrd_FnCtx_returned(demoThrd, Thrd_join(thrd))));

    var thrd_defer = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from(demoThrdDefer, { time_Duration_secs }).base));
    defer_(report(u8_l("main"), u8_l("ret: %d\n"), Thrd_FnCtx_returned(demoThrdDefer, Thrd_join(thrd_defer))));

    for (usize i = 0; i < 20; ++i) {
        report(u8_l("main"), u8_l("current: %zu\n"), i);
        time_sleep(time_Duration_fromSecs_f64(0.1));
    }

    return_ok({});
} $unguarded;
