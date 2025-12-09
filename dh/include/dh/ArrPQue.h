/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrPQue.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-12-09 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ArrPQue
 *
 * @brief   Dynamic array priority queue implementation (min-heap)
 * @details This header provides a priority queue implementation with dynamic memory allocation.
 *          The priority queue maintains a binary heap structure where the minimum element
 *          is always at the root. Supports operations for initialization, modification,
 *          and element access. Enqueuing/dequeuing items is O(log n).
 */
#ifndef ArrPQue__included
#define ArrPQue__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "cmp.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

typedef cmp_OrdCtxFn ArrPQue_OrdFn;
$extern fn_((ArrPQue_OrdFn_defaultAsc(cmp_MathType type))(ArrPQue_OrdFn));
$extern fn_((ArrPQue_OrdFn_defaultDesc(cmp_MathType type))(ArrPQue_OrdFn));

typedef struct ArrPQue_Ctx {
    var_(inner, u_P_const$raw);
    var_(ordFn, ArrPQue_OrdFn);
} ArrPQue_Ctx;
T_use_P$(ArrPQue_Ctx);
$extern fn_((ArrPQue_Ctx_defaultAsc(cmp_MathType type))(ArrPQue_Ctx));
$extern fn_((ArrPQue_Ctx_defaultDesc(cmp_MathType type))(ArrPQue_Ctx));

/* ArrPQue Anonymous */
#define ArrPQue$$(_T...) __comp_anon__ArrPQue$$(_T)
/* ArrPQue Alias */
#define ArrPQue$(_T...) __comp_alias__ArrPQue$(_T)
/* ArrPQue Template */
#define T_decl_ArrPQue$(_T...) __comp_gen__T_decl_ArrPQue$(_T)
#define T_impl_ArrPQue$(_T...) __comp_gen__T_impl_ArrPQue$(_T)
#define T_use_ArrPQue$(_T...) __comp_gen__T_use_ArrPQue$(_T)

/* ArrPQue Raw Structure */
typedef struct ArrPQue {
    var_(items, S$raw);
    var_(cap, usize);
    var_(ctx, P_const$ArrPQue_Ctx);
    debug_only(var_(type, TypeInfo);)
} ArrPQue;
T_use$((ArrPQue)(O, E));
T_use_E$($set(mem_Err)(ArrPQue));

/* --- Function Prototypes --- */

$extern fn_((ArrPQue_empty(TypeInfo type, P_const$ArrPQue_Ctx ctx))(ArrPQue));
$extern fn_((ArrPQue_fixed(u_S$raw buf, P_const$ArrPQue_Ctx ctx))(ArrPQue));
$attr($must_check)
$extern fn_((ArrPQue_init(TypeInfo type, mem_Allocator gpa, usize cap, P_const$ArrPQue_Ctx ctx))(mem_Err$ArrPQue));
$extern fn_((ArrPQue_fini(ArrPQue* self, TypeInfo type, mem_Allocator gpa))(void));

