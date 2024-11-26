// build `clang -o test_mem -xc -std=c17 test_mem.c ../../src/*.c -static -g -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_mem.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of mem module (for Memory Management)
 */

#include "dh/mem.h"
#include "dh/debug/assert.h"

#include <stdio.h>
#include <string.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

// Test Ptr operations
void TEST_Ptr_basics() {
    TEST_PrintSection("Ptr Basic Operations");

    // Test creation
    i32 value = 42;
    Ptr ptr   = Ptr_ref(value);
    TEST_PrintResult("Ptr creation", !Ptr_isNull(ptr));
    TEST_PrintResult("Ptr element size", Ptr_size(ptr) == sizeof(i32));

    // Test null ptr
    TEST_PrintResult("Null ptr check", Ptr_isNull(Ptr_null));

    // Test casting
    i32* int_ptr = Ptr_cast(i32*, ptr);
    TEST_PrintResult("Ptr casting", int_ptr && *int_ptr == 42);

    // // Test invalid cast
    // char* char_ptr = Ptr_cast(char*, ptr);
    // TEST_PrintResult("Invalid cast", char_ptr == null);
}

// Test Slice operations
void TEST_Slice_basics() {
    TEST_PrintSection("Slice Basic Operations");

    const mem_Allocator* alloc = &mem_general;

    // Create a slice of integers
    Slice slice = mem_alloc(alloc, i32, 5);
    TEST_PrintResult("Slice allocation", !Slice_isNull(slice));
    TEST_PrintResult("Slice length", Slice_len(slice) == 5);
    TEST_PrintResult("Slice element size", Slice_elemSize(slice) == sizeof(i32));

    // Fill slice with values
    i32* data = Slice_cast(i32*, slice);
    for (usize i = 0; i < Slice_len(slice); ++i) {
        data[i] = (i32)i;
    }

    // Test element access
    for (usize i = 0; i < Slice_len(slice); ++i) {
        i32* elem = Slice_atOrNull(i32, slice, i);
        TEST_PrintResult("Slice element access", elem && *elem == (i32)i);
    }

    // Test subslicing
    Slice sub = Slice_subslice(slice, 1, 3);
    TEST_PrintResult("Subslice length", Slice_len(sub) == 2);
    TEST_PrintResult("Subslice element", *Slice_at(i32, sub, 0) == 1);

    mem_free(alloc, &slice);
}

void TEST_Memory_operations() {
    TEST_PrintSection("Memory Operations");

    const mem_Allocator* alloc = &mem_general;

    // Test allocation and initialization
    Slice nums = mem_alloc(alloc, i32, 3);
    i32*  data = Slice_cast(i32*, nums);
    data[0]    = 1;
    data[1]    = 2;
    data[2]    = 3;

    // Test reallocation
    nums = mem_realloc(alloc, nums, i32, 5);
    TEST_PrintResult("Realloc preserves data", Slice_len(nums) == 5 && *Slice_at(i32, nums, 0) == 1 && *Slice_at(i32, nums, 1) == 2 && *Slice_at(i32, nums, 2) == 3);

    mem_free(alloc, &nums);
}

void TEST_Memory_copy() {
    TEST_PrintSection("Memory Copy Operations");

    const mem_Allocator* alloc = &mem_general;

    // Create source data
    Slice src      = mem_alloc(alloc, i32, 3);
    i32*  src_data = Slice_cast(i32*, src);
    src_data[0]    = 1;
    src_data[1]    = 2;
    src_data[2]    = 3;

    // Create destination
    Slice dest = mem_alloc(alloc, i32, 3);

    // Copy memory
    mem_copySlice(dest, src);

    // Verify copy
    bool copy_success = true;
    for (usize i = 0; i < Slice_len(src); ++i) {
        i32* src_elem  = Slice_atOrNull(i32, src, i);
        i32* dest_elem = Slice_atOrNull(i32, dest, i);
        if (!src_elem || !dest_elem || *src_elem != *dest_elem) {
            copy_success = false;
            break;
        }
    }
    TEST_PrintResult("Memory copy", copy_success);

    mem_free(alloc, &src);
    mem_free(alloc, &dest);
}

void TEST_Slice_iteration() {
    TEST_PrintSection("Slice Iteration");

    const mem_Allocator* alloc = &mem_general;

    // Create test data
    Slice nums = mem_alloc(alloc, i32, 5);
    i32*  data = Slice_cast(i32*, nums);
    for (usize i = 0; i < Slice_len(nums); ++i) {
        data[i] = (i32)i;
    }

    // Test iteration
    i32 sum = 0;
    Slice_iter(i32, value, nums) {
        sum += *value;
    }
    TEST_PrintResult("Slice iteration", sum == 10); // 0+1+2+3+4 = 10

    mem_free(alloc, &nums);
}

