#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/Random.h"

#define main_no_args (1)
#include "dh/main.h"
#include "dh/TEST.h"
#include <stdlib.h>

/*========== Test Targets ===================================================*/

// Helper function to perform a safe multiplication, avoiding potential overflow
use_ErrSet(MulErr, Overflow);
force_inline Err$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(Err$usize);
    if (0 < lhs && SIZE_MAX / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulErr_err(MulErrCode_Overflow));
    }
    return_ok(lhs * rhs);
}
// Modernized merge sort with temporary buffer (stable sort)
static Err$void mergeSortWithTmpRecur( // NOLINT
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr arg,
    Sli$u8 temp_buffer
) {
    scope_reserveReturn(Err$void) {
        if (num <= 1) { return_void(); /* Nothing to sort */ }

        let mid        = num / 2;
        let base_bytes = as$(u8*, base); // For pointer arithmetic
        let temp_bytes = as$(u8*, temp_buffer.ptr);

        // Sort each half recursively
        try(mergeSortWithTmpRecur(base_bytes, mid, size, comp, arg, temp_buffer));
        try(mergeSortWithTmpRecur(base_bytes + mid * size, num - mid, size, comp, arg, temp_buffer));

        // Merge the sorted halves using the temporary buffer
        usize left_index  = 0;
        usize right_index = mid;
        usize temp_index  = 0;

        while (left_index < mid && right_index < num) {
            if (comp(base_bytes + left_index * size, base_bytes + right_index * size, arg) <= 0) {
                memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, size);
                left_index++;
            } else {
                memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, size);
                right_index++;
            }
            temp_index++;
        }

        // Copy remaining elements
        if (left_index < mid) {
            let remaining = mid - left_index;
            memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, remaining * size);
            temp_index += remaining;
        }
        if (right_index < num) {
            let remaining = num - right_index;
            memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, remaining * size);
            temp_index += remaining;
        }

        // Copy all merged elements back
        memcpy(base_bytes, temp_bytes, temp_index * size);

        return_void();
    }
    scope_returnReserved;
}
// Modernized stable sort (using merge sort)
static Err$void stableSort(
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr        arg,
    mem_Allocator allocator
) {
    scope_reserveReturn(Err$void) {
        // Allocate temporary buffer using the provided allocator
        let checked_size = try(mulSafe(num, size));
        let temp_buffer  = meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer))); // Ensure cleanup

        // Perform merge sort
        try(mergeSortWithTmpRecur(base, num, size, comp, arg, temp_buffer));
        return_void();
    }
    scope_returnReserved;
}

/*========== Helper Functions ===============================================*/

static mem_Allocator testAllocator(void) {
    static Opt$mem_Allocator allocator = none();
    if_none(allocator) {
        assignSome(allocator, heap_Classic_allocator(&(heap_Classic){}));
    }
    return unwrap(allocator);
}

static cmp_Ord compareInt(anyptr_const a, anyptr_const b, anyptr_const arg) {
    unused(arg);
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia < ib) ? cmp_Ord_lt : ((ia > ib) ? cmp_Ord_gt : cmp_Ord_eq);
}

