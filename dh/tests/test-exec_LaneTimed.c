#include "dh-main.h"
#include "dh/exec/LaneTimed.h"
#include "dh/exec/Task.h"
#include "dh/heap/Sys.h"

T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((exec_Timer)(ArrPQue_enque));
$static fn_((test_exec_LaneTimed__identity(u32 value))(u32)) {
    return value;
};
fn_use_Clsr_((test_exec_LaneTimed__identity)(u32)(u32));

TEST_fn_("exec/LaneTimed: due timer returns waiting task to ready queue" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let clock = catch_((time_Awake_direct())(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("monotonic clock is unavailable"))))
    ));
    var timed = exec_LaneTimed_init(heap_Sys_alctr(&heap), clock);
    defer_(exec_LaneTimed_fini(&timed));
    var_(result, u32) = 0;
    var clsr = clsr_((test_exec_LaneTimed__identity)(7));
    let task = unwrap_(exec_Lane_spawnTask(&timed.lane, u_anyP(&result), clsr.as_raw));
    task->state = exec_Task_State_waiting;
    try_(ArrPQue_enque$exec_Timer(
        &timed.tasks_timer,
        timed.lane.gpa,
        (exec_Timer){
            .deadline = time_Awake_now(clock).raw,
            .task = task,
        }
    ));

    try_(TEST_expect(isSome(exec_LaneTimed_nextTimerDeadline(&timed))));
    exec_LaneTimed_wakeDueTimers(&timed);
    try_(TEST_expect(task->state == exec_Task_State_ready));
    try_(TEST_expect(isNone(exec_LaneTimed_nextTimerDeadline(&timed))));
    try_(TEST_expect(exec_Lane_runOneReady(&timed.lane)));
    try_(TEST_expect(task->state == exec_Task_State_done));
    try_(TEST_expect(*P_at((u_castP$((u32*)(task->result)))[0]) == 7));
} $unguarded(TEST_fn);
