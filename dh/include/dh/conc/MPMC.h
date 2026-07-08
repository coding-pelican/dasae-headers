/**
 * @file    conc/MPMC.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_MPMC
 */
#pragma once
#ifndef conc_MPMC__included
#define conc_MPMC__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

#define conc_MPMC$$(_T...) __comp_anon__conc_MPMC$$(_T)
#define conc_MPMC$(_T...) __comp_alias__conc_MPMC$(_T)
#define T_decl_conc_MPMC$(_T...) __comp_gen__T_decl_conc_MPMC$(_T)
#define T_impl_conc_MPMC$(_T...) __comp_gen__T_impl_conc_MPMC$(_T)
#define T_use_conc_MPMC$(_T...) __comp_gen__T_use_conc_MPMC$(_T)

#define conc_MPMC_Tx$$(_T...) __comp_anon__conc_MPMC_Tx$$(_T)
#define conc_MPMC_Tx$(_T...) __comp_alias__conc_MPMC_Tx$(_T)
#define T_decl_conc_MPMC_Tx$(_T...) __comp_gen__T_decl_conc_MPMC_Tx$(_T)
#define T_impl_conc_MPMC_Tx$(_T...) __comp_gen__T_impl_conc_MPMC_Tx$(_T)
#define T_use_conc_MPMC_Tx$(_T...) __comp_gen__T_use_conc_MPMC_Tx$(_T)

#define conc_MPMC_Rx$$(_T...) __comp_anon__conc_MPMC_Rx$$(_T)
#define conc_MPMC_Rx$(_T...) __comp_alias__conc_MPMC_Rx$(_T)
#define T_decl_conc_MPMC_Rx$(_T...) __comp_gen__T_decl_conc_MPMC_Rx$(_T)
#define T_impl_conc_MPMC_Rx$(_T...) __comp_gen__T_impl_conc_MPMC_Rx$(_T)
#define T_use_conc_MPMC_Rx$(_T...) __comp_gen__T_use_conc_MPMC_Rx$(_T)

T_alias$((conc_MPMC$raw)(struct conc_MPMC$raw));
T_alias$((conc_MPMC_Tx$raw)(struct conc_MPMC_Tx$raw));
T_alias$((conc_MPMC_Rx$raw)(struct conc_MPMC_Rx$raw));
T_use_atom_V$(usize);
T_use_atom_V$(bool);

struct conc_MPMC$raw {
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

struct conc_MPMC_Tx$raw { var_(chan, conc_MPMC$raw*); };
struct conc_MPMC_Rx$raw { var_(chan, conc_MPMC$raw*); };

$extern fn_((conc_MPMC_fixed(TypeInfo type, u_S$raw buf))(conc_MPMC$raw));
$extern fn_((conc_MPMC_split(conc_MPMC$raw* self, conc_MPMC_Tx$raw* tx, conc_MPMC_Rx$raw* rx))(void));
$extern fn_((conc_MPMC_close(conc_MPMC$raw* self))(void));
$extern fn_((conc_MPMC_isClosed(const conc_MPMC$raw* self))(bool));
$extern fn_((conc_MPMC_len(const conc_MPMC$raw* self))(usize));
$extern fn_((conc_MPMC_cap(const conc_MPMC$raw* self))(usize));
$extern fn_((conc_MPMC_Tx_clone(conc_MPMC_Tx$raw self))(conc_MPMC_Tx$raw));
$extern fn_((conc_MPMC_Tx_drop(conc_MPMC_Tx$raw* self))(void));
$extern fn_((conc_MPMC_Rx_clone(conc_MPMC_Rx$raw self))(conc_MPMC_Rx$raw));
$extern fn_((conc_MPMC_Rx_drop(conc_MPMC_Rx$raw* self))(void));
$attr($must_check)
$extern fn_((conc_MPMC_Tx_trySend(conc_MPMC_Tx$raw* self, TypeInfo type, u_V$raw item))(E$($set(conc_E)(void))));
$attr($must_check)
$extern fn_((conc_MPMC_Tx_send(conc_MPMC_Tx$raw* self, TypeInfo type, Sched sched, u_V$raw item))(E$($set(conc_WaitE)(void))));
$extern fn_((conc_MPMC_Rx_tryRecv(conc_MPMC_Rx$raw* self, TypeInfo type, V$raw ret_mem))(E$($set(conc_E)(V$raw))));
$attr($must_check)
$extern fn_((conc_MPMC_Rx_recv(conc_MPMC_Rx$raw* self, TypeInfo type, Sched sched, V$raw ret_mem))(E$($set(conc_WaitE)(V$raw))));
$attr($must_check)
$extern fn_((conc_MPMC_Rx_recvFor(conc_MPMC_Rx$raw* self, TypeInfo type, Sched sched, time_Dur dur, V$raw ret_mem))(E$($set(conc_TimedE)(V$raw))));
$attr($must_check)
$extern fn_((conc_MPMC_Rx_recvUntil(conc_MPMC_Rx$raw* self, TypeInfo type, Sched sched, time_Inst deadline, V$raw ret_mem))(E$($set(conc_TimedE)(V$raw))));
$extern fn_((conc_MPMC_Rx_asAwaitSrc(conc_MPMC_Rx$raw* self, TypeInfo type))(conc_AwaitSrc));

#define __comp_anon__conc_MPMC$$(_T...) \
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
        var_(as_raw, conc_MPMC$raw) $like_ref; \
    }
