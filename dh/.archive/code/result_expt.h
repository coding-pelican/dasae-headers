#ifndef RESULT_INCLUDED
#define RESULT_INCLUDED (1)

#include "dh/debug/assert.h"
#include "dh/core.h"
#include "dh/core/ptr.h"
#include <string.h>

// Result variants
typedef enum ResultType {
    ResultType_ok,
    ResultType_err
} ResultType;

// Result structure using union
typedef struct Result {
    ResultType type;
    union {
        sptr ok;
        sptr err;
    } value;
} Result;

// Initialize an Ok result
force_inline Result Result_ok(sptr out_ptr, const sptr value) {
    // debug_assert_fmt(Ptr_isNonnull(out_ptr), "Output pointer cannot be null");
    // debug_assert_fmt(Ptr_size(value) > 0, "Size must be greater than 0");
    // debug_assert_fmt(Ptr_isNonnull(value), "Value cannot be null");

    mem_copySize(out_ptr, value, cptr_size(value.core));
    return (Result){
        .type  = ResultType_ok,
        .value = { .ok = out_ptr }
    };
}

// Initialize an Err result
force_inline Result Result_err(sptr out_ptr, const sptr error) {
    // debug_assert_fmt(Ptr_isNonnull(out_ptr), "Output pointer cannot be null");
    // debug_assert_fmt(Ptr_size(error) > 0, "Size must be greater than 0");
    // debug_assert_fmt(Ptr_isNonnull(error), "Error cannot be null");

    mem_copySize(out_ptr, error, cptr_size(error.core));
    return (Result){
        .type  = ResultType_err,
        .value = { .err = out_ptr }
    };
}

// Check if Result is Ok
force_inline bool Result_isOk(const Result r) {
    return r.type == ResultType_ok;
}

// Check if Result is Err
force_inline bool Result_isErr(const Result r) {
    return r.type == ResultType_err;
}

// Get Ok value (caller must ensure result is Ok)
force_inline sptr Result_unwrapOk(const Result r) {
    debug_assert_fmt(Result_isOk(r), "Cannot unwrap an Err value");
    return r.value.ok;
}

// Get Err value (caller must ensure result is Err)
force_inline sptr Result_unwrapErr(const Result r) {
    debug_assert_fmt(Result_isErr(r), "Cannot unwrap_err an Ok value");
    return r.value.err;
}

#endif /* RESULT_INCLUDED */
