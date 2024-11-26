// bench_perf_ptr.c

#include "dh/core/Ptr.h"

#include <time.h>
#include <stdio.h>

// By value
static_inline anyptr pass_by_value(Ptr p) {
    return intToRaw(anyptr, p.bits & ((1ULL << 48) - 1));
}

// By pointer
static_inline anyptr pass_by_pointer(Ptr* p) {
    return intToRaw(anyptr, p->bits & ((1ULL << 48) - 1));
}

// Test function
void benchmark(usize iterations) {
    // NOLINTBEGIN(hicpp-no-assembler)
    Ptr test_ptr       = cleared();
    test_ptr.raw       = intToRaw(anyptr, 0x1234567890);
    test_ptr.meta.size = 42;

    clock_t start  = 0;
    clock_t end    = 0;
    anyptr  result = null;

    // Test pass by value
    start = clock();
    for (usize i = 0; i < iterations; ++i) {
        result = pass_by_value(test_ptr);
        __asm__ volatile("" : : "r"(result) : "memory"); // Prevent optimization
    }
    end            = clock();
    f64 value_time = as(f64, end - start) / CLOCKS_PER_SEC;

    // Test pass by pointer
    start = clock();
    for (usize i = 0; i < iterations; ++i) {
        result = pass_by_pointer(&test_ptr);
        __asm__ volatile("" : : "r"(result) : "memory"); // Prevent optimization
    }
    end              = clock();
    f64 pointer_time = as(f64, end - start) / CLOCKS_PER_SEC;

    printf("Pass by value time: %lg seconds\n", value_time);
    printf("Pass by pointer time: %lg seconds\n", pointer_time);
    printf("Ratio (pointer/value): %lg\n", pointer_time / value_time);
    // NOLINTEND(hicpp-no-assembler)
}

int main(void) {
    benchmark(100000000);
    return 0;
}
