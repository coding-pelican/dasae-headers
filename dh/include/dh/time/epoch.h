#pragma once
#ifndef time_epoch__included
#define time_epoch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/* --- Epoch Offsets --- */
#define time_epoch_posix __comp_const__time_epoch_posix
#define time_epoch_dos __comp_const__time_epoch_dos
#define time_epoch_ios __comp_const__time_epoch_ios
#define time_epoch_openvms __comp_const__time_epoch_openvms
#define time_epoch_zos __comp_const__time_epoch_zos
#define time_epoch_windows __comp_const__time_epoch_windows
#define time_epoch_amiga __comp_const__time_epoch_amiga
#define time_epoch_pickos __comp_const__time_epoch_pickos
#define time_epoch_gps __comp_const__time_epoch_gps
#define time_epoch_clr __comp_const__time_epoch_clr

/* --- Epoch Aliases --- */
#define time_epoch_unix time_epoch_posix
#define time_epoch_android time_epoch_posix
#define time_epoch_os2 time_epoch_dos
#define time_epoch_bios time_epoch_dos
#define time_epoch_vfat time_epoch_dos
#define time_epoch_ntfs time_epoch_windows
#define time_epoch_ntp time_epoch_zos
#define time_epoch_jbase time_epoch_pickos
#define time_epoch_aros time_epoch_amiga
#define time_epoch_morphos time_epoch_amiga
#define time_epoch_brew time_epoch_gps
#define time_epoch_atsc time_epoch_gps
#define time_epoch_go time_epoch_clr

/* --- Calendar Constants --- */
#define time_epoch_year __comp_const__time_epoch_year
#define time_epoch_secs_per_day time_secs_per_day

/* --- Year --- */
/// Calendar year. Valid epoch decoding starts at `time_epoch_year`.
T_alias$((time_epoch_Year)(u16));

/// Month number in the range 1..12.
T_alias$((time_epoch_MonthNum)(u8));
/// Day index within a month, in the range 0..30.
T_alias$((time_epoch_MonthDayIdx)(u8));
/// Number of days in a month, in the range 28..31.
T_alias$((time_epoch_MonthDayCount)(u8));
/// Day index within a year, in the range 0..365.
T_alias$((time_epoch_YearDayIdx)(u16));
/// Number of days in a year, in the range 365..366.
T_alias$((time_epoch_YearDayCount)(u16));

$attr($inline_always)
$static fn_((time_epoch_isLeapYear(time_epoch_Year year))(bool));
$attr($inline_always)
$static fn_((time_epoch_daysInYear(time_epoch_Year year))(time_epoch_YearDayCount));

/* --- Month --- */
/// Calendar month, using the conventional 1-based month number.
T_alias$((time_epoch_Month)(enum_((time_epoch_Month $fits($packed))(
    time_epoch_Month_jan = 1,
    time_epoch_Month_feb,
    time_epoch_Month_mar,
    time_epoch_Month_apr,
    time_epoch_Month_may,
    time_epoch_Month_jun,
    time_epoch_Month_jul,
    time_epoch_Month_aug,
    time_epoch_Month_sep,
    time_epoch_Month_oct,
    time_epoch_Month_nov,
    time_epoch_Month_dec
))));
claim_assert_static(eqlType$(time_epoch_Month, u8));
$attr($inline_always)
$static fn_((time_epoch_Month_toNum(time_epoch_Month self))(time_epoch_MonthNum));
$attr($inline_always)
$static fn_((time_epoch_daysInMonth(time_epoch_Year year, time_epoch_Month month))(time_epoch_MonthDayCount));

/* --- Month Day --- */
T_alias$((time_epoch_MonthDay)(struct time_epoch_MonthDay {
    /// Calendar month.
    var_(month, time_epoch_Month);
    /// Zero-based day index within `month`, in the range 0..30.
    var_(day_idx, time_epoch_MonthDayIdx);
}));

/* --- Year Day --- */
T_alias$((time_epoch_YearDay)(struct time_epoch_YearDay {
    /// Calendar year.
    var_(year, time_epoch_Year);
    /// Zero-based day index within `year`, in the range 0..365.
    var_(day_idx, time_epoch_YearDayIdx);
}));
$attr($inline_always)
$static fn_((time_epoch_YearDay_monthDay(time_epoch_YearDay self))(time_epoch_MonthDay));

/* --- Day --- */
/// Day count since POSIX epoch. For `u64` epoch seconds this fits in 47 bits.
T_alias$((time_epoch_DayCount)(u64));

T_alias$((time_epoch_Day)(struct time_epoch_Day {
    /// Whole days since POSIX epoch.
    var_(days, time_epoch_DayCount);
}));
$attr($inline_always)
$static fn_((time_epoch_Day_yearDay(time_epoch_Day self))(time_epoch_YearDay));

/* --- Day Seconds --- */
/// Seconds since start of day, in the range 0..86399.
T_alias$((time_epoch_DaySecCount)(u32));
/// Hour within day, in the range 0..23.
T_alias$((time_epoch_Hour)(u8));
/// Minute within hour, in the range 0..59.
T_alias$((time_epoch_Min)(u8));
/// Second within minute, in the range 0..59.
T_alias$((time_epoch_Sec)(u8));

T_alias$((time_epoch_DaySecs)(struct time_epoch_DaySecs {
    /// Seconds since start of day.
    var_(secs, time_epoch_DaySecCount);
}));
$attr($inline_always)
$static fn_((time_epoch_DaySecs_hours(time_epoch_DaySecs self))(time_epoch_Hour));
$attr($inline_always)
$static fn_((time_epoch_DaySecs_mins(time_epoch_DaySecs self))(time_epoch_Min));
$attr($inline_always)
$static fn_((time_epoch_DaySecs_secs(time_epoch_DaySecs self))(time_epoch_Sec));

