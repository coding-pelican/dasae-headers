#pragma once
#ifndef thrd_Waker__included
#define thrd_Waker__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Waker {
    var_(ctx, P$raw);
    fn_(((*wakeFn)(P$raw ctx))(void));
} thrd_Waker;
$attr($inline_always)
$static fn_((thrd_Waker_isValid(thrd_Waker self))(bool));
$attr($inline_always)
$static fn_((thrd_Waker_assertValid(P$raw ctx, fn_(((*wakeFn)(P$raw ctx))(void))))(void));
$attr($inline_always)
$static fn_((thrd_Waker_ensureValid(thrd_Waker self))(thrd_Waker));

$extern fn_((thrd_Waker_wake(thrd_Waker self))(void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((thrd_Waker_isValid(thrd_Waker self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.wakeFn);
};
fn_((thrd_Waker_assertValid(P$raw ctx, fn_(((*wakeFn)(P$raw ctx))(void))))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(wakeFn);
};
fn_((thrd_Waker_ensureValid(thrd_Waker self))(thrd_Waker)) {
    return thrd_Waker_assertValid(self.ctx, self.wakeFn), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Waker__included */
