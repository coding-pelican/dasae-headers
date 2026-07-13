/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    OnceEvt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_OnceEvt
 *
 * @brief   One-way event for thread management
 * @details Defines an event that can be set once and cannot be reset.
 */
#pragma once
#ifndef thrd_OnceEvt__included
#define thrd_OnceEvt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "ftx.h"
#include "Waiter.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_OnceEvt {
    var_(state, atom_V$u32);
    var_(lock, thrd_Mtx);
    var_(waiters, thrd_wait_Chain);
} thrd_OnceEvt;

#define thrd_OnceEvt_State_unset 0u
#define thrd_OnceEvt_State_waiting 1u
#define thrd_OnceEvt_State_set 2u

#define thrd_OnceEvt_init_static(/*void*/) \
    ____thrd_OnceEvt_init_static()
#define thrd_OnceEvt_initSet_static(/*void*/) \
    ____thrd_OnceEvt_initSet_static()
$extern fn_((thrd_OnceEvt_init(void))(thrd_OnceEvt));
$extern fn_((thrd_OnceEvt_initSet(void))(thrd_OnceEvt));
$extern fn_((thrd_OnceEvt_fini(thrd_OnceEvt* self))(void));

typedef struct thrd_OnceEvt_Tok thrd_OnceEvt_Tok;
$extern fn_((thrd_OnceEvt_tok(thrd_OnceEvt* self))(thrd_OnceEvt_Tok));
typedef struct thrd_OnceEvt_Sig thrd_OnceEvt_Sig;
$extern fn_((thrd_OnceEvt_sig(thrd_OnceEvt* self))(thrd_OnceEvt_Sig));

$extern fn_((thrd_OnceEvt_isSet(const thrd_OnceEvt* self))(bool));
$extern fn_((thrd_OnceEvt_tryWait(thrd_OnceEvt* self))(bool));
$attr($must_check)
$extern fn_((thrd_OnceEvt_wait(
    thrd_OnceEvt* self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((thrd_OnceEvt_waitFor(
    thrd_OnceEvt* self, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void));
$extern fn_((thrd_OnceEvt_waitProtcd(thrd_OnceEvt* self))(void));
$extern fn_((thrd_OnceEvt_wakeable(thrd_OnceEvt* self))(thrd_Wakeable));
$extern fn_((thrd_OnceEvt_set(thrd_OnceEvt* self))(void));

struct thrd_OnceEvt_Tok {
    var_(event, thrd_OnceEvt*);
};
$extern fn_((thrd_OnceEvt_Tok_isSet(thrd_OnceEvt_Tok self))(bool));
$extern fn_((thrd_OnceEvt_Tok_tryWait(thrd_OnceEvt_Tok self))(bool));
$attr($must_check)
$extern fn_((thrd_OnceEvt_Tok_wait(
    thrd_OnceEvt_Tok self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((thrd_OnceEvt_Tok_waitFor(
    thrd_OnceEvt_Tok self, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void));
$extern fn_((thrd_OnceEvt_Tok_waitProtcd(thrd_OnceEvt_Tok self))(void));
$extern fn_((thrd_OnceEvt_Tok_wakeable(thrd_OnceEvt_Tok self))(thrd_Wakeable));

struct thrd_OnceEvt_Sig {
    var_(event, thrd_OnceEvt*);
};
$extern fn_((thrd_OnceEvt_Sig_set(thrd_OnceEvt_Sig self))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_OnceEvt_init_static() l$((thrd_OnceEvt){ \
    .state = atom_V_init(thrd_OnceEvt_State_unset), \
    .lock = thrd_Mtx_init_static(), \
    .waiters = thrd_wait_Chain_init_static(), \
})
#define ____thrd_OnceEvt_initSet_static() l$((thrd_OnceEvt){ \
    .state = atom_V_init(thrd_OnceEvt_State_set), \
    .lock = thrd_Mtx_init_static(), \
    .waiters = thrd_wait_Chain_init_static(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_OnceEvt__included */
