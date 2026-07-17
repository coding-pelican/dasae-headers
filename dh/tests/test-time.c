#include "dh-main.h"
#include "dh/time.h"

TEST_fn_("time/Dur: constructors accessors and comparisons" $scope) {
    let zero = time_Dur_zero;
    let sec = time_Dur_fromSecs(1);
    let millis = time_Dur_fromMillis(1500);
    let micros = time_Dur_fromMicros(u64_(1, 500, 000));
    let nanos = time_Dur_fromNanos(u64_(1, 500, 000, 000));

    try_(TEST_expect(time_Dur_isZero(zero)));
    try_(TEST_expect(sec.secs == 1));
    try_(TEST_expect(sec.nanos == 0));
    try_(TEST_expect(millis.secs == 1));
    try_(TEST_expect(millis.nanos == 500 * time_nanos_per_milli));
    try_(TEST_expect(time_Dur_eq(millis, micros)));
    try_(TEST_expect(time_Dur_eq(micros, nanos)));
    try_(TEST_expect(time_Dur_lt(sec, millis)));
    try_(TEST_expect(time_Dur_subsecMillis(millis) == 500));
    try_(TEST_expect(time_Dur_subsecMicros(millis) == 500000));
    try_(TEST_expect(time_Dur_subsecNanos(millis) == 500000000));
} $unscoped(TEST_fn);

TEST_fn_("time/Dur: checked arithmetic" $scope) {
    let one_sec = time_Dur_fromSecs(1);
    let half_sec = time_Dur_fromMillis(500);
    let one_and_half = time_Dur_fromMillis(1500);

    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_addChkd(one_sec, half_sec)), one_and_half)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_subChkd(one_and_half, one_sec)), half_sec)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_mulChkd$u32(half_sec, 3)), one_and_half)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_divChkd$u32(one_and_half, 3)), half_sec)));
    try_(TEST_expect(isNone(time_Dur_subChkd(one_sec, one_and_half))));
    try_(TEST_expect(isNone(time_Dur_divChkd$u32(one_sec, 0))));
} $unscoped(TEST_fn);

TEST_fn_("time/Dur: checked arithmetic normalizes results and saturation clamps" $scope) {
    let max = time_Dur_from(u64_limit_max, time_nanos_per_sec - 1);
    let one_nano = time_Dur_nano;
    let one_sec = time_Dur_fromSecs(1);

    let half = unwrap_(time_Dur_divChkd$u32(one_sec, 2));
    try_(TEST_expect(half.secs == 0));
    try_(TEST_expect(half.nanos == time_nanos_per_sec / 2));
    try_(TEST_expect(isNone(time_Dur_addChkd(max, one_nano))));
    try_(TEST_expect(isNone(time_Dur_mulChkd$u32(max, 2))));
    try_(TEST_expect(time_Dur_eq(time_Dur_addSat(max, one_nano), max)));
    try_(TEST_expect(time_Dur_eq(time_Dur_subSat(one_sec, time_Dur_fromSecs(2)), time_Dur_zero)));
    try_(TEST_expect(time_Dur_eq(time_Dur_mulSat$u32(max, 2), max)));
} $unscoped(TEST_fn);

TEST_fn_("time/Inst: constructor normalizes nanos and checked arithmetic rejects overflow" $scope) {
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

TEST_fn_("time/Awake: monotonic duration since earlier instant" $scope) {
    let clock = catch_((time_Awake_direct())($ignore, return_ok(try_(TEST_skip()))));
    let begin = time_Awake_now(clock);
    try_(time_Awake_sleepMillis(clock, 1));
    let end = time_Awake_now(clock);
    let elapsed = unwrap_(time_Awake_Inst_durSinceChkd(end, begin));

    try_(TEST_expect(cmp_ge$(time_Awake_Inst)(end, begin)));
    try_(TEST_expect(!time_Dur_isZero(elapsed)));
    try_(TEST_expect(isNone(time_Awake_Inst_durSinceChkd(begin, end))));
} $unscoped(TEST_fn);

TEST_fn_("time/Awake: direct resolution is a non-zero duration when supported" $scope) {
    let clock = catch_((time_Awake_direct())($ignore, return_ok(try_(TEST_skip()))));
    let resolution = try_(time_Awake_resolution(clock));

    try_(TEST_expect(!time_Dur_isZero(resolution)));
} $unscoped(TEST_fn);

TEST_fn_("time/Real: unix epoch conversion and duration" $scope) {
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
