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

#include "chan.h"
#include "Select.h"

/*========== Macros and Declarations ========================================*/

/*
 * Progress note: this channel has no per-slot state. Multiple producers claim
 * ring positions and publish them in order. That prevents consumers from
 * reading unpublished slots, but a producer preempted after claim can make
 * later producers spin in publish until it resumes.
 */
typedef struct thrd_MPSC {
    var_(buf, S$raw);
    var_(head, atom_V$usize);
    var_(tail_claim, atom_V$usize);
    var_(tail_commit, atom_V$usize);
    var_(closed, atom_V$bool);
    var_(wait_lock, thrd_Mtx);
    var_(send_waiters, thrd_wait_List);
    var_(recv_waiters, thrd_wait_List);
    var_(type, debug_TypeInfo);
} thrd_MPSC;
$extern fn_((thrd_MPSC_init(u_S$raw buf))(thrd_MPSC));
#define T_use_thrd_MPSC_init$(_T...) __gen__T_use_thrd_MPSC_init$(_T)
$extern fn_((thrd_MPSC_fini(thrd_MPSC* self))(void));
#define T_use_thrd_MPSC_fini$(_T...) __gen__T_use_thrd_MPSC_fini$(_T)
$extern fn_((thrd_MPSC_len(thrd_MPSC* self))(usize));
#define T_use_thrd_MPSC_len$(_T...) __gen__T_use_thrd_MPSC_len$(_T)
$extern fn_((thrd_MPSC_cap(thrd_MPSC* self))(usize));
#define T_use_thrd_MPSC_cap$(_T...) __gen__T_use_thrd_MPSC_cap$(_T)
$extern fn_((thrd_MPSC_isFull(thrd_MPSC* self))(bool));
#define T_use_thrd_MPSC_isFull$(_T...) __gen__T_use_thrd_MPSC_isFull$(_T)
$extern fn_((thrd_MPSC_isEmpty(thrd_MPSC* self))(bool));
#define T_use_thrd_MPSC_isEmpty$(_T...) __gen__T_use_thrd_MPSC_isEmpty$(_T)
$extern fn_((thrd_MPSC_isOpen(thrd_MPSC* self))(bool));
#define T_use_thrd_MPSC_isOpen$(_T...) __gen__T_use_thrd_MPSC_isOpen$(_T)
$extern fn_((thrd_MPSC_isClosed(thrd_MPSC* self))(bool));
#define T_use_thrd_MPSC_isClosed$(_T...) __gen__T_use_thrd_MPSC_isClosed$(_T)

typedef struct thrd_MPSC_Tx {
    var_(chan, thrd_MPSC*);
} thrd_MPSC_Tx;
typedef struct thrd_MPSC_Tx_SendOp {
    var_(tx, thrd_MPSC_Tx);
    var_(item, u_P$raw);
} thrd_MPSC_Tx_SendOp;
typedef struct thrd_MPSC_Rx {
    var_(chan, thrd_MPSC*);
} thrd_MPSC_Rx;
typedef struct thrd_MPSC_Rx_RecvOp {
    var_(rx, thrd_MPSC_Rx);
    var_(type, debug_TypeInfo);
} thrd_MPSC_Rx_RecvOp;
typedef Tup$$(thrd_MPSC_Tx, thrd_MPSC_Rx) thrd_MPSC_Chans;

#define thrd_MPSC$(_T...) __alias__thrd_MPSC$(_T)
#define thrd_MPSC$$(_T...) __anon__thrd_MPSC$$(_T)
#define T_decl_thrd_MPSC$(_T...) __gen__T_decl_thrd_MPSC$(_T)
#define T_impl_thrd_MPSC$(_T...) __gen__T_impl_thrd_MPSC$(_T)
#define T_use_thrd_MPSC$(_T...) __gen__T_use_thrd_MPSC$(_T)
#define thrd_MPSC_Tx$(_T...) __alias__thrd_MPSC_Tx$(_T)
#define thrd_MPSC_Tx$$(_T...) __anon__thrd_MPSC_Tx$$(_T)
#define T_decl_thrd_MPSC_Tx$(_T...) __gen__T_decl_thrd_MPSC_Tx$(_T)
#define T_impl_thrd_MPSC_Tx$(_T...) __gen__T_impl_thrd_MPSC_Tx$(_T)
#define T_use_thrd_MPSC_Tx$(_T...) __gen__T_use_thrd_MPSC_Tx$(_T)
#define thrd_MPSC_Rx$(_T...) __alias__thrd_MPSC_Rx$(_T)
#define thrd_MPSC_Rx$$(_T...) __anon__thrd_MPSC_Rx$$(_T)
#define T_decl_thrd_MPSC_Rx$(_T...) __gen__T_decl_thrd_MPSC_Rx$(_T)
#define T_impl_thrd_MPSC_Rx$(_T...) __gen__T_impl_thrd_MPSC_Rx$(_T)
#define T_use_thrd_MPSC_Rx$(_T...) __gen__T_use_thrd_MPSC_Rx$(_T)
#define thrd_MPSC_Chans$(_T...) Tup$$(thrd_MPSC_Tx$(_T), thrd_MPSC_Rx$(_T))

