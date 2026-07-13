/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ftx.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_ftx
 */
#pragma once
#ifndef thrd_ftx__included
#define thrd_ftx__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Wakeable.h"
#include "../time/self/Awake.h"

/*========== Macros and Declarations ========================================*/

#if !defined(thrd_ftx_use_pthread)
#define thrd_ftx_use_pthread __comp_bool__thrd_ftx_use_pthread
#endif /* !defined(thrd_ftx_use_pthread) */
#define __comp_bool__thrd_ftx_use_pthread thrd_ftx__use_pthread_default

#define thrd_ftx__use_pthread_default __comp_bool__thrd_ftx__use_pthread_default
#define __comp_bool__thrd_ftx__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_default_(pp_false) \
    ) pp_end \
)

errset_((thrd_ftx_E)(thrd_ftx_Unsupported) $union_errset_(Sched_TimedE));

$attr($must_check)
$extern fn_((thrd_ftx_wait(
    const atom_V$u32* ptr, u32 expect, thrd_Wakeable cancel_src
))(thrd_ftx_E$void));
$attr($must_check)
$extern fn_((thrd_ftx_waitFor(
    const atom_V$u32* ptr, u32 expect, thrd_Wakeable cancel_src, time_Dur timeout
))(thrd_ftx_E$void));
$extern fn_((thrd_ftx_waitProtcd(const atom_V$u32* ptr, u32 expect))(void));
$extern fn_((thrd_ftx_wake(const atom_V$u32* ptr, u32 max_waiters))(void));

typedef struct thrd_ftx_Deadline {
    var_(timeout, O$time_Dur);
    var_(started, time_Awake_Inst);
} thrd_ftx_Deadline;
$extern fn_((thrd_ftx_Deadline_init(O$time_Dur expires))(thrd_ftx_Deadline));
$attr($must_check)
$extern fn_((thrd_ftx_Deadline_wait(
    thrd_ftx_Deadline* self, const atom_V$u32* ptr, u32 expect, O$thrd_Wakeable cancel_src
))(thrd_ftx_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_ftx__included */
