/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SPSC.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-07-13 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_SPSC
 *
 * @brief   SPSC OS-thread blocking channel surface.
 * @details Defines a single-producer, single-consumer OS-thread blocking
 *          channel surface.
 */
#pragma once
#ifndef thrd_SPSC__included
#define thrd_SPSC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "chan.h"
#include "Select.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_SPSC {
    var_(buf, S$raw);
    var_(head, atom_V$usize);
    var_(tail, atom_V$usize);
    var_(closed, atom_V$bool);
    var_(wait_lock, thrd_Mtx);
    var_(send_waiters, thrd_wait_Chain);
    var_(recv_waiters, thrd_wait_Chain);
    var_(type, debug_TypeInfo);
} thrd_SPSC;
$extern fn_((thrd_SPSC_init(u_S$raw buf))(thrd_SPSC));
#define T_use_thrd_SPSC_init$(_T...) __gen__T_use_thrd_SPSC_init$(_T)
$extern fn_((thrd_SPSC_fini(thrd_SPSC* self))(void));
#define T_use_thrd_SPSC_fini$(_T...) __gen__T_use_thrd_SPSC_fini$(_T)

$extern fn_((thrd_SPSC_len(thrd_SPSC* self))(usize));
#define T_use_thrd_SPSC_len$(_T...) __gen__T_use_thrd_SPSC_len$(_T)
$extern fn_((thrd_SPSC_cap(thrd_SPSC* self))(usize));
#define T_use_thrd_SPSC_cap$(_T...) __gen__T_use_thrd_SPSC_cap$(_T)
$extern fn_((thrd_SPSC_isFull(thrd_SPSC* self))(bool));
#define T_use_thrd_SPSC_isFull$(_T...) __gen__T_use_thrd_SPSC_isFull$(_T)
$extern fn_((thrd_SPSC_isEmpty(thrd_SPSC* self))(bool));
#define T_use_thrd_SPSC_isEmpty$(_T...) __gen__T_use_thrd_SPSC_isEmpty$(_T)
$extern fn_((thrd_SPSC_isOpen(thrd_SPSC* self))(bool));
#define T_use_thrd_SPSC_isOpen$(_T...) __gen__T_use_thrd_SPSC_isOpen$(_T)
$extern fn_((thrd_SPSC_isClosed(thrd_SPSC* self))(bool));
#define T_use_thrd_SPSC_isClosed$(_T...) __gen__T_use_thrd_SPSC_isClosed$(_T)

typedef struct thrd_SPSC_Tx {
    var_(chan, thrd_SPSC*);
} thrd_SPSC_Tx;
typedef struct thrd_SPSC_Tx_SendOp {
    var_(tx, thrd_SPSC_Tx);
    var_(item, u_P$raw);
} thrd_SPSC_Tx_SendOp;
typedef struct thrd_SPSC_Rx {
    var_(chan, thrd_SPSC*);
} thrd_SPSC_Rx;
typedef struct thrd_SPSC_Rx_RecvOp {
    var_(rx, thrd_SPSC_Rx);
    var_(type, TypeInfo);
} thrd_SPSC_Rx_RecvOp;
typedef Tup$$(thrd_SPSC_Tx, thrd_SPSC_Rx) thrd_SPSC_Chans;

