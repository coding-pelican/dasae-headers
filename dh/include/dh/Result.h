/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Result.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-11-30 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  Result
 *
 * @brief   Header for Result type handling
 * @details Generic Result type for error handling
 */

#ifndef RESULT_INCLUDED
#define RESULT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/prim.h"
#include "core/ptr.h"
#include "debug/assert.h"
#include "Error.h"

/*========== Macros and Definitions =========================================*/

// Result tag enum
typedef enum ResultTag {
    ResultTag_err = 0,
    ResultTag_ok
} ResultTag;

// Base Result structure for internal use
typedef struct ResultBase {
    ResultTag tag;
    union {
        anyptr value;
        anyptr error;
    } data;
} ResultBase;

force_inline bool ResultBase_isOk(ResultBase* self) {
    return self->tag == ResultTag_ok;
}

force_inline bool ResultBase_isErr(ResultBase* self) {
    return self->tag == ResultTag_err;
}

force_inline ResultBase* ResultBase_unwrap(ResultBase* self) {
    return debug_assert_true(ResultBase_isOk(self)), self;
}

force_inline ResultBase* ResultBase_unwrapErr(ResultBase* self) {
    return debug_assert_true(ResultBase_isErr(self)), self;
}

force_inline ResultBase* ResultBase_unwrapOr(ResultBase* self, ResultBase* default_val) {
    return ResultBase_isOk(self) ? self : default_val;
}

// Result definition for concrete types
#define Result(T, E)        \
    typedef union {         \
        ResultBase Base[1]; \
        struct {            \
            ResultTag tag;  \
            union {         \
                T value;    \
                E error;    \
            } data;         \
        };                  \
    }

// Result creation
#define Result_ok(TResult, val)  ((TResult){ .tag = ResultTag_ok, .data.value = (val) })
#define Result_err(TResult, err) ((TResult){ .tag = ResultTag_err, .data.error = (err) })

// Result operations
#define Result_isOk(self)  ResultBase_isOk((ResultBase*)&(self))
#define Result_isErr(self) ResultBase_isErr((ResultBase*)&(self))

#define Result_unwrap(TResult, self) \
    (((TResult*)ResultBase_unwrap((ResultBase*)(TResult[1]){ self }))->data.value) // NOLINT

#define Result_unwrapErr(TResult, self) \
    (((TResult*)ResultBase_unwrapErr((ResultBase*)(TResult[1]){ self }))->data.error) // NOLINT

#define Result_unwrapOr(TResult, self, default_val) \
    (((TResult*)ResultBase_unwrapOr((ResultBase*)(TResult[1]){ self }, (ResultBase*)(TResult[1]){ Result_ok(TResult, default_val) }))->data.value) // NOLINT

// Result built-in types
Result(u8, Error) Result_u8;
Result(u16, Error) Result_u16;
Result(u32, Error) Result_u32;
Result(u64, Error) Result_u64;
Result(uptr, Error) Result_uptr;
Result(usize, Error) Result_usize;

Result(i8, Error) Result_i8;
Result(i16, Error) Result_i16;
Result(i32, Error) Result_i32;
Result(i64, Error) Result_i64;
Result(iptr, Error) Result_iptr;
Result(isize, Error) Result_isize;

Result(f32, Error) Result_f32;
Result(f64, Error) Result_f64;

Result(bool, Error) Result_bool;
Result(char, Error) Result_char;

Result(anyptr, Error) Result_anyptr;
Result(cptr, Error) Result_cptr;
Result(sptr, Error) Result_sptr;
Result(mptr, Error) Result_mptr;
Result(Slice, Error) Result_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RESULT_INCLUDED */
