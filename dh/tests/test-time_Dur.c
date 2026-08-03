#include "dh-main.h"
#include "dh/time/Dur.h"

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

TEST_fn_("time/Dur: checked arithmetic normalizes and rejects overflow" $scope) {
    let one_sec = time_Dur_fromSecs(1);
    let half_sec = time_Dur_fromMillis(500);
    let one_and_half = time_Dur_fromMillis(1500);
    let max = time_Dur_from(u64_limit_max, time_nanos_per_sec - 1);
    let half = unwrap_(time_Dur_divChkd$u32(one_sec, 2));

    try_(TEST_expect(half.secs == 0));
    try_(TEST_expect(half.nanos == time_nanos_per_sec / 2));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_addChkd(one_sec, half_sec)), one_and_half)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_subChkd(one_and_half, one_sec)), half_sec)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_mulChkd$u32(half_sec, 3)), one_and_half)));
    try_(TEST_expect(time_Dur_eq(unwrap_(time_Dur_divChkd$u32(one_and_half, 3)), half_sec)));
    try_(TEST_expect(isNone(time_Dur_subChkd(one_sec, one_and_half))));
    try_(TEST_expect(isNone(time_Dur_divChkd$u32(one_sec, 0))));
    try_(TEST_expect(isNone(time_Dur_addChkd(max, time_Dur_nano))));
    try_(TEST_expect(isNone(time_Dur_mulChkd$u32(max, 2))));
    try_(TEST_expect(time_Dur_eq(time_Dur_addSat(max, time_Dur_nano), max)));
    try_(TEST_expect(time_Dur_eq(time_Dur_subSat(one_sec, time_Dur_fromSecs(2)), time_Dur_zero)));
    try_(TEST_expect(time_Dur_eq(time_Dur_mulSat$u32(max, 2), max)));
} $unscoped(TEST_fn);
