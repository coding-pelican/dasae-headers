/**
 * @file    Cond.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Cond
 * @brief   Scheduler-aware Cond surface.
 */
#pragma once
#ifndef conc_Cond__included
#define conc_Cond__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"

typedef struct conc_Cond {
    var_(state, atom_V$usize);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
} conc_Cond;

#define conc_Cond_init_static(/*void*/) l$((conc_Cond){ .state = atom_V_init(0), .lock = atom_SpinLock_init(), .waiters = none(), })
$extern fn_((conc_Cond_init(void))(conc_Cond));
$extern fn_((conc_Cond_fini(conc_Cond* self))(void));

#if defined(__cplusplus)
}
#endif
#endif