#define thrd_SPSC$(_T...) __alias__thrd_SPSC$(_T)
#define thrd_SPSC$$(_T...) __anon__thrd_SPSC$$(_T)
#define T_decl_thrd_SPSC$(_T...) __gen__T_decl_thrd_SPSC$(_T)
#define T_impl_thrd_SPSC$(_T...) __gen__T_impl_thrd_SPSC$(_T)
#define T_use_thrd_SPSC$(_T...) __gen__T_use_thrd_SPSC$(_T)
#define thrd_SPSC_Tx$(_T...) __alias__thrd_SPSC_Tx$(_T)
#define thrd_SPSC_Tx$$(_T...) __anon__thrd_SPSC_Tx$$(_T)
#define T_decl_thrd_SPSC_Tx$(_T...) __gen__T_decl_thrd_SPSC_Tx$(_T)
#define T_impl_thrd_SPSC_Tx$(_T...) __gen__T_impl_thrd_SPSC_Tx$(_T)
#define T_use_thrd_SPSC_Tx$(_T...) __gen__T_use_thrd_SPSC_Tx$(_T)
#define thrd_SPSC_Tx_SendOp$(_T...) __alias__thrd_SPSC_Tx_SendOp$(_T)
#define thrd_SPSC_Tx_SendOp$$(_T...) __anon__thrd_SPSC_Tx_SendOp$$(_T)
#define T_decl_thrd_SPSC_Tx_SendOp$(_T...) __gen__T_decl_thrd_SPSC_Tx_SendOp$(_T)
#define T_impl_thrd_SPSC_Tx_SendOp$(_T...) __gen__T_impl_thrd_SPSC_Tx_SendOp$(_T)
#define T_use_thrd_SPSC_Tx_SendOp$(_T...) __gen__T_use_thrd_SPSC_Tx_SendOp$(_T)
#define thrd_SPSC_Rx$(_T...) __alias__thrd_SPSC_Rx$(_T)
#define thrd_SPSC_Rx$$(_T...) __anon__thrd_SPSC_Rx$$(_T)
#define T_decl_thrd_SPSC_Rx$(_T...) __gen__T_decl_thrd_SPSC_Rx$(_T)
#define T_impl_thrd_SPSC_Rx$(_T...) __gen__T_impl_thrd_SPSC_Rx$(_T)
#define T_use_thrd_SPSC_Rx$(_T...) __gen__T_use_thrd_SPSC_Rx$(_T)
#define thrd_SPSC_Chans$(_T...) Tup$$(thrd_SPSC_Tx$(_T), thrd_SPSC_Rx$(_T))

$extern fn_((thrd_SPSC_open(thrd_SPSC* self))(thrd_SPSC_Chans));
#define T_use_thrd_SPSC_open$(_T...) __gen__T_use_thrd_SPSC_open$(_T)
$extern fn_((thrd_SPSC_close(thrd_SPSC* self))(void));
#define T_use_thrd_SPSC_close$(_T...) __gen__T_use_thrd_SPSC_close$(_T)

$extern fn_((thrd_SPSC_tx(thrd_SPSC* self))(thrd_SPSC_Tx));
#define T_use_thrd_SPSC_tx$(_T...) __gen__T_use_thrd_SPSC_tx$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_trySend(thrd_SPSC_Tx self, u_V$raw item))(thrd_chan_E$void));
#define T_use_thrd_SPSC_Tx_trySend$(_T...) __gen__T_use_thrd_SPSC_Tx_trySend$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_send(thrd_SPSC_Tx self, u_V$raw item, thrd_Wakeable cancel_src))(thrd_chan_WaitE$void));
#define T_use_thrd_SPSC_Tx_send$(_T...) __gen__T_use_thrd_SPSC_Tx_send$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Tx_sendFor(thrd_SPSC_Tx self, u_V$raw item, thrd_Wakeable cancel_src, time_Dur dur))(thrd_chan_TimedE$void));
#define T_use_thrd_SPSC_Tx_sendFor$(_T...) __gen__T_use_thrd_SPSC_Tx_sendFor$(_T)
$extern fn_((thrd_SPSC_Tx_sendOp(thrd_SPSC_Tx self, u_P$raw item))(thrd_SPSC_Tx_SendOp));
#define T_use_thrd_SPSC_Tx_sendOp$(_T...) __gen__T_use_thrd_SPSC_Tx_sendOp$(_T)
$extern fn_((thrd_SPSC_Tx_SendOp_op(thrd_SPSC_Tx_SendOp* self))(thrd_Select_Op));
#define T_use_thrd_SPSC_Tx_SendOp_op$(_T...) __gen__T_use_thrd_SPSC_Tx_SendOp_op$(_T)

