/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-17 (date of last update)
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

#include "core/prim.h"
#include "debug/assert.h"
#include "mem/common.h"
#include "TypeInfo.h"
#include "Ptr.h"

/*========== Macros and Definitions =========================================*/

/* Slice ([]T in Zig) */
typedef struct Sli {
    anyptr addr;
    usize  len;
} Sli;

/* Sentinel-terminated slice ([:sentinel]T in Zig) */
typedef struct SSli {
    anyptr addr;
    usize  len;
    anyptr sentinel;
} SSli;

/* Core slice operations */
force_inline Sli    Sli_from(TypeInfo type, anyptr addr, usize begin, usize end);
force_inline anyptr Sli_at(TypeInfo type, Sli self, usize index);
force_inline Sli    Sli_slice(TypeInfo type, Sli self, usize begin, usize end);

/* Sentinel slice operations */
force_inline SSli   SSli_from(TypeInfo type, anyptr addr, usize begin, usize end, anyptr sentinel);
force_inline anyptr SSli_at(TypeInfo type, SSli self, usize index);
force_inline Sli    SSli_slice(TypeInfo type, SSli self, usize begin, usize end);
force_inline SSli   SSli_sliceS(TypeInfo type, SSli self, usize begin, usize end, anyptr sentinel);

#define using_Sli(T)      IMPL_using_Sli(T, pp_join(_, Ref, T), pp_join(_, Sli, T), pp_join(_, SSli, T))
#define using_SliConst(T) IMPL_using_Sli(const T, pp_join(_, RefConst, T), pp_join(_, SliConst, T), pp_join(_, SSliConst, T))

/* Span functions */

/* Calculate length of sentinel-terminated sequence */
force_inline usize mem_slen(TypeInfo type, anyptr addr, anyptr sentinel);
/* Create slice from sentinel-terminated pointer */
force_inline Sli   mem_span(TypeInfo type, SPtr ptr);
/* Create sentinel-terminated slice from sentinel-terminated pointer with new sentinel */
force_inline SSli  mem_spanS(TypeInfo type, SPtr ptr, anyptr new_sentinel);

/* Implementation */

force_inline Sli Sli_from(TypeInfo type, anyptr addr, usize begin, usize end) {
    debug_assert_nonnull(addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(begin < end, "Begin index must be less than end index");
    debug_assert_fmt(mem_isAligned((usize)addr, type.align), "Address must be properly aligned");
    return (Sli){
        .addr = as(u8*, addr) + (begin * type.size),
        .len  = end - begin
    };
}

force_inline anyptr Sli_at(TypeInfo type, Sli self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(index < self.len, "Index out of bounds");
    return as(u8*, self.addr) + (index * type.size);
}

force_inline Sli Sli_slice(TypeInfo type, Sli self, usize begin, usize end) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(begin < end, "Begin index must be less than end index");
    debug_assert_fmt(end <= self.len, "End index out of bounds");
    return (Sli){
        .addr = as(u8*, self.addr) + (begin * type.size),
        .len  = end - begin
    };
}

force_inline SSli SSli_from(TypeInfo type, anyptr addr, usize begin, usize end, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(begin < end, "Begin index must be less than end index");
    debug_assert_fmt(mem_isAligned(as(usize, addr), type.align), "Address must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, sentinel), type.align), "Sentinel must be properly aligned");
    return (SSli){
        .addr     = as(u8*, addr) + (begin * type.size),
        .len      = end - begin,
        .sentinel = sentinel
    };
}

force_inline anyptr SSli_at(TypeInfo type, SSli self, usize index) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(index < self.len, "Index out of bounds");
    return as(u8*, self.addr) + (index * type.size);
}

force_inline Sli SSli_slice(TypeInfo type, SSli self, usize begin, usize end) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(begin < end, "Begin index must be less than end index");
    debug_assert_fmt(end <= self.len, "End index out of bounds");
    return (Sli){
        .addr = as(u8*, self.addr) + (begin * type.size),
        .len  = end - begin
    };
}

