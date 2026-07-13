/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc
 *
 * @brief   Scheduler-aware concurrency composition primitives.
 * @details `conc` types own their own state only. They do not own `Sched`;
 *          any operation that may schedule, suspend, observe cancellation, or
 *          request concurrency receives `Sched` explicitly.
 */
#pragma once
#ifndef conc_base__included
#define conc_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "../atom.h"
#include "../Sched/base.h"
#include "../Future/base.h"
#include "../time/Dur.h"
#include "../time/Inst.h"

/*========== Macros and Declarations ========================================*/

errset_((conc_chan_ClosedE)(conc_chan_Closed));
T_use_E$($set(conc_chan_ClosedE)(V$raw));
errset_((conc_chan_CapE)(conc_chan_Empty, conc_chan_Full));
T_use_E$($set(conc_chan_CapE)(V$raw));
errset_((conc_chan_E)() $union_errset_(conc_chan_ClosedE, conc_chan_CapE));
T_use_E$($set(conc_chan_E)(V$raw));

errset_((conc_chan_WaitE)() $union_errset_(conc_chan_ClosedE, Sched_Cancelable));
T_use_E$($set(conc_chan_WaitE)(V$raw));
errset_((conc_chan_TimedE)() $union_errset_(conc_chan_ClosedE, Sched_Cancelable, Sched_TimeoutE));
T_use_E$($set(conc_chan_TimedE)(V$raw));
T_use_E$($set(conc_chan_ClosedE)(u_V$raw));
T_use_E$($set(conc_chan_CapE)(u_V$raw));
T_use_E$($set(conc_chan_E)(u_V$raw));
T_use_E$($set(conc_chan_WaitE)(u_V$raw));
T_use_E$($set(conc_chan_TimedE)(u_V$raw));

T_alias$((conc_Awakeable_VTbl)(struct conc_Awakeable_VTbl));
/// Erased readiness source for `conc_Select` and async waits.
T_alias$((conc_Awakeable)(struct conc_Awakeable {
    var_(ctx, P$raw);
    var_(vtbl, const conc_Awakeable_VTbl*);
}));
$attr($inline_always)
$static fn_((conc_Awakeable_init(P$raw ctx, const conc_Awakeable_VTbl* vtbl))(conc_Awakeable));

T_alias$((conc_AwaitLink_State)(enum_((conc_AwaitLink_State $fits($packed))(
    conc_AwaitLink_State_idle = 0,
    conc_AwaitLink_State_linked,
    conc_AwaitLink_State_woken,
    conc_AwaitLink_State_unlinked
))));
claim_assert_static(eqlType$(conc_AwaitLink_State, u8));
T_use_atom_V$(conc_AwaitLink_State);
T_alias$((conc_AwaitLink_WakeFn)(fn_(((*)(P$raw ctx))(void) $T)));

/// Transient intrusive link between an await source and a waiting operation.
/// This may contain a wake callback installed by `conc_Select_await` or another
/// waiting operation. The link does not make the source own `Sched`.
T_alias$((conc_AwaitLink)(struct conc_AwaitLink));
T_use_P$(conc_AwaitLink);
T_use_O$(P$conc_AwaitLink);
struct conc_AwaitLink {
    var_(state, atom_V$conc_AwaitLink_State);
    var_(next, O$P$conc_AwaitLink);
    var_(wake_ctx, P$raw);
    var_(wakeFn, conc_AwaitLink_WakeFn);
    var_(case_idx, usize);
};
$attr($inline_always)
$static fn_((conc_AwaitLink_init(P$raw wake_ctx, conc_AwaitLink_WakeFn wakeFn, usize case_idx))(conc_AwaitLink));

struct conc_Awakeable_VTbl {
    /// Try to consume a ready value without scheduling. Returns false if the
    /// source is not ready. `out` points to caller-owned storage for the
    /// source-specific payload type.
    fn_(((*pollFn)(P$raw ctx, u_P$raw out))(bool));
    /// Register `link` for a future wake. Returns true when the source became
    /// ready during registration, in which case the caller must treat the case
    /// as immediately ready and avoid parking.
    fn_(((*linkFn)(P$raw ctx, Sched sched, conc_AwaitLink* link))(bool));
    /// Remove a previously registered link when another select case wins or a
    /// wait operation is canceled/timed out.
    fn_(((*unlinkFn)(P$raw ctx, conc_AwaitLink* link))(void));
    /// Propagate cooperative cancellation into the source when supported.
    fn_(((*cancelFn)(P$raw ctx, Sched sched))(void));
};

/*========== Macro and Definitions ==========================================*/

#if on_analysis_active_only || on_comptime
fn_((conc_AwaitLink_init(P$raw wake_ctx, conc_AwaitLink_WakeFn wakeFn, usize case_idx))(conc_AwaitLink)) {
    claim_assert_nonnull(wakeFn);
    return (conc_AwaitLink){
        .state = atom_V_init(conc_AwaitLink_State_idle),
        .next = none(),
        .wake_ctx = wake_ctx,
        .wakeFn = wakeFn,
        .case_idx = case_idx,
    };
};
fn_((conc_Awakeable_init(P$raw ctx, const conc_Awakeable_VTbl* vtbl))(conc_Awakeable)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->pollFn);
    claim_assert_nonnull(vtbl->linkFn);
    claim_assert_nonnull(vtbl->unlinkFn);
    claim_assert_nonnull(vtbl->cancelFn);
    return (conc_Awakeable){ .ctx = ctx, .vtbl = vtbl };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_base__included */
