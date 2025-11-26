/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrStk.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-03-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  ArrStk
 *
 * @brief   Dynamic array stack implementation
 * @details This header provides an array stack implementation with dynamic memory allocation.
 *          The array stack automatically resizes as elements are added or removed.
 *          Supports operations for initialization, modification, and element access.
 *          Pushing/popping items from the stack is O(1).
 */

/*========== Cheat Sheet ====================================================*/

#ifndef ArrStk__included
#define ArrStk__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrStk Anonymous */
#define ArrStk$$(_T...) __comp_anon__ArrStk$$(_T)
/* ArrStk Alias */
#define ArrStk$(_T...) __comp_alias__ArrStk$(_T)
/* ArrStk Template */
#define T_decl_ArrStk$(_T...) __comp_gen__T_decl_ArrStk$(_T)
#define T_impl_ArrStk$(_T...) __comp_gen__T_impl_ArrStk$(_T)
#define T_use_ArrStk$(_T...) __comp_gen__T_use_ArrStk$(_T)

/* ArrStk Raw Structure */
typedef struct ArrStk {
    var_(items, S$raw);
    var_(cap, usize);
    debug_only(var_(type, TypeInfo);)
} ArrStk;
T_use$((ArrStk)(O, E));
T_use_E$($set(mem_Err)(ArrStk));

/*========== Function Prototypes ============================================*/

