/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptrs.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  NONE
 *
 * @brief Smart pointer implementation with metadata
 * @details Implements a smart pointer type that carries type information
 * and metadata along with the pointer. Two implementations are provided:
 *
 * 64-bit systems:
 * 1. Bitfield within pointer (CORE_CORE_PTR_IMPL_BITFIELD)
 *    - Uses high 16 bits of (unused bits in) aligned pointers
 *    - 48-bit address space (256 TB) + metadata
 *    - Single word size
 *
 * 2. Double-width structure (CORE_CORE_PTR_IMPL_DOUBLE)
 *    - Full 64-bit pointer + metadata word
 *    - Full 64-bit address space support
 *    - Two word size
 *
 * 32-bit systems:
 * - Always uses double-width structure
 * - Simpler implementation
 *
 * Metadata layout:
 * - size:  12 bits (0 to 4096 bytes)
 * - align: 3 bits  (2^0 to 2^7 alignment)
 * - flags: 1 bit   (user-defined)
 */

#ifndef CORE_PTRS_INCLUDED
#define CORE_PTRS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/core/prim/ptr.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"
#include "dh/claim/unreachable.h"

#include <stdalign.h>
#include <stddef.h>
#include <string.h>

/*========== Metadata Layout ================================================*/

// Core pointer metadata layout
#define cptr__bits_addr_len (48) // 256 TB address space

#define cptr__bits_size_len  (12) // Up to 4096 bytes size
#define cptr__bits_algin_len (3)  // Up to 128 bytes alignment
#define cptr__bits_flags_len (1)  // boolean flags

// Masks and shifts
#define cptr__mask_bits_size  ((1ull << cptr__bits_size_len) - 1)
#define cptr__mask_bits_align ((1ull << cptr__bits_algin_len) - 1)
#define cptr__mask_bits_flags ((1ull << cptr__bits_flags_len) - 1)

#define cptr__shift_bits_size  (0)
#define cptr__shift_bits_align (cptr__bits_size_len)
#define cptr__shift_bits_flags (cptr__bits_size_len + cptr__bits_algin_len)

/*========== Macros and Definitions =========================================*/

#ifndef CORE_CPTR_INCLUDED
#define CORE_CPTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Core Pointer Types ============================================*/

// Base immutable pointer with metadata
typedef union cptr {
    const void* raw_;
    uptr        bits_;
    struct {
        uptr addr  : 48; // 256TB address space
        uptr size  : 12; // Up to 4096 bytes size
        uptr align : 3;  // Up to 128-byte alignment
        uptr flags : 1;  // For allowzero, etc
    } meta_;
} cptr;
claim_assert_static_msg(sizeof(cptr) == sizeof(anyptr), "cptr size mismatch");

// Base mutable pointer with metadata
typedef union cptr_mut {
    void* raw_;
    uptr  bits_;
    struct {
        uptr addr  : 48;
        uptr size  : 12;
        uptr align : 3;
        uptr flags : 1;
    } meta_;
} cptr_mut;
claim_assert_static_msg(sizeof(cptr_mut) == sizeof(anyptr_mut), "cptr_mut size mismatch");

/*========== Core Functions ================================================*/

// Creation
force_inline cptr     cptr_make(const void* raw, usize size, usize align);
force_inline cptr_mut cptr_mut_make(void* raw, usize size, usize align);
#define cptr_from(T, raw)     IMPL_cptr_from(T, raw)
#define cptr_mut_from(T, raw) IMPL_cptr_mut_from(T, raw)

// Access
force_inline const void* cptr_raw(cptr self);
force_inline void*       cptr_mut_raw(cptr_mut self);

// Metadata
force_inline usize cptr_size(cptr self);
force_inline usize cptr_mut_size(cptr_mut self);
force_inline usize cptr_align(cptr self);
force_inline usize cptr_mut_align(cptr_mut self);
force_inline bool  cptr_flags(cptr self);
force_inline bool  cptr_mut_flags(cptr_mut self);

// Flags
force_inline void     cptr_setFlags(cptr* self, bool flags);
force_inline void     cptr_mut_setFlags(cptr_mut* self, bool flags);
force_inline cptr     cptr_withFlags(cptr self, bool flags);
force_inline cptr_mut cptr_mut_withFlags(cptr_mut self, bool flags);

// Safety checks
force_inline bool cptr_hasMinSize(cptr self, usize required);
force_inline bool cptr_mut_hasMinSize(cptr_mut self, usize required);
force_inline bool cptr_isAligned(cptr self);
force_inline bool cptr_mut_isAligned(cptr_mut self);
force_inline bool cptr_isNull(cptr self);
force_inline bool cptr_mut_isNull(cptr_mut self);

// Conversion
force_inline cptr     cptr_fromMut(cptr_mut ptr);
force_inline cptr_mut cptr_asMut(cptr ptr);

// Internal helpers
force_inline unsigned int cptr__calcAlignOrder(uptr align);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_CPTR_INCLUDED */

#ifndef CORE_CPTR_IMPL_INCLUDED
#define CORE_CPTR_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Internal Helpers ==============================================*/

// Calculate log2 of alignment for compact storage
force_inline unsigned int cptr__calcAlignOrder(uptr align) {
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned int)__builtin_ctzll((unsigned long long)align);
#elif defined(_MSC_VER)
#if BUILTIN_PLTF_64BIT
    unsigned long index = 0;
    _BitScanForward64(&index, (unsigned __int64)align);
    return (unsigned int)index;
#else
    unsigned long index = 0;
    _BitScanForward(&index, (unsigned long)align);
    return (unsigned int)index;
