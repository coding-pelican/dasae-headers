#include "dh-main.h"
#include "dh/time/self/Thrd.h"

TEST_fn_("time/self/Thrd: noop source has zero time and explicit resolution failure" $scope) {
    let resolution_failed = eval_(bool $scope)(catch_((time_Thrd_resoln(
        time_Thrd_noop
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);

    try_(TEST_expect(time_Thrd_isValid(time_Thrd_noop)));
    try_(TEST_expect(time_Thrd_isValid(time_Thrd_failing)));
    try_(TEST_expect(time_Inst_isZero(time_Thrd_now(time_Thrd_noop).raw)));
    try_(TEST_expect(resolution_failed));
} $unscoped(TEST_fn);

TEST_fn_("time/self/Thrd: direct thread clock is monotonic" $scope) {
    let clock = catch_((time_Thrd_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let begin = time_Thrd_now(clock);
    let end = time_Thrd_now(clock);

    try_(TEST_expect(cmp_ge$(time_Thrd_Inst)(end, begin)));
    try_(TEST_expect(isSome(time_Thrd_Inst_durSinceChkd(end, begin))));
} $unscoped(TEST_fn);
