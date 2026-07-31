#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/heap/Sys.h"
#include "dh/heap/ThrdSafe.h"
#include "dh/time/Dur.h"
#include "dh/time/self/Awake.h"
#include "dh/io/stream.h"

$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), thrd_currId(), label);
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};
$static fn_((sleepMillis(time_Awake clock, u64 millis))(void)) {
    catch_((time_Awake_sleepMillis(clock, millis))($ignore, $do_nothing));
};

T_use$((u32)(
    thrd_Que,
    thrd_Que_init,
    thrd_Que_fini,
    thrd_Que_trySend,
    thrd_Que_recvOp
));
T_use$((i64)(
    thrd_Que,
    thrd_Que_init,
    thrd_Que_fini,
    thrd_Que_trySend,
    thrd_Que_recvOp
));
T_use$((Void)(thrd_spawn, thrd_join));

typedef struct MetricProducer {
    var_(clock, time_Awake);
    var_(out, thrd_Que$u32*);
} MetricProducer;
$static fn_((metricProducer(MetricProducer* self))(Void) $scope) {
    for_(($r(0, 3))(i)) {
        sleepMillis(self->clock, 80);
        let value = as$(u32)(100 + i);
        catch_((thrd_Que_trySend$u32(self->out, value))($ignore, return_void()));
        report(u8_l("metric"), u8_l("sent {:u}"), value);
    } $end(for);
    return_void();
} $unscoped(fn);
fn_use_Clsr_((metricProducer)(MetricProducer*)(Void));

typedef struct AlertProducer {
    var_(clock, time_Awake);
    var_(out, thrd_Que$i64*);
} AlertProducer;
$static fn_((alertProducer(AlertProducer* self))(Void) $scope) {
    for_(($r(0, 3))(i)) {
        sleepMillis(self->clock, 130);
        let value = as$(i64)(-10 - as$(i64)(i));
        catch_((thrd_Que_trySend$i64(self->out, value))($ignore, return_void()));
        report(u8_l("alert"), u8_l("sent {:d}"), value);
    } $end(for);
    return_void();
} $unscoped(fn);
fn_use_Clsr_((alertProducer)(AlertProducer*)(Void));

typedef variant_((SelectMsg $fits($packed))(
    (SelectMsg_metric, u32),
    (SelectMsg_alert, i64)
)) SelectMsg;
T_use_thrd_Select_Arm$(SelectMsg);
T_use$((SelectMsg)(
    thrd_Select_Arm_from,
    thrd_Select_Arm_take,
    thrd_Select_fixed,
    thrd_Select_from,
    thrd_Select_watchWithin,
    thrd_Select_waitMutProtcd
));
fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;

    var sys_heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys_heap));
    var thrd_safe_heap = heap_ThrdSafe_init(heap_Sys_alctr(&sys_heap));
    defer_(heap_ThrdSafe_fini(&thrd_safe_heap));
    let gpa = heap_ThrdSafe_alctr(&thrd_safe_heap);
    let spawn_cfg = thrd_SpawnCfg_default(gpa);
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));

    var metric_buf = A_zero$((A$$(4, u32)));
    var metric_q = thrd_Que_init$u32(A_ref$((S$u32)(metric_buf)));
    defer_(thrd_Que_fini$u32(&metric_q));
    var alert_buf = A_zero$((A$$(4, i64)));
    var alert_q = thrd_Que_init$i64(A_ref$((S$i64)(alert_buf)));
    defer_(thrd_Que_fini$i64(&alert_q));

    var metric_ctx = (MetricProducer){
        .clock = clock,
        .out = &metric_q,
    };
    var metric_work = clsr_((metricProducer)(&metric_ctx));
    let metric_thread = try_(thrd_spawn$Void(spawn_cfg, metric_work.as_base));
    defer_(thrd_join$Void(metric_thread));
    var alert_ctx = (AlertProducer){
        .clock = clock,
        .out = &alert_q,
    };
    var alert_work = clsr_((alertProducer)(&alert_ctx));
    let alert_thread = try_(thrd_spawn$Void(spawn_cfg, alert_work.as_base));
    defer_(thrd_join$Void(alert_thread));

    for_(($r(0, 3))($ignore)) {
        var select_buf = A_zero$((A$$(2, thrd_Select_Arm$SelectMsg)));
        var select = thrd_Select_fixed$SelectMsg(A_ref$((S$thrd_Select_Arm$SelectMsg)(select_buf)));
        var metric_recv = thrd_Que_recvOp$u32(&metric_q);
        thrd_Select_watchWithin$SelectMsg(
            &select, SelectMsg_metric, thrd_Que_RecvOp_op(&metric_recv)
        );
        var alert_recv = thrd_Que_recvOp$i64(&alert_q);
        thrd_Select_watchWithin$SelectMsg(
            &select, SelectMsg_alert, thrd_Que_RecvOp_op(&alert_recv)
        );
        let selected = thrd_Select_waitMutProtcd$SelectMsg(&select);
        match_((thrd_Select_Arm_take$SelectMsg(selected))) {
        patt_((SelectMsg_metric)(val)) {
            report(u8_l("select"), u8_l("received metric {:u}"), val);
        } $end(patt);
        patt_((SelectMsg_alert)(val)) {
            report(u8_l("select"), u8_l("received alert {:d}"), val);
        } $end(patt);
        } $end(match);
    }  $end(for);

    for_(($r(0, 3))($ignore)) {
        var select_recv_ops = A_from$((thrd_Que_RecvOp){
            thrd_Que_recvOp$u32(&metric_q),
            thrd_Que_recvOp$i64(&alert_q),
        });
        var select_arms = A_from$((thrd_Select_Arm$SelectMsg){
            thrd_Select_Arm_from$SelectMsg(SelectMsg_metric, thrd_Que_RecvOp_op(A_at((select_recv_ops)[0]))),
            thrd_Select_Arm_from$SelectMsg(SelectMsg_alert, thrd_Que_RecvOp_op(A_at((select_recv_ops)[1]))),
        });
        var select = thrd_Select_from$SelectMsg(A_ref$((S$thrd_Select_Arm$SelectMsg)(select_arms)));
        let selected = thrd_Select_waitMutProtcd$SelectMsg(&select);
        match_((thrd_Select_Arm_take$SelectMsg(selected))) {
        patt_((SelectMsg_metric)(val)) {
            report(u8_l("select"), u8_l("received metric {:u}"), val);
        } $end(patt);
        patt_((SelectMsg_alert)(val)) {
            report(u8_l("select"), u8_l("received alert {:d}"), val);
        } $end(patt);
        } $end(match);
    }  $end(for);

    return_ok_void(report(u8_l("main"), u8_l("select sample completed")));
} $unguarded(fn);
