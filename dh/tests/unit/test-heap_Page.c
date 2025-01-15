/**
 * @file    test_heap_Page.c
 * @brief   Unit tests for page allocator implementation
 */

#include "dh/core.h"
#include "dh/mem/common.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/debug/assert.h"

#include "dh/TEST.h"
#include "dh/meta/common.h"

#define MAIN_NO_HIJACK (1)
#include "dh/main.h"

#include <stdio.h>

/*========== Basic Operations Tests ====================================*/

TEST_Result TEST_heap_Page_Basic(void) {
    TEST_Result result = TEST_makeResult("Page allocator basic operations");

    // Initialize allocator
    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // // Test zero-size allocation
    // TypeInfo zero_type    = { .size = 0, .align = 1 };
    // let      zero_sli_res = mem_Allocator_alloc(allocator, zero_type, 1);
    // TEST_condition(!zero_sli_res.is_err);

    // if (!zero_sli_res.is_err) {
    //     meta_Sli zero_sli = zero_sli_res.ok;
    //     // Free the memory using anySli since it's a slice
    //     mem_Allocator_free(allocator, meta_sliToAny(zero_sli));
    // }

    // Test page-sized allocation
    TypeInfo page_type = { .size = mem_page_size, .align = mem_page_size };
    let      page_res  = mem_Allocator_create(allocator, page_type);
    TEST_condition(!page_res.is_err);

    if (!page_res.is_err) {
        meta_Ptr ptr = page_res.ok;
        TEST_condition(ptr.addr != null);
        TEST_condition(mem_isAligned((usize)ptr.addr, mem_page_size));

        // Write and read test
        u8* data = ptr.addr;
        *data    = 42;
        TEST_condition(*data == 42);

        // Free using meta_ptrToAny since it's a single item pointer
        mem_Allocator_destroy(allocator, meta_ptrToAny(ptr));
    }

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Alignment Tests ==========================================*/

TEST_Result TEST_heap_Page_Alignment(void) {
    TEST_Result result = TEST_makeResult("Page allocator alignment");

    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // Test various alignments up to page size
    for (usize align = 1; align <= mem_page_size; align *= 2) {
        TypeInfo type = { .size = align, .align = align };
        let      res  = mem_Allocator_create(allocator, type);
        TEST_condition(!res.is_err);

        if (!res.is_err) {
            meta_Ptr ptr = res.ok;
            TEST_condition(ptr.addr != null);
            TEST_condition(mem_isAligned((usize)ptr.addr, align));

            mem_Allocator_destroy(allocator, meta_ptrToAny(ptr));
        }
    }

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Slice Operations Tests ==================================*/

TEST_Result TEST_heap_Page_Slices(void) {
    TEST_Result result = TEST_makeResult("Page allocator slice operations");

    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // Test slices with page-multiple sizes
    TypeInfo page_type = { .size = mem_page_size, .align = mem_page_size };
    let      slice_res = mem_Allocator_alloc(allocator, page_type, 2); // 2 pages
    TEST_condition(!slice_res.is_err);

    if (!slice_res.is_err) {
        meta_Sli slice = slice_res.ok;
        TEST_condition(slice.addr != null);
        TEST_condition(slice.len == 2);
        TEST_condition(mem_isAligned((usize)slice.addr, mem_page_size));

        // Access check
        u8* pages            = slice.addr;
        pages[0]             = 0xAA;
        pages[mem_page_size] = 0xBB;
        TEST_condition(pages[0] == 0xAA);
        TEST_condition(pages[mem_page_size] == 0xBB);

        let memory = meta_sliToAny(slice);

        // Try shrinking (should succeed)
        let shrink_ok = mem_Allocator_resize(allocator, memory, 1);
        TEST_condition(shrink_ok);

        // Try expanding back (should fail - page allocator doesn't support expansion)
        let expand_not_ok = mem_Allocator_resize(allocator, memory, 3);
        TEST_condition(!expand_not_ok);

        // // Try reallocation
        // let realloc_res = mem_Allocator_realloc(allocator, memory, 3);
        // TEST_condition(!realloc_res.has_value);

        mem_Allocator_free(allocator, memory);
    }

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Large Allocation Tests ==================================*/

TEST_Result TEST_heap_Page_LargeAlloc(void) {
    TEST_Result result = TEST_makeResult("Page allocator large allocations");

    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // Test allocation near the size limit
    const usize almost_max = usize_limit - (mem_page_size - 1);
    TypeInfo    large_type = { .size = 1, .align = mem_page_size };
    let         large_res  = mem_Allocator_alloc(allocator, large_type, almost_max);
    TEST_condition(large_res.is_err); // Should fail gracefully

    // Test reasonably large but valid allocation
    const usize large_but_valid = 16 * mem_page_size; // 16 pages
    let         valid_res       = mem_Allocator_alloc(allocator, large_type, large_but_valid);
    TEST_condition(!valid_res.is_err);

    if (!valid_res.is_err) {
        meta_Sli large_slice = valid_res.ok;
        TEST_condition(large_slice.addr != null);
        TEST_condition(large_slice.len == large_but_valid);
        TEST_condition(mem_isAligned((usize)large_slice.addr, mem_page_size));

        mem_Allocator_free(allocator, meta_sliToAny(large_slice));
    }

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Error Handling Tests ====================================*/

TEST_Result TEST_heap_Page_Errors(void) {
    TEST_Result result = TEST_makeResult("Page allocator error handling");

    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // Test invalid size
    TypeInfo type     = { .size = 1, .align = mem_page_size };
    let      huge_res = mem_Allocator_alloc(allocator, type, usize_limit);
    TEST_condition(huge_res.is_err);

    // // Test alignment larger than page size (should fail)
    // TypeInfo over_aligned = { .size = mem_page_size, .align = mem_page_size * 2 };
    // let      align_res    = mem_Allocator_create(allocator, over_aligned);
    // TEST_condition(align_res.is_err);

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Multi-Page Access Test ==================================*/

TEST_Result TEST_heap_Page_MultiPageAccess(void) {
    TEST_Result result = TEST_makeResult("Page allocator multi-page access");

    heap_Page     heap_ctx  = {};
    mem_Allocator allocator = heap_Page_allocator(&heap_ctx);
    TEST_condition(!heap_Page_init(allocator).is_err);

    // Allocate multiple pages
    const usize num_pages = 4;
    TypeInfo    type      = { .size = mem_page_size, .align = mem_page_size };
    let         res       = mem_Allocator_alloc(allocator, type, num_pages);
    TEST_condition(!res.is_err);

    if (!res.is_err) {
        meta_Sli slice = res.ok;
        TEST_condition(slice.addr != null);
        TEST_condition(slice.len == num_pages);

        // Write to each page
        u8* pages = slice.addr;
        for (usize i = 0; i < num_pages; ++i) {
            pages[i * mem_page_size] = (u8)(i + 1);
        }

        // Verify writes
        for (usize i = 0; i < num_pages; ++i) {
            TEST_condition(pages[i * mem_page_size] == (u8)(i + 1));
        }

        mem_Allocator_free(allocator, meta_sliToAny(slice));
    }

    heap_Page_fini(allocator);
    return TEST_completeResult(&result);
}

/*========== Test Runner ============================================*/

int main(int argc, const char* argv[]) {
    unused(argc), unused(argv);

    TEST_init();
    printf("Starting Page Allocator Tests\n");

    // Run all tests
    TEST_printSection("Basic Operations");
    TEST_printResult(TEST_heap_Page_Basic());

    TEST_printSection("Alignment");
    TEST_printResult(TEST_heap_Page_Alignment());

    TEST_printSection("Slice Operations");
    TEST_printResult(TEST_heap_Page_Slices());

    TEST_printSection("Large Allocations");
    TEST_printResult(TEST_heap_Page_LargeAlloc());

    TEST_printSection("Error Handling");
    TEST_printResult(TEST_heap_Page_Errors());

    TEST_printSection("Multi-Page Access");
    TEST_printResult(TEST_heap_Page_MultiPageAccess());

    // Print final summary
    TEST_printSummary();
    return TEST_fini();
}
