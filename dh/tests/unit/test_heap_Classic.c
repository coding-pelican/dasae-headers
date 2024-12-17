/**
 * @file    test_heap_Classic.c
 * @brief   Unit tests for heap allocator implementation
 */

#include "dh/core/prim/struct.h"
#include "dh/heap/Classic.h"
#include "dh/debug/assert.h"
#include <stdio.h>
#include <string.h>

/* Test framework import */
#include "dh/TEST.h"

/*========== Basic Operations Tests ====================================*/

TEST_Result TEST_heap_Classic_Basic(void) {
    TEST_Result result = TEST_makeResult("Heap Classic basic operations");

    // Initialize allocator
    heap_Classic  heap_ctx  = cleared();
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Test zero-size allocation
    PtrBase_MetaData meta_zero = PtrBase_typeInfo(u8);
    meta_zero.type_size        = 0; // Force zero size
    Res_Mptr zero_res          = heap_Classic_alloc(allocator.ctx, meta_zero);
    TEST_condition(Res_isOk(zero_res));
    TEST_condition(Mptr_isUndefined(Res_unwrap(Res_Mptr, zero_res)));

    // Test i32 allocation
    PtrBase_MetaData meta_i32 = PtrBase_typeInfo(i32);
    Res_Mptr         i32_res  = heap_Classic_alloc(allocator.ctx, meta_i32);
    TEST_condition(Res_isOk(i32_res));

    if (Res_isOk(i32_res)) {
        Mptr ptr = Res_unwrap(Res_Mptr, i32_res);
        TEST_condition(!Mptr_isUndefined(ptr));
        TEST_condition(Mptr_isAligned(ptr));
        TEST_condition(Mptr_size(ptr) == sizeof(i32));
        TEST_condition(Mptr_align(ptr) == alignof(i32));

        // Write and read test
        i32* data = Mptr_rawCast(i32*, ptr);
        *data     = 42;
        TEST_condition(Mptr_at(i32, ptr, 0) == 42);

        // Free memory
        heap_Classic_free(allocator.ctx, ptr);
    }

    return TEST_completeResult(&result);
}

/*========== Alignment Tests ==========================================*/

TEST_Result TEST_heap_Classic_Alignment(void) {
    TEST_Result result = TEST_makeResult("Heap Classic alignment");

    heap_Classic  heap_ctx  = cleared();
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Test various alignments using a struct
    typedef struct AlignedStruct {
        i32 value;
    } AlignedStruct;

    // Create aligned structs with different alignments
    for (usize align_order = 0; align_order <= 4; align_order++) {
        PtrBase_MetaData meta = {
            .type_size  = sizeof(AlignedStruct),
            .log2_align = align_order,
            .is_defined = true
        };

        Res_Mptr res = heap_Classic_alloc(allocator.ctx, meta);
        TEST_condition(Res_isOk(res));

        if (Res_isOk(res)) {
            Mptr ptr = Res_unwrap(Res_Mptr, res);
            TEST_condition(!Mptr_isUndefined(ptr));
            TEST_condition(Mptr_align(ptr) == (1ull << align_order));
            TEST_condition(Mptr_isAligned(ptr));
            heap_Classic_free(allocator.ctx, ptr);
        }
    }

    return TEST_completeResult(&result);
}

/*========== Resize Operations Tests =================================*/

TEST_Result TEST_heap_Classic_Resize(void) {
    TEST_Result result = TEST_makeResult("Heap Classic resize operations");

    heap_Classic  heap_ctx  = cleared();
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    // Allocate array of 4 integers
    struct PtrBase_MetaData meta = PtrBase_typeInfo(i32);
    Res_Mptr                res  = heap_Classic_alloc(allocator.ctx, meta);
    TEST_condition(Res_isOk(res));

    if (Res_isOk(res)) {
        Mptr ptr = Res_unwrap(Res_Mptr, res);

        // Test trying to grow to 8 elements (should fail)
        bool grow_result = heap_Classic_resize(allocator.ctx, ptr, 8);
        TEST_condition(!grow_result); // Growing should fail

        // Test shrinking to 2 elements (should succeed)
        bool shrink_result = heap_Classic_resize(allocator.ctx, ptr, 2);
        TEST_condition(shrink_result); // Shrinking should succeed

        // Test keeping same size (should succeed)
        bool same_result = heap_Classic_resize(allocator.ctx, ptr, 4);
        TEST_condition(same_result); // Same size should succeed

        // Test resize to zero elements (should succeed)
        bool zero_result = heap_Classic_resize(allocator.ctx, ptr, 0);
        TEST_condition(zero_result); // Zero should always succeed

        heap_Classic_free(allocator.ctx, ptr);
    }

    return TEST_completeResult(&result);
}

