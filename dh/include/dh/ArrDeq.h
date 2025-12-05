/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrDeq.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-12-03 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ArrDeq
 *
 * @brief   Dynamic array deque implementation
 * @details This header provides a contiguous, growable, double-ended queue implementation with dynamic memory allocation.
 *          The array deque automatically resizes as elements are added or removed.
 *          Supports operations for initialization, modification, and element access.
 *          Pushing/popping items from either end of the queue is O(1).
 */
#ifndef ArrDeq__included
#define ArrDeq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrDeq Anonymous */
#define ArrDeq$$(_T...) __comp_anon__ArrDeq$$(_T)
/* ArrDeq Alias */
#define ArrDeq$(_T...) __comp_alias__ArrDeq$(_T)
/* ArrDeq Template */
#define T_decl_ArrDeq$(_T...) __comp_gen__T_decl_ArrDeq$(_T)
#define T_impl_ArrDeq$(_T...) __comp_gen__T_impl_ArrDeq$(_T)
#define T_use_ArrDeq$(_T...) __comp_gen__T_use_ArrDeq$(_T)

/* ArrDeq Raw Structure */
typedef struct ArrDeq {
    var_(buf, S$raw);
    var_(head, usize);
    var_(len, usize);
    debug_only(var_(type, TypeInfo);)
} ArrDeq;
T_use$((ArrDeq)(O, E));
T_use_E$($set(mem_Err)(ArrDeq));

extern fn_((ArrDeq_empty(TypeInfo type))(ArrDeq));
extern fn_((ArrDeq_fixed(u_S$raw buf))(ArrDeq));
extern fn_((ArrDeq_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrDeq)) $must_check;
extern fn_((ArrDeq_fini(ArrDeq* self, TypeInfo type, mem_Allocator gpa))(void));

typedef struct ArrDeq_Grip {
    var_(buf, S$raw);
    var_(head, usize*);
    var_(len, usize*);
    var_(ctx, ArrDeq);
} ArrDeq_Grip;
#define ArrDeq_Grip$$(_T...) __comp_anon__ArrDeq_Grip$$(_T)
#define ArrDeq_Grip$(_T...) __comp_alias__ArrDeq_Grip$(_T)
#define T_decl_ArrDeq_Grip$(_T...) __comp_gen__T_decl_ArrDeq_Grip$(_T)
#define T_impl_ArrDeq_Grip$(_T...) __comp_gen__T_impl_ArrDeq_Grip$(_T)
#define T_use_ArrDeq_Grip$(_T...) __comp_gen__T_use_ArrDeq_Grip$(_T)
extern fn_((ArrDeq_grip(u_S$raw buf, usize* head, usize* len))(ArrDeq_Grip));
extern fn_((ArrDeq_Grip_release(ArrDeq_Grip* self, TypeInfo type))(void));

