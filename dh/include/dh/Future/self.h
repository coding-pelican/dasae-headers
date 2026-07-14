#pragma once
#ifndef Future_self__included
#define Future_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../Sched/base.h"

/*========== Macros and Declarations ========================================*/

#define Future$$(_$T...) __type__Future$$(_$T)
#define Future$(_$T...) __alias__Future$(_$T)
#define T_decl_Future$(_$T...) __stmt__T_decl_Future$(_$T)
#define T_impl_Future$(_$T...) __stmt__T_impl_Future$(_$T)
#define T_use_Future$(_$T...) __stmt__T_use_Future$(_$T)
struct Future$raw {
    T_embed$(struct {
        var_(any_future, O$P$FutureAny);
        var_(type, debug_TypeInfo);
    });
    var_(result_, V$raw) $flexible;
};
$extern fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw));
#define T_use_Future_await$(_$T...) __stmt__T_use_Future_await$(_$T)
$extern fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw));
#define T_use_Future_cancel$(_$T...) __stmt__T_use_Future_cancel$(_$T)

T_alias$((u_Fields_Idx$Future)(enum_((u_Fields_Idx$Future $fits($packed))(
    u_Fields_Idx_any_future$Future = 0,
    u_Fields_Idx_type$Future,
    u_Fields_Idx_result_$Future,
    count$u_Fields_Idx$Future
))));
$static let_(u_Fields_type$Future, A$$(count$u_Fields_Idx$Future, TypeInfo)) = A_init({
    [u_Fields_Idx_any_future$Future] = typeInfo$(FieldType$(Future$raw, any_future)),
    [u_Fields_Idx_type$Future] = typeInfo$(FieldType$(Future$raw, type)),
    [u_Fields_Idx_result_$Future] = typeInfo$(FieldType$(Future$raw, result_)),
});

$attr($inline_always)
$static fn_((Future_result(const Future$raw* self, TypeInfo type))(u_P_const$raw));
#define T_use_Future_result$(_$T...) __stmt__T_use_Future_result$(_$T)
$attr($inline_always)
$static fn_((Future_resultMut(Future$raw* self, TypeInfo type))(u_P$raw));
#define T_use_Future_resultMut$(_$T...) __stmt__T_use_Future_resultMut$(_$T)

/*========== Macro and Definitions ==========================================*/

#define __type__Future$$(_$T...) \
    union { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$FutureAny); \
                var_(type, debug_TypeInfo); \
            }); \
            T_embed$(union { \
                var_(result, _$T); \
                var_(result_, _$T) $like_ref; \
            }); \
        }); \
        var_(as_raw, Future$raw) $flexible; \
    }
#define __alias__Future$(_$T...) tpl$(Future, _$T)
#define __stmt__T_decl_Future$(_$T...) \
    T_alias$((Future$(_$T))(union Future$(_$T)))
#define __stmt__T_impl_Future$(_$T...) \
    union Future$(_$T) { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$FutureAny); \
                var_(type, debug_TypeInfo); \
            }); \
            T_embed$(union { \
                var_(result, _$T); \
                var_(result_, _$T) $like_ref; \
            }); \
        }); \
        var_(as_raw, Future$raw) $flexible; \
    }
#define __stmt__T_use_Future$(_$T...) \
    T_decl_Future$(_$T); \
    T_impl_Future$(_$T)

#include "../u-meta.h"

#if on_analysis_active_only || on_comptime
fn_((Future_result(const Future$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$Future)(
        (.val[u_Fields_Idx_result_$Future])(type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_result_$Future);
};
fn_((Future_resultMut(Future$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$Future)(
        (.val[u_Fields_Idx_result_$Future])(type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_result_$Future);
};
#endif /* on_analysis_active_only || on_comptime */

#define __stmt__T_use_Future_result$(_$T...) /* clang-format off */  \
    $attr($inline_always) \
    $static fn_((tpl$(Future_result, _$T)(const Future$(_$T)* self))(const _$T*)) { \
        return u_castP$((const _$T*)(Future_result(self->as_raw, typeInfo$(_$T)))); \
    } /* clang-format on */
#define __stmt__T_use_Future_resultMut$(_$T...) /* clang-format off */\
    $attr($inline_always) \
    $static fn_((tpl$(Future_resultMut, _$T)(Future$(_$T)* self))(_$T*)) { \
        return u_castP$((_$T*)(Future_resultMut(self->as_raw, typeInfo$(_$T)))); \
    } /* clang-format on */
#define __stmt__T_use_Future_await$(_$T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(Future_await, _$T)(Future$(_$T)* self, Sched sched))(_$T)) { \
        return u_castV$((_$T)(Future_await(self->as_raw, sched, u_retV$(_$T)))); \
    }/* clang-format on */
#define __stmt__T_use_Future_cancel$(_$T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(Future_cancel, _$T)(Future$(_$T)* self, Sched sched))(_$T)) { \
        return u_castV$((_$T)(Future_cancel(self->as_raw, sched, u_retV$(_$T)))); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Future_self__included */
