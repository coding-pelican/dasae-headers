/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Res.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-12-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Res
 *
 * @brief   Header for Result type handling
 * @details Generic Result type for error handling
 */

#ifndef CORE_RES_INCLUDED
#define CORE_RES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"
#include "ptr.h"
#include "Err.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

// Res tag enum
typedef enum ResTag {
    ResTag_err = 0,
    ResTag_ok
} ResTag;

// Res definition for concrete types
#define Res(T, E)        \
    typedef union {      \
        ResBase Base[1]; \
        struct {         \
            ResTag tag;  \
            union {      \
                T value; \
                E error; \
            } data;      \
        };               \
    }

// Res creation
#define Res_ok(TRes, val)  ((TRes){ .tag = ResTag_ok, .data.value = (val) })
#define Res_err(TRes, err) ((TRes){ .tag = ResTag_err, .data.error = (err) })

// Res operations
#define Res_isOk(self)  ResBase_isOk((ResBase*)&(self))
#define Res_isErr(self) ResBase_isErr((ResBase*)&(self))

#define Res_unwrap(TRes, self) \
    (((TRes*)ResBase_unwrap((ResBase*)(TRes[1]){ self }))->data.value) // NOLINT

#define Res_unwrapErr(TRes, self) \
    (((TRes*)ResBase_unwrapErr((ResBase*)(TRes[1]){ self }))->data.error) // NOLINT

#define Res_unwrapOr(TRes, self, val_default) \
    (((TRes*)ResBase_unwrapOr((ResBase*)(TRes[1]){ self }, (ResBase*)(TRes[1]){ Res_ok(TRes, val_default) }))->data.value) // NOLINT

#define Res_try

// Base Res structure for internal use
typedef struct ResBase {
    ResTag tag;
    union {
        anyptr value;
        anyptr error;
    } data;
} ResBase;

force_inline bool ResBase_isOk(ResBase* self) {
    return self->tag == ResTag_ok;
}

force_inline bool ResBase_isErr(ResBase* self) {
    return self->tag == ResTag_err;
}

force_inline ResBase* ResBase_unwrap(ResBase* self) {
    return debug_assert_true(ResBase_isOk(self)), self;
}

force_inline ResBase* ResBase_unwrapErr(ResBase* self) {
    return debug_assert_true(ResBase_isErr(self)), self;
}

force_inline ResBase* ResBase_unwrapOr(ResBase* self, ResBase* val_default) {
    return ResBase_isOk(self) ? self : val_default;
}

/* Result built-in types */
// primitive types
Res(u8, Err) Res_u8;
Res(u16, Err) Res_u16;
Res(u32, Err) Res_u32;
Res(u64, Err) Res_u64;
Res(usize, Err) Res_usize;

Res(i8, Err) Res_i8;
Res(i16, Err) Res_i16;
Res(i32, Err) Res_i32;
Res(i64, Err) Res_i64;
Res(isize, Err) Res_isize;

Res(f32, Err) Res_f32;
Res(f64, Err) Res_f64;

Res(bool, Err) Res_bool;
Res(char, Err) Res_char;
Res(Void, Err) Res_Void;

Res(anyptr, Err) Res_anyptr;
Res(PtrBase, Err) Res_PtrBase;
Res(Sptr, Err) Res_Sptr;
Res(Mptr, Err) Res_Mptr;
Res(Slice, Err) Res_Slice;

// single-item pointer types
Res(Sptr_u8, Err) Res_Sptr_u8;
Res(Sptr_u16, Err) Res_Sptr_u16;
Res(Sptr_u32, Err) Res_Sptr_u32;
Res(Sptr_u64, Err) Res_Sptr_u64;
Res(Sptr_usize, Err) Res_Sptr_usize;

Res(Sptr_i8, Err) Res_Sptr_i8;
Res(Sptr_i16, Err) Res_Sptr_i16;
Res(Sptr_i32, Err) Res_Sptr_i32;
Res(Sptr_i64, Err) Res_Sptr_i64;
Res(Sptr_isize, Err) Res_Sptr_isize;

Res(Sptr_f32, Err) Res_Sptr_f32;
Res(Sptr_f64, Err) Res_Sptr_f64;

Res(Sptr_bool, Err) Res_Sptr_bool;
Res(Sptr_char, Err) Res_Sptr_char;

Res(Sptr_anyptr, Err) Res_Sptr_anyptr;
Res(Sptr_PtrBase, Err) Res_Sptr_PtrBase;
Res(Sptr_Sptr, Err) Res_Sptr_Sptr;
Res(Sptr_Mptr, Err) Res_Sptr_Mptr;
Res(Sptr_Slice, Err) Res_Sptr_Slice;

// many-item pointer types
Res(Mptr_u8, Err) Res_Mptr_u8;
Res(Mptr_u16, Err) Res_Mptr_u16;
Res(Mptr_u32, Err) Res_Mptr_u32;
Res(Mptr_u64, Err) Res_Mptr_u64;
Res(Mptr_usize, Err) Res_Mptr_usize;

Res(Mptr_i8, Err) Res_Mptr_i8;
Res(Mptr_i16, Err) Res_Mptr_i16;
Res(Mptr_i32, Err) Res_Mptr_i32;
Res(Mptr_i64, Err) Res_Mptr_i64;
Res(Mptr_isize, Err) Res_Mptr_isize;

Res(Mptr_f32, Err) Res_Mptr_f32;
Res(Mptr_f64, Err) Res_Mptr_f64;

Res(Mptr_bool, Err) Res_Mptr_bool;
Res(Mptr_char, Err) Res_Mptr_char;

Res(Mptr_anyptr, Err) Res_Mptr_anyptr;
Res(Mptr_PtrBase, Err) Res_Mptr_PtrBase;
Res(Mptr_Sptr, Err) Res_Mptr_Sptr;
Res(Mptr_Mptr, Err) Res_Mptr_Mptr;
Res(Mptr_Slice, Err) Res_Mptr_Slice;

// slice types
Res(Slice_u8, Err) Res_Slice_u8;
Res(Slice_u16, Err) Res_Slice_u16;
Res(Slice_u32, Err) Res_Slice_u32;
Res(Slice_u64, Err) Res_Slice_u64;
Res(Slice_usize, Err) Res_Slice_usize;

Res(Slice_i8, Err) Res_Slice_i8;
Res(Slice_i16, Err) Res_Slice_i16;
Res(Slice_i32, Err) Res_Slice_i32;
Res(Slice_i64, Err) Res_Slice_i64;
Res(Slice_isize, Err) Res_Slice_isize;

Res(Slice_f32, Err) Res_Slice_f32;
Res(Slice_f64, Err) Res_Slice_f64;

Res(Slice_bool, Err) Res_Slice_bool;
Res(Slice_char, Err) Res_Slice_char;

Res(Slice_anyptr, Err) Res_Slice_anyptr;
Res(Slice_PtrBase, Err) Res_Slice_PtrBase;
Res(Slice_Sptr, Err) Res_Slice_Sptr;
Res(Slice_Mptr, Err) Res_Slice_Mptr;
Res(Slice_Slice, Err) Res_Slice_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_RES_INCLUDED */
