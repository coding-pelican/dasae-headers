/**
 * @file    conc/ResetEvt.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_ResetEvt
 */
#pragma once
#ifndef conc_ResetEvt__included
#define conc_ResetEvt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

T_alias$((conc_ResetEvt)(struct conc_ResetEvt));

T_alias$((conc_ResetEvt_State)(enum_((conc_ResetEvt_State $fits($packed))(
    conc_ResetEvt_State_unset = 0,
    conc_ResetEvt_State_set,
))));
claim_assert_static(eqlType$(conc_ResetEvt_State, u8));
T_use_atom_V$(conc_ResetEvt_State);

struct conc_ResetEvt {
    var_(state, atom_V$conc_ResetEvt_State);
    var_(lock, atom_SpinLock);
    var_(waiters, O$P$conc_AwaitLink);
};
T_alias$((conc_ResetEvt_Tok)(struct conc_ResetEvt_Tok));
T_alias$((conc_ResetEvt_Sig)(struct conc_ResetEvt_Sig));
struct conc_ResetEvt_Tok {
    var_(evt, conc_ResetEvt*);
};
struct conc_ResetEvt_Sig {
    var_(evt, conc_ResetEvt*);
};

#define conc_ResetEvt_init_static(/*void*/) ____conc_ResetEvt_init_static()
$extern fn_((conc_ResetEvt_init(void))(conc_ResetEvt));
$extern fn_((conc_ResetEvt_tok(conc_ResetEvt* self))(conc_ResetEvt_Tok));
$extern fn_((conc_ResetEvt_sig(conc_ResetEvt* self))(conc_ResetEvt_Sig));
$extern fn_((conc_ResetEvt_isSet(const conc_ResetEvt* self))(bool));
$extern fn_((conc_ResetEvt_set(conc_ResetEvt* self))(void));
$extern fn_((conc_ResetEvt_reset(conc_ResetEvt* self))(void));
$attr($must_check)
$extern fn_((conc_ResetEvt_wait(conc_ResetEvt* self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_ResetEvt_waitFor(conc_ResetEvt* self, Sched sched, time_Dur dur))(Sched_TimedE$void));
$attr($must_check)
$extern fn_((conc_ResetEvt_waitUntil(conc_ResetEvt* self, Sched sched, time_Inst deadline))(Sched_TimedE$void));
$extern fn_((conc_ResetEvt_Tok_isSet(conc_ResetEvt_Tok self))(bool));
$attr($must_check)
$extern fn_((conc_ResetEvt_Tok_wait(conc_ResetEvt_Tok self, Sched sched))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((conc_ResetEvt_Tok_waitFor(conc_ResetEvt_Tok self, Sched sched, time_Dur dur))(Sched_TimedE$void));
$attr($must_check)
$extern fn_((conc_ResetEvt_Tok_waitUntil(conc_ResetEvt_Tok self, Sched sched, time_Inst deadline))(Sched_TimedE$void));
$extern fn_((conc_ResetEvt_Sig_set(conc_ResetEvt_Sig self))(void));
$extern fn_((conc_ResetEvt_Sig_reset(conc_ResetEvt_Sig self))(void));
$extern fn_((conc_ResetEvt_asAwakeable(conc_ResetEvt* self))(conc_Awakeable));

#define ____conc_ResetEvt_init_static() l$((conc_ResetEvt){ \
    .state = atom_V_init(conc_ResetEvt_State_unset), \
    .lock = atom_SpinLock_init(), \
    .waiters = none(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_ResetEvt__included */
