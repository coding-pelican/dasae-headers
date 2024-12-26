/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-12-10 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  NONE
 *
 * @brief Smart pointer implementation with metadata
 * @details Implements a smart pointer type for run-time generics that carries type information
 * and metadata along with the pointer.
 */

#ifndef CORE_PTR_INCLUDED
#define CORE_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim/int.h"
#include "prim/flt.h"
#include "prim/bool.h"
#include "prim/ptr.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"

#include <stdalign.h>
#include <string.h>

/*========== Metadata Layout ================================================*/

// TODO: Change the log2_align bitfield length to 4 or remove the log2_align calculation and change to normal align.
/* NOTE: Ack, math mistake!
The value of log2_align actually means 2^(log2_align), so the calculation is incorrect now.
Most implementations set the maximum practical alignment limit at around 4096 bytes (4 KB, the page size of many systems),
and for the most common compilers (GCC, Clang, MSVC) the maximum alignment is typically
- x86/x64: up to 8192 bytes (8 KB)
- ARM: typically up to 16 bytes
So, even if log2_align has only 4 bits, it can represent up to 64 KB of alignment (2^4 == 16 and 2^16 == 65536).
*/

// TODO: Remove 'undefined' macro functions

/* Metadata bits for 64-bit and 32-bit systems */
#if BUILTIN_PLTF_64BIT
#define PtrBase_MetaData_bits_len_type_size  (51) /* Up to 2 PB size */
#define PtrBase_MetaData_bits_len_log2_align (12) /* Up to 4 KB alignment */
#define PtrBase_MetaData_bits_len_is_defined (1)  /* Flag checking undefined */
#else
#define PtrBase_MetaData_bits_len_type_size  (25) /* Up to 32 MB size */
#define PtrBase_MetaData_bits_len_log2_align (6)  /* Up to 64 Bytes alignment */
#define PtrBase_MetaData_bits_len_is_defined (1)  /* Flag checking undefined */
#endif

/* Metadata masks and shifts */
#define PtrBase_MetaData_bits_mask_type_size  ((1ull << PtrBase_MetaData_bits_len_type_size) - 1)
#define PtrBase_MetaData_bits_mask_log2_align ((1ull << PtrBase_MetaData_bits_len_log2_align) - 1)
#define PtrBase_MetaData_bits_mask_is_defined ((1ull << PtrBase_MetaData_bits_len_is_defined) - 1)

#define PtrBase_MetaData_bits_shift_type_size  (0)
#define PtrBase_MetaData_bits_shift_log2_align (PtrBase_MetaData_bits_len_type_size)
#define PtrBase_MetaData_bits_shift_is_defined (PtrBase_MetaData_bits_len_type_size + PtrBase_MetaData_bits_len_log2_align)

/*========== Type Definitions ===============================================*/

/* Metadata */
typedef struct PtrBase_MetaData {
    usize type_size : PtrBase_MetaData_bits_len_type_size;
    usize log2_align : PtrBase_MetaData_bits_len_log2_align;
    usize is_defined : PtrBase_MetaData_bits_len_is_defined;
} PtrBase_MetaData;

/* Pointer Base */
typedef struct PtrBase {
    usize            addr;
    PtrBase_MetaData meta;
} PtrBase;
static const PtrBase PtrBase_undefined = { 0, { 0 } };

/* Single-item pointer (*T in Zig) */
typedef struct Sptr {
    PtrBase Base;
} Sptr;
static const Sptr Sptr_undefined = { PtrBase_undefined };

/* Many-item pointer ([*]T in Zig) */
typedef struct Mptr {
    PtrBase Base;
} Mptr;
static const Mptr Mptr_undefined = { PtrBase_undefined };

/* Slice ([]T in Zig) */
typedef struct Slice {
    PtrBase Base;
    usize   len;
} Slice;
static const Slice Slice_undefined = { PtrBase_undefined, 0 };

/* Type declarations */
#define Sptr(T)                    \
    union {                        \
        Sptr Sptr;                 \
        struct {                   \
            rawptr(T) const addr;  \
            PtrBase_MetaData meta; \
        };                         \
    }
#define Mptr(T)                    \
    union {                        \
        Mptr Mptr;                 \
        struct {                   \
            rawptr(T) const addr;  \
            PtrBase_MetaData meta; \
        };                         \
    }
#define Slice(T)                   \
    union {                        \
        Slice Slice;               \
        struct {                   \
            rawptr(T) const addr;  \
            PtrBase_MetaData meta; \
            usize            len;  \
        };                         \
    }

/*========== Size Assertions ================================================*/

