/**
 * @file    conc/SPSC.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_SPSC
 */
#pragma once
#ifndef conc_SPSC__included
#define conc_SPSC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

#define conc_SPSC$$(_T...) __comp_anon__conc_SPSC$$(_T)
#define conc_SPSC$(_T...) __comp_alias__conc_SPSC$(_T)
#define T_decl_conc_SPSC$(_T...) __comp_gen__T_decl_conc_SPSC$(_T)
#define T_impl_conc_SPSC$(_T...) __comp_gen__T_impl_conc_SPSC$(_T)
#define T_use_conc_SPSC$(_T...) __comp_gen__T_use_conc_SPSC$(_T)

#define conc_SPSC_Tx$$(_T...) __comp_anon__conc_SPSC_Tx$$(_T)
#define conc_SPSC_Tx$(_T...) __comp_alias__conc_SPSC_Tx$(_T)
#define T_decl_conc_SPSC_Tx$(_T...) __comp_gen__T_decl_conc_SPSC_Tx$(_T)
#define T_impl_conc_SPSC_Tx$(_T...) __comp_gen__T_impl_conc_SPSC_Tx$(_T)
#define T_use_conc_SPSC_Tx$(_T...) __comp_gen__T_use_conc_SPSC_Tx$(_T)

#define conc_SPSC_Rx$$(_T...) __comp_anon__conc_SPSC_Rx$$(_T)
#define conc_SPSC_Rx$(_T...) __comp_alias__conc_SPSC_Rx$(_T)
#define T_decl_conc_SPSC_Rx$(_T...) __comp_gen__T_decl_conc_SPSC_Rx$(_T)
#define T_impl_conc_SPSC_Rx$(_T...) __comp_gen__T_impl_conc_SPSC_Rx$(_T)
#define T_use_conc_SPSC_Rx$(_T...) __comp_gen__T_use_conc_SPSC_Rx$(_T)

T_alias$((conc_SPSC$raw)(struct conc_SPSC$raw));
T_alias$((conc_SPSC_Tx$raw)(struct conc_SPSC_Tx$raw));
T_alias$((conc_SPSC_Rx$raw)(struct conc_SPSC_Rx$raw));

struct conc_SPSC$raw {
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

struct conc_SPSC_Tx$raw { var_(chan, conc_SPSC$raw*); };
struct conc_SPSC_Rx$raw { var_(chan, conc_SPSC$raw*); };

$extern fn_((conc_SPSC_fixed(TypeInfo type, u_S$raw buf))(conc_SPSC$raw));
$extern fn_((conc_SPSC_split(conc_SPSC$raw* self, conc_SPSC_Tx$raw* tx, conc_SPSC_Rx$raw* rx))(void));
$extern fn_((conc_SPSC_close(conc_SPSC$raw* self))(void));
$extern fn_((conc_SPSC_isClosed(const conc_SPSC$raw* self))(bool));
$extern fn_((conc_SPSC_len(const conc_SPSC$raw* self))(usize));
$extern fn_((conc_SPSC_cap(const conc_SPSC$raw* self))(usize));
$extern fn_((conc_SPSC_Tx_drop(conc_SPSC_Tx$raw* self))(void));
$extern fn_((conc_SPSC_Rx_drop(conc_SPSC_Rx$raw* self))(void));
$attr($must_check)
$extern fn_((conc_SPSC_Tx_trySend(conc_SPSC_Tx$raw* self, TypeInfo type, u_V$raw item))(conc_chan_E$void));
$attr($must_check)
$extern fn_((conc_SPSC_Tx_send(conc_SPSC_Tx$raw* self, TypeInfo type, Sched sched, u_V$raw item))(conc_chan_WaitE$void));
$extern fn_((conc_SPSC_Rx_tryRecv(conc_SPSC_Rx$raw* self, TypeInfo type, u_V$raw ret_mem))(conc_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPSC_Rx_recv(conc_SPSC_Rx$raw* self, TypeInfo type, Sched sched, u_V$raw ret_mem))(conc_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPSC_Rx_recvFor(conc_SPSC_Rx$raw* self, TypeInfo type, Sched sched, time_Dur dur, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPSC_Rx_recvUntil(conc_SPSC_Rx$raw* self, TypeInfo type, Sched sched, time_Inst deadline, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$extern fn_((conc_SPSC_Rx_asAwaitSrc(conc_SPSC_Rx$raw* self, TypeInfo type))(conc_AwaitSrc));

#define __comp_anon__conc_SPSC$$(_T...) \
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
        var_(as_raw, conc_SPSC$raw) $like_ref; \
    }
#define __comp_alias__conc_SPSC$(_T...) tpl$(conc_SPSC, _T)
#define __comp_gen__T_decl_conc_SPSC$(_T...) \
    $maybe_unused typedef union conc_SPSC$(_T) conc_SPSC$(_T)
#define __comp_gen__T_impl_conc_SPSC$(_T...) \
    union conc_SPSC$(_T) { \
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
        var_(as_raw, conc_SPSC$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_SPSC$(_T...) \
    T_decl_conc_SPSC$(_T); \
    T_impl_conc_SPSC$(_T)

#define __comp_anon__conc_SPSC_Tx$$(_T...) union { var_(chan, conc_SPSC$(_T)*); var_(as_raw, conc_SPSC_Tx$raw) $like_ref; }
#define __comp_alias__conc_SPSC_Tx$(_T...) tpl$(conc_SPSC_Tx, _T)
#define __comp_gen__T_decl_conc_SPSC_Tx$(_T...) $maybe_unused typedef union conc_SPSC_Tx$(_T) conc_SPSC_Tx$(_T)
#define __comp_gen__T_impl_conc_SPSC_Tx$(_T...) union conc_SPSC_Tx$(_T) { var_(chan, conc_SPSC$(_T)*); var_(as_raw, conc_SPSC_Tx$raw) $like_ref; }
#define __comp_gen__T_use_conc_SPSC_Tx$(_T...) T_decl_conc_SPSC_Tx$(_T); T_impl_conc_SPSC_Tx$(_T)

#define __comp_anon__conc_SPSC_Rx$$(_T...) union { var_(chan, conc_SPSC$(_T)*); var_(as_raw, conc_SPSC_Rx$raw) $like_ref; }
#define __comp_alias__conc_SPSC_Rx$(_T...) tpl$(conc_SPSC_Rx, _T)
#define __comp_gen__T_decl_conc_SPSC_Rx$(_T...) $maybe_unused typedef union conc_SPSC_Rx$(_T) conc_SPSC_Rx$(_T)
#define __comp_gen__T_impl_conc_SPSC_Rx$(_T...) union conc_SPSC_Rx$(_T) { var_(chan, conc_SPSC$(_T)*); var_(as_raw, conc_SPSC_Rx$raw) $like_ref; }
#define __comp_gen__T_use_conc_SPSC_Rx$(_T...) T_decl_conc_SPSC_Rx$(_T); T_impl_conc_SPSC_Rx$(_T)

#define T_use_conc_SPSC_all$(_T...) \
    T_use_conc_SPSC$(_T); \
    T_use_conc_SPSC_Tx$(_T); \
    T_use_conc_SPSC_Rx$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_SPSC__included */