typedef struct Point {
    i32 x;
    i32 y;
} Point;

void TEST_Custom_types() {
    TEST_PrintSection("Custom Types");

    const mem_Allocator* alloc = &mem_general;

    // Test with struct
    Slice  points = mem_alloc(alloc, Point, 3);
    Point* data   = Slice_cast(Point*, points);

    data[0] = literal(Point, 1, 1);
    data[1] = literal(Point, 2, 2);
    data[2] = literal(Point, 3, 3);

    bool struct_test = true;
    Slice_iter(Point, p, points) {
        if (p->x != p->y) {
            struct_test = false;
            break;
        }
    }
    TEST_PrintResult("Struct handling", struct_test);

    mem_free(alloc, &points);
}

void TEST_Error_handling() {
    TEST_PrintSection("Error Handling");

    // Test null checks
    TEST_PrintResult("Null Ptr check", Ptr_isNull(Ptr_null));
    TEST_PrintResult("Null Slice check", Slice_isNull(Slice_null));

    // Test invalid operations
    Slice invalid = Slice_subsliceOrNull(Slice_null, 0, 1);
    TEST_PrintResult("Invalid subslice", Slice_isNull(invalid));

    // Test out of bounds access
    Slice nums           = mem_alloc(&mem_general, i32, 3);
    i32*  invalid_access = Slice_atOrNull(i32, nums, 5);
    TEST_PrintResult("Out of bounds access", invalid_access == null);

    mem_free(&mem_general, &nums);
}

// Test structures for conversion tests
typedef struct Base {
    i32 x;
} Base;

typedef struct Derived {
    Base base;
    i32  y;
} Derived;

typedef struct OtherType {
    i32 value;
} OtherType;

// Ptr type conversion tests
void TEST_Ptr_conversion() {
    TEST_PrintSection("Ptr Type Conversions");

    // Test exact size conversions
    Base base = { .x = 42 };
    Ptr  ptr  = Ptr_ref(base);

    // Same size, same type
    Base* base_ptr = Ptr_cast(Base*, ptr);
    TEST_PrintResult("Same type cast", base_ptr && base_ptr->x == 42);

    // Same size, different type
    OtherType* other = Ptr_cast(OtherType*, ptr);
    TEST_PrintResult("Same size different type", other != null);

    // Different size (should fail)
    // Derived* derived = Ptr_cast(Derived*, ptr);
    // TEST_PrintResult("Different size cast fails", derived == null);

    // Test minimum size conversions
    Derived large     = { .base = { .x = 42 }, .y = 123 };
    Ptr     large_ptr = Ptr_ref(large);

    // Convert to smaller type
    Base* as_base = Ptr_cast(Base*, large_ptr);
    TEST_PrintResult("Convert to smaller type", as_base && as_base->x == 42);

    // Convert to same size
    Derived* as_derived = Ptr_cast(Derived*, large_ptr);
    TEST_PrintResult("Convert to same size", as_derived && as_derived->base.x == 42 && as_derived->y == 123);
}

// Slice conversion tests
void TEST_Slice_conversion() {
    TEST_PrintSection("Slice Type Conversions");

    const mem_Allocator* alloc = &mem_general;

    // Create array of Derived
    Slice    slice = mem_alloc(alloc, Derived, 3);
    Derived* data  = Slice_cast(Derived*, slice);
    if (data) {
        for (usize i = 0; i < 3; ++i) {
            data[i].base.x = (i32)i;
            data[i].y      = (i32)i * 10;
        }
    }

    // Test exact size cast
    Derived* as_derived = Slice_cast(Derived*, slice);
    TEST_PrintResult("Exact size cast", as_derived && as_derived[0].base.x == 0 && as_derived[0].y == 0);

    // Test conversion to base type
    Base* as_base = Slice_cast(Base*, slice);
    TEST_PrintResult("Convert to base type", as_base && as_base[0].x == 0);

    // Test individual element access
    Base* elem0 = Slice_atOrNull(Base, slice, 0);
    TEST_PrintResult("Element access as base", elem0 && elem0->x == 0);

    // Test out of bounds access
    Base* elem_out = Slice_atOrNull(Base, slice, 3);
    TEST_PrintResult("Out of bounds access", elem_out == null);

    mem_free(alloc, &slice);
}

int main(void) {
    printf("Starting Memory Management Tests\n");

    TEST_Ptr_basics();
    TEST_Slice_basics();
    TEST_Memory_operations();
    TEST_Memory_copy();
    TEST_Slice_iteration();
    TEST_Custom_types();
    TEST_Error_handling();
    TEST_Ptr_conversion();
    TEST_Slice_conversion();

    printf("\nAll tests completed.\n");
    return 0;
}
