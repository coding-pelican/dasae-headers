/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file Select.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Select
 * @brief   Heterogeneous OS-thread blocking wait selection surface.
 */
#pragma once
#ifndef thrd_Select__included
#define thrd_Select__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "CancelTok.h"
#include "../meta.h"

/*========== Macros and Declarations ========================================*/

T_alias$((thrd_Select_Op_VTbl)(struct thrd_Select_Op_VTbl));
T_alias$((thrd_Select_Op)(struct thrd_Select_Op {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(thrd_Select_Op_VTbl));
    var_(src, thrd_wait_Src);
}));
$attr($inline_always)
$static fn_((thrd_Select_Op_isValid(thrd_Select_Op self))(bool));
$attr($inline_always)
$static fn_((thrd_Select_Op_assertValid(P$raw ctx, P_const$$(thrd_Select_Op_VTbl) vtbl, thrd_wait_Src src))(void));
$attr($inline_always)
$static fn_((thrd_Select_Op_ensureValid(thrd_Select_Op self))(thrd_Select_Op));

typedef struct thrd_Select_Case {
    var_(field_idx, usize);
    var_(op, thrd_Select_Op);
    var_(result, u_P$raw);
    var_(link, thrd_wait_Link);
} thrd_Select_Case;
T_use_prl$(thrd_Select_Case);
typedef struct thrd_Select {
    var_(cases, S$thrd_Select_Case);
    var_(len, usize);
    var_(result_type, debug_TypeInfo);
} thrd_Select;

typedef struct thrd_Select_Done$raw {
    T_embed$(struct {
        var_(field_idx, usize);
        var_(type, debug_TypeInfo);
    });
    var_(result_, V$raw) $flexible;
} thrd_Select_Done$raw;
T_use$((thrd_Select_Done$raw)(
    P,
    u_V,
    (O, u_V),
    ($spec(E, $set(Sched_Cancelable)), u_V),
    ($spec(E, $set(Sched_TimedE)), u_V)
));

#define thrd_Select$(_T...) __alias__thrd_Select$(_T)
#define thrd_Select$$(_T...) __anon__thrd_Select$$(_T)
#define T_decl_thrd_Select$(_T...) __gen__T_decl_thrd_Select$(_T)
#define T_impl_thrd_Select$(_T...) __gen__T_impl_thrd_Select$(_T)
#define T_use_thrd_Select$(_T...) __gen__T_use_thrd_Select$(_T)
#define thrd_Select_Done$(_T...) __alias__thrd_Select_Done$(_T)
#define thrd_Select_Done$$(_T...) __anon__thrd_Select_Done$$(_T)
#define T_decl_thrd_Select_Done$(_T...) __gen__T_decl_thrd_Select_Done$(_T)
#define T_impl_thrd_Select_Done$(_T...) __gen__T_impl_thrd_Select_Done$(_T)
#define T_use_thrd_Select_Done$(_T...) __gen__T_use_thrd_Select_Done$(_T)

T_alias$((u_Fields_Idx$thrd_Select_Done)(enum_((u_Fields_Idx$thrd_Select_Done $fits($packed))(
    u_Fields_Idx_field_idx$thrd_Select_Done = 0,
    u_Fields_Idx_type$thrd_Select_Done,
    u_Fields_Idx_result_$thrd_Select_Done,
    count$u_Fields_Idx$thrd_Select_Done
))));
$static let_(u_Fields_type$thrd_Select_Done, A$$(count$u_Fields_Idx$thrd_Select_Done, TypeInfo)) = A_init({
    [u_Fields_Idx_field_idx$thrd_Select_Done] = typeInfo$(FieldType$(thrd_Select_Done$raw, field_idx)),
    [u_Fields_Idx_type$thrd_Select_Done] = typeInfo$(FieldType$(thrd_Select_Done$raw, type)),
    [u_Fields_Idx_result_$thrd_Select_Done] = typeInfo$(FieldType$(thrd_Select_Done$raw, result_)),
});
$attr($inline_always)
$static fn_((thrd_Select_Done_typeInfo(TypeInfo result_type))(TypeInfo));
$attr($inline_always)
$static fn_((thrd_Select_Done_init(usize field_idx, TypeInfo result_type, u_V$thrd_Select_Done$raw ret_mem))(u_V$thrd_Select_Done$raw));
#define T_use_thrd_Select_Done_init$(_T...) __gen__T_use_thrd_Select_Done_init$(_T)
$attr($inline_always)
$static fn_((thrd_Select_Done_result(const thrd_Select_Done$raw* self, TypeInfo result_type))(u_P_const$raw));
#define T_use_thrd_Select_Done_result$(_T...) __gen__T_use_thrd_Select_Done_result$(_T)
$attr($inline_always)
$static fn_((thrd_Select_Done_resultMut(thrd_Select_Done$raw* self, TypeInfo result_type))(u_P$raw));
#define T_use_thrd_Select_Done_resultMut$(_T...) __gen__T_use_thrd_Select_Done_resultMut$(_T)

