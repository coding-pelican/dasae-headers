/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrPDeq.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-09 (date of creation)
 * @updated 2025-12-09 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  ArrPDeq
 *
 * @brief   Dynamic array priority dequeue implementation (min-max heap)
 * @details This header provides a priority dequeue implementation with dynamic memory allocation.
 *          The priority dequeue maintains a min-max heap structure where both the minimum and
 *          maximum elements can be efficiently accessed. Supports operations for initialization,
 *          modification, and element access. Enqueuing/dequeuing items is O(log n).
 */
#ifndef ArrPDeq__included
#define ArrPDeq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "cmp.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

typedef cmp_OrdCtxFn ArrPDeq_OrdFn;
$extern fn_((ArrPDeq_OrdFn_default(cmp_MathType type))(ArrPDeq_OrdFn));

typedef struct ArrPDeq_Ctx {
    var_(inner, u_P_const$raw);
    var_(ordFn, ArrPDeq_OrdFn);
} ArrPDeq_Ctx;
T_use_P$(ArrPDeq_Ctx);
$extern fn_((ArrPDeq_Ctx_default(cmp_MathType type))(ArrPDeq_Ctx));

/* ArrPDeq Anonymous */
#define ArrPDeq$$(_T...) __comp_anon__ArrPDeq$$(_T)
/* ArrPDeq Alias */
#define ArrPDeq$(_T...) __comp_alias__ArrPDeq$(_T)
/* ArrPDeq Template */
#define T_decl_ArrPDeq$(_T...) __comp_gen__T_decl_ArrPDeq$(_T)
#define T_impl_ArrPDeq$(_T...) __comp_gen__T_impl_ArrPDeq$(_T)
#define T_use_ArrPDeq$(_T...) __comp_gen__T_use_ArrPDeq$(_T)

/* ArrPDeq Raw Structure */
typedef struct ArrPDeq {
    var_(items, S$raw);
    var_(cap, usize);
    var_(ctx, P_const$ArrPDeq_Ctx);
    debug_only(var_(type, TypeInfo);)
} ArrPDeq;
T_use$((ArrPDeq)(O, E));
T_use_E$($set(mem_Err)(ArrPDeq));

/* --- Function Prototypes --- */

$extern fn_((ArrPDeq_empty(TypeInfo type, P_const$ArrPDeq_Ctx ctx))(ArrPDeq));
$extern fn_((ArrPDeq_fixed(u_S$raw buf, P_const$ArrPDeq_Ctx ctx))(ArrPDeq));
$attr($must_check)
$extern fn_((ArrPDeq_init(TypeInfo type, mem_Allocator gpa, usize cap, P_const$ArrPDeq_Ctx ctx))(mem_Err$ArrPDeq));
$extern fn_((ArrPDeq_fini(ArrPDeq* self, TypeInfo type, mem_Allocator gpa))(void));

