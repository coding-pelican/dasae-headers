/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Group.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Group
 *
 * @brief   OS-thread group lifecycle owner.
 * @details Owns spawned detached work completion through `thrd_Latch` and
 *          exposes a cooperative cancellation token source. Cancellation is
 *          observed only by closures that receive and check the group token.
 */
#pragma once
#ifndef thrd_Group__included
#define thrd_Group__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"
#include "Latch.h"
#include "CancelTok.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Group {
    var_(latch, thrd_Latch);
    var_(cancel, thrd_CancelTok_Src);
} thrd_Group;
#define thrd_Group_init_static(/*void*/) \
    ____thrd_Group_init_static()
$extern fn_((thrd_Group_init(void))(thrd_Group));
$extern fn_((thrd_Group_fini(thrd_Group* self))(void));
$extern fn_((thrd_Group_start(thrd_Group* self))(void));
$extern fn_((thrd_Group_startN(thrd_Group* self, usize n))(void));
$extern fn_((thrd_Group_finish(thrd_Group* self))(void));
$extern fn_((thrd_Group_wait(thrd_Group* self))(void));
$attr($must_check)
$extern fn_((thrd_Group_timedWait(thrd_Group* self, time_Dur timeout))(thrd_ftx_E$void));
$extern fn_((thrd_Group_isDone(thrd_Group* self))(bool));
$extern fn_((thrd_Group_value(thrd_Group* self))(usize));
$extern fn_((thrd_Group_cancelTok(thrd_Group* self))(thrd_CancelTok));
$extern fn_((thrd_Group_cancel(thrd_Group* self))(void));
$extern fn_((thrd_Group_isCanceled(thrd_Group* self))(bool));
/// Spawns a new detached thread and counts it in the group latch.
$extern fn_((thrd_Group_spawn(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_Group_init_static() l$((thrd_Group){ \
    .latch = thrd_Latch_init_static(), \
    .cancel = thrd_CancelTok_Src_init_static(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Group__included */
