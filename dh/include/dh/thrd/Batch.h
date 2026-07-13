/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Batch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-07-13 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Batch
 *
 * @brief   Homogeneous OS-thread result completion batch surface.
 */
#pragma once
#ifndef thrd_Batch__included
#define thrd_Batch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* thrd_Batch__included */

/*========== Includes =======================================================*/

#include "CancelTok.h"
#include "Group.h"
#include "Que.h"

/*========== Macros and Declarations ========================================*/

typedef u64 thrd_Batch_Id;
T_use_atom_V$(thrd_Batch_Id);
T_use_E$($set(thrd_spawn_E)(thrd_Batch_Id));
errset_((thrd_Batch_SpawnE)() $union_errset_(thrd_spawn_E, thrd_chan_CapE));
T_use_E$($set(thrd_Batch_SpawnE)(thrd_Batch_Id));

typedef struct thrd_Batch {
    var_(group, thrd_Group);
    var_(done, thrd_Que);
    var_(next_id, atom_V$thrd_Batch_Id);
    var_(lock, thrd_Mtx);
    var_(waiters, thrd_wait_Chain);
    var_(type, debug_TypeInfo);
} thrd_Batch;

typedef struct thrd_Batch_Done$raw {
    T_embed$(struct {
        var_(id, thrd_Batch_Id);
        var_(type, debug_TypeInfo);
    });
    var_(result_, V$raw) $flexible;
} thrd_Batch_Done$raw;
T_use$((thrd_Batch_Done$raw)(P, u_V, (O, u_V), ($spec(E, $set(Sched_Cancelable)), O, u_V)));

#define thrd_Batch$(_T...) __alias__thrd_Batch$(_T)
#define thrd_Batch$$(_T...) __anon__thrd_Batch$$(_T)
#define T_decl_thrd_Batch$(_T...) __gen__T_decl_thrd_Batch$(_T)
#define T_impl_thrd_Batch$(_T...) __gen__T_impl_thrd_Batch$(_T)
#define T_use_thrd_Batch$(_T...) __gen__T_use_thrd_Batch$(_T)
#define thrd_Batch_Done$(_T...) __alias__thrd_Batch_Done$(_T)
#define thrd_Batch_Done$$(_T...) __anon__thrd_Batch_Done$$(_T)
#define T_decl_thrd_Batch_Done$(_T...) __gen__T_decl_thrd_Batch_Done$(_T)
#define T_impl_thrd_Batch_Done$(_T...) __gen__T_impl_thrd_Batch_Done$(_T)
#define T_use_thrd_Batch_Done$(_T...) __gen__T_use_thrd_Batch_Done$(_T)

T_alias$((u_Fields_Idx$thrd_Batch_Done)(enum_((u_Fields_Idx$thrd_Batch_Done $fits($packed))(
    u_Fields_Idx_id$thrd_Batch_Done = 0,
    u_Fields_Idx_type$thrd_Batch_Done,
    u_Fields_Idx_result_$thrd_Batch_Done,
    count$u_Fields_Idx$thrd_Batch_Done
))));
$static let_(u_Fields_type$thrd_Batch_Done, A$$(count$u_Fields_Idx$thrd_Batch_Done, TypeInfo)) = A_init({
    [u_Fields_Idx_id$thrd_Batch_Done] = typeInfo$(FieldType$(thrd_Batch_Done$raw, id)),
    [u_Fields_Idx_type$thrd_Batch_Done] = typeInfo$(FieldType$(thrd_Batch_Done$raw, type)),
    [u_Fields_Idx_result_$thrd_Batch_Done] = typeInfo$(FieldType$(thrd_Batch_Done$raw, result_)),
});
$attr($inline_always)
$static fn_((thrd_Batch_Done_typeInfo(TypeInfo result_type))(TypeInfo));
$attr($inline_always)
$static fn_((thrd_Batch_Done_result(const thrd_Batch_Done$raw* self, TypeInfo result_type))(u_P_const$raw));
#define T_use_thrd_Batch_Done_result$(_T...) __gen__T_use_thrd_Batch_Done_result$(_T)
$attr($inline_always)
$static fn_((thrd_Batch_Done_resultMut(thrd_Batch_Done$raw* self, TypeInfo result_type))(u_P$raw));
#define T_use_thrd_Batch_Done_resultMut$(_T...) __gen__T_use_thrd_Batch_Done_resultMut$(_T)

