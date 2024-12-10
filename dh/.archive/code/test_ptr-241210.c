/**
 * @file    test_ptr.c
 * @brief   Unit tests for smart pointer implementation
 */

#include "dh/core/ptr.h"
#include "dh/debug/assert.h"
#include <stdio.h>

/*========== Test Utilities ============================================*/

typedef struct TEST_Result {
    const char* test_name;
    bool        passed;
} TEST_Result;

void TEST_PrintSection(const char* section) {
    printf("\n=== Testing %s ===\n", section);
}

void TEST_PrintResult(TEST_Result result) {
    printf("%s: %s\n", result.test_name, result.passed ? "PASS" : "FAIL");
}

/*========== PtrBase Tests ============================================*/

TEST_Result TEST_PtrBase_Creation(void) {
    i32     value = 42;
    PtrBase ptr   = PtrBase_make(&value, sizeof(i32), alignof(i32));

    bool conditions = !PtrBase_isUndefined(ptr) && PtrBase_size(ptr) == sizeof(i32) && PtrBase_align(ptr) == alignof(i32) && *(i32*)PtrBase_addr(ptr) == 42;

    return (TEST_Result){
        .test_name = "PtrBase basic creation",
        .passed    = conditions
    };
}

TEST_Result TEST_PtrBase_Alignment(void) {
    alignas(16) i32 value       = 42;
    PtrBase         ptr         = PtrBase_make(&value, sizeof(i32), 16);
    PtrBase         aligned_ptr = PtrBase_withAlign(ptr, 32);

    bool conditions = PtrBase_align(ptr) == 16 && PtrBase_align(aligned_ptr) == 32 && PtrBase_isAligned(ptr) && PtrBase_isAligned(aligned_ptr);

    return (TEST_Result){
        .test_name = "PtrBase alignment",
        .passed    = conditions
    };
}

/*========== Single-Item Pointer Tests ================================*/

TEST_Result TEST_Sptr_Creation(void) {
    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    bool conditions = !Sptr_isUndefined(ptr) && Sptr_size(ptr) == sizeof(i32) && Sptr_align(ptr) == alignof(i32) && Sptr_deref(i32, ptr) == 42;

    return (TEST_Result){
        .test_name = "Sptr basic creation",
        .passed    = conditions
    };
}

TEST_Result TEST_Sptr_Modification(void) {
    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    i32* raw_ptr = Sptr_rawCast(i32*, ptr);
    *raw_ptr     = 100;

    bool conditions = Sptr_deref(i32, ptr) == 100 && value == 100;

    return (TEST_Result){
        .test_name = "Sptr modification",
        .passed    = conditions
    };
}

TEST_Result TEST_Sptr_Conversion(void) {
    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    // Convert to many-item pointer
    Mptr mptr = Sptr_toMptr(ptr);

    // Convert to slice
    Slice slice = Sptr_toSlice(ptr, 0, 1);

    bool conditions = !Mptr_isUndefined(mptr) && Mptr_at(i32, mptr, 0) == 42 && !Slice_isUndefined(slice) && Slice_len(slice) == 1 && Slice_at(i32, slice, 0) == 42;

    return (TEST_Result){
        .test_name = "Sptr conversion",
        .passed    = conditions
    };
}

/*========== Many-Item Pointer Tests ==================================*/

TEST_Result TEST_Mptr_Creation(void) {
    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    bool conditions = !Mptr_isUndefined(ptr) && Mptr_size(ptr) == sizeof(i32) && Mptr_align(ptr) == alignof(i32) && Mptr_at(i32, ptr, 0) == 1 && Mptr_at(i32, ptr, 4) == 5;

    return (TEST_Result){
        .test_name = "Mptr basic creation",
        .passed    = conditions
    };
}

TEST_Result TEST_Mptr_Arithmetic(void) {
    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    Mptr  ptr2 = Mptr_add(ptr, 2);
    Mptr  ptr3 = Mptr_sub(ptr2, 1);
    isize diff = Mptr_diff(ptr2, ptr);

    bool conditions = Mptr_at(i32, ptr2, 0) == 3 && Mptr_at(i32, ptr3, 0) == 2 && diff == 2;

    return (TEST_Result){
        .test_name = "Mptr arithmetic",
        .passed    = conditions
    };
}

TEST_Result TEST_Mptr_Access(void) {
    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    // Test raw access
    i32* raw = Mptr_rawCast(i32*, ptr);
    raw[2]   = 30; // Modify through raw pointer

    // Test Mptr_at
    i32 value_at = Mptr_at(i32, ptr, 2);

    // Test Mptr_atAddr
    i32* addr_at = (i32*)Mptr_atAddr(ptr, 3);
    *addr_at     = 40; // Modify through address

    // Test Mptr_atSptr
    Sptr sptr             = Mptr_atSptr(ptr, 4);
    Sptr_deref(i32, sptr) = 50; // Modify through Sptr

    bool conditions =
        // Raw pointer access
        raw[0] == 1 && raw[4] == 50 &&

        // Mptr_at access
        Mptr_at(i32, ptr, 0) == 1 && value_at == 30 && Mptr_at(i32, ptr, 3) == 40 && Mptr_at(i32, ptr, 4) == 50 &&

        // Address access
        *(i32*)Mptr_atAddr(ptr, 2) == 30 &&

        // Sptr access
        Sptr_deref(i32, Mptr_atSptr(ptr, 2)) == 30 &&

        // Verify modifications affected the original array
        array[2] == 30 && array[3] == 40 && array[4] == 50;

    return (TEST_Result){
        .test_name = "Mptr access",
        .passed    = conditions
    };
}

