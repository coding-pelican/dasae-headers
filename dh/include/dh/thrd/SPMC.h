/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SPMC.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_SPMC
 * @brief   SPMC OS-thread blocking channel surface.
 */
#pragma once
#ifndef thrd_SPMC__included
#define thrd_SPMC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Que.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_SPMC {
    var_(que, thrd_Que);
} thrd_SPMC;
$extern fn_((thrd_SPMC_init(u_S$raw buf))(thrd_SPMC));
$extern fn_((thrd_SPMC_fini(thrd_SPMC* self, TypeInfo type))(void));

typedef struct thrd_SPMC_Tx {
    var_(chan, thrd_SPMC*);
    var_(type, debug_TypeInfo);
} thrd_SPMC_Tx;
$extern fn_((thrd_SPMC_tx(thrd_SPMC* self, TypeInfo type))(thrd_SPMC_Tx));
$attr($must_check)
$extern fn_((thrd_SPMC_Tx_send(thrd_SPMC_Tx self, u_V$raw item))(thrd_chan_WaitE$void));
$attr($must_check)
$extern fn_((thrd_SPMC_Tx_trySend(thrd_SPMC_Tx self, u_V$raw item))(thrd_chan_E$void));
$attr($must_check)
$extern fn_((thrd_SPMC_Tx_sendFor(thrd_SPMC_Tx self, u_V$raw item, time_Dur dur))(thrd_chan_TimedE$void));

typedef struct thrd_SPMC_Rx {
    var_(chan, thrd_SPMC*);
    var_(type, debug_TypeInfo);
} thrd_SPMC_Rx;
$extern fn_((thrd_SPMC_rx(thrd_SPMC* self, TypeInfo type))(thrd_SPMC_Rx));
$attr($must_check)
$extern fn_((thrd_SPMC_Rx_tryRecv(thrd_SPMC_Rx self, u_V$raw out))(thrd_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((thrd_SPMC_Rx_recv(thrd_SPMC_Rx self, u_V$raw out))(thrd_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((thrd_SPMC_Rx_recvFor(thrd_SPMC_Rx self, time_Dur dur, u_V$raw out))(thrd_chan_TimedE$u_V$raw));

typedef Tup$$(thrd_SPMC_Tx, thrd_SPMC_Rx) thrd_SPMC_Chans;
$extern fn_((thrd_SPMC_open(thrd_SPMC* self, TypeInfo type))(thrd_SPMC_Chans));
$extern fn_((thrd_SPMC_close(thrd_SPMC* self, TypeInfo type))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_SPMC__included */