#endif
#else
    unsigned int order = 0;
    while (align > 1) {
        align >>= 1;
        order++;
    }
    return order;
#endif
}

/*========== Creation Implementation ======================================*/

force_inline cptr cptr_make(const void* raw, usize size, usize align) {
    cptr self = cleared();
    if (!rawptrIsNull(raw)) {
        self.raw_        = raw;
        self.meta_.size  = size & ((1ull << 12) - 1);         // 12 bits for size
        self.meta_.align = cptr__calcAlignOrder(align) & 0x7; // 3 bits for align
        self.meta_.flags = 0;
    }
    return self;
}

force_inline cptr_mut cptr_mut_make(void* raw, usize size, usize align) {
    cptr_mut self = cleared();
    if (!rawptrIsNull(raw)) {
        self.raw_        = raw;
        self.meta_.size  = size & ((1ull << 12) - 1);
        self.meta_.align = cptr__calcAlignOrder(align) & 0x7;
        self.meta_.flags = 0;
    }
    return self;
}

#define IMPL_cptr_from(T, raw) \
    cptr_make((raw), sizeof(T), alignof(T))

#define IMPL_cptr_mut_from(T, raw) \
    cptr_mut_make((raw), sizeof(T), alignof(T))

/*========== Access Implementation ========================================*/

force_inline const void* cptr_raw(cptr self) {
    return intToRawptr(const void*, self.bits_ & ((1ull << 48) - 1));
}

force_inline void* cptr_mut_raw(cptr_mut self) {
    return intToRawptr(void*, self.bits_ & ((1ull << 48) - 1));
}

// Metadata

force_inline usize cptr_size(cptr self) {
    return self.meta_.size;
}

force_inline usize cptr_mut_size(cptr_mut self) {
    return self.meta_.size;
}

force_inline usize cptr_align(cptr self) {
    return 1ull << self.meta_.align;
}

force_inline usize cptr_mut_align(cptr_mut self) {
    return 1ull << self.meta_.align;
}

force_inline bool cptr_flags(cptr self) {
    return self.meta_.flags != 0;
}

force_inline bool cptr_mut_flags(cptr_mut self) {
    return self.meta_.flags != 0;
}

/*========== Flag Implementation =========================================*/

force_inline void cptr_setFlags(cptr* self, bool flags) {
    self->meta_.flags = flags & 1;
}

force_inline void cptr_mut_setFlags(cptr_mut* self, bool flags) {
    self->meta_.flags = flags & 1;
}

force_inline cptr cptr_withFlags(cptr self, bool flags) {
    cptr result        = self;
    result.meta_.flags = flags & 1;
    return result;
}

force_inline cptr_mut cptr_mut_withFlags(cptr_mut self, bool flags) {
    cptr_mut result    = self;
    result.meta_.flags = flags & 1;
    return result;
}

/*========== Safety Checks Implementation ================================*/

force_inline bool cptr_hasMinSize(cptr self, usize required) {
    return self.meta_.size >= required;
}

force_inline bool cptr_mut_hasMinSize(cptr_mut self, usize required) {
    return self.meta_.size >= required;
}

force_inline bool cptr_isAligned(cptr self) {
    return (rawptrToInt(self.raw_) % (1ull << self.meta_.align)) == 0;
}

force_inline bool cptr_mut_isAligned(cptr_mut self) {
    return (rawptrToInt(self.raw_) % (1ull << self.meta_.align)) == 0;
}

force_inline bool cptr_isNull(cptr self) {
    return rawptrIsNull(self.raw_) || (!cptr_flags(self) && rawptrToInt(self.raw_) == 0);
}

force_inline bool cptr_mut_isNull(cptr_mut self) {
    return rawptrIsNull(self.raw_) || (!cptr_mut_flags(self) && rawptrToInt(self.raw_) == 0);
}

/*========== Conversion Implementation ===================================*/

force_inline cptr cptr_fromMut(cptr_mut ptr) {
    cptr result;
    result.bits_ = ptr.bits_;
    return result;
}

