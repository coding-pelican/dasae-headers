/**
 * @file    test_ptr.c
 * @brief   Unit tests for smart pointer implementation
 */

#include "dh/core/ptr.h"
#include "dh/debug/assert.h"

/* Test framework import */
#include "dh/TEST.h" // Assuming we move the framework to a header

/*========== PtrBase Tests ============================================*/

TEST_Result TEST_PtrBase_Creation(void) {
    TEST_Result result = TEST_makeResult("PtrBase basic creation");

    i32     value = 42;
    PtrBase ptr   = PtrBase_make(&value, sizeof(i32), alignof(i32));

    TEST_condition(!PtrBase_isUndefined(ptr));
    TEST_condition(PtrBase_size(ptr) == sizeof(i32));
    TEST_condition(PtrBase_align(ptr) == alignof(i32));
    TEST_condition(*(i32*)PtrBase_addr(ptr) == 42);

    return TEST_completeResult(&result);
}

TEST_Result TEST_PtrBase_Alignment(void) {
    TEST_Result result = TEST_makeResult("PtrBase alignment");

    alignas(32) i32 value       = 42;                                    // Ensure base value has 32-byte alignment
    PtrBase         ptr         = PtrBase_make(&value, sizeof(i32), 16); // Create with 16-byte alignment
    PtrBase         aligned_ptr = PtrBase_withAlign(ptr, 32);            // Increase to 32-byte alignment

    // Test base pointer with 16-byte alignment
    TEST_condition(PtrBase_align(ptr) == 16);
    TEST_condition(PtrBase_isAligned(ptr));

    // Test increased alignment to 32 bytes
    TEST_condition(PtrBase_align(aligned_ptr) == 32);
    TEST_condition(PtrBase_isAligned(aligned_ptr));

    // Verify address hasn't changed
    TEST_condition(aligned_ptr.addr == ptr.addr);

    return TEST_completeResult(&result);
}

/*========== Single-Item Pointer Tests ================================*/

