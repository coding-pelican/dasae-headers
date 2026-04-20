#ifndef Future__included
#define Future__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

#define Future$$(_T...) __type__Future$$(_T)
#define Future$(_T...) __alias__Future$(_T)
#define T_decl_Future$(_T...) __stmt__T_decl_Future$(_T)
#define T_impl_Future$(_T...) __stmt__T_impl_Future$(_T)
#define T_use_Future$(_T...) __stmt__T_use_Future$(_T)

struct Future$raw {
    T_embed$(struct {
        var_(any_future, O$P$Future$raw);
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

/*========== Macro and Definitions ==========================================*/

#define __type__Future$$(_T...) \
    union { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$Future$raw); \
                debug_only(var_(type, TypeInfo);) \
            }); \
            var_(result, _T); \
        }); \
        var_(as_raw, Future$raw) $like_ref; \
    }
#define __alias__Future$(_T...) tpl_id(Future, _T)
#define __stmt__T_decl_Future$(_T...) \
    T_alias$((Future$(_T))(union Future$(_T)))
#define __stmt__T_impl_Future$(_T...) \
    union Future$(_T) { \
        T_embed$(struct { \
            T_embed$(struct { \
                var_(any_future, O$P$Future$raw); \
                debug_only(var_(type, TypeInfo);) \
            }); \
            var_(result, _T); \
        }); \
        var_(as_raw, Future$raw) $like_ref; \
    }
#define __stmt__T_use_Future$(_T...) \
    T_decl_Future$(_T); \
    T_impl_Future$(_T)

#include "dh/meta.h"

fn_((Future_result(const Future$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, 1);
};

fn_((Future_resultMut(Future$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, 1);
};


#define __stmt__T_use_Future_await$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id(Future_await, _T)(Future$(_T)* self, Sched sched))(_T)) /* clang-format on */
#define __stmt__T_use_Future_cancel$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl_id(Future_cancel, _T)(Future$(_T)* self, Sched sched))(_T)) /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Future__included */