$extern fn_((thrd_Batch_Done_from(thrd_Batch_Id id, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(u_V$thrd_Batch_Done$raw));
#define T_use_thrd_Batch_Done_from$(_T...) __gen__T_use_thrd_Batch_Done_from$(_T)
$extern fn_((thrd_Batch_Done_into(const thrd_Batch_Done$raw* self, TypeInfo result_type, u_V$raw ret_mem))(u_V$raw));
#define T_use_thrd_Batch_Done_into$(_T...) __gen__T_use_thrd_Batch_Done_into$(_T)
$extern fn_((thrd_Batch_Done_take(thrd_Batch_Done$raw* self, TypeInfo result_type, u_V$raw ret_mem))(u_V$raw));
#define T_use_thrd_Batch_Done_take$(_T...) __gen__T_use_thrd_Batch_Done_take$(_T)

$extern fn_((thrd_Batch_init(TypeInfo result_type, u_S$raw done_buf))(thrd_Batch));
#define T_use_thrd_Batch_init$(_T...) __gen__T_use_thrd_Batch_init$(_T)
$extern fn_((thrd_Batch_fini(thrd_Batch* self, TypeInfo result_type))(void));
#define T_use_thrd_Batch_fini$(_T...) __gen__T_use_thrd_Batch_fini$(_T)

$attr($must_check)
$extern fn_((thrd_Batch_spawn(thrd_Batch* self, TypeInfo result_type, mem_Alctr gpa, Clsr$raw* clsr))(thrd_Batch_SpawnE$thrd_Batch_Id));
#define T_use_thrd_Batch_spawn$(_T...) __gen__T_use_thrd_Batch_spawn$(_T)

$attr($must_check)
$extern fn_((thrd_Batch_tryNext(thrd_Batch* self, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw));
#define T_use_thrd_Batch_tryNext$(_T...) __gen__T_use_thrd_Batch_tryNext$(_T)
$attr($must_check)
$extern fn_((thrd_Batch_next(thrd_Batch* self, TypeInfo result_type, thrd_CancelTok cancel, u_V$thrd_Batch_Done$raw ret_mem))(Sched_Cancelable$O$u_V$thrd_Batch_Done$raw));
#define T_use_thrd_Batch_next$(_T...) __gen__T_use_thrd_Batch_next$(_T)
$attr($must_check)
$extern fn_((thrd_Batch_nextProtcd(thrd_Batch* self, TypeInfo result_type, u_V$thrd_Batch_Done$raw ret_mem))(O$u_V$thrd_Batch_Done$raw));
#define T_use_thrd_Batch_nextProtcd$(_T...) __gen__T_use_thrd_Batch_nextProtcd$(_T)
$attr($must_check)
$extern fn_((thrd_Batch_wait(thrd_Batch* self, TypeInfo result_type, thrd_CancelTok cancel))(Sched_Cancelable$void));
#define T_use_thrd_Batch_wait$(_T...) __gen__T_use_thrd_Batch_wait$(_T)
$extern fn_((thrd_Batch_waitProtcd(thrd_Batch* self, TypeInfo result_type))(void));
#define T_use_thrd_Batch_waitProtcd$(_T...) __gen__T_use_thrd_Batch_waitProtcd$(_T)

/*========== Macros and Definitions =========================================*/

/* clang-format off */
#define __alias__thrd_Batch$(_T...) tpl$(thrd_Batch, _T)
#define __anon__thrd_Batch$$(_T...) TypeOf(union { \
        T_embed$(struct { \
            var_(group, thrd_Group); \
            var_(done, thrd_Que$$(thrd_Batch_Done$$(_T))); \
            var_(next_id, atom_V$thrd_Batch_Id); \
            var_(lock, thrd_Mtx); \
            var_(waiters, thrd_wait_Chain); \
            var_(type, debug_TypeInfo); \
        }); \
    var_(as_raw, thrd_Batch) $like_ref; \
})
#define __gen__T_decl_thrd_Batch$(_T...) \
    $maybe_unused typedef union thrd_Batch$(_T) thrd_Batch$(_T)
#define __gen__T_impl_thrd_Batch$(_T...) \
    union thrd_Batch$(_T) { \
        T_embed$(struct { \
            var_(group, thrd_Group); \
            var_(done, thrd_Que$$(thrd_Batch_Done$(_T))); \
            var_(next_id, atom_V$thrd_Batch_Id); \
            var_(lock, thrd_Mtx); \
            var_(waiters, thrd_wait_Chain); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, thrd_Batch) $like_ref; \
    }
#define __gen__T_use_thrd_Batch$(_T...) \
    T_use_thrd_Batch_Done$(_T); \
    T_use_thrd_Que$(thrd_Batch_Done$(_T)); \
    T_decl_thrd_Batch$(_T); \
    T_impl_thrd_Batch$(_T)

#define __alias__thrd_Batch_Done$(_T...) tpl$(thrd_Batch_Done, _T)
#define __anon__thrd_Batch_Done$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(id, thrd_Batch_Id); \
        var_(type, debug_TypeInfo); \
        T_embed$(union { \
            var_(result, _T); \
            var_(result_, _T) $like_ref; \
        }); \
    }); \
    var_(as_raw, thrd_Batch_Done$raw) $flexible; \
})
#define __gen__T_decl_thrd_Batch_Done$(_T...) \
    $maybe_unused typedef union thrd_Batch_Done$(_T) thrd_Batch_Done$(_T)
