/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Select.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-07-13 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Select
 *
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
#include "chan.h"
#include "../mem.h"
#include "../ArrList.h"
#include "../u-meta.h"

/*========== Macros and Declarations ========================================*/

T_alias$((thrd_Select_Op_VTbl)(struct thrd_Select_Op_VTbl));
T_alias$((thrd_Select_Op)(struct thrd_Select_Op {
    var_(ctx, P$raw);
    var_(data, u_P$raw);
    var_(result_type, TypeInfo);
    var_(vtbl, P_const$$(thrd_Select_Op_VTbl));
    var_(src, thrd_Wakeable);
}));
$attr($inline_always)
$static fn_((thrd_Select_Op_isValid(thrd_Select_Op self))(bool));
$attr($inline_always)
$static fn_((thrd_Select_Op_assertValid(
    P$raw ctx,
    u_P$raw data,
    TypeInfo result_type,
    P_const$$(thrd_Select_Op_VTbl) vtbl,
    thrd_Wakeable src
))(void));
$attr($inline_always)
$static fn_((thrd_Select_Op_ensureValid(thrd_Select_Op self))(thrd_Select_Op));

typedef struct thrd_Select_Arm$raw {
    var_(op, thrd_Select_Op);
    var_(link, thrd_wait_Link);
    var_(type, debug_TypeInfo);
    var_(commit_offset, usize);
    var_(result_, V$raw) $flexible;
} thrd_Select_Arm$raw;
T_use_P$(thrd_Select_Arm$raw);
T_use_O$(P$thrd_Select_Arm$raw);
T_use$((thrd_Select_Arm$raw)(u_V, S, u_S));
T_use$((u_V$thrd_Select_Arm$raw)(O));
T_use_E$($set(Sched_Cancelable)(P$thrd_Select_Arm$raw));
T_use_E$($set(Sched_TimedE)(P$thrd_Select_Arm$raw));
T_use_E$($set(Sched_Cancelable)(u_V$thrd_Select_Arm$raw));
T_use_E$($set(Sched_TimedE)(u_V$thrd_Select_Arm$raw));

#define thrd_Select_Arm$(_T...) __alias__thrd_Select_Arm$(_T)
#define thrd_Select_Arm$$(_T...) __anon__thrd_Select_Arm$$(_T)
#define T_decl_thrd_Select_Arm$(_T...) __gen__T_decl_thrd_Select_Arm$(_T)
#define T_impl_thrd_Select_Arm$(_T...) __gen__T_impl_thrd_Select_Arm$(_T)
#define T_use_thrd_Select_Arm$(_T...) __gen__T_use_thrd_Select_Arm$(_T)

T_alias$((u_Fields_Idx$thrd_Select_Arm)(enum_((u_Fields_Idx$thrd_Select_Arm $fits($packed))(
    u_Fields_Idx_op$thrd_Select_Arm = 0,
    u_Fields_Idx_link$thrd_Select_Arm,
    u_Fields_Idx_type$thrd_Select_Arm,
    u_Fields_Idx_commit_offset$thrd_Select_Arm,
    u_Fields_Idx_result_$thrd_Select_Arm,
    count$u_Fields_Idx$thrd_Select_Arm
))));
$static let_(u_Fields_type$thrd_Select_Arm, A$$(count$u_Fields_Idx$thrd_Select_Arm, TypeInfo)) = A_init({
    [u_Fields_Idx_op$thrd_Select_Arm] = typeInfo$(FieldType$(thrd_Select_Arm$raw, op)),
    [u_Fields_Idx_link$thrd_Select_Arm] = typeInfo$(FieldType$(thrd_Select_Arm$raw, link)),
    [u_Fields_Idx_type$thrd_Select_Arm] = typeInfo$(FieldType$(thrd_Select_Arm$raw, type)),
    [u_Fields_Idx_commit_offset$thrd_Select_Arm] = typeInfo$(FieldType$(thrd_Select_Arm$raw, commit_offset)),
    [u_Fields_Idx_result_$thrd_Select_Arm] = typeInfo$(FieldType$(thrd_Select_Arm$raw, result_)),
});
$attr($inline_always)
$static fn_((thrd_Select_Arm_typeInfo(TypeInfo result_type))(TypeInfo));
$attr($inline_always)
$static fn_((thrd_Select_Arm_result(const thrd_Select_Arm$raw* self, TypeInfo type))(u_P_const$raw));
#define T_use_thrd_Select_Arm_result$(_T...) __gen__T_use_thrd_Select_Arm_result$(_T)
$attr($inline_always)
$static fn_((thrd_Select_Arm_resultMut(thrd_Select_Arm$raw* self, TypeInfo type))(u_P$raw));
#define T_use_thrd_Select_Arm_resultMut$(_T...) __gen__T_use_thrd_Select_Arm_resultMut$(_T)

