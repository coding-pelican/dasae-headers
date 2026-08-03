#include "dh-main.h"
#include "dh/time/Inst.h"

TEST_fn_("time/Inst: normalization and checked arithmetic preserve ordering" $scope) {
    let normalized = time_Inst_from(1, time_nanos_per_sec + 5);
    let later = time_Inst_from(3, 100);
    let earlier = time_Inst_from(1, 200);
    let diff = unwrap_(time_Inst_durSinceChkd(later, earlier));
    let max = time_Inst_from(u64_limit_max, time_nanos_per_sec - 1);

    try_(TEST_expect(normalized.secs == 2));
    try_(TEST_expect(normalized.nanos == 5));
    try_(TEST_expect(time_Inst_ticks(time_Inst_from(1, 2)) == as$(u64)(time_nanos_per_sec) + 2));
    try_(TEST_expect(diff.secs == 1));
    try_(TEST_expect(diff.nanos == time_nanos_per_sec - 100));
    try_(TEST_expect(isNone(time_Inst_durSinceChkd(earlier, later))));
    try_(TEST_expect(isNone(time_Inst_addChkdDur(max, time_Dur_nano))));
    try_(TEST_expect(isNone(time_Inst_subChkdDur(time_Inst_from(0, 0), time_Dur_nano))));
} $unscoped(TEST_fn);
