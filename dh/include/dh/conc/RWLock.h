/**
 * @file    RWLock.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_RWLock
 * @brief   Scheduler-aware RWLock surface.
 */
#pragma once
#ifndef conc_RWLock__included
#define conc_RWLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"

typedef struct conc_RWLock {
    var_(state, atom_V$usize);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
} conc_RWLock;

#define conc_RWLock_init_static(/*void*/) l$((conc_RWLock){ .state = atom_V_init(0), .lock = atom_SpinLock_init(), .waiters = none(), })
$extern fn_((conc_RWLock_init(void))(conc_RWLock));
$extern fn_((conc_RWLock_fini(conc_RWLock* self))(void));

#if defined(__cplusplus)
}
#endif
#endif
