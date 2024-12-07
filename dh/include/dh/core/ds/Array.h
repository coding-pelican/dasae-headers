/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-12-06 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/ds
 * @prefix  Arr
 *
 * @brief   Header for Fixed-Length Array
 * @details Generic Fixed-Length Array type for following copy-value-semantics
            when passing array as argument or using array as return value.
 */

#ifndef CORE_ARRAY_INCLUDED
#define CORE_ARRAY_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/debug/assert.h"

/*========== Array Base Type =========================================*/

// Array base structure (common layout for all array types)
typedef struct ArrayBase {
    cptr  ptr; // Points to data with size/align info
    usize len; // Number of elements
} ArrayBase;

/*========== Array Definition =======================================*/

// Fixed-size array with length N storing elements of type T
#define Array(T, N)        \
    typedef struct {       \
        ArrayBase base;    \
        T         data[N]; \
    }

/*========== Array Base Operations =================================*/

// Initialize array base
force_inline ArrayBase ArrayBase_make(anyptr data, usize elem_size, usize elem_align, usize len) {
    return (ArrayBase){
        .ptr = cptr_make(data, elem_size, elem_align),
        .len = len
    };
}

// Access array properties
force_inline anyptr ArrayBase_raw(ArrayBase self) {
    return cptr_raw(self.ptr);
}

force_inline usize ArrayBase_size(ArrayBase self) {
    return cptr_size(self.ptr);
}

force_inline usize ArrayBase_align(ArrayBase self) {
    return cptr_align(self.ptr);
}

force_inline usize ArrayBase_len(ArrayBase self) {
    return self.len;
}

force_inline Sptr ArrayBase_at(ArrayBase self, usize index) {
    debug_assert_fmt(index < self.len, "Index out of bounds");
    return cptr_make(
        cptr_raw(self.ptr) + ArrayBase_len(self) * index,
        ArrayBase_size(self),
        ArrayBase_align(self)
    );
}

// Array base checks
force_inline bool ArrayBase_isZero(ArrayBase self) {
    return cptr_isZero(self.ptr);
}

force_inline bool ArrayBase_isAligned(ArrayBase self) {
    return cptr_isAligned(self.ptr);
}

// Convert array to slice
force_inline Slice ArrayBase_asSlice(ArrayBase self) {
    return Slice_make(self.ptr, self.len);
}

/*========== Type-Safe Creation ====================================*/

#define Array_init(T, N, self) \
    (self)->base = ArrayBase_make((self)->data, sizeof(T), alignof(T), N)

/*========== Type-Safe Access =====================================*/

#define Array_at(T, self, idx) \
    Sptr_deref(T, ArrayBase_at((self)->base, idx))

/*========== Memory Operations ===================================*/

force_inline void ArrayBase_copy(ArrayBase dest, ArrayBase src) {
    debug_assert(ArrayBase_len(dest) >= ArrayBase_len(src));
    debug_assert(ArrayBase_size(dest) == ArrayBase_size(src));
    memcpy(
        ArrayBase_raw(dest),
        ArrayBase_raw(src),
        ArrayBase_len(src) * ArrayBase_size(src)
    );
}

force_inline void ArrayBase_clear(ArrayBase self) {
    memset(
        ArrayBase_raw(self),
        0,
        ArrayBase_len(self) * ArrayBase_size(self)
    );
}

#define Array_copy(dest, src)                                   \
    do {                                                        \
        ArrayBase_copy((dest)->base, (src)->base);              \
        memcpy((dest)->data, (src)->data, sizeof((src)->data)); \
    } while (0)

#define Array_clear(self)                              \
    do {                                               \
        ArrayBase_clear((self)->base);                 \
        memset((self)->data, 0, sizeof((self)->data)); \
    } while (0)

/*========== Common Array Types ==================================*/

// Byte arrays
Array(u8, 32) Array_u8_32;
Array(u8, 64) Array_u8_64;
Array(u8, 128) Array_u8_128;
Array(u8, 256) Array_u8_256;

// Character arrays
Array(char, 32) Array_char_32;
Array(char, 64) Array_char_64;
Array(char, 128) Array_char_128;
Array(char, 256) Array_char_256;

// Integer arrays
Array(i32, 16) Array_i32_16;
Array(i32, 32) Array_i32_32;
Array(i32, 64) Array_i32_64;

// Pointer arrays
Array(anyptr, 16) Array_anyptr_16;
Array(anyptr, 32) Array_anyptr_32;
Array(anyptr, 64) Array_anyptr_64;

#if defined(__cplusplus)
}
#endif
#endif /* CORE_ARRAY_INCLUDED */
