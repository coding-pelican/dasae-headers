#pragma once
#ifndef thrd_wait__included
#define thrd_wait__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Waker.h"
#include "../ListSgl.h"

/*========== Macros and Declarations ========================================*/

/*
 * A link may point to a stack-owned waiter. The owner must unlink it from every
 * source before the waiter leaves scope. Source implementations must serialize
 * link, unlink, and wake against the same wait-list lock so wakeFn cannot run
 * after unlink returns.
 */
T_alias$((thrd_wait_Link_Data)(struct thrd_wait_Link_Data {
    var_(waker, thrd_Waker);
    var_(case_idx, usize);
}));
T_use$((thrd_wait_Link_Data)(ListSgl_Link, ListSgl_Adp, ListSgl));

T_alias$((thrd_wait_Link)(struct thrd_wait_Link {
    var_(inner, ListSgl_Adp$thrd_wait_Link_Data);
}));
T_use$((thrd_wait_Link)(P, (O, P)));
$extern fn_((thrd_wait_Link_from(thrd_Waker waker, usize case_idx))(thrd_wait_Link));
$extern fn_((thrd_wait_Link_data(const thrd_wait_Link* link))(const thrd_wait_Link_Data*));
$extern fn_((thrd_wait_Link_dataMut(thrd_wait_Link* link))(thrd_wait_Link_Data*));

T_alias$((thrd_wait_Chain)(struct thrd_wait_Chain {
    var_(inner, ListSgl$thrd_wait_Link_Data);
}));
#define thrd_wait_Chain_init_static(/*void*/) \
    ____thrd_wait_Chain_init_static()
$extern fn_((thrd_wait_Chain_init(void))(thrd_wait_Chain));
$extern fn_((thrd_wait_Chain_prepend(thrd_wait_Chain* self, thrd_wait_Link* link))(void));
$extern fn_((thrd_wait_Chain_wakeAll(thrd_wait_Chain* self))(void));
$extern fn_((thrd_wait_Chain_wakeOne(thrd_wait_Chain* self))(void));
$extern fn_((thrd_wait_Chain_unlink(thrd_wait_Chain* self, thrd_wait_Link* link))(void));

/*========== Macros and Definitions =========================================*/

#define ____thrd_wait_Chain_init_static() l$((thrd_wait_Chain){ \
    .inner = { \
        .first = none(), \
        .type = $typing(typeInfo$(thrd_wait_Link_Data)), \
    }, \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_wait__included */
