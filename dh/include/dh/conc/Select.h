/**
 * @file    conc/Select.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Select
 */
#pragma once
#ifndef conc_Select__included
#define conc_Select__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"
#include "../Future.h"

#define conc_Select$$(_U...) __comp_anon__conc_Select$$(_U)
#define conc_Select$(_U...) __comp_alias__conc_Select$(_U)
#define T_decl_conc_Select$(_U...) __comp_gen__T_decl_conc_Select$(_U)
#define T_impl_conc_Select$(_U...) __comp_gen__T_impl_conc_Select$(_U)
#define T_use_conc_Select$(_U...) __comp_gen__T_use_conc_Select$(_U)

T_alias$((conc_Select_Case)(struct conc_Select_Case));
T_alias$((conc_Select$raw)(struct conc_Select$raw));

struct conc_Select_Case {
    var_(field_idx, u32);
    var_(src, conc_AwaitSrc);
    var_(result, u_P$raw);
    var_(link, conc_AwaitLink);
};
T_use_prl$(conc_Select_Case);

struct conc_Select$raw {
    var_(cases, S$conc_Select_Case);
    var_(len, usize);
    var_(type, debug_TypeInfo);
};

$extern fn_((conc_Select_init(TypeInfo union_ty, S$conc_Select_Case cases))(conc_Select$raw));
$extern fn_((conc_Select_addSrc(conc_Select$raw* self, u32 field_idx, conc_AwaitSrc src, u_P$raw result))(void));
$extern fn_((conc_Select_addFuture(conc_Select$raw* self, u32 field_idx, Future$raw* fut, TypeInfo field_ty, u_P$raw result))(void));
$extern fn_((conc_Select_poll(conc_Select$raw* self, TypeInfo union_ty, u_V$raw ret_mem))(bool));
$attr($must_check)
$extern fn_((conc_Select_await(conc_Select$raw* self, TypeInfo union_ty, Sched sched, u_V$raw ret_mem))(Sched_Cancelable$V$raw));
$attr($must_check)
$extern fn_((conc_Select_awaitFor(conc_Select$raw* self, TypeInfo union_ty, Sched sched, time_Dur dur, u_V$raw ret_mem))(E$($set(Sched_TimedE)(V$raw))));
$attr($must_check)
$extern fn_((conc_Select_awaitUntil(conc_Select$raw* self, TypeInfo union_ty, Sched sched, time_Inst deadline, u_V$raw ret_mem))(E$($set(Sched_TimedE)(V$raw))));

#define __comp_anon__conc_Select$$(_U...) \
    union { \
        T_embed$(struct { \
            var_(cases, S$conc_Select_Case); \
            var_(len, usize); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Select$raw) $like_ref; \
    }
#define __comp_alias__conc_Select$(_U...) tpl$(conc_Select, _U)
#define __comp_gen__T_decl_conc_Select$(_U...) \
    $maybe_unused typedef union conc_Select$(_U) conc_Select$(_U)
#define __comp_gen__T_impl_conc_Select$(_U...) \
    union conc_Select$(_U) { \
        T_embed$(struct { \
            var_(cases, S$conc_Select_Case); \
            var_(len, usize); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Select$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_Select$(_U...) \
    T_decl_conc_Select$(_U); \
    T_impl_conc_Select$(_U)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_Select__included */
