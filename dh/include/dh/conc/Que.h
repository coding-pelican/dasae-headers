/**
 * @file    conc/Que.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Que
 */
#pragma once
#ifndef conc_Que__included
#define conc_Que__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

#define conc_Que$$(_T...) __comp_anon__conc_Que$$(_T)
#define conc_Que$(_T...) __comp_alias__conc_Que$(_T)
#define T_decl_conc_Que$(_T...) __comp_gen__T_decl_conc_Que$(_T)
#define T_impl_conc_Que$(_T...) __comp_gen__T_impl_conc_Que$(_T)
#define T_use_conc_Que$(_T...) __comp_gen__T_use_conc_Que$(_T)

T_alias$((conc_Que$raw)(struct conc_Que$raw));

struct conc_Que$raw {
    var_(buf, S$raw);
    var_(head, usize);
    var_(len, usize);
    var_(closed, bool);
    var_(lock, atom_SpinLock);
    var_(send_waiters, O$P$conc_AwaitLink);
    var_(recv_waiters, O$P$conc_AwaitLink);
    var_(type, debug_TypeInfo);
};

$extern fn_((conc_Que_fixed(TypeInfo type, u_S$raw buf))(conc_Que$raw));
$extern fn_((conc_Que_len(const conc_Que$raw* self))(usize));
$extern fn_((conc_Que_cap(const conc_Que$raw* self))(usize));
$extern fn_((conc_Que_close(conc_Que$raw* self))(void));
$extern fn_((conc_Que_isClosed(const conc_Que$raw* self))(bool));
$attr($must_check)
$extern fn_((conc_Que_trySend(conc_Que$raw* self, TypeInfo type, u_V$raw item))(E$($set(conc_E)(void))));
$extern fn_((conc_Que_tryRecv(conc_Que$raw* self, TypeInfo type, V$raw ret_mem))(E$($set(conc_E)(V$raw))));
$attr($must_check)
$extern fn_((conc_Que_send(conc_Que$raw* self, TypeInfo type, Sched sched, u_V$raw item))(E$($set(conc_WaitE)(void))));
$attr($must_check)
$extern fn_((conc_Que_recv(conc_Que$raw* self, TypeInfo type, Sched sched, V$raw ret_mem))(E$($set(conc_WaitE)(V$raw))));
$extern fn_((conc_Que_recvSrc(conc_Que$raw* self, TypeInfo type))(conc_AwaitSrc));

#define __comp_anon__conc_Que$$(_T...) \
    union { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            var_(closed, bool); \
            var_(lock, atom_SpinLock); \
            var_(send_waiters, O$P$conc_AwaitLink); \
            var_(recv_waiters, O$P$conc_AwaitLink); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Que$raw) $like_ref; \
    }
#define __comp_alias__conc_Que$(_T...) tpl$(conc_Que, _T)
#define __comp_gen__T_decl_conc_Que$(_T...) \
    $maybe_unused typedef union conc_Que$(_T) conc_Que$(_T)
#define __comp_gen__T_impl_conc_Que$(_T...) \
    union conc_Que$(_T) { \
        T_embed$(struct { \
            var_(buf, S$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            var_(closed, bool); \
            var_(lock, atom_SpinLock); \
            var_(send_waiters, O$P$conc_AwaitLink); \
            var_(recv_waiters, O$P$conc_AwaitLink); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Que$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_Que$(_T...) \
    T_decl_conc_Que$(_T); \
    T_impl_conc_Que$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_Que__included */
