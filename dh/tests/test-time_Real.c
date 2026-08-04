#include "dh-main.h"
#include "dh/time/self/Real.h"

TEST_fn_("time/self/Real: noop source has zero time and explicit resolution failure" $scope) {
    let resolution_failed = eval_(bool $scope)(catch_((time_Real_resolution(
        time_Real_noop
    ))(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);

    try_(TEST_expect(time_Real_isValid(time_Real_noop)));
    try_(TEST_expect(time_Real_isValid(time_Real_failing)));
    try_(TEST_expect(time_Inst_isZero(time_Real_now(time_Real_noop).raw)));
    try_(TEST_expect(resolution_failed));
} $unscoped(TEST_fn);

TEST_fn_("time/self/Real: Unix epoch conversion preserves duration" $scope) {
    let epoch = time_Real_Inst_fromUnixEpoch(0);
    let later = time_Real_Inst_fromUnixEpoch(123);
    let duration = unwrap_(time_Real_Inst_durSinceChkd(later, epoch));

    try_(TEST_expect(time_Real_Inst_toUnixEpoch(epoch) == 0));
    try_(TEST_expect(time_Real_Inst_toUnixEpoch(later) == 123));
    try_(TEST_expect(duration.secs == 123));
    try_(TEST_expect(duration.nanos == 0));
    try_(TEST_expect(time_Real_Inst_lt(epoch, later)));
    try_(TEST_expect(isNone(time_Real_Inst_durSinceChkd(epoch, later)))); /* NOLINT(readability-suspicious-call-argument) */
} $unscoped(TEST_fn);
