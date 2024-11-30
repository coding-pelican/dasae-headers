/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_option.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-11-30 (date of last update)
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of Option type
 */

#include "dh/Option.h"
#include "dh/debug/assert.h"

#include <stdio.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

static Option_i32 get_value(i32 x) {
    if (x < 0) {
        return Option_none(Option_i32);
    }
    return Option_some(Option_i32, x);
}

void TEST_Option_basics(void) {
    TEST_PrintSection("Option Basic Operations");

    // Test Some value
    Option_i32 some = Option_some(Option_i32, 42);
    TEST_PrintResult("Option Some creation", Option_hasValue(some));
    TEST_PrintResult("Option Some value", Option_unwrap(Option_i32, some) == 42);

    // Test None value
    Option_i32 none = Option_none(Option_i32);
    TEST_PrintResult("Option None creation", !Option_hasValue(none));
}

void TEST_Option_unwrap_or(void) {
    TEST_PrintSection("Option UnwrapOr Operations");

    const i32 items[] = { 1, 2, 3, 4, 5 };
    i32       index   = 2;

    // Test unwrap_or with side effects
    i32 value = Option_unwrapOr(Option_i32, get_value(-1), items[index++]);
    TEST_PrintResult("UnwrapOr side effect", index == 3);
    TEST_PrintResult("UnwrapOr default value", value == items[2]);

    // Test unwrap_or with successful case
    i32 success_value = Option_unwrapOr(Option_i32, get_value(5), 0);
    TEST_PrintResult("UnwrapOr success value", success_value == 5);
}

void TEST_Option_pointer_types(void) {
    TEST_PrintSection("Option Pointer Types");

    i32  value = 42;
    i32* ptr   = &value;

    // Test pointer Some
    Option_anyptr some_ptr = Option_some(Option_anyptr, ptr);
    TEST_PrintResult("Option pointer Some", Option_hasValue(some_ptr));
    TEST_PrintResult("Option pointer value", Option_unwrap(Option_anyptr, some_ptr) == ptr);

    // Test pointer None
    Option_anyptr none_ptr = Option_none(Option_anyptr);
    TEST_PrintResult("Option pointer None", !Option_hasValue(none_ptr));

    i32  default_val = 0;
    i32* unwrapped   = Option_unwrapOr(Option_anyptr, none_ptr, &default_val);
    TEST_PrintResult("Option pointer unwrap_or", unwrapped == &default_val);
}

// Example usage:
// Option_sptr result = mem_tryAllocT(alloc, int);
// if (result.has_value) {
//     int* ptr = result.value.raw;
//     // use ptr
// }

// // Memory allocation with Option
// force_inline Option_sptr mem_tryAlloc(mem_Allocator* alloc, usize size) {
//     sptr ptr = mem_alloc(alloc, size, __FILE__, __LINE__, __func__);
//     return ptr.raw ? Option_some(Option_sptr, ptr) : Option_none(sptr);
// }

// #define mem_tryAllocT(alloc, T) \
//     mem_tryAlloc(alloc, sizeof(T))

int main(void) {
    printf("Starting Option Type Tests\n");

    TEST_Option_basics();
    TEST_Option_unwrap_or();
    TEST_Option_pointer_types();

    printf("\nAll tests completed.\n");
    return 0;
}
