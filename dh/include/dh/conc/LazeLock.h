/**
 * @file    LazeLock.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_LazeLock
 */
#pragma once
#ifndef conc_LazeLock__included
#define conc_LazeLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "OnceLock.h"

#define conc_LazeLock$(_T...) tpl$(conc_LazeLock, _T)
#define T_decl_conc_LazeLock$(_T...) $maybe_unused typedef struct conc_LazeLock$(_T) conc_LazeLock$(_T)
#define T_impl_conc_LazeLock$(_T...) struct conc_LazeLock$(_T) { var_(once, conc_Once); var_(value, _T); }
#define T_use_conc_LazeLock$(_T...) T_decl_conc_LazeLock$(_T); T_impl_conc_LazeLock$(_T)
#define conc_LazeLock_init$(_T...) l$((conc_LazeLock$(_T)){ .once = conc_Once_init_static(), .value = cleared(), })
#define conc_LazeLock_isSet(_p_self) conc_Once_isDone(&(_p_self)->once)
#define conc_LazeLock_getOrInit(_p_self, _sched, _init_expr) __step__conc_LazeLock_getOrInit(_p_self, _sched, _init_expr)
#define __step__conc_LazeLock_getOrInit(_p_self, _sched, _init_expr) ({ \
    if (conc_Once_tryBegin(&(_p_self)->once)) { \
        (_p_self)->value = (_init_expr); \
        conc_Once_finish(&(_p_self)->once); \
    } else { \
        try_(conc_Once_wait(&(_p_self)->once, _sched)); \
    } \
    &(_p_self)->value; \
})

#if defined(__cplusplus)
}
#endif
#endif
