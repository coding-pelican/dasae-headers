#pragma once
#ifndef clsr__included
#define clsr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((clsr_kind(const Clsr$raw* self))(Clsr_Kind));
#define T_use_clsr_kind$(_$T...) __stmt__T_use_clsr_kind$(_$T)
$attr($inline_always)
$static fn_((clsr_invokeToStep(Clsr$raw* self, TypeInfo type))(O$u_P_const$raw));
#define T_use_clsr_invokeToStep$(_$T...) __stmt__T_use_clsr_invokeToStep$(_$T)
$attr($inline_always)
$static fn_((clsr_invokeToComplete(Clsr$raw* self, TypeInfo type))(u_P_const$raw));
#define T_use_clsr_invokeToComplete$(_$T...) __stmt__T_use_clsr_invokeToComplete$(_$T)

/*========== Macros and Definitions =========================================*/

#include "u-meta.h"

#if on_analysis_active_only || on_comptime
fn_((clsr_kind(P_const$$(Clsr$raw) self))(Clsr_Kind)) {
    claim_assert_nonnull(self);
    claim_assert(self->kind != Clsr_Kind_undefined);
    return self->kind;
};

fn_((clsr_invokeToStep(Clsr$raw* self, TypeInfo type))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    let ret = P_meta((type)(as$(P$raw)(invoke_(self)->ret_))).as_const;
    if (clsr_kind(self) == Clsr_Kind_co) {
        let frame = local_({
            let fields = typeInfosFrom(type, typeInfo$(Co_Frame$raw));
            let record_type = u_typeInfoRecord(fields);
            let record = P_meta((record_type)(as$(P$raw)(self->ctx_))).as_const;
            local_return_(u_castP$((const Co_Frame$raw*)(u_fieldPtr(record, fields, 1))));
        });
        if (frame->ctx_->ctrl.state != Co_State_ready) return_none();
    }
    return_some(ret);
} $unscoped(fn);

fn_((clsr_invokeToComplete(Clsr$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self);
    var_(step, O$u_P_const$raw) = none();
    while_none(step = clsr_invokeToStep(self, type)) $do_nothing;
    return unwrap_(step);
};
#endif /* on_analysis_active_only || on_comptime */

#define __stmt__T_use_clsr_kind$(_$T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(clsr_kind, _$T)(Clsr$(_$T)* self))(Clsr_Kind)) { \
        return clsr_kind(self->as_raw); \
    } /* clang-format on */
T_use$((Void)(clsr_kind));

#define __stmt__T_use_clsr_invokeToStep$(_$T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(clsr_invokeToStep, _$T)(Clsr$(_$T)* self))(O$(P_const$(_$T))) $scope) { \
        if_some((clsr_invokeToStep(self->as_raw, typeInfo$(_$T)))(ret)) { \
            return_some(u_castP$((const _$T*)(ret))); \
        } else_none { \
            return_none(); \
        } \
        claim_unreachable; \
    } $unscoped(fn) /* clang-format on */

#define __stmt__T_use_clsr_invokeToComplete$(_$T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(clsr_invokeToComplete, _$T)(Clsr$(_$T)* self))(const _$T*)) { \
        return u_castP$((const _$T*)(clsr_invokeToComplete(self->as_raw, typeInfo$(_$T)))); \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* clsr__included */
