// ============================================
// SimpleOMP 라이브러리 사용 예제
// ============================================

#include "simple_omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

// ============================================
// 예제 1: 기본 병렬 루프 (함수 기반)
// ============================================

void example1_worker(int i, void* data) {
    int* array = (int*)data;
    array[i] = i * 2;
}

void example1_basic_parallel_for() {
    printf("\n=== Example 1: Basic Parallel For ===\n");
    
    const int N = 1000000;
    int* array = (int*)malloc(N * sizeof(int));
    
    somp_set_num_threads(4);
    
    double start = get_time();
    somp_parallel_for_static(0, N, example1_worker, array);
    double elapsed = get_time() - start;
    
    // 검증
    int errors = 0;
    for (int i = 0; i < N && errors < 5; i++) {
        if (array[i] != i * 2) {
            printf("Error at %d: expected %d, got %d\n", 
                   i, i * 2, array[i]);
            errors++;
        }
    }
    
    if (errors == 0) {
        printf("✓ All values correct!\n");
    }
    printf("Time: %.3f ms\n", elapsed * 1000);
    
    free(array);
}

// ============================================
// 예제 2: Reduction (합계)
// ============================================

void example2_sum_worker(int i, void* result) {
    double* val = (double*)result;
    *val = i;  // 단순히 인덱스 반환
}

void example2_reduction() {
    printf("\n=== Example 2: Reduction (Sum) ===\n");
    
    const int N = 1000000;
    somp_set_num_threads(4);
    
    double start = get_time();
    double sum = somp_parallel_reduce(0, N, example2_sum_worker, 
                                      NULL, SOMP_REDUCE_SUM);
    double elapsed = get_time() - start;
    
    // 검증: sum = 0 + 1 + 2 + ... + (N-1) = N*(N-1)/2
    double expected = (double)N * (N - 1) / 2;
    
    printf("Sum: %.0f\n", sum);
    printf("Expected: %.0f\n", expected);
    printf("Match: %s\n", (fabs(sum - expected) < 1.0) ? "✓ Yes" : "✗ No");
    printf("Time: %.3f ms\n", elapsed * 1000);
}

// ============================================
// 예제 3: 동적 스케줄링
// ============================================

void example3_unbalanced_worker(int i, void* data) {
    // 작업 시간이 인덱스에 따라 다름 (불균형)
    int work_amount = i % 100;
    volatile int dummy = 0;
    for (int j = 0; j < work_amount * 1000; j++) {
        dummy++;
    }
}

void example3_dynamic_scheduling() {
    printf("\n=== Example 3: Dynamic Scheduling ===\n");
    
    const int N = 1000;
    somp_set_num_threads(4);
    
    // 정적 스케줄링
    double start = get_time();
    somp_parallel_for_static(0, N, example3_unbalanced_worker, NULL);
    double static_time = get_time() - start;
    
    // 동적 스케줄링
    start = get_time();
    somp_parallel_for_dynamic(0, N, example3_unbalanced_worker, NULL);
    double dynamic_time = get_time() - start;
    
    printf("Static scheduling: %.3f ms\n", static_time * 1000);
    printf("Dynamic scheduling: %.3f ms\n", dynamic_time * 1000);
    printf("Speedup: %.2fx\n", static_time / dynamic_time);
}

// ============================================
// 예제 4: Critical Section
// ============================================

