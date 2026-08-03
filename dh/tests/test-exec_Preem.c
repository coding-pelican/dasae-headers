#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec/Preem.h"
#include "dh/heap/Sys.h"
#include "dh/time/self/Awake.h"
#include "dh/time/Dur.h"

T_use$((u32)(
    Clsr_Ctx,
    Clsr_Rtn,
    Clsr,
    Future,
    Future_await,
    Future_cancel,
    Sched_spawn
));

$static fn_((test_exec_Preem_addOne(u32 value))(u32)) {
    return value + 1;
};
fn_use_Clsr_((test_exec_Preem_addOne)(u32)(u32));

$static fn_((test_exec_Preem_sleepThenReturn(time_Awake time, time_Dur dur, u32 value))(u32)) {
    catch_((time_Awake_sleep(time, dur))($ignore, $do_nothing));
    return value;
};
fn_use_Clsr_((test_exec_Preem_sleepThenReturn)(time_Awake, time_Dur, u32)(u32));

TEST_fn_("exec/Preem: await joins spawned thread and copies result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);

    var clsr = clsr_((test_exec_Preem_addOne)(41));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Preem: cancel joins running thread and discards task result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);
    let time = try_(time_Awake_direct());

    var clsr = clsr_((test_exec_Preem_sleepThenReturn)(time, time_Dur_fromMillis(25), 123));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    try_(TEST_expect(Future_cancel$u32(&future, sched) == 0));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Preem: cancel after completion joins thread and preserves result" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var preem = exec_Preem_init(heap_Sys_alctr(&heap));
    defer_(exec_Preem_fini(&preem));
    let sched = Sched_preem(&preem);
    let time = try_(time_Awake_direct());

    var clsr = clsr_((test_exec_Preem_sleepThenReturn)(time, time_Dur_fromMillis(1), 321));
    var future = try_(Sched_spawn$u32(sched, clsr.as_base));
    catch_((time_Awake_sleep(time, time_Dur_fromMillis(25)))($ignore, $do_nothing));
    try_(TEST_expect(Future_cancel$u32(&future, sched) == 321));
    try_(TEST_expect(isNone(future.any_future)));
    return_ok({});
} $unguarded(TEST_fn);
