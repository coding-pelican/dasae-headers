/**
 * @file    Once.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Once
 * @brief   Scheduler-aware run-once primitive surface.
 */
#pragma once
#ifndef conc_Once__included
#define conc_Once__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"
#include "OnceEvt.h"
#include "../clsr.h"

typedef struct conc_Once {
    var_(state, atom_V$u32);
    var_(done_evt, conc_OnceEvt);
} conc_Once;
#define conc_Once_init_static(/*void*/) ____conc_Once_init_static()
$extern fn_((conc_Once_init(void))(conc_Once));
$extern fn_((conc_Once_fini(conc_Once* self))(void));
$extern fn_((conc_Once_tryBegin(conc_Once* self))(bool));
$extern fn_((conc_Once_finish(conc_Once* self))(void));
$extern fn_((conc_Once_isDone(conc_Once const* self))(bool));
$attr($must_check)
$extern fn_((conc_Once_wait(conc_Once* self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_Once_run(conc_Once* self, Sched sched, Clsr$Void* clsr))(Sched_Cancelable$void));

#define ____conc_Once_init_static() l$((conc_Once){ \
    .state = atom_V_init(0u), \
    .done_evt = conc_OnceEvt_init_static(), \
})

#if defined(__cplusplus)
}
#endif
#endif
