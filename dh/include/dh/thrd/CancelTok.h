/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    CancelTok.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_CancelTok
 *
 * @brief   Cancellation token for thread-domain cooperative cancellation
 * @details Defines a read-only cancellation token and its source.
 */
#pragma once
#ifndef thrd_CancelTok__included
#define thrd_CancelTok__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "OnceEvt.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_CancelTok_Src {
    var_(event, thrd_OnceEvt);
} thrd_CancelTok_Src;
#define thrd_CancelTok_Src_init_static(/*void*/) \
    ____thrd_CancelTok_Src_init_static()
$extern fn_((thrd_CancelTok_Src_init(void))(thrd_CancelTok_Src));
$extern fn_((thrd_CancelTok_Src_fini(thrd_CancelTok_Src* self))(void));
typedef struct thrd_CancelTok thrd_CancelTok;
$extern fn_((thrd_CancelTok_Src_tok(thrd_CancelTok_Src* self))(thrd_CancelTok));
$extern fn_((thrd_CancelTok_Src_isCanceled(thrd_CancelTok_Src* self))(bool));
$extern fn_((thrd_CancelTok_Src_cancel(thrd_CancelTok_Src* self))(void));

struct thrd_CancelTok {
    var_(event, thrd_OnceEvt_Tok);
};
$extern fn_((thrd_CancelTok_isCanceled(thrd_CancelTok self))(bool));
$attr($must_check)
$extern fn_((thrd_CancelTok_check(thrd_CancelTok self))(Sched_Cancelable$void));
$extern fn_((thrd_CancelTok_tryWait(thrd_CancelTok self))(bool));
$extern fn_((thrd_CancelTok_waitProtcd(thrd_CancelTok self))(void));
$extern fn_((thrd_CancelTok_wakeable(thrd_CancelTok self))(thrd_Wakeable));

/*========== Macros and Definitions =========================================*/

#define ____thrd_CancelTok_Src_init_static() l$((thrd_CancelTok_Src){ \
    .event = thrd_OnceEvt_init_static(), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_CancelTok__included */
