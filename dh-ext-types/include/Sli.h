/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Implementation of Zig-style slices with reference wrapper
 * @details
 */

#ifndef SLI_INCLUDED
#define SLI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Ptr.h"

/*========== Macros and Definitions =========================================*/

/* Slice ([]T in Zig) */
typedef struct Sli {
    union {
        Ptr ptr;
        struct {
            anyptr   addr;
            TypeInfo type;
        };
    };
    usize len;
} Sli;

/* Sentinel-terminated slice ([:sentinel]T in Zig) */
typedef struct SliS {
    union {
        Sli core;
        struct {
            union {
                Ptr ptr;
                struct {
                    anyptr   addr;
                    TypeInfo type;
                };
            };
            usize len;
        };
    };
    anyptr sentinel;
} SliS;

/* Core slice operations */
force_inline Sli    Sli_from(TypeInfo type, anyptr addr, usize begin, usize end);
force_inline anyptr Sli_at(TypeInfo type, Sli self, usize index);
force_inline Sli    Sli_slice(TypeInfo type, Sli self, usize begin, usize end);

/* Sentinel slice operations */
force_inline SliS   SliS_from(TypeInfo type, anyptr addr, usize begin, usize end, anyptr sentinel);
force_inline anyptr SliS_at(TypeInfo type, SliS self, usize index);
force_inline Sli    SliS_slice(TypeInfo type, SliS self, usize begin, usize end);
force_inline SliS   SliS_sliceS(TypeInfo type, SliS self, usize begin, usize end, anyptr sentinel);

/* Span functions */
/* Calculate length of sentinel-terminated sequence */
force_inline usize mem_slen(TypeInfo type, anyptr addr, anyptr sentinel);
/* Create slice from sentinel-terminated pointer */
force_inline Sli   mem_span(TypeInfo type, PtrS ptr);
/* Create sentinel-terminated slice from sentinel-terminated pointer with new sentinel */
force_inline SliS  mem_spanS(TypeInfo type, PtrS ptr, anyptr new_sentinel);

/*========== Implementations ================================================*/

force_inline Sli Sli_from(TypeInfo type, anyptr addr, usize begin, usize end) {
    debug_assert_nonnull(addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        begin < end,
        "Begin index must be less than end index (begin: %zu, end: %zu)",
        begin,
        end
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, addr), type.align),
        "Address must be properly aligned (addr: %p, type.align: %zu)",
        addr,
        type.align
    );
    return (Sli){
        .addr = as(u8*, addr) + (begin * type.size),
        .type = type,
        .len  = end - begin
    };
}

force_inline anyptr Sli_at(TypeInfo type, Sli self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        index < self.len,
        "Index out of bounds (index: %zu, len: %zu)",
        index,
        self.len
    );
    return as(u8*, self.addr) + (index * type.size);
}

force_inline Sli Sli_slice(TypeInfo type, Sli self, usize begin, usize end) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        begin < end,
        "Begin index must be less than end index (begin: %zu, end: %zu)",
        begin,
        end
    );
    debug_assert_fmt(
        end <= self.len,
        "End index out of bounds (end: %zu, len: %zu)",
        end,
        self.len
    );
    return (Sli){
        .addr = as(u8*, self.addr) + (begin * type.size),
        .type = type,
        .len  = end - begin
    };
}

force_inline SliS SliS_from(TypeInfo type, anyptr addr, usize begin, usize end, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        begin < end,
        "Begin index must be less than end index (begin: %zu, end: %zu)",
        begin,
        end
    );
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
    return (SliS){
        .addr     = as(u8*, addr) + (begin * type.size),
        .type     = type,
        .len      = end - begin,
        .sentinel = sentinel
    };
}

force_inline anyptr SliS_at(TypeInfo type, SliS self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        index < self.len,
        "Index out of bounds (index: %zu, len: %zu)",
        index,
        self.len
    );
    return as(u8*, self.addr) + (index * type.size);
}

force_inline Sli SliS_slice(TypeInfo type, SliS self, usize begin, usize end) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        begin < end,
        "Begin index must be less than end index (begin: %zu, end: %zu)",
        begin,
        end
    );
    debug_assert_fmt(
        end <= self.len,
        "End index out of bounds (end: %zu, len: %zu)",
        end,
        self.len
    );
    return (Sli){
        .addr = as(u8*, self.addr) + (begin * type.size),
        .type = type,
        .len  = end - begin
    };
}

force_inline SliS SliS_sliceS(TypeInfo type, SliS self, usize begin, usize end, anyptr sentinel) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        begin < end,
        "Begin index must be less than end index (begin: %zu, end: %zu)",
        begin,
        end
    );
    debug_assert_fmt(
        end <= self.len,
        "End index out of bounds (end: %zu, len: %zu)",
        end,
        self.len
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, sentinel), type.align),
        "Sentinel must be properly aligned (sentinel: %p, type.align: %zu)",
        sentinel,
        type.align
    );
    return (SliS){
        .addr     = as(u8*, self.addr) + (begin * type.size),
        .type     = type,
        .len      = end - begin,
        .sentinel = sentinel
    };
}