claim_assert_static_msg(sizeof(PtrBase) == sizeof(anyptr) * 2, "PtrBase size mismatch");
claim_assert_static_msg(sizeof(Sptr) == sizeof(anyptr) * 2, "Sptr size mismatch");
claim_assert_static_msg(sizeof(Mptr) == sizeof(anyptr) * 2, "Mptr size mismatch");
claim_assert_static_msg(sizeof(Slice) == sizeof(anyptr) * 2 + sizeof(usize), "Slice size mismatch");

/*========== Core Functions =================================================*/

/* Base pointer operations */
force_inline PtrBase PtrBase_make(anyptr addr, usize size, usize align);
force_inline PtrBase PtrBase_makeWithMeta(anyptr addr, PtrBase_MetaData meta);
force_inline PtrBase PtrBase_withAlign(PtrBase self, usize new_align);
#define PtrBase_from(T, var) IMPL_PtrBase_from(T, var)

/* Common operations for all pointer types */
force_inline anyptr PtrBase_addr(PtrBase self);
force_inline usize  PtrBase_size(PtrBase self);
force_inline usize  PtrBase_align(PtrBase self);
force_inline bool   PtrBase_isAligned(PtrBase self);
force_inline bool   PtrBase_isUndefined(PtrBase self);

/* Type metadata generation */
#define PtrBase_typeInfo(T)               IMPL_PtrBase_typeInfo(T)
#define PtrBase_makeTypeInfo(size, align) IMPL_PtrBase_makeTypeInfo(size, align)
/* Create PtrBase from type info and address */
force_inline PtrBase PtrBase_fromAddr(anyptr addr, PtrBase_MetaData meta);

/* Create undefined PtrBase with type info */
#define PtrBase_undefined(T) IMPL_PtrBase_undefined(T)

/* Base pointer helper functions */
force_inline usize PtrBase__calcLog2OrderAlign(usize alignment);
force_inline usize PtrBase__calcAlignLog2Order(usize log2_align);
force_inline bool  PtrBase__hasMinSize(PtrBase self, usize required_size);
force_inline bool  PtrBase__hasMinAlign(PtrBase self, usize required_align);

/* Single-item pointer operations */
#define Sptr_ref(T, var)    IMPL_Sptr_ref(T, var)
#define Sptr_deref(T, self) IMPL_Sptr_deref(T, self)

force_inline anyptr Sptr_addr(Sptr self);
force_inline usize  Sptr_size(Sptr self);
force_inline usize  Sptr_align(Sptr self);
force_inline bool   Sptr_isAligned(Sptr self);
force_inline bool   Sptr_isUndefined(Sptr self);

force_inline Mptr  Sptr_toMptr(Sptr self);
force_inline Slice Sptr_toSlice(Sptr self, usize begin, usize len);

#define Sptr_undefined(T) IMPL_Sptr_undefined(T)

/* Single-item pointer helper functions */
force_inline void    Sptr__validateDeref(Sptr self, usize type_size);
force_inline void    Sptr__validateConversion(Sptr self);
force_inline void    Sptr__validateSlice(Sptr self, usize begin, usize len);
force_inline PtrBase Sptr__makeRef(anyptr addr, usize size, usize align);
force_inline anyptr  Sptr__getDerefAddr(Sptr self, usize type_size);

/* Many-item pointer operations */
#define Mptr_ref(T, var)        IMPL_Mptr_ref(T, var)
#define Mptr_at(T, self, index) IMPL_Mptr_at(T, self, index)
force_inline anyptr Mptr_atAddr(Mptr self, usize index);
force_inline Sptr   Mptr_atSptr(Mptr self, usize index);
force_inline usize  Mptr_len(Mptr self);

force_inline anyptr Mptr_addr(Mptr self);
force_inline usize  Mptr_size(Mptr self);
force_inline usize  Mptr_align(Mptr self);
force_inline bool   Mptr_isAligned(Mptr self);
force_inline bool   Mptr_isUndefined(Mptr self);

force_inline Mptr  Mptr_add(Mptr self, isize offset);
force_inline Mptr  Mptr_sub(Mptr self, isize offset);
force_inline isize Mptr_diff(Mptr lhs, Mptr rhs);
force_inline Slice Mptr_toSlice(Mptr self, usize begin, usize end);

#define Mptr_undefined(T) IMPL_Mptr_undefined(T)

/* Many-item pointer helper functions */
force_inline void    Mptr__validateAccess(Mptr self, usize type_size);
force_inline void    Mptr__validateAdd(Mptr self, isize offset);
force_inline void    Mptr__validateDiff(Mptr lhs, Mptr rhs);
force_inline void    Mptr__validateSlice(Mptr self, usize begin, usize end);
force_inline PtrBase Mptr__makeRef(anyptr addr, usize size, usize align);
force_inline anyptr  Mptr__getAtAddr(Mptr self, usize index, usize type_size);

