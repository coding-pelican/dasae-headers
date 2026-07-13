/**
 * @file    conc/MPSC.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_MPSC
 */
#pragma once
#ifndef conc_MPSC__included
#define conc_MPSC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

#define conc_MPSC$$(_T...) __comp_anon__conc_MPSC$$(_T)
#define conc_MPSC$(_T...) __comp_alias__conc_MPSC$(_T)
#define T_decl_conc_MPSC$(_T...) __comp_gen__T_decl_conc_MPSC$(_T)
#define T_impl_conc_MPSC$(_T...) __comp_gen__T_impl_conc_MPSC$(_T)
#define T_use_conc_MPSC$(_T...) __comp_gen__T_use_conc_MPSC$(_T)

#define conc_MPSC_Tx$$(_T...) __comp_anon__conc_MPSC_Tx$$(_T)
#define conc_MPSC_Tx$(_T...) __comp_alias__conc_MPSC_Tx$(_T)
#define T_decl_conc_MPSC_Tx$(_T...) __comp_gen__T_decl_conc_MPSC_Tx$(_T)
#define T_impl_conc_MPSC_Tx$(_T...) __comp_gen__T_impl_conc_MPSC_Tx$(_T)
#define T_use_conc_MPSC_Tx$(_T...) __comp_gen__T_use_conc_MPSC_Tx$(_T)

#define conc_MPSC_Rx$$(_T...) __comp_anon__conc_MPSC_Rx$$(_T)
#define conc_MPSC_Rx$(_T...) __comp_alias__conc_MPSC_Rx$(_T)
#define T_decl_conc_MPSC_Rx$(_T...) __comp_gen__T_decl_conc_MPSC_Rx$(_T)
#define T_impl_conc_MPSC_Rx$(_T...) __comp_gen__T_impl_conc_MPSC_Rx$(_T)
#define T_use_conc_MPSC_Rx$(_T...) __comp_gen__T_use_conc_MPSC_Rx$(_T)

T_alias$((conc_MPSC$raw)(struct conc_MPSC$raw));
T_alias$((conc_MPSC_Tx$raw)(struct conc_MPSC_Tx$raw));
T_alias$((conc_MPSC_Rx$raw)(struct conc_MPSC_Rx$raw));

struct conc_MPSC$raw {
    var_(buf, S$raw);
    var_(head, atom_V$usize);
    var_(tail, atom_V$usize);
    var_(tx_count, atom_V$usize);
    var_(rx_count, atom_V$usize);
    var_(closed, atom_V$bool);
    var_(lock, atom_SpinLock);
    var_(send_waiters, O$P$conc_AwaitLink);
    var_(recv_waiters, O$P$conc_AwaitLink);
    var_(type, debug_TypeInfo);
};

struct conc_MPSC_Tx$raw {
    var_(chan, conc_MPSC$raw*);
};
struct conc_MPSC_Rx$raw {
    var_(chan, conc_MPSC$raw*);
};

