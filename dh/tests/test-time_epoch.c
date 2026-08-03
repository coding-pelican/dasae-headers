#include "dh-main.h"
#include "dh/time/epoch.h"

TEST_fn_("time/epoch: reference offsets and aliases match common epochs" $scope) {
    try_(TEST_expect(time_epoch_clr == i64_(-62, 135, 769, 600ll)));
    try_(TEST_expect(time_epoch_windows == i64_(-11, 644, 473, 600ll)));
    try_(TEST_expect(time_epoch_openvms == i64_(-3, 506, 716, 800ll)));
    try_(TEST_expect(time_epoch_zos == i64_(-2, 208, 988, 800ll)));
    try_(TEST_expect(time_epoch_pickos == i64_(-63, 244, 800ll)));
    try_(TEST_expect(time_epoch_posix == 0));
    try_(TEST_expect(time_epoch_amiga == i64_(252, 460, 800ll)));
    try_(TEST_expect(time_epoch_dos == i64_(315, 532, 800ll)));
    try_(TEST_expect(time_epoch_gps == i64_(315, 964, 800ll)));
    try_(TEST_expect(time_epoch_ios == i64_(978, 307, 200ll)));
    try_(TEST_expect(time_epoch_go == time_epoch_clr));
    try_(TEST_expect(time_epoch_ntfs == time_epoch_windows));
    try_(TEST_expect(time_epoch_ntp == time_epoch_zos));
    try_(TEST_expect(time_epoch_jbase == time_epoch_pickos));
    try_(TEST_expect(time_epoch_unix == time_epoch_posix));
    try_(TEST_expect(time_epoch_android == time_epoch_posix));
    try_(TEST_expect(time_epoch_aros == time_epoch_amiga));
    try_(TEST_expect(time_epoch_morphos == time_epoch_amiga));
    try_(TEST_expect(time_epoch_os2 == time_epoch_dos));
    try_(TEST_expect(time_epoch_bios == time_epoch_dos));
    try_(TEST_expect(time_epoch_vfat == time_epoch_dos));
    try_(TEST_expect(time_epoch_brew == time_epoch_gps));
    try_(TEST_expect(time_epoch_atsc == time_epoch_gps));
} $unscoped(TEST_fn);

TEST_fn_("time/epoch: project and language milestones remain stable" $scope) {
    try_(TEST_expect(time_epoch_c_year == 1972));
    try_(TEST_expect(time_epoch_cpp_year == 1979));
    try_(TEST_expect(time_epoch_csharp_year == 2000));
    try_(TEST_expect(time_epoch_rust_year == 2015));
    try_(TEST_expect(time_epoch_zig_year == 2017));
    try_(TEST_expect(time_epoch_dh_eureka == i64_(1, 729, 048, 500ll)));
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

TEST_fn_("time/epoch: seconds decode into calendar and day time" $scope) {
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

    let july_2003 = time_epoch_Secs_from(u64_(1, 058, 781, 600));
    let july_2003_day = time_epoch_Secs_day(july_2003);
    let july_2003_day_secs = time_epoch_Secs_daySecs(july_2003);
    let july_2003_year_day = time_epoch_Day_yearDay(july_2003_day);
    let july_2003_month_day = time_epoch_YearDay_monthDay(july_2003_year_day);
    try_(TEST_expect(july_2003_year_day.year == 2003));
    try_(TEST_expect(july_2003_month_day.month == time_epoch_Month_jul));
    try_(TEST_expect(july_2003_month_day.day_idx + 1 == 21));
    try_(TEST_expect(time_epoch_DaySecs_hours(july_2003_day_secs) == 10));
    try_(TEST_expect(time_epoch_DaySecs_mins(july_2003_day_secs) == 0));
    try_(TEST_expect(time_epoch_DaySecs_secs(july_2003_day_secs) == 0));

    let march_2004 = time_epoch_Secs_from(u64_(1, 079, 964, 000));
    let march_2004_day = time_epoch_Secs_day(march_2004);
    let march_2004_day_secs = time_epoch_Secs_daySecs(march_2004);
    let march_2004_year_day = time_epoch_Day_yearDay(march_2004_day);
    let march_2004_month_day = time_epoch_YearDay_monthDay(march_2004_year_day);
    try_(TEST_expect(march_2004_year_day.year == 2004));
    try_(TEST_expect(march_2004_month_day.month == time_epoch_Month_mar));
    try_(TEST_expect(march_2004_month_day.day_idx + 1 == 22));
    try_(TEST_expect(time_epoch_DaySecs_hours(march_2004_day_secs) == 14));
    try_(TEST_expect(time_epoch_DaySecs_mins(march_2004_day_secs) == 0));
    try_(TEST_expect(time_epoch_DaySecs_secs(march_2004_day_secs) == 0));

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