/* Slice operations */
#define Slice_refArray(T, var_arr)              IMPL_Slice_refArray(T, var_arr)
#define Slice_fromArray(T, var_arr, begin, end) IMPL_Slice_fromArray(T, var_arr, begin, end)
force_inline Slice Slice_fromSptr(Sptr ptr, usize begin, usize end);
force_inline Slice Slice_fromMptr(Mptr ptr, usize begin, usize end);
force_inline Slice Slice_slice(Slice self, usize begin, usize end);
force_inline Slice Slice_prefix(Slice self, usize end);
force_inline Slice Slice_suffix(Slice self, usize begin);

#define Slice_at(T, self, index) IMPL_Slice_at(T, self, index)
#define Slice_begin(T, self)     IMPL_Slice_begin(T, self)
#define Slice_end(T, self)       IMPL_Slice_end(T, self)
force_inline Mptr  Slice_atMptr(Slice self, usize index);
force_inline Mptr  Slice_beginMptr(Slice self);
force_inline Mptr  Slice_endMptr(Slice self);
force_inline usize Slice_len(Slice self);

force_inline anyptr Slice_addr(Slice self);
force_inline usize  Slice_size(Slice self);
force_inline usize  Slice_align(Slice self);
force_inline bool   Slice_isAligned(Slice self);
force_inline bool   Slice_isUndefined(Slice self);

#define Slice_checkTypeSize(T, self)   IMPL_Slice_checkTypeSize(T, self)
#define Slice_checkAlignment(self)     IMPL_Slice_checkAlignment(self)
#define Slice_checkBounds(self, index) IMPL_Slice_checkBounds(self, index)

#define Slice_undefined(T) IMPL_Slice_undefined(T)

/* Slice helper functions */
force_inline void    Slice__validateAccess(Slice self, usize type_size);
force_inline void    Slice__validateIndex(Slice self, usize index);
force_inline void    Slice__validateRange(Slice self, usize begin, usize end);
force_inline PtrBase Slice__makeBase(anyptr addr, usize size, usize align);
force_inline Slice   Slice__makeFromBase(PtrBase base, usize len);
force_inline anyptr  Slice__getAtAddr(Slice self, usize index, usize type_size);

/* Conversion functions */
#define PtrBase_rawCast(TDestRawPtr, self) IMPL_PtrBase_rawCast(TDestRawPtr, self)
#define Sptr_rawCast(TDestRawPtr, self)    IMPL_Sptr_rawCast(TDestRawPtr, self)
#define Mptr_rawCast(TDestRawPtr, self)    IMPL_Mptr_rawCast(TDestRawPtr, self)
#define Slice_rawCast(TDestRawPtr, self)   IMPL_Slice_rawCast(TDestRawPtr, self)

/* Conversion helper functions */
force_inline anyptr PtrBase__getRawCastAddr(PtrBase self);
force_inline anyptr Sptr__getRawCastAddr(Sptr self);
force_inline anyptr Mptr__getRawCastAddr(Mptr self);
force_inline anyptr Slice__getRawCastAddr(Slice self);

/*========== Void Type ======================================================*/

/* Void type for present empty type when use generic type parameter (like Res(T, E)) */
typedef struct Void {
    u8 unused_[0];
} Void;

/*========== Type Info ======================================================*/

typedef PtrBase_MetaData TypeInfo;
#define typeInfo(T) (             \
    (TypeInfo){                   \
        .type_size  = sizeof(T),  \
        .log2_align = alignof(T), \
        .is_defined = true }      \
)

/*========== Built-in Types =================================================*/

/* Single-Item Pointers */
typedef Sptr(u8) Sptr_u8;
typedef Sptr(u16) Sptr_u16;
typedef Sptr(u32) Sptr_u32;
typedef Sptr(u64) Sptr_u64;
typedef Sptr(usize) Sptr_usize;

typedef Sptr(i8) Sptr_i8;
typedef Sptr(i16) Sptr_i16;
typedef Sptr(i32) Sptr_i32;
typedef Sptr(i64) Sptr_i64;
typedef Sptr(isize) Sptr_isize;

typedef Sptr(f32) Sptr_f32;
typedef Sptr(f64) Sptr_f64;

typedef Sptr(bool) Sptr_bool;
typedef Sptr(char) Sptr_char;

typedef Sptr(anyptr) Sptr_anyptr;
typedef Sptr(PtrBase) Sptr_PtrBase;
typedef Sptr(Sptr) Sptr_Sptr;
typedef Sptr(Mptr) Sptr_Mptr;
typedef Sptr(Slice) Sptr_Slice;

