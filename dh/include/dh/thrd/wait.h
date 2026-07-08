#pragma once
#ifndef thrd_wait__included
#define thrd_wait__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../Sched/base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((thrd_wait_Link)(struct thrd_wait_Link));
T_use$((thrd_wait_Link)(P, (O, P)));
struct thrd_wait_Link {
    var_(next, O$P$thrd_wait_Link);
    var_(wake_ctx, P$raw);
    fn_(((*wakeFn)(P$raw ctx))(void));
    var_(case_idx, usize);
};

T_alias$((thrd_wait_Src_VTbl)(struct thrd_wait_Src_VTbl));
T_alias$((thrd_wait_Src)(struct thrd_wait_Src {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(thrd_wait_Src_VTbl));
}));
struct thrd_wait_Src_VTbl {
    fn_(((*pollFn)(P$raw ctx))(O$u_P$raw));
    fn_(((*linkFn)(P$raw ctx, thrd_wait_Link* link))(bool));
    fn_(((*unlinkFn)(P$raw ctx, thrd_wait_Link* link))(void));
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_wait__included */