force_inline SSli SSli_sliceS(TypeInfo type, SSli self, usize begin, usize end, anyptr sentinel) {
    debug_assert_nonnull(self.addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(begin < end, "Begin index must be less than end index");
    debug_assert_fmt(end <= self.len, "End index out of bounds");
    debug_assert_fmt(mem_isAligned(as(usize, sentinel), type.align), "Sentinel must be properly aligned");
    return (SSli){
        .addr     = as(u8*, self.addr) + (begin * type.size),
        .len      = end - begin,
        .sentinel = sentinel
    };
}

#define IMPL_using_Sli(T, AliasRef, AliasSli, AliasSSli)                                                         \
    /* Slice types with reference wrapper */                                                                     \
    typedef union {                                                                                              \
        Sli base;                                                                                                \
        struct {                                                                                                 \
            rawptr(AliasRef) addr;                                                                               \
            usize len;                                                                                           \
        };                                                                                                       \
    } AliasSli; /* NOLINT */                                                                                     \
    typedef union {                                                                                              \
        SSli base;                                                                                               \
        struct {                                                                                                 \
            rawptr(AliasRef) addr;                                                                               \
            usize    len;                                                                                        \
            AliasRef sentinel[1];                                                                                \
        };                                                                                                       \
    } AliasSSli; /* NOLINT */                                                                                    \
    /* Slice interface */                                                                                        \
    force_inline AliasSli pp_join(_, AliasSli, from)(rawptr(T) addr, usize begin, usize end) {                   \
        return (AliasSli){                                                                                       \
            .base = Sli_from(typeInfo(AliasRef), as(anyptr, addr), begin, end)                                   \
        };                                                                                                       \
    }                                                                                                            \
    force_inline rawptr(AliasRef) pp_join(_, AliasSli, at)(AliasSli self, usize index) {                         \
        return Sli_at(typeInfo(AliasRef), self.base, index);                                                     \
    }                                                                                                            \
    force_inline AliasSli pp_join(_, AliasSli, slice)(AliasSli self, usize begin, usize end) {                   \
        return (AliasSli){                                                                                       \
            .base = Sli_slice(typeInfo(AliasRef), self.base, begin, end)                                         \
        };                                                                                                       \
    }                                                                                                            \
    /* Sentinel slice interface */                                                                               \
    force_inline AliasSSli pp_join(_, AliasSSli, from)(rawptr(T) addr, usize begin, usize end, T sentinel) {     \
        AliasSSli result = {                                                                                     \
            .sentinel[0].value = sentinel,                                                                       \
        };                                                                                                       \
        return (AliasSSli){                                                                                      \
            .base = SSli_from(typeInfo(AliasRef), as(anyptr, addr), begin, end, as(anyptr, &result.sentinel[0])) \
        };                                                                                                       \
    }                                                                                                            \
    force_inline rawptr(AliasRef) pp_join(_, AliasSSli, at)(AliasSSli self, usize index) {                       \
        return SSli_at(typeInfo(AliasRef), self.base, index);                                                    \
    }                                                                                                            \
    force_inline AliasSli pp_join(_, AliasSSli, slice)(AliasSSli self, usize begin, usize end) {                 \
        return (AliasSli){                                                                                       \
            .base = SSli_slice(typeInfo(AliasRef), self.base, begin, end)                                        \
        };                                                                                                       \
    }                                                                                                            \
    force_inline AliasSSli pp_join(_, AliasSSli, sliceS)(AliasSSli self, usize begin, usize end, T sentinel) {   \
        AliasSSli result = {                                                                                     \
            .sentinel[0].value = sentinel,                                                                       \
        };                                                                                                       \
        return (AliasSSli){                                                                                      \
            .base = SSli_sliceS(typeInfo(AliasRef), self.base, begin, end, as(anyptr, &result.sentinel[0]))      \
        };                                                                                                       \
    }

force_inline usize mem_slen(TypeInfo type, anyptr addr, anyptr sentinel) {
    debug_assert_nonnull(addr);
    debug_assert_nonnull(sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(mem_isAligned(as(usize, addr), type.align), "Address must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, sentinel), type.align), "Sentinel must be properly aligned");

    usize len  = 0;
    u8*   curr = as(u8*, addr);
    while (mem_cmp(curr, sentinel, type.size) != 0) {
        len++;
        curr += type.size;
    }
    return len;
}

force_inline Sli mem_span(TypeInfo type, SPtr ptr) {
    debug_assert_nonnull(ptr.addr);
    debug_assert_nonnull(ptr.sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(mem_isAligned(as(usize, ptr.addr), type.align), "Address must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, ptr.sentinel), type.align), "Sentinel must be properly aligned");

    return (Sli){
        .addr = ptr.addr,
        .len  = mem_slen(type, ptr.addr, ptr.sentinel)
    };
}

force_inline SSli mem_spanS(TypeInfo type, SPtr ptr, anyptr new_sentinel) {
    debug_assert_nonnull(ptr.addr);
    debug_assert_nonnull(ptr.sentinel);
    debug_assert_nonnull(new_sentinel);
    debug_assert_fmt(type.size > 0, "Type size must be greater than 0");
    debug_assert_fmt(mem_isAligned(as(usize, ptr.addr), type.align), "Address must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, ptr.sentinel), type.align), "Sentinel must be properly aligned");
    debug_assert_fmt(mem_isAligned(as(usize, new_sentinel), type.align), "New sentinel must be properly aligned");

    return (SSli){
        .addr     = ptr.addr,
        .len      = mem_slen(type, ptr.addr, ptr.sentinel),
        .sentinel = new_sentinel
    };
}

/* Builtin mutable types */
using_Sli(u8);
using_Sli(u16);
using_Sli(u32);
using_Sli(u64);
using_Sli(usize);

using_Sli(i8);
using_Sli(i16);
using_Sli(i32);
using_Sli(i64);
using_Sli(isize);

using_Sli(f32);
using_Sli(f64);

using_Sli(bool);
using_Sli(char);

/* Builtin const types */
using_SliConst(u8);
using_SliConst(u16);
using_SliConst(u32);
using_SliConst(u64);
using_SliConst(usize);

using_SliConst(i8);
using_SliConst(i16);
using_SliConst(i32);
using_SliConst(i64);
using_SliConst(isize);

using_SliConst(f32);
using_SliConst(f64);

using_SliConst(bool);
using_SliConst(char);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */
