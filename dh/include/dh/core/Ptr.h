/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
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

#ifndef CORE_PTR_INCLUDED
#define CORE_PTR_INCLUDED
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"
#include "dh/claim/unreachable.h"

#include <stdalign.h>
#include <stddef.h>
#include <string.h>

/*========== Metadata Layout ================================================*/

// Core pointer metadata layout
#define cptr_bits_addr_len (48) // 256 TB address space

#define cptr_bits_size_len  (12) // Up to 4096 bytes size
#define cptr_bits_algin_len (3)  // Up to 128 bytes alignment
#define cptr_bits_flags_len (1)  // boolean flags

// Masks and shifts
#define cptr_mask_bits_size  ((1ull << cptr_bits_size_len) - 1)
#define cptr_mask_bits_align ((1ull << cptr_bits_algin_len) - 1)
#define cptr_mask_bits_flags ((1ull << cptr_bits_flags_len) - 1)

#define cptr_shift_bits_size  (0)
#define cptr_shift_bits_align (cptr_bits_size_len)
#define cptr_shift_bits_flags (cptr_bits_size_len + cptr_bits_algin_len)

/*========== Core Pointer Type ==============================================*/

typedef union cptr {
    anyptr raw_;
    uptr   bits_;
    struct {
        uptr addr : cptr_bits_addr_len;   // 256TB address space
        uptr size : cptr_bits_size_len;   // Up to 4096 bytes size
        uptr align : cptr_bits_algin_len; // Up to 128 bytes alignment
        uptr flags : cptr_bits_flags_len; // For allowzero, etc
    } meta_;
} cptr;
claim_assert_static_msg(sizeof(cptr) == sizeof(anyptr), "cptr size mismatch");

// void test() {
//     cptr p = *(cptr*)&((union {
//         i32* raw_;
//         uptr bits_;
//         struct {
//             uptr addr : cptr_bits_addr_len;
//             uptr size : cptr_bits_size_len;
//             uptr align : cptr_bits_algin_len;
//             uptr flags : cptr_bits_flags_len;
//         } meta_;
//     }){ .raw_ = 0 });
// }

/*========== Core Operations ================================================*/

// Creation
force_inline cptr ptr_make(anyptr raw, usize elem_size, usize elem_align);
force_inline cptr ptr_makeWithFlags(anyptr raw, usize elem_size, usize elem_align, bool flags);
force_inline cptr ptr_withFlags(cptr self, bool flags);

// Access
force_inline const anyptr ptr_raw(cptr self);
force_inline usize        ptr_size(cptr self);
force_inline usize        ptr_align(cptr self);
force_inline bool         ptr_flags(cptr self);
force_inline void         ptr_setFlags(cptr* self, bool flags);

// Safety
force_inline bool ptr_isNull(cptr self);
force_inline bool ptr_hasMinSize(cptr self, usize required_size);
force_inline bool ptr_isAligned(cptr self);

// Internal helpers
force_inline unsigned int ptr__calcAlignOrder(uptr align);

/*========== Single-Item Pointer ========================================*/

typedef cptr sptr; // Alias for semantic clarity
#define sptr(THint) sptr

// Creation
#define sptr_make(T, raw) \
    ptr_make(raw, sizeof(T), alignof(T))

#define sptr_ref(T, var) \
    sptr_make(T, &(var))

// Access
#define sptr_deref(T, self) \
    (*(T*)ptr_raw(self))

#define sptr_get(T, self) \
    (*(T*)ptr_raw(self))

#define sptr_set(T, self, val) \
    (*(T*)ptr_raw(self) = (val))

/*========== Many-Item Pointer =========================================*/

typedef cptr mptr; // Alias for semantic clarity
#define mptr(THint) mptr

// Creation
#define mptr_make(T, raw) \
    ptr_make(raw, sizeof(T), alignof(T))

#define mptr_ref(T, var) \
    mptr_make(T, &(var))

// Access
#define mptr_index(T, self, idx) \
    (((T*)ptr_raw(self))[idx])

// Arithmetic
force_inline mptr    mptr_add(mptr self, ptrdiff offset);
force_inline mptr    mptr_sub(mptr self, ptrdiff offset);
force_inline ptrdiff mptr_diff(mptr lhs, mptr rhs);

/*========== Slice Type ===============================================*/

typedef struct Slice {
    mptr  ptr;
    usize len;
} Slice;
#define Slice(THint) Slice

// Creation
force_inline Slice Slice_make(mptr ptr, usize len);

#define Slice_fromArray(T, arr) \
    Slice_make(mptr_make(T, arr), sizeof(arr) / sizeof(T))

#define Slice_fromRange(T, cptr, begin, end) \
    Slice_make(mptr_add(mptr_make(T, cptr), begin), (end) - (begin))

// Access
force_inline const anyptr Slice_raw(Slice self);
force_inline usize        Slice_len(Slice self);

#define Slice_get(T, self, idx) \
    (((T*)Slice_raw(self))[idx])

#define Slice_set(T, self, idx, val) \
    (((T*)Slice_raw(self))[idx] = (val))

// Sub-slice
#define Slice_slice(T, self, begin, end) \
    Slice_fromRange(T, (anyptr)Slice_raw(self), begin, end)

// Memory operations
void Slice_copy(Slice dest, Slice src);
void Slice_clear(Slice self);

/*========== Type-Safe Macros =========================================*/

// Type checking
#define ptr_isType(T, self) \
    ptr_size(self) == sizeof(T)

// Safe conversion helpers
#define ptr_asSlice(T, self, len) \
    Slice_make(mptr_make(T, ptr_raw(self)), len)

// Optional support
#define ptr_unwrap(T, opt) \
    (*(T*)ptr_raw(Opt_unwrap(opt)))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PTR_INCLUDED */
