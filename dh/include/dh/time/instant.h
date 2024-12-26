/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
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

/*========== Macros and Definitions =========================================*/

struct time_Instant {
    time_SysTime time_;
};
using_ExtTypes(time_Instant);

/* Operations */
extern time_Instant  time_Instant_now(void);
extern time_Duration time_Instant_elapsed(time_Instant self);
extern time_Duration time_Instant_durationSince(time_Instant self, time_Instant earlier);

/* Validation */
extern bool time_Instant_isValid(time_Instant self);

/* Conversion */
extern u64 time_Instant_toNanos(time_Instant self);

/* Arithmetic */
extern Opt_time_Instant time_Instant_addDurationChecked(time_Instant, time_Duration);
extern Opt_time_Instant time_Instant_subDurationChecked(time_Instant, time_Duration);

extern time_Instant op_fnAddBy(time_Instant, time_Duration);
extern time_Instant op_fnSubBy(time_Instant, time_Duration);

/* Comparison */
extern cmp_fnCmp(time_Instant);
cmp_fnEq_default(time_Instant);
cmp_fnNe_default(time_Instant);
cmp_fnLt_default(time_Instant);
cmp_fnGt_default(time_Instant);
cmp_fnLe_default(time_Instant);
cmp_fnGe_default(time_Instant);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_INSTANT_INCLUDED */
