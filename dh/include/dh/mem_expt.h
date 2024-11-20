/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem_expt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-20 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  mem
 *
 * @brief   Memory management with Zig-style semantics
 * @details Provides type-safe memory management with clear separation between
 *          single-element pointers (Ptr) and multi-element views (Slice).
 *          Replaces anyptr with type-safe alternatives.
 */

#ifndef MEM_EXPT_INCLUDED
#define MEM_EXPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Core Types =====================================================*/

typedef struct Ptr {
    anyptr raw;
    usize  elem_size;
} Ptr;

static const Ptr Ptr_null = { null, 0 };

typedef struct Slice {
    Ptr   ptr;
    usize len;
} Slice;

static const Slice Slice_null = { Ptr_null, 0 };

typedef struct Allocator {
    anyptr (*allocRaw)(usize size);
    void (*freeRaw)(anyptr ptr);
    anyptr (*reallocRaw)(anyptr ptr, usize size);
} Allocator;

/*
 * static anyptr mem_general_allocRaw(usize size);
 * static void   mem_general_freeRaw(anyptr ptr);
 * static anyptr mem_general_reallocRaw(anyptr ptr, usize size);
 */
extern const Allocator mem_general;

/*========== Core Memory Operations =========================================*/

/* Regular functions for allocation */
Ptr   mem_createSize(const Allocator* alloc, usize elem_size);               // Base function
void  mem_destroy(const Allocator* alloc, Ptr ptr);                          // Base function
Ptr   mem_allocSize(const Allocator* alloc, usize elem_size);                // Base function
Slice mem_allocSizeN(const Allocator* alloc, usize elem_size, usize count);  // Base function
void  mem_free(const Allocator* alloc, Ptr ptr);                             // Base function
void  mem_freeSlice(const Allocator* alloc, Slice slice);                    // Base function
Slice mem_reallocSize(const Allocator* alloc, Slice slice, usize new_count); // Base function

/* Type-safe allocation wrappers */
#define mem_create(_alloc, T) \
    mem_createSize(_alloc, sizeof(T))

#define mem_alloc(_alloc, T) \
    mem_allocSize(_alloc, sizeof(T))

#define mem_allocN(_alloc, T, _count) \
    mem_allocSizeN(_alloc, sizeof(T), _count)

#define mem_realloc(_alloc, _slice, T, _new_count) \
    mem_reallocSize(_alloc, _slice, _new_count)

/* Raw memory operations (when needed) */
anyptr mem_allocRaw(const Allocator* alloc, usize size);                   // Base function
void   mem_freeRaw(const Allocator* alloc, anyptr ptr);                    // Base function
anyptr mem_reallocRaw(const Allocator* alloc, anyptr ptr, usize new_size); // Base function

/*========== Memory Manipulation ============================================*/

/* Regular functions */
void mem_copySize(Ptr dest, Ptr src, usize size); // Base function
void mem_moveSize(Ptr dest, Ptr src, usize size); // Base function
void mem_setSize(Ptr ptr, i32 val, usize size);   // Base function
i32  mem_cmpSize(Ptr lhs, Ptr rhs, usize size);   // Base function

/* Type-safe wrappers */
#define mem_copy(_dest, _src, T) \
    mem_copySize(_dest, _src, sizeof(T))

#define mem_move(_dest, _src, T) \
    mem_moveSize(_dest, _src, sizeof(T))

#define mem_set(_ptr, _value, T) \
    mem_setSize(_ptr, _value, sizeof(T))

#define mem_cmp(_lhs, _rhs, T) \
    mem_cmpSize(_lhs, _rhs, sizeof(T))

/*========== Ptr Operations =================================================*/

/* Regular functions */
Ptr    Ptr_from(anyptr raw, usize elem_size);      // Base function
bool   Ptr_isNull(Ptr p);                          // Base function
usize  Ptr_elemSize(Ptr p);                        // Base function
anyptr Ptr_raw(Ptr p);                             // Base function
anyptr Ptr_at(Ptr p, usize idx);                   // Base function
bool   Ptr_hasMinSize(Ptr p, usize required_size); // Base function

/* Type-safe wrappers */
#define Ptr_cast(_p, T) \
    (Ptr_hasMinSize(_p, sizeof(T)) ? (T*)Ptr_raw(_p) : null)

#define Ptr_atT(_p, _idx, T) \
    (Ptr_elemSize(_p) == sizeof(T) ? (T*)Ptr_at(_p, _idx) : null)

/*========== Slice Operations ===============================================*/

/* Regular functions */
Slice  Slice_from(Ptr ptr, usize len);                        // Base function
Slice  Slice_fromRaw(anyptr raw, usize elem_size, usize len); // Base function
bool   Slice_isNull(Slice sl);                                // Base function
bool   Slice_isEmpty(Slice sl);                               // Base function
usize  Slice_len(Slice sl);                                   // Base function
usize  Slice_elemSize(Slice sl);                              // Base function
Ptr    Slice_ptr(Slice sl);                                   // Base function
anyptr Slice_at(Slice sl, usize idx);                         // Base function
Slice  Slice_subslice(Slice sl, usize start, usize end);      // Base function
Slice  Slice_prefix(Slice sl, usize end);                     // Base function
Slice  Slice_suffix(Slice sl, usize start);                   // Base function

/* Type-safe wrappers */
#define Slice_cast(_sl, T) \
    (Ptr_elemSize(Slice_ptr(_sl)) == sizeof(T) ? (T*)Ptr_raw(Slice_ptr(_sl)) : null)

#define Slice_atT(_sl, _idx, T) \
    (((_idx) < Slice_len(_sl) && Ptr_elemSize(Slice_ptr(_sl)) == sizeof(T)) ? (T*)Slice_at(_sl, _idx) : null)

// NOLINTBEGIN(bugprone-macro-parentheses)
#define Slice_iter(_sl, T, _var)                            \
    for (                                                   \
        T* _var = Slice_cast(_sl, T);                       \
        _var && _var < Slice_cast(_sl, T) + Slice_len(_sl); \
        ++_var                                              \
    )
// NOLINTEND(bugprone-macro-parentheses)

/*========== Type Checking ==================================================*/

bool mem_isTypeSize(usize elem_size, usize type_size); // Base function

#define mem_isType(_ptrOrSlice, T) \
    mem_isTypeSize((_ptrOrSlice).elem_size, sizeof(T))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_EXPT_INCLUDED */
