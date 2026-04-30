#include "dh-main.h"
#include "dh/exec.h"
#include "dh/time.h"
#include "dh/io.h"
#include "dh/fs.h"
#include "dh/fmt/common.h"
#include "dh/heap/Page.h"
#include "dh/Rand.h"

$static fn_((Term_clear(io_Self io))(void)) { io_stream_print(io, u8_l("\x1b[2J\x1b[H")); };
$static fn_((Term_home(io_Self io))(void)) { io_stream_print(io, u8_l("\x1b[1;1H")); };
$static fn_((Term_nl(io_Self io))(void)) { io_stream_nl(io); };
$static fn_((Term_moveCursor(io_Self io, u32 x, u32 y))(void)) {
    io_stream_print(io, u8_l("\x1b[{:u};{:u}H"), y + 1, x + 1);
};
$static fn_((Term_writeTypo(io_Self io, u8 typo))(void)) {
    io_stream_print(io, u8_l("{:c}"), typo);
};
$static fn_((Term_writeTypoAt(io_Self io, u32 x, u32 y, u8 typo))(void)) {
    Term_moveCursor(io, x, y);
    Term_writeTypo(io, typo);
};
$static fn_((Term_writeText(io_Self io, S_const$u8 text))(void)) {
    io_stream_print(io, u8_l("{:s}"), text);
};
$attr($maybe_unused)
$static fn_((Term_writeTextAt(io_Self io, u32 x, u32 y, S_const$u8 text))(void)) {
    Term_moveCursor(io, x, y);
    Term_writeText(io, text);
};

$static fn_((sleepUnchkd(time_Awake time, time_Dur dur))(void)) {
    catch_((time_Awake_sleep(time, dur))($ignore, $do_nothing));
};

$static fn_((trimLine(S$u8 line))(S$u8)) {
    while (line.len > 0) {
        let last = *S_at((line)[line.len - 1]);
        if (last != as$(u8)('\n') && last != as$(u8)('\r')) break;
        line = S_prefix((line)(line.len - 1));
    }
    return line;
};

T_alias$((Runtime)(struct Runtime {
    var_(time, time_Awake);
    var_(io, io_Self);
    var_(fs, fs_Self);
    var_(sched, Sched);
}));

$static fn_((Term_readLine(Runtime rt, S$u8 mem))(S$u8)) {
    var file_io = fs_File_io(io_getStdIn(), rt.fs);
    let bytes = catch_((io_Reader_read(fs_File_IO_reader(&file_io), mem))(
        $ignore, 0
    ));
    return trimLine(S_prefix((mem)(bytes)));
};

T_use$((Void)(Future, Sched_async, Future_await, Future_cancel));
$static fn_((typeEffectWithInterval(Runtime rt, S_const$u8 text, time_Dur interval, u32 x, u32 y))(Void));
fn_use_Closure_((typeEffectWithInterval)(Runtime, S_const$u8, time_Dur, u32, u32)(Void));
$static fn_((typeEffectOverDuration(Runtime rt, S_const$u8 text, f64 dur_secs, u32 x, u32 y))(Void));
fn_use_Closure_((typeEffectOverDuration)(Runtime, S_const$u8, f64, u32, u32)(Void));
$static fn_((typeEffectRealistic(Runtime rt, S_const$u8 text, f64 base_interval_secs, bool add_randomness, u32 x, u32 y))(Void));
fn_use_Closure_((typeEffectRealistic)(Runtime, S_const$u8, f64, bool, u32, u32)(Void));

fn_((typeEffectWithInterval(Runtime rt, S_const$u8 text, time_Dur interval, u32 x, u32 y))(Void) $scope) {
    if (text.len == 0) return_void();
    for_(($rt(text.len))(i)) {
        Term_writeTypoAt(rt.io, x + as$(u32)(i), y, *S_at((text)[i]));
        sleepUnchkd(rt.time, interval);
    } $end(for);
} $unscoped(fn);

fn_((typeEffectOverDuration(Runtime rt, S_const$u8 text, f64 dur_secs, u32 x, u32 y))(Void) $scope) {
    if (text.len == 0) return_void();
    let interval_millis = pri_max(
        (as$(u64)(1)), (as$(u64)((dur_secs * as$(f64)(time_millis_per_sec)) / as$(f64)(text.len)))
    );
    typeEffectWithInterval(rt, text, time_Dur_fromMillis(interval_millis), x, y);
} $unscoped(fn);

fn_((typeEffectRealistic(Runtime rt, S_const$u8 text, f64 base_interval_secs, bool add_randomness, u32 x, u32 y))(Void) $scope) {
    if (text.len == 0) return_void();

    var rand = add_randomness ? Rand_init() : Rand_default;
    for_(($rt(text.len))(i)) {
        let typo = *S_at((text)[i]);
        Term_writeTypoAt(rt.io, x + as$(u32)(i), y, typo);

        var delay_ms = as$(u64)(base_interval_secs * as$(f64)(time_millis_per_sec));
        if (add_randomness) {
            let base_i64 = as$(i64)(delay_ms);
            let jitter = Rand_rangeIInt(&rand, -30, 50);
            let jittered = base_i64 + jitter;
            delay_ms = jittered <= 10 ? 10 : as$(u64)(jittered);

            if (typo == as$(u8)('.') || typo == as$(u8)('!') || typo == as$(u8)('?')) {
                delay_ms += Rand_rangeUInt(&rand, 200, 500);
            } else if (typo == as$(u8)(',') || typo == as$(u8)(';')) {
                delay_ms += Rand_rangeUInt(&rand, 100, 200);
            } else if (typo == as$(u8)(' ')) {
                delay_ms += Rand_rangeUInt(&rand, 20, 80);
            }
        }
        if (delay_ms < 10) delay_ms = 10;
        sleepUnchkd(rt.time, time_Dur_fromMillis(delay_ms));
    } $end(for);
} $unscoped(fn);