extern fn_((ArrDeq_len(ArrDeq self))(usize));
extern fn_((ArrDeq_cap(ArrDeq self))(usize));
extern fn_((ArrDeq_isEmpty(ArrDeq self))(bool));
extern fn_((ArrDeq_isFull(ArrDeq self))(bool));
extern fn_((ArrDeq_head(ArrDeq self))(usize));
extern fn_((ArrDeq_tail(ArrDeq self))(usize));
extern fn_((ArrDeq_at(ArrDeq self, TypeInfo type, usize idx))(u_P_const$raw));
extern fn_((ArrDeq_atMut(ArrDeq self, TypeInfo type, usize idx))(u_P$raw));
extern fn_((ArrDeq_front(ArrDeq self, TypeInfo type))(O$u_P_const$raw));
extern fn_((ArrDeq_frontMut(ArrDeq self, TypeInfo type))(O$u_P$raw));
extern fn_((ArrDeq_back(ArrDeq self, TypeInfo type))(O$u_P_const$raw));
extern fn_((ArrDeq_backMut(ArrDeq self, TypeInfo type))(O$u_P$raw));
extern fn_((ArrDeq_buf(ArrDeq self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrDeq_bufMut(ArrDeq self, TypeInfo type))(u_S$raw));

extern fn_((ArrDeq_ensureCap(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_ensureCapPrecise(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_ensureUnusedCap(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_clearRetainingCap(ArrDeq* self))(void));
extern fn_((ArrDeq_clearAndFree(ArrDeq* self, TypeInfo type, mem_Allocator gpa))(void));

extern fn_((ArrDeq_append(ArrDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_appendFixed(ArrDeq* self, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_appendWithin(ArrDeq* self, u_V$raw item))(void));

extern fn_((ArrDeq_prepend(ArrDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_prependFixed(ArrDeq* self, u_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrDeq_prependWithin(ArrDeq* self, u_V$raw item))(void));

extern fn_((ArrDeq_pop(ArrDeq* self, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrDeq_shift(ArrDeq* self, u_V$raw ret_mem))(O$u_V$raw));

/* ArrDeq_Iter Anonymous */
#define ArrDeq_Iter$$(_T...) __comp_anon__ArrDeq_Iter$$(_T)
/* ArrDeq_Iter Alias */
#define ArrDeq_Iter$(_T...) __comp_alias__ArrDeq_Iter$(_T)
/* ArrDeq_Iter Template */
#define T_decl_ArrDeq_Iter$(_T...) __comp_gen__T_decl_ArrDeq_Iter$(_T)
#define T_impl_ArrDeq_Iter$(_T...) __comp_gen__T_impl_ArrDeq_Iter$(_T)
#define T_use_ArrDeq_Iter$(_T...) __comp_gen__T_use_ArrDeq_Iter$(_T)

/* ArrDeq_Iter Raw Structure */
typedef struct ArrDeq_Iter {
    var_(deq, const ArrDeq*);
    var_(idx, usize);
    debug_only(var_(type, TypeInfo);)
} ArrDeq_Iter;
extern fn_((ArrDeq_iter(const ArrDeq* self, TypeInfo type))(ArrDeq_Iter));
extern fn_((ArrDeq_Iter_next(ArrDeq_Iter* self, TypeInfo type))(O$u_P_const$raw));
extern fn_((ArrDeq_Iter_nextMut(ArrDeq_Iter* self, TypeInfo type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrDeq$$(_T...) \
    union { \
        struct { \
            var_(buf, S$$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrDeq) $like_ref; \
    }
#define __comp_alias__ArrDeq$(_T...) pp_join($, ArrDeq, _T)
#define __comp_gen__T_decl_ArrDeq$(_T...) \
    $maybe_unused typedef union ArrDeq$(_T) ArrDeq$(_T); \
    T_decl_O$(ArrDeq$(_T)); \
    T_decl_E$(ArrDeq$(_T)); \
    T_decl_E$($set(mem_Err)(ArrDeq$(_T)))
#define __comp_gen__T_impl_ArrDeq$(_T...) \
    union ArrDeq$(_T) { \
        struct { \
            var_(buf, S$(_T)); \
            var_(head, usize); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrDeq) $like_ref; \
    }; \
    T_impl_O$(ArrDeq$(_T)); \
    T_impl_E$(ArrDeq$(_T)); \
    T_impl_E$($set(mem_Err)(ArrDeq$(_T)))
#define __comp_gen__T_use_ArrDeq$(_T...) \
    T_decl_ArrDeq$(_T); \
    T_impl_ArrDeq$(_T)

/* clang-format off */
#define T_use_ArrDeq_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_empty, _T)(void))(ArrDeq$(_T))) { \
        return type$((ArrDeq$(_T))(ArrDeq_empty(typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_fixed$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_fixed, _T)(S$(_T) buf))(ArrDeq$(_T))) { \
        return type$((ArrDeq$(_T))(ArrDeq_fixed(u_anyS(buf)))); \
    }
#define T_use_ArrDeq_init$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_init, _T)(mem_Allocator gpa, usize cap))(E$($set(mem_Err)(ArrDeq$(_T)))) $scope) { \
        return_(typeE$((ReturnType)(ArrDeq_init(typeInfo$(_T), gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_ArrDeq_fini$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_fini, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrDeq_fini(self->as_raw, typeInfo$(_T), gpa); \
    }
/* clang-format on */

#define __comp_anon__ArrDeq_Grip$$(_T...) \
    union { \
        struct { \
            var_(buf, S$$(_T)); \
            var_(head, usize*); \
            var_(len, usize*); \
            var_(ctx, ArrDeq$$(_T)); \
        }; \
        var_(as_raw, ArrDeq_Grip) $like_ref; \
    }
#define __comp_alias__ArrDeq_Grip$(_T...) pp_join($, ArrDeq_Grip, _T)
#define __comp_gen__T_decl_ArrDeq_Grip$(_T...) \
    $maybe_unused typedef union ArrDeq_Grip$(_T) ArrDeq_Grip$(_T)
#define __comp_gen__T_impl_ArrDeq_Grip$(_T...) \
    union ArrDeq_Grip$(_T) { \
        struct { \
            var_(buf, S$(_T)); \
            var_(head, usize*); \
            var_(len, usize*); \
            var_(ctx, ArrDeq$(_T)); \
        }; \
        var_(as_raw, ArrDeq_Grip) $like_ref; \
    }
#define __comp_gen__T_use_ArrDeq_Grip$(_T...) \
    T_decl_ArrDeq_Grip$(_T); \
    T_impl_ArrDeq_Grip$(_T)

/* clang-format off */
#define T_use_ArrDeq_grip$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_grip, _T)(S$(_T) buf, usize* head, usize* len))(ArrDeq_Grip$(_T))) { \
        return type$((ArrDeq_Grip$(_T))(ArrDeq_grip(u_anyS(buf), head, len))); \
    }
#define T_use_ArrDeq_Grip_release$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_Grip_release, _T)(P$$(ArrDeq_Grip$(_T)) self))(void)) { \
        return ArrDeq_Grip_release(self->as_raw, typeInfo$(_T)); \
    }
/* clang-format on */

/* clang-format off */
#define T_use_ArrDeq_len$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_len, _T)(ArrDeq$(_T) self))(usize)) { \
        return ArrDeq_len(*self.as_raw); \
    }
#define T_use_ArrDeq_cap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_cap, _T)(ArrDeq$(_T) self))(usize)) { \
        return ArrDeq_cap(*self.as_raw); \
    }
#define T_use_ArrDeq_head$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_head, _T)(ArrDeq$(_T) self))(usize)) { \
        return ArrDeq_head(*self.as_raw); \
    }
#define T_use_ArrDeq_tail$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_tail, _T)(ArrDeq$(_T) self))(usize)) { \
        return ArrDeq_tail(*self.as_raw); \
    }
#define T_use_ArrDeq_front$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_front, _T)(ArrDeq$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrDeq_front(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_frontMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_frontMut, _T)(ArrDeq$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrDeq_frontMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_back$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_back, _T)(ArrDeq$(_T) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrDeq_back(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_backMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_backMut, _T)(ArrDeq$(_T) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrDeq_backMut(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_at$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_at, _T)(ArrDeq$(_T) self, usize idx))(const _T*)) { \
        return u_castP$((const _T*)(ArrDeq_at(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrDeq_atMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_atMut, _T)(ArrDeq$(_T) self, usize idx))(_T*)) { \
        return u_castP$((_T*)(ArrDeq_atMut(*self.as_raw, typeInfo$(_T), idx))); \
    }
#define T_use_ArrDeq_buf$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_buf, _T)(ArrDeq$(_T) self))(S$(const _T))) { \
        return u_castS$((S$(const _T))(ArrDeq_buf(*self.as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_bufMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_bufMut, _T)(ArrDeq$(_T) self))(S$(_T))) { \
        return u_castS$((S$(_T))(ArrDeq_bufMut(*self.as_raw, typeInfo$(_T)))); \
    }

#define T_use_ArrDeq_ensureCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_ensureCap, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrDeq_ensureCap(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrDeq_ensureCapPrecise$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_ensureCapPrecise, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) { \
        return ArrDeq_ensureCapPrecise(self->as_raw, typeInfo$(_T), gpa, new_cap); \
    }
#define T_use_ArrDeq_ensureUnusedCap$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_ensureUnusedCap, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa, usize additional))(mem_Err$void)) { \
        return ArrDeq_ensureUnusedCap(self->as_raw, typeInfo$(_T), gpa, additional); \
    }
#define T_use_ArrDeq_clearRetainingCap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_clearRetainingCap, _T)(P$$(ArrDeq$(_T)) self))(void)) { \
        return ArrDeq_clearRetainingCap(self->as_raw); \
    }
#define T_use_ArrDeq_clearAndFree$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_clearAndFree, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa))(void)) { \
        return ArrDeq_clearAndFree(self->as_raw, typeInfo$(_T), gpa); \
    }

#define T_use_ArrDeq_append$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_append, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrDeq_append(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrDeq_appendFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_appendFixed, _T)(P$$(ArrDeq$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrDeq_appendFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrDeq_appendWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_appendWithin, _T)(P$$(ArrDeq$(_T)) self, _T item))(void)) { \
        return ArrDeq_appendWithin(self->as_raw, u_anyV(item)); \
    }

#define T_use_ArrDeq_prepend$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_prepend, _T)(P$$(ArrDeq$(_T)) self, mem_Allocator gpa, _T item))(mem_Err$void)) { \
        return ArrDeq_prepend(self->as_raw, gpa, u_anyV(item)); \
    }
#define T_use_ArrDeq_prependFixed$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id(ArrDeq_prependFixed, _T)(P$$(ArrDeq$(_T)) self, _T item))(mem_Err$void)) { \
        return ArrDeq_prependFixed(self->as_raw, u_anyV(item)); \
    }
#define T_use_ArrDeq_prependWithin$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_prependWithin, _T)(P$$(ArrDeq$(_T)) self, _T item))(void)) { \
        return ArrDeq_prependWithin(self->as_raw, u_anyV(item)); \
    }

#define T_use_ArrDeq_pop$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_pop, _T)(P$$(ArrDeq$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrDeq_pop(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
#define T_use_ArrDeq_shift$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_shift, _T)(P$$(ArrDeq$(_T)) self))(O$(_T)) $scope) { \
        return_(u_castO$((ReturnType)(ArrDeq_shift(self->as_raw, u_retV$(_T))))); \
    } $unscoped_(fn)
/* clang-format on */

#define __comp_anon__ArrDeq_Iter$$(_T...) \
    union { \
        struct { \
            var_(deq, P_const$$(ArrDeq$$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrDeq_Iter) $like_ref; \
    }
#define __comp_alias__ArrDeq_Iter$(_T...) pp_join($, ArrDeq_Iter, _T)
#define __comp_gen__T_decl_ArrDeq_Iter$(_T...) \
    $maybe_unused typedef union ArrDeq_Iter$(_T) ArrDeq_Iter$(_T)
#define __comp_gen__T_impl_ArrDeq_Iter$(_T...) \
    union ArrDeq_Iter$(_T) { \
        struct { \
            var_(deq, P_const$$(ArrDeq$(_T))); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ArrDeq_Iter) $like_ref; \
    }
#define __comp_gen__T_use_ArrDeq_Iter$(_T...) \
    T_decl_ArrDeq_Iter$(_T); \
    T_impl_ArrDeq_Iter$(_T)

/* clang-format off */
#define T_use_ArrDeq_iter$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_iter, _T)(P_const$$(ArrDeq$(_T)) self))(ArrDeq_Iter$(_T))) { \
        return ArrDeq_iter(self->as_raw, typeInfo$(_T)); \
    }
#define T_use_ArrDeq_Iter_next$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_Iter_next, _T)(P$$(ArrDeq_Iter$(_T)) self))(O$(P_const$(_T)))) { \
        return u_castO$((O$(P_const$(_T)))(ArrDeq_Iter_next(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ArrDeq_Iter_nextMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(ArrDeq_Iter_nextMut, _T)(P$$(ArrDeq_Iter$(_T)) self))(O$(P$(_T)))) { \
        return u_castO$((O$(P$(_T)))(ArrDeq_Iter_nextMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrDeq__included */
