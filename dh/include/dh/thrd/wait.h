#pragma once
#ifndef thrd_wait__included
#define thrd_wait__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../ListSgl.h"

/*========== Macros and Declarations ========================================*/

/*
 * A link may point to a stack-owned waiter. The owner must unlink it from every
 * source before the waiter leaves scope. Source implementations must serialize
 * link, unlink, and wake against the same wait-list lock so wakeFn cannot run
 * after unlink returns.
 */
T_alias$((thrd_wait_Link_Data)(struct thrd_wait_Link_Data {
    var_(wake_ctx, P$raw);
    fn_(((*wakeFn)(P$raw ctx))(void));
    var_(case_idx, usize);
}));
T_use$((thrd_wait_Link_Data)(ListSgl_Link, ListSgl_Adp, ListSgl));
T_alias$((thrd_wait_Link)(ListSgl_Adp$thrd_wait_Link_Data));
T_use$((thrd_wait_Link)(P, (O, P)));

typedef struct thrd_wait_List {
    var_(inner, ListSgl$thrd_wait_Link_Data);
} thrd_wait_List;
#define thrd_wait_List_init_static(/*void*/) \
    ____thrd_wait_List_init_static()
$extern fn_((thrd_wait_List_init(void))(thrd_wait_List));
$extern fn_((thrd_wait_List_prepend(thrd_wait_List* self, thrd_wait_Link* link))(void));
$extern fn_((thrd_wait_List_wakeAll(thrd_wait_List* self))(void));
$extern fn_((thrd_wait_List_wakeOne(thrd_wait_List* self))(void));
$extern fn_((thrd_wait_List_unlink(thrd_wait_List* self, thrd_wait_Link* link))(void));

T_alias$((thrd_wait_Src_VTbl)(struct thrd_wait_Src_VTbl));
T_alias$((thrd_wait_Src)(struct thrd_wait_Src {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(thrd_wait_Src_VTbl));
}));
T_use_O$(thrd_wait_Src);
$attr($inline_always)
$static fn_((thrd_wait_Src_isValid(thrd_wait_Src self))(bool));
$attr($inline_always)
$static fn_((thrd_wait_Src_assertValid(P$raw ctx, P_const$$(thrd_wait_Src_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((thrd_wait_Src_ensureValid(thrd_wait_Src self))(thrd_wait_Src));

struct thrd_wait_Src_VTbl {
    fn_(((*readyFn)(P$raw ctx))(bool));
    fn_(((*linkFn)(P$raw ctx, thrd_wait_Link* link))(bool));
    fn_(((*unlinkFn)(P$raw ctx, thrd_wait_Link* link))(void));
};

/*========== Macros and Definitions =========================================*/

#define ____thrd_wait_List_init_static() l$((thrd_wait_List){ \
    .inner = { \
        .first = none(), \
        .type = $typing(typeInfo$(thrd_wait_Link_Data)), \
    }, \
})

#if on_analysis_active_only || on_comptime
fn_((thrd_wait_Src_isValid(thrd_wait_Src self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->readyFn)
        && isNonnull(self.vtbl->linkFn)
        && isNonnull(self.vtbl->unlinkFn);
};
fn_((thrd_wait_Src_assertValid(P$raw ctx, P_const$$(thrd_wait_Src_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->readyFn);
    claim_assert_nonnull(vtbl->linkFn);
    claim_assert_nonnull(vtbl->unlinkFn);
};
fn_((thrd_wait_Src_ensureValid(thrd_wait_Src self))(thrd_wait_Src)) {
    return thrd_wait_Src_assertValid(self.ctx, self.vtbl), self;
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_wait__included */