$extern fn_((thrd_Select_Arm_from(
    TypeInfo type, u_V$raw tag, thrd_Select_Op op, u_V$thrd_Select_Arm$raw ret_mem
))(u_V$thrd_Select_Arm$raw));
#define T_use_thrd_Select_Arm_from$(_T...) __gen__T_use_thrd_Select_Arm_from$(_T)
$extern fn_((thrd_Select_Arm_into(thrd_Select_Arm$raw* self, u_V$raw ret_mem))(u_V$raw));
#define T_use_thrd_Select_Arm_into$(_T...) __gen__T_use_thrd_Select_Arm_into$(_T)
$extern fn_((thrd_Select_Arm_take(thrd_Select_Arm$raw* self, u_V$raw ret_mem))(u_V$raw));
#define T_use_thrd_Select_Arm_take$(_T...) __gen__T_use_thrd_Select_Arm_take$(_T)

typedef struct thrd_Select {
    var_(arms, ArrList);
} thrd_Select;
T_use_E$($set(mem_E)(thrd_Select));

$extern fn_((thrd_Select_fixed(TypeInfo type, u_S$thrd_Select_Arm$raw arms))(thrd_Select));
#define T_use_thrd_Select_fixed$(_T...) __gen__T_use_thrd_Select_fixed$(_T)
$attr($must_check)
$extern fn_((thrd_Select_init(TypeInfo type, mem_Alctr gpa, usize cap))(mem_E$thrd_Select));
#define T_use_thrd_Select_init$(_T...) __gen__T_use_thrd_Select_init$(_T)
$extern fn_((thrd_Select_fini(thrd_Select* self, TypeInfo type, mem_Alctr gpa))(void));
#define T_use_thrd_Select_fini$(_T...) __gen__T_use_thrd_Select_fini$(_T)
$extern fn_((thrd_Select_from(TypeInfo type, u_S$thrd_Select_Arm$raw arms))(thrd_Select));
#define T_use_thrd_Select_from$(_T...) __gen__T_use_thrd_Select_from$(_T)

$attr($must_check)
$extern fn_((thrd_Select_watch(
    thrd_Select* self, mem_Alctr gpa, TypeInfo type, u_V$thrd_Select_Arm$raw arm
))(mem_E$void));
#define T_use_thrd_Select_watch$(_T...) __gen__T_use_thrd_Select_watch$(_T)
$attr($must_check)
$extern fn_((thrd_Select_watchFixed(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw arm
))(thrd_chan_CapE$void));
#define T_use_thrd_Select_watchFixed$(_T...) __gen__T_use_thrd_Select_watchFixed$(_T)
$extern fn_((thrd_Select_watchWithin(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw arm
))(void));
#define T_use_thrd_Select_watchWithin$(_T...) __gen__T_use_thrd_Select_watchWithin$(_T)
$attr($must_check)
$extern fn_((thrd_Select_poll(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw ret_mem
))(O$u_V$thrd_Select_Arm$raw));
#define T_use_thrd_Select_poll$(_T...) __gen__T_use_thrd_Select_poll$(_T)
$attr($must_check)
$extern fn_((thrd_Select_pollMut(
    thrd_Select* self, TypeInfo type
))(O$P$thrd_Select_Arm$raw));
#define T_use_thrd_Select_pollMut$(_T...) __gen__T_use_thrd_Select_pollMut$(_T)

