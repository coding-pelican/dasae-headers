/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrList.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-03-23 (date of last update)
 * @version v0.1-alpha.5
 * @ingroup dasae-headers(dh)
 * @prefix  ArrList
 *
 * @brief   Dynamic array list implementation
 * @details This header provides an array list implementation with dynamic memory allocation.
 *          The array list automatically resizes as elements are added or removed.
 *          Supports operations for initialization, modification, and element access.
 */

/*========== Cheat Sheet ====================================================*/

#ifndef ArrList__included
#define ArrList__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrList Anonymous */
#define ArrList$$(_T...)       __comp_anon__ArrList$$(_T)
/* ArrList Alias */
#define ArrList$(_T...)        __comp_alias__ArrList$(_T)
/* ArrList Template */
#define T_decl_ArrList$(_T...) __comp_gen__T_decl_ArrList$(_T)
#define T_impl_ArrList$(_T...) __comp_gen__T_impl_ArrList$(_T)
#define T_use_ArrList$(_T...)  __comp_gen__T_use_ArrList$(_T)

/* ArrList Raw Structure */
typedef struct ArrList {
    var_(items, S$raw);
    var_(cap, usize);
    debug_only(var_(type, TypeInfo);)
} ArrList;
T_use$(ArrList, (O, E));
T_use_E$($set(mem_Err)(ArrList));

/*========== Function Prototypes ============================================*/

