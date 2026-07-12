/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    LazeLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_LazeLock
 *
 * @brief   Lazily initialized storage for thread management
 */
#pragma once
#ifndef thrd_LazeLock__included
#define thrd_LazeLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Once.h"
#include "../clsr.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_LazeLock$raw {
    var_(once, thrd_Once);
    var_(val_type, debug_TypeInfo);
    var_(val_, V$raw) $flexible;
} thrd_LazeLock$raw;
T_use$((thrd_LazeLock$raw)(u_V));
#define thrd_LazeLock$(_T...) __alias__thrd_LazeLock$(_T)
#define thrd_LazeLock$$(_T...) __anon__thrd_LazeLock$$(_T)
#define T_decl_thrd_LazeLock$(_T...) __gen__T_decl_thrd_LazeLock$(_T)
#define T_impl_thrd_LazeLock$(_T...) __gen__T_impl_thrd_LazeLock$(_T)
#define T_use_thrd_LazeLock$(_T...) __gen__T_use_thrd_LazeLock$(_T)

#define thrd_LazeLock_init_static$(_T...) ____thrd_LazeLock_init_static$(_T)
#define thrd_LazeLock_init_static(_type /*: TypeInfo*/...) ____thrd_LazeLock_init_static(_type)
#define thrd_LazeLock_init$(_T...) ____thrd_LazeLock_init$(_T)
$extern fn_((thrd_LazeLock_init(TypeInfo val_type, u_V$thrd_LazeLock$raw ret_mem))(u_V$thrd_LazeLock$raw));
#define T_use_thrd_LazeLock_init$(_T...) __gen__T_use_thrd_LazeLock_init$(_T)
$extern fn_((thrd_LazeLock_fini(thrd_LazeLock$raw* self))(void));
#define T_use_thrd_LazeLock_fini$(_T...) __gen__T_use_thrd_LazeLock_fini$(_T)

$extern fn_((thrd_LazeLock_isSet(const thrd_LazeLock$raw* self))(bool));
#define T_use_thrd_LazeLock_isSet$(_T...) __gen__T_use_thrd_LazeLock_isSet$(_T)
$extern fn_((thrd_LazeLock_wait(thrd_LazeLock$raw* self))(void));
#define T_use_thrd_LazeLock_wait$(_T...) __gen__T_use_thrd_LazeLock_wait$(_T)
$extern fn_((thrd_LazeLock_get(thrd_LazeLock$raw* self, TypeInfo val_type))(u_P$raw));
#define T_use_thrd_LazeLock_get$(_T...) __gen__T_use_thrd_LazeLock_get$(_T)
$extern fn_((thrd_LazeLock_getOrInit(thrd_LazeLock$raw* self, TypeInfo val_type, Clsr$raw* init))(u_P$raw));
#define T_use_thrd_LazeLock_getOrInit$(_T...) __gen__T_use_thrd_LazeLock_getOrInit$(_T)

T_alias$((u_Fields_Idx$thrd_LazeLock)(enum_((u_Fields_Idx$thrd_LazeLock $fits($packed))(
    u_Fields_Idx_once$thrd_LazeLock = 0,
    u_Fields_Idx_val_type$thrd_LazeLock,
    u_Fields_Idx_val_$thrd_LazeLock,
    count$u_Fields_Idx$thrd_LazeLock
))));
$static let_(u_Fields_type$thrd_LazeLock, A$$(count$u_Fields_Idx$thrd_LazeLock, TypeInfo)) = A_init({
    [u_Fields_Idx_once$thrd_LazeLock] = typeInfo$(FieldType$(thrd_LazeLock$raw, once)),
    [u_Fields_Idx_val_type$thrd_LazeLock] = typeInfo$(FieldType$(thrd_LazeLock$raw, val_type)),
    [u_Fields_Idx_val_$thrd_LazeLock] = typeInfo$(FieldType$(thrd_LazeLock$raw, val_)),
});