$extern fn_((thrd_SPSC_rx(thrd_SPSC* self))(thrd_SPSC_Rx));
#define T_use_thrd_SPSC_rx$(_T...) __gen__T_use_thrd_SPSC_rx$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_tryRecv(thrd_SPSC_Rx self, u_V$raw ret_mem))(thrd_chan_E$u_V$raw));
#define T_use_thrd_SPSC_Rx_tryRecv$(_T...) __gen__T_use_thrd_SPSC_Rx_tryRecv$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_recv(thrd_SPSC_Rx self, thrd_Wakeable cancel_src, u_V$raw ret_mem))(thrd_chan_WaitE$u_V$raw));
#define T_use_thrd_SPSC_Rx_recv$(_T...) __gen__T_use_thrd_SPSC_Rx_recv$(_T)
$attr($must_check)
$extern fn_((thrd_SPSC_Rx_recvFor(thrd_SPSC_Rx self, thrd_Wakeable cancel_src, time_Dur dur, u_V$raw ret_mem))(thrd_chan_TimedE$u_V$raw));
#define T_use_thrd_SPSC_Rx_recvFor$(_T...) __gen__T_use_thrd_SPSC_Rx_recvFor$(_T)
$extern fn_((thrd_SPSC_Rx_recvOp(thrd_SPSC_Rx self, TypeInfo type))(thrd_SPSC_Rx_RecvOp));
#define T_use_thrd_SPSC_Rx_recvOp$(_T...) __gen__T_use_thrd_SPSC_Rx_recvOp$(_T)
$extern fn_((thrd_SPSC_Rx_RecvOp_op(thrd_SPSC_Rx_RecvOp* self))(thrd_Select_Op));

/*========== Macros and Definitions =========================================*/

/* clang-format off */
#define __alias__thrd_SPSC$(_T...) tpl$(thrd_SPSC, _T)
#define __anon__thrd_SPSC$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(buf, S$$(_T)); \
        var_(head, atom_V$usize); \
        var_(tail, atom_V$usize); \
        var_(closed, atom_V$bool); \
        var_(wait_lock, thrd_Mtx); \
        var_(send_waiters, thrd_wait_Chain); \
        var_(recv_waiters, thrd_wait_Chain); \
        var_(type, debug_TypeInfo); \
    }); \
    var_(as_raw, thrd_SPSC) $like_ref; \
})
#define __gen__T_decl_thrd_SPSC$(_T...) $maybe_unused typedef union thrd_SPSC$(_T) thrd_SPSC$(_T)
#define __gen__T_impl_thrd_SPSC$(_T...) \
    union thrd_SPSC$(_T) { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, atom_V$usize); \
            var_(tail, atom_V$usize); \
            var_(closed, atom_V$bool); \
            var_(wait_lock, thrd_Mtx); \
            var_(send_waiters, thrd_wait_Chain); \
            var_(recv_waiters, thrd_wait_Chain); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, thrd_SPSC) $like_ref; \
    }
#define __gen__T_use_thrd_SPSC$(_T...) T_decl_thrd_SPSC$(_T); T_impl_thrd_SPSC$(_T)

#define __alias__thrd_SPSC_Tx$(_T...) tpl$(thrd_SPSC_Tx, _T)
#define __anon__thrd_SPSC_Tx$$(_T...) TypeOf(union { var_(chan, thrd_SPSC$(_T)*); var_(as_raw, thrd_SPSC_Tx) $like_ref; })
#define __gen__T_decl_thrd_SPSC_Tx$(_T...) $maybe_unused typedef union thrd_SPSC_Tx$(_T) thrd_SPSC_Tx$(_T)
#define __gen__T_impl_thrd_SPSC_Tx$(_T...) union thrd_SPSC_Tx$(_T) { var_(chan, thrd_SPSC$(_T)*); var_(as_raw, thrd_SPSC_Tx) $like_ref; }
#define __gen__T_use_thrd_SPSC_Tx$(_T...) T_decl_thrd_SPSC_Tx$(_T); T_impl_thrd_SPSC_Tx$(_T)

