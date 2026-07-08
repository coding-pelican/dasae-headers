/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    MPMC.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_MPMC
 * @brief   MPMC OS-thread blocking channel surface.
 */
#pragma once
#ifndef thrd_MPMC__included
#define thrd_MPMC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Que.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_MPMC {
    var_(que, thrd_Que);
} thrd_MPMC;
$extern fn_((thrd_MPMC_init(u_S$raw buf))(thrd_MPMC));
$extern fn_((thrd_MPMC_fini(thrd_MPMC* self, TypeInfo type))(void));

typedef struct thrd_MPMC_Tx {
    var_(chan, thrd_MPMC*);
    var_(type, debug_TypeInfo);
} thrd_MPMC_Tx;
$extern fn_((thrd_MPMC_tx(thrd_MPMC* self, TypeInfo type))(thrd_MPMC_Tx));
$attr($must_check)
$extern fn_((thrd_MPMC_Tx_trySend(thrd_MPMC_Tx self, u_V$raw item))(thrd_chan_E$void));
$attr($must_check)
$extern fn_((thrd_MPMC_Tx_send(thrd_MPMC_Tx self, u_V$raw item))(thrd_chan_WaitE$void));
$attr($must_check)
$extern fn_((thrd_MPMC_Tx_sendFor(thrd_MPMC_Tx self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void));

typedef struct thrd_MPMC_Rx {
    var_(chan, thrd_MPMC*);
    var_(type, debug_TypeInfo);
} thrd_MPMC_Rx;
$extern fn_((thrd_MPMC_rx(thrd_MPMC* self, TypeInfo type))(thrd_MPMC_Rx));
$attr($must_check)
$extern fn_((thrd_MPMC_Rx_tryRecv(thrd_MPMC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((thrd_MPMC_Rx_recv(thrd_MPMC_Rx self, u_V$raw out))(thrd_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((thrd_MPMC_Rx_recvFor(thrd_MPMC_Rx self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw));

typedef Tup$$(thrd_MPMC_Tx, thrd_MPMC_Rx) thrd_MPMC_Chans;
$extern fn_((thrd_MPMC_open(thrd_MPMC* self, TypeInfo type))(thrd_MPMC_Chans));
$extern fn_((thrd_MPMC_close(thrd_MPMC* self, TypeInfo type))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_MPMC__included */