extern fn_((ArrList_empty(TypeInfo type))(ArrList));
extern fn_((ArrList_fromBuf(u_S$raw buf))(ArrList));
extern fn_((ArrList_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrList)) $must_check;
extern fn_((ArrList_fini(ArrList* self, TypeInfo type, mem_Allocator gpa))(void));
extern fn_((ArrList_clone(ArrList self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrList)) $must_check;

extern fn_((ArrList_len(ArrList self))(usize));
extern fn_((ArrList_cap(ArrList self))(usize));
extern fn_((ArrList_at(ArrList self, TypeInfo type, usize idx))(u_P_const$raw));
extern fn_((ArrList_atMut(ArrList self, TypeInfo type, usize idx))(u_P$raw));
extern fn_((ArrList_items(ArrList self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrList_itemsMut(ArrList self, TypeInfo type))(u_S$raw));
extern fn_((ArrList_itemsCapped(ArrList self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrList_itemsCappedMut(ArrList self, TypeInfo type))(u_S$raw));
extern fn_((ArrList_itemsUnused(ArrList self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrList_itemsUnusedMut(ArrList self, TypeInfo type))(u_S$raw));

extern fn_((ArrList_ensureCap(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrList_ensureCapPrecise(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrList_ensureUnusedCap(ArrList* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrList_expandToCap(ArrList* self))(void));
extern fn_((ArrList_resize(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_len))(mem_Err$void)) $must_check;
extern fn_((ArrList_shrinkRetainingCap(ArrList* self, usize new_len))(void));
extern fn_((ArrList_shrinkAndFree(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void));
extern fn_((ArrList_clearRetainingCap(ArrList* self))(void));
extern fn_((ArrList_clearAndFree(ArrList* self, TypeInfo type, mem_Allocator gpa))(void));

extern fn_((ArrList_addBack(ArrList* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrList_addBackFixed(ArrList* self, TypeInfo type))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrList_addBackWithin(ArrList* self, TypeInfo type))(u_P$raw));
extern fn_((ArrList_addBackN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addBackFixedN(ArrList* self, TypeInfo type, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addBackWithinN(ArrList* self, TypeInfo type, usize n))(u_S$raw));

extern fn_((ArrList_addAtN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize idx, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addAtFixedN(ArrList* self, TypeInfo type, usize idx, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addAtWithinN(ArrList* self, TypeInfo type, usize idx, usize n))(u_S$raw));

extern fn_((ArrList_addFront(ArrList* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrList_addFrontFixed(ArrList* self, TypeInfo type))(mem_Err$u_P$raw)) $must_check;
extern fn_((ArrList_addFrontWithin(ArrList* self, TypeInfo type))(u_P$raw));
extern fn_((ArrList_addFrontN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addFrontFixedN(ArrList* self, TypeInfo type, usize n))(mem_Err$u_S$raw)) $must_check;
extern fn_((ArrList_addFrontWithinN(ArrList* self, TypeInfo type, usize n))(u_S$raw));

extern fn_((ArrList_append(ArrList* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixed(ArrList* self, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithin(ArrList* self, u_V$raw item))(void));
extern fn_((ArrList_appendS(ArrList* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixedS(ArrList* self, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithinS(ArrList* self, u_S_const$raw items))(void));
extern fn_((ArrList_appendN(ArrList* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixedN(ArrList* self, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithinN(ArrList* self, u_V$raw item, usize n))(void));

extern fn_((ArrList_insert(ArrList* self, mem_Allocator gpa, usize idx, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertFixed(ArrList* self, usize idx, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertWithin(ArrList* self, usize idx, u_V$raw item))(void));
extern fn_((ArrList_insertS(ArrList* self, mem_Allocator gpa, usize idx, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertFixedS(ArrList* self, usize idx, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertWithinS(ArrList* self, usize idx, u_S_const$raw items))(void));

extern fn_((ArrList_prepend(ArrList* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixed(ArrList* self, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithin(ArrList* self, u_V$raw item))(void));
extern fn_((ArrList_prependS(ArrList* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixedS(ArrList* self, u_S_const$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithinS(ArrList* self, u_S_const$raw items))(void));
extern fn_((ArrList_prependN(ArrList* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixedN(ArrList* self, u_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithinN(ArrList* self, u_V$raw item, usize n))(void));

extern fn_((ArrList_replace(ArrList* self, mem_Allocator gpa, R range, u_S_const$raw new_items))(mem_Err$void)) $must_check;
extern fn_((ArrList_replaceFixed(ArrList* self, R range, u_S_const$raw new_items))(mem_Err$void)) $must_check;
extern fn_((ArrList_replaceWithin(ArrList* self, R range, u_S_const$raw new_items))(void));

extern fn_((ArrList_pop(ArrList* self, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrList_removeOrd(ArrList* self, usize idx, u_V$raw ret_mem))(u_V$raw));
extern fn_((ArrList_removeSwap(ArrList* self, usize idx, u_V$raw ret_mem))(u_V$raw));
extern fn_((ArrList_shift(ArrList* self, u_V$raw ret_mem))(O$u_V$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrList$$(_T...) \
    union { \
        struct { \
            var_(items, S$$(_T)); \
            var_(cap, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ptr, ArrList); \
    }
#define __comp_alias__ArrList$(_T...) pp_join($, ArrList, _T)
#define __comp_gen__T_decl_ArrList$(_T...) \
    $maybe_unused typedef union ArrList$(_T) ArrList$(_T); \
    T_decl_O$(ArrList$(_T)); \
    T_decl_E$(ArrList$(_T)); \
    T_decl_E$($set(mem_Err)(ArrList$(_T)))
#define __comp_gen__T_impl_ArrList$(_T...) \
    union ArrList$(_T) { \
        struct { \
            var_(items, S$(_T)); \
            var_(cap, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ptr, ArrList); \
    }; \
    T_impl_O$(ArrList$(_T)); \
    T_impl_E$(ArrList$(_T)); \
    T_impl_E$($set(mem_Err)(ArrList$(_T)))
#define __comp_gen__T_use_ArrList$(_T...) \
    T_decl_ArrList$(_T); \
    T_impl_ArrList$(_T)

/* #define comp_op__ArrList_anonCast$(__anon, T_ArrList, var_anon...) blk({ \
    let __anon = &var_anon; \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(T_ArrList)); \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(T_ArrList)); \
    claim_assert_static(validateField(TypeOf(*__anon), base, FieldTypeOf(T_ArrList, base))); \
    claim_assert_static(validateField(TypeOf(*__anon), type, FieldTypeOf(T_ArrList, type))); \
    claim_assert_static(fieldAnonTypeCastable(T_ArrList, *__anon, Sli, items)); \
    claim_assert_static(validateField(TypeOf(*__anon), cap, FieldTypeOf(T_ArrList, cap))); \
    claim_assert_static(validateField(TypeOf(*__anon), gpa, FieldTypeOf(T_ArrList, gpa))); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), base) == fieldPadding(T_ArrList, base)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), type) == fieldPadding(T_ArrList, type)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), items) == fieldPadding(T_ArrList, items)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), cap) == fieldPadding(T_ArrList, cap)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), gpa) == fieldPadding(T_ArrList, gpa)); \
    blk_return rawderef(as$(($P$(T_ArrList))(__anon))); \
}) */

/* clang-format off */
#define T_use_ArrList_empty$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_empty, _T))(ArrList$(_T))) { \
        return type$((ArrList$(_T))(ArrList_empty(typeInfo$(_T)))); \
    }
#define T_use_ArrList_fromBuf$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_fromBuf, _T)(S$(_T) buf))(ArrList$(_T))) { \
        return type$((ArrList$(_T))(ArrList_fromBuf(u_any$S(buf)))); \
    }
#define T_use_ArrList_init$(_T...) \
    $inline_always $must_check\
    $static fn_((tpl_id(ArrList_init, _T)(mem_Allocator gpa, usize cap))(E$($set(mem_Err)(ArrList$(_T)))) $scope) { \
        return_(type$E((ReturnType)(ArrList_init(typeInfo$(_T), gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_ArrList_fini$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_fini, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrList_fini(self->as_raw, typeInfo$(_T), gpa); \
    }
#define T_use_ArrList_clone$(_T...) \
    $inline_always $must_check\
    $static fn_((tpl_id(ArrList_clone, _T)(ArrList$(_T) self, mem_Allocator gpa))(E$($set(mem_Err)(ArrList$(_T)))) $scope) { \
        return_(type$E((ReturnType)(ArrList_clone(self->as_raw, typeInfo$(_T), gpa)))); \
    } $unscoped_(fn)

#define T_use_ArrList_len$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_len, _T)(ArrList$(_T) self))(usize)) { \
        return ArrList_len(self.as_raw); \
    }
#define T_use_ArrList_cap$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_cap, _T)(ArrList$(_T) self))(usize)) { \
        return ArrList_cap(self.as_raw); \
    }
#define T_use_ArrList_at$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_at, _T)(ArrList$(_T) self, TypeInfo type, usize idx))(const _T*)) { \
        return u_castP((const _T*)(ArrList_at(self.as_raw, type, idx))); \
    }
#define T_use_ArrList_atMut$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_atMut, _T)(ArrList$(_T) self, TypeInfo type, usize idx))(_T*)) { \
        return u_castP((_T*)(ArrList_atMut(self.as_raw, type, idx))); \
    }
#define T_use_ArrList_items$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_items, _T)(ArrList$(_T) self, TypeInfo type))(S$(const _T))) { \
        return u_castS((S$(const _T))(ArrList_items(self.as_raw, type))); \
    }
#define T_use_ArrList_itemsMut$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_itemsMut, _T)(ArrList$(_T) self, TypeInfo type))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_itemsMut(self.as_raw, type))); \
    }
#define T_use_ArrList_itemsCapped$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_itemsCapped, _T)(ArrList$(_T) self, TypeInfo type))(S$(const _T))) { \
        return u_castS((S$(const _T))(ArrList_itemsCapped(self.as_raw, type))); \
    }
#define T_use_ArrList_itemsCappedMut$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_itemsCappedMut, _T)(ArrList$(_T) self, TypeInfo type))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_itemsCappedMut(self.as_raw, type))); \
    }
#define T_use_ArrList_itemsUnused$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_itemsUnused, _T)(ArrList$(_T) self, TypeInfo type))(S$(const _T))) { \
        return u_castS((S$(const _T))(ArrList_itemsUnused(self.as_raw, type))); \
    }
#define T_use_ArrList_itemsUnusedMut$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_itemsUnusedMut, _T)(ArrList$(_T) self, TypeInfo type))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_itemsUnusedMut(self.as_raw, type))); \
    }

#define T_use_ArrList_ensureCap$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_ensureCap, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrList_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrList_ensureCapPrecise$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_ensureCapPrecise, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrList_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrList_ensureUnusedCap$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_ensureUnusedCap, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrList_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrList_expandToCap$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_expandToCap, _T)(P$$(ArrList$(_T)) self))(void)) { \
        return ArrList_expandToCap(self->as_raw); \
    }
#define T_use_ArrList_resize$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_resize, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize new_len))(mem_Err$void)) { \
        return ArrList_resize(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrList_shrinkRetainingCap$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_shrinkRetainingCap, _T)(P$$(ArrList$(_T)) self, usize new_len))(void)) { \
        return ArrList_shrinkRetainingCap(self->as_raw, new_len); \
    }
#define T_use_ArrList_shrinkAndFree$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_shrinkAndFree, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize new_len))(void)) { \
        return ArrList_shrinkAndFree(self->as_raw, typeInfo$(_T), gpa, new_len); \
    }
#define T_use_ArrList_clearRetainingCap$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_clearRetainingCap, _T)(P$$(ArrList$(_T)) self))(void)) { \
        return ArrList_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrList_clearAndFree$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_clearAndFree, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrList_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrList_addBack$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addBack, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addBack(self->as_raw, typeInfo$(_T), gpa)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addBackFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addBackFixed, _T)(P$$(ArrList$(_T)) self))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addBackFixed(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrList_addBackWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_addBackWithin, _T)(P$$(ArrList$(_T)) self))(_T*)) { \
        return u_castP((_T*)(ArrList_addBackWithin(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrList_addBackN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addBackN, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addBackN(self->as_raw, typeInfo$(_T), gpa, n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addBackFixedN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addBackFixedN, _T)(P$$(ArrList$(_T)) self, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addBackFixedN(self->as_raw, typeInfo$(_T), n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addBackWithinN$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_addBackWithinN, _T)(P$$(ArrList$(_T)) self, usize n))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_addBackWithinN(self->as_raw, typeInfo$(_T), n))); \
    }

#define T_use_ArrList_addAtN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addAtN, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize idx, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addAtN(self->as_raw, typeInfo$(_T), gpa, idx, n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addAtFixedN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addAtFixedN, _T)(P$$(ArrList$(_T)) self, usize idx, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addAtFixedN(self->as_raw, typeInfo$(_T), idx, n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addAtWithinN$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_addAtWithinN, _T)(P$$(ArrList$(_T)) self, usize idx, usize n))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_addAtWithinN(self->as_raw, typeInfo$(_T), idx, n))); \
    }

#define T_use_ArrList_addFront$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addFront, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addFront(self->as_raw, typeInfo$(_T), gpa)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addFrontFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addFrontFixed, _T)(P$$(ArrList$(_T)) self))(E$($set(mem_Err)(P$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addFrontFixed(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrList_addFrontWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_addFrontWithin, _T)(P$$(ArrList$(_T)) self))(_T*)) { \
        return u_castP((_T*)(ArrList_addFrontWithin(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrList_addFrontN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addFrontN, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addFrontN(self->as_raw, typeInfo$(_T), gpa, n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addFrontFixedN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_addFrontFixedN, _T)(P$$(ArrList$(_T)) self, usize n))(E$($set(mem_Err)(S$(_T)))) $scope) { \
        return_(u_castE((ReturnType)(ArrList_addFrontFixedN(self->as_raw, typeInfo$(_T), n)))); \
    } $unscoped_(fn)
#define T_use_ArrList_addFrontWithinN$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_addFrontWithinN, _T)(P$$(ArrList$(_T)) self, usize n))(S$(_T))) { \
        return u_castS((S$(_T))(ArrList_addFrontWithinN(self->as_raw, typeInfo$(_T), n))); \
    }

#define T_use_ArrList_append$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_append, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrList_append(self->as_raw, gpa, u_any$V(item)); \
    }
#define T_use_ArrList_appendFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_appendFixed, _T)(P$$(ArrList$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrList_appendFixed(self->as_raw, u_any$V(item)); \
    }
#define T_use_ArrList_appendWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_appendWithin, _T)(P$$(ArrList$(_T)) self, _T item))(void)) { \
        return ArrList_appendWithin(self->as_raw, u_any$V(item)); \
    }
#define T_use_ArrList_appendS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_appendS, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_appendS(self->as_raw, gpa, u_any$S_const(items)); \
    }
#define T_use_ArrList_appendFixedS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_appendFixedS, _T)(P$$(ArrList$(_T)) self, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_appendFixedS(self->as_raw, u_any$S_const(items)); \
    }
#define T_use_ArrList_appendWithinS$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_appendWithinS, _T)(P$$(ArrList$(_T)) self, S$(const _T) items))(void)) { \
        return ArrList_appendWithinS(self->as_raw, u_any$S_const(items)); \
    }
#define T_use_ArrList_appendN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_appendN, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, _T item, usize n))(mem_Err$void)) { \
        return ArrList_appendN(self->as_raw, gpa, u_any$V(item), n); \
    }
#define T_use_ArrList_appendFixedN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_appendFixedN, _T)(P$$(ArrList$(_T)) self, _T item, usize n))(mem_Err$void)) { \
        return ArrList_appendFixedN(self->as_raw, u_any$V(item), n); \
    }
#define T_use_ArrList_appendWithinN$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_appendWithinN, _T)(P$$(ArrList$(_T)) self, _T item, usize n))(void)) { \
        return ArrList_appendWithinN(self->as_raw, u_any$V(item), n); \
    }

#define T_use_ArrList_insert$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_insert, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize idx, _T item))(mem_Err$void)) { \
        return ArrList_insert(self->as_raw, gpa, idx, u_any$V(item)); \
    }
#define T_use_ArrList_insertFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_insertFixed, _T)(P$$(ArrList$(_T)) self, usize idx, _T item))(mem_Err$void)) { \
        return ArrList_insertFixed(self->as_raw, idx, u_any$V(item)); \
    }
#define T_use_ArrList_insertWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_insertWithin, _T)(P$$(ArrList$(_T)) self, usize idx, _T item))(void)) { \
        return ArrList_insertWithin(self->as_raw, idx, u_any$V(item)); \
    }
#define T_use_ArrList_insertS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_insertS, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, usize idx, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_insertS(self->as_raw, gpa, idx, u_any$S_const(items)); \
    }
#define T_use_ArrList_insertFixedS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_insertFixedS, _T)(P$$(ArrList$(_T)) self, usize idx, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_insertFixedS(self->as_raw, idx, u_any$S_const(items)); \
    }
#define T_use_ArrList_insertWithinS$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_insertWithinS, _T)(P$$(ArrList$(_T)) self, usize idx, S$(const _T) items))(void)) { \
        return ArrList_insertWithinS(self->as_raw, idx, u_any$S_const(items)); \
    }

#define T_use_ArrList_prepend$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prepend, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrList_prepend(self->as_raw, gpa, u_any$V(item)); \
    }