#define __gen__T_impl_thrd_Batch_Done$(_T...) \
    union thrd_Batch_Done$(_T) { \
        T_embed$(struct { \
            var_(id, thrd_Batch_Id); \
            var_(type, debug_TypeInfo); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, thrd_Batch_Done$raw) $flexible; \
    }
#define __gen__T_use_thrd_Batch_Done$(_T...) \
    T_decl_thrd_Batch_Done$(_T); \
    T_impl_thrd_Batch_Done$(_T); \
    T_use_P$(thrd_Batch_Done$(_T)); \
    T_use_S$(thrd_Batch_Done$(_T)); \
    T_use_O$(thrd_Batch_Done$(_T)); \
    T_use_E$($set(Sched_Cancelable)(O$(thrd_Batch_Done$(_T))))

#define __gen__T_use_thrd_Batch_Done_result$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_Done_result, _T)(P_const$$(thrd_Batch_Done$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(thrd_Batch_Done_result(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_Batch_Done_resultMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_Done_resultMut, _T)(P$$(thrd_Batch_Done$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_Batch_Done_resultMut(self->as_raw, typeInfo$(_T)))); \
    }

#define __gen__T_use_thrd_Batch_Done_from$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_Done_from, _T)(thrd_Batch_Id id))(thrd_Batch_Done$(_T))) { \
        return u_castV$((thrd_Batch_Done$(_T))(thrd_Batch_Done_from(id, typeInfo$(_T), u_asV$((u_V$thrd_Batch_Done$raw)(u_retV$(thrd_Batch_Done$(_T))))))); \
    }
#define __gen__T_use_thrd_Batch_Done_into$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_Done_into, _T)(thrd_Batch_Done$(_T) self))(_T)) { \
        return u_castV$((_T)(thrd_Batch_Done_into(self.as_raw, typeInfo$(_T), u_retV$(_T)))); \
    }