#define T_use_thrd_LazeLock_val$(_T...) __gen__T_use_thrd_LazeLock_val$(_T)
$attr($inline_always)
$static fn_((thrd_LazeLock_val(const thrd_LazeLock$raw* self, TypeInfo val_type))(u_P_const$raw));
#define T_use_thrd_LazeLock_valMut$(_T...) __gen__T_use_thrd_LazeLock_valMut$(_T)
$attr($inline_always)
$static fn_((thrd_LazeLock_valMut(thrd_LazeLock$raw* self, TypeInfo val_type))(u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __alias__thrd_LazeLock$(_T...) tpl$(thrd_LazeLock, _T)
#define __anon__thrd_LazeLock$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(once, thrd_Once); \
        var_(val_type, debug_TypeInfo); \
        T_embed$(union { \
            var_(val, _T); \
            var_(val_, _T) $like_ref; \
        }); \
    }); \
    var_(as_raw, thrd_LazeLock$raw) $flexible; \
})
#define __gen__T_decl_thrd_LazeLock$(_T...) \
    $maybe_unused typedef union thrd_LazeLock$(_T) thrd_LazeLock$(_T);
#define __gen__T_impl_thrd_LazeLock$(_T...) \
    union thrd_LazeLock$(_T) { \
        T_embed$(struct { \
            var_(once, thrd_Once); \
            var_(val_type, debug_TypeInfo); \
            T_embed$(union { \
                var_(val, _T); \
                var_(val_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, thrd_LazeLock$raw) $flexible; \
    }
#define __gen__T_use_thrd_LazeLock$(_T...) \
    T_decl_thrd_LazeLock$(_T); \
    T_impl_thrd_LazeLock$(_T)

#define ____thrd_LazeLock_init_static$(_T...) l$((thrd_LazeLock$(_T)){ \
    .once = thrd_Once_init_static(), \
    .val_type = $typing(typeInfo$(_T)), \
    .val = cleared(), \
})
#define ____thrd_LazeLock_init_static(_type /*: TypeInfo*/...) { \
    .once = thrd_Once_init_static(), \
    .val_type = $typing(_type), \
    .val_ = cleared(), \
}
#define ____thrd_LazeLock_init$(_T...) ____thrd_LazeLock_init_static$(_T)

/* clang-format off */
#define __gen__T_use_thrd_LazeLock_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_init, _T)(void))(thrd_LazeLock$(_T))) { \
        return type$((thrd_LazeLock$(_T))(thrd_LazeLock_init(typeInfo$(_T), u_asV$((u_V$thrd_LazeLock$raw)(u_retV$(thrd_LazeLock$(_T))))))); \
    }
#define __gen__T_use_thrd_LazeLock_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_fini, _T)(P$$(thrd_LazeLock$(_T)) self))(void)) { \
        return thrd_LazeLock_fini(self->as_raw); \
    }
#define __gen__T_use_thrd_LazeLock_isSet$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_isSet, _T)(P_const$$(thrd_LazeLock$(_T)) self))(bool)) { \
        return thrd_LazeLock_isSet(self->as_raw); \
    }
#define __gen__T_use_thrd_LazeLock_wait$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_wait, _T)(P$$(thrd_LazeLock$(_T)) self))(void)) { \
        return thrd_LazeLock_wait(self->as_raw); \
    }
#define __gen__T_use_thrd_LazeLock_get$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_get, _T)(P$$(thrd_LazeLock$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_LazeLock_get(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_LazeLock_getOrInit$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_getOrInit, _T)(P$$(thrd_LazeLock$(_T)) self, P$$(Clsr$(_T)) init))(_T*)) { \
        return u_castP$((_T*)(thrd_LazeLock_getOrInit(self->as_raw, typeInfo$(_T), init->as_raw))); \
    }
#define __gen__T_use_thrd_LazeLock_val$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_val, _T)(P_const$$(thrd_LazeLock$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(thrd_LazeLock_val(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_LazeLock_valMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_LazeLock_valMut, _T)(P$$(thrd_LazeLock$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_LazeLock_valMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#include "../meta.h"

#if on_analysis_active_only || on_comptime
fn_((thrd_LazeLock_val(const thrd_LazeLock$raw* self, TypeInfo val_type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_LazeLock)(
        (.val[u_Fields_Idx_val_$thrd_LazeLock])(val_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_val_$thrd_LazeLock);
};
fn_((thrd_LazeLock_valMut(thrd_LazeLock$raw* self, TypeInfo val_type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_LazeLock)(
        (.val[u_Fields_Idx_val_$thrd_LazeLock])(val_type)
    )));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_val_$thrd_LazeLock);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_LazeLock__included */
