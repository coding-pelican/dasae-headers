/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    OnceLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_OnceLock
 *
 * @brief   Once-initialized storage for thread management
 */
#pragma once
#ifndef thrd_OnceLock__included
#define thrd_OnceLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Once.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_OnceLock$raw {
    var_(once, thrd_Once);
    var_(val_type, debug_TypeInfo);
    var_(val_, V$raw) $flexible;
} thrd_OnceLock$raw;
T_use$((thrd_OnceLock$raw)(u_V));
#define thrd_OnceLock$(_T...) __alias__thrd_OnceLock$(_T)
#define thrd_OnceLock$$(_T...) __anon__thrd_OnceLock$$(_T)
#define T_decl_thrd_OnceLock$(_T...) __gen__T_decl_thrd_OnceLock$(_T)
#define T_impl_thrd_OnceLock$(_T...) __gen__T_impl_thrd_OnceLock$(_T)
#define T_use_thrd_OnceLock$(_T...) __gen__T_use_thrd_OnceLock$(_T)

#define thrd_OnceLock_init_static$(_T...) ____thrd_OnceLock_init_static$(_T)
#define thrd_OnceLock_init_static(_type /*: TypeInfo*/...) ____thrd_OnceLock_init_static(_type)
$extern fn_((thrd_OnceLock_init(TypeInfo val_type, u_V$thrd_OnceLock$raw ret_mem))(u_V$thrd_OnceLock$raw));
#define T_use_thrd_OnceLock_init$(_T...) __gen__T_use_thrd_OnceLock_init$(_T)
$extern fn_((thrd_OnceLock_fini(thrd_OnceLock$raw* self))(void));
#define T_use_thrd_OnceLock_fini$(_T...) __gen__T_use_thrd_OnceLock_fini$(_T)

$extern fn_((thrd_OnceLock_isSet(const thrd_OnceLock$raw* self))(bool));
#define T_use_thrd_OnceLock_isSet$(_T...) __gen__T_use_thrd_OnceLock_isSet$(_T)
$extern fn_((thrd_OnceLock_trySet(thrd_OnceLock$raw* self, u_V$raw val))(bool));
#define T_use_thrd_OnceLock_trySet$(_T...) __gen__T_use_thrd_OnceLock_trySet$(_T)
$extern fn_((thrd_OnceLock_wait(thrd_OnceLock$raw* self))(void));
#define T_use_thrd_OnceLock_wait$(_T...) __gen__T_use_thrd_OnceLock_wait$(_T)
$extern fn_((thrd_OnceLock_get(thrd_OnceLock$raw* self, u_V$raw ret_mem))(u_V$raw));
#define T_use_thrd_OnceLock_get$(_T...) __gen__T_use_thrd_OnceLock_get$(_T)

T_alias$((u_Fields_Idx$thrd_OnceLock)(enum_((u_Fields_Idx$thrd_OnceLock $fits($packed))(
    u_Fields_Idx_once$thrd_OnceLock = 0,
    u_Fields_Idx_val_type$thrd_OnceLock,
    u_Fields_Idx_val_$thrd_OnceLock,
    count$u_Fields_Idx$thrd_OnceLock
))));
$static let_(u_Fields_type$thrd_OnceLock, A$$(count$u_Fields_Idx$thrd_OnceLock, TypeInfo)) = A_init({
    [u_Fields_Idx_once$thrd_OnceLock] = typeInfo$(FieldType$(thrd_OnceLock$raw, once)),
    [u_Fields_Idx_val_type$thrd_OnceLock] = typeInfo$(FieldType$(thrd_OnceLock$raw, val_type)),
    [u_Fields_Idx_val_$thrd_OnceLock] = typeInfo$(FieldType$(thrd_OnceLock$raw, val_)),
});

