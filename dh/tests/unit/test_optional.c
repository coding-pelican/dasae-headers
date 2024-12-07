/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_optional.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-12-01 (date of last update)
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of Optional type
 */

#include "dh/core/Opt.h"
#include "dh/debug/assert.h"

#include <stdio.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

static Opt_i32 get_value(i32 x) {
    if (x < 0) {
        return Opt_none(Opt_i32);
    }
    return Opt_some(Opt_i32, x);
}

void TEST_Opt_basics(void) {
    TEST_PrintSection("Optional Basic Operations");

    // Test Some value
    Opt_i32 some = Opt_some(Opt_i32, 42);
    TEST_PrintResult("Optional Some creation", Opt_isSome(some));
    TEST_PrintResult("Optional Some value", Opt_unwrap(Opt_i32, some) == 42);

    // Test None value
    Opt_i32 none = Opt_none(Opt_i32);
    TEST_PrintResult("Optional None creation", !Opt_isSome(none));
}

void TEST_Opt_unwrap_or(void) {
    TEST_PrintSection("Optional UnwrapOr Operations");

    const i32 items[] = { 1, 2, 3, 4, 5 };
    i32       index   = 2;

    // Test unwrap_or with side effects
    i32 value = Opt_unwrapOr(Opt_i32, get_value(-1), items[index++]);
    TEST_PrintResult("UnwrapOr side effect", index == 3);
    TEST_PrintResult("UnwrapOr default value", value == items[2]);

    // Test unwrap_or with successful case
    i32 success_value = Opt_unwrapOr(Opt_i32, get_value(5), 0);
    TEST_PrintResult("UnwrapOr success value", success_value == 5);
}

void TEST_Opt_pointer_types(void) {
    TEST_PrintSection("Optional Pointer Types");

    i32  value = 42;
    i32* ptr   = &value;

    // Test pointer Some
    Opt_anyptr some_ptr = Opt_some(Opt_anyptr, ptr);
    TEST_PrintResult("Optional pointer Some", Opt_isSome(some_ptr));
    TEST_PrintResult("Optional pointer value", Opt_unwrap(Opt_anyptr, some_ptr) == ptr);

    // Test pointer None
    Opt_anyptr none_ptr = Opt_none(Opt_anyptr);
    TEST_PrintResult("Optional pointer None", !Opt_isSome(none_ptr));

    i32  default_val = 0;
    i32* unwrapped   = Opt_unwrapOr(Opt_anyptr, none_ptr, &default_val);
    TEST_PrintResult("Optional pointer unwrap_or", unwrapped == &default_val);
}

// Example usage:
// Opt_Sptr result = mem_tryAllocT(alloc, int);
// if (result.has_value) {
//     int* ptr = result.value.raw;
//     // use ptr
// }

// // Memory allocation with Optional
// force_inline Opt_Sptr mem_tryAlloc(mem_Allocator* alloc, usize size) {
//     Sptr ptr = mem_alloc(alloc, size, __FILE__, __LINE__, __func__);
//     return ptr.raw ? Opt_some(Opt_Sptr, ptr) : Opt_none(Sptr);
// }

// #define mem_tryAllocT(alloc, T) \
//     mem_tryAlloc(alloc, sizeof(T))

int main(void) {
    printf("Starting Optional Type Tests\n");

    TEST_Opt_basics();
    TEST_Opt_unwrap_or();
    TEST_Opt_pointer_types();

    printf("\nAll tests completed.\n");
    return 0;
}