#define __alias__thrd_SPSC_Tx_SendOp$(_T...) tpl$(thrd_SPSC_Tx_SendOp, _T)
#define __anon__thrd_SPSC_Tx_SendOp$$(_T...) TypeOf(struct { \
    var_(tx, thrd_SPSC_Tx$(_T)); \
    var_(item, _T); \
})
#define __gen__T_decl_thrd_SPSC_Tx_SendOp$(_T...) $maybe_unused typedef struct thrd_SPSC_Tx_SendOp$(_T) thrd_SPSC_Tx_SendOp$(_T)
#define __gen__T_impl_thrd_SPSC_Tx_SendOp$(_T...) \
    struct thrd_SPSC_Tx_SendOp$(_T) { \
        var_(tx, thrd_SPSC_Tx$(_T)); \
        var_(item, _T); \
    }
#define __gen__T_use_thrd_SPSC_Tx_SendOp$(_T...) T_decl_thrd_SPSC_Tx_SendOp$(_T); T_impl_thrd_SPSC_Tx_SendOp$(_T)

#define __alias__thrd_SPSC_Rx$(_T...) tpl$(thrd_SPSC_Rx, _T)
#define __anon__thrd_SPSC_Rx$$(_T...) TypeOf(union { var_(chan, thrd_SPSC$(_T)*); var_(as_raw, thrd_SPSC_Rx) $like_ref; })
#define __gen__T_decl_thrd_SPSC_Rx$(_T...) \
    $maybe_unused typedef union thrd_SPSC_Rx$(_T) thrd_SPSC_Rx$(_T); \
    T_decl_E$($set(thrd_chan_E)(_T)); \
    T_decl_E$($set(thrd_chan_WaitE)(_T)); \
    T_decl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_impl_thrd_SPSC_Rx$(_T...) \
    union thrd_SPSC_Rx$(_T) { var_(chan, thrd_SPSC$(_T)*); var_(as_raw, thrd_SPSC_Rx) $like_ref; }; \
    T_impl_E$($set(thrd_chan_E)(_T)); \
    T_impl_E$($set(thrd_chan_WaitE)(_T)); \
    T_impl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_use_thrd_SPSC_Rx$(_T...) T_decl_thrd_SPSC_Rx$(_T); T_impl_thrd_SPSC_Rx$(_T)

#define __gen__T_use_thrd_SPSC_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_init, _T)(S$(_T) buf))(thrd_SPSC$(_T))) { \
        return type$((thrd_SPSC$(_T))(thrd_SPSC_init(u_anyS(buf)))); \
    }
#define __gen__T_use_thrd_SPSC_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_fini, _T)(P$$(thrd_SPSC$(_T)) self))(void)) { \
        return thrd_SPSC_fini(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_len, _T)(P$$(thrd_SPSC$(_T)) self))(usize)) { \
        return thrd_SPSC_len(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_cap, _T)(P$$(thrd_SPSC$(_T)) self))(usize)) { \
        return thrd_SPSC_cap(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_isFull$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_isFull, _T)(P$$(thrd_SPSC$(_T)) self))(bool)) { \
        return thrd_SPSC_isFull(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_isEmpty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_isEmpty, _T)(P$$(thrd_SPSC$(_T)) self))(bool)) { \
        return thrd_SPSC_isEmpty(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_isOpen$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_isOpen, _T)(P$$(thrd_SPSC$(_T)) self))(bool)) { \
        return thrd_SPSC_isOpen(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_isClosed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_isClosed, _T)(P$$(thrd_SPSC$(_T)) self))(bool)) { \
        return thrd_SPSC_isClosed(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_tx$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_tx, _T)(P$$(thrd_SPSC$(_T)) self))(thrd_SPSC_Tx$(_T))) { \
        return type$((thrd_SPSC_Tx$(_T))(thrd_SPSC_tx(self->as_raw))); \
    }
