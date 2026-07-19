#include "dh-main.h"
#include "dh/time.h"

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

TEST_fn_("time/epoch: reference offsets and aliases match common epochs" $scope) {
    try_(TEST_expect(time_epoch_posix == 0));
    try_(TEST_expect(time_epoch_dos == i64_(315, 532, 800ll)));
    try_(TEST_expect(time_epoch_ios == i64_(978, 307, 200ll)));
    try_(TEST_expect(time_epoch_openvms == i64_(-3, 506, 716, 800ll)));
    try_(TEST_expect(time_epoch_zos == i64_(-2, 208, 988, 800ll)));
    try_(TEST_expect(time_epoch_windows == i64_(-11, 644, 473, 600ll)));
    try_(TEST_expect(time_epoch_amiga == i64_(252, 460, 800ll)));
    try_(TEST_expect(time_epoch_pickos == i64_(-63, 244, 800ll)));
    try_(TEST_expect(time_epoch_gps == i64_(315, 964, 800ll)));
    try_(TEST_expect(time_epoch_clr == i64_(-62, 135, 769, 600ll)));
    try_(TEST_expect(time_epoch_unix == time_epoch_posix));
    try_(TEST_expect(time_epoch_ntfs == time_epoch_windows));
    try_(TEST_expect(time_epoch_ntp == time_epoch_zos));
    try_(TEST_expect(time_epoch_go == time_epoch_clr));
} $unscoped(TEST_fn);

TEST_fn_("time/epoch: calendar helpers handle leap years and month sizes" $scope) {
    try_(TEST_expect(!time_epoch_isLeapYear(2095)));
    try_(TEST_expect(time_epoch_isLeapYear(2096)));
    try_(TEST_expect(!time_epoch_isLeapYear(2100)));
    try_(TEST_expect(time_epoch_isLeapYear(2400)));
    try_(TEST_expect(time_epoch_daysInYear(2096) == 366));
    try_(TEST_expect(time_epoch_daysInMonth(2096, time_epoch_Month_feb) == 29));
    try_(TEST_expect(time_epoch_daysInMonth(2095, time_epoch_Month_feb) == 28));
    try_(TEST_expect(time_epoch_daysInMonth(2021, time_epoch_Month_jul) == 31));
    try_(TEST_expect(time_epoch_Month_toNum(time_epoch_Month_dec) == 12));
} $unscoped(TEST_fn);

TEST_fn_("time/epoch: seconds decode into year day month day and day time" $scope) {
    let epoch = time_epoch_Secs_from(0);
    let epoch_day = time_epoch_Secs_day(epoch);
    let epoch_day_secs = time_epoch_Secs_daySecs(epoch);
    let epoch_year_day = time_epoch_Day_yearDay(epoch_day);
    let epoch_month_day = time_epoch_YearDay_monthDay(epoch_year_day);
    try_(TEST_expect(epoch_year_day.year == 1970));
    try_(TEST_expect(epoch_year_day.day_idx == 0));
    try_(TEST_expect(epoch_month_day.month == time_epoch_Month_jan));
    try_(TEST_expect(epoch_month_day.day_idx == 0));
    try_(TEST_expect(time_epoch_DaySecs_hours(epoch_day_secs) == 0));
    try_(TEST_expect(time_epoch_DaySecs_mins(epoch_day_secs) == 0));
    try_(TEST_expect(time_epoch_DaySecs_secs(epoch_day_secs) == 0));

    let last_1970 = time_epoch_Secs_from(u64_(31, 535, 999));
    let last_1970_day = time_epoch_Secs_day(last_1970);
    let last_1970_day_secs = time_epoch_Secs_daySecs(last_1970);
    let last_1970_year_day = time_epoch_Day_yearDay(last_1970_day);
    let last_1970_month_day = time_epoch_YearDay_monthDay(last_1970_year_day);
    try_(TEST_expect(last_1970_year_day.year == 1970));
    try_(TEST_expect(last_1970_year_day.day_idx == 364));
    try_(TEST_expect(last_1970_month_day.month == time_epoch_Month_dec));
    try_(TEST_expect(last_1970_month_day.day_idx == 30));
    try_(TEST_expect(time_epoch_DaySecs_hours(last_1970_day_secs) == 23));
    try_(TEST_expect(time_epoch_DaySecs_mins(last_1970_day_secs) == 59));
    try_(TEST_expect(time_epoch_DaySecs_secs(last_1970_day_secs) == 59));

    let june_2021 = time_epoch_Secs_from(u64_(1, 622, 924, 906));
    let june_day = time_epoch_Secs_day(june_2021);
    let june_day_secs = time_epoch_Secs_daySecs(june_2021);
    let june_year_day = time_epoch_Day_yearDay(june_day);
    let june_month_day = time_epoch_YearDay_monthDay(june_year_day);
    try_(TEST_expect(june_year_day.year == 2021));
    try_(TEST_expect(june_year_day.day_idx == 31 + 28 + 31 + 30 + 31 + 4));
    try_(TEST_expect(june_month_day.month == time_epoch_Month_jun));
    try_(TEST_expect(june_month_day.day_idx == 4));
    try_(TEST_expect(time_epoch_DaySecs_hours(june_day_secs) == 20));
    try_(TEST_expect(time_epoch_DaySecs_mins(june_day_secs) == 28));
    try_(TEST_expect(time_epoch_DaySecs_secs(june_day_secs) == 26));

    let july_2021 = time_epoch_Secs_from(u64_(1, 625, 159, 473));
    let july_day = time_epoch_Secs_day(july_2021);
    let july_day_secs = time_epoch_Secs_daySecs(july_2021);
    let july_year_day = time_epoch_Day_yearDay(july_day);
    let july_month_day = time_epoch_YearDay_monthDay(july_year_day);
    try_(TEST_expect(july_year_day.year == 2021));
    try_(TEST_expect(july_year_day.day_idx == 31 + 28 + 31 + 30 + 31 + 30));
    try_(TEST_expect(july_month_day.month == time_epoch_Month_jul));
    try_(TEST_expect(july_month_day.day_idx == 0));
    try_(TEST_expect(time_epoch_DaySecs_hours(july_day_secs) == 17));
    try_(TEST_expect(time_epoch_DaySecs_mins(july_day_secs) == 11));
    try_(TEST_expect(time_epoch_DaySecs_secs(july_day_secs) == 13));
} $unscoped(TEST_fn);

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
