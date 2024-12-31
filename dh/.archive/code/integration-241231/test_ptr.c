// build `clang -o test_ptr -xc -std=c17 test_ptr.c ../../src/*.c -static -g  -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

/**
 * @file ptr_align.h
 * Cross-platform aligned memory allocation
 */
#ifndef PTR_ALIGN_H
#define PTR_ALIGN_H

#include "dh/heap/Classic.h"
#include "dh/mem/Allocator.h"
#include "dh/time/SysTime.h"

#if defined(_WIN32)
#include <malloc.h>
#define ptr_aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#define ptr_aligned_free(ptr)              _aligned_free(ptr)
#elif defined(__APPLE__)
#include <stdlib.h>
static inline void* ptr_aligned_alloc(size_t alignment, size_t size) {
    void* ptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}
#define ptr_aligned_free(ptr) free(ptr)
#else // Linux and others
#include <stdlib.h>
#define ptr_aligned_alloc(alignment, size) aligned_alloc(alignment, size)
#define ptr_aligned_free(ptr)              free(ptr)
#endif

#endif // PTR_ALIGN_H

/**
 * @file ptr_perf_test.c
 * Performance tests for Ptr implementation
 */
#include "dh/ext_types.h"
#include "dh/mem.h"
#include "dh/time.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*========== Timing Utilities =======================================*/

typedef struct {
    const char* name;
    usize       cycles;
    usize       ns;
    usize       iterations;
} PerfResult;

static usize rdtsc(void) {
#if defined(__x86_64__)
    unsigned int lo = 0;
    unsigned int hi = 0;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi)); // NOLINT
    return ((usize)hi << 32) | lo;
#elif defined(__aarch64__)
    usize val;
    __asm__ __volatile__("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#else
    return clock();
#endif
}

static usize get_ns(void) {
    return time_SysTime_now().QuadPart;
}

#define MEASURE(result, iterations, code)                \
    do {                                                 \
        usize start_cycles = rdtsc();                    \
        usize start_ns     = get_ns();                   \
        for (usize i = 0; i < (iterations); i++) {       \
            code;                                        \
        }                                                \
        usize end_cycles    = rdtsc();                   \
        usize end_ns        = get_ns();                  \
        (result).cycles     = end_cycles - start_cycles; \
        (result).ns         = end_ns - start_ns;         \
        (result).iterations = iterations;                \
    } while (0)

/*========== Test Cases ===========================================*/

// Basic operations test
static PerfResult test_basic_ops(usize iterations) {
    PerfResult result = { "Basic Operations", 0, 0, 0 };
    int        value  = 42;

    MEASURE(result, iterations, {
        const Ptr_i32 p = Ptr_i32_from(&value);
        p.ref->value += p.ref->value + 1;
        const bool is_null = p.addr == null;
        unused(is_null);
    });

    return result;
}

static PerfResult test_raw_basic_ops(usize iterations) {
    PerfResult result = { "Raw Pointer Baseline", 0, 0, 0 };
    int        value  = 42;

    MEASURE(result, iterations, {
        int* const p       = &value;
        *p                 = *p + 1;
        const bool is_null = p == null;
        unused(is_null);
    });

    return result;
}

// // Memory barrier test
// static PerfResult test_memory_barriers(usize iterations) {
//     PerfResult result = { "Memory Barriers", 0, 0, 0 };
//     Ptr        p      = Ptr_ref(result);

//     MEASURE(result, iterations, {
//         Ptr_loadAcquire(&p);
//         Ptr_barrier();
//         Ptr_storeRelease(&p, p);
//     });

//     return result;
// }

// // Platform-specific optimizations test
// #ifdef BUILTIN_PLTF_64BIT
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
// static PerfResult test_compression(usize iterations) {
//     PerfResult result = { "Pointer Compression", 0, 0, 0 };
//     Ptr        p      = Ptr_from(&result, sizeof(result), false);

//     MEASURE(result, iterations, {
//         uintptr_t compressed   = Ptr_compress(p);
//         Ptr       decompressed = Ptr_decompress(compressed);
//         Ptr_isCanonical(decompressed);
//     });

//     return result;
// }

// #ifdef __AVX2__
// static PerfResult test_simd_ops(usize iterations) {
//     PerfResult result  = { "SIMD Operations", 0, 0, 0 };
//     void*      aligned = aligned_alloc(32, 1024);
//     Ptr        p       = Ptr_from(aligned, 1024, false);

//     MEASURE(result, iterations, {
//         Ptr_isAligned32(p);
//         Ptr_prefetch(p);
//     });

//     free(aligned);
//     return result;
// }
// #endif
// #endif
// #endif

#undef createFrom
#define createFrom(x) ((TypeOf(x)[1]){ x })

// Array access patterns
static PerfResult test_array_access(usize iterations, usize array_size) {
    PerfResult result = { "Array Access", 0, 0, 0 };

    Sli_i32 array = {
        .base = ResErr_Sli_unwrap(
            mem_Allocator_alloc(
                heap_Classic_allocator(),
                typeInfo(i32),
                array_size
            )
        )
    };

    MEASURE(result, iterations, {
        for (usize i = 0; i < array_size; ++i) {
            Sli_i32_at(array, i)->value = i;
        }
    });

    mem_Allocator_free(heap_Classic_allocator(), array.base);
    return result;
}