/* Many-item Pointers */
typedef Mptr(u8) Mptr_u8;
typedef Mptr(u16) Mptr_u16;
typedef Mptr(u32) Mptr_u32;
typedef Mptr(u64) Mptr_u64;
typedef Mptr(usize) Mptr_usize;

typedef Mptr(i8) Mptr_i8;
typedef Mptr(i16) Mptr_i16;
typedef Mptr(i32) Mptr_i32;
typedef Mptr(i64) Mptr_i64;
typedef Mptr(isize) Mptr_isize;

typedef Mptr(f32) Mptr_f32;
typedef Mptr(f64) Mptr_f64;

typedef Mptr(bool) Mptr_bool;
typedef Mptr(char) Mptr_char;

typedef Mptr(anyptr) Mptr_anyptr;
typedef Mptr(PtrBase) Mptr_PtrBase;
typedef Mptr(Sptr) Mptr_Sptr;
typedef Mptr(Mptr) Mptr_Mptr;
typedef Mptr(Slice) Mptr_Slice;

/* Slice Types */
typedef Slice(u8) Slice_u8;
typedef Slice(u16) Slice_u16;
typedef Slice(u32) Slice_u32;
typedef Slice(u64) Slice_u64;
typedef Slice(usize) Slice_usize;

typedef Slice(i8) Slice_i8;
typedef Slice(i16) Slice_i16;
typedef Slice(i32) Slice_i32;
typedef Slice(i64) Slice_i64;
typedef Slice(isize) Slice_isize;

typedef Slice(f32) Slice_f32;
typedef Slice(f64) Slice_f64;

typedef Slice(bool) Slice_bool;
typedef Slice(char) Slice_char;

typedef Slice(anyptr) Slice_anyptr;
typedef Slice(PtrBase) Slice_PtrBase;
typedef Slice(Sptr) Slice_Sptr;
typedef Slice(Mptr) Slice_Mptr;
typedef Slice(Slice) Slice_Slice;

/*========== Helper Functions ================================================*/

force_inline usize PtrBase__calcLog2OrderAlign(usize alignment) {
#if defined(__GNUC__) || defined(__clang__)
    return (usize)__builtin_ctzll((unsigned long long)alignment);
#elif defined(_MSC_VER)
#if BUILTIN_PLTF_64BIT
    unsigned long index = 0;
    _BitScanForward64(&index, (unsigned __int64)alignment);
    return (usize)index;
#else
    unsigned long index = 0;
    _BitScanForward(&index, (unsigned long)alignment);
    return (usize)index;
#endif
#else
    usize order = 0;
    while (alignment > 1) {
        alignment >>= 1;
        order++;
    }
    return order;
#endif
}

force_inline usize PtrBase__calcAlignLog2Order(usize log2_align) {
    return ((usize)1) << log2_align;
}

force_inline bool PtrBase__hasMinSize(PtrBase self, usize required_size) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to check size of undefined pointer");
    return self.meta.type_size >= required_size;
}

force_inline bool PtrBase__hasMinAlign(PtrBase self, usize required_align) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to check alignment of undefined pointer");
    return PtrBase_align(self) >= required_align;
}

#define IMPL_PtrBase_typeInfo(T)                               \
    ((PtrBase_MetaData){                                       \
        .type_size  = sizeof(T),                               \
        .log2_align = PtrBase__calcLog2OrderAlign(alignof(T)), \
        .is_defined = true })

#define IMPL_PtrBase_makeTypeInfo(size, align)            \
    ((PtrBase_MetaData){                                  \
        .type_size  = (size),                             \
        .log2_align = PtrBase__calcLog2OrderAlign(align), \
        .is_defined = true })

force_inline PtrBase PtrBase_fromAddr(anyptr addr, PtrBase_MetaData meta) {
    debug_assert_fmt(addr != null, "Cannot create PtrBase with null address");
    return (PtrBase){
        .addr = rawptrToInt(addr),
        .meta = meta
    };
}

// force_inline PtrBase PtrBase_undefined(PtrBase_MetaData meta) {
//     meta.is_defined = false;
//     return (PtrBase){
//         .addr = 0,
//         .meta = meta
//     };
// }

/*========== Base Pointer Creation ==========================================*/

force_inline PtrBase PtrBase_make(anyptr addr, usize size, usize align) {
    debug_assert_fmt(addr != null, "Attempt to create pointer from null address");
    debug_assert_fmt(size > 0, "Type size must be greater than 0");
    debug_assert_fmt(align > 0, "Alignment must be greater than 0");
    debug_assert_fmt((align & (align - 1)) == 0, "Alignment must be power of 2");

    return (PtrBase){
        .addr = rawptrToInt(addr),
        .meta = {
            .type_size  = size,
            .log2_align = PtrBase__calcLog2OrderAlign(align),
            .is_defined = true }
    };
}

force_inline PtrBase PtrBase_makeWithMeta(anyptr addr, PtrBase_MetaData meta) {
    debug_assert_fmt(addr != null, "Attempt to create pointer from null address");
    debug_assert_fmt(meta.type_size > 0, "Type size must be greater than 0");
    debug_assert_fmt(meta.log2_align < sizeof(usize) * 8, "Log2 alignment too large");

    return (PtrBase){
        .addr = rawptrToInt(addr),
        .meta = meta
    };
}

force_inline PtrBase PtrBase_withAlign(PtrBase self, usize new_align) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to align undefined pointer");
    debug_assert_fmt(new_align > 0, "New alignment must be greater than 0");
    debug_assert_fmt((new_align & (new_align - 1)) == 0, "New alignment must be power of 2");
    debug_assert_fmt(new_align >= PtrBase_align(self), "Cannot decrease alignment");

    return (PtrBase){
        .addr = self.addr,
        .meta = {
            .type_size  = self.meta.type_size,
            .log2_align = PtrBase__calcLog2OrderAlign(new_align),
            .is_defined = true }
    };
}

#define IMPL_PtrBase_from(T, var) \
    PtrBase_make(&(var), sizeof(T), alignof(T))

/*========== Base Pointer Operations ======================================*/

force_inline anyptr PtrBase_addr(PtrBase self) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to get address of undefined pointer");
    return intToRawptr(anyptr, self.addr);
}

force_inline usize PtrBase_size(PtrBase self) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to get size of undefined pointer");
    return self.meta.type_size;
}

force_inline usize PtrBase_align(PtrBase self) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to get alignment of undefined pointer");
    return PtrBase__calcAlignLog2Order(self.meta.log2_align);
}

force_inline bool PtrBase_isAligned(PtrBase self) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to check alignment of undefined pointer");
    const usize mask = PtrBase__calcAlignLog2Order(self.meta.log2_align) - 1;
    return (self.addr & mask) == 0;
}

force_inline bool PtrBase_isUndefined(PtrBase self) {
    return !self.meta.is_defined;
}

#define IMPL_PtrBase_undefined(T)                                  \
    ((PtrBase){                                                    \
        .addr = 0,                                                 \
        .meta = {                                                  \
            .type_size  = sizeof(T),                               \
            .log2_align = PtrBase__calcLog2OrderAlign(alignof(T)), \
            .is_defined = false } })


/*========== Helper Functions ==============================================*/

#if DEBUG_ENABLED
force_inline void Sptr__validateDeref(Sptr self, usize type_size) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Attempt to deref undefined Sptr");
    debug_assert_fmt(PtrBase__hasMinSize(self.Base, type_size), "Type size mismatch in Sptr_deref");
    debug_assert_fmt(Sptr_isAligned(self), "Misaligned Sptr access");
}

force_inline void Sptr__validateConversion(Sptr self) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Attempt to convert undefined Sptr");
}

force_inline void Sptr__validateSlice(Sptr self, usize begin, usize len) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Attempt to slice undefined Sptr");
    debug_assert_fmt(len > 0, "Slice length must be greater than 0");
    debug_assert_fmt(begin + len > begin, "Slice range overflow");
}
#else
#define Sptr__validateDeref(self, type_size)  unused(0)
#define Sptr__validateConversion(self)        unused(0)
#define Sptr__validateSlice(self, begin, len) unused(0)
#endif

/*========== Core Single-Item Pointer Implementation =======================*/

/* Reference operations */
force_inline PtrBase Sptr__makeRef(anyptr addr, usize size, usize align) {
    debug_assert_fmt(addr != null, "Attempt to reference null address");
    return PtrBase_make(addr, size, align);
}

#define IMPL_Sptr_ref(T, var) \
    ((Sptr){ .Base = Sptr__makeRef(&(var), sizeof(T), alignof(T)) })

force_inline Sptr refSptr(TypeInfo type_info, anyptr addr) {
    return (Sptr){ .Base = Sptr__makeRef(addr, type_info.type_size, type_info.log2_align) };
}

/* Dereference operations */
force_inline anyptr Sptr__getDerefAddr(Sptr self, usize type_size) {
    Sptr__validateDeref(self, type_size);
    unused(type_size);
    return Sptr_addr(self);
}

#define IMPL_Sptr_deref(T, self) \
    (*((T*)Sptr__getDerefAddr((self), sizeof(T))))

/* Address and metadata access */
force_inline anyptr Sptr_addr(Sptr self) {
    return PtrBase_addr(self.Base);
}

force_inline usize Sptr_size(Sptr self) {
    return PtrBase_size(self.Base);
}