#define T_use_ArrList_prependFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prependFixed, _T)(P$$(ArrList$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrList_prependFixed(self->as_raw, u_any$V(item)); \
    }
#define T_use_ArrList_prependWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_prependWithin, _T)(P$$(ArrList$(_T)) self, _T item))(void)) { \
        return ArrList_prependWithin(self->as_raw, u_any$V(item)); \
    }
#define T_use_ArrList_prependS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prependS, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_prependS(self->as_raw, gpa, u_any$S_const(items)); \
    }
#define T_use_ArrList_prependFixedS$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prependFixedS, _T)(P$$(ArrList$(_T)) self, S$(const _T) items))(mem_Err$void)) { \
        return ArrList_prependFixedS(self->as_raw, u_any$S_const(items)); \
    }
#define T_use_ArrList_prependWithinS$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_prependWithinS, _T)(P$$(ArrList$(_T)) self, S$(const _T) items))(void)) { \
        return ArrList_prependWithinS(self->as_raw, u_any$S_const(items)); \
    }
#define T_use_ArrList_prependN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prependN, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, _T item, usize n))(mem_Err$void)) { \
        return ArrList_prependN(self->as_raw, gpa, u_any$V(item), n); \
    }
#define T_use_ArrList_prependFixedN$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_prependFixedN, _T)(P$$(ArrList$(_T)) self, _T item, usize n))(mem_Err$void)) { \
        return ArrList_prependFixedN(self->as_raw, u_any$V(item), n); \
    }
#define T_use_ArrList_prependWithinN$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_prependWithinN, _T)(P$$(ArrList$(_T)) self, _T item, usize n))(void)) { \
        return ArrList_prependWithinN(self->as_raw, u_any$V(item), n); \
    }

#define T_use_ArrList_replace$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_replace, _T)(P$$(ArrList$(_T)) self, mem_Allocator gpa, R range, S$(const _T) new_items))(mem_Err$void)) { \
        return ArrList_replace(self->as_raw, gpa, range, u_any$S_const(new_items)); \
    }
#define T_use_ArrList_replaceFixed$(_T...) \
    $inline_always $must_check \
    $static fn_((tpl_id(ArrList_replaceFixed, _T)(P$$(ArrList$(_T)) self, R range, S$(const _T) new_items))(mem_Err$void)) { \
        return ArrList_replaceFixed(self->as_raw, range, u_any$S_const(new_items)); \
    }
#define T_use_ArrList_replaceWithin$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_replaceWithin, _T)(P$$(ArrList$(_T)) self, R range, S$(const _T) new_items))(void)) { \
        return ArrList_replaceWithin(self->as_raw, range, u_any$S_const(new_items)); \
    }

#define T_use_ArrList_pop$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_pop, _T)(P$$(ArrList$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO((ReturnType)(ArrList_pop(self->as_raw, u_ret$V(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrList_removeOrd$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_removeOrd, _T)(P$$(ArrList$(_T)) self, usize idx))(_T)) { \
        return u_cast((_T)(ArrList_removeOrd(self->as_raw, idx, u_ret$V(_T)))); \
    }
#define T_use_ArrList_removeSwap$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_removeSwap, _T)(P$$(ArrList$(_T)) self, usize idx))(_T)) { \
        return u_cast((_T)(ArrList_removeSwap(self->as_raw, idx, u_ret$V(_T)))); \
    }
#define T_use_ArrList_shift$(_T...) \
    $inline_always \
    $static fn_((tpl_id(ArrList_shift, _T)(P$$(ArrList$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO((ReturnType)(ArrList_shift(self->as_raw, u_ret$V(_T))))); \
    } $unscoped_(fn)
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrList__included */