#define T_use_thrd_OnceLock_val$(_T...) __gen__T_use_thrd_OnceLock_val$(_T)
$attr($inline_always)
$static fn_((thrd_OnceLock_val(const thrd_OnceLock$raw* self, TypeInfo val_type))(u_P_const$raw));
#define T_use_thrd_OnceLock_valMut$(_T...) __gen__T_use_thrd_OnceLock_valMut$(_T)
$attr($inline_always)
$static fn_((thrd_OnceLock_valMut(thrd_OnceLock$raw* self, TypeInfo val_type))(u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __alias__thrd_OnceLock$(_T...) tpl$(thrd_OnceLock, _T)
#define __anon__thrd_OnceLock$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(once, thrd_Once); \
        var_(val_type, debug_TypeInfo); \
        T_embed$(union { \
            var_(val, _T); \
            var_(val_, _T) $like_ref; \
        }); \
    }); \
    var_(as_raw, thrd_OnceLock$raw) $flexible; \
})
#define __gen__T_decl_thrd_OnceLock$(_T...) \
    $maybe_unused typedef union thrd_OnceLock$(_T) thrd_OnceLock$(_T);
#define __gen__T_impl_thrd_OnceLock$(_T...) \
    union thrd_OnceLock$(_T) { \
        T_embed$(struct { \
            var_(once, thrd_Once); \
            var_(val_type, debug_TypeInfo); \
            T_embed$(union { \
                var_(val, _T); \
                var_(val_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, thrd_OnceLock$raw) $flexible; \
    }
#define __gen__T_use_thrd_OnceLock$(_T...) \
    T_decl_thrd_OnceLock$(_T); \
    T_impl_thrd_OnceLock$(_T)

#define ____thrd_OnceLock_init_static$(_T...) l$((thrd_OnceLock$(_T)){ \
    .once = thrd_Once_init_static(), \
    .val_type = $typing(typeInfo$(_T)), \
    .val = cleared(), \
})
#define ____thrd_OnceLock_init_static(_type /*: TypeInfo*/...) { \
    .once = thrd_Once_init_static(), \
    .val_type = $typing(_type), \
    .val_ = cleared(), \
}

/* clang-format off */
#define __gen__T_use_thrd_OnceLock_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_init, _T)(void))(thrd_OnceLock$(_T))) { \
        return type$((thrd_OnceLock$(_T))(thrd_OnceLock_init(typeInfo$(_T), u_asV$((u_V$thrd_OnceLock$raw)(u_retV$(thrd_OnceLock$(_T))))))); \
    }
#define __gen__T_use_thrd_OnceLock_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_fini, _T)(P$$(thrd_OnceLock$(_T)) self))(void)) { \
        return thrd_OnceLock_fini(self->as_raw); \
    }
#define __gen__T_use_thrd_OnceLock_isSet$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_isSet, _T)(P_const$$(thrd_OnceLock$(_T)) self))(bool)) { \
        return thrd_OnceLock_isSet(self->as_raw); \
    }
#define __gen__T_use_thrd_OnceLock_trySet$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_trySet, _T)(P$$(thrd_OnceLock$(_T)) self, _T val))(bool)) { \
        return thrd_OnceLock_trySet(self->as_raw, u_anyV(val)); \
    }
#define __gen__T_use_thrd_OnceLock_wait$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_wait, _T)(P$$(thrd_OnceLock$(_T)) self))(void)) { \
        return thrd_OnceLock_wait(self->as_raw); \
    }
#define __gen__T_use_thrd_OnceLock_get$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_get, _T)(P$$(thrd_OnceLock$(_T)) self))(_T)) { \
        return u_castV$((_T)(thrd_OnceLock_get(self->as_raw, u_retV$(_T)))); \
    }
#define __gen__T_use_thrd_OnceLock_val$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_val, _T)(P_const$$(thrd_OnceLock$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(thrd_OnceLock_val(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_OnceLock_valMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_OnceLock_valMut, _T)(P$$(thrd_OnceLock$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_OnceLock_valMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#include "../meta.h"

#if on_analysis_active_only || on_comptime
fn_((thrd_OnceLock_val(const thrd_OnceLock$raw* self, TypeInfo val_type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_OnceLock)(
        (.val[u_Fields_Idx_val_$thrd_OnceLock])(val_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_val_$thrd_OnceLock);
};
fn_((thrd_OnceLock_valMut(thrd_OnceLock$raw* self, TypeInfo val_type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_OnceLock)(
        (.val[u_Fields_Idx_val_$thrd_OnceLock])(val_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_val_$thrd_OnceLock);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_OnceLock__included */
