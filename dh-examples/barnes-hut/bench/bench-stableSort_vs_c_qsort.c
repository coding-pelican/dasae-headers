#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/Rand.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define INSERTION_THRESHOLD 32

/*========== Test Targets ===================================================*/

// Helper function to perform a safe multiplication, avoiding potential overflow
config_ErrSet(MulErr, Overflow);
$inline_always E$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(E$usize);
    if (0 < lhs && usize_limit / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulE_err(MulErrCode_Overflow));
    }
    return_ok(lhs * rhs);
}
// Swap two elements of given size
$inline_always $must_check E$void swapElements(u8* a, u8* b, usize size) {
    scope_reserveReturn(E$void) {
        u8* temp = alloca(size);
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
        return_void();
    }
    scope_returnReserved;
}
// Insertion sort for small arrays
static $must_check E$void insertionSort(
    P$raw base,
    usize num,
    usize size,
    cmp_Ord (*comp)(P_const$raw, P_const$raw, P_const$raw),
    P_const$raw arg
) {
    scope_reserveReturn(E$void) {
        u8* base_ptr = (u8*)base;
        for (usize i = 1; i < num; ++i) {
            u8*   current = base_ptr + i * size;
            usize j       = i;
            while (j > 0) {
                u8* prev = current - size;
                if (comp(prev, current, arg) <= 0) {
                    break;
                }
                try_(swapElements(prev, current, size));
                current = prev;
                j--;
            }
        }
        return_void();
    }
    scope_returnReserved;
}
// Modernized merge sort with temporary buffer (stable sort)
static $must_check E$void mergeSortWithTmpRecur( // NOLINT
    P$raw base, usize num, usize size, cmp_Ord (*comp)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg), P_const$raw arg, S$u8 temp_buffer
) {
    scope_reserveReturn(E$void) {
        if (num <= INSERTION_THRESHOLD) {
            return insertionSort(base, num, size, comp, arg);
        }

        u8*   base_bytes = (u8*)base;
        usize mid        = num / 2;

        // Sort each half recursively
        try_(mergeSortWithTmpRecur(base_bytes, mid, size, comp, arg, temp_buffer));
        try_(mergeSortWithTmpRecur(base_bytes + mid * size, num - mid, size, comp, arg, temp_buffer));

        // Check if merging is necessary
        u8* const left_last   = base_bytes + (mid - 1) * size;
        u8* const right_first = base_bytes + mid * size;
        if (comp(left_last, right_first, arg) <= 0) {
            return_void(); // Already ordered, no merge needed
        }

        // Merge the sorted halves using the temporary buffer
        u8* left_ptr  = base_bytes;
        u8* left_end  = left_ptr + mid * size;
        u8* right_ptr = base_bytes + mid * size;
        u8* right_end = base_bytes + num * size;
        u8* temp_ptr  = (u8*)temp_buffer.ptr;

        while (left_ptr < left_end && right_ptr < right_end) {
            if (comp(left_ptr, right_ptr, arg) <= 0) {
                memcpy(temp_ptr, left_ptr, size);
                left_ptr += size;
            } else {
                memcpy(temp_ptr, right_ptr, size);
                right_ptr += size;
            }
            temp_ptr += size;
        }

        // Copy remaining elements
        if (left_ptr < left_end) {
            usize bytes_left = left_end - left_ptr;
            memcpy(temp_ptr, left_ptr, bytes_left);
            temp_ptr += bytes_left;
        }
        if (right_ptr < right_end) {
            usize bytes_right = right_end - right_ptr;
            memcpy(temp_ptr, right_ptr, bytes_right);
            temp_ptr += bytes_right;
        }

        // Copy merged elements back to the original array
        usize total_bytes = temp_ptr - (u8*)temp_buffer.ptr;
        memcpy(base_bytes, temp_buffer.ptr, total_bytes);

        return_void();
    }
    scope_returnReserved;
}
// Modernized stable sort (using merge sort)
static $must_check E$void stableSort(
    P$raw base,
    usize num,
    usize size,
    cmp_Ord (*comp)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P$raw         arg,
    mem_Allocator allocator
) {
    scope_reserveReturn(E$void) {
        let checked_size = try_(mulSafe(num, size));
        let temp_buffer  = meta_cast$(S$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
        defer_(mem_Allocator_free(allocator, anySli(temp_buffer)));

        try_(mergeSortWithTmpRecur(base, num, size, comp, arg, temp_buffer));
        return_void();
    }
    scope_returnReserved;
}

/*========== Helper Functions ===============================================*/

static mem_Allocator testAllocator(void) {
    static O$mem_Allocator allocator = none();
    if_none(allocator) {
        toSome(allocator, heap_Page_allocator(&(heap_Page){}));
    }
    return unwrap(allocator);
}

/*========== Benchmarking Functions =========================================*/

#define BENCHMARK(NAME, FUNC, ITER, WORK_ARR, SRC_ARR, ARR_LEN, ALLOCATOR) \
    printf("%-20s | %5d elements | %4d iterations\n", NAME, (i32)(ARR_LEN), ITER); \
    do { \
        clock_t       overhead_time = 0; \
        const clock_t begin         = clock(); \
        for (i32 i = 0; i < (ITER); ++i) { \
            const clock_t overhead_begin = clock(); \
            memcpy(WORK_ARR, SRC_ARR, (ARR_LEN) * sizeof(i32)); \
            overhead_time += (clock() - overhead_begin); \
            let_ignore = BENCHMARK_##FUNC(WORK_ARR, ARR_LEN, ALLOCATOR); \
        } \
        const clock_t end      = clock(); \
        const f64     total_ms = ((f64)(end - begin - overhead_time) * 1000.0) / CLOCKS_PER_SEC; \
        const f64     avg_ms   = total_ms / (ITER); \
        printf("  %-18s: %.3f ms (avg = %.3f ms)\n", NAME, total_ms, avg_ms); \
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

static $inline cmp_Ord stableSort_compareInts(const void* lhs, const void* rhs, const void* arg) {
    $unused(arg);
    return prim_cmp(*as$(i32*, lhs), *as$(i32*, rhs));
}

static void generateRandData(i32* ptr, usize len) {
    for (usize i = 0; i < len; ++i) {
        ptr[i] = Rand_i32() % 10000;
    }
}

must_check E$void benchmark(usize sample_data_count, i32 iterations) {
    scope_reserveReturn(E$void) {
        var allocator = testAllocator();

        let data = meta_cast$(S$i32, try_(mem_Allocator_alloc(allocator, typeInfo$(i32), sample_data_count)));
        defer_(mem_Allocator_free(allocator, anySli(data)));
        generateRandData(data.ptr, data.len);

        // Make copies for fair comparison
        let qsort_data = meta_cast$(S$i32, try_(mem_Allocator_alloc(allocator, typeInfo$(i32), sample_data_count)));
        defer_(mem_Allocator_free(allocator, anySli(qsort_data)));
        memcpy(qsort_data.ptr, data.ptr, data.len * sizeof(i32));

        let stable_sort_data = meta_cast$(S$i32, try_(mem_Allocator_alloc(allocator, typeInfo$(i32), sample_data_count)));
        defer_(mem_Allocator_free(allocator, anySli(stable_sort_data)));
        memcpy(stable_sort_data.ptr, data.ptr, data.len * sizeof(i32));

        // Benchmark qsort
        BENCHMARK("(std)qsort", qsort, iterations,
                  qsort_data.ptr, // destination (work array)
                  data.ptr,       // source
                  data.len, allocator);

        // Benchmark stableSort
        BENCHMARK("(dh)stableSort", stableSort, iterations,
                  stable_sort_data.ptr, // destination (work array)
                  data.ptr,             // source
                  data.len, allocator);

        return_void();
    }
    scope_returnReserved;
}

/*========== Benchmark Runner ===============================================*/

int main(void) {
    const i32 iterations = 100;
    const i32 seed       = 12345;
    Rand_initWithSeed(seed); // fixed seed => reproducible random data

    printf("=== C `(dh)stableSort` vs C `(std)qsort` Benchmark ===\n");

    printf("\n=== Benchmark Parameters ===\n");
    printf("  Iterations: %d\n", iterations);
    printf("  Seed:       %d\n", seed);

    printf("\nbenchmarking...\n");

    printf("\n-- 1,000 elements --\n");
    catch_from(benchmark(1000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 10,000 elements --\n");
    catch_from(benchmark(10000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 100,000 elements --\n");
    catch_from(benchmark(100000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 1,000,000 elements --\n");
    catch_from(benchmark(1000000, iterations), err, {
        exit(err.ctx);
    });

    printf("\ndone.\n");

    let_ignore = getchar();
    return 0;
}