// #define IMPL_using_Sli(T, AliasRef, AliasPtr, AliasPtrS, AliasSli, AliasSliS)                                    \
//     /* Slice types with reference wrapper */                                                                     \
//     typedef union {                                                                                              \
//         Sli base;                                                                                                \
//         struct {                                                                                                 \
//             union {                                                                                              \
//                 AliasPtr ptr;                                                                                    \
//                 rawptr(T) addr;                                                                                  \
//                 rawptr(AliasRef) ref;                                                                            \
//             };                                                                                                   \
//             usize len;                                                                                           \
//         };                                                                                                       \
//     } AliasSli; /* NOLINT */                                                                                     \
//     typedef union {                                                                                              \
//         SliS base;                                                                                               \
//         struct {                                                                                                 \
//             union {                                                                                              \
//                 AliasSli core;                                                                                   \
//                 struct {                                                                                         \
//                     union {                                                                                      \
//                         AliasPtr ptr;                                                                            \
//                         rawptr(T) addr;                                                                          \
//                         rawptr(AliasRef) ref;                                                                    \
//                     };                                                                                           \
//                     usize len;                                                                                   \
//                 };                                                                                               \
//             };                                                                                                   \
//             AliasRef sentinel[1];                                                                                \
//         };                                                                                                       \
//     } AliasSliS; /* NOLINT */                                                                                    \
//     /* Slice interface */                                                                                        \
//     force_inline AliasSli pp_join(_, AliasSli, from)(rawptr(T) addr, usize begin, usize end) {                   \
//         return (AliasSli){                                                                                       \
//             .base = Sli_from(typeInfo(AliasRef), as(anyptr, addr), begin, end)                                   \
//         };                                                                                                       \
//     }                                                                                                            \
//     force_inline rawptr(AliasRef) pp_join(_, AliasSli, at)(AliasSli self, usize index) {                         \
//         return Sli_at(typeInfo(AliasRef), self.base, index);                                                     \
//     }                                                                                                            \
//     force_inline AliasSli pp_join(_, AliasSli, slice)(AliasSli self, usize begin, usize end) {                   \
//         return (AliasSli){                                                                                       \
//             .base = Sli_slice(typeInfo(AliasRef), self.base, begin, end)                                         \
//         };                                                                                                       \
//     }                                                                                                            \
//     /* Sentinel slice interface */                                                                               \
//     force_inline AliasSliS pp_join(_, AliasSliS, from)(rawptr(T) addr, usize begin, usize end, T sentinel) {     \
//         AliasSliS result = {                                                                                     \
//             .sentinel[0].value = sentinel,                                                                       \
//         };                                                                                                       \
//         return (AliasSliS){                                                                                      \
//             .base = SliS_from(typeInfo(AliasRef), as(anyptr, addr), begin, end, as(anyptr, &result.sentinel[0])) \
//         };                                                                                                       \
//     }                                                                                                            \
//     force_inline rawptr(AliasRef) pp_join(_, AliasSliS, at)(AliasSliS self, usize index) {                       \
//         return SliS_at(typeInfo(AliasRef), self.base, index);                                                    \
//     }                                                                                                            \
//     force_inline AliasSli pp_join(_, AliasSliS, slice)(AliasSliS self, usize begin, usize end) {                 \
//         return (AliasSli){                                                                                       \
//             .base = SliS_slice(typeInfo(AliasRef), self.base, begin, end)                                        \
//         };                                                                                                       \
//     }                                                                                                            \
//     force_inline AliasSliS pp_join(_, AliasSliS, sliceS)(AliasSliS self, usize begin, usize end, T sentinel) {   \
//         AliasSliS result = {                                                                                     \
//             .sentinel[0].value = sentinel,                                                                       \
//         };                                                                                                       \
//         return (AliasSliS){                                                                                      \
//             .base = SliS_sliceS(typeInfo(AliasRef), self.base, begin, end, as(anyptr, &result.sentinel[0]))      \
//         };                                                                                                       \
//     }

force_inline usize mem_slen(TypeInfo type, anyptr addr, anyptr sentinel) {
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
    usize len  = 0;
    u8*   curr = as(u8*, addr);
    while (mem_cmp(curr, sentinel, type.size) != 0) {
        len++;
        curr += type.size;
    }
    return len;
}

force_inline Sli mem_span(TypeInfo type, PtrS ptr) {
    debug_assert_nonnull(ptr.addr);
    debug_assert_nonnull(ptr.sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        mem_isAligned(as(usize, ptr.addr), type.align),
        "Address must be properly aligned (ptr.addr: %p, type.align: %zu)",
        ptr.addr,
        type.align
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, ptr.sentinel), type.align),
        "Sentinel must be properly aligned (ptr.sentinel: %p, type.align: %zu)",
        ptr.sentinel,
        type.align
    );
    return (Sli){
        .addr = ptr.addr,
        .type = type,
        .len  = mem_slen(type, ptr.addr, ptr.sentinel)
    };
}

force_inline SliS mem_spanS(TypeInfo type, PtrS ptr, anyptr new_sentinel) {
    debug_assert_nonnull(ptr.addr);
    debug_assert_nonnull(ptr.sentinel);
    debug_assert_nonnull(new_sentinel);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_fmt(
        mem_isAligned(as(usize, ptr.addr), type.align),
        "Address must be properly aligned (ptr.addr: %p, type.align: %zu)",
        ptr.addr,
        type.align
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, ptr.sentinel), type.align),
        "Sentinel must be properly aligned (ptr.sentinel: %p, type.align: %zu)",
        ptr.sentinel,
        type.align
    );
    debug_assert_fmt(
        mem_isAligned(as(usize, new_sentinel), type.align),
        "New sentinel must be properly aligned (new_sentinel: %p, type.align: %zu)",
        new_sentinel,
        type.align
    );
    return (SliS){
        .addr     = ptr.addr,
        .type     = type,
        .len      = mem_slen(type, ptr.addr, ptr.sentinel),
        .sentinel = new_sentinel
    };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */
