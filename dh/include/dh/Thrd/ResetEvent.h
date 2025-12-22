/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ResetEvent.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_ResetEvent
 *
 * @brief   Reset event for thread management
 * @details Defines reset event for thread management.
 */
#ifndef Thrd_ResetEvent__included
#define Thrd_ResetEvent__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "Ftx.h"

/*========== Macros and Declarations ========================================*/

errset_((Thrd_ResetEvent_Err)(Timeout));
typedef struct Thrd_ResetEvent {
    var_(state, atom_V$u32);
} Thrd_ResetEvent;
$extern fn_((Thrd_ResetEvent_init(void))(Thrd_ResetEvent));
$extern fn_((Thrd_ResetEvent_fini(Thrd_ResetEvent* self))(void));
$extern fn_((Thrd_ResetEvent_wait(Thrd_ResetEvent* self))(void));
$attr($must_check)
$extern fn_((Thrd_ResetEvent_timedWait(Thrd_ResetEvent* self, time_Duration timeout))(Thrd_ResetEvent_Err$void));
$extern fn_((Thrd_ResetEvent_set(Thrd_ResetEvent* self))(void));
$extern fn_((Thrd_ResetEvent_reset(Thrd_ResetEvent* self))(void));
$extern fn_((Thrd_ResetEvent_isSet(const Thrd_ResetEvent* self))(bool));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_ResetEvent__included */
