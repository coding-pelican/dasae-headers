/**
 * @file    Mtx.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Mtx
 * @brief   Scheduler-aware Mtx surface.
 */
#pragma once
#ifndef conc_Mtx__included
#define conc_Mtx__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"

typedef struct conc_Mtx {
    var_(state, atom_V$usize);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
} conc_Mtx;

#define conc_Mtx_init_static(/*void*/) l$((conc_Mtx){ .state = atom_V_init(0), .lock = atom_SpinLock_init(), .waiters = none(), })
$extern fn_((conc_Mtx_init(void))(conc_Mtx));
$extern fn_((conc_Mtx_fini(conc_Mtx* self))(void));

#if defined(__cplusplus)
}
#endif
#endif
