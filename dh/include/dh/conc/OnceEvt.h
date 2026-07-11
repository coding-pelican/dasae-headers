/**
 * @file    conc/OnceEvt.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_OnceEvt
 */
#pragma once
#ifndef conc_OnceEvt__included
#define conc_OnceEvt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

T_alias$((conc_OnceEvt)(struct conc_OnceEvt));

T_alias$((conc_OnceEvt_State)(enum_((conc_OnceEvt_State $fits($packed))(
    conc_OnceEvt_State_unset = 0,
    conc_OnceEvt_State_set,
))));
claim_assert_static(eqlType$(conc_OnceEvt_State, u8));
T_use_atom_V$(conc_OnceEvt_State);

struct conc_OnceEvt {
    var_(state, atom_V$conc_OnceEvt_State);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
};
T_alias$((conc_OnceEvt_Tok)(struct conc_OnceEvt_Tok));
T_alias$((conc_OnceEvt_Sig)(struct conc_OnceEvt_Sig));
struct conc_OnceEvt_Tok { var_(evt, conc_OnceEvt*); };
struct conc_OnceEvt_Sig { var_(evt, conc_OnceEvt*); };

#define conc_OnceEvt_init_static(/*void*/) ____conc_OnceEvt_init_static()
$extern fn_((conc_OnceEvt_init(void))(conc_OnceEvt));
$extern fn_((conc_OnceEvt_tok(conc_OnceEvt* self))(conc_OnceEvt_Tok));
$extern fn_((conc_OnceEvt_sig(conc_OnceEvt* self))(conc_OnceEvt_Sig));
$extern fn_((conc_OnceEvt_isSet(const conc_OnceEvt* self))(bool));
$extern fn_((conc_OnceEvt_set(conc_OnceEvt* self))(void));
$attr($must_check)
$extern fn_((conc_OnceEvt_wait(conc_OnceEvt* self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_OnceEvt_waitFor(conc_OnceEvt* self, Sched sched, time_Dur dur))(Sched_TimedE$void));
$attr($must_check)
$extern fn_((conc_OnceEvt_waitUntil(conc_OnceEvt* self, Sched sched, time_Inst deadline))(Sched_TimedE$void));
$extern fn_((conc_OnceEvt_Tok_isSet(conc_OnceEvt_Tok self))(bool));
$attr($must_check)
$extern fn_((conc_OnceEvt_Tok_wait(conc_OnceEvt_Tok self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_OnceEvt_Tok_waitFor(conc_OnceEvt_Tok self, Sched sched, time_Dur dur))(Sched_TimedE$void));
$attr($must_check)
$extern fn_((conc_OnceEvt_Tok_waitUntil(conc_OnceEvt_Tok self, Sched sched, time_Inst deadline))(Sched_TimedE$void));
$extern fn_((conc_OnceEvt_Sig_set(conc_OnceEvt_Sig self))(void));
$extern fn_((conc_OnceEvt_asAwaitSrc(conc_OnceEvt* self))(conc_AwaitSrc));

#define ____conc_OnceEvt_init_static() l$((conc_OnceEvt){ \
    .state = atom_V_init(conc_OnceEvt_State_unset), \
    .lock = atom_SpinLock_init(), \
    .waiters = none(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_OnceEvt__included */
