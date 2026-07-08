/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Que.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Que
 * @brief   Endpoint-less OS-thread blocking queue surface.
 */
#pragma once
#ifndef thrd_Que__included
#define thrd_Que__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "chan.h"
#include "../ArrQue.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Que {
    var_(que, ArrQue);
    var_(closed, bool);
    var_(lock, thrd_Mtx);
    var_(not_empty, thrd_Cond);
    var_(not_full, thrd_Cond);
} thrd_Que;
$extern fn_((thrd_Que_init(u_S$raw buf))(thrd_Que));
$extern fn_((thrd_Que_fini(thrd_Que* self, TypeInfo type))(void));

$extern fn_((thrd_Que_len(thrd_Que* self))(usize));
$extern fn_((thrd_Que_cap(thrd_Que* self))(usize));

$extern fn_((thrd_Que_close(thrd_Que* self))(void));
$extern fn_((thrd_Que_isClosed(thrd_Que* self))(bool));

$attr($must_check)
$extern fn_((thrd_Que_send(thrd_Que* self, u_V$raw item))(thrd_chan_WaitE$void));
$attr($must_check)
$extern fn_((thrd_Que_trySend(thrd_Que* self, u_V$raw item))(thrd_chan_E$void));
$attr($must_check)
$extern fn_((thrd_Que_sendFor(thrd_Que* self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void));

$attr($must_check)
$extern fn_((thrd_Que_recv(thrd_Que* self, u_V$raw out))(thrd_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((thrd_Que_tryRecv(thrd_Que* self, u_V$raw out))(thrd_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((thrd_Que_recvFor(thrd_Que* self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Que__included */
