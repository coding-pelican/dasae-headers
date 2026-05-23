#include "dh-main.h"
#include "dh/time.h"

TEST_fn_("time/Duration: constructors accessors and comparisons" $scope) {
    let zero = time_Duration_zero;
    let sec = time_Duration_fromSecs(1);
    let millis = time_Duration_fromMillis(1500);
    let micros = time_Duration_fromMicros(u64_(1, 500, 000));
    let nanos = time_Duration_fromNanos(u64_(1, 500, 000, 000));

    try_(TEST_expect(time_Duration_isZero(zero)));
    try_(TEST_expect(sec.secs == 1));
    try_(TEST_expect(sec.nanos == 0));
    try_(TEST_expect(millis.secs == 1));
    try_(TEST_expect(millis.nanos == 500 * time_nanos_per_milli));
    try_(TEST_expect(time_Duration_eq(millis, micros)));
    try_(TEST_expect(time_Duration_eq(micros, nanos)));
    try_(TEST_expect(time_Duration_lt(sec, millis)));
    try_(TEST_expect(time_Duration_subsecMillis(millis) == 500));
    try_(TEST_expect(time_Duration_subsecMicros(millis) == 500000));
    try_(TEST_expect(time_Duration_subsecNanos(millis) == 500000000));
} $unscoped(TEST_fn);

TEST_fn_("time/Duration: checked arithmetic" $scope) {
    let one_sec = time_Duration_fromSecs(1);
    let half_sec = time_Duration_fromMillis(500);
    let one_and_half = time_Duration_fromMillis(1500);

    try_(TEST_expect(time_Duration_eq(unwrap_(time_Duration_addChkd(one_sec, half_sec)), one_and_half)));
    try_(TEST_expect(time_Duration_eq(unwrap_(time_Duration_subChkd(one_and_half, one_sec)), half_sec)));
    try_(TEST_expect(time_Duration_eq(unwrap_(time_Duration_mulChkd$u32(half_sec, 3)), one_and_half)));
    try_(TEST_expect(time_Duration_eq(unwrap_(time_Duration_divChkd$u32(one_and_half, 3)), half_sec)));
    try_(TEST_expect(isNone(time_Duration_subChkd(one_sec, one_and_half))));
    try_(TEST_expect(isNone(time_Duration_divChkd$u32(one_sec, 0))));
} $unscoped(TEST_fn);

TEST_fn_("time/Instant: monotonic duration since earlier instant" $scope) {
    let begin = time_Instant_now();
    time_sleepMillis(1);
    let end = time_Instant_now();
    let elapsed = unwrap_(time_Instant_durationSinceChkd(end, begin));

    try_(TEST_expect(cmp_ge$(time_Instant)(end, begin)));
    try_(TEST_expect(!time_Duration_isZero(elapsed)));
    try_(TEST_expect(isNone(time_Instant_durationSinceChkd(begin, end))));
} $unscoped(TEST_fn);

TEST_fn_("time/SysTime: unix epoch conversion and duration" $scope) {
    let epoch = time_SysTime_fromUnixEpoch(0);
    let later = time_SysTime_fromUnixEpoch(123);
    let duration = unwrap_(time_SysTime_durationSinceChkd(later, epoch));

    try_(TEST_expect(time_SysTime_toUnixEpoch(epoch) == 0));
    try_(TEST_expect(time_SysTime_toUnixEpoch(later) == 123));
    try_(TEST_expect(duration.secs == 123));
    try_(TEST_expect(duration.nanos == 0));
    try_(TEST_expect(time_SysTime_lt(epoch, later)));
    try_(TEST_expect(isNone(time_SysTime_durationSinceChkd(epoch, later)))); /* NOLINT(readability-suspicious-call-argument) */
} $unscoped(TEST_fn);
