/**
 * @file    Sem.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Sem
 * @brief   Scheduler-aware Sem surface.
 */
#pragma once
#ifndef conc_Sem__included
#define conc_Sem__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"

typedef struct conc_Sem {
    var_(state, atom_V$usize);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
} conc_Sem;

#define conc_Sem_init_static(/*void*/) l$((conc_Sem){ .state = atom_V_init(0), .lock = atom_SpinLock_init(), .waiters = none(), })
$extern fn_((conc_Sem_init(void))(conc_Sem));
$extern fn_((conc_Sem_fini(conc_Sem* self))(void));

#if defined(__cplusplus)
}
#endif
#endif
