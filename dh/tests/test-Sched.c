#include "dh-main.h"
#include "dh/Future.h"
#include "dh/Sched/self.h"

T_use$((u32)(Clsr_Ctx, Clsr_Rtn, Clsr, Future, Sched_spawn));
$static fn_((test_Sched__identity(u32 value))(u32)) {
    return value;
};
fn_use_Clsr_((test_Sched__identity)(u32)(u32));

TEST_fn_("Sched/self: failing scheduler preserves unavailable and canceled contracts" $scope) {
    try_(TEST_expect(Sched_isValid(Sched_noop)));
    try_(TEST_expect(Sched_isValid(Sched_failing)));

    if_err((Sched_spawn$u32(Sched_failing, clsr_((test_Sched__identity)(1)).as_base))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_ConcUnavailable().as_any)));
    } else_ok(future) {
        let_ignore = future;
        try_(TEST_expect(false));
    }

    if_err((Sched_idle(Sched_failing))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$Sched_Canceled().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unscoped(TEST_fn);
