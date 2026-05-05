#if UNUSED_CODE
#include "dh/except.h"

#include <string.h>

// Example function that may throw an exception
void divide(int numerator, int denominator) {
    if (denominator == 0) {
        throw(ExceptionType_divide_by_zero, "Division by zero attempted");
    }
    printf("Result: %d\n", numerator / denominator);
}

// Example of custom array bounds checking
void access_array(int* arr, int size, int index) {
    if (index < 0 || index >= size) {
        throw(ExceptionType_index_out_of_bounds, "Array index out of bounds");
    }
    printf("Value at index %d: %d\n", index, arr[index]);
}

int main(void) {
    // Example 1: Division by zero
    try {
        printf("Attempting division...\n");
        divide(10, 0);
    } catch (ex) {
        printf("Caught exception: %s at %s:%d\n", ex.message, ex.file, ex.line);
    }
    tried;

    // Example 2: Array bounds checking
    int arr[] = { 1, 2, 3, 4, 5 };
    try {
        access_array(arr, 5, 10); // This will throw an exception
    } catch (ex) {
        printf("Caught exception: %s at %s:%d\n", ex.message, ex.file, ex.line);
    }
    tried;

    // Example 3: Nested try-catch blocks
    try {
        printf("Outer try block\n");
        try {
            printf("Inner try block\n");
            throw(1, "Custom exception thrown");
        } catch (inner_ex) {
            printf("Inner catch: %s\n", inner_ex.message);
            throw(ExceptionType_invalid_argument, "Rethrowing as different exception");
        }
        tried;
    } catch (outer_ex) {
        printf("Outer catch: %s\n", outer_ex.message);
    }
    tried;

    return 0;
}
#endif /* UNUSED_CODE */