/*========== Stress Tests ============================================*/

TEST_Result TEST_heap_Classic_Stress(void) {
    TEST_Result result = TEST_makeResult("Heap Classic stress test");

    heap_Classic  heap_ctx  = cleared();
    mem_Allocator allocator = heap_Classic_allocator(&heap_ctx);

    const usize num_allocs   = 1000;
    Mptr        allocs[1000] = cleared();
    usize       successful   = 0;

    // Multiple allocations with varying sizes
    for (usize i = 0; i < num_allocs; ++i) {
        PtrBase_MetaData meta = {
            .type_size  = (i % 128) + 1,
            .log2_align = 3, // 8-byte alignment
            .is_defined = true
        };

        Res_Mptr res = heap_Classic_alloc(allocator.ctx, meta);
        if (Res_isOk(res)) {
            allocs[successful++] = Res_unwrap(Res_Mptr, res);
        }
    }

    TEST_condition(successful > 0);
    TEST_condition(successful == num_allocs);

    // Verify all allocations
    usize successful_count_not_undefined = 0;
    usize successful_count_align_correct = 0;
    usize successful_count_size_correct  = 0;
    for (usize i = 0; i < successful; ++i) {
        successful_count_not_undefined += (!Mptr_isUndefined(allocs[i]));
        successful_count_align_correct += Mptr_isAligned(allocs[i]);
        successful_count_size_correct += Mptr_size(allocs[i]) == ((i % 128) + 1);
    }
    TEST_condition(successful_count_not_undefined == successful);
    TEST_condition(successful_count_align_correct == successful);
    TEST_condition(successful_count_size_correct == successful);

    // Free all allocations
    for (usize i = 0; i < successful; ++i) {
        heap_Classic_free(allocator.ctx, allocs[i]);
    }

    return TEST_completeResult(&result);
}

/*========== Error Handling Tests ====================================*/

TEST_Result TEST_heap_Classic_Errors(void) {
    TEST_Result result = TEST_makeResult("Heap Classic error handling");

    heap_Classic heap_ctx     = cleared();
    Sptr         invalid_self = cleared(); // Undefined self pointer

    // Test undefined self
    PtrBase_MetaData meta         = PtrBase_typeInfo(i32);
    Res_Mptr         null_ctx_res = heap_Classic_alloc(invalid_self, meta);
    TEST_condition(Res_isErr(null_ctx_res));

    // Test excessive allocation
    mem_Allocator    allocator = heap_Classic_allocator(&heap_ctx);
    PtrBase_MetaData huge_meta = {
        .type_size  = SIZE_MAX, // Maximum size
        .log2_align = 3,
        .is_defined = true
    };
    Res_Mptr huge_res = heap_Classic_alloc(allocator.ctx, huge_meta);
    TEST_condition(Res_isErr(huge_res));

    return TEST_completeResult(&result);
}

/*========== Main Test Runner ========================================*/

int main(void) {
    TEST_init();

    printf("Starting Heap Classic Allocator Tests\n");

    // Run all tests
    TEST_printSection("Basic Operations");
    TEST_printResult(TEST_heap_Classic_Basic());

    TEST_printSection("Alignment");
    TEST_printResult(TEST_heap_Classic_Alignment());

    TEST_printSection("Resize Operations");
    TEST_printResult(TEST_heap_Classic_Resize());

    TEST_printSection("Stress Testing");
    TEST_printResult(TEST_heap_Classic_Stress());

    TEST_printSection("Error Handling");
    TEST_printResult(TEST_heap_Classic_Errors());

    // Print final summary
    TEST_printSummary();

    return TEST_fini();
}
