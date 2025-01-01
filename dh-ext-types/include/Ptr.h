/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Implementation of Zig-style pointers with reference wrapper
 * @details
 */

#ifndef PTR_INCLUDED
#define PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "mem/common.h"

/*========== Macros and Definitions =========================================*/

/* Combined single-item/many-item pointer (similar to *T/[*]T in Zig) */
typedef struct Ptr {
    anyptr   addr;
    TypeInfo type;
} Ptr;

/* Sentinel-terminated pointer (similar to [*:sentinel]T in Zig) */
typedef struct PtrS {
    union {
        Ptr core;
        struct {
            anyptr   addr;
            TypeInfo type;
        };
    };
    anyptr sentinel;
} PtrS;

/* Core pointer operations */
force_inline Ptr    Ptr_from(TypeInfo type, anyptr addr);
force_inline anyptr Ptr_at(TypeInfo type, Ptr self, usize index);

/* Sentinel pointer operations */
force_inline PtrS   PtrS_from(TypeInfo type, anyptr addr, anyptr sentinel);
force_inline anyptr PtrS_at(TypeInfo type, PtrS self, usize index);
force_inline anyptr PtrS_sentinel(TypeInfo type, PtrS self);
force_inline bool   PtrS_isSentinel(TypeInfo type, PtrS self, usize index);

/*========== Implementations ================================================*/

force_inline Ptr Ptr_from(TypeInfo type, anyptr addr) {
    debug_assert_nonnull(addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        mem_isAligned(as(usize, addr), type.align),
        "Address must be properly aligned (addr: %p, type.align: %zu)",
        addr,
        type.align
    );
    return (Ptr){ .addr = addr, .type = type };
}

force_inline anyptr Ptr_at(TypeInfo type, Ptr self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    return as(u8*, self.addr) + (index * type.size);
}

force_inline PtrS PtrS_from(TypeInfo type, anyptr addr, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        mem_isAligned(as(usize, addr), type.align),
        "Address must be properly aligned (addr: %p, type.align: %zu)",
        addr,
        type.align
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, sentinel), type.align),
        "Sentinel must be properly aligned (sentinel: %p, type.align: %zu)",
        sentinel,
        type.align
    );
    return (PtrS){
        .addr     = addr,
        .type     = type,
        .sentinel = sentinel
    };
}

force_inline anyptr PtrS_at(TypeInfo type, PtrS self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    return as(u8*, self.addr) + (index * type.size);
}

force_inline anyptr PtrS_sentinel(TypeInfo type, PtrS self) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    return self.sentinel;
}

force_inline bool PtrS_isSentinel(TypeInfo type, PtrS self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    return mem_cmp(PtrS_at(type, self, index), self.sentinel, type.size) == 0;
}

// #define IMPL_using_Ptr(T, AliasRef, AliasPtr, AliasPtrS)                                             \
//     /* Reference wrapper struct */                                                                   \
//     typedef struct {                                                                                 \
//         T value;                                                                                     \
//     } AliasRef; /* NOLINT */                                                                         \
//     /* Pointer types with reference wrapper */                                                       \
//     typedef union {                                                                                  \
//         Ptr base;                                                                                    \
//         union {                                                                                      \
//             rawptr(T) addr;                                                                          \
//             rawptr(AliasRef) ref;                                                                    \
//         };                                                                                           \
//     } AliasPtr; /* NOLINT */                                                                         \
//     typedef union {                                                                                  \
//         PtrS base;                                                                                   \
//         struct {                                                                                     \
//             union {                                                                                  \
//                 AliasPtr core;                                                                       \
//                 union {                                                                              \
//                     rawptr(T) addr;                                                                  \
//                     rawptr(AliasRef) ref;                                                            \
//                 };                                                                                   \
//             };                                                                                       \
//             AliasRef sentinel[1];                                                                    \
//         };                                                                                           \
//     } AliasPtrS; /* NOLINT */                                                                        \
//     /* Pointer interface */                                                                          \
//     force_inline AliasPtr pp_join(_, AliasPtr, from)(rawptr(T) addr) {                               \
//         return (AliasPtr){ .base = Ptr_from(typeInfo(AliasRef), as(anyptr, addr)) };                 \
//     }                                                                                                \
//     force_inline rawptr(AliasRef) pp_join(_, AliasPtr, at)(AliasPtr self, usize index) {             \
//         return Ptr_at(typeInfo(AliasRef), self.base, index);                                         \
//     }                                                                                                \
//     /* Sentinel pointer interface */                                                                 \
//     force_inline AliasPtrS pp_join(_, AliasPtrS, from)(rawptr(T) addr, T sentinel) {                 \
//         AliasPtrS result = {                                                                         \
//             .sentinel[0].value = sentinel                                                            \
//         };                                                                                           \
//         return (AliasPtrS){                                                                          \
//             .base = PtrS_from(typeInfo(AliasRef), as(anyptr, addr), as(anyptr, &result.sentinel[0])) \
//         };                                                                                           \
//     }                                                                                                \
//     force_inline rawptr(AliasRef) pp_join(_, AliasPtrS, at)(AliasPtrS self, usize index) {           \
//         return PtrS_at(typeInfo(AliasRef), self.base, index);                                        \
//     }                                                                                                \
//     force_inline rawptr(AliasRef) pp_join(_, AliasPtrS, sentinel)(AliasPtrS self) {                  \
//         return PtrS_sentinel(typeInfo(AliasRef), self.base);                                         \
//     }                                                                                                \
//     force_inline bool pp_join(_, AliasPtrS, isSentinel)(AliasPtrS self, usize index) {               \
//         return PtrS_isSentinel(typeInfo(AliasRef), self.base, index);                                \
//     }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PTR_INCLUDED */
