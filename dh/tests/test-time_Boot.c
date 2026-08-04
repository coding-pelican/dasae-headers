#include "dh-main.h"
#include "dh/time/self/Boot.h"

TEST_fn_("time/self/Boot: noop source has zero time and explicit failures" $scope) {
    let resolution_failed = eval_(bool $scope)(catch_((time_Boot_resoln(
        time_Boot_noop
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    let sleep_canceled = eval_(bool $scope)(catch_((time_Boot_sleepNanos(
        time_Boot_noop, 1
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$Sched_Canceled));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);

    try_(TEST_expect(time_Boot_isValid(time_Boot_noop)));
    try_(TEST_expect(time_Boot_isValid(time_Boot_failing)));
    try_(TEST_expect(time_Inst_isZero(time_Boot_now(time_Boot_noop).raw)));
    try_(TEST_expect(resolution_failed));
    try_(TEST_expect(sleep_canceled));
} $unscoped(TEST_fn);

TEST_fn_("time/self/Boot: direct clock advances across sleep" $scope) {
    let clock = catch_((time_Boot_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let begin = time_Boot_now(clock);
    try_(time_Boot_sleepMillis(clock, 1));
    let end = time_Boot_now(clock);
    let elapsed = unwrap_(time_Boot_Inst_durSinceChkd(end, begin));

    try_(TEST_expect(cmp_ge$(time_Boot_Inst)(end, begin)));
    try_(TEST_expect(!time_Dur_isZero(elapsed)));
} $unscoped(TEST_fn);