$extern fn_((ArrPDeq_len(ArrPDeq self))(usize));
$extern fn_((ArrPDeq_cap(ArrPDeq self))(usize));
$extern fn_((ArrPDeq_isEmpty(ArrPDeq self))(bool));
$extern fn_((ArrPDeq_isFull(ArrPDeq self))(bool));
$extern fn_((ArrPDeq_peekMin(ArrPDeq self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrPDeq_peekMinMut(ArrPDeq self, TypeInfo type))(O$u_P$raw));
$extern fn_((ArrPDeq_peekMax(ArrPDeq self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrPDeq_peekMaxMut(ArrPDeq self, TypeInfo type))(O$u_P$raw));
$extern fn_((ArrPDeq_at(ArrPDeq self, TypeInfo type, usize idx))(u_P_const$raw));
$extern fn_((ArrPDeq_atMut(ArrPDeq self, TypeInfo type, usize idx))(u_P$raw));
$extern fn_((ArrPDeq_items(ArrPDeq self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPDeq_itemsMut(ArrPDeq self, TypeInfo type))(u_S$raw));
$extern fn_((ArrPDeq_itemsCapped(ArrPDeq self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPDeq_itemsCappedMut(ArrPDeq self, TypeInfo type))(u_S$raw));
$extern fn_((ArrPDeq_itemsUnused(ArrPDeq self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPDeq_itemsUnusedMut(ArrPDeq self, TypeInfo type))(u_S$raw));

$attr($must_check)
$extern fn_((ArrPDeq_ensureCap(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPDeq_ensureCapPrecise(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPDeq_ensureUnusedCap(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void));
$extern fn_((ArrPDeq_shrinkRetainingCap(ArrPDeq* self, usize new_len))(void));
$extern fn_((ArrPDeq_shrinkAndFree(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void));
$extern fn_((ArrPDeq_clearRetainingCap(ArrPDeq* self))(void));
$extern fn_((ArrPDeq_clearAndFree(ArrPDeq* self, TypeInfo type, mem_Allocator gpa))(void));

$attr($must_check)
$extern fn_((ArrPDeq_enque(ArrPDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPDeq_enqueFixed(ArrPDeq* self, u_V$raw item))(mem_Err$void));
$extern fn_((ArrPDeq_enqueWithin(ArrPDeq* self, u_V$raw item))(void));
$attr($must_check)
$extern fn_((ArrPDeq_enqueS(ArrPDeq* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPDeq_enqueSFixed(ArrPDeq* self, u_S_const$raw items))(mem_Err$void));
$extern fn_((ArrPDeq_enqueSWithin(ArrPDeq* self, u_S_const$raw items))(void));

$extern fn_((ArrPDeq_dequeMin(ArrPDeq* self, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((ArrPDeq_dequeMax(ArrPDeq* self, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((ArrPDeq_removeAt(ArrPDeq* self, usize idx, u_V$raw ret_mem))(u_V$raw));

$attr($must_check)
$extern fn_((ArrPDeq_update(ArrPDeq* self, u_V$raw old_item, u_V$raw new_item))(mem_Err$void));

/* ArrPDeq_Iter Anonymous */
#define ArrPDeq_Iter$$(_T...) __comp_anon__ArrPDeq_Iter$$(_T)
/* ArrPDeq_Iter Alias */
#define ArrPDeq_Iter$(_T...) __comp_alias__ArrPDeq_Iter$(_T)
/* ArrPDeq_Iter Template */
#define T_decl_ArrPDeq_Iter$(_T...) __comp_gen__T_decl_ArrPDeq_Iter$(_T)
#define T_impl_ArrPDeq_Iter$(_T...) __comp_gen__T_impl_ArrPDeq_Iter$(_T)
#define T_use_ArrPDeq_Iter$(_T...) __comp_gen__T_use_ArrPDeq_Iter$(_T)

/* ArrPDeq_Iter Raw Structure */
typedef struct ArrPDeq_Iter {
    var_(deq, const ArrPDeq*);
    var_(idx, usize);
    debug_only(var_(type, TypeInfo);)
} ArrPDeq_Iter;
$extern fn_((ArrPDeq_iter(const ArrPDeq* self, TypeInfo type))(ArrPDeq_Iter));
$extern fn_((ArrPDeq_Iter_next(ArrPDeq_Iter* self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrPDeq_Iter_nextMut(ArrPDeq_Iter* self, TypeInfo type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrPDeq$$(_T...) \
    union { \
        struct { \
            var_(items, S$$(_T)); \
            var_(cap, usize); \
            var_(ctx, P_const$ArrPDeq_Ctx); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPDeq) $like_ref; \
    }
#define __comp_alias__ArrPDeq$(_T...) pp_join($, ArrPDeq, _T)
#define __comp_gen__T_decl_ArrPDeq$(_T...) \
    $maybe_unused typedef union ArrPDeq$(_T) ArrPDeq$(_T); \
    T_decl_O$(ArrPDeq$(_T)); \
    T_decl_E$(ArrPDeq$(_T)); \
    T_decl_E$($set(mem_Err)(ArrPDeq$(_T)))
#define __comp_gen__T_impl_ArrPDeq$(_T...) \
    union ArrPDeq$(_T) { \
        struct { \
            var_(items, S$(_T)); \
            var_(cap, usize); \
            var_(ctx, P_const$ArrPDeq_Ctx); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPDeq) $like_ref; \
    }; \
    T_impl_O$(ArrPDeq$(_T)); \
    T_impl_E$(ArrPDeq$(_T)); \
    T_impl_E$($set(mem_Err)(ArrPDeq$(_T)))
#define __comp_gen__T_use_ArrPDeq$(_T...) \
    T_decl_ArrPDeq$(_T); \
    T_impl_ArrPDeq$(_T)

/* clang-format off */
#define T_use_ArrPDeq_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_empty, _T)(P_const$ArrPDeq_Ctx ctx))(ArrPDeq$(_T))) { \
        return type$((ArrPDeq$(_T))(ArrPDeq_empty(typeInfo$(_T), ctx))); \
    }
#define T_use_ArrPDeq_fixed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_fixed, _T)(S$(_T) buf, P_const$ArrPDeq_Ctx ctx))(ArrPDeq$(_T))) { \
        return type$((ArrPDeq$(_T))(ArrPDeq_fixed(u_anyS(buf), ctx))); \
    }
#define T_use_ArrPDeq_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_init, _T)(mem_Allocator gpa, usize cap, P_const$ArrPDeq_Ctx ctx))(E$($set(mem_Err)(ArrPDeq$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrPDeq_init(typeInfo$(_T), gpa, cap, ctx)))); \
    } $unscoped_(fn)
#define T_use_ArrPDeq_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_fini, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrPDeq_fini(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrPDeq_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_len, _T)(ArrPDeq$(_T) self))(usize)) { \
        return ArrPDeq_len(*self.as_raw); \
    }
#define T_use_ArrPDeq_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_cap, _T)(ArrPDeq$(_T) self))(usize)) { \
        return ArrPDeq_cap(*self.as_raw); \
    }
#define T_use_ArrPDeq_peekMin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_peekMin, _T)(ArrPDeq$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrPDeq_peekMin(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_peekMinMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_peekMinMut, _T)(ArrPDeq$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrPDeq_peekMinMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_peekMax$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_peekMax, _T)(ArrPDeq$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrPDeq_peekMax(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_peekMaxMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_peekMaxMut, _T)(ArrPDeq$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrPDeq_peekMaxMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_at$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_at, _T)(ArrPDeq$(_T) self, usize idx))(const _T*)) { \
        return u_castP$((const _T*)(ArrPDeq_at(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrPDeq_atMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_atMut, _T)(ArrPDeq$(_T) self, usize idx))(_T*)) { \
        return u_castP$((_T*)(ArrPDeq_atMut(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrPDeq_items$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_items, _T)(ArrPDeq$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPDeq_items(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_itemsMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_itemsMut, _T)(ArrPDeq$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPDeq_itemsMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_itemsCapped$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_itemsCapped, _T)(ArrPDeq$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPDeq_itemsCapped(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_itemsCappedMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_itemsCappedMut, _T)(ArrPDeq$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPDeq_itemsCappedMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_itemsUnused$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_itemsUnused, _T)(ArrPDeq$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPDeq_itemsUnused(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_itemsUnusedMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_itemsUnusedMut, _T)(ArrPDeq$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPDeq_itemsUnusedMut(*self.as_raw, typeInfo$(_T)))); \
    }

#define T_use_ArrPDeq_ensureCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_ensureCap, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrPDeq_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrPDeq_ensureCapPrecise$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_ensureCapPrecise, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrPDeq_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrPDeq_ensureUnusedCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_ensureUnusedCap, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrPDeq_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrPDeq_shrinkRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_shrinkRetainingCap, _T)(P$$(ArrPDeq$(_T)) self, usize new_len))(void)) { \
        return ArrPDeq_shrinkRetainingCap(self->as_raw, new_len); \
    }
#define T_use_ArrPDeq_shrinkAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_shrinkAndFree, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, usize new_len))(void)) { \
        return ArrPDeq_shrinkAndFree(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrPDeq_clearRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_clearRetainingCap, _T)(P$$(ArrPDeq$(_T)) self))(void)) { \
        return ArrPDeq_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrPDeq_clearAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_clearAndFree, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrPDeq_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrPDeq_enque$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_enque, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrPDeq_enque(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrPDeq_enqueFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_enqueFixed, _T)(P$$(ArrPDeq$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrPDeq_enqueFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrPDeq_enqueWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_enqueWithin, _T)(P$$(ArrPDeq$(_T)) self, _T item))(void)) { \
        return ArrPDeq_enqueWithin(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrPDeq_enqueS$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_enqueS, _T)(P$$(ArrPDeq$(_T)) self, mem_Allocator gpa, S_const$(_T) items))(mem_Err$void)) { \
        return ArrPDeq_enqueS(self->as_raw, gpa, u_anyS(items)); \
    }
#define T_use_ArrPDeq_enqueSFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_enqueSFixed, _T)(P$$(ArrPDeq$(_T)) self, S_const$(_T) items))(mem_Err$void)) { \
        return ArrPDeq_enqueSFixed(self->as_raw, u_anyS(items)); \
    }
#define T_use_ArrPDeq_enqueSWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_enqueSWithin, _T)(P$$(ArrPDeq$(_T)) self, S_const$(_T) items))(void)) { \
        return ArrPDeq_enqueSWithin(self->as_raw, u_anyS(items)); \
    }

#define T_use_ArrPDeq_dequeMin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_dequeMin, _T)(P$$(ArrPDeq$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrPDeq_dequeMin(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrPDeq_dequeMax$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_dequeMax, _T)(P$$(ArrPDeq$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrPDeq_dequeMax(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrPDeq_removeAt$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_removeAt, _T)(P$$(ArrPDeq$(_T)) self, usize idx))(_T)) { \
        return u_cast((_T)(ArrPDeq_removeAt(self->as_raw, idx, u_retV$(_T)))); \
    }

#define T_use_ArrPDeq_update$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPDeq_update, _T)(P$$(ArrPDeq$(_T)) self, _T old_item, _T new_item))(mem_Err$void)) { \
        return ArrPDeq_update(self->as_raw, u_anyV(old_item), u_anyV(new_item)); \
    }
/* clang-format on */

#define __comp_anon__ArrPDeq_Iter$$(_T...) \
    union { \
        struct { \
            var_(deq, P_const$$(ArrPDeq$$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPDeq_Iter) $like_ref; \
    }
#define __comp_alias__ArrPDeq_Iter$(_T...) pp_join($, ArrPDeq_Iter, _T)
#define __comp_gen__T_decl_ArrPDeq_Iter$(_T...) \
    $maybe_unused typedef union ArrPDeq_Iter$(_T) ArrPDeq_Iter$(_T)
#define __comp_gen__T_impl_ArrPDeq_Iter$(_T...) \
    union ArrPDeq_Iter$(_T) { \
        struct { \
            var_(deq, P_const$$(ArrPDeq$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPDeq_Iter) $like_ref; \
    }
#define __comp_gen__T_use_ArrPDeq_Iter$(_T...) \
    T_decl_ArrPDeq_Iter$(_T); \
    T_impl_ArrPDeq_Iter$(_T)

/* clang-format off */
#define T_use_ArrPDeq_iter$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_iter, _T)(P_const$$(ArrPDeq$(_T)) self))(ArrPDeq_Iter$(_T))) { \
        return type$((ArrPDeq_Iter$(_T))(ArrPDeq_iter(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_Iter_next$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_Iter_next, _T)(P$$(ArrPDeq_Iter$(_T)) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrPDeq_Iter_next(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPDeq_Iter_nextMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPDeq_Iter_nextMut, _T)(P$$(ArrPDeq_Iter$(_T)) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrPDeq_Iter_nextMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrPDeq__included */
