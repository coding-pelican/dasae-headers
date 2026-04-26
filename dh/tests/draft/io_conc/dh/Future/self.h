#ifndef Future_self__included
#define Future_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../Sched/base.h"

/*========== Macros and Declarations ========================================*/

#define Future$$(_T...) __type__Future$$(_T)
#define Future$(_T...) __alias__Future$(_T)
#define T_decl_Future$(_T...) __stmt__T_decl_Future$(_T)
#define T_impl_Future$(_T...) __stmt__T_impl_Future$(_T)
#define T_use_Future$(_T...) __stmt__T_use_Future$(_T)
struct Future$raw {
    T_embed$(struct {
        var_(any_future, O$P$FutureAny);
        debug_only(var_(type, TypeInfo);)
    });
    var_(result_, V$raw) $flexible;
};
$attr($inline_always)
$static fn_((Future_result(const Future$raw* self, TypeInfo type))(u_P_const$raw));
#define T_use_Future_result$(_T...) __stmt__T_use_Future_result$(_T)
$attr($inline_always)
$static fn_((Future_resultMut(Future$raw* self, TypeInfo type))(u_P$raw));
#define T_use_Future_resultMut$(_T...) __stmt__T_use_Future_resultMut$(_T)

$extern fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw));
#define T_use_Future_await$(_T...) __stmt__T_use_Future_await$(_T)
$extern fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw));
#define T_use_Future_cancel$(_T...) __stmt__T_use_Future_cancel$(_T)

T_alias$((u_Fields_Idx$Future)(enum_((u_Fields_Idx$Future $fits($packed))(
    u_Fields_Idx_any_future$Future = 0,
    u_Fields_Idx_type$Future = 1,
    u_Fields_Idx_result_$Future = 2,
    count$u_Fields_Idx$Future
))));
$static let_(u_Fields_type$Future, A$$(count$u_Fields_Idx$Future, TypeInfo)) = A_init({
    [u_Fields_Idx_any_future$Future] = typeInfo$(FieldType$(Future$raw, any_future)),
    [u_Fields_Idx_type$Future] = typeInfo$(pp_if_(debug_comp_enabled)(pp_then_(FieldType$(Future$raw, type)), pp_else_(Void))),
    [u_Fields_Idx_result_$Future] = typeInfo$(FieldType$(Future$raw, result_)),
});

/*========== Macro and Definitions ==========================================*/

#define __type__Future$$(_T...) \
    union { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$FutureAny); \
                debug_only(var_(type, TypeInfo);) \
            }); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, Future$raw) $flexible; \
    }
#define __alias__Future$(_T...) tpl$(Future, _T)
#define __stmt__T_decl_Future$(_T...) \
    T_alias$((Future$(_T))(union Future$(_T)))
#define __stmt__T_impl_Future$(_T...) \
    union Future$(_T) { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$FutureAny); \
                debug_only(var_(type, TypeInfo);) \
            }); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, Future$raw) $flexible; \
    }
#define __stmt__T_use_Future$(_T...) \
    T_decl_Future$(_T); \
    T_impl_Future$(_T)

#include "dh/meta.h"

fn_((Future_result(const Future$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(u_Fields_type$Future));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_result_$Future);
};
fn_((Future_resultMut(Future$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(u_Fields_type$Future));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_result_$Future);
};

#define __stmt__T_use_Future_result$(_T...) /* clang-format off */  \
    $attr($inline_always) \
    $static fn_((tpl$(Future_result, _T)(const Future$(_T)* self))(const _T*)) { \
        return u_castP$((_T)(Future_result(self->as_raw, typeInfo$(_T)))); \
    } /* clang-format on */
#define __stmt__T_use_Future_resultMut$(_T...) /* clang-format off */\
    $attr($inline_always) \
    $static fn_((tpl$(Future_resultMut, _T)(Future$(_T)* self))(_T*)) { \
        return u_castP$((_T)(Future_resultMut(self->as_raw, typeInfo$(_T)))); \
    } /* clang-format on */

#define __stmt__T_use_Future_await$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(Future_await, _T)(Future$(_T)* self, Sched sched))(_T)) { \
        return u_castV$((_T)(Future_await(self->as_raw, sched, u_retV$(_T)))); \
    }/* clang-format on */
#define __stmt__T_use_Future_cancel$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(Future_cancel, _T)(Future$(_T)* self, Sched sched))(_T)) { \
        return u_castV$((_T)(Future_cancel(self->as_raw, sched, u_retV$(_T)))); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Future_self__included */
