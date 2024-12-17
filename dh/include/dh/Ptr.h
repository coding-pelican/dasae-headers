/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-17 (date of last update)
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

#include "core/prim.h"
#include "debug/assert.h"
#include "mem/common.h"
#include "TypeInfo.h"

/*========== Macros and Definitions =========================================*/

/* Combined single-item/many-item pointer (similar to *T/[*]T in Zig) */
typedef struct Ptr {
    anyptr addr;
} Ptr;

/* Sentinel-terminated pointer (similar to [*:sentinel]T in Zig) */
typedef struct SPtr {
    anyptr addr;
    anyptr sentinel;
} SPtr;

/* Core pointer operations */
force_inline Ptr    Ptr_from(TypeInfo type, anyptr addr);
force_inline anyptr Ptr_at(TypeInfo type, Ptr self, usize index);

/* Sentinel pointer operations */
force_inline SPtr   SPtr_from(TypeInfo type, anyptr addr, anyptr sentinel);
force_inline anyptr SPtr_at(TypeInfo type, SPtr self, usize index);
force_inline anyptr SPtr_sentinel(TypeInfo type, SPtr self);
force_inline bool   SPtr_isSentinel(TypeInfo type, SPtr self, usize index);

#define using_Ptr(T)      IMPL_using_Ptr(T, pp_join(_, Ref, T), pp_join(_, Ptr, T), pp_join(_, SPtr, T))
#define using_PtrConst(T) IMPL_using_Ptr(const T, pp_join(_, RefConst, T), pp_join(_, PtrConst, T), pp_join(_, SPtrConst, T))

/* Implementation */

force_inline Ptr Ptr_from(TypeInfo type, anyptr addr) {
    debug_assert_nonnull(addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(mem_isAligned(as(usize, addr), type.align), "Address must be properly aligned");
    return (Ptr){ .addr = addr };
}

force_inline anyptr Ptr_at(TypeInfo type, Ptr self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    return (u8*)self.addr + (index * type.size);
}

force_inline SPtr SPtr_from(TypeInfo type, anyptr addr, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(mem_isAligned(as(usize, addr), type.align), "Address must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, sentinel), type.align), "Sentinel must be properly aligned");
    return (SPtr){
        .addr     = addr,
        .sentinel = sentinel
    };
}

force_inline anyptr SPtr_at(TypeInfo type, SPtr self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    return as(u8*, self.addr) + (index * type.size);
}

force_inline anyptr SPtr_sentinel(TypeInfo type, SPtr self) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    return self.sentinel;
}

force_inline bool SPtr_isSentinel(TypeInfo type, SPtr self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(self.sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    return mem_cmp(SPtr_at(type, self, index), self.sentinel, type.size) == 0;
}

#define IMPL_using_Ptr(T, AliasRef, AliasPtr, AliasSPtr)                                             \
    /* Reference wrapper struct */                                                                   \
    typedef struct {                                                                                 \
        T value;                                                                                     \
    } AliasRef; /* NOLINT */                                                                         \
    /* Pointer types with reference wrapper */                                                       \
    typedef union {                                                                                  \
        Ptr base;                                                                                    \
        struct {                                                                                     \
            rawptr(AliasRef) addr;                                                                   \
        };                                                                                           \
    } AliasPtr; /* NOLINT */                                                                         \
    typedef union {                                                                                  \
        SPtr base;                                                                                   \
        struct {                                                                                     \
            rawptr(AliasRef) addr;                                                                   \
            AliasRef sentinel[1];                                                                    \
        };                                                                                           \
    } AliasSPtr; /* NOLINT */                                                                        \
    /* Pointer interface */                                                                          \
    force_inline AliasPtr pp_join(_, AliasPtr, from)(rawptr(T) addr) {                               \
        return (AliasPtr){ .base = Ptr_from(typeInfo(AliasRef), as(anyptr, addr)) };                 \
    }                                                                                                \
    force_inline rawptr(AliasRef) pp_join(_, AliasPtr, at)(AliasPtr self, usize index) {             \
        return Ptr_at(typeInfo(AliasRef), self.base, index);                                         \
    }                                                                                                \
    /* Sentinel pointer interface */                                                                 \
    force_inline AliasSPtr pp_join(_, AliasSPtr, from)(rawptr(T) addr, T sentinel) {                 \
        AliasSPtr result = {                                                                         \
            .sentinel[0].value = sentinel                                                            \
        };                                                                                           \
        return (AliasSPtr){                                                                          \
            .base = SPtr_from(typeInfo(AliasRef), as(anyptr, addr), as(anyptr, &result.sentinel[0])) \
        };                                                                                           \
    }                                                                                                \
    force_inline rawptr(AliasRef) pp_join(_, AliasSPtr, at)(AliasSPtr self, usize index) {           \
        return SPtr_at(typeInfo(AliasRef), self.base, index);                                        \
    }                                                                                                \
    force_inline rawptr(AliasRef) pp_join(_, AliasSPtr, sentinel)(AliasSPtr self) {                  \
        return SPtr_sentinel(typeInfo(AliasRef), self.base);                                         \
    }                                                                                                \
    force_inline bool pp_join(_, AliasSPtr, isSentinel)(AliasSPtr self, usize index) {               \
        return SPtr_isSentinel(typeInfo(AliasRef), self.base, index);                                \
    }

/* Builtin mutable types */
using_Ptr(u8);
using_Ptr(u16);
using_Ptr(u32);
using_Ptr(u64);
using_Ptr(usize);

using_Ptr(i8);
using_Ptr(i16);
using_Ptr(i32);
using_Ptr(i64);
using_Ptr(isize);

using_Ptr(f32);
using_Ptr(f64);

using_Ptr(bool);
using_Ptr(char);

/* Builtin const types */
using_PtrConst(u8);
using_PtrConst(u16);
using_PtrConst(u32);
using_PtrConst(u64);
using_PtrConst(usize);

using_PtrConst(i8);
using_PtrConst(i16);
using_PtrConst(i32);
using_PtrConst(i64);
using_PtrConst(isize);

using_PtrConst(f32);
using_PtrConst(f64);

using_PtrConst(bool);
using_PtrConst(char);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PTR_INCLUDED */
