/**
 * @file    test_heap_Classic.c
 * @brief   Unit tests for heap allocator implementation
 */

#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/debug/assert.h"

#include "dh/TEST.h"

#define main_no_hijack (1)
#include "dh/main.h"

#include <stdio.h>

/*========== Basic Operations Tests ====================================*/

TEST_Result TEST_heap_Classic_Basic(void) {
    TEST_Result result = TEST_makeResult("Heap Classic basic operations");

    // Initialize allocator
    heap_Classic  heap_ctx  = {};
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Test zero-size allocation
    TypeInfo zero_type    = { .size = 0, .align = 1 };
    let      zero_sli_res = mem_Allocator_alloc(allocator, zero_type, 1);
    TEST_condition(!zero_sli_res.is_err);

    if (!zero_sli_res.is_err) {
        meta_Sli zero_sli = zero_sli_res.ok;
        AnyType  memory   = { .ctx = &zero_sli, .type = zero_type };
        mem_Allocator_free(allocator, memory);
    }

    // Test i32 allocation
    TypeInfo i32_type = typeInfo$(i32);
    let      i32_res  = mem_Allocator_create(allocator, i32_type);
    TEST_condition(!i32_res.is_err);

    if (!i32_res.is_err) {
        meta_Ptr ptr = i32_res.ok;
        TEST_condition(ptr.addr != null);
        TEST_condition(((usize)ptr.addr % ptr.type.align) == 0); // Check alignment

        // Write and read test
        i32* data = ptr.addr;
        *data     = 42;
        TEST_condition(*(i32*)ptr.addr == 42);

        // Free memory
        AnyType memory = { .ctx = &ptr, .type = i32_type };
        mem_Allocator_destroy(allocator, memory);
    }

    return TEST_completeResult(&result);
}

/*========== Alignment Tests ==========================================*/

TEST_Result TEST_heap_Classic_Alignment(void) {
    TEST_Result result = TEST_makeResult("Heap Classic alignment");

    heap_Classic  heap_ctx  = {};
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Test various alignments
    for (usize align = 1; align <= 16; align *= 2) {
        TypeInfo type = { .size = sizeof(i32), .align = align };
        let      res  = mem_Allocator_create(allocator, type);
        TEST_condition(!res.is_err);

        if (!res.is_err) {
            meta_Ptr ptr = res.ok;
            TEST_condition(ptr.addr != null);
            TEST_condition(((usize)ptr.addr % align) == 0);

            AnyType memory = { .ctx = &ptr, .type = type };
            mem_Allocator_destroy(allocator, memory);
        }
    }

    return TEST_completeResult(&result);
}

/*========== Slice Operations Tests ==================================*/

TEST_Result TEST_heap_Classic_Slices(void) {
    TEST_Result result = TEST_makeResult("Heap Classic slice operations");

    heap_Classic  heap_ctx  = {};
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Allocate slice of integers
    TypeInfo i32_type  = typeInfo$(i32);
    let      slice_res = mem_Allocator_alloc(allocator, i32_type, 4);
    TEST_condition(!slice_res.is_err);

    if (!slice_res.is_err) {
        meta_Sli slice = slice_res.ok;
        TEST_condition(slice.addr != null);
        TEST_condition(slice.len == 4);
        TEST_condition(((usize)slice.addr % slice.type.align) == 0);

        // Test resize operations
        AnyType memory = { .ctx = &slice, .type = i32_type };

        // Try reallocation
        let realloc_res = mem_Allocator_realloc(allocator, memory, 8);
        TEST_condition(realloc_res.has_value);

        if (realloc_res.has_value) {
            meta_Sli new_slice = realloc_res.value;
            TEST_condition(new_slice.len == 8);
            AnyType new_memory = { .ctx = &new_slice, .type = i32_type };
            mem_Allocator_free(allocator, new_memory);
        } else {
            mem_Allocator_free(allocator, memory);
        }
    }

    return TEST_completeResult(&result);
}

/*========== Stress Tests ============================================*/

