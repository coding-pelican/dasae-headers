// ptr_types.h
#ifndef PTR_TYPES_H
#define PTR_TYPES_H

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/core/prim/ptr.h"

#include <assert.h>
#include <string.h>

/*========== Metadata Layout ===============================================*/

// Metadata bit layout for 64-bit systems
#define PTR_BITS_ADDR_LEN  (48) // 256 TB address space
#define PTR_BITS_SIZE_LEN  (12) // Up to 4096 bytes size
#define PTR_BITS_ALIGN_LEN (3)  // Up to 128 bytes alignment
#define PTR_BITS_FLAGS_LEN (1)  // Boolean flags

// Masks and shifts
#define PTR_MASK_BITS_SIZE  ((1ull << PTR_BITS_SIZE_LEN) - 1)
#define PTR_MASK_BITS_ALIGN ((1ull << PTR_BITS_ALIGN_LEN) - 1)
#define PTR_MASK_BITS_FLAGS ((1ull << PTR_BITS_FLAGS_LEN) - 1)

#define PTR_SHIFT_BITS_SIZE  (0)
#define PTR_SHIFT_BITS_ALIGN (PTR_BITS_SIZE_LEN)

/*========== Core Pointer Types =========================================*/

// Base const pointer type
typedef union Ptr {
    const anyptr raw;  // Direct pointer access
    uptr         bits; // Raw bits access
    struct {
        uptr addr : PTR_BITS_ADDR_LEN;   // Address bits
        uptr size : PTR_BITS_SIZE_LEN;   // Size bits
        uptr align : PTR_BITS_ALIGN_LEN; // Alignment bits
        uptr flags : PTR_BITS_FLAGS_LEN; // Flags bits
    } meta;
} Ptr;

// Base mutable pointer type
typedef union PtrMut {
    anyptr raw;  // Direct pointer access
    uptr   bits; // Raw bits access
    struct {
        uptr addr : PTR_BITS_ADDR_LEN;   // Address bits
        uptr size : PTR_BITS_SIZE_LEN;   // Size bits
        uptr align : PTR_BITS_ALIGN_LEN; // Alignment bits
        uptr flags : PTR_BITS_FLAGS_LEN; // Flags bits
    } meta;
} PtrMut;

/*========== Single-Item Pointer Types =================================*/

typedef struct SPtr {
    Ptr core;
} SPtr; // Single const item

typedef struct SPtrMut {
    PtrMut core;
} SPtrMut; // Single mutable item

/*========== Many-Item Pointer Types ===================================*/

typedef struct MPtr {
    Ptr core;
} MPtr; // Many const items

typedef struct MPtrMut {
    PtrMut core;
} MPtrMut; // Many mutable items

/*========== Array Pointer Types =======================================*/

typedef struct ArrPtr {
    Ptr   ptr;
    usize len;
} ArrPtr; // Const array pointer

typedef struct ArrPtrMut {
    PtrMut ptr;
    usize  len;
} ArrPtrMut; // Mutable array pointer

/*========== Slice Types ==============================================*/

typedef struct Slice {
    Ptr   ptr;
    usize len;
} Slice; // Const slice

typedef struct SliceMut {
    PtrMut ptr;
    usize  len;
} SliceMut; // Mutable slice

/*========== Sentinel Types ==========================================*/

typedef struct SentinelSlice {
    Ptr   ptr;
    usize len;
    u8    sentinel[16];
    usize sentinel_size;
} SentinelSlice; // Const sentinel-terminated slice

typedef struct SentinelSliceMut {
    PtrMut ptr;
    usize  len;
    u8     sentinel[16];
    usize  sentinel_size;
} SentinelSliceMut; // Mutable sentinel-terminated slice

/*========== Type Conversion Functions ===============================*/

// Convert mutable to const pointers
static inline Ptr Ptr_asConst(PtrMut ptr) {
    return (Ptr){ .bits = ptr.bits };
}

static inline SPtr SPtr_asConst(SPtrMut ptr) {
    return (SPtr){ .core = Ptr_asConst(ptr.core) };
}

static inline MPtr MPtr_asConst(MPtrMut ptr) {
    return (MPtr){ .core = Ptr_asConst(ptr.core) };
}

static inline ArrPtr ArrPtr_asConst(ArrPtrMut ptr) {
    return (ArrPtr){
        .ptr = Ptr_asConst(ptr.ptr),
        .len = ptr.len
    };
}

static inline Slice Slice_asConst(SliceMut slice) {
    return (Slice){
        .ptr = Ptr_asConst(slice.ptr),
        .len = slice.len
    };
}

/*========== Creation Macros =========================================*/

// Create const pointers
#define Ptr_new(T, ptr) \

#define SPtr_new(T, ptr) \

#define ManyPtr_new(T, ptr) \

#define ArrayPtr_new(T, ptr, count) \

#define Slice_new(T, ptr, count) \

// Create mutable pointers
#define PtrMut_new(T, ptr) \

#define SPtrMut_new(T, ptr) \

#define ManyPtrMut_new(T, ptr) \

#define ArrayPtrMut_new(T, ptr, count) \

#define SliceMut_new(T, ptr, count) \

/*========== Access Macros ===========================================*/

// Read-only access
#define ptr_read(T, ptr) \
    (*(const T*)((ptr).core.raw))

#define slice_get(T, slice, index) \
    (*((const T*)(char*)((slice).core.raw) + (index)))

// Mutable access
#define ptr_write(T, ptr, value) \
    (*(T*)((ptr).core.raw) = (value))

#define slice_set(T, slice, index, value) \
    (*(T*)((char*)((slice).core.raw) + (index)) = (value))

#endif // PTR_TYPES_H
