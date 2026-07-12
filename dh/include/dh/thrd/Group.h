/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
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
 * @details Owns spawned work completion through `thrd_Latch` and joins
 *          spawned threads before successful waits return.
 *          Cancellation belongs to the caller or to a higher-level owner.
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

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Group_Node thrd_Group_Node;
T_use_P$(thrd_Group_Node);
T_use_O$(P$thrd_Group_Node);
struct thrd_Group_Node {
    var_(next, O$P$thrd_Group_Node);
    var_(gpa, mem_Alctr);
    var_(self, thrd_Self);
};

typedef struct thrd_Group {
    var_(latch, thrd_Latch);
    var_(lock, thrd_Mtx);
    var_(threads, O$P$thrd_Group_Node);
} thrd_Group;
#define thrd_Group_init_static(/*void*/) \
    ____thrd_Group_init_static()
$extern fn_((thrd_Group_init(void))(thrd_Group));
$extern fn_((thrd_Group_fini(thrd_Group* self))(void));

$extern fn_((thrd_Group_start(thrd_Group* self))(void));
$extern fn_((thrd_Group_startN(thrd_Group* self, usize n))(void));
/// Spawns a new thread and counts it in the group latch.
$extern fn_((thrd_Group_spawn(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(void));
$extern fn_((thrd_Group_adopt(thrd_Group* self, thrd_Group_Node* node, thrd_Self thrd))(void));
$extern fn_((thrd_Group_finish(thrd_Group* self))(void));

$extern fn_((thrd_Group_isDone(thrd_Group* self))(bool));
$extern fn_((thrd_Group_value(thrd_Group* self))(usize));

$extern fn_((thrd_Group_tryWait(thrd_Group* self))(bool));
$attr($must_check)
$extern fn_((thrd_Group_wait(thrd_Group* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((thrd_Group_waitFor(thrd_Group* self, thrd_wait_Src cancel_src, time_Dur timeout))(Sched_TimedE$void));
$extern fn_((thrd_Group_waitProtcd(thrd_Group* self))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_Group_init_static() l$((thrd_Group){ \
    .latch = thrd_Latch_init_static(), \
    .lock = thrd_Mtx_init_static(), \
    .threads = none(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Group__included */
