/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    instant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-10 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
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

/*========== Macros and Definitions =========================================*/


struct Instant {
    SystemTime time_;
};
Instant  Instant_now(void);
Duration Instant_elapsed(Instant start);
Duration Instant_durationSince(Instant start, Instant earlier);

bool Instant_eq(Instant lhs, Instant rhs);
bool Instant_ne(Instant lhs, Instant rhs);
bool Instant_lt(Instant lhs, Instant rhs);
bool Instant_le(Instant lhs, Instant rhs);
bool Instant_gt(Instant lhs, Instant rhs);
bool Instant_ge(Instant lhs, Instant rhs);
bool Instant_isValid(Instant ins);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_INSTANT_INCLUDED */