force_inline cptr_mut cptr_asMut(cptr ptr) {
    cptr_mut result;
    result.bits_ = ptr.bits_;
    return result;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_CPTR_IMPL_INCLUDED */

#ifndef CORE_SPTR_INCLUDED
#define CORE_SPTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "cptr.h"
#include "opt.h"

/*========== Types ========================================================*/

// Single-item immutable pointer
typedef struct sptr {
    cptr core;
} sptr;
claim_assert_static_msg(sizeof(sptr) == sizeof(cptr), "sptr size mismatch");

// Single-item mutable pointer
typedef struct sptr_mut {
    cptr_mut core;
} sptr_mut;
claim_assert_static_msg(sizeof(sptr_mut) == sizeof(cptr_mut), "sptr_mut size mismatch");

/*========== Creation Functions ===========================================*/

force_inline sptr     sptr_fromRaw(const void* raw, usize type_size);
force_inline sptr_mut sptr_mut_fromRaw(void* raw, usize type_size);

#define sptr_ref(T, var) \
    sptr_fromRaw(ref(var), sizeof(T))

#define sptr_mut_ref(T, var) \
    sptr_mut_fromRaw(ref(var), sizeof(T))

/*========== Access Functions ============================================*/

force_inline const void* sptr_raw(sptr self);
force_inline void*       sptr_mut_raw(sptr_mut self);

#define sptr_deref(T, self) \
    deref(T, sptr_raw(self))

#define sptr_mut_deref(T, self) \
    deref_mut(T, sptr_mut_raw(self))

#define sptr_write(T, self, val) \
    (sptr_mut_deref(T, self) = (val))

// Creates slice from single item [0..1]
#define sptr_slice(T, self) \
    Slice_fromRange(T, sptr_raw(self), 0, 1)

#define sptr_mut_slice(T, self) \
    Slice_mut_fromRange(T, sptr_mut_raw(self), 0, 1)

/*========== Safety Functions ===========================================*/

force_inline bool sptr_isNull(sptr self);
force_inline bool sptr_mut_isNull(sptr_mut self);
force_inline bool sptr_hasSize(sptr self, usize type_size);
force_inline bool sptr_mut_hasSize(sptr_mut self, usize type_size);

/*========== Type Checking =============================================*/

// These functions return true if the pointer points to the given type
#define sptr_isType(T, self) \
    sptr_hasSize(self, sizeof(T))

#define sptr_mut_isType(T, self) \
    sptr_mut_hasSize(self, sizeof(T))

/*========== Conversion Functions ======================================*/

// Convert mutable to immutable pointer
force_inline sptr     sptr_fromMut(sptr_mut ptr);
force_inline sptr_mut sptr_asMut(sptr self);

/*========== Optional Functions ========================================*/

// Create optional single item pointer
typedef Optional(sptr) opt_sptr;
typedef Optional(sptr_mut) opt_sptr_mut;

// Optional ptr creation
force_inline opt_sptr     sptr_some(sptr self);
force_inline opt_sptr     sptr_none(void);
force_inline opt_sptr_mut sptr_mut_some(sptr_mut self);
force_inline opt_sptr_mut sptr_mut_none(void);

// Optional ptr checking
#define sptr_unwrap(T, opt) \
    sptr_deref(T, Opt_unwrap(opt))

#define sptr_mut_unwrap(T, opt) \
    sptr_mut_deref(T, Opt_unwrap(opt))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_SPTR_INCLUDED */

#ifndef CORE_SPTR_IMPL_INCLUDED
#define CORE_SPTR_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Creation Implementation ====================================*/

force_inline sptr sptr_fromRaw(const void* raw, usize type_size) {
    return (sptr){
        .core = cptr_make(raw, type_size, alignof(max_align_t))
    };
}

force_inline sptr_mut sptr_mut_fromRaw(void* raw, usize type_size) {
    return (sptr_mut){
        .core = cptr_mut_make(raw, type_size, alignof(max_align_t))
    };
}

/*========== Access Implementation =====================================*/

force_inline const void* sptr_raw(sptr self) {
    debug_assert_nonnull(cptr_raw(self.core));
    debug_assert_true(cptr_isAligned(self.core));
    return cptr_raw(self.core);
}

force_inline void* sptr_mut_raw(sptr_mut self) {
    debug_assert_nonnull(cptr_mut_raw(self.core));
    debug_assert_true(cptr_mut_isAligned(self.core));
    return cptr_mut_raw(self.core);
}

/*========== Safety Implementation ====================================*/

force_inline bool sptr_isNull(sptr self) {
    return cptr_isNull(self.core);
}

force_inline bool sptr_mut_isNull(sptr_mut self) {
    return cptr_mut_isNull(self.core);
}

force_inline bool sptr_hasSize(sptr self, usize type_size) {
    return cptr_size(self.core) == type_size;
}

force_inline bool sptr_mut_hasSize(sptr_mut self, usize type_size) {
    return cptr_mut_size(self.core) == type_size;
}

/*========== Conversion Implementation ================================*/

force_inline sptr sptr_fromMut(sptr_mut self) {
    return (sptr){
        .core = cptr_fromMut(self.core)
    };
}

force_inline sptr_mut sptr_asMut(sptr self) {
    return (sptr_mut){
        .core = cptr_asMut(self.core)
    };
}

/*========== Optional Implementation ==================================*/

force_inline opt_sptr sptr_some(sptr self) {
    debug_assert_false(sptr_isNull(self));
    return opt_some(opt_sptr, self);
}

force_inline opt_sptr sptr_none(void) {
    return opt_none(opt_sptr);
}

force_inline opt_sptr_mut sptr_mut_some(sptr_mut self) {
    debug_assert_false(sptr_mut_isNull(self));
    return opt_some(opt_sptr_mut, self);
}

force_inline opt_sptr_mut sptr_mut_none(void) {
    return opt_none(opt_sptr_mut);
}

/*========== Helper Macros Implementation =============================*/

#define IMPL_sptr_slice(T, self, begin, end) \
    Slice_fromRange(T, sptr_raw(self), begin, end)

#define IMPL_sptr_mut_slice(T, self, begin, end) \
    Slice_mut_fromRange(T, sptr_mut_raw(self), begin, end)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_SPTR_IMPL_INCLUDED */

#ifndef CORE_MPTR_INCLUDED
#define CORE_MPTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Types ========================================================*/

typedef struct mptr {
    cptr core;
} mptr;
claim_assert_static_msg(sizeof(mptr) == sizeof(cptr), "mptr size mismatch");

typedef struct mptr_mut {
    cptr_mut core;
} mptr_mut;
claim_assert_static_msg(sizeof(mptr_mut) == sizeof(cptr_mut), "mptr_mut size mismatch");

/*========== Creation Functions ===========================================*/

force_inline mptr     mptr_fromRaw(const void* raw, usize type_size);
force_inline mptr_mut mptr_mut_fromRaw(void* raw, usize type_size);

#define mptr_ref(T, var) \
    mptr_fromRaw(ref(var), sizeof(T))

#define mptr_mut_ref(T, var) \
    mptr_mut_fromRaw(ref(var), sizeof(T))

/*========== Access Functions ============================================*/

force_inline const void* mptr_raw(mptr self);
force_inline void*       mptr_mut_raw(mptr_mut self);

// Index access
#define mptr_index(T, self, idx) \
    deref(T, (const T*)mptr_raw(self) + (idx))

#define mptr_mut_index(T, self, idx) \
    deref_mut(T, (T*)mptr_mut_raw(self) + (idx))

#define mptr_write(T, self, idx, val) \
    (mptr_mut_index(T, self, idx) = (val))

/*========== Pointer Arithmetic =========================================*/

force_inline mptr     mptr_add(mptr self, ptrdiff offset);
force_inline mptr     mptr_sub(mptr self, ptrdiff offset);
force_inline mptr_mut mptr_mut_add(mptr_mut self, ptrdiff offset);
force_inline mptr_mut mptr_mut_sub(mptr_mut self, ptrdiff offset);
force_inline ptrdiff  mptr_diff(mptr lhs, mptr rhs);
force_inline ptrdiff  mptr_mut_diff(mptr_mut lhs, mptr_mut rhs);

/*========== Slice Creation ============================================*/

// Create slice from range [begin..end]
#define mptr_slice(T, self, begin, end) \
    Slice_fromRange(T, mptr_raw(self), begin, end)

#define mptr_mut_slice(T, self, begin, end) \
    Slice_mut_fromRange(T, mptr_mut_raw(self), begin, end)

// Create slice from start with length
#define mptr_sliceLen(T, self, start, len) \
    Slice_fromStart(T, mptr_raw(self), start, len)

#define mptr_mut_sliceLen(T, self, start, len) \
    Slice_mut_fromStart(T, mptr_mut_raw(self), start, len)

// Create slice from start to end of array (if length known)
#define mptr_sliceFrom(T, self, start, total_len) \
    Slice_fromStart(T, mptr_raw(self), start, (total_len) - (start))

#define mptr_mut_sliceFrom(T, self, start, total_len) \
    Slice_mut_fromStart(T, mptr_mut_raw(self), start, (total_len) - (start))

/*========== Safety Functions ===========================================*/

force_inline bool mptr_isNull(mptr self);
force_inline bool mptr_mut_isNull(mptr_mut self);
force_inline bool mptr_hasSize(mptr self, usize type_size);
force_inline bool mptr_mut_hasSize(mptr_mut self, usize type_size);

/*========== Type Checking =============================================*/

#define mptr_isType(T, self) \
    mptr_hasSize(self, sizeof(T))

#define mptr_mut_isType(T, self) \
    mptr_mut_hasSize(self, sizeof(T))

/*========== Conversion Functions ======================================*/

force_inline mptr     mptr_fromMut(mptr_mut self);
force_inline mptr_mut mptr_asMut(mptr self);
force_inline sptr     mptr_asSingle(mptr self);
force_inline sptr_mut mptr_mut_asSingle(mptr_mut self);

/*========== Optional Functions ========================================*/

typedef Optional(mptr) opt_mptr;
typedef Optional(mptr_mut) opt_mptr_mut;

force_inline opt_mptr     mptr_some(mptr self);
force_inline opt_mptr     mptr_none(void);
force_inline opt_mptr_mut mptr_mut_some(mptr_mut self);
force_inline opt_mptr_mut mptr_mut_none(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_MPTR_INCLUDED */

#ifndef CORE_MPTR_IMPL_INCLUDED
#define CORE_MPTR_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Creation Implementation ====================================*/

force_inline mptr mptr_fromRaw(const void* raw, usize type_size) {
    return (mptr){
        .core = cptr_make(raw, type_size, _Alignof(max_align_t))
    };
}

force_inline mptr_mut mptr_mut_fromRaw(void* raw, usize type_size) {
    return (mptr_mut){
        .core = cptr_mut_make(raw, type_size, _Alignof(max_align_t))
    };
}

/*========== Access Implementation =====================================*/

force_inline const void* mptr_raw(mptr self) {
    debug_assert_nonnull(cptr_raw(self.core));
    debug_assert_true(cptr_isAligned(self.core));
    return cptr_raw(self.core);
}

force_inline void* mptr_mut_raw(mptr_mut self) {
    debug_assert_nonnull(cptr_mut_raw(self.core));
    debug_assert_true(cptr_mut_isAligned(self.core));
    return cptr_mut_raw(self.core);
}

/*========== Pointer Arithmetic Implementation =========================*/

force_inline mptr mptr_add(mptr self, ptrdiff offset) {
    return mptr_fromRaw(
        (const char*)mptr_raw(self) + offset * cptr_size(self.core),
        cptr_size(self.core)
    );
}

force_inline mptr mptr_sub(mptr self, ptrdiff offset) {
    return mptr_fromRaw(
        (const char*)mptr_raw(self) - offset * cptr_size(self.core),
        cptr_size(self.core)
    );
}

force_inline mptr_mut mptr_mut_add(mptr_mut self, ptrdiff offset) {
    return mptr_mut_fromRaw(
        (char*)mptr_mut_raw(self) + offset * cptr_mut_size(self.core),
        cptr_mut_size(self.core)
    );
}

force_inline mptr_mut mptr_mut_sub(mptr_mut self, ptrdiff offset) {
    return mptr_mut_fromRaw(
        (char*)mptr_mut_raw(self) - offset * cptr_mut_size(self.core),
        cptr_mut_size(self.core)
    );
}

force_inline ptrdiff mptr_diff(mptr lhs, mptr rhs) {
    debug_assert_eq(cptr_size(lhs.core), cptr_size(rhs.core));
    return (ptrdiff)(((const char*)mptr_raw(lhs) - (const char*)mptr_raw(rhs)) / cptr_size(lhs.core));
}

force_inline ptrdiff mptr_mut_diff(mptr_mut lhs, mptr_mut rhs) {
    debug_assert_eq(cptr_mut_size(lhs.core), cptr_mut_size(rhs.core));
    return (ptrdiff)(((char*)mptr_mut_raw(lhs) - (char*)mptr_mut_raw(rhs)) / cptr_mut_size(lhs.core));
}

/*========== Safety Implementation ====================================*/

force_inline bool mptr_isNull(mptr self) {
    return cptr_isNull(self.core);
}

force_inline bool mptr_mut_isNull(mptr_mut self) {
    return cptr_mut_isNull(self.core);
}

force_inline bool mptr_hasSize(mptr self, usize type_size) {
    return cptr_size(self.core) == type_size;
}

force_inline bool mptr_mut_hasSize(mptr_mut self, usize type_size) {
    return cptr_mut_size(self.core) == type_size;
}

/*========== Conversion Implementation ================================*/

force_inline mptr mptr_fromMut(mptr_mut self) {
    return (mptr){
        .core = cptr_fromMut(self.core)
    };
}

force_inline mptr_mut mptr_asMut(mptr self) {
    return (mptr_mut){
        .core = cptr_asMut(self.core)
    };
}

force_inline sptr mptr_asSingle(mptr self) {
    return (sptr){
        .core = self.core
    };
}

force_inline sptr_mut mptr_mut_asSingle(mptr_mut self) {
    return (sptr_mut){
        .core = self.core
    };
}

/*========== Optional Implementation ==================================*/

force_inline opt_mptr mptr_some(mptr self) {
    debug_assert_false(mptr_isNull(self));
    return opt_some(opt_mptr, self);
}

force_inline opt_mptr mptr_none(void) {
    return opt_none(opt_mptr);
}

force_inline opt_mptr_mut mptr_mut_some(mptr_mut self) {
    debug_assert_false(mptr_mut_isNull(self));
    return opt_some(opt_mptr_mut, self);
}

force_inline opt_mptr_mut mptr_mut_none(void) {
    return opt_none(opt_mptr_mut);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_MPTR_IMPL_INCLUDED */

#ifndef CORE_NPTR_INCLUDED
#define CORE_NPTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Types ========================================================*/

typedef struct nptr {
    cptr  core;
    usize len;
} nptr;
claim_assert_static_msg(sizeof(nptr) == sizeof(usize) * 2, "nptr size mismatch");

typedef struct nptr_mut {
    cptr_mut core;
    usize    len;
} nptr_mut;
claim_assert_static_msg(sizeof(nptr_mut) == sizeof(usize) * 2, "nptr_mut size mismatch");

/*========== Creation Functions ===========================================*/

force_inline nptr     nptr_fromRaw(const void* raw, usize type_size, usize count);
force_inline nptr_mut nptr_mut_fromRaw(void* raw, usize type_size, usize count);

#define nptr_ref(T, count, var) \
    nptr_fromRaw(ref(var), sizeof(T), count)

#define nptr_mut_ref(T, count, var) \
    nptr_mut_fromRaw(ref(var), sizeof(T), count)

/*========== Access Functions ============================================*/

force_inline const void* nptr_raw(nptr self);
force_inline void*       nptr_mut_raw(nptr_mut self);
force_inline usize       nptr_len(nptr self);
force_inline usize       nptr_mut_len(nptr_mut self);

// Index access with bounds checking
#define nptr_index(T, self, idx)                \
    (debug_assert_true((idx) < nptr_len(self)), \
     deref(T, (const T*)nptr_raw(self) + (idx)))

#define nptr_mut_index(T, self, idx)                \
    (debug_assert_true((idx) < nptr_mut_len(self)), \
     deref_mut(T, (T*)nptr_mut_raw(self) + (idx)))

#define nptr_write(T, self, idx, val) \
    (nptr_mut_index(T, self, idx) = (val))

/*========== Pointer Arithmetic =========================================*/

force_inline ptrdiff nptr_diff(nptr lhs, nptr rhs);
force_inline ptrdiff nptr_mut_diff(nptr_mut lhs, nptr_mut rhs);

/*========== Slice Creation ============================================*/

// Create slice from range [begin..end] with bounds checking
#define nptr_slice(T, self, begin, end)                              \
    (debug_assert_true((begin) <= (end) && (end) <= nptr_len(self)), \
     Slice_fromRange(T, nptr_raw(self), begin, end))

#define nptr_mut_slice(T, self, begin, end)                              \
    (debug_assert_true((begin) <= (end) && (end) <= nptr_mut_len(self)), \
     Slice_mut_fromRange(T, nptr_mut_raw(self), begin, end))

// Create slice from entire array
#define nptr_wholeSlice(T, self) \
    Slice_fromRange(T, nptr_raw(self), 0, nptr_len(self))

#define nptr_mut_wholeSlice(T, self) \
    Slice_mut_fromRange(T, nptr_mut_raw(self), 0, nptr_mut_len(self))

/*========== Safety Functions ===========================================*/

force_inline bool nptr_isNull(nptr self);
force_inline bool nptr_mut_isNull(nptr_mut self);
force_inline bool nptr_hasSize(nptr self, usize type_size);
force_inline bool nptr_mut_hasSize(nptr_mut self, usize type_size);
force_inline bool nptr_inBounds(nptr self, usize index);
force_inline bool nptr_mut_inBounds(nptr_mut self, usize index);

/*========== Type Checking =============================================*/

#define nptr_isType(T, self) \
    nptr_hasSize(self, sizeof(T))

#define nptr_mut_isType(T, self) \
    nptr_mut_hasSize(self, sizeof(T))

/*========== Conversion Functions ======================================*/

force_inline nptr     nptr_fromMut(nptr_mut self);
force_inline nptr_mut nptr_asMut(nptr self);
force_inline sptr     nptr_asSingle(nptr self);
force_inline sptr_mut nptr_mut_asSingle(nptr_mut self);
force_inline mptr     nptr_asMany(nptr self);
force_inline mptr_mut nptr_mut_asMany(nptr_mut self);

/*========== Optional Functions ========================================*/

typedef Optional(nptr) opt_nptr;
typedef Optional(nptr_mut) opt_nptr_mut;

force_inline opt_nptr     nptr_some(nptr self);
force_inline opt_nptr     nptr_none(void);
force_inline opt_nptr_mut nptr_mut_some(nptr_mut self);
force_inline opt_nptr_mut nptr_mut_none(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_NPTR_INCLUDED */

#ifndef CORE_NPTR_IMPL_INCLUDED
#define CORE_NPTR_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Creation Implementation ====================================*/

force_inline nptr nptr_fromRaw(const void* raw, usize type_size, usize count) {
    return (nptr){
        .core = cptr_make(raw, type_size, _Alignof(max_align_t)),
        .len  = count
    };
}

force_inline nptr_mut nptr_mut_fromRaw(void* raw, usize type_size, usize count) {
    return (nptr_mut){
        .core = cptr_mut_make(raw, type_size, _Alignof(max_align_t)),
        .len  = count
    };
}

/*========== Access Implementation =====================================*/

force_inline const void* nptr_raw(nptr self) {
    debug_assert_nonnull(cptr_raw(self.core));
    debug_assert_true(cptr_isAligned(self.core));
    return cptr_raw(self.core);
}

force_inline void* nptr_mut_raw(nptr_mut self) {
    debug_assert_nonnull(cptr_mut_raw(self.core));
    debug_assert_true(cptr_mut_isAligned(self.core));
    return cptr_mut_raw(self.core);
}

force_inline usize nptr_len(nptr self) {
    return self.len;
}

force_inline usize nptr_mut_len(nptr_mut self) {
    return self.len;
}

/*========== Pointer Arithmetic Implementation =========================*/

force_inline ptrdiff nptr_diff(nptr lhs, nptr rhs) {
    debug_assert_eq(cptr_size(lhs.core), cptr_size(rhs.core));
    return (ptrdiff)(((const char*)nptr_raw(lhs) - (const char*)nptr_raw(rhs)) / cptr_size(lhs.core));
}

force_inline ptrdiff nptr_mut_diff(nptr_mut lhs, nptr_mut rhs) {
    debug_assert_eq(cptr_mut_size(lhs.core), cptr_mut_size(rhs.core));
    return (ptrdiff)(((char*)nptr_mut_raw(lhs) - (char*)nptr_mut_raw(rhs)) / cptr_mut_size(lhs.core));
}

/*========== Safety Implementation ====================================*/

force_inline bool nptr_isNull(nptr self) {
    return cptr_isNull(self.core);
}

force_inline bool nptr_mut_isNull(nptr_mut self) {
    return cptr_mut_isNull(self.core);
}

force_inline bool nptr_hasSize(nptr self, usize type_size) {
    return cptr_size(self.core) == type_size;
}

force_inline bool nptr_mut_hasSize(nptr_mut self, usize type_size) {
    return cptr_mut_size(self.core) == type_size;
}

force_inline bool nptr_inBounds(nptr self, usize index) {
    return index < self.len;
}

force_inline bool nptr_mut_inBounds(nptr_mut self, usize index) {
    return index < self.len;
}

/*========== Conversion Implementation ================================*/

force_inline nptr nptr_fromMut(nptr_mut self) {
    return (nptr){
        .core = cptr_fromMut(self.core),
        .len  = self.len
    };
}

force_inline nptr_mut nptr_asMut(nptr self) {
    return (nptr_mut){
        .core = cptr_asMut(self.core),
        .len  = self.len
    };
}

force_inline sptr nptr_asSingle(nptr self) {
    debug_assert_true(self.len > 0);
    return (sptr){
        .core = self.core
    };
}

force_inline sptr_mut nptr_mut_asSingle(nptr_mut self) {
    debug_assert_true(self.len > 0);
    return (sptr_mut){
        .core = self.core
    };
}

force_inline mptr nptr_asMany(nptr self) {
    return (mptr){
        .core = self.core
    };
}

force_inline mptr_mut nptr_mut_asMany(nptr_mut self) {
    return (mptr_mut){
        .core = self.core
    };
}

/*========== Optional Implementation ==================================*/

force_inline opt_nptr nptr_some(nptr self) {
    debug_assert_false(nptr_isNull(self));
    return opt_some(opt_nptr, self);
}

force_inline opt_nptr nptr_none(void) {
    return opt_none(opt_nptr);
}

force_inline opt_nptr_mut nptr_mut_some(nptr_mut self) {
    debug_assert_false(nptr_mut_isNull(self));
    return opt_some(opt_nptr_mut, self);
}

force_inline opt_nptr_mut nptr_mut_none(void) {
    return opt_none(opt_nptr_mut);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_NPTR_IMPL_INCLUDED */

#ifndef CORE_SLICE_INCLUDED
#define CORE_SLICE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Types ========================================================*/

typedef struct Slice {
    mptr  ptr;
    usize len;
} Slice;
claim_assert_static_msg(sizeof(Slice) == sizeof(usize) * 2, "Slice size mismatch");

typedef struct Slice_mut {
    mptr_mut ptr;
    usize    len;
} Slice_mut;
claim_assert_static_msg(sizeof(Slice_mut) == sizeof(usize) * 2, "Slice_mut size mismatch");

/*========== Creation Functions ===========================================*/

force_inline Slice     Slice_fromParts(mptr ptr, usize len);
force_inline Slice_mut Slice_mut_fromParts(mptr_mut ptr, usize len);

#define Slice_fromArray(T, var) \
    Slice_fromParts(mptr_ref(T, var), sizeof(var) / sizeof(T))

#define Slice_mut_fromArray(T, var) \
    Slice_mut_fromParts(mptr_mut_ref(T, var), sizeof(var) / sizeof(T))

#define Slice_fromRange(T, data, begin, end) \
    Slice_fromParts(mptr_add(mptr_ref(T, data), begin), (end) - (begin))

#define Slice_mut_fromRange(T, data, begin, end) \
    Slice_mut_fromParts(mptr_mut_add(mptr_mut_ref(T, data), begin), (end) - (begin))

// Alternative initialization using result location
#define Slice_fromLiteral(T, ...) \
    Slice_fromArray(T, ((T[]){ __VA_ARGS__ }))

/*========== Access Functions ============================================*/

force_inline const void* Slice_raw(Slice self);
force_inline void*       Slice_mut_raw(Slice_mut self);
force_inline usize       Slice_len(Slice self);
force_inline usize       Slice_mut_len(Slice_mut self);

// Index access with bounds checking
#define Slice_get(T, self, idx)                  \
    (debug_assert_true((idx) < Slice_len(self)), \
     deref(T, (const T*)Slice_raw(self) + (idx)))

#define Slice_mut_get(T, self, idx)                  \
    (debug_assert_true((idx) < Slice_mut_len(self)), \
     deref(T, (T*)Slice_mut_raw(self) + (idx)))

#define Slice_set(T, self, idx, val)                 \
    (debug_assert_true((idx) < Slice_mut_len(self)), \
     (((T*)Slice_mut_raw(self))[idx] = (val)))

/*========== Slice Operations ===========================================*/

// Create sub-slice
#define Slice_slice(T, self, begin, end)                              \
    (debug_assert_true((begin) <= (end) && (end) <= Slice_len(self)), \
     Slice_fromRange(T, Slice_raw(self), begin, end))

#define Slice_mut_slice(T, self, begin, end)                              \
    (debug_assert_true((begin) <= (end) && (end) <= Slice_mut_len(self)), \
     Slice_mut_fromRange(T, Slice_mut_raw(self), begin, end))

// Get pointer to first element
force_inline sptr     Slice_ptr(Slice self);
force_inline sptr_mut Slice_mut_ptr(Slice_mut self);

// Get many-item pointer
force_inline mptr     Slice_manyPtr(Slice self);
force_inline mptr_mut Slice_mut_manyPtr(Slice_mut self);

/*========== Comparison Functions =======================================*/

// Compare entire slices
force_inline bool Slice_eql(Slice lhs, Slice rhs);
force_inline bool Slice_mut_eql(Slice_mut lhs, Slice_mut rhs);

// Compare portions of slices
#define Slice_eqlRange(T, lhs, l_begin, l_end, rhs, r_begin, r_end) \
    (((l_end) - (l_begin) == (r_end) - (r_begin)) && Slice_eql(Slice_slice(T, lhs, l_begin, l_end), Slice_slice(T, rhs, r_begin, r_end)))

/*========== Memory Functions ==========================================*/

// Copy between slices
force_inline void Slice_copy(Slice_mut dest, Slice src);
force_inline void Slice_mut_copy(Slice_mut dest, Slice_mut src);

// Clear slice (set to 0)
force_inline void Slice_clear(Slice_mut self);

/*========== Safety Functions =========================================*/

force_inline bool Slice_isNull(Slice self);
force_inline bool Slice_mut_isNull(Slice_mut self);
force_inline bool Slice_isEmpty(Slice self);
force_inline bool Slice_mut_isEmpty(Slice_mut self);
force_inline bool Slice_hasSize(Slice self, usize type_size);
force_inline bool Slice_mut_hasSize(Slice_mut self, usize type_size);
force_inline bool Slice_inBounds(Slice self, usize index);
force_inline bool Slice_mut_inBounds(Slice_mut self, usize index);

/*========== Type Checking ===========================================*/

#define Slice_isType(T, self) \
    Slice_hasSize(self, sizeof(T))

#define Slice_mut_isType(T, self) \
    Slice_mut_hasSize(self, sizeof(T))

/*========== Conversion Functions ====================================*/

force_inline Slice Slice_fromMut(Slice_mut self);

/*========== Optional Functions ======================================*/

typedef Optional(Slice) opt_Slice;
typedef Optional(Slice_mut) opt_Slice_mut;

force_inline opt_Slice     Slice_some(Slice self);
force_inline opt_Slice     Slice_none(void);
force_inline opt_Slice_mut Slice_mut_some(Slice_mut self);
force_inline opt_Slice_mut Slice_mut_none(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_SLICE_INCLUDED */

#ifndef CORE_SLICE_IMPL_INCLUDED
#define CORE_SLICE_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <string.h> // For memcpy, memset

/*========== Creation Implementation ====================================*/

force_inline Slice Slice_fromParts(mptr ptr, usize len) {
    return (Slice){ .ptr = ptr, .len = len };
}

force_inline Slice_mut Slice_mut_fromParts(mptr_mut ptr, usize len) {
    return (Slice_mut){ .ptr = ptr, .len = len };
}

/*========== Access Implementation =====================================*/

force_inline const void* Slice_raw(Slice self) {
    debug_assert_nonnull(mptr_raw(self.ptr));
    debug_assert_true(cptr_isAligned(self.ptr.core));
    return mptr_raw(self.ptr);
}

force_inline void* Slice_mut_raw(Slice_mut self) {
    debug_assert_nonnull(mptr_mut_raw(self.ptr));
    debug_assert_true(cptr_mut_isAligned(self.ptr.core));
    return mptr_mut_raw(self.ptr);
}

force_inline usize Slice_len(Slice self) {
    return self.len;
}

force_inline usize Slice_mut_len(Slice_mut self) {
    return self.len;
}

/*========== Slice Operation Implementation ===========================*/

force_inline sptr Slice_ptr(Slice self) {
    debug_assert_true(self.len > 0);
    return sptr_fromRaw(Slice_raw(self), cptr_size(self.ptr.core));
}

force_inline sptr_mut Slice_mut_ptr(Slice_mut self) {
    debug_assert_true(self.len > 0);
    return sptr_mut_fromRaw(Slice_mut_raw(self), cptr_mut_size(self.ptr.core));
}

force_inline mptr Slice_manyPtr(Slice self) {
    return self.ptr;
}

force_inline mptr_mut Slice_mut_manyPtr(Slice_mut self) {
    return self.ptr;
}

/*========== Comparison Implementation ================================*/

force_inline bool Slice_eql(Slice lhs, Slice rhs) {
    if (lhs.len != rhs.len || !mptr_hasSize(lhs.ptr, cptr_size(rhs.ptr.core))) {
        return false;
    }
    return memcmp(Slice_raw(lhs), Slice_raw(rhs), lhs.len * cptr_size(lhs.ptr.core)) == 0;
}

force_inline bool Slice_mut_eql(Slice_mut lhs, Slice_mut rhs) {
    if (lhs.len != rhs.len || !mptr_mut_hasSize(lhs.ptr, cptr_mut_size(rhs.ptr.core))) {
        return false;
    }
    return memcmp(Slice_mut_raw(lhs), Slice_mut_raw(rhs), lhs.len * cptr_mut_size(lhs.ptr.core)) == 0;
}

/*========== Memory Operation Implementation ==========================*/

force_inline void Slice_copy(Slice_mut dest, Slice src) {
    debug_assert_eq(dest.len, src.len);
    debug_assert_true(mptr_mut_hasSize(dest.ptr, cptr_size(src.ptr.core)));
    memcpy(Slice_mut_raw(dest), Slice_raw(src), dest.len * cptr_mut_size(dest.ptr.core));
}

force_inline void Slice_mut_copy(Slice_mut dest, Slice_mut src) {
    debug_assert_eq(dest.len, src.len);
    debug_assert_true(mptr_mut_hasSize(dest.ptr, cptr_mut_size(src.ptr.core)));
    memmove(Slice_mut_raw(dest), Slice_mut_raw(src), dest.len * cptr_mut_size(dest.ptr.core));
}

force_inline void Slice_clear(Slice_mut self) {
    memset(Slice_mut_raw(self), 0, self.len * cptr_mut_size(self.ptr.core));
}

/*========== Safety Implementation ====================================*/

force_inline bool Slice_isNull(Slice self) {
    return mptr_isNull(self.ptr);
}

force_inline bool Slice_mut_isNull(Slice_mut self) {
    return mptr_mut_isNull(self.ptr);
}

force_inline bool Slice_isEmpty(Slice self) {
    return self.len == 0;
}

force_inline bool Slice_mut_isEmpty(Slice_mut self) {
    return self.len == 0;
}

force_inline bool Slice_hasSize(Slice self, usize type_size) {
    return mptr_hasSize(self.ptr, type_size);
}

force_inline bool Slice_mut_hasSize(Slice_mut self, usize type_size) {
    return mptr_mut_hasSize(self.ptr, type_size);
}

force_inline bool Slice_inBounds(Slice self, usize index) {
    return index < self.len;
}

force_inline bool Slice_mut_inBounds(Slice_mut self, usize index) {
    return index < self.len;
}

/*========== Conversion Implementation ================================*/

force_inline Slice Slice_fromMut(Slice_mut self) {
    return (Slice){
        .ptr = mptr_fromMut(self.ptr),
        .len = self.len
    };
}

/*========== Optional Implementation ==================================*/

force_inline opt_Slice Slice_some(Slice self) {
    debug_assert_false(Slice_isNull(self));
    return opt_some(opt_Slice, self);
}

force_inline opt_Slice Slice_none(void) {
    return opt_none(opt_Slice);
}

force_inline opt_Slice_mut Slice_mut_some(Slice_mut self) {
    debug_assert_false(Slice_mut_isNull(self));
    return opt_some(opt_Slice_mut, self);
}

force_inline opt_Slice_mut Slice_mut_none(void) {
    return opt_none(opt_Slice_mut);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_SLICE_IMPL_INCLUDED */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PTRS_INCLUDED */
