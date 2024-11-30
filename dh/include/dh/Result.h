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

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

// Result tag enum
typedef enum ResultTag {
    ResultTag_Ok,
    ResultTag_Err
} ResultTag;

// Common error type
typedef enum ResultErr {
    ResultErr_none = 0,
    ResultErr_out_of_memory,
    ResultErr_invalid_argument,
    ResultErr_not_found,
    ResultErr_access_denied,
    // Add more standard error types as needed
} ResultErr;

// Base Result structure for internal use
typedef struct ResultBase {
    ResultTag tag;
    union {
        anyptr value;
        anyptr error;
    } data;
} ResultBase;

force_inline bool ResultBase_isOk(ResultBase* self) {
    return self->tag == ResultTag_Ok;
}

force_inline bool ResultBase_isErr(ResultBase* self) {
    return self->tag == ResultTag_Err;
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
#define Result_ok(TResult, val)  ((TResult){ .tag = ResultTag_Ok, .data.value = (val) })
#define Result_err(TResult, err) ((TResult){ .tag = ResultTag_Err, .data.error = (err) })

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
Result(u8, ResultErr) Result_u8_Err;
Result(u16, ResultErr) Result_u16_Err;
Result(u32, ResultErr) Result_u32_Err;
Result(u64, ResultErr) Result_u64_Err;
Result(uptr, ResultErr) Result_uptr_Err;
Result(usize, ResultErr) Result_usize_Err;

Result(i8, ResultErr) Result_i8_Err;
Result(i16, ResultErr) Result_i16_Err;
Result(i32, ResultErr) Result_i32_Err;
Result(i64, ResultErr) Result_i64_Err;
Result(iptr, ResultErr) Result_iptr_Err;
Result(isize, ResultErr) Result_isize_Err;

Result(f32, ResultErr) Result_f32_Err;
Result(f64, ResultErr) Result_f64_Err;

Result(bool, ResultErr) Result_bool_Err;
Result(char, ResultErr) Result_char_Err;

Result(anyptr, ResultErr) Result_anyptr_Err;
Result(cptr, ResultErr) Result_cptr_Err;
Result(sptr, ResultErr) Result_sptr_Err;
Result(mptr, ResultErr) Result_mptr_Err;
Result(Slice, ResultErr) Result_Slice_Err;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RESULT_INCLUDED */