$extern fn_((thrd_Select_init(TypeInfo result_ty, S$thrd_Select_Case cases))(thrd_Select));
#define T_use_thrd_Select_init$(_T...) __gen__T_use_thrd_Select_init$(_T)

$extern fn_((thrd_Select_watch(thrd_Select* self, usize field_idx, thrd_Select_Op op, u_P$raw result))(void));
#define T_use_thrd_Select_watch$(_T...) __gen__T_use_thrd_Select_watch$(_T)
$attr($must_check)
$extern fn_((thrd_Select_poll(thrd_Select* self, u_V$thrd_Select_Done$raw ret_mem))(O$u_V$thrd_Select_Done$raw));
#define T_use_thrd_Select_poll$(_T...) __gen__T_use_thrd_Select_poll$(_T)

$attr($must_check)
$extern fn_((thrd_Select_wait(thrd_Select* self, thrd_CancelTok cancel, u_V$thrd_Select_Done$raw ret_mem))(Sched_Cancelable$u_V$thrd_Select_Done$raw));
#define T_use_thrd_Select_wait$(_T...) __gen__T_use_thrd_Select_wait$(_T)
$attr($must_check)
$extern fn_((thrd_Select_waitFor(thrd_Select* self, thrd_CancelTok cancel, time_Dur dur, u_V$thrd_Select_Done$raw ret_mem))(Sched_TimedE$u_V$thrd_Select_Done$raw));
#define T_use_thrd_Select_waitFor$(_T...) __gen__T_use_thrd_Select_waitFor$(_T)
$extern fn_((thrd_Select_waitProtcd(thrd_Select* self, u_V$thrd_Select_Done$raw ret_mem))(u_V$thrd_Select_Done$raw));
#define T_use_thrd_Select_waitProtcd$(_T...) __gen__T_use_thrd_Select_waitProtcd$(_T)

/*========== Macros and Definitions =========================================*/

struct thrd_Select_Op_VTbl {
    fn_(((*pollFn)(P$raw ctx, u_P$raw result))(bool));
};

#if on_analysis_active_only || on_comptime
fn_((thrd_Select_Op_isValid(thrd_Select_Op self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->pollFn)
        && thrd_wait_Src_isValid(self.src);
};
fn_((thrd_Select_Op_assertValid(P$raw ctx, P_const$$(thrd_Select_Op_VTbl) vtbl, thrd_wait_Src src))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->pollFn);
    thrd_wait_Src_assertValid(src.ctx, src.vtbl);
};
fn_((thrd_Select_Op_ensureValid(thrd_Select_Op self))(thrd_Select_Op)) {
    return thrd_Select_Op_assertValid(self.ctx, self.vtbl, self.src), self;
};
#endif /* on_analysis_active_only || on_comptime */

/* clang-format off */
#define __alias__thrd_Select$(_T...) tpl$(thrd_Select, _T)
#define __anon__thrd_Select$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(cases, S$thrd_Select_Case); \
        var_(len, usize); \
        var_(result_type, debug_TypeInfo); \
    }); \
    var_(as_raw, thrd_Select) $like_ref; \
})
#define __gen__T_decl_thrd_Select$(_T...) \
    $maybe_unused typedef union thrd_Select$(_T) thrd_Select$(_T)
#define __gen__T_impl_thrd_Select$(_T...) \
    union thrd_Select$(_T) { \
        T_embed$(struct { \
            var_(cases, S$thrd_Select_Case); \
            var_(len, usize); \
            var_(result_type, debug_TypeInfo); \
        }); \
        var_(as_raw, thrd_Select) $like_ref; \
    }
#define __gen__T_use_thrd_Select$(_T...) \
    T_use_thrd_Select_Done$(_T); \
    T_decl_thrd_Select$(_T); \
    T_impl_thrd_Select$(_T)

#define __alias__thrd_Select_Done$(_T...) tpl$(thrd_Select_Done, _T)
#define __anon__thrd_Select_Done$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(field_idx, usize); \
        var_(type, debug_TypeInfo); \
        T_embed$(union { \
            var_(result, _T); \
            var_(result_, _T) $like_ref; \
        }); \
    }); \
    var_(as_raw, thrd_Select_Done$raw) $flexible; \
})
#define __gen__T_decl_thrd_Select_Done$(_T...) \
    $maybe_unused typedef union thrd_Select_Done$(_T) thrd_Select_Done$(_T)