static bool isSorted(const int* arr, usize size) {
    for (usize i = 1; i < size; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

typedef struct {
    int value;
    int seq;
} TestElem;
use_Sli$(TestElem);
force_inline cmp_fnCmp(TestElem) {
    if (self.value < other.value) { return cmp_Ord_lt; }
    if (self.value > other.value) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
static cmp_Ord compareTestElem(anyptr_const lhs, anyptr_const rhs, anyptr_const arg) {
    unused(arg);
    let self  = as$(const TestElem*, lhs);
    let other = as$(const TestElem*, rhs);
    return TestElem_cmp(*self, *other);
}
static bool isStable(const TestElem* arr, usize size) {
    for (usize i = 1; i < size; ++i) {
        if (arr[i].value == arr[i - 1].value && arr[i].seq <= arr[i - 1].seq) {
            return false;
        }
    }
    return true;
}

/*========== Basic Sorting Tests ============================================*/

use_Err$(TEST_Result);
Err$TEST_Result TEST_stableSort_Basic(void) {
    scope_reserveReturn(Err$TEST_Result) {
        var result    = TEST_makeResult("stableSort basic functionality");
        var allocator = testAllocator();

        int data[] = { 5, 2, 8, 1, 3 };
        let size   = sizeof(data) / sizeof(data[0]);

        // Create copy for sorting
        let arr = meta_cast$(Sli$(int), try(mem_Allocator_alloc(allocator, typeInfo(int), size)));
        defer(mem_Allocator_free(allocator, anySli(arr)));
        {
            claim_assert(arr.len == size);
            claim_assert(arr.len * sizeOf$(int) == sizeOf(data));
            memcpy(arr.ptr, data, sizeof(data));
        }

        let res = stableSort(arr.ptr, arr.len, sizeof(int), compareInt, null, allocator);
        TEST_condition(!res.is_err);
        TEST_condition(isSorted(arr.ptr, arr.len));

        return_ok(TEST_completeResult(&result));
    }
    scope_returnReserved;
}

/*========== Stability Tests ================================================*/

Err$TEST_Result TEST_stableSort_Stability(void) {
    scope_reserveReturn(Err$TEST_Result) {
        var result    = TEST_makeResult("stableSort stability");
        var allocator = testAllocator();

        // Create array with duplicate keys but different sequence numbers
        TestElem data[] = {
            { 5, 1 }, { 2, 1 }, { 5, 2 }, { 2, 2 }, { 3, 1 }
        };
        let size = sizeof(data) / sizeof(data[0]);

        let arr = meta_cast$(Sli$TestElem, try(mem_Allocator_alloc(allocator, typeInfo(TestElem), size)));
        defer(mem_Allocator_free(allocator, anySli(arr)));
        {
            claim_assert(arr.len == size);
            claim_assert(arr.len * sizeOf$(TestElem) == sizeOf(data));
            memcpy(arr.ptr, data, sizeof(data));
        }

        let res = stableSort(arr.ptr, arr.len, sizeof(TestElem), compareTestElem, null, allocator);

        TEST_condition(!res.is_err);
        TEST_condition(isStable(arr.ptr, arr.len));

        return_ok(TEST_completeResult(&result));
    }
    scope_returnReserved;
}

/*========== Edge Case Tests ================================================*/

Err$TEST_Result TEST_stableSort_EdgeCases(void) {
    scope_reserveReturn(Err$TEST_Result) {
        var result    = TEST_makeResult("stableSort edge cases");
        var allocator = testAllocator();

        // Test empty array
        let res1 = stableSort(null, 0, sizeof(int), compareInt, null, allocator);
        TEST_condition(!res1.is_err);

        // Test single element
        int single = 42;
        let res2   = stableSort(&single, 1, sizeof(int), compareInt, null, allocator);
        TEST_condition(!res2.is_err);
        TEST_condition(single == 42);

        // Test already sorted array
        int sorted[] = { 1, 2, 3, 4, 5 };
        let res3     = stableSort(sorted, 5, sizeof(int), compareInt, null, allocator);
        TEST_condition(!res3.is_err);
        TEST_condition(isSorted(sorted, 5));

        return_ok(TEST_completeResult(&result));
    }
    scope_returnReserved;
}

/*========== Larger Dataset Tests ===========================================*/

// Generates an array of random integers
static void generateRandomIntArray(int* arr, usize size) {
    for (usize i = 0; i < size; ++i) {
        arr[i] = Random_i32();
    }
}

// Generates an array of TestElem with random values and sequential seq numbers
static void generateRandomTestElemArray(TestElem* arr, usize size) {
    for (usize i = 0; i < size; ++i) {
        arr[i].value = Random_i32();
        arr[i].seq   = (int)i;
    }
}

Err$TEST_Result TEST_stableSort_LargeDataset_Unstable(void) {
    scope_reserveReturn(Err$TEST_Result) {
        var result    = TEST_makeResult("stableSort large dataset (unstable)");
        var allocator = testAllocator();

        let size = 10000; // Adjust as needed

        let arr = meta_cast$(Sli$(int), try(mem_Allocator_alloc(allocator, typeInfo(int), size)));
        defer(mem_Allocator_free(allocator, anySli(arr)));
        generateRandomIntArray(arr.ptr, size);

        let res = stableSort(arr.ptr, arr.len, sizeof(int), compareInt, null, allocator);
        TEST_condition(!res.is_err);
        TEST_condition(isSorted(arr.ptr, arr.len));

        return_ok(TEST_completeResult(&result));
    }
    scope_returnReserved;
}

Err$TEST_Result TEST_stableSort_LargeDataset_Stable(void) {
    scope_reserveReturn(Err$TEST_Result) {
        var result    = TEST_makeResult("stableSort large dataset (stable)");
        var allocator = testAllocator();

        let size = 10000; // Adjust as needed

        let arr = meta_cast$(Sli$TestElem, try(mem_Allocator_alloc(allocator, typeInfo(TestElem), size)));
        defer(mem_Allocator_free(allocator, anySli(arr)));
        generateRandomTestElemArray(arr.ptr, size);

        let res = stableSort(arr.ptr, arr.len, sizeof(TestElem), compareTestElem, null, allocator);
        TEST_condition(!res.is_err);
        TEST_condition(isStable(arr.ptr, arr.len));

        return_ok(TEST_completeResult(&result));
    }
    scope_returnReserved;
}

/*========== Test Runner ====================================================*/

Err$void dh_main(void) {
    reserveReturn(Err$void);

    // Seed the random number generator (important for reproducibility)
    Random_initWithSeed(12345); // Use a fixed seed for consistent test results

    TEST_init();
    printf("Starting stableSort Tests\n");

    TEST_printSection("Basic Functionality");
    TEST_printResult(try(TEST_stableSort_Basic()));

    TEST_printSection("Stability");
    TEST_printResult(try(TEST_stableSort_Stability()));

    TEST_printSection("Edge Cases");
    TEST_printResult(try(TEST_stableSort_EdgeCases()));

    TEST_printSection("Large Datasets (Unstable)");
    TEST_printResult(try(TEST_stableSort_LargeDataset_Unstable()));

    TEST_printSection("Large Datasets (Stable)");
    TEST_printResult(try(TEST_stableSort_LargeDataset_Stable()));

    TEST_printSummary();
    TEST_fini();
    return_ok({});
}