$extern fn_((conc_MPSC_fixed(TypeInfo type, u_S$raw buf))(conc_MPSC$raw));
$extern fn_((conc_MPSC_split(conc_MPSC$raw* self, conc_MPSC_Tx$raw* tx, conc_MPSC_Rx$raw* rx))(void));
$extern fn_((conc_MPSC_close(conc_MPSC$raw* self))(void));
$extern fn_((conc_MPSC_isClosed(const conc_MPSC$raw* self))(bool));
$extern fn_((conc_MPSC_len(const conc_MPSC$raw* self))(usize));
$extern fn_((conc_MPSC_cap(const conc_MPSC$raw* self))(usize));
$extern fn_((conc_MPSC_Tx_clone(conc_MPSC_Tx$raw self))(conc_MPSC_Tx$raw));
$extern fn_((conc_MPSC_Tx_drop(conc_MPSC_Tx$raw* self))(void));
$extern fn_((conc_MPSC_Rx_drop(conc_MPSC_Rx$raw* self))(void));
$attr($must_check)
$extern fn_((conc_MPSC_Tx_trySend(conc_MPSC_Tx$raw* self, TypeInfo type, u_V$raw item))(conc_chan_E$void));
$attr($must_check)
$extern fn_((conc_MPSC_Tx_send(conc_MPSC_Tx$raw* self, TypeInfo type, Sched sched, u_V$raw item))(conc_chan_WaitE$void));
$extern fn_((conc_MPSC_Rx_tryRecv(conc_MPSC_Rx$raw* self, TypeInfo type, u_V$raw ret_mem))(conc_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((conc_MPSC_Rx_recv(conc_MPSC_Rx$raw* self, TypeInfo type, Sched sched, u_V$raw ret_mem))(conc_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((conc_MPSC_Rx_recvFor(conc_MPSC_Rx$raw* self, TypeInfo type, Sched sched, time_Dur dur, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$attr($must_check)
$extern fn_((conc_MPSC_Rx_recvUntil(conc_MPSC_Rx$raw* self, TypeInfo type, Sched sched, time_Inst deadline, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$extern fn_((conc_MPSC_Rx_asAwakeable(conc_MPSC_Rx$raw* self, TypeInfo type))(conc_Awakeable));

#define __comp_anon__conc_MPSC$$(_T...) \
    union { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, atom_V$usize); \
            var_(tail, atom_V$usize); \
            var_(tx_count, atom_V$usize); \
            var_(rx_count, atom_V$usize); \
            var_(closed, atom_V$bool); \
            var_(lock, atom_SpinLock); \
            var_(send_waiters, O$P$conc_AwaitLink); \
            var_(recv_waiters, O$P$conc_AwaitLink); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_MPSC$raw) $like_ref; \
    }
#define __comp_alias__conc_MPSC$(_T...) tpl$(conc_MPSC, _T)
#define __comp_gen__T_decl_conc_MPSC$(_T...) \
    $maybe_unused typedef union conc_MPSC$(_T) conc_MPSC$(_T)
#define __comp_gen__T_impl_conc_MPSC$(_T...) \
    union conc_MPSC$(_T) { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, atom_V$usize); \
            var_(tail, atom_V$usize); \
            var_(tx_count, atom_V$usize); \
            var_(rx_count, atom_V$usize); \
            var_(closed, atom_V$bool); \
            var_(lock, atom_SpinLock); \
            var_(send_waiters, O$P$conc_AwaitLink); \
            var_(recv_waiters, O$P$conc_AwaitLink); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_MPSC$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_MPSC$(_T...) \
    T_decl_conc_MPSC$(_T); \
    T_impl_conc_MPSC$(_T)

#define __comp_anon__conc_MPSC_Tx$$(_T...) \
    union { \
        var_(chan, conc_MPSC$(_T) *); \
        var_(as_raw, conc_MPSC_Tx$raw) $like_ref; \
    }
#define __comp_alias__conc_MPSC_Tx$(_T...) tpl$(conc_MPSC_Tx, _T)
#define __comp_gen__T_decl_conc_MPSC_Tx$(_T...) $maybe_unused typedef union conc_MPSC_Tx$(_T) conc_MPSC_Tx$(_T)
#define __comp_gen__T_impl_conc_MPSC_Tx$(_T...) \
    union conc_MPSC_Tx$(_T) { \
        var_(chan, conc_MPSC$(_T) *); \
        var_(as_raw, conc_MPSC_Tx$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_MPSC_Tx$(_T...) \
    T_decl_conc_MPSC_Tx$(_T); \
    T_impl_conc_MPSC_Tx$(_T)

#define __comp_anon__conc_MPSC_Rx$$(_T...) \
    union { \
        var_(chan, conc_MPSC$(_T) *); \
        var_(as_raw, conc_MPSC_Rx$raw) $like_ref; \
    }
#define __comp_alias__conc_MPSC_Rx$(_T...) tpl$(conc_MPSC_Rx, _T)
#define __comp_gen__T_decl_conc_MPSC_Rx$(_T...) $maybe_unused typedef union conc_MPSC_Rx$(_T) conc_MPSC_Rx$(_T)
#define __comp_gen__T_impl_conc_MPSC_Rx$(_T...) \
    union conc_MPSC_Rx$(_T) { \
        var_(chan, conc_MPSC$(_T) *); \
        var_(as_raw, conc_MPSC_Rx$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_MPSC_Rx$(_T...) \
    T_decl_conc_MPSC_Rx$(_T); \
    T_impl_conc_MPSC_Rx$(_T)

#define T_use_conc_MPSC_all$(_T...) \
    T_use_conc_MPSC$(_T); \
    T_use_conc_MPSC_Tx$(_T); \
    T_use_conc_MPSC_Rx$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_MPSC__included */
