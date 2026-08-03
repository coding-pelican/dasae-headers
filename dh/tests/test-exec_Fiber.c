#include "dh-main.h"
#include "dh/exec/Fiber.h"
#include "dh/heap/Sys.h"

$static var_(test_exec_Fiber_owner, u8) = 0;
$static var_(test_exec_Fiber_task, u8) = 0;

$static fn_((test_exec_Fiber_noop(P$raw owner, P$raw task))(void)) {
    let_ignore = owner;
    let_ignore = task;
};

TEST_fn_("exec/Fiber: result slab preserves typed aligned storage" $guard) {
    pp_if_(pp_not(co_Fiber_supported))(pp_then_(try_(TEST_skip())));
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var fiber = try_(exec_Fiber_init(
        gpa,
        &test_exec_Fiber_owner,
        &test_exec_Fiber_task,
        test_exec_Fiber_noop,
        typeInfo$(u64)
    ));
    defer_(exec_Fiber_fini(fiber, gpa, typeInfo$(u64)));

    let result = exec_Fiber_resultMut(fiber, typeInfo$(u64));
    try_(TEST_expect(ptrToInt(result.raw) % alignOf$(u64) == 0));
    asg_l((as$(u64*)(result.raw))(u64_(0x0123456789abcdef)));

    let result_const = exec_Fiber_result(fiber, typeInfo$(u64));
    try_(TEST_expect(result_const.raw == result.raw));
    try_(TEST_expect(
        *P_at((as$(const u64*)(result_const.raw))[0]) == u64_(0x0123456789abcdef)
    ));
    return_ok({});
} $unguarded(TEST_fn);
