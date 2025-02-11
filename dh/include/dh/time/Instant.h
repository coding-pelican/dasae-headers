/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-02-08 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Instant
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */

#ifndef TIME_INSTANT_INCLUDED
#define TIME_INSTANT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "SysTime.h"

/*========== Structures =====================================================*/

struct time_Instant {
    time_SysTime point;
};
impl_Opt$(time_Instant);

/*========== Operations =====================================================*/

extern time_Instant      time_Instant_now(void);
extern time_Duration     time_Instant_elapsed(time_Instant self);
extern time_Duration     time_Instant_durationSince(time_Instant later, time_Instant earlier);
extern Opt$time_Duration time_Instant_chkdDurationSince(time_Instant later, time_Instant earlier);

/*========== Arithmetic Operations ==========================================*/

extern time_Instant       op_fnAddBy(time_Instant, time_Duration);
extern time_Instant       op_fnAddAsgBy(time_Instant, time_Duration);
force_inline time_Instant op_fnWrapAddBy(addDuration, time_Instant, time_Duration);
force_inline time_Instant op_fnWrapAddAsgBy(addAsgDuration, time_Instant, time_Duration);
extern time_Instant       op_fnSubBy(time_Instant, time_Duration);
extern time_Instant       op_fnSubAsgBy(time_Instant, time_Duration);
force_inline time_Instant op_fnWrapSubBy(subDuration, time_Instant, time_Duration);
force_inline time_Instant op_fnWrapSubAsgBy(subAsgDuration, time_Instant, time_Duration);
extern Opt$time_Instant   time_Instant_chkdAddDuration(time_Instant lhs, time_Duration rhs);
extern Opt$time_Instant   time_Instant_chkdSubDuration(time_Instant lhs, time_Duration rhs);

/*========== Time Conversion to/from Unix Epoch =============================*/

extern time_Instant time_Instant_fromUnixEpoch(u64 secs);
extern u64          time_Instant_toUnixEpoch(time_Instant self);

/*========== Comparison =====================================================*/

extern cmp_fnCmp(time_Instant);
cmp_fnEq_default(time_Instant);
cmp_fnNe_default(time_Instant);
cmp_fnLt_default(time_Instant);
cmp_fnGt_default(time_Instant);
cmp_fnLe_default(time_Instant);
cmp_fnGe_default(time_Instant);

/*========== Constants ======================================================*/

static const time_Instant time_Instant_unix_epoch = { .point = time_SysTime_unix_epoch };

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_INSTANT_INCLUDED */
