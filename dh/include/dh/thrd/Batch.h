/**
 * @file    Batch.h
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Batch
 * @brief   Homogeneous OS-thread result completion batch surface.
 */
#pragma once
#ifndef thrd_Batch__included
#define thrd_Batch__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "Group.h"
#include "Que.h"
#include "../ArrList.h"
#include "../mem/Alctr.h"
#include "../clsr.h"

typedef u64 thrd_Batch_Id;
T_use_atom_V$(thrd_Batch_Id);
T_use_E$($set(thrd_spawn_E)(thrd_Batch_Id));

typedef struct thrd_Batch {
    var_(group, thrd_Group);
    var_(done, thrd_Que);
    var_(next_id, atom_V$thrd_Batch_Id);
    var_(type, debug_TypeInfo);
} thrd_Batch;

typedef struct thrd_Batch_Done$raw {
    T_embed$(struct {
        var_(id, thrd_Batch_Id);
        var_(type, debug_TypeInfo);
    });
    var_(result_, V$raw) $flexible;
} thrd_Batch_Done$raw;
T_use$((thrd_Batch_Done$raw)(P, u_V, (O, u_V), ($spec(E, $set(thrd_chan_WaitE)), O, u_V)));

#define thrd_Batch$(_T...) tpl$(thrd_Batch, _T)
#define thrd_Batch_Done$(_T...) tpl$(thrd_Batch_Done, _T)
#define T_decl_thrd_Batch$(_T...) $maybe_unused typedef union thrd_Batch$(_T) thrd_Batch$(_T)
#define T_decl_thrd_Batch_Done$(_T...) $maybe_unused typedef union thrd_Batch_Done$(_T) thrd_Batch_Done$(_T)
#define T_impl_thrd_Batch$(_T...) \
    union thrd_Batch$(_T) { \
        var_(as_raw, thrd_Batch) $like_ref; \
    }
#define T_impl_thrd_Batch_Done$(_T...) \
    union thrd_Batch_Done$(_T) { \
        T_embed$(struct { var_(id, u64); var_(result, _T); }); \
        var_(as_raw, thrd_Batch_Done$raw) $like_ref; \
    }
#define T_use_thrd_Batch$(_T...) \
    T_decl_thrd_Batch$(_T); \
    T_impl_thrd_Batch$(_T)
#define T_use_thrd_Batch_Done$(_T...) \
    T_decl_thrd_Batch_Done$(_T); \
    T_impl_thrd_Batch_Done$(_T)

$extern fn_((thrd_Batch_init(u_S$raw done_buf))(thrd_Batch));
$extern fn_((thrd_Batch_fini(thrd_Batch* self, TypeInfo done_type))(void));
$extern fn_((thrd_Batch_cancelTok(thrd_Batch* self))(thrd_CancelTok));
$extern fn_((thrd_Batch_cancel(thrd_Batch* self))(void));
$attr($must_check)
$extern fn_((thrd_Batch_spawn(thrd_Batch* self, TypeInfo done_type, mem_Alctr gpa, Clsr$raw* clsr))(thrd_spawn_E$thrd_Batch_Id));
$attr($must_check)
$extern fn_((thrd_Batch_tryNext(thrd_Batch* self, u_V$raw ret_mem))(O$u_V$thrd_Batch_Done$raw));
$attr($must_check)
$extern fn_((thrd_Batch_next(thrd_Batch* self, u_V$raw ret_mem))(thrd_chan_WaitE$O$u_V$thrd_Batch_Done$raw));
$extern fn_((thrd_Batch_wait(thrd_Batch* self))(void));

#if defined(__cplusplus)
}
#endif
#endif