$attr($must_check)
$extern fn_((thrd_Select_wait(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, u_V$thrd_Select_Arm$raw ret_mem
))(Sched_Cancelable$u_V$thrd_Select_Arm$raw));
#define T_use_thrd_Select_wait$(_T...) __gen__T_use_thrd_Select_wait$(_T)
$attr($must_check)
$extern fn_((thrd_Select_waitMut(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel
))(Sched_Cancelable$P$thrd_Select_Arm$raw));
#define T_use_thrd_Select_waitMut$(_T...) __gen__T_use_thrd_Select_waitMut$(_T)
$attr($must_check)
$extern fn_((thrd_Select_waitFor(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, time_Dur dur, u_V$thrd_Select_Arm$raw ret_mem
))(Sched_TimedE$u_V$thrd_Select_Arm$raw));
#define T_use_thrd_Select_waitFor$(_T...) __gen__T_use_thrd_Select_waitFor$(_T)
$attr($must_check)
$extern fn_((thrd_Select_waitMutFor(
    thrd_Select* self, TypeInfo type, thrd_CancelTok cancel, time_Dur dur
))(Sched_TimedE$P$thrd_Select_Arm$raw));
#define T_use_thrd_Select_waitMutFor$(_T...) __gen__T_use_thrd_Select_waitMutFor$(_T)
$extern fn_((thrd_Select_waitProtcd(
    thrd_Select* self, TypeInfo type, u_V$thrd_Select_Arm$raw ret_mem
))(u_V$thrd_Select_Arm$raw));
#define T_use_thrd_Select_waitProtcd$(_T...) __gen__T_use_thrd_Select_waitProtcd$(_T)
$extern fn_((thrd_Select_waitMutProtcd(
    thrd_Select* self, TypeInfo type
))(P$thrd_Select_Arm$raw));
#define T_use_thrd_Select_waitMutProtcd$(_T...) __gen__T_use_thrd_Select_waitMutProtcd$(_T)

/*========== Macros and Definitions =========================================*/

struct thrd_Select_Op_VTbl {
    fn_(((*pollFn)(P$raw ctx))(bool));
    fn_(((*commitFn)(P$raw ctx, u_P$raw data, u_P$raw result))(bool));
};

#if on_analysis_active_only || on_comptime
fn_((thrd_Select_Op_isValid(thrd_Select_Op self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.data.raw)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->pollFn)
        && isNonnull(self.vtbl->commitFn)
        && thrd_Wakeable_isValid(self.src);
};
fn_((thrd_Select_Op_assertValid(
    P$raw ctx,
    u_P$raw data,
    TypeInfo result_type,
    P_const$$(thrd_Select_Op_VTbl) vtbl,
    thrd_Wakeable src
))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(data.raw);
    let_ignore = result_type;
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->pollFn);
    claim_assert_nonnull(vtbl->commitFn);
    thrd_Wakeable_assertValid(src.ctx, src.vtbl);
};
fn_((thrd_Select_Op_ensureValid(thrd_Select_Op self))(thrd_Select_Op)) {
    return thrd_Select_Op_assertValid(self.ctx, self.data, self.result_type, self.vtbl, self.src), self;
};
#endif /* on_analysis_active_only || on_comptime */

/* clang-format off */
#define __alias__thrd_Select_Arm$(_T...) tpl$(thrd_Select_Arm, _T)
#define __anon__thrd_Select_Arm$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(op, thrd_Select_Op); \
        var_(link, thrd_wait_Link); \
        var_(type, debug_TypeInfo); \
        var_(commit_offset, usize); \
        T_embed$(union { \
            var_(result, _T); \
            var_(result_, _T) $like_ref; \
        }); \
    }); \
    var_(as_raw, thrd_Select_Arm$raw) $flexible; \
})
#define __gen__T_decl_thrd_Select_Arm$(_T...) \
    $maybe_unused typedef union thrd_Select_Arm$(_T) thrd_Select_Arm$(_T)
#define __gen__T_impl_thrd_Select_Arm$(_T...) \
    union thrd_Select_Arm$(_T) { \
        T_embed$(struct { \
            var_(op, thrd_Select_Op); \
            var_(link, thrd_wait_Link); \
            var_(type, debug_TypeInfo); \
            var_(commit_offset, usize); \
            T_embed$(union { \
                var_(result, _T); \
                var_(result_, _T) $like_ref; \
            }); \
        }); \
        var_(as_raw, thrd_Select_Arm$raw) $flexible; \
    }
