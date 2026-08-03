#include "dh-main.h"
#include "dh/exec/Task.h"

TEST_fn_("exec/Task: cancel protection defers consumption and recancel re-arms" $guard) {
    var_(task, exec_Task) = {
        .state = exec_Task_State_running,
        .cancel = exec_Task_Cancel_none,
        .cancel_protection = Sched_CancelProtcn_unblocked,
        .result = cleared(),
        .inner = cleared(),
        .fiber = none(),
    };

    try_(TEST_expect(!exec_Task_hasCancelRequest(&task)));
    exec_Task_requestCancel(&task);
    try_(TEST_expect(exec_Task_hasCancelRequest(&task)));

    try_(TEST_expect(
        exec_Task_swapCancelProtcn(&task, Sched_CancelProtcn_blocked)
        == Sched_CancelProtcn_unblocked
    ));
    try_(TEST_expect(isOk(exec_Task_idle(&task))));
    try_(TEST_expect(exec_Task_hasCancelRequest(&task)));

    try_(TEST_expect(
        exec_Task_swapCancelProtcn(&task, Sched_CancelProtcn_unblocked)
        == Sched_CancelProtcn_blocked
    ));
    try_(TEST_expect(isErr(exec_Task_idle(&task))));
    try_(TEST_expect(!exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(task.cancel == exec_Task_Cancel_acknowledged));

    exec_Task_recancel(&task);
    try_(TEST_expect(exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(isErr(exec_Task_idle(&task))));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("exec/Task: completed task rejects new cancel requests" $guard) {
    var_(task, exec_Task) = {
        .state = exec_Task_State_done,
        .cancel = exec_Task_Cancel_none,
        .cancel_protection = Sched_CancelProtcn_unblocked,
        .result = cleared(),
        .inner = cleared(),
        .fiber = none(),
    };

    try_(TEST_expect(exec_Task_isDone(&task)));
    exec_Task_requestCancel(&task);
    try_(TEST_expect(!exec_Task_hasCancelRequest(&task)));
    try_(TEST_expect(task.cancel == exec_Task_Cancel_none));
    return_ok({});
} $unguarded(TEST_fn);
