/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    time.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  time
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */

#ifndef TIME_INCLUDED
#define TIME_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "time/cfg.h"
#include "time/common.h"
#include "time/Duration.h"
#include "time/Instant.h"
#include "time/SysTime.h"

/*========== Macros and Definitions =========================================*/

force_inline time_SysTime op_fnWrapAddBy(addDuration, time_SysTime, time_Duration);
force_inline time_SysTime op_fnWrapSubBy(subDuration, time_SysTime, time_Duration);
force_inline time_Instant op_fnWrapAddBy(addDuration, time_Instant, time_Duration);
force_inline time_Instant op_fnWrapSubBy(subDuration, time_Instant, time_Duration);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* TIME_INCLUDED */