#define __gen__T_use_thrd_SPSC_rx$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_rx, _T)(P$$(thrd_SPSC$(_T)) self))(thrd_SPSC_Rx$(_T))) { \
        return type$((thrd_SPSC_Rx$(_T))(thrd_SPSC_rx(self->as_raw))); \
    }
#define __gen__T_use_thrd_SPSC_open$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_open, _T)(P$$(thrd_SPSC$(_T)) self))(thrd_SPSC_Chans$(_T))) { \
        return type$((thrd_SPSC_Chans$(_T))(thrd_SPSC_open(self->as_raw))); \
    }
#define __gen__T_use_thrd_SPSC_close$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_close, _T)(P$$(thrd_SPSC$(_T)) self))(void)) { \
        return thrd_SPSC_close(self->as_raw); \
    }
#define __gen__T_use_thrd_SPSC_Tx_trySend$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Tx_trySend, _T)(thrd_SPSC_Tx$(_T) self, _T item))(thrd_chan_E$void)) { \
        return thrd_SPSC_Tx_trySend(*self.as_raw, u_anyV(item)); \
    }
#define __gen__T_use_thrd_SPSC_Tx_send$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Tx_send, _T)(thrd_SPSC_Tx$(_T) self, _T item, thrd_Wakeable cancel_src))(thrd_chan_WaitE$void)) { \
        return thrd_SPSC_Tx_send(*self.as_raw, u_anyV(item), cancel_src); \
    }
#define __gen__T_use_thrd_SPSC_Tx_sendFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Tx_sendFor, _T)(thrd_SPSC_Tx$(_T) self, _T item, thrd_Wakeable cancel_src, time_Dur dur))(thrd_chan_TimedE$void)) { \
        return thrd_SPSC_Tx_sendFor(*self.as_raw, u_anyV(item), cancel_src, dur); \
    }
#define __gen__T_use_thrd_SPSC_Tx_sendOp$(_T...) \
    T_use_thrd_SPSC_Tx_SendOp$(_T); \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_Tx_sendOp, _T)(thrd_SPSC_Tx$(_T) self, _T item))(thrd_SPSC_Tx_SendOp$(_T))) { \
        return (thrd_SPSC_Tx_SendOp$(_T)){ .tx = self, .item = item }; \
    }
#define __gen__T_use_thrd_SPSC_Tx_SendOp_op$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_Tx_SendOp_op, _T)(P$$(thrd_SPSC_Tx_SendOp$(_T)) self))(thrd_Select_Op)) { \
        claim_assert_nonnull(self); \
        var raw = thrd_SPSC_Tx_sendOp(*self->tx.as_raw, u_anyP(&self->item)); \
        return thrd_SPSC_Tx_SendOp_op(&raw); \
    }
#define __gen__T_use_thrd_SPSC_Rx_tryRecv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Rx_tryRecv, _T)(thrd_SPSC_Rx$(_T) self))(E$($set(thrd_chan_E)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_SPSC_Rx_tryRecv(*self.as_raw, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_SPSC_Rx_recv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Rx_recv, _T)(thrd_SPSC_Rx$(_T) self, thrd_Wakeable cancel_src))(E$($set(thrd_chan_WaitE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_SPSC_Rx_recv(*self.as_raw, cancel_src, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_SPSC_Rx_recvFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_SPSC_Rx_recvFor, _T)(thrd_SPSC_Rx$(_T) self, thrd_Wakeable cancel_src, time_Dur dur))(E$($set(thrd_chan_TimedE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_SPSC_Rx_recvFor(*self.as_raw, cancel_src, dur, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_SPSC_Rx_recvOp$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_SPSC_Rx_recvOp, _T)(thrd_SPSC_Rx$(_T) self))(thrd_SPSC_Rx_RecvOp)) { \
        return thrd_SPSC_Rx_recvOp(*self.as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_SPSC__included */
