/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ResetEvent.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_ResetEvent
 *
 * @brief   Reset event for thread management
 * @details Defines reset event for thread management.
 */
#pragma once
#ifndef thrd_ResetEvent__included
#define thrd_ResetEvent__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "ftx.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_ResetEvent {
    var_(state, atom_V$u32);
} thrd_ResetEvent;
#define thrd_ResetEvent_init_static(/*void*/) ____thrd_ResetEvent_init_static()
$extern fn_((thrd_ResetEvent_init(void))(thrd_ResetEvent));
$extern fn_((thrd_ResetEvent_fini(thrd_ResetEvent* self))(void));
$extern fn_((thrd_ResetEvent_wait(thrd_ResetEvent* self))(void));
$attr($must_check)
$extern fn_((thrd_ResetEvent_timedWait(thrd_ResetEvent* self, time_Dur timeout))(thrd_ftx_E$void));
$extern fn_((thrd_ResetEvent_set(thrd_ResetEvent* self))(void));
$extern fn_((thrd_ResetEvent_reset(thrd_ResetEvent* self))(void));
$extern fn_((thrd_ResetEvent_isSet(const thrd_ResetEvent* self))(bool));

/*========== Macros and Definitions =========================================*/

#define ____thrd_ResetEvent_init_static() l$((thrd_ResetEvent){ \
    .state = atom_V_init(0u), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_ResetEvent__included */
