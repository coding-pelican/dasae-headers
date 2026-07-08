/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Latch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Latch
 *
 * @brief   One-shot count latch for OS-thread blocking waits.
 * @details Owns a count only. It does not own spawned threads, closures, or
 *          cancellation. Add/start must happen before wait begins.
 */
#pragma once
#ifndef thrd_Latch__included
#define thrd_Latch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "OnceEvt.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Latch {
    var_(state, atom_V$usize);
    var_(done_evt, thrd_OnceEvt);
} thrd_Latch;
#define thrd_Latch_init_static(/*void*/) \
    ____thrd_Latch_init_static()
$extern fn_((thrd_Latch_init(void))(thrd_Latch));
$extern fn_((thrd_Latch_fini(thrd_Latch* self))(void));
$extern fn_((thrd_Latch_start(thrd_Latch* self))(void));
$extern fn_((thrd_Latch_startOn(atom_V$usize* state))(void));
$extern fn_((thrd_Latch_startN(thrd_Latch* self, usize n))(void));
$extern fn_((thrd_Latch_startNOn(atom_V$usize* state, usize n))(void));
$extern fn_((thrd_Latch_finish(thrd_Latch* self))(void));
$extern fn_((thrd_Latch_finishOn(atom_V$usize* state, thrd_OnceEvt* event))(void));
$extern fn_((thrd_Latch_wait(thrd_Latch* self))(void));
$extern fn_((thrd_Latch_waitOn(atom_V$usize* state, thrd_OnceEvt* event))(void));
$attr($must_check)
$extern fn_((thrd_Latch_timedWait(thrd_Latch* self, time_Dur timeout))(thrd_ftx_E$void));
$attr($must_check)
$extern fn_((thrd_Latch_timedWaitOn(atom_V$usize* state, thrd_OnceEvt* event, time_Dur timeout))(thrd_ftx_E$void));
$extern fn_((thrd_Latch_isDone(thrd_Latch* self))(bool));
$extern fn_((thrd_Latch_isDoneOn(atom_V$usize* state))(bool));
$extern fn_((thrd_Latch_value(thrd_Latch* self))(usize));
$extern fn_((thrd_Latch_valueOn(atom_V$usize* state))(usize));
$extern fn_((thrd_Latch_tok(thrd_Latch* self))(thrd_OnceEvt_Tok));

/*========== Macros and Definitions =========================================*/

#define ____thrd_Latch_init_static() l$((thrd_Latch){ \
    .state = atom_V_init(0), \
    .done_evt = thrd_OnceEvt_init_static(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Latch__included */
