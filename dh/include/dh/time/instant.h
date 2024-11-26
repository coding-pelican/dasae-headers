/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
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

/* Operations */
extern time_Instant  time_Instant_now(void);
extern time_Duration time_Instant_elapsed(time_Instant self);
extern time_Duration time_Instant_durationSince(time_Instant self, time_Instant earlier);

/* Validation */
extern bool time_Instant_isValid(time_Instant self);

/* Conversion */
extern u64 time_Instant_toNanos(time_Instant self);

/* Arithmetic */
extern bool         ops_try_add_other(time_Instant, time_Duration, time_Instant);
extern bool         ops_try_sub_other(time_Instant, time_Duration, time_Instant);
extern time_Instant ops_add_other(time_Instant, time_Duration);
extern time_Instant ops_sub_other(time_Instant, time_Duration);

/* Comparison */
extern cmp_Ord time_Instant_cmp(time_Instant self, time_Instant other);
cmp_eq_impl(time_Instant);
cmp_ne_impl(time_Instant);
cmp_lt_impl(time_Instant);
cmp_gt_impl(time_Instant);
cmp_le_impl(time_Instant);
cmp_ge_impl(time_Instant);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_INSTANT_INCLUDED */
