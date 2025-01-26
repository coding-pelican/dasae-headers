#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/Random.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/*========== Test Targets ===================================================*/

// Helper function to perform a safe multiplication, avoiding potential overflow
use_Err(MulErr, Overflow);
force_inline Err$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(Err$usize);
    if (0 < lhs && usize_limit / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulErr_err(MulErrType_Overflow));
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
) must_check;
// Modernized stable sort (using merge sort)
static Err$void stableSort(
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr        arg,
    mem_Allocator allocator
) must_check;

/*========== Test Targets Implementation ====================================*/

Err$void mergeSortWithTmpRecur( // NOLINT
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
Err$void stableSort(
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
        assignSome(allocator, heap_Page_allocator(&(heap_Page){}));
    }
    return unwrap(allocator);
}

/*========== Benchmarking Functions =========================================*/

#define BENCHMARK(NAME, FUNC, ITER, WORK_ARR, SRC_ARR, ARR_LEN, ALLOCATOR)                       \
    printf("%-20s | %5d elements | %4d iterations\n", NAME, (i32)(ARR_LEN), ITER);               \
    do {                                                                                         \
        clock_t       overhead_time = 0;                                                         \
        const clock_t begin         = clock();                                                   \
        for (i32 i = 0; i < (ITER); ++i) {                                                       \
            const clock_t overhead_begin = clock();                                              \
            memcpy(WORK_ARR, SRC_ARR, (ARR_LEN) * sizeof(i32));                                  \
            overhead_time += (clock() - overhead_begin);                                         \
            ignore BENCHMARK_##FUNC(WORK_ARR, ARR_LEN, ALLOCATOR);                               \
        }                                                                                        \
        const clock_t end      = clock();                                                        \
        const f64     total_ms = ((f64)(end - begin - overhead_time) * 1000.0) / CLOCKS_PER_SEC; \
        const f64     avg_ms   = total_ms / (ITER);                                              \
        printf("  %-18s: %.3f ms (avg = %.3f ms)\n", NAME, total_ms, avg_ms);                    \
    } while (0)

#define BENCHMARK_qsort(WORK_ARR, ARR_LEN, ALLOCATOR) \
    IMPL__BENCHMARK_qsort(WORK_ARR, ARR_LEN)

#define IMPL__BENCHMARK_qsort(WORK_ARR, ARR_LEN) \
    qsort(WORK_ARR, ARR_LEN, sizeof(i32), qsort_compareInts)

#define BENCHMARK_stableSort(WORK_ARR, ARR_LEN, ALLOCATOR) \
    IMPL__BENCHMARK_stableSort(WORK_ARR, ARR_LEN, ALLOCATOR)

#define IMPL__BENCHMARK_stableSort(WORK_ARR, ARR_LEN, ALLOCATOR) \
    stableSort(WORK_ARR, ARR_LEN, sizeof(i32), stableSort_compareInts, null, ALLOCATOR)

static i32 qsort_compareInts(const void* lhs, const void* rhs) {
    return (*as$(i32*, lhs) - *as$(i32*, rhs));
}

force_inline cmp_Ord stableSort_compareInts(const void* lhs, const void* rhs, const void* arg) {
    unused(arg);
    return prim_cmp(*as$(i32*, lhs), *as$(i32*, rhs));
}

static void generateRandomData(i32* ptr, usize len) {
    for (usize i = 0; i < len; ++i) {
        ptr[i] = Random_i32() % 1000;
    }
}

must_check Err$void benchmark(usize sample_data_count, i32 iterations) {
    scope_reserveReturn(Err$void) {
        var allocator = testAllocator();

        let data = meta_cast$(Sli$i32, try(mem_Allocator_alloc(allocator, typeInfo(i32), sample_data_count)));
        defer(mem_Allocator_free(allocator, anySli(data)));
        generateRandomData(data.ptr, data.len);

        // Make copies for fair comparison
        let qsort_data = meta_cast$(Sli$i32, try(mem_Allocator_alloc(allocator, typeInfo(i32), sample_data_count)));
        defer(mem_Allocator_free(allocator, anySli(qsort_data)));
        memcpy(qsort_data.ptr, data.ptr, data.len * sizeof(i32));

        let stable_sort_data = meta_cast$(Sli$i32, try(mem_Allocator_alloc(allocator, typeInfo(i32), sample_data_count)));
        defer(mem_Allocator_free(allocator, anySli(stable_sort_data)));
        memcpy(stable_sort_data.ptr, data.ptr, data.len * sizeof(i32));

        // Benchmark qsort
        BENCHMARK("(std)qsort", qsort, iterations,
                  qsort_data.ptr, // destination (work array)
                  data.ptr,       // source
                  data.len,
                  allocator);

        // Benchmark stableSort
        BENCHMARK("(dh)stableSort", stableSort, iterations,
                  stable_sort_data.ptr, // destination (work array)
                  data.ptr,             // source
                  data.len,
                  allocator);

        return_void();
    }
    scope_returnReserved;
}

/*========== Benchmark Runner ===============================================*/

int main(void) {
    const i32 iterations = 100;
    const i32 seed       = 12345;
    Random_initWithSeed(seed); // fixed seed => reproducible random data

    printf("=== C `(dh)stableSort` vs C `(std)qsort` Benchmark ===\n");

    printf("\n-- 1,000 elements --\n");
    catch (benchmark(1000, iterations), err, {
        exit(Err_type(err));
    });
    printf("\n-- 10,000 elements --\n");
    catch (benchmark(10000, iterations), err, {
        exit(Err_type(err));
    });
    printf("\n-- 100,000 elements --\n");
    catch (benchmark(100000, iterations), err, {
        exit(Err_type(err));
    });

    ignore getchar();
    return 0;
}
