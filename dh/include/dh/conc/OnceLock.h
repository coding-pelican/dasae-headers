/**
 * @file    OnceLock.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_OnceLock
 */
#pragma once
#ifndef conc_OnceLock__included
#define conc_OnceLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "Once.h"

#define conc_OnceLock$(_T...) tpl$(conc_OnceLock, _T)
#define T_decl_conc_OnceLock$(_T...) $maybe_unused typedef struct conc_OnceLock$(_T) conc_OnceLock$(_T)
#define T_impl_conc_OnceLock$(_T...) struct conc_OnceLock$(_T) { var_(once, conc_Once); var_(value, _T); }
#define T_use_conc_OnceLock$(_T...) T_decl_conc_OnceLock$(_T); T_impl_conc_OnceLock$(_T)
#define conc_OnceLock_init$(_T...) l$((conc_OnceLock$(_T)){ .once = conc_Once_init_static(), .value = cleared(), })
#define conc_OnceLock_isSet(_p_self) conc_Once_isDone(&(_p_self)->once)
#define conc_OnceLock_tryGet(_p_self) (conc_Once_isDone(&(_p_self)->once) ? some(&(_p_self)->value) : none())
#define conc_OnceLock_get(_p_self, _sched) (try_(conc_Once_wait(&(_p_self)->once, _sched)), &(_p_self)->value)
#define conc_OnceLock_trySet(_p_self, _val) __step__conc_OnceLock_trySet(_p_self, _val)
#define __step__conc_OnceLock_trySet(_p_self, _val) ({ \
    let_(__ok, bool) = conc_Once_tryBegin(&(_p_self)->once); \
    if (__ok) { (_p_self)->value = (_val); conc_Once_finish(&(_p_self)->once); } \
    __ok; \
})

#if defined(__cplusplus)
}
#endif
#endif
