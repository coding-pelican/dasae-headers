#include "dh-main.h"
#include "dh/time/self/Awake.h"

TEST_fn_("time/self/Awake: noop source has zero time and explicit failures" $scope) {
    let resolution_failed = eval_(bool $scope)(catch_((time_Awake_resoln(
        time_Awake_noop
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    let sleep_canceled = eval_(bool $scope)(catch_((time_Awake_sleepNanos(
        time_Awake_noop, 1
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$Sched_Canceled));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);

    try_(TEST_expect(time_Awake_isValid(time_Awake_noop)));
    try_(TEST_expect(time_Awake_isValid(time_Awake_failing)));
    try_(TEST_expect(time_Inst_isZero(time_Awake_now(time_Awake_noop).raw)));
    try_(TEST_expect(resolution_failed));
    try_(TEST_expect(sleep_canceled));
} $unscoped(TEST_fn);

TEST_fn_("time/self/Awake: monotonic clock advances across sleep" $scope) {
    let clock = catch_((time_Awake_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let begin = time_Awake_now(clock);
    try_(time_Awake_sleepMillis(clock, 1));
    let end = time_Awake_now(clock);
    let elapsed = unwrap_(time_Awake_Inst_durSinceChkd(end, begin));

    try_(TEST_expect(cmp_ge$(time_Awake_Inst)(end, begin)));
    try_(TEST_expect(!time_Dur_isZero(elapsed)));
    try_(TEST_expect(isNone(time_Awake_Inst_durSinceChkd(begin, end))));
} $unscoped(TEST_fn);

TEST_fn_("time/self/Awake: direct resolution is non-zero when supported" $scope) {
    let clock = catch_((time_Awake_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let resolution = try_(time_Awake_resoln(clock));
    try_(TEST_expect(!time_Dur_isZero(resolution)));
} $unscoped(TEST_fn);