pp_if_(Co_Fiber_supported)(
    pp_then_(
        $static fn_((runMain(Runtime rt))(Void) $guard) {
    let sample_text = u8_l("Hello, World! This is a typing effect demonstration.");
    var_(read_mem, A$$(1024, u8)) = A_zero();
    var line = as$(u32)(0);

    Term_clear(rt.io);
    Term_home(rt.io);
    Term_writeText(rt.io, u8_l("=== Typing Effect Demo ==="));
    Term_nl(rt.io);

    var demo1 = closure_(typeEffectWithInterval)(rt, sample_text, time_Dur_fromMillis(100), 0, 1);
    var demo2 = closure_(typeEffectOverDuration)(rt, sample_text, 3.0, 0, 2);
    var demo3 = closure_(typeEffectWithInterval)(rt, sample_text, time_Dur_fromMillis(125), 0, 3);
    var demo4 = closure_(typeEffectOverDuration)(rt, sample_text, 5.0, 0, 4);
    var demo5 = closure_(typeEffectWithInterval)(rt, sample_text, time_Dur_fromMillis(75), 0, 5);
    var demo6 = closure_(typeEffectOverDuration)(rt, sample_text, 4.0, 0, 6);
    var demo7 = closure_(typeEffectRealistic)(rt, sample_text, 0.08, true, 0, 7);

    var task1 = Sched_async$Void(rt.sched, demo1.as_base);
    defer_(Future_cancel$Void(&task1, rt.sched));
    var task2 = Sched_async$Void(rt.sched, demo2.as_base);
    defer_(Future_cancel$Void(&task2, rt.sched));
    var task3 = Sched_async$Void(rt.sched, demo3.as_base);
    defer_(Future_cancel$Void(&task3, rt.sched));
    var task4 = Sched_async$Void(rt.sched, demo4.as_base);
    defer_(Future_cancel$Void(&task4, rt.sched));
    var task5 = Sched_async$Void(rt.sched, demo5.as_base);
    defer_(Future_cancel$Void(&task5, rt.sched));
    var task6 = Sched_async$Void(rt.sched, demo6.as_base);
    defer_(Future_cancel$Void(&task6, rt.sched));
    var task7 = Sched_async$Void(rt.sched, demo7.as_base);
    defer_(Future_cancel$Void(&task7, rt.sched));

    Future_await$Void(&task1, rt.sched);
    Future_await$Void(&task2, rt.sched);
    Future_await$Void(&task3, rt.sched);
    Future_await$Void(&task4, rt.sched);
    Future_await$Void(&task5, rt.sched);
    Future_await$Void(&task6, rt.sched);
    Future_await$Void(&task7, rt.sched);

    line = 9;
    Term_moveCursor(rt.io, 0, line++);
    Term_writeText(rt.io, u8_l("=== Interactive Mode ==="));
    Term_nl(rt.io);

    Term_writeText(rt.io, u8_l("Enter text to type: "));
    let user_text = Term_readLine(rt, A_ref$((S$u8)(read_mem)));
    Term_nl(rt.io);

    if (user_text.len > 0) {
        Term_writeText(rt.io, u8_l("Enter interval in seconds (e.g., 0.1): "));
        let suffix = S_suffix((A_ref$((S$u8)(read_mem)))(user_text.len));
        let interval_text = Term_readLine(rt, suffix);
        Term_nl(rt.io);
        if_ok((fmt_parse$f64(interval_text.as_const))(interval_secs)) blk_defer {
            let prompt = u8_l("Typing your text: ");
            Term_writeText(rt.io, prompt);
            var interactive = closure_(typeEffectWithInterval)(
                rt,
                user_text.as_const,
                time_Dur_fromMillis(as$(u64)(interval_secs * as$(f64)(time_millis_per_sec))),
                (as$(u32)(prompt.len)),
                line
            );
            var interactive_task = Sched_async$Void(rt.sched, interactive.as_base);
            defer_(Future_cancel$Void(&interactive_task, rt.sched));
            Future_await$Void(&interactive_task, rt.sched);
        } blk_deferral else_err((_err)) blk_defer {
            let_ignore = _err;
            let prompt = u8_l("Using default realistic typing: ");
            Term_writeText(rt.io, prompt);
            var interactive = closure_(typeEffectRealistic)(
                rt,
                user_text.as_const,
                0.08,
                true,
                (as$(u32)(prompt.len)),
                line
            );
            var interactive_task = Sched_async$Void(rt.sched, interactive.as_base);
            defer_(Future_cancel$Void(&interactive_task, rt.sched));
            Future_await$Void(&interactive_task, rt.sched);
        } blk_deferral;

        Term_nl(rt.io);
        Term_writeText(rt.io, u8_l("Press Enter to exit."));
        Term_nl(rt.io);
        let_ignore = Term_readLine(rt, A_ref$((S$u8)(read_mem)));
    }
    return_void();
        } $unguarded(fn);
        fn_use_Closure_((runMain)(Runtime)(Void))
    ),
    pp_else_(
        co_fn_(runCoMain, (Runtime rt;), Void);
        co_fn_guard(
            runCoMain,
            co_locals_({
                var_(sample_text, S_const$u8);
            }),
            co_locals_mut_({
                var_(read_mem, A$$(1024, u8));
                var_(line, u32);
                var_(demo1, Closure_(typeEffectWithInterval));
                var_(task1, Future$Void);
                var_(demo2, Closure_(typeEffectOverDuration));
                var_(task2, Future$Void);
                var_(demo3, Closure_(typeEffectWithInterval));
                var_(task3, Future$Void);
                var_(demo4, Closure_(typeEffectOverDuration));
                var_(task4, Future$Void);
                var_(demo5, Closure_(typeEffectWithInterval));
                var_(task5, Future$Void);
                var_(demo6, Closure_(typeEffectOverDuration));
                var_(task6, Future$Void);
                var_(demo7, Closure_(typeEffectRealistic));
                var_(task7, Future$Void);
                var_(interactive_demo, union {
                    var_(parse_ok, Closure_(typeEffectWithInterval));
                    var_(parse_err, Closure_(typeEffectRealistic));
                });
                var_(interactive_task, Future$Void);
            }),
            co_suspended_({
                var_(awaiting, Void);
            }),
            co_deferrable_(10)
        ) {
    co_let_(sample_text) = $co_init(sample_text)(u8_l("Hello, World! This is a typing effect demonstration."));
    co_var_(read_mem) = $co_init_mut(read_mem)(A_zero());
    co_var_(line) = $co_init_mut(line)(0);

    Term_clear($co_arg(rt).io);
    Term_home($co_arg(rt).io);
    Term_writeText($co_arg(rt).io, u8_l("=== Typing Effect Demo ==="));
    Term_nl($co_arg(rt).io);

    co_var_(demo1) = closure_(typeEffectWithInterval)($co_arg(rt), $co(sample_text), time_Dur_fromMillis(100), 0, 1);
    co_var_(demo2) = closure_(typeEffectOverDuration)($co_arg(rt), $co(sample_text), 3.0, 0, 2);
    co_var_(demo3) = closure_(typeEffectWithInterval)($co_arg(rt), $co(sample_text), time_Dur_fromMillis(125), 0, 3);
    co_var_(demo4) = closure_(typeEffectOverDuration)($co_arg(rt), $co(sample_text), 5.0, 0, 4);
    co_var_(demo5) = closure_(typeEffectWithInterval)($co_arg(rt), $co(sample_text), time_Dur_fromMillis(75), 0, 5);
    co_var_(demo6) = closure_(typeEffectOverDuration)($co_arg(rt), $co(sample_text), 4.0, 0, 6);
    co_var_(demo7) = closure_(typeEffectRealistic)($co_arg(rt), $co(sample_text), 0.08, true, 0, 7);

    co_var_(task1) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo1).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task1), $co_arg(rt).sched));
    co_var_(task2) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo2).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task2), $co_arg(rt).sched));
    co_var_(task3) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo3).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task3), $co_arg(rt).sched));
    co_var_(task4) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo4).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task4), $co_arg(rt).sched));
    co_var_(task5) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo5).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task5), $co_arg(rt).sched));
    co_var_(task6) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo6).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task6), $co_arg(rt).sched));
    co_var_(task7) = Sched_async$Void($co_arg(rt).sched, $co_mut(demo7).as_base);
    co_defer_(Future_cancel$Void(&$co_mut(task7), $co_arg(rt).sched));

    suspend_((awaiting)(Future_await$Void(&$co_mut(task1), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task2), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task3), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task4), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task5), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task6), $co_arg(rt).sched)));
    suspend_((awaiting)(Future_await$Void(&$co_mut(task7), $co_arg(rt).sched)));

    $co_mut(line) = 9;
    Term_moveCursor($co_arg(rt).io, 0, $co_mut(line++));
    Term_writeText($co_arg(rt).io, u8_l("=== Interactive Mode ==="));
    Term_nl($co_arg(rt).io);

    Term_writeText($co_arg(rt).io, u8_l("Enter text to type: "));
    let user_text = Term_readLine($co_arg(rt), A_ref$((S$u8)($co_mut(read_mem))));
    Term_nl($co_arg(rt).io);

    if (user_text.len > 0) {
        Term_writeText($co_arg(rt).io, u8_l("Enter interval in seconds (e.g., 0.1): "));
        let suffix = S_suffix((A_ref$((S$u8)($co_mut(read_mem))))(user_text.len));
        let interval_text = Term_readLine($co_arg(rt), suffix);
        Term_nl($co_arg(rt).io);
        if_ok((fmt_parse$f64(interval_text.as_const))(interval_secs)) co_blk_defer {
            let prompt = u8_l("Typing your text: ");
            Term_writeText($co_arg(rt).io, prompt);
            co_var_(interactive_demo) = $co_init_mut(interactive_demo)(closure_(typeEffectWithInterval)(
                $co_arg(rt),
                user_text.as_const,
                time_Dur_fromMillis(as$(u64)(interval_secs * as$(f64)(time_millis_per_sec))),
                as$(u32)(prompt.len),
                $co_mut(line)
            ));
            co_var_(interactive_task) = Sched_async$Void($co_arg(rt).sched, $co_mut(interactive_demo).parse_ok.as_base);
            co_defer_(Future_cancel$Void(&$co_mut(interactive_task), $co_arg(rt).sched));
            suspend_((awaiting)(Future_await$Void(&$co_mut(interactive_task), $co_arg(rt).sched)));
        } co_blk_deferral else_err((_err)) co_blk_defer {
            let_ignore = _err;
            let prompt = u8_l("Using default realistic typing: ");
            Term_writeText($co_arg(rt).io, prompt);
            co_var_(interactive_demo) = $co_init_mut(interactive_demo)(closure_(typeEffectRealistic)(
                $co_arg(rt),
                user_text.as_const,
                0.08, true,
                as$(u32)(prompt.len),
                $co_mut(line)
            ));
            co_var_(interactive_task) = Sched_async$Void($co_arg(rt).sched, $co_mut(interactive_demo).parse_err.as_base);
            co_defer_(Future_cancel$Void(&$co_mut(interactive_task), $co_arg(rt).sched));
            suspend_((awaiting)(Future_await$Void(&$co_mut(interactive_task), $co_arg(rt).sched)));
        } co_blk_deferral;

        Term_nl($co_arg(rt).io);
        Term_writeText($co_arg(rt).io, u8_l("Press Enter to exit."));
        Term_nl($co_arg(rt).io);
        let_ignore = Term_readLine($co_arg(rt), A_ref$((S$u8)($co_mut(read_mem))));
    }
    co_return_({});
        } $unguarded(co_fn);
        co_use_Closure_((runCoMain)(Runtime)(Void))
    ));


fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let gpa = heap_Page_alctr(&l0$((heap_Page)));
    var loop = exec_Coop_init(gpa, try_(time_Awake_direct()), exec_Evented_noop);
    defer_(exec_Coop_fini(&loop));

    let_(rt, Runtime) = {
        .time = time_Awake_evented(&loop),
        .io = io_evented(&loop),
        .fs = fs_evented(&loop),
        .sched = Sched_coop(&loop),
    };
    var main_task = closure_(runMain)(rt);
    var future = Sched_async$Void(rt.sched, main_task.as_base);
    defer_(Future_cancel$Void(&future, rt.sched));
    return_ok(Future_await$Void(&future, rt.sched));
} $unguarded(fn);
