/**
 * @file    conc/Batch.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Batch
 *
 * @brief   Homogeneous concurrent execution batch.
 */
#pragma once
#ifndef conc_Batch__included
#define conc_Batch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"
#include "../Future.h"
#include "../Sched.h"
#include "../ArrList.h"
#include "../mem/Alctr.h"
#include "../clsr.h"

#define conc_Batch$$(_T...) __comp_anon__conc_Batch$$(_T)
#define conc_Batch$(_T...) __comp_alias__conc_Batch$(_T)
#define T_decl_conc_Batch$(_T...) __comp_gen__T_decl_conc_Batch$(_T)
#define T_impl_conc_Batch$(_T...) __comp_gen__T_impl_conc_Batch$(_T)
#define T_use_conc_Batch$(_T...) __comp_gen__T_use_conc_Batch$(_T)

#define conc_Batch_Done$$(_T...) __comp_anon__conc_Batch_Done$$(_T)
#define conc_Batch_Done$(_T...) __comp_alias__conc_Batch_Done$(_T)
#define T_decl_conc_Batch_Done$(_T...) __comp_gen__T_decl_conc_Batch_Done$(_T)
#define T_impl_conc_Batch_Done$(_T...) __comp_gen__T_impl_conc_Batch_Done$(_T)
#define T_use_conc_Batch_Done$(_T...) __comp_gen__T_use_conc_Batch_Done$(_T)

errset_((conc_Batch_SpawnE)() $union_errset_(mem_E, Sched_ConcE));
T_use_E$($set(conc_Batch_SpawnE)(u64));

T_alias$((conc_Batch_Node)(struct conc_Batch_Node));
T_alias$((conc_Batch$raw)(struct conc_Batch$raw));
T_alias$((conc_Batch_Done$raw)(struct conc_Batch_Done$raw));
T_use_E$($set(mem_E)(u64));

struct conc_Batch_Node {
    var_(id, u64);
    var_(future, V$Future$raw);
    var_(done_next, O$usize);
};
T_use_prl$(conc_Batch_Node);
T_use_ArrList$(conc_Batch_Node);

struct conc_Batch$raw {
    var_(nodes, ArrList$conc_Batch_Node);
    var_(done_head, O$usize);
    var_(done_tail, O$usize);
    var_(next_id, u64);
    var_(active_count, usize);
    var_(cancel_requested, atom_V$bool);
    var_(type, debug_TypeInfo);
};
T_use_E$($set(mem_E)(conc_Batch$raw));

struct conc_Batch_Done$raw {
    T_embed$(struct {
        var_(id, u64);
        var_(type, debug_TypeInfo);
    });
    var_(result_, V$raw) $flexible;
};
T_use_O$(conc_Batch_Done$raw);
T_use_E$($set(Sched_Cancelable)(O$conc_Batch_Done$raw));

$extern fn_((conc_Batch_empty(TypeInfo type))(conc_Batch$raw));
$extern fn_((conc_Batch_init(TypeInfo type, mem_Alctr gpa, usize cap))(E$($set(mem_E)(conc_Batch$raw))));
$extern fn_((conc_Batch_fini(conc_Batch$raw* self, TypeInfo type, mem_Alctr gpa))(void));
$extern fn_((conc_Batch_add(conc_Batch$raw* self, TypeInfo type, mem_Alctr gpa, V$Future$raw fut))(E$($set(mem_E)(u64))));
$extern fn_((conc_Batch_async(conc_Batch$raw* self, TypeInfo type, mem_Alctr gpa, Sched sched, Clsr$raw* clsr))(E$($set(mem_E)(u64))));
$attr($must_check)
$extern fn_((conc_Batch_spawn(conc_Batch$raw* self, TypeInfo type, mem_Alctr gpa, Sched sched, Clsr$raw* clsr))(E$($set(conc_Batch_SpawnE)(u64))));
$extern fn_((conc_Batch_tryNext(conc_Batch$raw* self, TypeInfo type, V$raw ret_mem))(O$conc_Batch_Done$raw));
$attr($must_check)
$extern fn_((conc_Batch_next(conc_Batch$raw* self, TypeInfo type, Sched sched, V$raw ret_mem))(E$($set(Sched_Cancelable)(O$conc_Batch_Done$raw))));
$attr($must_check)
$extern fn_((conc_Batch_await(conc_Batch$raw* self, TypeInfo type, Sched sched))(Sched_Cancelable$void));
$extern fn_((conc_Batch_cancel(conc_Batch$raw* self, TypeInfo type, Sched sched))(void));

#define __comp_anon__conc_Batch$$(_T...) \
    union { \
        T_embed$(struct { \
            var_(nodes, ArrList$conc_Batch_Node); \
            var_(done_head, O$usize); \
            var_(done_tail, O$usize); \
            var_(next_id, u64); \
            var_(active_count, usize); \
            var_(cancel_requested, atom_V$bool); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Batch$raw) $like_ref; \
    }
#define __comp_alias__conc_Batch$(_T...) tpl$(conc_Batch, _T)
#define __comp_gen__T_decl_conc_Batch$(_T...) \
    $maybe_unused typedef union conc_Batch$(_T) conc_Batch$(_T)
#define __comp_gen__T_impl_conc_Batch$(_T...) \
    union conc_Batch$(_T) { \
        T_embed$(struct { \
            var_(nodes, ArrList$conc_Batch_Node); \
            var_(done_head, O$usize); \
            var_(done_tail, O$usize); \
            var_(next_id, u64); \
            var_(active_count, usize); \
            var_(cancel_requested, atom_V$bool); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, conc_Batch$raw) $like_ref; \
    }
#define __comp_gen__T_use_conc_Batch$(_T...) \
    T_decl_conc_Batch$(_T); \
    T_impl_conc_Batch$(_T)

#define __comp_anon__conc_Batch_Done$$(_T...) \
    union { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(id, u64); \
                var_(type, debug_TypeInfo); \
            }); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, conc_Batch_Done$raw) $flexible; \
    }
#define __comp_alias__conc_Batch_Done$(_T...) tpl$(conc_Batch_Done, _T)
#define __comp_gen__T_decl_conc_Batch_Done$(_T...) \
    $maybe_unused typedef union conc_Batch_Done$(_T) conc_Batch_Done$(_T); \
    T_decl_O$(conc_Batch_Done$(_T)); \
    T_decl_E$($set(Sched_Cancelable)(O$(conc_Batch_Done$(_T))))
#define __comp_gen__T_impl_conc_Batch_Done$(_T...) \
    union conc_Batch_Done$(_T) { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(id, u64); \
                var_(type, debug_TypeInfo); \
            }); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, conc_Batch_Done$raw) $flexible; \
    }; \
    T_impl_O$(conc_Batch_Done$(_T)); \
    T_impl_E$($set(Sched_Cancelable)(O$(conc_Batch_Done$(_T))))
#define __comp_gen__T_use_conc_Batch_Done$(_T...) \
    T_decl_conc_Batch_Done$(_T); \
    T_impl_conc_Batch_Done$(_T)

#define T_use_conc_Batch_all$(_T...) \
    T_use_Future$(_T); \
    T_use_conc_Batch$(_T); \
    T_use_conc_Batch_Done$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_Batch__included */
