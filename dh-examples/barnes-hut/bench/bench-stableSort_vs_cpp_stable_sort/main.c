#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/Random.h"

#include <time.h>

#include "benchmark_cpp_stable_sort.hpp"

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
static must_check Err$void mergeSortWithTmpRecur( // NOLINT
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
static must_check Err$void stableSort(
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
        arr[i].value = Random_i32();
        arr[i].seq   = (i32)i;
    }
}

/*========== Benchmark Implementation =======================================*/

BenchResult benchmark_stableSort(BenchParams params) {
    var allocator = testAllocator();

    // Copy test data
    let c_data = meta_cast$(
        Sli$TestElem, catch (mem_Allocator_alloc(allocator, typeInfo(TestElem), params.len), err, {
            exit(Err_type(err));
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
    let stable = isStable(c_data.ptr, c_data.len);
    mem_Allocator_free(allocator, anySli(c_data));

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
