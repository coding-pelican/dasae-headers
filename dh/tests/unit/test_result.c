#include "dh/result.h"

// Example usage
#include <stdio.h>

// Custom error type
typedef struct {
    i32  code;
    char message[64];
} Error;

// Function that returns a Result
Result checkedDivide(f64 a, f64 b, Ptr out) {
    debug_assert_fmt(Ptr_hasMinSize(out, sizeof(f64)), "Output size must match f64 size");

    if (b == 0.0) {
        Error err = {
            .code    = -1,
            .message = "division by zero"
        };
        return Result_err(out, Ptr_ref(err));
    }

    f64 result = a / b;
    return Result_ok(out, Ptr_ref(result));
}

// Example with array bounds checking
Result getArrayElement(i32* arr, usize arr_size, usize index, Ptr out) {
    debug_assert_fmt(Ptr_hasMinSize(out, sizeof(i32)), "Output size must match i32 size");
    debug_assert(arr != null && "Array cannot be null");

    if (index >= arr_size) {
        Error err = (Error){
            .code    = -2,
            .message = "index out of bounds"
        };
        return Result_err(out, Ptr_ref(err));
    }

    return Result_ok(out, Ptr_ref(arr[index]));
}

i32 main(void) {
    // Example 1: Division
    static union {
        f64   value;
        Error error;
    } result_space; // Static storage for result

    Result div_result = checkedDivide(10.0, 2.0, Ptr_ref(result_space));
    if (Result_isOk(div_result)) {
        printf("Division result: %f\n", Ptr_deref(f64, Result_unwrapOk(div_result)));
    }

    // Example 2: Division by zero
    Result div_err = checkedDivide(10.0, 0.0, Ptr_ref(result_space));
    if (Result_isErr(div_err)) {
        Error* error = Ptr_cast(Error*, Result_unwrapErr(div_err));
        printf("Error: [%d] %s\n", error->code, error->message);
    }

    // Example 3: Array bounds checking
    i32 arr[] = { 1, 2, 3, 4, 5 };
    static union {
        i32   value;
        Error error;
    } arr_result_space;

    Result arr_result = getArrayElement(arr, 5, 2, Ptr_ref(arr_result_space));
    if (Result_isOk(arr_result)) {
        printf("Array element: %d\n", Ptr_deref(i32, Result_unwrapOk(arr_result)));
    }

    // Try accessing out of bounds
    Result arr_err = getArrayElement(arr, 5, 10, Ptr_ref(arr_result_space));
    if (Result_isErr(arr_err)) {
        Error* error = Ptr_cast(Error*, Result_unwrapErr(arr_err));
        printf("Error: [%d] %s\n", error->code, error->message);
    }

    return 0;
}
