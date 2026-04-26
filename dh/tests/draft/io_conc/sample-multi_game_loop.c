#include "dh-main.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/net.h"
#include "dh/io.h"
#include "dh/heap/Page.h"

$static fn_((report(io_Self io, S_const$u8 fmt, ...))(void)) {
    using_(var args = l0$((va_list))) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(io, fmt, args);
    }
    io_stream_nl(io);
};

T_alias$((GameRuntime)(struct GameRuntime {
    var_(frame_clock, time_Awake);
    var_(net_clock, time_Awake);
    var_(io_loop, exec_Coop);
    var_(net_sched, Sched);
    var_(net, net_Self);
    var_(io, io_Self);
    var_(ticks, usize);
}));

/*
 * Evented operation surface expected from the eventual WASM/browser backend.
 * The sample uses these as the runtime boundary rather than as direct syscalls.
 */
$static fn_((sample_wasmEvented(void))(exec_Evented));
$static fn_((sample_net_accept(net_Svr* server, net_Self net))(net_Stream));
$static fn_((sample_net_read(net_Stream stream, S$u8 buf))(usize));
$static fn_((sample_net_write(net_Stream stream, S_const$u8 bytes))(usize));

T_use$((usize)(Co_Ctx, Co_Rtn, Co_Frame));
T_use$((usize)(Closure_Ctx, Closure_Rtn, Closure));
T_use$((usize)(Future, Sched_async, Future_cancel));

co_fn_(sample_sessionActor, (P$$(GameRuntime) rt; net_Stream stream;), usize);
co_fn_scope(
    sample_sessionActor,
    co_locals_({}),
    co_locals_mut_({
        var_(buf, A$$(512, u8));
    }),
    co_suspended_({
        var_(n, usize);
    })
) {
    while (true) {
        suspend_((n)(sample_net_read($co_arg(stream), A_ref$((S$u8)($co_mut(buf))))));
        if ($co_suspended(n) == 0) co_return_(0);
        suspend_((n)(sample_net_write(
            $co_arg(stream),
            A_prefix$((S_const$u8)(($co_mut(buf)))($co_suspended(n)))
        )));
    }
} $unscoped(co_fn);
co_use_Closure_((sample_sessionActor)(GameRuntime*, net_Stream)(usize));

co_fn_(sample_acceptLoop, (P$$(GameRuntime) rt; P$$(net_Svr) server;), usize);
co_fn_scope(
    sample_acceptLoop,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(stream, net_Stream);
    })
) {
    while (true) {
        suspend_((stream)(sample_net_accept($co_arg(server), $co_arg(rt)->net)));
        let_ignore = Sched_async$usize(
            $co_arg(rt)->net_sched,
            closure_(sample_sessionActor)($co_arg(rt), ($co_suspended(stream))).as_base
        );
    }
    co_return_(0);
} $unscoped(co_fn);
co_use_Closure_((sample_acceptLoop)(GameRuntime*, net_Svr*)(usize));

$static fn_((sample_frameTick(GameRuntime* rt, time_Awake_Inst frame_begin))(void) $scope) {
    let net_budget = time_Dur_fromMillis(2);
    let net_deadline = time_Awake_Inst_addChkdDur(frame_begin, net_budget);
    if_some((net_deadline)(deadline)) {
        exec_Coop_runUntil(&rt->io_loop, deadline);
    } else_none {
        exec_Coop_run(&rt->io_loop);
    };

    rt->ticks++;
    report(rt->io, u8_l("frame {:uz}"), rt->ticks);
} $unscoped(fn);

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    let evented = sample_wasmEvented();

    var rt = (GameRuntime){
        .frame_clock = try_(time_Awake_direct()),
        .io_loop = exec_Coop_init(gpa, try_(time_Awake_direct()), evented),
        .io = try_(io_direct()),
        .ticks = 0,
    };
    defer_(exec_Coop_fini(&rt.io_loop));

    rt.net_clock = time_Awake_evented(&rt.io_loop);
    rt.net_sched = Sched_coop(&rt.io_loop);
    rt.net = net_evented(&rt.io_loop);

    /*
     * In a browser host this would be created during startup and the returned
     * callback would be pumped by requestAnimationFrame. The key point is that
     * the frame loop and the evented actors share one thread but not one
     * blocking sleep path.
     */
    var listener = try_(net_listenIp(
        rt.net,
        &l$((net_IpAddr){
            .family = net_Addr_Family_ip4,
            .ip4 = {
                .bytes = A_init({ 127, 0, 0, 1 }),
                .port = 3456,
            },
        }),
        l$((net_ListenOpts){
            .mode = net_Sock_Mode_stream,
            .protocol = net_Prot_tcp,
            .kernel_backlog = 4096,
            .reuse_address = true,
            .nonblocking = true,
        })
    ));
    defer_(net_Svr_close(&listener, rt.net));

    var accept_loop = Sched_async$usize(
        rt.net_sched,
        closure_(sample_acceptLoop)(&rt, &listener).as_base
    );
    defer_(let_ignore = Future_cancel$usize(&accept_loop, rt.net_sched));

    for_(($rt(4))(frame)) {
        let_ignore = frame;
        let frame_begin = time_Awake_now(rt.frame_clock);
        sample_frameTick(&rt, frame_begin);
    } $end(for);

    return_ok({});
} $unguarded(fn);
