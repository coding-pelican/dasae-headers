/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Once.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Once
 *
 * @brief   Run-once primitive for thread management
 */
#pragma once
#ifndef thrd_Once__included
#define thrd_Once__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "ftx.h"
#include "../clsr.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Once {
    var_(state, atom_V$u32);
} thrd_Once;

#define thrd_Once_init_static(/*void*/) \
    ____thrd_Once_init_static()
$extern fn_((thrd_Once_init(void))(thrd_Once));
$extern fn_((thrd_Once_fini(thrd_Once* self))(void));

$extern fn_((thrd_Once_tryBegin(thrd_Once* self))(bool));
$extern fn_((thrd_Once_finish(thrd_Once* self))(void));
$extern fn_((thrd_Once_wait(thrd_Once* self))(void));
$extern fn_((thrd_Once_isDone(const thrd_Once* self))(bool));
$extern fn_((thrd_Once_run(thrd_Once* self, Clsr$Void* clsr))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_Once_init_static() l$((thrd_Once){ \
    .state = atom_V_init(0u), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Once__included */