#define __gen__T_use_thrd_Select_Arm$(_T...) \
    T_decl_thrd_Select_Arm$(_T); \
    T_impl_thrd_Select_Arm$(_T); \
    T_use$((thrd_Select_Arm$(_T))(P, S, O)); \
    T_use_O$(P$(thrd_Select_Arm$(_T))); \
    T_use_E$($set(Sched_Cancelable)(P$(thrd_Select_Arm$(_T)))); \
    T_use_E$($set(Sched_TimedE)(P$(thrd_Select_Arm$(_T)))); \
    T_use_E$($set(Sched_Cancelable)(thrd_Select_Arm$(_T))); \
    T_use_E$($set(Sched_TimedE)(thrd_Select_Arm$(_T)))

#define __gen__T_use_thrd_Select_Arm_result$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Arm_result, _T)(P_const$$(thrd_Select_Arm$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(thrd_Select_Arm_result(self->as_raw, typeInfo$(_T)))); \
    }
#define __gen__T_use_thrd_Select_Arm_resultMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Arm_resultMut, _T)(P$$(thrd_Select_Arm$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(thrd_Select_Arm_resultMut(self->as_raw, typeInfo$(_T)))); \
    }

#define __gen__T_use_thrd_Select_Arm_from$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Arm_from, _T)(enum_Tag$(_T) tag, thrd_Select_Op op))(thrd_Select_Arm$(_T))) { \
        return u_castV$((thrd_Select_Arm$(_T))(thrd_Select_Arm_from( \
            typeInfo$(_T), u_anyV(tag), op, u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        ))); \
    }
#define __gen__T_use_thrd_Select_Arm_into$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Arm_into, _T)(thrd_Select_Arm$(_T) self))(_T)) { \
        return u_castV$((_T)(thrd_Select_Arm_into(self.as_raw, u_retV$(_T)))); \
    }
#define __gen__T_use_thrd_Select_Arm_take$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_Arm_take, _T)(P$$(thrd_Select_Arm$(_T)) self))(_T)) { \
        claim_assert_nonnull(self); \
        return u_castV$((_T)(thrd_Select_Arm_take(self->as_raw, u_retV$(_T)))); \
    }

#define __gen__T_use_thrd_Select_fixed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_fixed, _T)(S$(thrd_Select_Arm$(_T)) arms))(thrd_Select)) { \
        return thrd_Select_fixed(typeInfo$(_T), u_asS$((u_S$thrd_Select_Arm$raw)(u_anyS(arms)))); \
    }
#define __gen__T_use_thrd_Select_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_init, _T)(mem_Alctr gpa, usize cap))(mem_E$thrd_Select)) { \
        return thrd_Select_init(typeInfo$(_T), gpa, cap); \
    }
#define __gen__T_use_thrd_Select_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_fini, _T)(thrd_Select* self, mem_Alctr gpa))(void)) { \
        return thrd_Select_fini(self, typeInfo$(_T), gpa); \
    }
#define __gen__T_use_thrd_Select_from$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_from, _T)(S$(thrd_Select_Arm$(_T)) arms))(thrd_Select)) { \
        return thrd_Select_from(typeInfo$(_T), u_asS$((u_S$thrd_Select_Arm$raw)(u_anyS(arms)))); \
    }

#define __gen__T_use_thrd_Select_watch$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_watch, _T)(thrd_Select* self, mem_Alctr gpa, enum_Tag$(_T) tag, thrd_Select_Op op))(mem_E$void)) { \
        return thrd_Select_watch(self, gpa, typeInfo$(_T), thrd_Select_Arm_from( \
            typeInfo$(_T), u_anyV(tag), op, u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )); \
    }
#define __gen__T_use_thrd_Select_watchFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_watchFixed, _T)(thrd_Select* self, enum_Tag$(_T) tag, thrd_Select_Op op))(thrd_chan_CapE$void)) { \
        return thrd_Select_watchFixed(self, typeInfo$(_T), thrd_Select_Arm_from( \
            typeInfo$(_T), u_anyV(tag), op, u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )); \
    }
#define __gen__T_use_thrd_Select_watchWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_watchWithin, _T)(thrd_Select* self, enum_Tag$(_T) tag, thrd_Select_Op op))(void)) { \
        return thrd_Select_watchWithin(self, typeInfo$(_T), thrd_Select_Arm_from( \
            typeInfo$(_T), u_anyV(tag), op, u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )); \
    }