TEST_Result TEST_heap_Classic_Stress(void) {
    TEST_Result result = TEST_makeResult("Heap Classic stress test");

    heap_Classic  heap_ctx  = {};
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    const usize num_allocs   = 1000;
    meta_Sli    slices[1000] = { 0 };
    usize       successful   = 0;

    // Multiple allocations with varying sizes
    for (usize i = 0; i < num_allocs; ++i) {
        TypeInfo type = { .size = sizeof(i32), .align = alignof(i32) };
        let      res  = mem_Allocator_alloc(allocator, type, (i % 128) + 1);
        if (!res.is_err) {
            slices[successful++] = res.ok;
        }
    }

    TEST_condition(successful > 0);

    usize count_nonnull_slice = 0;
    usize count_correct_align = 0;
    usize count_correct_len   = 0;

    // Verify all allocations
    for (usize i = 0; i < successful; ++i) {
        count_nonnull_slice += (slices[i].addr != null);
        count_correct_align += (((usize)slices[i].addr % slices[i].type.align) == 0);
        count_correct_len += (slices[i].len == ((i % 128) + 1));

        // Free allocation
        AnyType memory = { .ctx = &slices[i], .type = slices[i].type };
        mem_Allocator_free(allocator, memory);
    }

    TEST_condition(count_nonnull_slice > 0);
    TEST_condition(count_correct_align > 0);
    TEST_condition(count_correct_len > 0);
    TEST_condition(successful == count_nonnull_slice);
    TEST_condition(successful == count_correct_align);
    TEST_condition(successful == count_correct_len);

    return TEST_completeResult(&result);
}

/*========== Error Handling Tests ====================================*/

TEST_Result TEST_heap_Classic_Errors(void) {
    TEST_Result result = TEST_makeResult("Heap Classic error handling");

    // Test with invalid allocator
    // mem_Allocator invalid_allocator = { 0 }; // Null vtable and context
    // TypeInfo      type              = typeInfo$(i32);
    // let           null_ctx_res      = mem_Allocator_create(invalid_allocator, type);
    // TEST_condition(null_ctx_res.is_err);

    // Test with valid allocator but excessive size
    heap_Classic  heap_ctx  = {};
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Try to allocate with maximum size which should fail
    TypeInfo huge_type = { .size = sizeof(i32), .align = alignof(i32) };
    let      huge_res  = mem_Allocator_alloc(allocator, huge_type, SIZE_MAX / sizeof(i32) + 1);
    catch_from(huge_res, err, {
        unused(err);
        TEST_condition(huge_res.is_err);
    });
    var ptr = meta_castSli$(Sli$u8, huge_res.ok);
    mem_Allocator_free(allocator, anySli(ptr));

    return TEST_completeResult(&result);
}

/*========== Test Runner ============================================*/

int main(int argc, const char* argv[]) {
    unused(argc), unused(argv);

    TEST_init();
    printf("Starting Heap Classic Allocator Tests\n");

    // Run all tests
    TEST_printSection("Basic Operations");
    TEST_printResult(TEST_heap_Classic_Basic());

    TEST_printSection("Alignment");
    TEST_printResult(TEST_heap_Classic_Alignment());

    TEST_printSection("Slice Operations");
    TEST_printResult(TEST_heap_Classic_Slices());

    TEST_printSection("Stress Testing");
    TEST_printResult(TEST_heap_Classic_Stress());

    TEST_printSection("Error Handling");
    TEST_printResult(TEST_heap_Classic_Errors());

    // Print final summary
    TEST_printSummary();
    return TEST_fini();
}

// Err$void dh_main(int argc, const char* argv[]) {
//     reserveReturn(Err$void);
//     unused(argc), unused(argv);

//     TEST_init();
//     printf("Starting Heap Classic Allocator Tests\n");

//     // Run all tests
//     TEST_printSection("Basic Operations");
//     TEST_printResult(TEST_heap_Classic_Basic());

//     TEST_printSection("Alignment");
//     TEST_printResult(TEST_heap_Classic_Alignment());

//     TEST_printSection("Slice Operations");
//     TEST_printResult(TEST_heap_Classic_Slices());

//     TEST_printSection("Stress Testing");
//     TEST_printResult(TEST_heap_Classic_Stress());

//     TEST_printSection("Error Handling");
//     TEST_printResult(TEST_heap_Classic_Errors());

//     // Print final summary
//     TEST_printSummary();
//     TEST_fini();

//     return_ok({});
// }
