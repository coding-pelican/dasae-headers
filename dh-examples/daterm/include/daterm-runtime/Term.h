/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Term.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_Term
 */
#pragma once
#ifndef daterm_Term__included
#define daterm_Term__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Event.h"
#include "Caps.h"
#include "Query.h"
#include "Txn.h"
#include <dh/io/Reader.h>
#include <dh/io/Writer.h>
#include <dh/time/Dur.h>
#include <dh/Sched.h>

/*========== Macros and Declarations ========================================*/

errset_((daterm_Term_WaitE)() $union_errset_(Sched_Cancelable, Sched_TimeoutE));
T_use_E$($set(Sched_Cancelable)(daterm_Event));
T_use_E$($set(daterm_Term_WaitE)(daterm_Event));

T_alias$((daterm_Term_VTbl)(struct daterm_Term_VTbl));
T_alias$((daterm_Term)(struct daterm_Term {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(daterm_Term_VTbl));
}));
T_use_prl$(daterm_Term);

$attr($inline_always)
$static fn_((daterm_Term_isValid(daterm_Term self))(bool));
$attr($inline_always)
$static fn_((daterm_Term_assertValid(P$raw ctx, P_const$$(daterm_Term_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((daterm_Term_ensureValid(daterm_Term self))(daterm_Term));

/* --- Event Polling --- */

/// Parse next event from terminal input (non-blocking)
/// Returns `none` if no event available
/// Requires terminal to be in raw mode
$extern fn_((daterm_Term_poll(daterm_Term self))(O$daterm_Event));
/// Wait for next event; may return `Sched_Canceled`.
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Term_wait(daterm_Term self))(Sched_Cancelable$daterm_Event));
/// Wait for next event, cancel, or timeout.
/// Returns `Sched_Timeout` if timeout expires.
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Term_waitTimed(daterm_Term self, time_Dur timeout))(daterm_Term_WaitE$daterm_Event));
/// Protected wait for next event; does not introduce a cancellation point.
/// Requires terminal to be in raw mode
$extern fn_((daterm_Term_waitProtcd(daterm_Term self))(daterm_Event));

/* --- IO Contracts --- */

$extern fn_((daterm_Term_reader(daterm_Term self))(io_Reader));
$extern fn_((daterm_Term_writer(daterm_Term self))(io_Writer));
$attr($must_check)
$extern fn_((daterm_Term_flush(daterm_Term self))(E$void));
$extern fn_((daterm_Term_caps(daterm_Term self))(daterm_TermCaps));

/* --- Runtime Queries and Transactions --- */

$attr($must_check)
$extern fn_((daterm_Term_queryLocal(daterm_Term self, daterm_Query query))(E$daterm_Query_Result));
$attr($must_check)
$extern fn_((daterm_Term_queryNativeScreenCells(daterm_Term self))(E$daterm_CellSize));
$attr($must_check)
$extern fn_((daterm_Term_queryCachedScreenCells(daterm_Term self))(E$daterm_CellSize));
$attr($must_check)
$extern fn_((daterm_Term_queryNativeCursorPos(daterm_Term self))(E$daterm_Pos));
$attr($must_check)
$extern fn_((daterm_Term_runTxn(daterm_Term self, daterm_Txn txn))(daterm_Txn_E$Void));

struct daterm_Term_VTbl {
    $attr($must_check)
    fn_(((*pollFn)(P$raw ctx))(O$daterm_Event));
    $attr($must_check)
    fn_(((*waitFn)(P$raw ctx))(Sched_Cancelable$daterm_Event));
    $attr($must_check)
    fn_(((*waitTimedFn)(P$raw ctx, time_Dur timeout))(daterm_Term_WaitE$daterm_Event));
    fn_(((*waitProtcdFn)(P$raw ctx))(daterm_Event));

    fn_(((*readerFn)(P$raw ctx))(io_Reader));
    fn_(((*writerFn)(P$raw ctx))(io_Writer));
    $attr($must_check)
    fn_(((*flushFn)(P$raw ctx))(E$void));
    fn_(((*capsFn)(P$raw ctx))(daterm_TermCaps));

    $attr($must_check)
    fn_(((*queryLocalFn)(P$raw ctx, daterm_Query query))(E$daterm_Query_Result));
    $attr($must_check)
    fn_(((*runTxnFn)(P$raw ctx, daterm_Txn txn))(daterm_Txn_E$Void));
};

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((daterm_Term_isValid(daterm_Term self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->pollFn)
        && isNonnull(self.vtbl->waitFn)
        && isNonnull(self.vtbl->waitTimedFn)
        && isNonnull(self.vtbl->waitProtcdFn)
        && isNonnull(self.vtbl->readerFn)
        && isNonnull(self.vtbl->writerFn)
        && isNonnull(self.vtbl->flushFn)
        && isNonnull(self.vtbl->capsFn)
        && isNonnull(self.vtbl->queryLocalFn)
        && isNonnull(self.vtbl->runTxnFn);
};
fn_((daterm_Term_assertValid(P$raw ctx, P_const$$(daterm_Term_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->pollFn);
    claim_assert_nonnull(vtbl->waitFn);
    claim_assert_nonnull(vtbl->waitTimedFn);
    claim_assert_nonnull(vtbl->waitProtcdFn);
    claim_assert_nonnull(vtbl->readerFn);
    claim_assert_nonnull(vtbl->writerFn);
    claim_assert_nonnull(vtbl->flushFn);
    claim_assert_nonnull(vtbl->capsFn);
    claim_assert_nonnull(vtbl->queryLocalFn);
    claim_assert_nonnull(vtbl->runTxnFn);
};
fn_((daterm_Term_ensureValid(daterm_Term self))(daterm_Term)) {
    return daterm_Term_assertValid(self.ctx, self.vtbl), self;
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Term__included */
