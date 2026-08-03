#include "dh-main.h"
#include "dh/Future/self.h"
#include "dh/Sched/self.h"
#include "dh/exec/Seq.h"
#include "dh/heap/Sys.h"

T_use$((u32)(
    Clsr_Ctx, Clsr_Rtn, Clsr,
    Future, Future_await, Future_cancel,
    Sched_async
));
$static fn_((test_Future__addOne(u32 value))(u32)) {
    return value + 1;
};
fn_use_Clsr_((test_Future__addOne)(u32)(u32));

TEST_fn_("Future/self: await consumes scheduler-owned result once" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var seq = exec_Seq_init(heap_Sys_alctr(&heap));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    var future = Sched_async$u32(sched, clsr_((test_Future__addOne)(41)).as_base);

    try_(TEST_expect(isSome(future.any_future)));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
    try_(TEST_expect(isNone(future.any_future)));
    try_(TEST_expect(Future_await$u32(&future, sched) == 42));
} $unguarded(TEST_fn);

TEST_fn_("Future/self: cancel consumes scheduler-owned handle idempotently" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var seq = exec_Seq_init(heap_Sys_alctr(&heap));
    defer_(exec_Seq_fini(&seq));
    let sched = Sched_seq(&seq);
    var future = Sched_async$u32(sched, clsr_((test_Future__addOne)(9)).as_base);

    try_(TEST_expect(isSome(future.any_future)));
    let_ignore = Future_cancel$u32(&future, sched);
    try_(TEST_expect(isNone(future.any_future)));
    let_ignore = Future_cancel$u32(&future, sched);
    try_(TEST_expect(isNone(future.any_future)));
} $unguarded(TEST_fn);
