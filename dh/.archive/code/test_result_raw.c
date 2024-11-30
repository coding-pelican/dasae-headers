#ifndef RESULT_H
#define RESULT_H

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// Result variants
typedef enum {
    ResultType_ok,
    ResultType_err
} ResultType;

// Result structure using union
typedef struct {
    ResultType type;
    union {
        void* ok;
        void* err;
    } value;
    size_t size; // Size of the stored value
} Result;

// Initialize an Ok result
static inline Result Result_ok(void* out_ptr, size_t size, const void* value) {
    assert(out_ptr != NULL && "Output pointer cannot be NULL");
    assert(size > 0 && "Size must be greater than 0");
    assert(value != NULL && "Value cannot be NULL");

    memcpy(out_ptr, value, size);
    return (Result){
        .type  = ResultType_ok,
        .value = { .ok = out_ptr },
        .size  = size
    };
}

// Initialize an Err result
static inline Result Result_err(void* out_ptr, size_t size, const void* error) {
    assert(out_ptr != NULL && "Output pointer cannot be NULL");
    assert(size > 0 && "Size must be greater than 0");
    assert(error != NULL && "Error cannot be NULL");

    memcpy(out_ptr, error, size);
    return (Result){
        .type  = ResultType_err,
        .value = { .err = out_ptr },
        .size  = size
    };
}

// Check if Result is Ok
static inline bool Result_is_ok(const Result* r) {
    assert(r != NULL && "Result pointer cannot be NULL");
    return r->type == ResultType_ok;
}

// Check if Result is Err
static inline bool Result_is_err(const Result* r) {
    assert(r != NULL && "Result pointer cannot be NULL");
    return r->type == ResultType_err;
}

// Get Ok value (caller must ensure result is Ok)
static inline void* Result_unwrap(const Result* r) {
    assert(r != NULL && "Result pointer cannot be NULL");
    assert(Result_is_ok(r) && "Cannot unwrap an Err value");
    return r->value.ok;
}

// Get Err value (caller must ensure result is Err)
static inline void* Result_unwrap_err(const Result* r) {
    assert(r != NULL && "Result pointer cannot be NULL");
    assert(Result_is_err(r) && "Cannot unwrap_err an Ok value");
    return r->value.err;
}

#endif

// Example usage
#include <stdio.h>

// Custom error type
typedef struct {
    int  code;
    char message[64];
} Error;

// Function that returns a Result
Result checkedDivide(double a, double b, void* out, size_t size) {
    assert(size == sizeof(double) && "Output size must match double size");

    if (b == 0.0) {
        Error err = {
            .code    = -1,
            .message = "division by zero"
        };
        return Result_err(out, sizeof(Error), &err);
    }

    double result = a / b;
    return Result_ok(out, sizeof(double), &result);
}

// Example with array bounds checking
Result getArrayElement(const int* arr, size_t arr_size, size_t index, void* out, size_t size) {
    assert(size == sizeof(int) && "Output size must match int size");
    assert(arr != NULL && "Array cannot be NULL");

    if (index >= arr_size) {
        Error err = {
            .code    = -2,
            .message = "index out of bounds"
        };
        return Result_err(out, sizeof(Error), &err);
    }

    return Result_ok(out, sizeof(int), &arr[index]);
}

int main() {
    // Example 1: Division
    union {
        double value;
        Error  error;
    } result_space; // Static storage for result

    Result div_result = checkedDivide(10.0, 2.0, &result_space, sizeof(double));

    if (Result_is_ok(&div_result)) {
        double* value = Result_unwrap(&div_result);
        printf("Division result: %f\n", *value);
    }

    // Example 2: Division by zero
    Result div_err = checkedDivide(10.0, 0.0, &result_space, sizeof(double));

    if (Result_is_err(&div_err)) {
        Error* error = Result_unwrap_err(&div_err);
        printf("Error: [%d] %s\n", error->code, error->message);
    }

    // Example 3: Array bounds checking
    int arr[] = { 1, 2, 3, 4, 5 };
    union {
        int   value;
        Error error;
    } arr_result_space;

    Result arr_result = getArrayElement(arr, 5, 2, &arr_result_space, sizeof(int));
    if (Result_is_ok(&arr_result)) {
        int* value = Result_unwrap(&arr_result);
        printf("Array element: %d\n", *value);
    }

    // Try accessing out of bounds
    Result arr_err = getArrayElement(arr, 5, 10, &arr_result_space, sizeof(int));
    if (Result_is_err(&arr_err)) {
        Error* error = Result_unwrap_err(&arr_err);
        printf("Error: [%d] %s\n", error->code, error->message);
    }

    return 0;
}
