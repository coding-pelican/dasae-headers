#include "dh-main.h"
#include "dh/exec/Lane.h"
#include "dh/exec/Task.h"
#include "dh/heap/Sys.h"

T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
$static fn_((test_exec_Lane__addOne(u32 value))(u32)) {
    return value + 1;
};
fn_use_Clsr_((test_exec_Lane__addOne)(u32)(u32));

TEST_fn_("exec/Lane: ready task runs through lane-owned result storage" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var lane = exec_Lane_init(heap_Sys_alctr(&heap));
    defer_(exec_Lane_fini(&lane));
    var_(result, u32) = 0;
    var clsr = clsr_((test_exec_Lane__addOne)(41));
    let task = unwrap_(exec_Lane_spawnTask(&lane, u_anyP(&result), clsr.as_raw));

    try_(TEST_expect(task->state == exec_Task_State_ready));
    try_(TEST_expect(exec_Lane_runOneReady(&lane)));
    try_(TEST_expect(task->state == exec_Task_State_done));
    try_(TEST_expect(*P_at((u_castP$((u32*)(task->result)))[0]) == 42));
    try_(TEST_expect(!exec_Lane_runOneReady(&lane)));
    try_(TEST_expect(isNone(exec_Lane_task(&lane))));
} $unguarded(TEST_fn);