$extern fn_((thrd_MPSC_open(thrd_MPSC* self))(thrd_MPSC_Chans));
#define T_use_thrd_MPSC_open$(_T...) __gen__T_use_thrd_MPSC_open$(_T)
$extern fn_((thrd_MPSC_close(thrd_MPSC* self))(void));
#define T_use_thrd_MPSC_close$(_T...) __gen__T_use_thrd_MPSC_close$(_T)

$extern fn_((thrd_MPSC_tx(thrd_MPSC* self))(thrd_MPSC_Tx));
#define T_use_thrd_MPSC_tx$(_T...) __gen__T_use_thrd_MPSC_tx$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_trySend(thrd_MPSC_Tx self, u_V$raw item))(thrd_chan_E$void));
#define T_use_thrd_MPSC_Tx_trySend$(_T...) __gen__T_use_thrd_MPSC_Tx_trySend$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_send(thrd_MPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void));
#define T_use_thrd_MPSC_Tx_send$(_T...) __gen__T_use_thrd_MPSC_Tx_send$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Tx_sendFor(thrd_MPSC_Tx self, u_V$raw item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void));
#define T_use_thrd_MPSC_Tx_sendFor$(_T...) __gen__T_use_thrd_MPSC_Tx_sendFor$(_T)
$extern fn_((thrd_MPSC_Tx_sendOp(thrd_MPSC_Tx self, u_P$raw item))(thrd_MPSC_Tx_SendOp));
#define T_use_thrd_MPSC_Tx_sendOp$(_T...) __gen__T_use_thrd_MPSC_Tx_sendOp$(_T)
$extern fn_((thrd_MPSC_Tx_SendOp_op(thrd_MPSC_Tx_SendOp* self))(thrd_Select_Op));

$extern fn_((thrd_MPSC_rx(thrd_MPSC* self))(thrd_MPSC_Rx));
#define T_use_thrd_MPSC_rx$(_T...) __gen__T_use_thrd_MPSC_rx$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_tryRecv(thrd_MPSC_Rx self, u_V$raw ret_mem))(thrd_chan_E$u_V$raw));
#define T_use_thrd_MPSC_Rx_tryRecv$(_T...) __gen__T_use_thrd_MPSC_Rx_tryRecv$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_recv(thrd_MPSC_Rx self, thrd_wait_Src cancel_src, u_V$raw ret_mem))(thrd_chan_WaitE$u_V$raw));
#define T_use_thrd_MPSC_Rx_recv$(_T...) __gen__T_use_thrd_MPSC_Rx_recv$(_T)
$attr($must_check)
$extern fn_((thrd_MPSC_Rx_recvFor(thrd_MPSC_Rx self, thrd_wait_Src cancel_src, time_Dur dur, u_V$raw ret_mem))(thrd_chan_TimedE$u_V$raw));
#define T_use_thrd_MPSC_Rx_recvFor$(_T...) __gen__T_use_thrd_MPSC_Rx_recvFor$(_T)
$extern fn_((thrd_MPSC_Rx_recvOp(thrd_MPSC_Rx self, TypeInfo type))(thrd_MPSC_Rx_RecvOp));
#define T_use_thrd_MPSC_Rx_recvOp$(_T...) __gen__T_use_thrd_MPSC_Rx_recvOp$(_T)
$extern fn_((thrd_MPSC_Rx_RecvOp_op(thrd_MPSC_Rx_RecvOp* self))(thrd_Select_Op));