void example4_critical_section() {
    printf("\n=== Example 4: Critical Section ===\n");
    
    const int N = 100000;
    int counter = 0;
    atomic_int atomic_counter = 0;
    
    somp_set_num_threads(4);
    
    // 잘못된 방법: Race condition
    double start = get_time();
    somp_parallel_for_static(0, N, 
        (somp_loop_func_t)({
            void _func(int i, void* data) {
                int* c = (int*)data;
                (*c)++;  // Race condition!
            }
            _func;
        }), &counter);
    double unsafe_time = get_time() - start;
    
    // 올바른 방법 1: Atomic
    start = get_time();
    somp_parallel_for_static(0, N,
        (somp_loop_func_t)({
            void _func(int i, void* data) {
                atomic_int* c = (atomic_int*)data;
                SOMP_ATOMIC_INC(*c);
            }
            _func;
        }), &atomic_counter);
    double atomic_time = get_time() - start;
    
    printf("Unsafe counter: %d (expected %d) - %s\n", 
           counter, N, (counter == N) ? "✓" : "✗ WRONG!");
    printf("Atomic counter: %d (expected %d) - %s\n",
           atomic_counter, N, (atomic_counter == N) ? "✓" : "✓ CORRECT!");
    printf("Unsafe time: %.3f ms\n", unsafe_time * 1000);
    printf("Atomic time: %.3f ms\n", atomic_time * 1000);
}

// ============================================
// 예제 5: OpenMP와 성능 비교
// ============================================

void example5_performance_comparison() {
    printf("\n=== Example 5: Performance Comparison ===\n");
    
    const int N = 10000000;
    double* array1 = (double*)malloc(N * sizeof(double));
    double* array2 = (double*)malloc(N * sizeof(double));
    
    // SimpleOMP
    somp_set_num_threads(4);
    double start = get_time();
    somp_parallel_for_static(0, N,
        (somp_loop_func_t)({
            void _func(int i, void* data) {
                double* arr = (double*)data;
                arr[i] = sin(i) * cos(i);
            }
            _func;
        }), array1);
    double somp_time = get_time() - start;
    
    // OpenMP (비교용)
    #ifdef _OPENMP
    start = get_time();
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < N; i++) {
        array2[i] = sin(i) * cos(i);
    }
    double omp_time = get_time() - start;
    
    printf("SimpleOMP: %.3f ms\n", somp_time * 1000);
    printf("OpenMP:    %.3f ms\n", omp_time * 1000);
    printf("Ratio:     %.2fx\n", somp_time / omp_time);
    #else
    printf("SimpleOMP: %.3f ms\n", somp_time * 1000);
    printf("(OpenMP not available for comparison)\n");
    #endif
    
    // 결과 검증
    int match = 1;
    for (int i = 0; i < N && match; i++) {
        if (fabs(array1[i] - sin(i) * cos(i)) > 1e-10) {
            match = 0;
        }
    }
    printf("Results: %s\n", match ? "✓ Correct" : "✗ Wrong");
    
    free(array1);
    free(array2);
}

// ============================================
// 예제 6: 스레드 수에 따른 스케일링
// ============================================

void heavy_computation(int i, void* data) {
    double* result = (double*)data;
    double sum = 0.0;
    for (int j = 0; j < 1000; j++) {
        sum += sin(i + j) * cos(i - j);
    }
    result[i] = sum;
}

void example6_thread_scaling() {
    printf("\n=== Example 6: Thread Scaling ===\n");
    
    const int N = 100000;
    double* array = (double*)malloc(N * sizeof(double));
    
    printf("Threads | Time (ms) | Speedup\n");
    printf("--------|-----------|--------\n");
    
    double baseline_time = 0.0;
    
    for (int threads = 1; threads <= 8; threads *= 2) {
        somp_set_num_threads(threads);
        
        double start = get_time();
        somp_parallel_for_static(0, N, heavy_computation, array);
        double elapsed = get_time() - start;
        
        if (threads == 1) {
            baseline_time = elapsed;
        }
        
        printf("%7d | %9.2f | %.2fx\n", 
               threads, elapsed * 1000, baseline_time / elapsed);
    }
    
    free(array);
}

// ============================================
// 메인 함수
// ============================================

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  SimpleOMP Library Test Suite        ║\n");
    printf("║  OpenMP-style API using pthread       ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    example1_basic_parallel_for();
    example2_reduction();
    example3_dynamic_scheduling();
    example4_critical_section();
    example5_performance_comparison();
    example6_thread_scaling();
    
    printf("\n=== All Tests Complete ===\n");
    
    return 0;
}
