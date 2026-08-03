#include "dh-main.h"
#include "dh/time/self/Real.h"

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