force_inline usize Sptr_align(Sptr self) {
    return PtrBase_align(self.Base);
}

force_inline bool Sptr_isAligned(Sptr self) {
    return PtrBase_isAligned(self.Base);
}

force_inline bool Sptr_isUndefined(Sptr self) {
    return PtrBase_isUndefined(self.Base);
}

/* Type conversion operations */
force_inline Mptr Sptr_toMptr(Sptr self) {
    Sptr__validateConversion(self);
    return (Mptr){
        .Base = self.Base
    };
}

force_inline Slice Sptr_toSlice(Sptr self, usize begin, usize len) {
    Sptr__validateSlice(self, begin, len);

    return (Slice){
        .Base = (PtrBase){
            .addr = rawptrToInt((u8*)Sptr_addr(self) + (begin * self.Base.meta.type_size)),
            .meta = self.Base.meta },
        .len = len
    };
}

#define IMPL_Sptr_undefined(T) \
    ((Sptr){ .Base = PtrBase_undefined(T) })

/*========== Helper Functions ==============================================*/

#if DEBUG_ENABLED
force_inline void Mptr__validateAccess(Mptr self, usize type_size) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to access undefined Mptr");
    debug_assert_fmt(PtrBase__hasMinSize(self.Base, type_size), "Type size mismatch in Mptr access");
    debug_assert_fmt(Mptr_isAligned(self), "Misaligned Mptr access");
}

force_inline void Mptr__validateAdd(Mptr self, isize offset) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to add to undefined Mptr");
    if (offset > 0) {
        debug_assert_fmt(SIZE_MAX - (usize)self.Base.addr >= (usize)offset * self.Base.meta.type_size, "Pointer arithmetic overflow");
    } else {
        debug_assert_fmt((usize)(-offset) * self.Base.meta.type_size <= (usize)self.Base.addr, "Pointer arithmetic underflow");
    }
}

force_inline void Mptr__validateDiff(Mptr lhs, Mptr rhs) {
    debug_assert_fmt(!Mptr_isUndefined(lhs) && !Mptr_isUndefined(rhs), "Attempt to diff undefined Mptr");
    debug_assert_fmt(lhs.Base.meta.type_size == rhs.Base.meta.type_size, "Type size mismatch in Mptr_diff");
}

force_inline void Mptr__validateSlice(Mptr self, usize begin, usize end) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to slice undefined Mptr");
    debug_assert_fmt(begin <= end, "Slice begin must not exceed end");
    debug_assert_fmt(end > begin, "Empty slice not allowed");
}
#else
#define Mptr__validateAccess(self, type_size) unused(0)
#define Mptr__validateAdd(self, offset)       unused(0)
#define Mptr__validateDiff(lhs, rhs)          unused(0)
#define Mptr__validateSlice(self, begin, end) unused(0)
#endif

/*========== Core Many-Item Pointer Implementation =========================*/

/* Reference operations */
force_inline PtrBase Mptr__makeRef(anyptr addr, usize size, usize align) {
    debug_assert_fmt(addr != null, "Attempt to reference null address");
    return PtrBase_make(addr, size, align);
}

#define IMPL_Mptr_ref(T, var) \
    ((Mptr){ .Base = Mptr__makeRef(var, sizeof(T), alignof(T)) })

/* Element access operations */
force_inline anyptr Mptr_atAddr(Mptr self, usize index) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to access undefined Mptr");
    return (u8*)Mptr_addr(self) + (index * self.Base.meta.type_size);
}

force_inline anyptr Mptr__getAtAddr(Mptr self, usize index, usize type_size) {
    Mptr__validateAccess(self, type_size);
    unused(type_size);
    return Mptr_atAddr(self, index);
}

force_inline Sptr Mptr_atSptr(Mptr self, usize index) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to get Sptr from undefined Mptr");

    return (Sptr){
        .Base = (PtrBase){
            .addr = rawptrToInt((u8*)Mptr_addr(self) + index * self.Base.meta.type_size),
            .meta = self.Base.meta }
    };
}

#define IMPL_Mptr_at(T, self, index) \
    (*((T*)Mptr__getAtAddr((self), (index), sizeof(T))))

/* Basic pointer operations */
force_inline anyptr Mptr_addr(Mptr self) {
    return PtrBase_addr(self.Base);
}

force_inline usize Mptr_size(Mptr self) {
    return PtrBase_size(self.Base);
}

force_inline usize Mptr_align(Mptr self) {
    return PtrBase_align(self.Base);
}

force_inline bool Mptr_isAligned(Mptr self) {
    return PtrBase_isAligned(self.Base);
}

force_inline bool Mptr_isUndefined(Mptr self) {
    return PtrBase_isUndefined(self.Base);
}

