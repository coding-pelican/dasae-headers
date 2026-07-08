/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    MPSC.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_MPSC
 * @brief   MPSC OS-thread blocking channel surface.
 */
#pragma once
#ifndef thrd_MPSC__included
#define thrd_MPSC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Que.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_MPSC {
    var_(que, thrd_Que);
} thrd_MPSC;
$extern fn_((thrd_MPSC_init(u_S$raw buf))(thrd_MPSC));
$extern fn_((thrd_MPSC_fini(thrd_MPSC* self, TypeInfo type))(void));

typedef struct thrd_MPSC_Tx {
    var_(chan, thrd_MPSC*);
    var_(type, debug_TypeInfo);
} thrd_MPSC_Tx;
$extern fn_((thrd_MPSC_tx(thrd_MPSC* self, TypeInfo type))(thrd_MPSC_Tx));
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_trySend(thrd_MPSC_Tx self, u_V$raw item))(thrd_chan_E$void));
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_send(thrd_MPSC_Tx self, u_V$raw item))(thrd_chan_WaitE$void));
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_sendFor(thrd_MPSC_Tx self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void));

typedef struct thrd_MPSC_Rx {
    var_(chan, thrd_MPSC*);
    var_(type, debug_TypeInfo);
} thrd_MPSC_Rx;
$extern fn_((thrd_MPSC_rx(thrd_MPSC* self, TypeInfo type))(thrd_MPSC_Rx));
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_recv(thrd_MPSC_Rx self, u_V$raw out))(thrd_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_tryRecv(thrd_MPSC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_recvFor(thrd_MPSC_Rx self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw));

typedef Tup$$(thrd_MPSC_Tx, thrd_MPSC_Rx) thrd_MPSC_Chans;
$extern fn_((thrd_MPSC_open(thrd_MPSC* self, TypeInfo type))(thrd_MPSC_Chans));
$extern fn_((thrd_MPSC_close(thrd_MPSC* self, TypeInfo type))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_MPSC__included */