// Raw array access patterns
static PerfResult test_raw_array_access(usize iterations, usize array_size) {
    PerfResult result = { "Raw Array Access", 0, 0, 0 };
    let        array  = mem_Allocator_rawAlloc(heap, typeInfo(i32), array_size);
    // Ptr p             = Slice_ptr(array);

    MEASURE(result, iterations, {
        for (usize i = 0; i < array_size; ++i) {
            *((int*)(((u8*)array) + i * sizeof(int))) = (int)i;
        }
    });

    mem_freeRaw(&mem_general, (void**)&array);
    return result;
}

// // Cache effects test
// static PerfResult test_cache_effects(usize iterations) {
//     PerfResult  result     = { "Cache Effects", 0, 0, 0 };
//     const usize cache_line = 64;
//     const usize num_lines  = 1024;

//     char* data = ptr_aligned_alloc(cache_line, cache_line * num_lines);
//     if (!data) {
//         result.name = "Cache Effects (Failed - Memory allocation error)";
//         return result;
//     }

//     Ptr ptrs[num_lines];

//     // Initialize pointers to start of each cache line
//     for (usize i = 0; i < num_lines; ++i) {
//         ptrs[i] = Ptr_fromRaw(data + i * cache_line, cache_line);
//     }

//     MEASURE(result, iterations, {
//         // Access pattern to test cache behavior
//         for (usize i = 0; i < num_lines; i += 16) {
//             volatile char* ptr = (volatile char*)Ptr_raw(ptrs[i]);
//             *ptr += 1;
//         }
//     });

//     ptr_aligned_free(data);
//     return result;
// }

void compare_implementations(const PerfResult* ptr_perf, const PerfResult* raw_ptr_perf, const char* name) {
    printf("TEST: Performance Comparison (%s):\n", name);
    printf("  Iterations: %zu\n", ptr_perf->iterations);
    printf("  Ptr total cycles: %llu\n", ptr_perf->cycles);
    printf("  Ptr cycles/op: %.2f\n", (f64)ptr_perf->cycles / (f64)ptr_perf->iterations);
    printf("  Ptr total time: %.2f ms\n", (f64)ptr_perf->ns / 1000000.0);
    printf("  Ptr time/op: %.2f ns\n", (f64)ptr_perf->ns / (f64)ptr_perf->iterations);
    printf("  Raw total cycles: %llu\n", raw_ptr_perf->cycles);
    printf("  Raw cycles/op: %.2f\n", (f64)raw_ptr_perf->cycles / (f64)raw_ptr_perf->iterations);
    printf("  Raw total time: %.2f ms\n", (f64)raw_ptr_perf->ns / 1000000.0);
    printf("  Raw time/op: %.2f ns\n", (f64)raw_ptr_perf->ns / (f64)raw_ptr_perf->iterations);
    printf("  Overhead: %.2f%%\n", ((f64)ptr_perf->cycles / (f64)raw_ptr_perf->cycles - 1.0) * 100.0);
    printf("\n");
}

/*========== Results Reporting ====================================*/

static void print_result(const PerfResult* result) {
    printf("TEST: %s\n", result->name);
    printf("  Iterations: %zu\n", result->iterations);
    printf("  Total cycles: %llu\n", result->cycles);
    printf("  Cycles/op: %.2f\n", (f64)result->cycles / (f64)result->iterations);
    printf("  Total time: %.2f ms\n", (f64)result->ns / 1000000.0);
    printf("  Time/op: %.2f ns\n", (f64)result->ns / (f64)result->iterations);
    printf("\n");
}

/*========== Main Test Runner =====================================*/

int main(void) {
    const usize BASE_ITERATIONS = 10000000;

    printf("Running Ptr Performance Tests\n");
    // printf("Implementation: %s\n", CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD ? "Bitfield" : "Double");
    printf("Architecture: %s\n", sizeof(void*) == 8 ? "64-bit" : "32-bit");
    printf("\n");

    // Basic tests
    printf("Testing...\n");
    compare_implementations(
        (const PerfResult[]){ test_basic_ops(BASE_ITERATIONS) },
        (const PerfResult[]){ test_raw_basic_ops(BASE_ITERATIONS) },
        "Basic Operations"
    );
    // printf("Testing...\n");
    // print_result((const PerfResult[]){ test_memory_barriers(BASE_ITERATIONS) });

    // printf("Testing...\n");
// Platform-specific tests
#ifdef BUILTIN_PLTF_64BIT
#if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
    // print_result((const PerfResult[]){ test_compression(BASE_ITERATIONS) });
#ifdef __AVX2__
    // print_result((const PerfResult[]){ test_simd_ops(BASE_ITERATIONS) });
#endif
#endif
#endif

    // Array and cache tests
    printf("Testing...\n");
    compare_implementations(
        (const PerfResult[]){ test_array_access(BASE_ITERATIONS / 100, 1000) },
        (const PerfResult[]){ test_raw_array_access(BASE_ITERATIONS / 100, 1000) },
        "Array Access"
    );

    // printf("Testing...\n");
    // print_result((const PerfResult[]){ test_cache_effects(BASE_ITERATIONS / 100) });

    return 0;
}
