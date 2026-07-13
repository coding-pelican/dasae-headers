#pragma once
#ifndef thrd_Wakeable__included
#define thrd_Wakeable__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "wait.h"

/*========== Macros and Declarations ========================================*/

T_alias$((thrd_Wakeable_VTbl)(struct thrd_Wakeable_VTbl));
T_alias$((thrd_Wakeable)(struct thrd_Wakeable {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(thrd_Wakeable_VTbl));
}));
T_use_O$(thrd_Wakeable);
$attr($inline_always)
$static fn_((thrd_Wakeable_isValid(thrd_Wakeable self))(bool));
$attr($inline_always)
$static fn_((thrd_Wakeable_assertValid(P$raw ctx, P_const$$(thrd_Wakeable_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((thrd_Wakeable_ensureValid(thrd_Wakeable self))(thrd_Wakeable));

$extern fn_((thrd_Wakeable_ready(thrd_Wakeable self))(bool));
$extern fn_((thrd_Wakeable_link(thrd_Wakeable self, thrd_wait_Link* link))(bool));
$extern fn_((thrd_Wakeable_unlink(thrd_Wakeable self, thrd_wait_Link* link))(void));

struct thrd_Wakeable_VTbl {
    fn_(((*readyFn)(P$raw ctx))(bool));
    fn_(((*linkFn)(P$raw ctx, thrd_wait_Link* link))(bool));
    fn_(((*unlinkFn)(P$raw ctx, thrd_wait_Link* link))(void));
};

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((thrd_Wakeable_isValid(thrd_Wakeable self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->readyFn)
        && isNonnull(self.vtbl->linkFn)
        && isNonnull(self.vtbl->unlinkFn);
};
fn_((thrd_Wakeable_assertValid(P$raw ctx, P_const$$(thrd_Wakeable_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->readyFn);
    claim_assert_nonnull(vtbl->linkFn);
    claim_assert_nonnull(vtbl->unlinkFn);
};
fn_((thrd_Wakeable_ensureValid(thrd_Wakeable self))(thrd_Wakeable)) {
    return thrd_Wakeable_assertValid(self.ctx, self.vtbl), self;
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Wakeable__included */