/* Pointer arithmetic operations */
force_inline Mptr Mptr_add(Mptr self, isize offset) {
    Mptr__validateAdd(self, offset);

    return (Mptr){
        .Base = (PtrBase){
            .addr = self.Base.addr + (offset * self.Base.meta.type_size),
            .meta = self.Base.meta }
    };
}

force_inline Mptr Mptr_sub(Mptr self, isize offset) {
    return Mptr_add(self, -offset);
}

force_inline isize Mptr_diff(Mptr lhs, Mptr rhs) {
    Mptr__validateDiff(lhs, rhs);

    const isize byte_diff = (isize)(lhs.Base.addr - rhs.Base.addr);
    return byte_diff / (isize)lhs.Base.meta.type_size;
}

/* Slice conversion */
force_inline Slice Mptr_toSlice(Mptr self, usize begin, usize end) {
    Mptr__validateSlice(self, begin, end);

    return (Slice){
        .Base = (PtrBase){
            .addr = rawptrToInt((u8*)Mptr_addr(self) + (begin * self.Base.meta.type_size)),
            .meta = self.Base.meta },
        .len = end - begin
    };
}

#define IMPL_Mptr_undefined(T) \
    ((Mptr){ .Base = PtrBase_undefined(T) })

/*========== Helper Functions ==============================================*/

#if DEBUG_ENABLED
force_inline void Slice__validateAccess(Slice self, usize type_size) {
    debug_assert_fmt(!Slice_isUndefined(self), "Attempt to access undefined Slice");
    debug_assert_fmt(PtrBase__hasMinSize(self.Base, type_size), "Type size mismatch in Slice access");
    debug_assert_fmt(Slice_isAligned(self), "Misaligned Slice access");
}

force_inline void Slice__validateIndex(Slice self, usize index) {
    debug_assert_fmt(index < self.len, "Slice index out of bounds");
}

force_inline void Slice__validateRange(Slice self, usize begin, usize end) {
    debug_assert_fmt(begin <= end, "Slice begin must not exceed end");
    debug_assert_fmt(end <= self.len, "Slice end out of bounds");
}
#else
#define Slice__validateAccess(self, type_size) unused(0)
#define Slice__validateIndex(self, index)      unused(0)
#define Slice__validateRange(self, begin, end) unused(0)
#endif

/*========== Slice Creation ==============================================*/

force_inline PtrBase Slice__makeBase(anyptr addr, usize size, usize align) {
    debug_assert_fmt(addr != null, "Attempt to create slice from null address");
    return PtrBase_make(addr, size, align);
}

force_inline Slice Slice__makeFromBase(PtrBase base, usize len) {
    return (Slice){
        .Base = base,
        .len  = len
    };
}

#define IMPL_Slice_refArray(T, var_arr)                    \
    Slice__makeFromBase(                                   \
        Slice__makeBase((var_arr), sizeof(T), alignof(T)), \
        sizeof(var_arr) / sizeof(T)                        \
    )

#define IMPL_Slice_fromArray(T, var_arr, begin, end) ({          \
    const usize arr_len = sizeof(var_arr) / sizeof(T);           \
    debug_assert_fmt((begin) <= (end), "Invalid slice range");   \
    debug_assert_fmt((end) <= arr_len, "Array bounds exceeded"); \
    Slice__makeFromBase(                                         \
        Slice__makeBase(                                         \
            (u8*)(var_arr) + (begin) * sizeof(T),                \
            sizeof(T),                                           \
            alignof(T)                                           \
        ),                                                       \
        (end) - (begin)                                          \
    );                                                           \
})

force_inline Slice Slice_fromSptr(Sptr ptr, usize begin, usize end) {
    debug_assert_fmt(!Sptr_isUndefined(ptr), "Attempt to create slice from undefined Sptr");
    debug_assert_fmt(begin <= end, "Invalid slice range");

    return Slice__makeFromBase(
        (PtrBase){
            .addr = rawptrToInt((u8*)Sptr_addr(ptr) + begin * ptr.Base.meta.type_size),
            .meta = ptr.Base.meta },
        end - begin
    );
}

force_inline Slice Slice_fromMptr(Mptr ptr, usize begin, usize end) {
    debug_assert_fmt(!Mptr_isUndefined(ptr), "Attempt to create slice from undefined Mptr");
    debug_assert_fmt(begin <= end, "Invalid slice range");

    return Slice__makeFromBase(
        (PtrBase){
            .addr = rawptrToInt((u8*)Mptr_addr(ptr) + begin * ptr.Base.meta.type_size),
            .meta = ptr.Base.meta },
        end - begin
    );
}

/*========== Slice Operations ===========================================*/

