/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrQue.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2026-01-03 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  ArrQue
 *
 * @brief   Dynamic array queue implementation
 * @details This header provides a contiguous, growable, queue implementation with dynamic memory allocation.
 *          The array queue automatically resizes as elements are added or removed.
 *          Supports operations for initialization, modification, and element access.
 *          Enqueuing/dequeuing items from the queue is O(1).
 */
#ifndef ArrQue__included
#define ArrQue__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrQue Anonymous */
#define ArrQue$$(_T...) __comp_anon__ArrQue$$(_T)
/* ArrQue Alias */
#define ArrQue$(_T...) __comp_alias__ArrQue$(_T)
/* ArrQue Template */
#define T_decl_ArrQue$(_T...) __comp_gen__T_decl_ArrQue$(_T)
#define T_impl_ArrQue$(_T...) __comp_gen__T_impl_ArrQue$(_T)
#define T_use_ArrQue$(_T...) __comp_gen__T_use_ArrQue$(_T)

/* ArrQue Raw Structure */
typedef struct ArrQue {
    var_(buf, S$raw);
    var_(head, usize);
    var_(len, usize);
    debug_only(var_(type, TypeInfo);)
} ArrQue;
T_use$((ArrQue)(O, E));
T_use_E$($set(mem_Err)(ArrQue));

$extern fn_((ArrQue_empty(TypeInfo type))(ArrQue));
$extern fn_((ArrQue_fixed(u_S$raw buf))(ArrQue));
$attr($must_check)
$extern fn_((ArrQue_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrQue));
$extern fn_((ArrQue_fini(ArrQue* self, TypeInfo type, mem_Allocator gpa))(void));

typedef struct ArrQue_Grip {
    var_(buf, S$raw);
    var_(head, usize*);
    var_(len, usize*);
    var_(ctx, ArrQue);
} ArrQue_Grip;
#define ArrQue_Grip$$(_T...) __comp_anon__ArrQue_Grip$$(_T)
#define ArrQue_Grip$(_T...) __comp_alias__ArrQue_Grip$(_T)
#define T_decl_ArrQue_Grip$(_T...) __comp_gen__T_decl_ArrQue_Grip$(_T)
#define T_impl_ArrQue_Grip$(_T...) __comp_gen__T_impl_ArrQue_Grip$(_T)
#define T_use_ArrQue_Grip$(_T...) __comp_gen__T_use_ArrQue_Grip$(_T)
$extern fn_((ArrQue_grip(u_S$raw buf, usize* head, usize* len))(ArrQue_Grip));
$extern fn_((ArrQue_Grip_release(ArrQue_Grip* self, TypeInfo type))(void));

$extern fn_((ArrQue_len(ArrQue self))(usize));
$extern fn_((ArrQue_cap(ArrQue self))(usize));
$extern fn_((ArrQue_isEmpty(ArrQue self))(bool));
$extern fn_((ArrQue_isFull(ArrQue self))(bool));
$extern fn_((ArrQue_head(ArrQue self))(usize));
$extern fn_((ArrQue_tail(ArrQue self))(usize));
$extern fn_((ArrQue_at(ArrQue self, TypeInfo type, usize idx))(u_P_const$raw));
$extern fn_((ArrQue_atMut(ArrQue self, TypeInfo type, usize idx))(u_P$raw));
$extern fn_((ArrQue_front(ArrQue self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrQue_frontMut(ArrQue self, TypeInfo type))(O$u_P$raw));
$extern fn_((ArrQue_back(ArrQue self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrQue_backMut(ArrQue self, TypeInfo type))(O$u_P$raw));
$extern fn_((ArrQue_buf(ArrQue self, TypeInfo type))(u_S_const$raw));
$extern fn_((ArrQue_bufMut(ArrQue self, TypeInfo type))(u_S$raw));

$attr($must_check)
$extern fn_((ArrQue_ensureCap(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrQue_ensureCapPrecise(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrQue_ensureUnusedCap(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void));
$extern fn_((ArrQue_clearRetainingCap(ArrQue* self))(void));
$extern fn_((ArrQue_clearAndFree(ArrQue* self, TypeInfo type, mem_Allocator gpa))(void));

$attr($must_check)
$extern fn_((ArrQue_enque(ArrQue* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void));
$attr($must_check)
$extern fn_((ArrQue_enqueFixed(ArrQue* self, u_V$raw item))(mem_Err$void));
$extern fn_((ArrQue_enqueWithin(ArrQue* self, u_V$raw item))(void));

$extern fn_((ArrQue_deque(ArrQue* self, u_V$raw ret_mem))(O$u_V$raw));

/* ArrQue_Iter Anonymous */
#define ArrQue_Iter$$(_T...) __comp_anon__ArrQue_Iter$$(_T)
/* ArrQue_Iter Alias */
#define ArrQue_Iter$(_T...) __comp_alias__ArrQue_Iter$(_T)
/* ArrQue_Iter Template */
#define T_decl_ArrQue_Iter$(_T...) __comp_gen__T_decl_ArrQue_Iter$(_T)
#define T_impl_ArrQue_Iter$(_T...) __comp_gen__T_impl_ArrQue_Iter$(_T)
#define T_use_ArrQue_Iter$(_T...) __comp_gen__T_use_ArrQue_Iter$(_T)

/* ArrQue_Iter Raw Structure */
typedef struct ArrQue_Iter {
    var_(que, const ArrQue*);
    var_(idx, usize);
    debug_only(var_(type, TypeInfo);)
} ArrQue_Iter;
$extern fn_((ArrQue_iter(const ArrQue* self, TypeInfo type))(ArrQue_Iter));
$extern fn_((ArrQue_Iter_next(ArrQue_Iter* self, TypeInfo type))(O$u_P_const$raw));
$extern fn_((ArrQue_Iter_nextMut(ArrQue_Iter* self, TypeInfo type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrQue$$(_T...) \
    union { \
        struct { \
            var_(buf, S$$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrQue) $like_ref; \
    }
#define __comp_alias__ArrQue$(_T...) pp_join($, ArrQue, _T)
#define __comp_gen__T_decl_ArrQue$(_T...) \
    $maybe_unused typedef union ArrQue$(_T) ArrQue$(_T); \
    T_decl_O$(ArrQue$(_T)); \
    T_decl_E$(ArrQue$(_T)); \
    T_decl_E$($set(mem_Err)(ArrQue$(_T)))
#define __comp_gen__T_impl_ArrQue$(_T...) \
    union ArrQue$(_T) { \
        struct { \
            var_(buf, S$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrQue) $like_ref; \
    }; \
    T_impl_O$(ArrQue$(_T)); \
    T_impl_E$(ArrQue$(_T)); \
    T_impl_E$($set(mem_Err)(ArrQue$(_T)))
#define __comp_gen__T_use_ArrQue$(_T...) \
    T_decl_ArrQue$(_T); \
    T_impl_ArrQue$(_T)

/* clang-format off */
#define T_use_ArrQue_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_empty, _T)(void))(ArrQue$(_T))) { \
        return type$((ArrQue$(_T))(ArrQue_empty(typeInfo$(_T)))); \
    }
#define T_use_ArrQue_fixed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_fixed, _T)(S$(_T) buf))(ArrQue$(_T))) { \
        return type$((ArrQue$(_T))(ArrQue_fixed(u_anyS(buf)))); \
    }
#define T_use_ArrQue_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_init, _T)(mem_Allocator gpa, usize cap))(E$($set(mem_Err)(ArrQue$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrQue_init(typeInfo$(_T), gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_ArrQue_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_fini, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrQue_fini(self->as_raw, typeInfo$(_T), gpa); \
    }
/* clang-format on */

#define __comp_anon__ArrQue_Grip$$(_T...) \
    union { \
        struct { \
            var_(buf, S$$(_T)); \
            var_(head, usize*); \
            var_(len, usize*); \
            var_(ctx, ArrQue$$(_T)); \
        }; \
        var_(as_raw, ArrQue_Grip) $like_ref; \
    }
#define __comp_alias__ArrQue_Grip$(_T...) pp_join($, ArrQue_Grip, _T)
#define __comp_gen__T_decl_ArrQue_Grip$(_T...) \
    $maybe_unused typedef union ArrQue_Grip$(_T) ArrQue_Grip$(_T)
#define __comp_gen__T_impl_ArrQue_Grip$(_T...) \
    union ArrQue_Grip$(_T) { \
        struct { \
            var_(buf, S$(_T)); \
            var_(head, usize*); \
            var_(len, usize*); \
            var_(ctx, ArrQue$(_T)); \
        }; \
        var_(as_raw, ArrQue_Grip) $like_ref; \
    }
#define __comp_gen__T_use_ArrQue_Grip$(_T...) \
    T_decl_ArrQue_Grip$(_T); \
    T_impl_ArrQue_Grip$(_T)

/* clang-format off */
#define T_use_ArrQue_grip$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_grip, _T)(S$(_T) buf, usize* head, usize* len))(ArrQue_Grip$(_T))) { \
        return type$((ArrDeq_Grip$(_T))(ArrDeq_grip(u_anyS(buf), head, len))); \
    }
#define T_use_ArrQue_Grip_release$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_Grip_release, _T)(P$$(ArrQue_Grip$(_T)) self))(void)) { \
        return ArrQue_Grip_release(self->as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

/* clang-format off */
#define T_use_ArrQue_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_len, _T)(ArrQue$(_T) self))(usize)) { \
        return ArrQue_len(*self.as_raw); \
    }
#define T_use_ArrQue_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_cap, _T)(ArrQue$(_T) self))(usize)) { \
        return ArrQue_cap(*self.as_raw); \
    }
#define T_use_ArrQue_head$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_head, _T)(ArrQue$(_T) self))(usize)) { \
        return ArrQue_head(*self.as_raw); \
    }
#define T_use_ArrQue_tail$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_tail, _T)(ArrQue$(_T) self))(usize)) { \
        return ArrQue_tail(*self.as_raw); \
    }
#define T_use_ArrQue_front$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_front, _T)(ArrQue$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrQue_front(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_frontMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_frontMut, _T)(ArrQue$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrQue_frontMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_back$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_back, _T)(ArrQue$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrQue_back(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_backMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_backMut, _T)(ArrQue$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrQue_backMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_at$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_at, _T)(ArrQue$(_T) self, usize idx))(const _T*)) { \
        return u_castP$((const _T*)(ArrQue_at(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrQue_atMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_atMut, _T)(ArrQue$(_T) self, usize idx))(_T*)) { \
        return u_castP$((_T*)(ArrQue_atMut(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrQue_buf$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_buf, _T)(ArrQue$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrQue_buf(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_bufMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_bufMut, _T)(ArrQue$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrQue_bufMut(*self.as_raw, typeInfo$(_T)))); \
    }

#define T_use_ArrQue_ensureCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_ensureCap, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrQue_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrQue_ensureCapPrecise$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_ensureCapPrecise, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrQue_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrQue_ensureUnusedCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_ensureUnusedCap, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrQue_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrQue_clearRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_clearRetainingCap, _T)(P$$(ArrQue$(_T)) self))(void)) { \
        return ArrQue_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrQue_clearAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_clearAndFree, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrQue_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrQue_enque$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_enque, _T)(P$$(ArrQue$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrQue_enque(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrQue_enqueFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrQue_enqueFixed, _T)(P$$(ArrQue$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrQue_enqueFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrQue_enqueWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_enqueWithin, _T)(P$$(ArrQue$(_T)) self, _T item))(void)) { \
        return ArrQue_enqueWithin(self->as_raw, u_anyV(item)); \
    }

#define T_use_ArrQue_deque$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_deque, _T)(P$$(ArrQue$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrQue_deque(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)

#define __comp_anon__ArrQue_Iter$$(_T...) \
    union { \
        struct { \
            var_(que, P_const$$(ArrQue$$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrQue_Iter) $like_ref; \
    }
#define __comp_alias__ArrQue_Iter$(_T...) pp_join($, ArrQue_Iter, _T)
#define __comp_gen__T_decl_ArrQue_Iter$(_T...) \
    $maybe_unused typedef union ArrQue_Iter$(_T) ArrQue_Iter$(_T)
#define __comp_gen__T_impl_ArrQue_Iter$(_T...) \
    union ArrQue_Iter$(_T) { \
        struct { \
            var_(que, P_const$$(ArrQue$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrQue_Iter) $like_ref; \
    }
#define __comp_gen__T_use_ArrQue_Iter$(_T...) \
    T_decl_ArrQue_Iter$(_T); \
    T_impl_ArrQue_Iter$(_T)

#define T_use_ArrQue_iter$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_iter, _T)(P_const$$(ArrQue$(_T)) self))(ArrQue_Iter$(_T))) { \
        return ArrQue_iter(self->as_raw, typeInfo$(_T)); \
    }
#define T_use_ArrQue_Iter_next$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_Iter_next, _T)(P$$(ArrQue_Iter$(_T)) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrQue_Iter_next(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrQue_Iter_nextMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrQue_Iter_nextMut, _T)(P$$(ArrQue_Iter$(_T)) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrQue_Iter_nextMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrQue__included */