#define __gen__T_use_thrd_Batch_Done_take$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_Done_take, _T)(P$$(thrd_Batch_Done$(_T)) self))(_T)) { \
        claim_assert_nonnull(self); \
        return u_castV$((_T)(thrd_Batch_Done_take(self->as_raw, typeInfo$(_T), u_retV$(_T)))); \
    }

#define __gen__T_use_thrd_Batch_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_init, _T)(S$(thrd_Batch_Done$(_T)) done_buf))(thrd_Batch$(_T))) { \
        return type$((thrd_Batch$(_T))(thrd_Batch_init(typeInfo$(_T), u_anyS(done_buf)))); \
    }
#define __gen__T_use_thrd_Batch_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_fini, _T)(P$$(thrd_Batch$(_T)) self))(void)) { \
        return thrd_Batch_fini(self->as_raw, typeInfo$(_T)); \
    }

#define __gen__T_use_thrd_Batch_spawn$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Batch_spawn, _T)(P$$(thrd_Batch$(_T)) self, mem_Alctr gpa, P$$(Clsr$(_T)) clsr))(thrd_Batch_SpawnE$thrd_Batch_Id)) { \
        return thrd_Batch_spawn(self->as_raw, typeInfo$(_T), gpa, clsr->as_raw); \
    }

#define __gen__T_use_thrd_Batch_tryNext$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Batch_tryNext, _T)(P$$(thrd_Batch$(_T)) self))(O$(thrd_Batch_Done$(_T))) $scope) { \
        return_(u_castO$((ReturnType)(thrd_Batch_tryNext(self->as_raw, typeInfo$(_T), u_asV$((u_V$thrd_Batch_Done$raw)(u_retV$(thrd_Batch_Done$(_T)))))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Batch_next$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Batch_next, _T)(P$$(thrd_Batch$(_T)) self, thrd_CancelTok cancel))(E$($set(Sched_Cancelable)(O$(thrd_Batch_Done$(_T))))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Batch_next(self->as_raw, typeInfo$(_T), cancel, u_asV$((u_V$thrd_Batch_Done$raw)(u_retV$(thrd_Batch_Done$(_T)))))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Batch_nextProtcd$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Batch_nextProtcd, _T)(P$$(thrd_Batch$(_T)) self))(O$(thrd_Batch_Done$(_T))) $scope) { \
        return_(u_castO$((ReturnType)(thrd_Batch_nextProtcd(self->as_raw, typeInfo$(_T), u_asV$((u_V$thrd_Batch_Done$raw)(u_retV$(thrd_Batch_Done$(_T)))))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Batch_wait$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Batch_wait, _T)(P$$(thrd_Batch$(_T)) self, thrd_CancelTok cancel))(Sched_Cancelable$void)) { \
        return thrd_Batch_wait(self->as_raw, typeInfo$(_T), cancel); \
    }
#define __gen__T_use_thrd_Batch_waitProtcd$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Batch_waitProtcd, _T)(P$$(thrd_Batch$(_T)) self))(void)) { \
        return thrd_Batch_waitProtcd(self->as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

#if on_analysis_active_only || on_comptime
fn_((thrd_Batch_Done_typeInfo(TypeInfo result_type))(TypeInfo)) {
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Batch_Done)(
        (.val[u_Fields_Idx_result_$thrd_Batch_Done])(result_type)
    )));
    return u_typeInfoRecord(ty_fields);
};
fn_((thrd_Batch_Done_result(const thrd_Batch_Done$raw* self, TypeInfo result_type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), result_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Batch_Done)(
        (.val[u_Fields_Idx_result_$thrd_Batch_Done])(result_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_result_$thrd_Batch_Done);
};
fn_((thrd_Batch_Done_resultMut(thrd_Batch_Done$raw* self, TypeInfo result_type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), result_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Batch_Done)(
        (.val[u_Fields_Idx_result_$thrd_Batch_Done])(result_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_result_$thrd_Batch_Done);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Batch__included */
