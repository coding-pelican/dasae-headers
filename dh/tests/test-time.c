#include "dh-main.h"
#include "dh/time/common.h"

TEST_fn_("time/common: unit constants cover subsecond through week conversions" $scope) {
    try_(TEST_expect(time_nanos_per_micro == 1000));
    try_(TEST_expect(time_nanos_per_milli == 1000 * time_nanos_per_micro));
    try_(TEST_expect(time_nanos_per_sec == 1000 * time_nanos_per_milli));
    try_(TEST_expect(time_nanos_per_min == 60 * as$(u64)(time_nanos_per_sec)));
    try_(TEST_expect(time_nanos_per_hour == 60 * time_nanos_per_min));
    try_(TEST_expect(time_nanos_per_day == 24 * time_nanos_per_hour));
    try_(TEST_expect(time_nanos_per_week == 7 * time_nanos_per_day));

    try_(TEST_expect(time_micros_per_milli == 1000));
    try_(TEST_expect(time_micros_per_sec == 1000 * time_micros_per_milli));
    try_(TEST_expect(time_micros_per_min == 60 * as$(u64)(time_micros_per_sec)));
    try_(TEST_expect(time_micros_per_hour == 60 * time_micros_per_min));
    try_(TEST_expect(time_micros_per_day == 24 * time_micros_per_hour));
    try_(TEST_expect(time_micros_per_week == 7 * time_micros_per_day));

    try_(TEST_expect(time_millis_per_sec == 1000));
    try_(TEST_expect(time_millis_per_min == 60 * time_millis_per_sec));
    try_(TEST_expect(time_millis_per_hour == 60 * time_millis_per_min));
    try_(TEST_expect(time_millis_per_day == 24 * time_millis_per_hour));
    try_(TEST_expect(time_millis_per_week == 7 * time_millis_per_day));

    try_(TEST_expect(time_secs_per_min == 60));
    try_(TEST_expect(time_secs_per_hour == 60 * time_secs_per_min));
    try_(TEST_expect(time_secs_per_day == 24 * time_secs_per_hour));
    try_(TEST_expect(time_secs_per_week == 7 * time_secs_per_day));
    try_(TEST_expect(time_mins_per_day == 24 * time_mins_per_hour));
    try_(TEST_expect(time_mins_per_week == 7 * time_mins_per_day));
    try_(TEST_expect(time_hours_per_week == 7 * time_hours_per_day));
} $unscoped(TEST_fn);
