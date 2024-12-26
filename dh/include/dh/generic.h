#ifndef GENERIC_INCLUDED
#define GENERIC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include <string.h>

/*========== Type Information System =========================================*/

// // Core type information (kept minimal - 8 bytes)
// /* Minimal type information */
// typedef struct TypeInfo {
//     usize size  : 52;
//     usize align : 12;
// } TypeInfo;
// #define typeInfo(T) (                                    \
//     (TypeInfo){ .size = sizeof(T), .align = alignof(T) } \
// )

// // Optional traits/interfaces
// typedef struct Copyable {
//     void (*copy)(void* dst, const void* src);
// } Copyable;

// typedef struct Equatable {
//     bool (*eq)(const void* a, const void* b);
// } Equatable;

// typedef struct Comparable {
//     i32 (*cmp)(const void* a, const void* b);
// } Comparable;

// typedef struct Hashable {
//     usize (*hash)(const void* value);
// } Hashable;

/*========== Generic Types =================================================*/

// Immutable and mutable pointer variants
typedef struct Ptr {
    void*          addr;
    const TypeInfo type;
} Ptr;

typedef struct PtrConst {
    const void*    addr;
    const TypeInfo type;
} PtrConst;

// Sentinel-terminated pointers
typedef struct PtrS {
    Ptr   core;
    void* sentinel;
} PtrS;

typedef struct PtrSConst {
    PtrConst    core;
    const void* sentinel;
} PtrSConst;

// Slices
typedef struct Sli {
    Ptr   ptr;
    usize len;
} Sli;

typedef struct SliConst {
    PtrConst ptr;
    usize    len;
} SliConst;

// Sentinel-terminated slices
typedef struct SliS {
    Sli   core;
    void* sentinel;
} SliS;

typedef struct SliSConst {
    SliConst    core;
    const void* sentinel;
} SliSConst;

// /* Combined single-item/many-item pointer (similar to *T/[*]T in Zig) */
// typedef struct Ptr {
//     void* addr;
// } Ptr;

// /* Sentinel-terminated pointer (similar to [*:sentinel]T in Zig) */
// typedef struct PtrS {
//     union {
//         Ptr core;
//         struct {
//             void* addr;
//         };
//     };
//     void* sentinel;
// } PtrS;

// /* Slice ([]T in Zig) */
// typedef struct Sli {
//     union {
//         Ptr    ptr;
//         anyptr addr;
//     };
//     usize len;
// } Sli;

// /* Sentinel-terminated slice ([:sentinel]T in Zig) */
// typedef struct SliS {
//     union {
//         Sli core;
//         struct {
//             union {
//                 Ptr    ptr;
//                 anyptr addr;
//             };
//             usize len;
//         };
//     };
//     anyptr sentinel;
// } SliS;

// /* Optional type */
// typedef struct Opt {
//     bool  has_value;
//     void* value;
// } Opt;

// /* Result tag type */
// typedef enum ResTag {
//     ResTag_err = 0,
//     ResTag_ok
// } ResTag;

// /* Core result type */
// typedef struct Res {
//     ResTag tag;
//     union {
//         anyptr value;
//         anyptr error;
//     };
// } Res;

// /* Core result default error type */
// typedef struct ResErr {
//     ResTag tag;
//     union {
//         anyptr value;
//         Err    error;
//     };
// } ResErr;

// /*========== Type Registration Macros =======================================*/

// // Optional trait implementations
// #define Gen_implCopyable(T, copyFn)        \
//     static const Copyable T##_copyable = { \
//         .copy = (copyFn)                   \
//     };                                     \
//     const Copyable* T##_asCopyable(void) { return &T##_copyable; }

// #define Gen_implEquatable(T, equalsFn)       \
//     static const Equatable T##_equatable = { \
//         .equals = (equalsFn)                 \
//     };                                       \
//     const Equatable* T##_asEquatable(void) { return &T##_equatable; }

// #define Gen_implComparable(T, compareFn)       \
//     static const Comparable T##_comparable = { \
//         .compare = (compareFn)                 \
//     };                                         \
//     const Comparable* T##_asComparable(void) { return &T##_comparable; }

/*========== Type-Safe Constructors ========================================*/

#define asPtr(T, p) \
    ((Ptr){ .addr = (p), .type = typeInfo(T) })