#define __comp_alias__conc_MPMC$(_T...) tpl$(conc_MPMC, _T)
#define __comp_gen__T_decl_conc_MPMC$(_T...) \
    $maybe_unused typedef union conc_MPMC$(_T) conc_MPMC$(_T)
#define __comp_gen__T_impl_conc_MPMC$(_T...) \
    union conc_MPMC$(_T) { \
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
        var_(as_raw, conc_MPMC$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_MPMC$(_T...) \
    T_decl_conc_MPMC$(_T); \
    T_impl_conc_MPMC$(_T)

#define __comp_anon__conc_MPMC_Tx$$(_T...) union { var_(chan, conc_MPMC$(_T)*); var_(as_raw, conc_MPMC_Tx$raw) $like_ref; }
#define __comp_alias__conc_MPMC_Tx$(_T...) tpl$(conc_MPMC_Tx, _T)
#define __comp_gen__T_decl_conc_MPMC_Tx$(_T...) $maybe_unused typedef union conc_MPMC_Tx$(_T) conc_MPMC_Tx$(_T)
#define __comp_gen__T_impl_conc_MPMC_Tx$(_T...) union conc_MPMC_Tx$(_T) { var_(chan, conc_MPMC$(_T)*); var_(as_raw, conc_MPMC_Tx$raw) $like_ref; }
#define __comp_gen__T_use_conc_MPMC_Tx$(_T...) T_decl_conc_MPMC_Tx$(_T); T_impl_conc_MPMC_Tx$(_T)

#define __comp_anon__conc_MPMC_Rx$$(_T...) union { var_(chan, conc_MPMC$(_T)*); var_(as_raw, conc_MPMC_Rx$raw) $like_ref; }
#define __comp_alias__conc_MPMC_Rx$(_T...) tpl$(conc_MPMC_Rx, _T)
#define __comp_gen__T_decl_conc_MPMC_Rx$(_T...) $maybe_unused typedef union conc_MPMC_Rx$(_T) conc_MPMC_Rx$(_T)
#define __comp_gen__T_impl_conc_MPMC_Rx$(_T...) union conc_MPMC_Rx$(_T) { var_(chan, conc_MPMC$(_T)*); var_(as_raw, conc_MPMC_Rx$raw) $like_ref; }
#define __comp_gen__T_use_conc_MPMC_Rx$(_T...) T_decl_conc_MPMC_Rx$(_T); T_impl_conc_MPMC_Rx$(_T)

#define T_use_conc_MPMC_all$(_T...) \
    T_use_conc_MPMC$(_T); \
    T_use_conc_MPMC_Tx$(_T); \
    T_use_conc_MPMC_Rx$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_MPMC__included */