/* --- Seconds --- */
T_alias$((time_epoch_Secs)(struct time_epoch_Secs {
    /// Seconds since POSIX epoch.
    var_(secs, u64);
}));
$attr($inline_always)
$static fn_((time_epoch_Secs_from(u64 secs))(time_epoch_Secs));
$attr($inline_always)
$static fn_((time_epoch_Secs_day(time_epoch_Secs self))(time_epoch_Day));
$attr($inline_always)
$static fn_((time_epoch_Secs_daySecs(time_epoch_Secs self))(time_epoch_DaySecs));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_epoch_posix (i64_(0ll))
#define __comp_const__time_epoch_dos (i64_(315, 532, 800ll))
#define __comp_const__time_epoch_ios (i64_(978, 307, 200ll))
#define __comp_const__time_epoch_openvms (i64_(-3, 506, 716, 800ll))
#define __comp_const__time_epoch_zos (i64_(-2, 208, 988, 800ll))
#define __comp_const__time_epoch_windows (i64_(-11, 644, 473, 600ll))
#define __comp_const__time_epoch_amiga (i64_(252, 460, 800ll))
#define __comp_const__time_epoch_pickos (i64_(-63, 244, 800ll))
#define __comp_const__time_epoch_gps (i64_(315, 964, 800ll))
#define __comp_const__time_epoch_clr (i64_(-62, 135, 769, 600ll))

#define __comp_const__time_epoch_year (time_epoch_Year)(1970u)

#if in_analysis_active_only || in_comptime
fn_((time_epoch_isLeapYear(time_epoch_Year year))(bool)) {
    if (year % 4 != 0) return false;
    if (year % 100 != 0) return true;
    return year % 400 == 0;
};
fn_((time_epoch_daysInYear(time_epoch_Year year))(time_epoch_YearDayCount)) {
    return time_epoch_isLeapYear(year) ? u16_(366u) : u16_(365u);
};

fn_((time_epoch_Month_toNum(time_epoch_Month self))(time_epoch_MonthNum)) {
    claim_assert(time_epoch_Month_jan <= self && self <= time_epoch_Month_dec);
    return as$(u8)(self);
};
fn_((time_epoch_daysInMonth(time_epoch_Year year, time_epoch_Month month))(time_epoch_MonthDayCount)) {
    switch (month) {
    case time_epoch_Month_jan: return u8_(31u);
    case time_epoch_Month_feb: return time_epoch_isLeapYear(year) ? u8_(29u) : u8_(28u);
    case time_epoch_Month_mar: return u8_(31u);
    case time_epoch_Month_apr: return u8_(30u);
    case time_epoch_Month_may: return u8_(31u);
    case time_epoch_Month_jun: return u8_(30u);
    case time_epoch_Month_jul: return u8_(31u);
    case time_epoch_Month_aug: return u8_(31u);
    case time_epoch_Month_sep: return u8_(30u);
    case time_epoch_Month_oct: return u8_(31u);
    case time_epoch_Month_nov: return u8_(30u);
    case time_epoch_Month_dec: return u8_(31u);
    }
};

fn_((time_epoch_YearDay_monthDay(time_epoch_YearDay self))(time_epoch_MonthDay)) {
    var_(month, time_epoch_Month) = time_epoch_Month_jan;
    var day_idx = self.day_idx;
    while (true) {
        let days_in_month = time_epoch_daysInMonth(self.year, month);
        if (day_idx < days_in_month) break;
        day_idx -= days_in_month;
        month = as$(time_epoch_Month)(as$(u8)(month) + 1);
    }
    return (time_epoch_MonthDay){ .month = month, .day_idx = as$(time_epoch_MonthDayIdx)(day_idx) };
};

fn_((time_epoch_Day_yearDay(time_epoch_Day self))(time_epoch_YearDay)) {
    var days = self.days;
    var year = time_epoch_year;
    while (true) {
        let days_in_year = time_epoch_daysInYear(year);
        if (days < days_in_year) break;
        days -= days_in_year;
        year += 1;
    }
    return (time_epoch_YearDay){ .year = year, .day_idx = as$(time_epoch_YearDayIdx)(days) };
};

fn_((time_epoch_DaySecs_hours(time_epoch_DaySecs self))(time_epoch_Hour)) {
    claim_assert(self.secs < time_secs_per_day);
    return as$(time_epoch_Hour)(self.secs / time_secs_per_hour);
};
fn_((time_epoch_DaySecs_mins(time_epoch_DaySecs self))(time_epoch_Min)) {
    claim_assert(self.secs < time_secs_per_day);
    return as$(time_epoch_Min)((self.secs % time_secs_per_hour) / time_secs_per_min);
};
fn_((time_epoch_DaySecs_secs(time_epoch_DaySecs self))(time_epoch_Sec)) {
    claim_assert(self.secs < time_secs_per_day);
    return as$(time_epoch_Sec)(self.secs % time_secs_per_min);
};

fn_((time_epoch_Secs_from(u64 secs))(time_epoch_Secs)) {
    return (time_epoch_Secs){ .secs = secs };
};
fn_((time_epoch_Secs_day(time_epoch_Secs self))(time_epoch_Day)) {
    return (time_epoch_Day){ .days = self.secs / time_epoch_secs_per_day };
};
fn_((time_epoch_Secs_daySecs(time_epoch_Secs self))(time_epoch_DaySecs)) {
    return (time_epoch_DaySecs){ .secs = as$(u32)(self.secs % time_epoch_secs_per_day) };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_epoch__included */