extern fn_((ArrStk_empty(TypeInfo type))(ArrStk));
extern fn_((ArrStk_fromBuf(u_S$raw buf))(ArrStk));
extern fn_((ArrStk_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrStk)) $must_check;
extern fn_((ArrStk_fini(ArrStk* self, TypeInfo type, mem_Allocator gpa))(void));
extern fn_((ArrStk_clone(ArrStk self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrStk)) $must_check;

extern fn_((ArrStk_len(ArrStk self))(usize));
extern fn_((ArrStk_cap(ArrStk self))(usize));
extern fn_((ArrStk_at(ArrStk self, TypeInfo type, usize idx))(u_P_const$raw));
extern fn_((ArrStk_atMut(ArrStk self, TypeInfo type, usize idx))(u_P$raw));
extern fn_((ArrStk_top(ArrStk self, TypeInfo type))(O$u_P_const$raw));
extern fn_((ArrStk_topMut(ArrStk self, TypeInfo type))(O$u_P$raw));
extern fn_((ArrStk_items(ArrStk self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrStk_itemsMut(ArrStk self, TypeInfo type))(u_S$raw));
extern fn_((ArrStk_itemsCapped(ArrStk self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrStk_itemsCappedMut(ArrStk self, TypeInfo type))(u_S$raw));
extern fn_((ArrStk_itemsUnused(ArrStk self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrStk_itemsUnusedMut(ArrStk self, TypeInfo type))(u_S$raw));

extern fn_((ArrStk_ensureCap(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrStk_ensureCapPrecise(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrStk_ensureUnusedCap(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrStk_expandToCap(ArrStk* self))(void));
extern fn_((ArrStk_resize(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_len))(mem_Err$void)) $must_check;
extern fn_((ArrStk_shrinkRetainingCap(ArrStk* self, usize new_len))(void));
extern fn_((ArrStk_shrinkAndFree(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void));
extern fn_((ArrStk_clearRetainingCap(ArrStk* self))(void));
extern fn_((ArrStk_clearAndFree(ArrStk* self, TypeInfo type, mem_Allocator gpa))(void));

extern fn_((ArrStk_add(ArrStk* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrStk_addFixed(ArrStk* self, TypeInfo type))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrStk_addWithin(ArrStk* self, TypeInfo type))(u_P$raw));
extern fn_((ArrStk_addN(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrStk_addFixedN(ArrStk* self, TypeInfo type, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrStk_addWithinN(ArrStk* self, TypeInfo type, usize n))(u_S$raw));

extern fn_((ArrStk_push(ArrStk* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushFixed(ArrStk* self, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushWithin(ArrStk* self, u_V$raw item))(void));
extern fn_((ArrStk_pushS(ArrStk* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushFixedS(ArrStk* self, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushWithinS(ArrStk* self, u_S_const$raw items))(void));
extern fn_((ArrStk_pushN(ArrStk* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushFixedN(ArrStk* self, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrStk_pushWithinN(ArrStk* self, u_V$raw item, usize n))(void));

extern fn_((ArrStk_pop(ArrStk* self, u_V$raw ret_mem))(O$u_V$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrStk$$(_T...) \
    union { \
        struct { \
            var_(items, S$$(_T)); \
            var_(cap, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrStk); \
    }
#define __comp_alias__ArrStk$(_T...) pp_join($, ArrStk, _T)
#define __comp_gen__T_decl_ArrStk$(_T...) \
    $maybe_unused typedef union ArrStk$(_T) ArrStk$(_T); \
    T_decl_O$(ArrStk$(_T)); \
    T_decl_E$(ArrStk$(_T)); \
    T_decl_E$($set(mem_Err)(ArrStk$(_T)))
#define __comp_gen__T_impl_ArrStk$(_T...) \
    union ArrStk$(_T) { \
        struct { \
            var_(items, S$(_T)); \
            var_(cap, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrStk); \
    }; \
    T_impl_O$(ArrStk$(_T)); \
    T_impl_E$(ArrStk$(_T)); \
    T_impl_E$($set(mem_Err)(ArrStk$(_T)))
#define __comp_gen__T_use_ArrStk$(_T...) \
    T_decl_ArrStk$(_T); \
    T_impl_ArrStk$(_T)

/* clang-format off */
#define T_use_ArrStk_empty$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_empty, _T))(ArrStk$(_T))) { \
        return type$((ArrStk$(_T))(ArrStk_empty(typeInfo$(_T)))); \
    }
#define T_use_ArrStk_fromBuf$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_fromBuf, _T)(S$(_T) buf))(ArrStk$(_T))) { \
        return type$((ArrStk$(_T))(ArrStk_fromBuf(u_anyS(buf)))); \
    }
#define T_use_ArrStk_init$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_init, _T)(mem_Allocator gpa, usize cap))(E$($set(mem_Err)(ArrStk$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrStk_init(typeInfo$(_T), gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_ArrStk_fini$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_fini, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrStk_fini(self->as_raw, typeInfo$(_T), gpa); \
    }
#define T_use_ArrStk_clone$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_clone, _T)(ArrStk$(_T) self, mem_Allocator gpa))(E$($set(mem_Err)(ArrStk$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrStk_clone(*self->as_raw, typeInfo$(_T), gpa)))); \
    } $unscoped_(fn)

#define T_use_ArrStk_len$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_len, _T)(ArrStk$(_T) self))(usize)) { \
        return ArrStk_len(*self.as_raw); \
    }
#define T_use_ArrStk_cap$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_cap, _T)(ArrStk$(_T) self))(usize)) { \
        return ArrStk_cap(*self.as_raw); \
    }
#define T_use_ArrStk_at$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_at, _T)(ArrStk$(_T) self, usize idx))(const _T*)) { \
        return u_castP$((const _T*)(ArrStk_at(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrStk_atMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_atMut, _T)(ArrStk$(_T) self, usize idx))(_T*)) { \
        return u_castP$((_T*)(ArrStk_atMut(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrStk_top$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_top, _T)(ArrStk$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrStk_top(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_topMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_topMut, _T)(ArrStk$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrStk_topMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_bottom$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_bottom, _T)(ArrStk$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrStk_bottom(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_bottomMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_bottomMut, _T)(ArrStk$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrStk_bottomMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_items$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_items, _T)(ArrStk$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrStk_items(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_itemsMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_itemsMut, _T)(ArrStk$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrStk_itemsMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_itemsCapped$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_itemsCapped, _T)(ArrStk$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrStk_itemsCapped(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_itemsCappedMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_itemsCappedMut, _T)(ArrStk$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrStk_itemsCappedMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_itemsUnused$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_itemsUnused, _T)(ArrStk$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrStk_itemsUnused(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_itemsUnusedMut$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_itemsUnusedMut, _T)(ArrStk$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrStk_itemsUnusedMut(*self.as_raw, typeInfo$(_T)))); \
    }

#define T_use_ArrStk_ensureCap$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_ensureCap, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrStk_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrStk_ensureCapPrecise$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_ensureCapPrecise, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrStk_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrStk_ensureUnusedCap$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_ensureUnusedCap, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrStk_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrStk_expandToCap$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_expandToCap, _T)(P$$(ArrStk$(_T)) self))(void)) { \
        return ArrStk_expandToCap(self->as_raw); \
    }
#define T_use_ArrStk_resize$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_resize, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize new_len))(mem_Err$void)) { \
        return ArrStk_resize(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrStk_shrinkRetainingCap$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_shrinkRetainingCap, _T)(P$$(ArrStk$(_T)) self, usize new_len))(void)) { \
        return ArrStk_shrinkRetainingCap(self->as_raw, new_len); \
    }
#define T_use_ArrStk_shrinkAndFree$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_shrinkAndFree, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize new_len))(void)) { \
        return ArrStk_shrinkAndFree(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrStk_clearRetainingCap$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_clearRetainingCap, _T)(P$$(ArrStk$(_T)) self))(void)) { \
        return ArrStk_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrStk_clearAndFree$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_clearAndFree, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrStk_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrStk_add$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_add, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(ArrStk_add(self->as_raw, typeInfo$(_T), gpa)))); \
    } $unscoped_(fn)
#define T_use_ArrStk_addFixed$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_addFixed, _T)(P$$(ArrStk$(_T)) self))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(ArrStk_addFixed(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrStk_addWithin$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_addWithin, _T)(P$$(ArrStk$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(ArrStk_addWithin(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrStk_addN$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_addN, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(ArrStk_addN(self->as_raw, typeInfo$(_T), gpa, n)))); \
    } $unscoped_(fn)
#define T_use_ArrStk_addFixedN$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_addFixedN, _T)(P$$(ArrStk$(_T)) self, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE$((ReturnType)(ArrStk_addFixedN(self->as_raw, typeInfo$(_T), n)))); \
    } $unscoped_(fn)
#define T_use_ArrStk_addWithinN$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_addWithinN, _T)(P$$(ArrStk$(_T)) self, usize n))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrStk_addWithinN(self->as_raw, typeInfo$(_T), n))); \
    }

#define T_use_ArrStk_push$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_push, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrStk_push(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrStk_pushFixed$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_pushFixed, _T)(P$$(ArrStk$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrStk_pushFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrStk_pushWithin$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_pushWithin, _T)(P$$(ArrStk$(_T)) self, _T item))(void)) { \
        return ArrStk_pushWithin(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrStk_pushS$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_pushS, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, S$(const _T) items))(mem_Err$void)) { \
        return ArrStk_pushS(self->as_raw, gpa, u_anyS(items)); \
    }
#define T_use_ArrStk_pushFixedS$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_pushFixedS, _T)(P$$(ArrStk$(_T)) self, S$(const _T) items))(mem_Err$void)) { \
        return ArrStk_pushFixedS(self->as_raw, u_anyS(items)); \
    }
#define T_use_ArrStk_pushWithinS$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_pushWithinS, _T)(P$$(ArrStk$(_T)) self, S$(const _T) items))(void)) { \
        return ArrStk_pushWithinS(self->as_raw, u_anyS(items)); \
    }
#define T_use_ArrStk_pushN$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_pushN, _T)(P$$(ArrStk$(_T)) self, mem_Allocator gpa, _T item, usize n))(mem_Err$void)) { \
        return ArrStk_pushN(self->as_raw, gpa, u_anyV(item), n); \
    }
#define T_use_ArrStk_pushFixedN$(_T...) \
    $must_check \
    $inline_always $static fn_((tpl_id(ArrStk_pushFixedN, _T)(P$$(ArrStk$(_T)) self, _T item, usize n))(mem_Err$void)) { \
        return ArrStk_pushFixedN(self->as_raw, u_anyV(item), n); \
    }
#define T_use_ArrStk_pushWithinN$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_pushWithinN, _T)(P$$(ArrStk$(_T)) self, _T item, usize n))(void)) { \
        return ArrStk_pushWithinN(self->as_raw, u_anyV(item), n); \
    }

#define T_use_ArrStk_pop$(_T...) \
    $inline_always $static fn_((tpl_id(ArrStk_pop, _T)(P$$(ArrStk$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrStk_pop(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrStk__included */
