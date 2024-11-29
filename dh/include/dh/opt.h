/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  opt
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef OPT_INCLUDED
#define OPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

// Optional definition
#define opt(T)          \
    typedef struct {    \
        T    value;     \
        bool has_value; \
    }

// Optional creation
#define opt_some(TOpt, val) ((TOpt){ .value = (val), .has_value = true })
#define opt_none(TOpt)      ((TOpt){ .has_value = false })

// Optional operations
#define opt_unwrap(opt) \
    (debug_assert_true((opt).has_value), (opt).value)

#define opt_unwrapOr(opt, default_val) \
    ((opt).has_value ? (opt).value : (default_val))

#define opt_map(TOpt, opt, fn) \
    ((opt).has_value ? opt_some(TOpt, fn((opt).value)) : opt_none(TOpt))

// Optional built-in types
opt(u8) opt_u8;
opt(u16) opt_u16;
opt(u32) opt_u32;
opt(u64) opt_u64;
opt(uptr) opt_uptr;
opt(usize) opt_usize;

opt(i8) opt_i8;
opt(i16) opt_i16;
opt(i32) opt_i32;
opt(i64) opt_i64;
opt(iptr) opt_iptr;
opt(isize) opt_isize;

opt(f32) opt_f32;
opt(f64) opt_f64;

opt(bool) opt_bool;
opt(char) opt_char;

opt(anyptr) opt_anyptr;
opt(cptr) opt_cptr;
opt(sptr) opt_sptr;
opt(mptr) opt_mptr;
opt(Slice) opt_Slice;

// Example usage:
// Optional(sptr) result = mem_tryAllocT(alloc, int);
// if (result.has_value) {
//     int* ptr = result.value.raw;
//     // use ptr
// }

// // Memory allocation with Optional
// force_inline Optional(sptr) mem_tryAlloc(mem_Allocator* alloc, usize size) {
//     sptr ptr = IMPL_mem_alloc(alloc, size, __FILE__, __LINE__, __func__);
//     return ptr.raw ? opt_some(sptr, ptr) : opt_none(sptr);
// }

// #define mem_tryAllocT(alloc, T) \
//     mem_tryAlloc(alloc, sizeof(T))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
