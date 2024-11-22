/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  mem
 *
 * @brief   Memory management with Zig-style semantics
 * @details Provides type-safe memory management with clear separation between
 *          single-element pointers (Ptr) and multi-element views (Slice).
 *          Replaces anyptr with type-safe alternatives.
 */

#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/core/Ptr.h"
#include "dh/core/Slice.h"

/*========== Core Types =====================================================*/

typedef struct mem_Allocator {
    anyptr (*allocRaw)(usize size);
    void (*freeRaw)(anyptr raw);
    anyptr (*reallocRaw)(anyptr raw, usize size);
} mem_Allocator;

// General allocator declaration
extern const mem_Allocator mem_general_purpose;
/*
 * static anyptr mem_general_allocRaw(usize size);
 * static void   mem_general_freeRaw(anyptr raw);
 * static anyptr mem_general_reallocRaw(anyptr raw, usize size);
 */

/*========== Core Memory Operations =========================================*/

/* Creation and destruction */
extern Ptr  mem_createSize(const mem_Allocator* alloc, usize elem_size); // Base function
extern void mem_destroy(const mem_Allocator* alloc, Ptr ptr);            // Base function

/* Regular functions for allocation */
extern Slice mem_allocSize(const mem_Allocator* alloc, usize elem_size, usize count);                    // Base function
extern void  mem_free(const mem_Allocator* alloc, Slice slice);                                          // Base function
extern Slice mem_reallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count); // Base function

/* Type-safe allocation wrappers */
#define mem_create(_alloc, T, ...) \
    (*(Error*)mem_createSize(_alloc, sizeof(T), __VA_ARGS__))

#define mem_alloc(_alloc, T, _count, ...) \
    (*(Error*)mem_allocSize(_alloc, sizeof(T), _count, __VA_ARGS__))

#define mem_realloc(_alloc, _slice, T, _new_count) \
    mem_reallocSize(_alloc, _slice, sizeof(T), _new_count)

/* Raw memory operations (when needed) */
extern anyptr mem_allocRaw(const mem_Allocator* alloc, usize size);                   // Base function
extern void   mem_freeRaw(const mem_Allocator* alloc, anyptr raw);                    // Base function
extern anyptr mem_reallocRaw(const mem_Allocator* alloc, anyptr raw, usize new_size); // Base function

/*========== Memory Manipulation ============================================*/

/* Regular functions */
extern void mem_copySize(Ptr dest, Ptr src, usize size); // Base function
extern void mem_moveSize(Ptr dest, Ptr src, usize size); // Base function
extern void mem_setSize(Ptr ptr, i32 val, usize size);   // Base function
extern i32  mem_cmpSize(Ptr lhs, Ptr rhs, usize size);   // Base function

/* Type-safe wrappers */
#define mem_copy(_dest, _src, T) \
    mem_copySize(_dest, _src, sizeof(T))

#define mem_move(_dest, _src, T) \
    mem_moveSize(_dest, _src, sizeof(T))

#define mem_set(_ptr, _value, T) \
    mem_setSize(_ptr, _value, sizeof(T))

#define mem_cmp(_lhs, _rhs, T) \
    mem_cmpSize(_lhs, _rhs, sizeof(T))

/* Raw memory operations (when needed) */
extern void mem_copyRaw(anyptr dest, anyptr src, usize size); // Base function
extern void mem_moveRaw(anyptr dest, anyptr src, usize size); // Base function
extern void mem_setRaw(anyptr raw, i32 val, usize size);      // Base function
extern i32  mem_cmpRaw(anyptr lhs, anyptr rhs, usize size);   // Base function

/*========== Ptr Operations =================================================*/

/* Construction */
extern Ptr Ptr_from(anyptr raw, usize elem_size);

/* Properties */
extern bool  Ptr_isNull(Ptr p);
extern usize Ptr_elemSize(Ptr p);

/* Access */
extern anyptr Ptr_raw(Ptr p);   // Generic pointer access
extern u8*    Ptr_bytes(Ptr p); // Byte access when needed

/* Offset operations */
extern Ptr Ptr_offset(Ptr p, usize count);           // Offset by elements
extern Ptr Ptr_byteOffset(Ptr p, usize byte_offset); // Offset by bytes

/* Validation */
extern bool Ptr_hasMinSize(Ptr p, usize required_size);

/* Type-safe access wrappers */
#define Ptr_as(_p, T)                                                                 \
    /* Convert when size is sufficient (e.g., base class ptr to derived class ptr) */ \
    (Ptr_hasMinSize(_p, sizeof(T)) ? (T*)Ptr_raw(_p) : null)

#define Ptr_cast(_p, T)                                                  \
    /* Convert only when sizes match exactly (e.g., int32 to float32) */ \
    (Ptr_elemSize(_p) == sizeof(T) ? (T*)Ptr_raw(_p) : null)

/*========== Slice Operations ===============================================*/

/* Construction */
extern Slice Slice_from(Ptr ptr, usize len);
extern Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len);

/* Properties */
extern bool  Slice_isNull(Slice sl);
extern bool  Slice_isEmpty(Slice sl);
extern usize Slice_len(Slice sl);
extern usize Slice_elemSize(Slice sl);

/* Access */
extern Ptr    Slice_ptr(Slice sl);   // Pointer access
extern anyptr Slice_raw(Slice sl);   // Generic pointer access
extern u8*    Slice_bytes(Slice sl); // Byte access when needed

/* Element access */
extern anyptr Slice_atRaw(Slice sl, usize idx);          // Access by raw element
extern u8*    Slice_byteAt(Slice sl, usize byte_offset); // Access by byte offset

/* Slicing */
extern Slice Slice_subslice(Slice sl, usize start, usize end);  // By elements
extern Slice Slice_byteSlice(Slice sl, usize start, usize end); // By bytes
extern Slice Slice_prefix(Slice sl, usize end);
extern Slice Slice_suffix(Slice sl, usize start);

/* Type-safe wrappers */
#define Slice_as(_sl, T)                  \
    /* Convert when size is sufficient */ \
    (Slice_elemSize(_sl) >= sizeof(T) ? (T*)Slice_raw(_sl) : null)

#define Slice_cast(_sl, T)                      \
    /* Convert only when sizes match exactly */ \
    (Slice_elemSize(_sl) == sizeof(T) ? (T*)Slice_raw(_sl) : null)

// Element access follows the same pattern
#define Slice_atAs(_sl, _idx, T) \
    (((_idx) < Slice_len(_sl) && Slice_elemSize(_sl) >= sizeof(T)) ? (T*)Slice_at(_sl, _idx) : null)

#define Slice_atT(_sl, _idx, T) \
    (((_idx) < Slice_len(_sl) && Slice_elemSize(_sl) == sizeof(T)) ? (T*)Slice_at(_sl, _idx) : null)

// NOLINTBEGIN(bugprone-macro-parentheses)
#define Slice_iter(_sl, T, _var)                             \
    for (T* _var = Slice_cast(_sl, T);                       \
         _var && _var < Slice_cast(_sl, T) + Slice_len(_sl); \
         ++_var)
// NOLINTEND(bugprone-macro-parentheses)

/*========== Type Checking ==================================================*/

extern bool mem_isTypeSize(usize elem_size, usize type_size); // Base function

#define mem_isType(_ptrOrSlice, T) \
    mem_isTypeSize((_ptrOrSlice).elem_size, sizeof(T))

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_INCLUDED */
