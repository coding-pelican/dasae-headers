/**
 * @file    conc/SPMC.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_SPMC
 */
#pragma once
#ifndef conc_SPMC__included
#define conc_SPMC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

#define conc_SPMC$$(_T...) __comp_anon__conc_SPMC$$(_T)
#define conc_SPMC$(_T...) __comp_alias__conc_SPMC$(_T)
#define T_decl_conc_SPMC$(_T...) __comp_gen__T_decl_conc_SPMC$(_T)
#define T_impl_conc_SPMC$(_T...) __comp_gen__T_impl_conc_SPMC$(_T)
#define T_use_conc_SPMC$(_T...) __comp_gen__T_use_conc_SPMC$(_T)

#define conc_SPMC_Tx$$(_T...) __comp_anon__conc_SPMC_Tx$$(_T)
#define conc_SPMC_Tx$(_T...) __comp_alias__conc_SPMC_Tx$(_T)
#define T_decl_conc_SPMC_Tx$(_T...) __comp_gen__T_decl_conc_SPMC_Tx$(_T)
#define T_impl_conc_SPMC_Tx$(_T...) __comp_gen__T_impl_conc_SPMC_Tx$(_T)
#define T_use_conc_SPMC_Tx$(_T...) __comp_gen__T_use_conc_SPMC_Tx$(_T)

#define conc_SPMC_Rx$$(_T...) __comp_anon__conc_SPMC_Rx$$(_T)
#define conc_SPMC_Rx$(_T...) __comp_alias__conc_SPMC_Rx$(_T)
#define T_decl_conc_SPMC_Rx$(_T...) __comp_gen__T_decl_conc_SPMC_Rx$(_T)
#define T_impl_conc_SPMC_Rx$(_T...) __comp_gen__T_impl_conc_SPMC_Rx$(_T)
#define T_use_conc_SPMC_Rx$(_T...) __comp_gen__T_use_conc_SPMC_Rx$(_T)

T_alias$((conc_SPMC$raw)(struct conc_SPMC$raw));
T_alias$((conc_SPMC_Tx$raw)(struct conc_SPMC_Tx$raw));
T_alias$((conc_SPMC_Rx$raw)(struct conc_SPMC_Rx$raw));

struct conc_SPMC$raw {
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

struct conc_SPMC_Tx$raw {
    var_(chan, conc_SPMC$raw*);
};
struct conc_SPMC_Rx$raw {
    var_(chan, conc_SPMC$raw*);
};

$extern fn_((conc_SPMC_fixed(TypeInfo type, u_S$raw buf))(conc_SPMC$raw));
$extern fn_((conc_SPMC_split(conc_SPMC$raw* self, conc_SPMC_Tx$raw* tx, conc_SPMC_Rx$raw* rx))(void));
$extern fn_((conc_SPMC_close(conc_SPMC$raw* self))(void));
$extern fn_((conc_SPMC_isClosed(const conc_SPMC$raw* self))(bool));
$extern fn_((conc_SPMC_len(const conc_SPMC$raw* self))(usize));
$extern fn_((conc_SPMC_cap(const conc_SPMC$raw* self))(usize));
$extern fn_((conc_SPMC_Tx_drop(conc_SPMC_Tx$raw* self))(void));
$extern fn_((conc_SPMC_Rx_clone(conc_SPMC_Rx$raw self))(conc_SPMC_Rx$raw));
$extern fn_((conc_SPMC_Rx_drop(conc_SPMC_Rx$raw* self))(void));
$attr($must_check)
$extern fn_((conc_SPMC_Tx_trySend(conc_SPMC_Tx$raw* self, TypeInfo type, u_V$raw item))(conc_chan_E$void));
$attr($must_check)
$extern fn_((conc_SPMC_Tx_send(conc_SPMC_Tx$raw* self, TypeInfo type, Sched sched, u_V$raw item))(conc_chan_WaitE$void));
$extern fn_((conc_SPMC_Rx_tryRecv(conc_SPMC_Rx$raw* self, TypeInfo type, u_V$raw ret_mem))(conc_chan_E$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPMC_Rx_recv(conc_SPMC_Rx$raw* self, TypeInfo type, Sched sched, u_V$raw ret_mem))(conc_chan_WaitE$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPMC_Rx_recvFor(conc_SPMC_Rx$raw* self, TypeInfo type, Sched sched, time_Dur dur, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$attr($must_check)
$extern fn_((conc_SPMC_Rx_recvUntil(conc_SPMC_Rx$raw* self, TypeInfo type, Sched sched, time_Inst deadline, u_V$raw ret_mem))(conc_chan_TimedE$u_V$raw));
$extern fn_((conc_SPMC_Rx_asAwakeable(conc_SPMC_Rx$raw* self, TypeInfo type))(conc_Awakeable));

#define __comp_anon__conc_SPMC$$(_T...) \
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
        var_(as_raw, conc_SPMC$raw) $like_ref; \
    }
#define __comp_alias__conc_SPMC$(_T...) tpl$(conc_SPMC, _T)
#define __comp_gen__T_decl_conc_SPMC$(_T...) \
    $maybe_unused typedef union conc_SPMC$(_T) conc_SPMC$(_T)
#define __comp_gen__T_impl_conc_SPMC$(_T...) \
    union conc_SPMC$(_T) { \
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
        var_(as_raw, conc_SPMC$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_SPMC$(_T...) \
    T_decl_conc_SPMC$(_T); \
    T_impl_conc_SPMC$(_T)

#define __comp_anon__conc_SPMC_Tx$$(_T...) \
    union { \
        var_(chan, conc_SPMC$(_T) *); \
        var_(as_raw, conc_SPMC_Tx$raw) $like_ref; \
    }
#define __comp_alias__conc_SPMC_Tx$(_T...) tpl$(conc_SPMC_Tx, _T)
#define __comp_gen__T_decl_conc_SPMC_Tx$(_T...) $maybe_unused typedef union conc_SPMC_Tx$(_T) conc_SPMC_Tx$(_T)
#define __comp_gen__T_impl_conc_SPMC_Tx$(_T...) \
    union conc_SPMC_Tx$(_T) { \
        var_(chan, conc_SPMC$(_T) *); \
        var_(as_raw, conc_SPMC_Tx$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_SPMC_Tx$(_T...) \
    T_decl_conc_SPMC_Tx$(_T); \
    T_impl_conc_SPMC_Tx$(_T)

#define __comp_anon__conc_SPMC_Rx$$(_T...) \
    union { \
        var_(chan, conc_SPMC$(_T) *); \
        var_(as_raw, conc_SPMC_Rx$raw) $like_ref; \
    }
#define __comp_alias__conc_SPMC_Rx$(_T...) tpl$(conc_SPMC_Rx, _T)
#define __comp_gen__T_decl_conc_SPMC_Rx$(_T...) $maybe_unused typedef union conc_SPMC_Rx$(_T) conc_SPMC_Rx$(_T)
#define __comp_gen__T_impl_conc_SPMC_Rx$(_T...) \
    union conc_SPMC_Rx$(_T) { \
        var_(chan, conc_SPMC$(_T) *); \
        var_(as_raw, conc_SPMC_Rx$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_SPMC_Rx$(_T...) \
    T_decl_conc_SPMC_Rx$(_T); \
    T_impl_conc_SPMC_Rx$(_T)

#define T_use_conc_SPMC_all$(_T...) \
    T_use_conc_SPMC$(_T); \
    T_use_conc_SPMC_Tx$(_T); \
    T_use_conc_SPMC_Rx$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_SPMC__included */