#define __gen__T_impl_thrd_Select_Done$(_T...) \
    union thrd_Select_Done$(_T) { \
        T_embed$(struct { \
            var_(field_idx, usize); \
            var_(type, debug_TypeInfo); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, thrd_Select_Done$raw) $flexible; \
    }
#define __gen__T_use_thrd_Select_Done$(_T...) \
    T_decl_thrd_Select_Done$(_T); \
    T_impl_thrd_Select_Done$(_T); \
    T_use_P$(thrd_Select_Done$(_T)); \
    T_use_S$(thrd_Select_Done$(_T)); \
    T_use_O$(thrd_Select_Done$(_T)); \
    T_use_E$($set(Sched_Cancelable)(thrd_Select_Done$(_T))); \
    T_use_E$($set(Sched_TimedE)(thrd_Select_Done$(_T)))

#define __gen__T_use_thrd_Select_Done_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Done_init, _T)(usize field_idx))(thrd_Select_Done$(_T))) { \
        return u_castV$((thrd_Select_Done$(_T))(thrd_Select_Done_init(field_idx, typeInfo$(_T), u_asV$((u_V$thrd_Select_Done$raw)(u_retV$(thrd_Select_Done$(_T))))))); \
    }
#define __gen__T_use_thrd_Select_Done_result$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Done_result, _T)(P_const$$(thrd_Select_Done$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(thrd_Select_Done_result(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_Select_Done_resultMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Done_resultMut, _T)(P$$(thrd_Select_Done$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_Select_Done_resultMut(self->as_raw, typeInfo$(_T)))); \
    }

#define __gen__T_use_thrd_Select_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_init, _T)(S$thrd_Select_Case cases))(thrd_Select$(_T))) { \
        return type$((thrd_Select$(_T))(thrd_Select_init(typeInfo$(_T), cases))); \
    }

#define __gen__T_use_thrd_Select_watch$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_watch, _T)(P$$(thrd_Select$(_T)) self, usize field_idx, thrd_Select_Op op, P$$(_T) result))(void)) { \
        return thrd_Select_watch(self->as_raw, field_idx, op, u_anyP(result)); \
    }
#define __gen__T_use_thrd_Select_poll$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_poll, _T)(P$$(thrd_Select$(_T)) self))(O$(thrd_Select_Done$(_T))) $scope) { \
        return_(u_castO$((ReturnType)(thrd_Select_poll(self->as_raw, u_asV$((u_V$thrd_Select_Done$raw)(u_retV$(thrd_Select_Done$(_T)))))))); \
    } $unscoped(fn)

#define __gen__T_use_thrd_Select_wait$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_wait, _T)(P$$(thrd_Select$(_T)) self, thrd_CancelTok cancel))(E$($set(Sched_Cancelable)(thrd_Select_Done$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Select_wait(self->as_raw, cancel, u_asV$((u_V$thrd_Select_Done$raw)(u_retV$(thrd_Select_Done$(_T)))))))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_waitFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_waitFor, _T)(P$$(thrd_Select$(_T)) self, thrd_CancelTok cancel, time_Dur dur))(E$($set(Sched_TimedE)(thrd_Select_Done$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Select_waitFor(self->as_raw, cancel, dur, u_asV$((u_V$thrd_Select_Done$raw)(u_retV$(thrd_Select_Done$(_T)))))))); \
    }
#define __gen__T_use_thrd_Select_waitProtcd$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_waitProtcd, _T)(P$$(thrd_Select$(_T)) self))(thrd_Select_Done$(_T))) { \
        return u_castV$((thrd_Select_Done$(_T))(thrd_Select_waitProtcd(self->as_raw, u_asV$((u_V$thrd_Select_Done$raw)(u_retV$(thrd_Select_Done$(_T))))))); \
    }
/* clang-format on */

#if on_analysis_active_only || on_comptime
fn_((thrd_Select_Done_typeInfo(TypeInfo result_type))(TypeInfo)) {
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Done)(
        (.val[u_Fields_Idx_result_$thrd_Select_Done])(result_type)
    )));
    return u_typeInfoRecord(ty_fields);
};
fn_((thrd_Select_Done_init(usize field_idx, TypeInfo result_type, u_V$thrd_Select_Done$raw ret_mem))(u_V$thrd_Select_Done$raw)) {
    claim_assert_nonnull(ret_mem.inner), debug_assert_eqBy(ret_mem.type, thrd_Select_Done_typeInfo(result_type), TypeInfo_eql);
    ret_mem.inner->field_idx = field_idx;
    ret_mem.inner->type = $typing(result_type);
    mem_set0P(thrd_Select_Done_resultMut(ret_mem.inner, result_type));
    return ret_mem;
};
fn_((thrd_Select_Done_result(const thrd_Select_Done$raw* self, TypeInfo result_type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), result_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Done)(
        (.val[u_Fields_Idx_result_$thrd_Select_Done])(result_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_result_$thrd_Select_Done);
};
fn_((thrd_Select_Done_resultMut(thrd_Select_Done$raw* self, TypeInfo result_type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), result_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Done)(
        (.val[u_Fields_Idx_result_$thrd_Select_Done])(result_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_result_$thrd_Select_Done);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Select__included */