/*========== Macros and Definitions =========================================*/

/* clang-format off */
#define __alias__thrd_MPSC$(_T...) tpl$(thrd_MPSC, _T)
#define __anon__thrd_MPSC$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(buf, S$$(_T)); \
        var_(head, atom_V$usize); \
        var_(tail_claim, atom_V$usize); \
        var_(tail_commit, atom_V$usize); \
        var_(closed, atom_V$bool); \
        var_(wait_lock, thrd_Mtx); \
        var_(send_waiters, thrd_wait_List); \
        var_(recv_waiters, thrd_wait_List); \
        var_(type, debug_TypeInfo); \
    }); \
    var_(as_raw, thrd_MPSC) $like_ref; \
})
#define __gen__T_decl_thrd_MPSC$(_T...) $maybe_unused typedef union thrd_MPSC$(_T) thrd_MPSC$(_T)
#define __gen__T_impl_thrd_MPSC$(_T...) \
    union thrd_MPSC$(_T) { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, atom_V$usize); \
            var_(tail_claim, atom_V$usize); \
            var_(tail_commit, atom_V$usize); \
            var_(closed, atom_V$bool); \
            var_(wait_lock, thrd_Mtx); \
            var_(send_waiters, thrd_wait_List); \
            var_(recv_waiters, thrd_wait_List); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, thrd_MPSC) $like_ref; \
    }
#define __gen__T_use_thrd_MPSC$(_T...) T_decl_thrd_MPSC$(_T); T_impl_thrd_MPSC$(_T)

#define __alias__thrd_MPSC_Tx$(_T...) tpl$(thrd_MPSC_Tx, _T)
#define __anon__thrd_MPSC_Tx$$(_T...) TypeOf(union { var_(chan, thrd_MPSC$(_T)*); var_(as_raw, thrd_MPSC_Tx) $like_ref; })
#define __gen__T_decl_thrd_MPSC_Tx$(_T...) $maybe_unused typedef union thrd_MPSC_Tx$(_T) thrd_MPSC_Tx$(_T)
#define __gen__T_impl_thrd_MPSC_Tx$(_T...) union thrd_MPSC_Tx$(_T) { var_(chan, thrd_MPSC$(_T)*); var_(as_raw, thrd_MPSC_Tx) $like_ref; }
#define __gen__T_use_thrd_MPSC_Tx$(_T...) T_decl_thrd_MPSC_Tx$(_T); T_impl_thrd_MPSC_Tx$(_T)

#define __alias__thrd_MPSC_Rx$(_T...) tpl$(thrd_MPSC_Rx, _T)
#define __anon__thrd_MPSC_Rx$$(_T...) TypeOf(union { var_(chan, thrd_MPSC$(_T)*); var_(as_raw, thrd_MPSC_Rx) $like_ref; })
#define __gen__T_decl_thrd_MPSC_Rx$(_T...) \
    $maybe_unused typedef union thrd_MPSC_Rx$(_T) thrd_MPSC_Rx$(_T); \
    T_decl_E$($set(thrd_chan_E)(_T)); \
    T_decl_E$($set(thrd_chan_WaitE)(_T)); \
    T_decl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_impl_thrd_MPSC_Rx$(_T...) \
    union thrd_MPSC_Rx$(_T) { var_(chan, thrd_MPSC$(_T)*); var_(as_raw, thrd_MPSC_Rx) $like_ref; }; \
    T_impl_E$($set(thrd_chan_E)(_T)); \
    T_impl_E$($set(thrd_chan_WaitE)(_T)); \
    T_impl_E$($set(thrd_chan_TimedE)(_T))
#define __gen__T_use_thrd_MPSC_Rx$(_T...) T_decl_thrd_MPSC_Rx$(_T); T_impl_thrd_MPSC_Rx$(_T)

#define __gen__T_use_thrd_MPSC_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_init, _T)(S$(_T) buf))(thrd_MPSC$(_T))) { \
        return type$((thrd_MPSC$(_T))(thrd_MPSC_init(u_anyS(buf)))); \
    }
