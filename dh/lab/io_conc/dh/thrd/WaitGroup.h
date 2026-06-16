/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    WaitGroup.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  thrd_WaitGroup
 *
 * @brief   Wait group for thread management
 * @details Defines wait group for thread management.
 */
#pragma once
#ifndef thrd_WaitGroup__included
#define thrd_WaitGroup__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"
#include "ResetEvent.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_WaitGroup {
    var_(state, atom_V$usize);
    var_(event, thrd_ResetEvent);
} thrd_WaitGroup;
#define thrd_WaitGroup_init_static(/*void*/) ____thrd_WaitGroup_init_static()
$extern fn_((thrd_WaitGroup_init(void))(thrd_WaitGroup));
$extern fn_((thrd_WaitGroup_fini(thrd_WaitGroup* self))(void));
$extern fn_((thrd_WaitGroup_start(thrd_WaitGroup* self))(void));
$extern fn_((thrd_WaitGroup_startOn(atom_V$usize* state))(void));
$extern fn_((thrd_WaitGroup_startN(thrd_WaitGroup* self, usize n))(void));
$extern fn_((thrd_WaitGroup_startNOn(atom_V$usize* state, usize n))(void));
$extern fn_((thrd_WaitGroup_finish(thrd_WaitGroup* self))(void));
$extern fn_((thrd_WaitGroup_finishOn(atom_V$usize* state, thrd_ResetEvent* event))(void));
$extern fn_((thrd_WaitGroup_wait(thrd_WaitGroup* self))(void));
$extern fn_((thrd_WaitGroup_waitOn(atom_V$usize* state, thrd_ResetEvent* event))(void));
$extern fn_((thrd_WaitGroup_reset(thrd_WaitGroup* self))(void));
$extern fn_((thrd_WaitGroup_resetOn(atom_V$usize* state, thrd_ResetEvent* event))(void));
$extern fn_((thrd_WaitGroup_isDone(thrd_WaitGroup* self))(bool));
$extern fn_((thrd_WaitGroup_isDoneOn(atom_V$usize* state))(bool));
$extern fn_((thrd_WaitGroup_value(thrd_WaitGroup* self))(usize));
$extern fn_((thrd_WaitGroup_valueOn(atom_V$usize* state))(usize));
/// Spawns a new thread for the closure. This is appropriate when the callee delegates all work.
$extern fn_((thrd_WaitGroup_spawn(thrd_WaitGroup* self, mem_Alctr gpa, Clsr$Void* clsr))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_WaitGroup_init_static() l$((thrd_WaitGroup){ \
    .state = atom_V_init(0), \
    .event = thrd_ResetEvent_init_static(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_WaitGroup__included */
