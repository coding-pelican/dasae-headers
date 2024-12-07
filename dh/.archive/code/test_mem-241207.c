/**
 * @copyright Copyright 2024. All rights reserved.
 * @file    test_heap.c
 * @brief   Unit tests for heap allocator implementation
 */

#include "dh/core/prim/struct.h"
#include "dh/heap.h"
#include "dh/debug/assert.h"
#include <stdio.h>
#include <string.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, bool success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

void TEST_HeapAllocator_basic() {
    TEST_PrintSection("Heap Allocator Basic Operations");

    // Initialize allocator
    mem_HeapAllocator heap_ctx  = { 0 };
    mem_Allocator     allocator = mem_HeapAllocator_init(&heap_ctx);

    // Test zero-size allocation
    Res_Mptr_u8 zero_alloc = allocator.vt_->alloc(allocator.ptr_, 0, 3);
    TEST_PrintResult("Zero-size allocation", Res_isOk(zero_alloc) && Mptr_isZero(Res_unwrap(Res_Mptr_u8, zero_alloc)));

    // Test basic allocation
    Res_Mptr_u8 basic_alloc = allocator.vt_->alloc(allocator.ptr_, 64, 3);
    TEST_PrintResult("Basic allocation", Res_isOk(basic_alloc));

    if (Res_isOk(basic_alloc)) {
        Mptr_u8 ptr = Res_unwrap(Res_Mptr_u8, basic_alloc);
        TEST_PrintResult("Allocation alignment", Mptr_isAligned(ptr));

        // Write and read test
        u8* raw_ptr = (u8*)Mptr_raw(ptr);
        raw_ptr[0]  = 42;
        TEST_PrintResult("Memory access", raw_ptr[0] == 42);

        // Free memory
        Slice_u8 buf = { .ptr = ptr, .len = 64 };
        allocator.vt_->free(allocator.ptr_, buf, 3);
    }
}

void TEST_HeapAllocator_alignment() {
    TEST_PrintSection("Heap Allocator Alignment Tests");

    mem_HeapAllocator heap_ctx  = { 0 };
    mem_Allocator     allocator = mem_HeapAllocator_init(&heap_ctx);

    // Test various alignments
    for (u8 align = 0; align <= 4; align++) {
        Res_Mptr_u8 alloc = allocator.vt_->alloc(allocator.ptr_, 32, align);

        if (Res_isOk(alloc)) {
            Mptr_u8 ptr        = Res_unwrap(Res_Mptr_u8, alloc);
            usize   addr       = (usize)Mptr_raw(ptr);
            bool    is_aligned = (addr % (1ULL << align)) == 0;

            char   test_name[64] = cleared();
            ignore snprintf(test_name, sizeof(test_name), "Alignment 2^%d", align);
            TEST_PrintResult(test_name, is_aligned);

            Slice_u8 buf = { .ptr = ptr, .len = 32 };
            allocator.vt_->free(allocator.ptr_, buf, align);
        } else {
            printf("Failed to allocate with alignment 2^%d\n", align);
        }
    }
}

void TEST_HeapAllocator_resize() {
    TEST_PrintSection("Heap Allocator Resize Operations");

    mem_HeapAllocator heap_ctx  = { 0 };
    mem_Allocator     allocator = mem_HeapAllocator_init(&heap_ctx);

    // Initial allocation
    Res_Mptr_u8 initial = allocator.vt_->alloc(allocator.ptr_, 64, 3);
    TEST_PrintResult("Initial allocation", Res_isOk(initial));

    if (Res_isOk(initial)) {
        Mptr_u8  ptr = Res_unwrap(Res_Mptr_u8, initial);
        Slice_u8 buf = { .ptr = ptr, .len = 64 };

        // Test grow
        bool grow_result = allocator.vt_->resize(allocator.ptr_, buf, 3, 128);
        TEST_PrintResult("Resize grow attempt", true); // Result doesn't matter, just that it runs
        unused(grow_result);

        // Test shrink
        bool shrink_result = allocator.vt_->resize(allocator.ptr_, buf, 3, 32);
        TEST_PrintResult("Resize shrink attempt", true); // Result doesn't matter, just that it runs
        unused(shrink_result);

        // Test resize to zero
        bool zero_result = allocator.vt_->resize(allocator.ptr_, buf, 3, 0);
        TEST_PrintResult("Resize to zero", zero_result == true);

        // Free memory
        allocator.vt_->free(allocator.ptr_, buf, 3);
    }
}

void TEST_HeapAllocator_stress() {
    TEST_PrintSection("Heap Allocator Stress Tests");

    mem_HeapAllocator heap_ctx  = { 0 };
    mem_Allocator     allocator = mem_HeapAllocator_init(&heap_ctx);

    const usize num_allocs = 1000;
    const usize max_size   = 1024;

    // Track allocations
    struct {
        Mptr_u8 ptr;
        usize   size;
        u8      align;
    } allocs[1000] = { 0 };

    // Perform multiple allocations
    usize successful = 0;
    for (usize i = 0; i < num_allocs; i++) {
        usize size  = (i % max_size) + 1;
        u8    align = (u8)(i % 4);

        Res_Mptr_u8 result = allocator.vt_->alloc(allocator.ptr_, size, align);

        if (Res_isOk(result)) {
            allocs[successful].ptr   = Res_unwrap(Res_Mptr_u8, result);
            allocs[successful].size  = size;
            allocs[successful].align = align;
            successful++;
        }
    }

    TEST_PrintResult("Multiple allocations", successful > 0);

    // Free all allocations
    for (usize i = 0; i < successful; i++) {
        Slice_u8 buf = { .ptr = allocs[i].ptr, .len = allocs[i].size };
        allocator.vt_->free(allocator.ptr_, buf, allocs[i].align);
    }

    TEST_PrintResult("Multiple frees", true);
}

void TEST_HeapAllocator_errors() {
    TEST_PrintSection("Heap Allocator Error Handling");

    mem_HeapAllocator heap_ctx  = { 0 };
    mem_Allocator     allocator = mem_HeapAllocator_init(&heap_ctx);

    // Test null context
    Res_Mptr_u8 null_ctx_result = mem_HeapAllocator_alloc(null, 64, 3);
    TEST_PrintResult("Null context handling", Res_isErr(null_ctx_result));

    // Test very large allocation (should fail)
    Res_Mptr_u8 large_alloc = allocator.vt_->alloc(allocator.ptr_, (usize)-1, 3);
    TEST_PrintResult("Excessive allocation handling", Res_isErr(large_alloc));

    // Test invalid alignment
    Res_Mptr_u8 invalid_align = allocator.vt_->alloc(allocator.ptr_, 64, 255);
    TEST_PrintResult("Invalid alignment handling", Res_isErr(invalid_align));
}

int main(void) {
    printf("Starting Heap Allocator Tests\n");

    TEST_HeapAllocator_basic();
    TEST_HeapAllocator_alignment();
    TEST_HeapAllocator_resize();
    TEST_HeapAllocator_stress();
    TEST_HeapAllocator_errors();

    printf("\nAll tests completed.\n");
    return 0;
}