#define __gen__T_use_thrd_MPSC_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_fini, _T)(P$$(thrd_MPSC$(_T)) self))(void)) { \
        return thrd_MPSC_fini(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_len, _T)(P$$(thrd_MPSC$(_T)) self))(usize)) { \
        return thrd_MPSC_len(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_cap, _T)(P$$(thrd_MPSC$(_T)) self))(usize)) { \
        return thrd_MPSC_cap(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_isFull$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_isFull, _T)(P$$(thrd_MPSC$(_T)) self))(bool)) { \
        return thrd_MPSC_isFull(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_isEmpty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_isEmpty, _T)(P$$(thrd_MPSC$(_T)) self))(bool)) { \
        return thrd_MPSC_isEmpty(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_isOpen$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_isOpen, _T)(P$$(thrd_MPSC$(_T)) self))(bool)) { \
        return thrd_MPSC_isOpen(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_isClosed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_isClosed, _T)(P$$(thrd_MPSC$(_T)) self))(bool)) { \
        return thrd_MPSC_isClosed(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_tx$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_tx, _T)(P$$(thrd_MPSC$(_T)) self))(thrd_MPSC_Tx$(_T))) { \
        return type$((thrd_MPSC_Tx$(_T))(thrd_MPSC_tx(self->as_raw))); \
    }
#define __gen__T_use_thrd_MPSC_rx$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_rx, _T)(P$$(thrd_MPSC$(_T)) self))(thrd_MPSC_Rx$(_T))) { \
        return type$((thrd_MPSC_Rx$(_T))(thrd_MPSC_rx(self->as_raw))); \
    }
#define __gen__T_use_thrd_MPSC_open$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_open, _T)(P$$(thrd_MPSC$(_T)) self))(thrd_MPSC_Chans$(_T))) { \
        return type$((thrd_MPSC_Chans$(_T))(thrd_MPSC_open(self->as_raw))); \
    }
#define __gen__T_use_thrd_MPSC_close$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_close, _T)(P$$(thrd_MPSC$(_T)) self))(void)) { \
        return thrd_MPSC_close(self->as_raw); \
    }
#define __gen__T_use_thrd_MPSC_Tx_trySend$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Tx_trySend, _T)(thrd_MPSC_Tx$(_T) self, _T item))(thrd_chan_E$void)) { \
        return thrd_MPSC_Tx_trySend(*self.as_raw, u_anyV(item)); \
    }
#define __gen__T_use_thrd_MPSC_Tx_send$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Tx_send, _T)(thrd_MPSC_Tx$(_T) self, _T item, thrd_wait_Src cancel_src))(thrd_chan_WaitE$void)) { \
        return thrd_MPSC_Tx_send(*self.as_raw, u_anyV(item), cancel_src); \
    }
#define __gen__T_use_thrd_MPSC_Tx_sendFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Tx_sendFor, _T)(thrd_MPSC_Tx$(_T) self, _T item, thrd_wait_Src cancel_src, time_Dur dur))(thrd_chan_TimedE$void)) { \
        return thrd_MPSC_Tx_sendFor(*self.as_raw, u_anyV(item), cancel_src, dur); \
    }
#define __gen__T_use_thrd_MPSC_Tx_sendOp$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_Tx_sendOp, _T)(thrd_MPSC_Tx$(_T) self, _T* item))(thrd_MPSC_Tx_SendOp)) { \
        return thrd_MPSC_Tx_sendOp(*self.as_raw, u_anyP(item)); \
    }
#define __gen__T_use_thrd_MPSC_Rx_recv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Rx_recv, _T)(thrd_MPSC_Rx$(_T) self, thrd_wait_Src cancel_src))(E$($set(thrd_chan_WaitE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_MPSC_Rx_recv(*self.as_raw, cancel_src, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_MPSC_Rx_tryRecv$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Rx_tryRecv, _T)(thrd_MPSC_Rx$(_T) self))(E$($set(thrd_chan_E)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_MPSC_Rx_tryRecv(*self.as_raw, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_MPSC_Rx_recvFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_MPSC_Rx_recvFor, _T)(thrd_MPSC_Rx$(_T) self, thrd_wait_Src cancel_src, time_Dur dur))(E$($set(thrd_chan_TimedE)(_T))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_MPSC_Rx_recvFor(*self.as_raw, cancel_src, dur, u_retV$(_T))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_MPSC_Rx_recvOp$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_MPSC_Rx_recvOp, _T)(thrd_MPSC_Rx$(_T) self))(thrd_MPSC_Rx_RecvOp)) { \
        return thrd_MPSC_Rx_recvOp(*self.as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_MPSC__included */