force_inline Slice Slice_slice(Slice self, usize begin, usize end) {
    debug_assert_fmt(!Slice_isUndefined(self), "Attempt to slice undefined Slice");
    Slice__validateRange(self, begin, end);

    return Slice__makeFromBase(
        (PtrBase){
            .addr = rawptrToInt((u8*)Slice_addr(self) + begin * self.Base.meta.type_size),
            .meta = self.Base.meta },
        end - begin
    );
}

force_inline Slice Slice_prefix(Slice self, usize end) {
    return Slice_slice(self, 0, end);
}

force_inline Slice Slice_suffix(Slice self, usize begin) {
    return Slice_slice(self, begin, self.len);
}

/*========== Element Access ============================================*/

force_inline anyptr Slice__getAtAddr(Slice self, usize index, usize type_size) {
    Slice__validateAccess(self, type_size);
    Slice__validateIndex(self, index);
    unused(type_size);
    return (u8*)Slice_addr(self) + (index * self.Base.meta.type_size);
}

#define IMPL_Slice_at(T, self, index) \
    (*((T*)Slice__getAtAddr((self), (index), sizeof(T))))

#define IMPL_Slice_begin(T, self) \
    IMPL_Slice_at(T, self, 0)

#define IMPL_Slice_end(T, self) \
    IMPL_Slice_at(T, self, (self).len - 1)

force_inline Mptr Slice_atMptr(Slice self, usize index) {
    Slice__validateIndex(self, index);
    return (Mptr){
        .Base = (PtrBase){
            .addr = rawptrToInt((u8*)Slice_addr(self) + index * self.Base.meta.type_size),
            .meta = self.Base.meta }
    };
}

force_inline Mptr Slice_beginMptr(Slice self) {
    return (Mptr){ .Base = self.Base };
}

force_inline Mptr Slice_endMptr(Slice self) {
    return Slice_atMptr(self, self.len);
}

force_inline usize Slice_len(Slice self) {
    return self.len;
}

/*========== Basic Operations =========================================*/

force_inline anyptr Slice_addr(Slice self) {
    return PtrBase_addr(self.Base);
}

force_inline usize Slice_size(Slice self) {
    return PtrBase_size(self.Base);
}

force_inline usize Slice_align(Slice self) {
    return PtrBase_align(self.Base);
}

force_inline bool Slice_isAligned(Slice self) {
    return PtrBase_isAligned(self.Base);
}

force_inline bool Slice_isUndefined(Slice self) {
    return PtrBase_isUndefined(self.Base);
}

/*========== Validation Macros ========================================*/

#define IMPL_Slice_checkTypeSize(T, self) \
    debug_assert_fmt(PtrBase__hasMinSize((self).Base, sizeof(T)), "Type size mismatch in Slice operation")

#define IMPL_Slice_checkAlignment(self) \
    debug_assert_fmt(Slice_isAligned(self), "Misaligned Slice access")

#define IMPL_Slice_checkBounds(self, index) \
    debug_assert_fmt((index) < (self).len, "Slice index out of bounds")

#define IMPL_Slice_undefined(T) \
    ((Slice){ .Base = PtrBase_undefined(T), .len = 0 })

/*========== Helper Functions ==============================================*/

force_inline anyptr PtrBase__getRawCastAddr(PtrBase self) {
    debug_assert_fmt(!PtrBase_isUndefined(self), "Attempt to raw cast undefined pointer");
    return PtrBase_addr(self);
}

force_inline anyptr Sptr__getRawCastAddr(Sptr self) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Attempt to raw cast undefined Sptr");
    return Sptr_addr(self);
}

force_inline anyptr Mptr__getRawCastAddr(Mptr self) {
    debug_assert_fmt(!Mptr_isUndefined(self), "Attempt to raw cast undefined Mptr");
    return Mptr_addr(self);
}

force_inline anyptr Slice__getRawCastAddr(Slice self) {
    debug_assert_fmt(!Slice_isUndefined(self), "Attempt to raw cast undefined Slice");
    return Slice_addr(self);
}

/*========== Raw Cast Implementation ==========================================*/

#define IMPL_PtrBase_rawCast(TDestRawPtr, self) \
    ((TDestRawPtr)PtrBase__getRawCastAddr(self))

#define IMPL_Sptr_rawCast(TDestRawPtr, self) \
    ((TDestRawPtr)Sptr__getRawCastAddr(self))

#define IMPL_Mptr_rawCast(TDestRawPtr, self) \
    ((TDestRawPtr)Mptr__getRawCastAddr(self))

#define IMPL_Slice_rawCast(TDestRawPtr, self) \
    ((TDestRawPtr)Slice__getRawCastAddr(self))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PTR_INCLUDED */