#define __gen__T_use_thrd_Select_poll$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_poll, _T)(thrd_Select* self))(O$(thrd_Select_Arm$(_T))) $scope) { \
        return_(u_castO$((ReturnType)(thrd_Select_poll( \
            self, typeInfo$(_T), u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_pollMut$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_pollMut, _T)(thrd_Select* self))(O$(P$(thrd_Select_Arm$(_T)))) $scope) { \
        let raw = thrd_Select_pollMut(self, typeInfo$(_T)); \
        if (isNone(raw)) return_(none()); \
        return_(some(ptrAlignCast$((P$(thrd_Select_Arm$(_T)))(unwrap_(raw))))); \
    } $unscoped(fn)

#define __gen__T_use_thrd_Select_wait$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_wait, _T)( \
        thrd_Select* self, thrd_CancelTok cancel \
    ))(E$($set(Sched_Cancelable)(thrd_Select_Arm$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Select_wait( \
            self, typeInfo$(_T), cancel, u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_waitMut$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_waitMut, _T)( \
        thrd_Select* self, thrd_CancelTok cancel \
    ))(E$($set(Sched_Cancelable)(P$(thrd_Select_Arm$(_T))))) $scope) { \
        let raw = thrd_Select_waitMut(self, typeInfo$(_T), cancel); \
        if (isErr(raw)) return_(err(raw.payload.err)); \
        return_(ok(ptrAlignCast$((P$(thrd_Select_Arm$(_T)))(raw.payload.ok)))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_waitFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_waitFor, _T)( \
        thrd_Select* self, thrd_CancelTok cancel, time_Dur dur \
    ))(E$($set(Sched_TimedE)(thrd_Select_Arm$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(thrd_Select_waitFor( \
            self, typeInfo$(_T), cancel, dur, \
            u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        )))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_waitMutFor$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(thrd_Select_waitMutFor, _T)( \
        thrd_Select* self, thrd_CancelTok cancel, time_Dur dur \
    ))(E$($set(Sched_TimedE)(P$(thrd_Select_Arm$(_T))))) $scope) { \
        let raw = thrd_Select_waitMutFor(self, typeInfo$(_T), cancel, dur); \
        if (isErr(raw)) return_(err(raw.payload.err)); \
        return_(ok(ptrAlignCast$((P$(thrd_Select_Arm$(_T)))(raw.payload.ok)))); \
    } $unscoped(fn)
#define __gen__T_use_thrd_Select_waitProtcd$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_waitProtcd, _T)(thrd_Select* self))(thrd_Select_Arm$(_T))) { \
        return u_castV$((thrd_Select_Arm$(_T))(thrd_Select_waitProtcd( \
            self, typeInfo$(_T), u_asV$((u_V$thrd_Select_Arm$raw)(u_retV$(thrd_Select_Arm$(_T)))) \
        ))); \
    }
#define __gen__T_use_thrd_Select_waitMutProtcd$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(thrd_Select_waitMutProtcd, _T)(thrd_Select* self))(P$(thrd_Select_Arm$(_T)))) { \
        return ptrAlignCast$((P$(thrd_Select_Arm$(_T)))(thrd_Select_waitMutProtcd(self, typeInfo$(_T)))); \
    }
/* clang-format on */

#if on_analysis_active_only || on_comptime
fn_((thrd_Select_Arm_typeInfo(TypeInfo result_type))(TypeInfo)) {
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Arm)(
        (.val[u_Fields_Idx_result_$thrd_Select_Arm])(result_type)
    )));
    return u_typeInfoRecord(ty_fields);
};
fn_((thrd_Select_Arm_result(const thrd_Select_Arm$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Arm)(
        (.val[u_Fields_Idx_result_$thrd_Select_Arm])(type)
    )));
    let u_self = P_meta((thrd_Select_Arm_typeInfo(type))(ptrCast$((P_const$raw)(self))));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_result_$thrd_Select_Arm);
};
fn_((thrd_Select_Arm_resultMut(thrd_Select_Arm$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    let ty_fields = A_ref$((S_const$TypeInfo)with_((u_Fields_type$thrd_Select_Arm)(
        (.val[u_Fields_Idx_result_$thrd_Select_Arm])(type)
    )));
    let u_self = P_meta((thrd_Select_Arm_typeInfo(type))(ptrCast$((P$raw)(self))));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_result_$thrd_Select_Arm);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Select__included */