$extern fn_((ArrPQue_len(ArrPQue self))(usize));
$extern fn_((ArrPQue_cap(ArrPQue self))(usize));
$extern fn_((ArrPQue_isEmpty(ArrPQue self))(bool));
$extern fn_((ArrPQue_isFull(ArrPQue self))(bool));
$extern fn_((ArrPQue_peek(ArrPQue self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrPQue_peekMut(ArrPQue self, TypeInfo type))(O$u_P$raw));
$extern fn_((ArrPQue_at(ArrPQue self, TypeInfo type, usize idx))(u_P_const$raw));
$extern fn_((ArrPQue_atMut(ArrPQue self, TypeInfo type, usize idx))(u_P$raw));
$extern fn_((ArrPQue_items(ArrPQue self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPQue_itemsMut(ArrPQue self, TypeInfo type))(u_S$raw));
$extern fn_((ArrPQue_itemsCapped(ArrPQue self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPQue_itemsCappedMut(ArrPQue self, TypeInfo type))(u_S$raw));
$extern fn_((ArrPQue_itemsUnused(ArrPQue self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrPQue_itemsUnusedMut(ArrPQue self, TypeInfo type))(u_S$raw));

$attr($must_check)
$extern fn_((ArrPQue_ensureCap(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPQue_ensureCapPrecise(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPQue_ensureUnusedCap(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void));
$extern fn_((ArrPQue_shrinkRetainingCap(ArrPQue* self, usize new_len))(void));
$extern fn_((ArrPQue_shrinkAndFree(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void));
$extern fn_((ArrPQue_clearRetainingCap(ArrPQue* self))(void));
$extern fn_((ArrPQue_clearAndFree(ArrPQue* self, TypeInfo type, mem_Allocator gpa))(void));

$attr($must_check)
$extern fn_((ArrPQue_enque(ArrPQue* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPQue_enqueFixed(ArrPQue* self, u_V$raw item))(mem_Err$void));
$extern fn_((ArrPQue_enqueWithin(ArrPQue* self, u_V$raw item))(void));
$attr($must_check)
$extern fn_((ArrPQue_enqueS(ArrPQue* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrPQue_enqueSFixed(ArrPQue* self, u_S_const$raw items))(mem_Err$void));
$extern fn_((ArrPQue_enqueSWithin(ArrPQue* self, u_S_const$raw items))(void));

$extern fn_((ArrPQue_deque(ArrPQue* self, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((ArrPQue_removeAt(ArrPQue* self, usize idx, u_V$raw ret_mem))(u_V$raw));

$attr($must_check)
$extern fn_((ArrPQue_update(ArrPQue* self, u_V$raw old_item, u_V$raw new_item))(mem_Err$void));

/* ArrPQue_Iter Anonymous */
#define ArrPQue_Iter$$(_T...) __comp_anon__ArrPQue_Iter$$(_T)
/* ArrPQue_Iter Alias */
#define ArrPQue_Iter$(_T...) __comp_alias__ArrPQue_Iter$(_T)
/* ArrPQue_Iter Template */
#define T_decl_ArrPQue_Iter$(_T...) __comp_gen__T_decl_ArrPQue_Iter$(_T)
#define T_impl_ArrPQue_Iter$(_T...) __comp_gen__T_impl_ArrPQue_Iter$(_T)
#define T_use_ArrPQue_Iter$(_T...) __comp_gen__T_use_ArrPQue_Iter$(_T)

/* ArrPQue_Iter Raw Structure */
typedef struct ArrPQue_Iter {
    var_(que, const ArrPQue*);
    var_(idx, usize);
    debug_only(var_(type, TypeInfo);)
} ArrPQue_Iter;
$extern fn_((ArrPQue_iter(const ArrPQue* self, TypeInfo type))(ArrPQue_Iter));
$extern fn_((ArrPQue_Iter_next(ArrPQue_Iter* self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrPQue_Iter_nextMut(ArrPQue_Iter* self, TypeInfo type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrPQue$$(_T...) \
    union { \
        struct { \
            var_(items, S$$(_T)); \
            var_(cap, usize); \
            var_(ctx, P_const$ArrPQue_Ctx); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPQue) $like_ref; \
    }
#define __comp_alias__ArrPQue$(_T...) pp_join($, ArrPQue, _T)
#define __comp_gen__T_decl_ArrPQue$(_T...) \
    $maybe_unused typedef union ArrPQue$(_T) ArrPQue$(_T); \
    T_decl_O$(ArrPQue$(_T)); \
    T_decl_E$(ArrPQue$(_T)); \
    T_decl_E$($set(mem_Err)(ArrPQue$(_T)))
#define __comp_gen__T_impl_ArrPQue$(_T...) \
    union ArrPQue$(_T) { \
        struct { \
            var_(items, S$(_T)); \
            var_(cap, usize); \
            var_(ctx, P_const$ArrPQue_Ctx); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPQue) $like_ref; \
    }; \
    T_impl_O$(ArrPQue$(_T)); \
    T_impl_E$(ArrPQue$(_T)); \
    T_impl_E$($set(mem_Err)(ArrPQue$(_T)))
#define __comp_gen__T_use_ArrPQue$(_T...) \
    T_decl_ArrPQue$(_T); \
    T_impl_ArrPQue$(_T)

/* clang-format off */
#define T_use_ArrPQue_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_empty, _T)(P_const$ArrPQue_Ctx ctx))(ArrPQue$(_T))) { \
        return type$((ArrPQue$(_T))(ArrPQue_empty(typeInfo$(_T), ctx))); \
    }
#define T_use_ArrPQue_fixed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_fixed, _T)(S$(_T) buf, P_const$ArrPQue_Ctx ctx))(ArrPQue$(_T))) { \
        return type$((ArrPQue$(_T))(ArrPQue_fixed(u_anyS(buf), ctx))); \
    }
#define T_use_ArrPQue_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_init, _T)(mem_Allocator gpa, usize cap, P_const$ArrPQue_Ctx ctx))(E$($set(mem_Err)(ArrPQue$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrPQue_init(typeInfo$(_T), gpa, cap, ctx)))); \
    } $unscoped_(fn)
#define T_use_ArrPQue_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_fini, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrPQue_fini(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrPQue_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_len, _T)(ArrPQue$(_T) self))(usize)) { \
        return ArrPQue_len(*self.as_raw); \
    }
#define T_use_ArrPQue_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_cap, _T)(ArrPQue$(_T) self))(usize)) { \
        return ArrPQue_cap(*self.as_raw); \
    }
#define T_use_ArrPQue_peek$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_peek, _T)(ArrPQue$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrPQue_peek(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_peekMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_peekMut, _T)(ArrPQue$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrPQue_peekMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_at$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_at, _T)(ArrPQue$(_T) self, usize idx))(const _T*)) { \
        return u_castP$((const _T*)(ArrPQue_at(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrPQue_atMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_atMut, _T)(ArrPQue$(_T) self, usize idx))(_T*)) { \
        return u_castP$((_T*)(ArrPQue_atMut(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrPQue_items$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_items, _T)(ArrPQue$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPQue_items(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_itemsMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_itemsMut, _T)(ArrPQue$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPQue_itemsMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_itemsCapped$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_itemsCapped, _T)(ArrPQue$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPQue_itemsCapped(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_itemsCappedMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_itemsCappedMut, _T)(ArrPQue$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPQue_itemsCappedMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_itemsUnused$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_itemsUnused, _T)(ArrPQue$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrPQue_itemsUnused(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_itemsUnusedMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_itemsUnusedMut, _T)(ArrPQue$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrPQue_itemsUnusedMut(*self.as_raw, typeInfo$(_T)))); \
    }

#define T_use_ArrPQue_ensureCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_ensureCap, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrPQue_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrPQue_ensureCapPrecise$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_ensureCapPrecise, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrPQue_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrPQue_ensureUnusedCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_ensureUnusedCap, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrPQue_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrPQue_shrinkRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_shrinkRetainingCap, _T)(P$$(ArrPQue$(_T)) self, usize new_len))(void)) { \
        return ArrPQue_shrinkRetainingCap(self->as_raw, new_len); \
    }
#define T_use_ArrPQue_shrinkAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_shrinkAndFree, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, usize new_len))(void)) { \
        return ArrPQue_shrinkAndFree(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrPQue_clearRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_clearRetainingCap, _T)(P$$(ArrPQue$(_T)) self))(void)) { \
        return ArrPQue_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrPQue_clearAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_clearAndFree, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrPQue_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrPQue_enque$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_enque, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrPQue_enque(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrPQue_enqueFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_enqueFixed, _T)(P$$(ArrPQue$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrPQue_enqueFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrPQue_enqueWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_enqueWithin, _T)(P$$(ArrPQue$(_T)) self, _T item))(void)) { \
        return ArrPQue_enqueWithin(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrPQue_enqueS$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_enqueS, _T)(P$$(ArrPQue$(_T)) self, mem_Allocator gpa, S_const$(_T) items))(mem_Err$void)) { \
        return ArrPQue_enqueS(self->as_raw, gpa, u_anyS(items)); \
    }
#define T_use_ArrPQue_enqueSFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_enqueSFixed, _T)(P$$(ArrPQue$(_T)) self, S_const$(_T) items))(mem_Err$void)) { \
        return ArrPQue_enqueSFixed(self->as_raw, u_anyS(items)); \
    }
#define T_use_ArrPQue_enqueSWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_enqueSWithin, _T)(P$$(ArrPQue$(_T)) self, S_const$(_T) items))(void)) { \
        return ArrPQue_enqueSWithin(self->as_raw, u_anyS(items)); \
    }

#define T_use_ArrPQue_deque$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_deque, _T)(P$$(ArrPQue$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrPQue_deque(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrPQue_removeAt$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_removeAt, _T)(P$$(ArrPQue$(_T)) self, usize idx))(_T)) { \
        return u_cast((_T)(ArrPQue_removeAt(self->as_raw, idx, u_retV$(_T)))); \
    }

#define T_use_ArrPQue_update$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrPQue_update, _T)(P$$(ArrPQue$(_T)) self, _T old_item, _T new_item))(mem_Err$void)) { \
        return ArrPQue_update(self->as_raw, u_anyV(old_item), u_anyV(new_item)); \
    }
/* clang-format on */

#define __comp_anon__ArrPQue_Iter$$(_T...) \
    union { \
        struct { \
            var_(que, P_const$$(ArrPQue$$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPQue_Iter) $like_ref; \
    }
#define __comp_alias__ArrPQue_Iter$(_T...) pp_join($, ArrPQue_Iter, _T)
#define __comp_gen__T_decl_ArrPQue_Iter$(_T...) \
    $maybe_unused typedef union ArrPQue_Iter$(_T) ArrPQue_Iter$(_T)
#define __comp_gen__T_impl_ArrPQue_Iter$(_T...) \
    union ArrPQue_Iter$(_T) { \
        struct { \
            var_(que, P_const$$(ArrPQue$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrPQue_Iter) $like_ref; \
    }
#define __comp_gen__T_use_ArrPQue_Iter$(_T...) \
    T_decl_ArrPQue_Iter$(_T); \
    T_impl_ArrPQue_Iter$(_T)

/* clang-format off */
#define T_use_ArrPQue_iter$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_iter, _T)(P_const$$(ArrPQue$(_T)) self))(ArrPQue_Iter$(_T))) { \
        return type$((ArrPQue_Iter$(_T))(ArrPQue_iter(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_Iter_next$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_Iter_next, _T)(P$$(ArrPQue_Iter$(_T)) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrPQue_Iter_next(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrPQue_Iter_nextMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrPQue_Iter_nextMut, _T)(P$$(ArrPQue_Iter$(_T)) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrPQue_Iter_nextMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrPQue__included */
