#include "dh-main.h"
#include "dh/exec/Fiber.h"
#include "dh/exec/Fiber-growable.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"
#include "dh/co/Fiber.h"

$static fn_((test_fiberGrowDebug_noop(P$raw owner, P$raw task))(void)) {
    let_ignore = owner;
    let_ignore = task;
};

$static var_(test_fiberGrowDebug_owner, u8) = 0;
$static var_(test_fiberGrowDebug_task, u8) = 0;

TEST_fn_("exec/Fiber - grow debug: proactive ensure from saved rsp" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);
    let policy = exec_Fiber_defaultStackPolicy();
    var fiber = try_(exec_Fiber_initWithPolicy(
        gpa,
        &test_fiberGrowDebug_owner,
        &test_fiberGrowDebug_task,
        test_fiberGrowDebug_noop,
        typeInfo$(i32),
        policy
    ));
    defer_(exec_Fiber_fini(fiber, gpa));
    let rsp = co_Fiber_Context_stackPtr(&fiber->context);
    let storage_begin = ptrToInt(fiber->storage.ptr);
    let storage_end = storage_begin + fiber->storage.len;
    try_(TEST_expect(fiber->is_virtual));
    try_(TEST_expect(fiber->guard_size != 0));
    try_(TEST_expect(rsp >= storage_begin));
    try_(TEST_expect(rsp <= storage_end));
    try_(TEST_expect(fiber->stack.len == mem_page_size));
    let_ignore = exec_Fiber_enter(fiber);
    exec_Fiber__ensureStackHeadroom(fiber, rsp, exec_Fiber_stack_switch_headroom);
    return_ok({});
} $unguarded(TEST_fn);
