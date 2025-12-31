/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Ftx.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Ftx
 */
#ifndef Thrd_Ftx__included
#define Thrd_Ftx__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "../atom.h"
#include "../time/Duration.h"
#include "../time/Instant.h"

/*========== Macros and Declarations ========================================*/

#if !defined(Thrd_Ftx_use_pthread)
#define Thrd_Ftx_use_pthread __comp_bool__Thrd_Ftx_use_pthread
#endif /* !defined(Thrd_Ftx_use_pthread) */
#define __comp_bool__Thrd_Ftx_use_pthread Thrd_Ftx__use_pthread_default

#define Thrd_Ftx__use_pthread_default __comp_bool__Thrd_Ftx__use_pthread_default
#define __comp_bool__Thrd_Ftx__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_default_(Thrd_use_pthread) \
    ) pp_end \
)

errset_((Thrd_Ftx_Err)(Timeout));
$extern fn_((Thrd_Ftx_wait(const atom_V$u32* ptr, u32 expect))(void));
$attr($must_check)
$extern fn_((Thrd_Ftx_timedWait(const atom_V$u32* ptr, u32 expect, time_Duration timeout))(Thrd_Ftx_Err$void));
$extern fn_((Thrd_Ftx_wake(const atom_V$u32* ptr, u32 max_waiters))(void));

typedef struct Thrd_Ftx_Deadline {
    var_(timeout, O$time_Duration);
    var_(started, time_Instant);
} Thrd_Ftx_Deadline;
$extern fn_((Thrd_Ftx_Deadline_init(O$time_Duration expires))(Thrd_Ftx_Deadline));
$attr($must_check)
$extern fn_((Thrd_Ftx_Deadline_wait(Thrd_Ftx_Deadline* self, const atom_V$u32* ptr, u32 expect))(Thrd_Ftx_Err$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_Ftx__included */
