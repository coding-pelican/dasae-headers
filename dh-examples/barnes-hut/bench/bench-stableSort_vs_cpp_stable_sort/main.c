#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/Random.h"

#include <time.h>

#include "benchmark_cpp_stable_sort.hpp"

#define INSERTION_THRESHOLD 32

/*========== Test Targets ===================================================*/

// Helper function to perform a safe multiplication, avoiding potential overflow
use_ErrSet(MulErr, Overflow);
force_inline Err$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(Err$usize);
    if (0 < lhs && usize_limit / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulErr_err(MulErrCode_Overflow));
    }
    return_ok(lhs * rhs);
}
// Swap two elements of given size
force_inline must_check Err$void swapElements(u8* a, u8* b, usize size) {
    scope_reserveReturn(Err$void) {
        u8* temp = alloca(size);
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
        return_void();
    }
    scope_returnReserved;
}
// Insertion sort for small arrays
static must_check Err$void insertionSort(
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const, anyptr_const, anyptr_const),
    anyptr_const arg
) {
    scope_reserveReturn(Err$void) {
        u8* base_ptr = (u8*)base;
        for (usize i = 1; i < num; ++i) {
            u8*   current = base_ptr + i * size;
            usize j       = i;
            while (j > 0) {
                u8* prev = current - size;
                if (comp(prev, current, arg) <= 0) {
                    break;
                }
                try(swapElements(prev, current, size));
                current = prev;
                j--;
            }
        }
        return_void();
    }
    scope_returnReserved;
}
// Modernized merge sort with temporary buffer (stable sort)
static must_check Err$void mergeSortWithTmpRecur( // NOLINT
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr_const arg,
    Sli$u8       temp_buffer
) {
    scope_reserveReturn(Err$void) {
        if (num <= INSERTION_THRESHOLD) {
            return insertionSort(base, num, size, comp, arg);
        }

        u8*   base_bytes = (u8*)base;
        usize mid        = num / 2;

        // Sort each half recursively
        try(mergeSortWithTmpRecur(base_bytes, mid, size, comp, arg, temp_buffer));
        try(mergeSortWithTmpRecur(base_bytes + mid * size, num - mid, size, comp, arg, temp_buffer));

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
static must_check Err$void stableSort(
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr        arg,
    mem_Allocator allocator
) {
    scope_reserveReturn(Err$void) {
        let checked_size = try(mulSafe(num, size));
        let temp_buffer  = meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer)));

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

use_Sli$(TestElem);
force_inline cmp_fnCmp(TestElem) {
    if (self.value < other.value) { return cmp_Ord_lt; }
    if (self.value > other.value) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
force_inline cmp_Ord compareTestElem(anyptr_const lhs, anyptr_const rhs, anyptr_const arg) {
    unused(arg);
    return TestElem_cmp(*as$(const TestElem*, lhs), *as$(const TestElem*, rhs));
}
static bool isSorted(const TestElem* arr, usize size) {
    for (usize i = 1; i < size; ++i) {
        if (arr[i].value < arr[i - 1].value) {
            return false;
        }
    }
    return true;
}
static bool isStable(const TestElem* arr, usize size) {
    for (usize i = 1; i < size; ++i) {
        if (arr[i].value == arr[i - 1].value && arr[i].seq <= arr[i - 1].seq) {
            return false;
        }
    }
    return true;
}
static void generateRandomTestElemArray(TestElem* arr, usize size) {
    for (usize i = 0; i < size; ++i) {
        arr[i].value = Random_i32() % 10000;
        arr[i].seq   = (i32)i;
    }
}

/*========== Benchmark Implementation =======================================*/

BenchResult benchmark_stableSort(BenchParams params) {
    var allocator = testAllocator();

    // Copy test data
    let c_data = meta_cast$(
        Sli$TestElem, catch (mem_Allocator_alloc(allocator, typeInfo(TestElem), params.len), err, {
            exit(err.ctx);
        })
    );
    memcpy(c_data.ptr, params.ptr, params.len * sizeof(TestElem));

    // Benchmark (dh)stableSort
    clock_t overhead_time = 0;
    let     begin         = clock();
    for (usize i = 0; i < params.iterations; ++i) {
        let overhead_begin = clock();
        memcpy(c_data.ptr, params.ptr, params.len * sizeof(TestElem));
        overhead_time += (clock() - overhead_begin);
        ignore stableSort(c_data.ptr, c_data.len, sizeof(TestElem), compareTestElem, nullptr, allocator);
    }
    let end      = clock();
    let total_ms = (as$(f64, end - begin - overhead_time) * 1000.0) / CLOCKS_PER_SEC;
    let avg_ms   = total_ms / as$(f64, params.iterations);

    // Verify stability for implementations
    let sorted = isSorted(c_data.ptr, c_data.len);
    let stable = isStable(c_data.ptr, c_data.len);
    mem_Allocator_free(allocator, anySli(c_data));
    if (!sorted) {
        exit(1);
    }

    return (BenchResult){
        .name     = "(dh)stableSort",
        .total_ms = total_ms,
        .avg_ms   = avg_ms,
        .stable   = stable
    };
}

must_check Err$void benchmark(usize sample_data_count, i32 iterations) {
    scope_reserveReturn(Err$void) {
        var allocator = testAllocator();

        let data = meta_cast$(Sli$TestElem, try(mem_Allocator_alloc(allocator, typeInfo(TestElem), sample_data_count)));
        defer(mem_Allocator_free(allocator, anySli(data)));
        generateRandomTestElemArray(data.ptr, data.len);

        let params = (BenchParams){
            .ptr        = data.ptr,
            .len        = data.len,
            .iterations = iterations,
        };

        // Benchmark std::stable_sort
        let std_stable_sort = benchmark_cpp_stable_sort(params);
        let dh_stable_sort  = benchmark_stableSort(params);

        // Print results
        printf("%-20s | %5d elements | %4d iterations\n", std_stable_sort.name, (i32)params.len, (i32)params.iterations);
        printf("  %-18s: %.3f ms (avg = %.3f ms)\n", std_stable_sort.name, std_stable_sort.total_ms, std_stable_sort.avg_ms);

        printf("%-20s | %5d elements | %4d iterations\n", dh_stable_sort.name, (i32)params.len, (i32)params.iterations);
        printf("  %-18s: %.3f ms (avg = %.3f ms)\n", dh_stable_sort.name, dh_stable_sort.total_ms, dh_stable_sort.avg_ms);

        printf("  Stability: %s | %s\n", std_stable_sort.stable ? "PASS" : "FAIL", dh_stable_sort.stable ? "PASS" : "FAIL");
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

    printf("\n=== Benchmark Parameters ===\n");
    printf("  Iterations: %d\n", iterations);
    printf("  Seed:       %d\n", seed);

    printf("\nbenchmarking...\n");

    printf("\n-- 1,000 elements --\n");
    catch (benchmark(1000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 10,000 elements --\n");
    catch (benchmark(10000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 100,000 elements --\n");
    catch (benchmark(100000, iterations), err, {
        exit(err.ctx);
    });
    printf("\n-- 1,000,000 elements --\n");
    catch (benchmark(1000000, iterations), err, {
        exit(err.ctx);
    });

    printf("\ndone.\n");

    ignore getchar();
    return 0;
}
