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

time_Instant time_Instant_now(void);

time_Duration time_Instant_elapsed(time_Instant ins);
time_Duration time_Instant_durationSince(time_Instant ins, time_Instant earlier);

time_Instant time_Instant_addDuration(time_Instant ins, time_Duration duration);
time_Instant time_Instant_subDuration(time_Instant ins, time_Duration duration);

u64 time_Instant_toNanos(time_Instant ins);

bool time_Instant_eq(time_Instant lhs, time_Instant rhs);
bool time_Instant_ne(time_Instant lhs, time_Instant rhs);
bool time_Instant_lt(time_Instant lhs, time_Instant rhs);
bool time_Instant_le(time_Instant lhs, time_Instant rhs);
bool time_Instant_gt(time_Instant lhs, time_Instant rhs);
bool time_Instant_ge(time_Instant lhs, time_Instant rhs);
bool time_Instant_isValid(time_Instant ins);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_INSTANT_INCLUDED */