TEST_Result TEST_Sptr_Creation(void) {
    TEST_Result result = TEST_makeResult("Sptr basic creation");

    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    TEST_condition(!Sptr_isUndefined(ptr));
    TEST_condition(Sptr_size(ptr) == sizeof(i32));
    TEST_condition(Sptr_align(ptr) == alignof(i32));
    TEST_condition(Sptr_deref(i32, ptr) == 42);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Sptr_Modification(void) {
    TEST_Result result = TEST_makeResult("Sptr modification");

    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    i32* raw_ptr = Sptr_rawCast(i32*, ptr);
    *raw_ptr     = 100;

    TEST_condition(Sptr_deref(i32, ptr) == 100);
    TEST_condition(value == 100);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Sptr_Conversion(void) {
    TEST_Result result = TEST_makeResult("Sptr conversion");

    i32  value = 42;
    Sptr ptr   = Sptr_ref(i32, value);

    Mptr  mptr  = Sptr_toMptr(ptr);
    Slice slice = Sptr_toSlice(ptr, 0, 1);

    TEST_condition(!Mptr_isUndefined(mptr));
    TEST_condition(Mptr_at(i32, mptr, 0) == 42);
    TEST_condition(!Slice_isUndefined(slice));
    TEST_condition(Slice_len(slice) == 1);
    TEST_condition(Slice_at(i32, slice, 0) == 42);

    return TEST_completeResult(&result);
}

/*========== Many-Item Pointer Tests ==================================*/

TEST_Result TEST_Mptr_Creation(void) {
    TEST_Result result = TEST_makeResult("Mptr basic creation");

    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    TEST_condition(!Mptr_isUndefined(ptr));
    TEST_condition(Mptr_size(ptr) == sizeof(i32));
    TEST_condition(Mptr_align(ptr) == alignof(i32));
    TEST_condition(Mptr_at(i32, ptr, 0) == 1);
    TEST_condition(Mptr_at(i32, ptr, 4) == 5);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Mptr_Arithmetic(void) {
    TEST_Result result = TEST_makeResult("Mptr arithmetic");

    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    Mptr  ptr2 = Mptr_add(ptr, 2);
    Mptr  ptr3 = Mptr_sub(ptr2, 1);
    isize diff = Mptr_diff(ptr2, ptr);

    TEST_condition(Mptr_at(i32, ptr2, 0) == 3);
    TEST_condition(Mptr_at(i32, ptr3, 0) == 2);
    TEST_condition(diff == 2);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Mptr_Access(void) {
    TEST_Result result = TEST_makeResult("Mptr access");

    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    // Test raw access
    i32* raw = Mptr_rawCast(i32*, ptr);
    raw[2]   = 30;

    // Test Mptr_at
    i32 value_at = Mptr_at(i32, ptr, 2);

    // Test Mptr_atAddr
    i32* addr_at = (i32*)Mptr_atAddr(ptr, 3);
    *addr_at     = 40;

    // Test Mptr_atSptr
    Sptr sptr             = Mptr_atSptr(ptr, 4);
    Sptr_deref(i32, sptr) = 50;

    TEST_condition(raw[0] == 1 && raw[4] == 50);
    TEST_condition(Mptr_at(i32, ptr, 0) == 1);
    TEST_condition(value_at == 30);
    TEST_condition(Mptr_at(i32, ptr, 3) == 40);
    TEST_condition(Mptr_at(i32, ptr, 4) == 50);
    TEST_condition(*(i32*)Mptr_atAddr(ptr, 2) == 30);
    TEST_condition(Sptr_deref(i32, Mptr_atSptr(ptr, 2)) == 30);
    TEST_condition(array[2] == 30 && array[3] == 40 && array[4] == 50);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Mptr_Conversion(void) {
    TEST_Result result = TEST_makeResult("Mptr conversion");

    i32  array[] = { 1, 2, 3, 4, 5 };
    Mptr ptr     = Mptr_ref(i32, array);

    Slice slice = Mptr_toSlice(ptr, 1, 4);

    TEST_condition(!Slice_isUndefined(slice));
    TEST_condition(Slice_len(slice) == 3);
    TEST_condition(Slice_at(i32, slice, 0) == 2);
    TEST_condition(Slice_at(i32, slice, 2) == 4);

    return TEST_completeResult(&result);
}

/*========== Slice Tests =============================================*/

TEST_Result TEST_Slice_Creation(void) {
    TEST_Result result = TEST_makeResult("Slice basic creation");

    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    TEST_condition(!Slice_isUndefined(slice));
    TEST_condition(Slice_size(slice) == sizeof(i32));
    TEST_condition(Slice_align(slice) == alignof(i32));
    TEST_condition(Slice_len(slice) == 5);
    TEST_condition(Slice_at(i32, slice, 0) == 1);
    TEST_condition(Slice_at(i32, slice, 4) == 5);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Slice_SubSlicing(void) {
    TEST_Result result = TEST_makeResult("Slice sub-slicing");

    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    Slice sub    = Slice_slice(slice, 1, 4);
    Slice prefix = Slice_prefix(slice, 3);
    Slice suffix = Slice_suffix(slice, 2);

    TEST_condition(Slice_len(sub) == 3);
    TEST_condition(Slice_at(i32, sub, 0) == 2);
    TEST_condition(Slice_at(i32, sub, 2) == 4);
    TEST_condition(Slice_len(prefix) == 3);
    TEST_condition(Slice_at(i32, prefix, 0) == 1);
    TEST_condition(Slice_len(suffix) == 3);
    TEST_condition(Slice_at(i32, suffix, 0) == 3);

    return TEST_completeResult(&result);
}

TEST_Result TEST_Slice_MptrAccess(void) {
    TEST_Result result = TEST_makeResult("Slice Mptr access");

    i32   array[] = { 1, 2, 3, 4, 5 };
    Slice slice   = Slice_refArray(i32, array);

    Mptr at_ptr    = Slice_atMptr(slice, 2);
    Mptr begin_ptr = Slice_beginMptr(slice);

    TEST_condition(Mptr_at(i32, at_ptr, 0) == 3);
    TEST_condition(Mptr_at(i32, begin_ptr, 0) == 1);
    TEST_condition(Mptr_at(i32, Mptr_add(begin_ptr, 1), 0) == 2);
    TEST_condition(Mptr_at(i32, Mptr_add(at_ptr, 1), 0) == 4);
    TEST_condition(Mptr_diff(at_ptr, begin_ptr) == 2);

    return TEST_completeResult(&result);
}

/*========== Error Cases Tests =======================================*/

TEST_Result TEST_ErrorCases(void) {
    TEST_Result result = TEST_makeResult("Error cases");

    i32   array[] = { 1, 2, 3 };
    Slice slice   = Slice_refArray(i32, array);

    // Test range checks
    // bool in_bounds   = true;
    // bool valid_slice = true;

    // Check in-bounds access succeeds
    TEST_condition(Slice_at(i32, slice, 0) == 1);
    TEST_condition(Slice_at(i32, slice, 2) == 3);

    // Check valid slice operations succeed
    Slice valid_sub = Slice_slice(slice, 0, 3);
    TEST_condition(Slice_len(valid_sub) == 3);
    TEST_condition(Slice_at(i32, valid_sub, 0) == 1);

    // Note: The following operations would trigger assertions in debug mode
    // Out of bounds: Slice_at(i32, slice, 3)
    // Invalid slice: Slice_slice(slice, 0, 4)

    return TEST_completeResult(&result);
}

/*========== Main Test Runner ========================================*/

int main(void) {
    TEST_init();

    printf("Starting Smart Pointer Tests\n");

    // PtrBase tests
    TEST_printSection("PtrBase");
    TEST_printResult(TEST_PtrBase_Creation());
    TEST_printResult(TEST_PtrBase_Alignment());

    // Sptr tests
    TEST_printSection("Single-Item Pointer");
    TEST_printResult(TEST_Sptr_Creation());
    TEST_printResult(TEST_Sptr_Modification());
    TEST_printResult(TEST_Sptr_Conversion());

    // Mptr tests
    TEST_printSection("Many-Item Pointer");
    TEST_printResult(TEST_Mptr_Creation());
    TEST_printResult(TEST_Mptr_Arithmetic());
    TEST_printResult(TEST_Mptr_Access());
    TEST_printResult(TEST_Mptr_Conversion());

    // Slice tests
    TEST_printSection("Slice");
    TEST_printResult(TEST_Slice_Creation());
    TEST_printResult(TEST_Slice_SubSlicing());
    TEST_printResult(TEST_Slice_MptrAccess());

    // Error cases
    TEST_printSection("Error Cases");
    TEST_printResult(TEST_ErrorCases());

    // Print final summary
    TEST_printSummary();

    return TEST_fini();
}
