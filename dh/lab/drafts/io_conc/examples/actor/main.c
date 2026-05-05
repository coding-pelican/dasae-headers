#include "dh-main.h"

#include "../../dh/exec.h"
#include "../../dh/time.h"
#include "../../dh/io/stream.h"
#include <dh/heap/Page.h>

#include "actor.h"

co_fn_(sample_counterActor, (P$$(actor_Unit) self; S_const$u8 name), Void);
co_fn_scope(
    sample_counterActor,
    co_locals_({}),
    co_locals_mut_({
        var_(msg, actor_Msg);
        var_(count, i64);
    }),
    co_suspended_({
        var_(idle, Void);
    })
) {
    while (true) {
        if_none(actor_Unit_tryRecv($co_arg(self))) {
            suspend_((idle)((Void){}));
            continue;
        } else_some(msg) {
            $co_mut(msg) = msg;
        }

        match_($co_mut(msg)) {
        pattern_((actor_Msg_increment)(delta)) {
            $co_mut(count) += delta;
        } $end(pattern);
        pattern_((actor_Msg_get_value)($ignore)) {
            io_stream_println(
                catch_((io_direct())($ignore, io_noop)),
                u8_l("[{:s}] value={:il}"),
                $co_arg(name),
                $co_mut(count)
            );
        } $end(pattern);
        pattern_((actor_Msg_stop)($ignore)) {
            io_stream_println(
                catch_((io_direct())($ignore, io_noop)),
                u8_l("[{:s}] stop value={:il}"),
                $co_arg(name),
                $co_mut(count)
            );
            co_return_({});
        } $end(pattern);
        default_() claim_unreachable $end(default);
        } $end(match);
    }
} $unscoped(co_fn);
co_use_Closure_((sample_counterActor)(P$$(actor_Unit), S_const$u8)(Void));

T_use$((mem_E$Void)(Co_Ctx, Co_Rtn, Co_Frame, Closure_Ctx, Closure_Rtn, Closure));
co_fn_(sample_feederActor, (P$$(actor_Unit) self; P$$(actor_Unit) dst; S_const$u8 name), mem_E$Void);
co_fn_scope(
    sample_feederActor,
    co_locals_({}),
    co_locals_mut_({
        var_(msg, actor_Msg);
    }),
    co_suspended_({
        var_(idle, Void);
    })
) {
    while (true) {
        if_none(actor_Unit_tryRecv($co_arg(self))) {
            suspend_((idle)((Void){}));
            continue;
        } else_some(msg) {
            $co_mut(msg) = msg;
        }

        match_($co_mut(msg)) {
        pattern_((actor_Msg_increment)(delta)) {
            try_(actor_Unit_send($co_arg(dst), actor_Msg_inc(delta)));
            try_(actor_Unit_send($co_arg(dst), actor_Msg_getValue()));
        } $end(pattern);
        pattern_((actor_Msg_get_value)($ignore)) $do_nothing $end(pattern);
        pattern_((actor_Msg_stop)($ignore)) {
            try_(actor_Unit_send($co_arg(dst), actor_Msg_stopNow()));
            io_stream_println(catch_((io_direct())($ignore, io_noop)), u8_l("[{:s}] stop"), $co_arg(name));
            co_return_(ok({}));
        } $end(pattern);
        default_() claim_unreachable $end(default);
        } $end(match);
    }
} $unscoped(co_fn);
co_use_Closure_((sample_feederActor)(P$$(actor_Unit), P$$(actor_Unit), S_const$u8)(mem_E$Void));
T_use$((mem_E)(actor_Unit_startE));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var page = l0$((heap_Page));
    let gpa = heap_Page_alctr(&page);
    var coop = exec_Coop_init(gpa, try_(time_Awake_direct()), exec_Evented_noop);
    defer_(exec_Coop_fini(&coop));
    let io = catch_((io_direct())($ignore, io_noop));

    var actors = try_(actor_Sys_init(&coop, gpa, 2));
    defer_(actor_Sys_fini(&actors));

    var counter = try_(actor_Sys_createUnit(&actors));
    defer_(actor_Sys_destroyUnit(&actors, &counter));
    actor_Unit_start(counter, closure_(sample_counterActor)(counter, u8_l("counter")).as_base);
    defer_(unwrap_(actor_Unit_exit(counter)));

    var feeder = try_(actor_Sys_createUnit(&actors));
    defer_(actor_Sys_destroyUnit(&actors, &feeder));
    actor_Unit_startE$mem_E(feeder, closure_(sample_feederActor)(feeder, counter, u8_l("feeder")).as_base);
    defer_({
        match_((unwrap_(actor_Unit_exit(feeder)))) {
        pattern_((actor_Exit_normal)($ignore)) $do_nothing $end(pattern);
        pattern_((actor_Exit_error)(err)) {
            io_stream_println(io, u8_l("[feeder] error: {:e}"), err);
        } $end(pattern);
        pattern_((actor_Exit_canceled)($ignore)) $do_nothing $end(pattern);
        } $end(match);
    });

    try_(actor_Unit_send(feeder, actor_Msg_inc(2)));
    try_(actor_Unit_send(feeder, actor_Msg_inc(5)));
    try_(actor_Unit_send(feeder, actor_Msg_stopNow()));

    return_ok_void(exec_Coop_run(&coop));
} $unguarded(fn);
