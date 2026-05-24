/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Term.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_Term
 */
#ifndef daterm_Term__included
#define daterm_Term__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Event.h"
#include <dh/io/Reader.h>
#include <dh/io/Writer.h>
#include <dh/time/Duration.h>

/*========== Macros and Declarations ========================================*/

errset_((daterm_Term_E)(
    daterm_Term_Timeout
));
T_use_E$($set(daterm_Term_E)(daterm_Event));

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
/// Wait for next event (blocking)
/// Requires terminal to be in raw mode
$extern fn_((daterm_Term_wait(daterm_Term self))(daterm_Event));
/// Parse next event with timeout (blocking up to timeout)
/// Returns `Timeout` if timeout expires
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Term_timedWait(daterm_Term self, time_Duration timeout))(daterm_Term_E$daterm_Event));

/* --- IO Contracts --- */

$extern fn_((daterm_Term_reader(daterm_Term self))(io_Reader));
$extern fn_((daterm_Term_writer(daterm_Term self))(io_Writer));

/* --- Queries --- */

/// Get current screen size
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Term_queryScreenSize(daterm_Term self))(E$daterm_Size));
/// Get current cursor position
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Term_queryCursorPos(daterm_Term self))(E$daterm_Pos));

struct daterm_Term_VTbl {
    $attr($must_check)
    fn_(((*pollFn)(P$raw ctx))(O$daterm_Event));
    fn_(((*waitFn)(P$raw ctx))(daterm_Event));
    $attr($must_check)
    fn_(((*timedWaitFn)(P$raw ctx, time_Duration timeout))(daterm_Term_E$daterm_Event));
    fn_(((*readerFn)(P$raw ctx))(io_Reader));
    fn_(((*writerFn)(P$raw ctx))(io_Writer));
    $attr($must_check)
    fn_(((*queryScreenSizeFn)(P$raw ctx))(E$daterm_Size));
    $attr($must_check)
    fn_(((*queryCursorPosFn)(P$raw ctx))(E$daterm_Pos));
};

/*========== Macros and Definitions =========================================*/

fn_((daterm_Term_isValid(daterm_Term self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->pollFn)
        && isNonnull(self.vtbl->waitFn)
        && isNonnull(self.vtbl->timedWaitFn)
        && isNonnull(self.vtbl->readerFn)
        && isNonnull(self.vtbl->writerFn)
        && isNonnull(self.vtbl->queryScreenSizeFn)
        && isNonnull(self.vtbl->queryCursorPosFn);
};
fn_((daterm_Term_assertValid(P$raw ctx, P_const$$(daterm_Term_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->pollFn);
    claim_assert_nonnull(vtbl->waitFn);
    claim_assert_nonnull(vtbl->timedWaitFn);
    claim_assert_nonnull(vtbl->readerFn);
    claim_assert_nonnull(vtbl->writerFn);
    claim_assert_nonnull(vtbl->queryScreenSizeFn);
    claim_assert_nonnull(vtbl->queryCursorPosFn);
};
fn_((daterm_Term_ensureValid(daterm_Term self))(daterm_Term)) {
    return daterm_Term_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Term__included */
