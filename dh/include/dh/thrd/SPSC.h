/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SPSC.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_SPSC
 * @brief   SPSC OS-thread blocking channel surface.
 */
#pragma once
#ifndef thrd_SPSC__included
#define thrd_SPSC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "chan.h"
#include "../ArrQue.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_SPSC {
    var_(buf, S$raw);
    var_(head, usize);
    var_(len, usize);
    var_(closed, bool);
    var_(lock, thrd_Mtx);
    var_(not_empty, thrd_Cond);
    var_(not_full, thrd_Cond);
    var_(type, debug_TypeInfo);
} thrd_SPSC;
$extern fn_((thrd_SPSC_init(u_S$raw buf))(thrd_SPSC));
$extern fn_((thrd_SPSC_fini(thrd_SPSC* self, TypeInfo type))(void));

typedef struct thrd_SPSC_Tx {
    var_(chan, thrd_SPSC*);
    var_(type, debug_TypeInfo);
} thrd_SPSC_Tx;
$extern fn_((thrd_SPSC_tx(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Tx));
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_send(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_WaitE$void));
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_trySend(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_E$void));
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_sendFor(thrd_SPSC_Tx self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void));

typedef struct thrd_SPSC_Rx {
    var_(chan, thrd_SPSC*);
    var_(type, debug_TypeInfo);
} thrd_SPSC_Rx;
$extern fn_((thrd_SPSC_rx(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Rx));
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_tryRecv(thrd_SPSC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_recv(thrd_SPSC_Rx self, u_V$raw out))(thrd_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_recvFor(thrd_SPSC_Rx self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw));

typedef Tup$$(thrd_SPSC_Tx, thrd_SPSC_Rx) thrd_SPSC_Chans;
$extern fn_((thrd_SPSC_open(thrd_SPSC* self, TypeInfo type))(thrd_SPSC_Chans));
$extern fn_((thrd_SPSC_close(thrd_SPSC* self, TypeInfo type))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_SPSC__included */