#define asPtrConst(T, p) \
    ((PtrConst){ .addr = (p), .type = typeInfo(T) })

#define asPtrS(T, p, s) \
    ((PtrS){ .core = asPtr(T, p), .sentinel = (s) })

#define asPtrSConst(T, p, s) \
    ((PtrSConst){ .core = asPtrConst(T, p), .sentinel = (s) })

#define asSli(T, p, l) \
    ((Sli){ .ptr = asPtr(T, p), .len = (l) })

#define asSliConst(T, p, l) \
    ((SliConst){ .ptr = asPtrConst(T, p), .len = (l) })

/*========== Generic Operations ===========================================*/

force_inline void* Ptr_at(Ptr self, usize index) {
    claim_assert_nonnull(self.addr);
    return (u8*)self.addr + (index * self.type.size);
}

force_inline const void* PtrConst_at(PtrConst self, usize index) {
    claim_assert_nonnull(self.addr);
    return (const u8*)self.addr + (index * self.type.size);
}

force_inline PtrConst Ptr_asConst(Ptr self) {
    claim_assert_nonnull(self.addr);
    return (PtrConst){
        .addr = self.addr,
        .type = self.type
    };
}

// // Cast operation for converting const to mutable (explicit and checked)
// force_inline bool Ptr_canCastFrom(PtrConst self) {
//     // Add your safety checks here
//     return true; // Simplified for example
// }

force_inline Ptr Ptr_constCast(PtrConst self) {
    claim_assert_nonnull(self.addr);
    // if (!Ptr_canCastFrom(self)) {
    //     return (Ptr){ null, self.type };
    // }
    return (Ptr){ (void*)self.addr, self.type };
}

// Slice operations
force_inline void* Sli_at(Sli self, usize index) {
    claim_assert_nonnull(self.ptr.addr);
    claim_assert_fmt(index < self.len, "Index out of bounds (len: %zu, index: %zu)", self.len, index);
    return Ptr_at(self.ptr, index);
}

force_inline const void* SliConst_at(SliConst self, usize index) {
    claim_assert_nonnull(self.ptr.addr);
    claim_assert_fmt(index < self.len, "Index out of bounds (len: %zu, index: %zu)", self.len, index);
    return PtrConst_at(self.ptr, index);
}

force_inline Sli Sli_slice(Sli self, usize begin, usize end) {
    claim_assert_nonnull(self.ptr.addr);
    claim_assert_fmt(begin <= end, "Invalid slice range (begin: %zu, end: %zu)", begin, end);
    claim_assert_fmt(begin <= self.len, "Slice begin out of bounds (len: %zu, begin: %zu)", self.len, begin);
    claim_assert_fmt(end <= self.len, "Slice end out of bounds (len: %zu, end: %zu)", self.len, end);
    return (Sli){
        .ptr = { Ptr_at(self.ptr, begin), self.ptr.type },
        .len = end - begin
    };
}

force_inline SliConst SliConst_slice(SliConst self, usize begin, usize end) {
    claim_assert_nonnull(self.ptr.addr);
    claim_assert_fmt(begin <= end, "Invalid slice range (begin: %zu, end: %zu)", begin, end);
    claim_assert_fmt(begin <= self.len, "Slice begin out of bounds (len: %zu, begin: %zu)", self.len, begin);
    claim_assert_fmt(end <= self.len, "Slice end out of bounds (len: %zu, end: %zu)", self.len, end);
    return (SliConst){
        .ptr = { PtrConst_at(self.ptr, begin), self.ptr.type },
        .len = end - begin
    };
}

// // Memory operations that use traits
// force_inline bool Equatable_eq(const Equatable* self, TypeInfo type, const void* lhs, const void* rhs) {
//     claim_assert_nonnull(self);
//     claim_assert_nonnull(lhs);
//     claim_assert_nonnull(rhs);
//     if (!self) {
//         return memcmp(lhs, rhs, type.size) == 0;
//     }
//     return self->eq(lhs, rhs);
// }

// force_inline void Copyable_copy(const Copyable* self, TypeInfo type, void* dst, const void* src) {
//     claim_assert_nonnull(self);
//     claim_assert_nonnull(dst);
//     claim_assert_nonnull(src);
//     if (!self) {
//         memcpy(dst, src, type.size);
//         return;
//     }
//     self->copy(dst, src);
// }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* GENERIC_INCLUDED */
