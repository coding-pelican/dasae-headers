/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-12-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Opt
 *
 * @brief   Header for Optional type handling
 * @details Generic Optional type for none(null) handling
 */

#ifndef CORE_OPT_INCLUDED
#define CORE_OPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"
#include "ptr.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

/* Optional tag enum */
typedef enum OptTag {
    OptTag_none = 0,
    OptTag_some
} OptTag;

/* Optional definition for concrete types */
#define Opt(T)           \
    typedef union {      \
        OptBase Base[1]; \
        struct {         \
            OptTag tag;  \
            union {      \
                T value; \
            } data;      \
        };               \
    }

/* Optional creation */
#define Opt_some(TOpt, val) ((TOpt){ .tag = OptTag_some, .data.value = (val) })
#define Opt_none(TOpt)      ((TOpt){ .tag = OptTag_none })

/* Optional checks */
#define Opt_isSome(self) OptBase_isNone((OptBase*)&(self))
#define Opt_isNone(self) OptBase_isNone((OptBase*)&(self))

// NOLINTBEGIN
/* Safe unwrapping with runtime checks */
#define Opt_unwrap(TOpt, self) (                                     \
    ((TOpt*)OptBase_unwrap((OptBase*)(TOpt[1]){ self }))->data.value \
)

/* Unwrap with default (orelse) */
#define Opt_unwrapOr(TOpt, self, val_default) (                                                                            \
    ((TOpt*)OptBase_unwrapOr((OptBase*)(TOpt[1]){ self }, (OptBase*)(TOpt[1]){ Opt_some(TOpt, val_default) }))->data.value \
)
// NOLINTEND

// /* Optional binding */
// #define let_Opt(T, var, opt)                                  \
//     for (T var = (Opt_isNone(opt) ? (T){ 0 } : (opt).data.value); \
//          !Opt_isNone(opt);                                        \
//          (opt).base.tag = OptTag_none)
/* Base Optional structure for internal use */

typedef struct OptBase {
    OptTag tag;
    union {
        anyptr value;
    } data;
} OptBase;

force_inline bool OptBase_isSome(OptBase* self) {
    return self->tag == OptTag_some;
}

force_inline bool OptBase_isNone(OptBase* self) {
    return self->tag == OptTag_none;
}

force_inline OptBase* OptBase_unwrap(OptBase* self) {
    return debug_assert_fmt(OptBase_isSome(self), "Attempted to unwrap none optional"), self;
}

force_inline OptBase* OptBase_unwrapOr(OptBase* self, OptBase* val_default) {
    return OptBase_isSome(self) ? self : val_default;
}

/* Optional built-in types */
// primitive types
Opt(u8) Opt_u8;
Opt(u16) Opt_u16;
Opt(u32) Opt_u32;
Opt(u64) Opt_u64;
Opt(usize) Opt_usize;

Opt(i8) Opt_i8;
Opt(i16) Opt_i16;
Opt(i32) Opt_i32;
Opt(i64) Opt_i64;
Opt(isize) Opt_isize;

Opt(f32) Opt_f32;
Opt(f64) Opt_f64;

Opt(bool) Opt_bool;
Opt(char) Opt_char;

Opt(anyptr) Opt_anyptr;
Opt(PtrBase) Opt_PtrBase;
Opt(Sptr) Opt_Sptr;
Opt(Mptr) Opt_Mptr;
Opt(Slice) Opt_Slice;

// single-item pointer types
Opt(Sptr_u8) Opt_Sptr_u8;
Opt(Sptr_u16) Opt_Sptr_u16;
Opt(Sptr_u32) Opt_Sptr_u32;
Opt(Sptr_u64) Opt_Sptr_u64;
Opt(Sptr_usize) Opt_Sptr_usize;

Opt(Sptr_i8) Opt_Sptr_i8;
Opt(Sptr_i16) Opt_Sptr_i16;
Opt(Sptr_i32) Opt_Sptr_i32;
Opt(Sptr_i64) Opt_Sptr_i64;
Opt(Sptr_isize) Opt_Sptr_isize;

Opt(Sptr_f32) Opt_Sptr_f32;
Opt(Sptr_f64) Opt_Sptr_f64;

Opt(Sptr_bool) Opt_Sptr_bool;
Opt(Sptr_char) Opt_Sptr_char;

Opt(Sptr_anyptr) Opt_Sptr_anyptr;
Opt(Sptr_PtrBase) Opt_Sptr_PtrBase;
Opt(Sptr_Sptr) Opt_Sptr_Sptr;
Opt(Sptr_Mptr) Opt_Sptr_Mptr;
Opt(Sptr_Slice) Opt_Sptr_Slice;

// many-item pointer types
Opt(Mptr_u8) Opt_Mptr_u8;
Opt(Mptr_u16) Opt_Mptr_u16;
Opt(Mptr_u32) Opt_Mptr_u32;
Opt(Mptr_u64) Opt_Mptr_u64;
Opt(Mptr_usize) Opt_Mptr_usize;

Opt(Mptr_i8) Opt_Mptr_i8;
Opt(Mptr_i16) Opt_Mptr_i16;
Opt(Mptr_i32) Opt_Mptr_i32;
Opt(Mptr_i64) Opt_Mptr_i64;
Opt(Mptr_isize) Opt_Mptr_isize;

Opt(Mptr_f32) Opt_Mptr_f32;
Opt(Mptr_f64) Opt_Mptr_f64;

Opt(Mptr_bool) Opt_Mptr_bool;
Opt(Mptr_char) Opt_Mptr_char;

Opt(Mptr_anyptr) Opt_Mptr_anyptr;
Opt(Mptr_PtrBase) Opt_Mptr_PtrBase;
Opt(Mptr_Sptr) Opt_Mptr_Sptr;
Opt(Mptr_Mptr) Opt_Mptr_Mptr;
Opt(Mptr_Slice) Opt_Mptr_Slice;

// slice types
Opt(Slice_u8) Opt_Slice_u8;
Opt(Slice_u16) Opt_Slice_u16;
Opt(Slice_u32) Opt_Slice_u32;
Opt(Slice_u64) Opt_Slice_u64;
Opt(Slice_usize) Opt_Slice_usize;

Opt(Slice_i8) Opt_Slice_i8;
Opt(Slice_i16) Opt_Slice_i16;
Opt(Slice_i32) Opt_Slice_i32;
Opt(Slice_i64) Opt_Slice_i64;
Opt(Slice_isize) Opt_Slice_isize;

Opt(Slice_f32) Opt_Slice_f32;
Opt(Slice_f64) Opt_Slice_f64;

Opt(Slice_bool) Opt_Slice_bool;
Opt(Slice_char) Opt_Slice_char;

Opt(Slice_anyptr) Opt_Slice_anyptr;
Opt(Slice_PtrBase) Opt_Slice_PtrBase;
Opt(Slice_Sptr) Opt_Slice_Sptr;
Opt(Slice_Mptr) Opt_Slice_Mptr;
Opt(Slice_Slice) Opt_Slice_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_OPT_INCLUDED */
