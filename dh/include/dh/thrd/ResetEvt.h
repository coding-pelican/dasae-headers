/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ResetEvt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_ResetEvt
 *
 * @brief   Reset event for thread management
 * @details Defines reset event for thread management.
 */
#pragma once
#ifndef thrd_ResetEvt__included
#define thrd_ResetEvt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "ftx.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_ResetEvt {
    var_(state, atom_V$u32);
} thrd_ResetEvt;
typedef struct thrd_ResetEvt_Tok {
    var_(event, thrd_ResetEvt*);
} thrd_ResetEvt_Tok;
typedef struct thrd_ResetEvt_Sig {
    var_(event, thrd_ResetEvt*);
} thrd_ResetEvt_Sig;

#define thrd_ResetEvt_init_static(/*void*/) \
    ____thrd_ResetEvt_init_static()
$extern fn_((thrd_ResetEvt_init(void))(thrd_ResetEvt));
$extern fn_((thrd_ResetEvt_fini(thrd_ResetEvt* self))(void));
$extern fn_((thrd_ResetEvt_tok(thrd_ResetEvt* self))(thrd_ResetEvt_Tok));
$extern fn_((thrd_ResetEvt_sig(thrd_ResetEvt* self))(thrd_ResetEvt_Sig));
$extern fn_((thrd_ResetEvt_wait(thrd_ResetEvt* self))(void));
$attr($must_check)
$extern fn_((thrd_ResetEvt_timedWait(thrd_ResetEvt* self, time_Dur timeout))(thrd_ftx_E$void));
$extern fn_((thrd_ResetEvt_set(thrd_ResetEvt* self))(void));
$extern fn_((thrd_ResetEvt_reset(thrd_ResetEvt* self))(void));
$extern fn_((thrd_ResetEvt_isSet(const thrd_ResetEvt* self))(bool));
$extern fn_((thrd_ResetEvt_Tok_wait(thrd_ResetEvt_Tok self))(void));
$attr($must_check)
$extern fn_((thrd_ResetEvt_Tok_timedWait(thrd_ResetEvt_Tok self, time_Dur timeout))(thrd_ftx_E$void));
$extern fn_((thrd_ResetEvt_Tok_isSet(thrd_ResetEvt_Tok self))(bool));
$extern fn_((thrd_ResetEvt_Sig_set(thrd_ResetEvt_Sig self))(void));
$extern fn_((thrd_ResetEvt_Sig_reset(thrd_ResetEvt_Sig self))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_ResetEvt_init_static() l$((thrd_ResetEvt){ \
    .state = atom_V_init(0u), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_ResetEvt__included */