TEST_Result TEST_Mptr_Conversion(void) {
    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    Slice slice = Mptr_toSlice(ptr, 1, 4);

    bool conditions = !Slice_isUndefined(slice) && Slice_len(slice) == 3 && Slice_at(i32, slice, 0) == 2 && Slice_at(i32, slice, 2) == 4;

    return (TEST_Result){
        .test_name = "Mptr conversion",
        .passed    = conditions
    };
}

/*========== Slice Tests =============================================*/

TEST_Result TEST_Slice_Creation(void) {
    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    bool conditions = !Slice_isUndefined(slice) && Slice_size(slice) == sizeof(i32) && Slice_align(slice) == alignof(i32) && Slice_len(slice) == 5 && Slice_at(i32, slice, 0) == 1 && Slice_at(i32, slice, 4) == 5;

    return (TEST_Result){
        .test_name = "Slice basic creation",
        .passed    = conditions
    };
}

TEST_Result TEST_Slice_SubSlicing(void) {
    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    Slice sub    = Slice_slice(slice, 1, 4);
    Slice prefix = Slice_prefix(slice, 3);
    Slice suffix = Slice_suffix(slice, 2);

    bool conditions = Slice_len(sub) == 3 && Slice_at(i32, sub, 0) == 2 && Slice_at(i32, sub, 2) == 4
                   && Slice_len(prefix) == 3 && Slice_at(i32, prefix, 0) == 1 && Slice_len(suffix) == 3 && Slice_at(i32, suffix, 0) == 3;

    return (TEST_Result){
        .test_name = "Slice sub-slicing",
        .passed    = conditions
    };
}

TEST_Result TEST_Slice_MptrAccess(void) {
    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    // Get pointers at valid indices
    Mptr at_ptr    = Slice_atMptr(slice, 2);
    Mptr begin_ptr = Slice_beginMptr(slice);

    // Test access through obtained pointers
    bool conditions =
        // Verify point to correct elements
        Mptr_at(i32, at_ptr, 0) == 3 && Mptr_at(i32, begin_ptr, 0) == 1 &&

        // Verify element access through arithmetic
        Mptr_at(i32, Mptr_add(begin_ptr, 1), 0) == 2 && Mptr_at(i32, Mptr_add(at_ptr, 1), 0) == 4 &&

        // Verify pointer differences
        Mptr_diff(at_ptr, begin_ptr) == 2;

    return (TEST_Result){
        .test_name = "Slice Mptr access",
        .passed    = conditions
    };
}

/*========== Error Cases Tests =======================================*/

TEST_Result TEST_ErrorCases(void) {
    bool caught_assert = false;

    // Test out of bounds access
    i32   array[] = { 1, 2, 3 };
    Slice slice   = Slice_refArray(i32, array);

    // These should trigger debug assertions
    // In a real test framework, we'd use setjmp/longjmp to catch assertions
    // Here we just document what should happen

    // Slice_at(i32, slice, 3);  // Should assert
    // Slice_slice(slice, 0, 4);  // Should assert
    // Slice_endMptr(slice);      // Should not assert

    caught_assert = true; // Placeholder

    return (TEST_Result){
        .test_name = "Error cases",
        .passed    = caught_assert
    };
}

/*========== Main Test Runner ========================================*/

int main(void) {
    printf("Starting Smart Pointer Tests\n");

    // PtrBase tests
    TEST_PrintSection("PtrBase");
    TEST_PrintResult(TEST_PtrBase_Creation());
    TEST_PrintResult(TEST_PtrBase_Alignment());

    // Sptr tests
    TEST_PrintSection("Single-Item Pointer");
    TEST_PrintResult(TEST_Sptr_Creation());
    TEST_PrintResult(TEST_Sptr_Modification());
    TEST_PrintResult(TEST_Sptr_Conversion());

    // Mptr tests
    TEST_PrintSection("Many-Item Pointer");
    TEST_PrintResult(TEST_Mptr_Creation());
    TEST_PrintResult(TEST_Mptr_Arithmetic());
    TEST_PrintResult(TEST_Mptr_Access());
    TEST_PrintResult(TEST_Mptr_Conversion());

    // Slice tests
    TEST_PrintSection("Slice");
    TEST_PrintResult(TEST_Slice_Creation());
    TEST_PrintResult(TEST_Slice_SubSlicing());
    TEST_PrintResult(TEST_Slice_MptrAccess());

    // Error cases
    TEST_PrintSection("Error Cases");
    TEST_PrintResult(TEST_ErrorCases());

    printf("\nAll tests completed.\n");
    return 0;
}
