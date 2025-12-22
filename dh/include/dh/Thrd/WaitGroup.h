/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    WaitGroup.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_WaitGroup
 *
 * @brief   Wait group for thread management
 * @details Defines wait group for thread management.
 */
#ifndef Thrd_WaitGroup__included
#define Thrd_WaitGroup__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "ResetEvent.h"

/*========== Macros and Declarations ========================================*/

typedef struct Thrd_WaitGroup {
    var_(state, atom_V$usize);
    var_(event, Thrd_ResetEvent);
} Thrd_WaitGroup;
$extern fn_((Thrd_WaitGroup_init(void))(Thrd_WaitGroup));
$extern fn_((Thrd_WaitGroup_fini(Thrd_WaitGroup* self))(void));
$extern fn_((Thrd_WaitGroup_start(Thrd_WaitGroup* self))(void));
$extern fn_((Thrd_WaitGroup_startN(Thrd_WaitGroup* self, usize n))(void));
$extern fn_((Thrd_WaitGroup_finish(Thrd_WaitGroup* self))(void));
$extern fn_((Thrd_WaitGroup_wait(Thrd_WaitGroup* self))(void));
$extern fn_((Thrd_WaitGroup_reset(Thrd_WaitGroup* self))(void));
$extern fn_((Thrd_WaitGroup_isDone(Thrd_WaitGroup* self))(bool));
$extern fn_((Thrd_WaitGroup_spawn(Thrd_WaitGroup* self, mem_Allocator gpa, Thrd_FnCtx* fn_ctx))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_WaitGroup__included */
