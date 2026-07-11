/**
 * @file    conc/Latch.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Latch
 */
#pragma once
#ifndef conc_Latch__included
#define conc_Latch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "OnceEvt.h"

T_alias$((conc_Latch)(struct conc_Latch));
struct conc_Latch {
    var_(count, atom_V$usize);
    var_(done_evt, conc_OnceEvt);
};

#define conc_Latch_init_static(_count) ____conc_Latch_init_static(_count)
$extern fn_((conc_Latch_init(usize count))(conc_Latch));
$extern fn_((conc_Latch_add(conc_Latch* self, usize n))(void));
$extern fn_((conc_Latch_done(conc_Latch* self))(void));
$extern fn_((conc_Latch_doneN(conc_Latch* self, usize n))(void));
$extern fn_((conc_Latch_isDone(const conc_Latch* self))(bool));
$extern fn_((conc_Latch_value(const conc_Latch* self))(usize));
$attr($must_check)
$extern fn_((conc_Latch_wait(conc_Latch* self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_Latch_waitFor(conc_Latch* self, Sched sched, time_Dur dur))(Sched_TimedE$void));
$attr($must_check)
$extern fn_((conc_Latch_waitUntil(conc_Latch* self, Sched sched, time_Inst deadline))(Sched_TimedE$void));

#define ____conc_Latch_init_static(_count) l$((conc_Latch){ \
    .count = atom_V_init(_count), \
    .done_evt = ((_count) == 0 ? (conc_OnceEvt){ \
        .state = atom_V_init(conc_OnceEvt_State_set), \
        .lock = atom_SpinLock_init(), \
        .waiters = none(), \
    } : conc_OnceEvt_init_static()), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_Latch__included */
