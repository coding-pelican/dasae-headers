// ============================================
// SimpleOMP v3 - Black Box + ARM Features
// 컴파일러 최적화 방지 + 확장 SIMD 감지
// ============================================

#include "simple_omp_v2.h"
#include "compiler_barrier.h"
#include "arm_simd_features.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ============================================
// 테스트 0: Feature Detection
// ============================================

void test_feature_detection() {
    printf("\n=== Test 0: Feature Detection ===\n");
    
    printf("\nCPU Information:\n");
    printf("  Cores: %d\n", somp_get_num_processors());
    printf("  Threads: %d\n", somp_get_num_threads());
    
    printf("\n");
    arm_print_features();
    
    printf("\nOptimal SIMD width: %d floats\n", arm_get_optimal_width());
}

// ============================================
// 테스트 1: Black Box 효과 확인
// ============================================

void test_black_box() {
    printf("\n=== Test 1: Black Box (Optimization Prevention) ===\n");
    
    const int N = 10000000;
    float* data = (float*)malloc(N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        data[i] = i * 0.5f;
    }
    
    printf("\nWithout Black Box:\n");
    double start = get_precise_time();
    for (int i = 0; i < N; i++) {
        data[i] = data[i] * 2.0f;
    }
    double time_no = get_precise_time() - start;
    printf("  Time: %.6f ms\n", time_no * 1000);
    
    printf("\nWith Black Box:\n");
    start = get_precise_time();
    for (int i = 0; i < N; i++) {
        data[i] = data[i] * 2.0f;
    }
    BLACK_BOX_MEM(data, N * sizeof(float));
    double time_with = get_precise_time() - start;
    printf("  Time: %.6f ms\n", time_with * 1000);
    
    printf("\nVerification: data[0] = %.2f\n", data[0]);
    
    free(data);
}

// ============================================
// 테스트 2: Thread Scaling (수정됨)
// ============================================

void test_thread_scaling_fixed() {
    printf("\n=== Test 2: Thread Scaling (Fixed) ===\n");
    
    const int N = 10000000;
    const int ITERATIONS = 10;
    
    float* a = (float*)malloc(N * sizeof(float));
    float* b = (float*)malloc(N * sizeof(float));
    float* c = (float*)malloc(N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        a[i] = i * 0.5f;
        b[i] = i * 0.3f;
    }
    
    int max_threads = somp_get_max_threads();
    
    printf("\nThreads | Time (ms) | Speedup | Efficiency\n");
    printf("--------|-----------|---------|------------\n");
    
    double baseline = 0.0;
    
    for (int t = 1; t <= max_threads && t <= 16; t *= 2) {
        somp_set_num_threads(t);
        
        BENCHMARK_WARMUP({
            somp_parallel_for(0, N,
                (somp_loop_func_t)({
                    void _func(int i, void* d) {
                        float** arr = (float**)d;
                        arr[2][i] = arr[0][i] + arr[1][i];
                    }
                    _func;
                }), (void*)(float*[]){a, b, c});
        });
        
        double total_time = 0.0;
        for (int iter = 0; iter < ITERATIONS; iter++) {
            double start = get_precise_time();
            
            somp_parallel_for(0, N,
                (somp_loop_func_t)({
                    void _func(int i, void* d) {
                        float** arr = (float**)d;
                        arr[2][i] = arr[0][i] + arr[1][i];
                    }
                    _func;
                }), (void*)(float*[]){a, b, c});
            
            BLACK_BOX_MEM(c, N * sizeof(float));
            
            total_time += get_precise_time() - start;
        }
        
        double avg_time = total_time / ITERATIONS;
        
        if (t == 1) {
            baseline = avg_time;
        }
        
        double speedup = baseline / avg_time;
        double efficiency = speedup / t * 100.0;
        
        printf("%7d | %9.3f | %7.2fx | %9.1f%%\n", 
               t, avg_time * 1000, speedup, efficiency);
    }
    
    free(a);
    free(b);
    free(c);
    
    somp_set_num_threads(0);
}

// ============================================
// 테스트 3: SIMD 언롤링 비교
// ============================================

void test_simd_unrolling() {
    printf("\n=== Test 3: SIMD Unrolling ===\n");
    
    #ifdef __ARM_NEON
    const int N = 10000000;
    float* a = (float*)malloc(N * sizeof(float));
    float* b = (float*)malloc(N * sizeof(float));
    float* c = (float*)malloc(N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        a[i] = i * 0.5f;
        b[i] = i * 0.3f;
    }
    
    printf("\nMethod                | Time (ms) | Speedup\n");
    printf("----------------------|-----------|--------\n");
    
    double start = get_precise_time();
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
    BLACK_BOX_MEM(c, N * sizeof(float));
    double scalar_time = get_precise_time() - start;
    printf("Scalar                | %9.3f | 1.00x\n", scalar_time * 1000);
    
    start = get_precise_time();
    somp_vector_add(a, b, c, N);
    BLACK_BOX_MEM(c, N * sizeof(float));
    double neon_time = get_precise_time() - start;
    printf("NEON (4 floats)       | %9.3f | %.2fx\n", 
           neon_time * 1000, scalar_time / neon_time);
    
    start = get_precise_time();
    arm_vector_add_unrolled(a, b, c, N);
    BLACK_BOX_MEM(c, N * sizeof(float));
    double neon2x_time = get_precise_time() - start;
    printf("NEON 2x unroll (8)    | %9.3f | %.2fx\n", 
           neon2x_time * 1000, scalar_time / neon2x_time);
    
    start = get_precise_time();
    arm_vector_add_unrolled4x(a, b, c, N);
    BLACK_BOX_MEM(c, N * sizeof(float));
    double neon4x_time = get_precise_time() - start;
    printf("NEON 4x unroll (16)   | %9.3f | %.2fx\n", 
           neon4x_time * 1000, scalar_time / neon4x_time);
    
    free(a);
    free(b);
    free(c);
    #else
    printf("  NEON not available\n");
    #endif
}

// ============================================
// 테스트 4: 벤치마크 매크로
// ============================================

void test_benchmark_macros() {
    printf("\n=== Test 4: Benchmark Macros ===\n");
    
    const int N = 10000000;
    float* data = (float*)malloc(N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        data[i] = i * 0.5f;
    }
    
    BENCHMARK_BLOCK_BEGIN(scale_operation, data, N * sizeof(float))
        for (int i = 0; i < N; i++) {
            data[i] = data[i] * 2.0f;
        }
    BENCHMARK_BLOCK_END(scale_operation)
    
    #ifdef __ARM_NEON
    BENCHMARK_BLOCK_BEGIN(simd_scale, data, N * sizeof(float))
        int i = 0;
        float32x4_t two = vdupq_n_f32(2.0f);
        for (; i + 4 <= N; i += 4) {
            float32x4_t v = vld1q_f32(&data[i]);
            v = vmulq_f32(v, two);
            vst1q_f32(&data[i], v);
        }
        for (; i < N; i++) {
            data[i] = data[i] * 2.0f;
        }
    BENCHMARK_BLOCK_END(simd_scale)
    #endif
    
    free(data);
}

// ============================================
// 테스트 5: 종합 성능
// ============================================

void test_comprehensive() {
    printf("\n=== Test 5: Comprehensive Performance ===\n");
    printf("N = 10M floats, c = a + b, 10 iterations\n\n");
    
    const int N = 10000000;
    const int ITERS = 10;
    
    float* a = (float*)malloc(N * sizeof(float));
    float* b = (float*)malloc(N * sizeof(float));
    float* c = (float*)malloc(N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        a[i] = i * 0.5f;
        b[i] = i * 0.3f;
    }
    
    printf("Method           | Time (ms) | Speedup\n");
    printf("-----------------|-----------|--------\n");
    
    double total = 0.0;
    for (int iter = 0; iter < ITERS; iter++) {
        double start = get_precise_time();
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }
        BLACK_BOX_MEM(c, N * sizeof(float));
        total += get_precise_time() - start;
    }
    double scalar_time = total / ITERS;
    printf("Scalar           | %9.3f | 1.00x\n", scalar_time * 1000);
    
    total = 0.0;
    for (int iter = 0; iter < ITERS; iter++) {
        double start = get_precise_time();
        somp_vector_add(a, b, c, N);
        BLACK_BOX_MEM(c, N * sizeof(float));
        total += get_precise_time() - start;
    }
    double simd_time = total / ITERS;
    printf("SIMD             | %9.3f | %.2fx\n", 
           simd_time * 1000, scalar_time / simd_time);
    
    #ifdef __ARM_NEON
    total = 0.0;
    for (int iter = 0; iter < ITERS; iter++) {
        double start = get_precise_time();
        arm_vector_add_unrolled(a, b, c, N);
        BLACK_BOX_MEM(c, N * sizeof(float));
        total += get_precise_time() - start;
    }
    double simd2x_time = total / ITERS;
    printf("SIMD 2x unroll   | %9.3f | %.2fx\n", 
           simd2x_time * 1000, scalar_time / simd2x_time);
    
    total = 0.0;
    for (int iter = 0; iter < ITERS; iter++) {
        double start = get_precise_time();
        arm_vector_add_unrolled4x(a, b, c, N);
        BLACK_BOX_MEM(c, N * sizeof(float));
        total += get_precise_time() - start;
    }
    double simd4x_time = total / ITERS;
    printf("SIMD 4x unroll   | %9.3f | %.2fx\n", 
           simd4x_time * 1000, scalar_time / simd4x_time);
    #endif
    
    somp_set_num_threads(0);
    int threads = somp_get_num_threads();
    total = 0.0;
    for (int iter = 0; iter < ITERS; iter++) {
        double start = get_precise_time();
        somp_parallel_for(0, N,
            (somp_loop_func_t)({
                void _func(int i, void* d) {
                    float** arr = (float**)d;
                    arr[2][i] = arr[0][i] + arr[1][i];
                }
                _func;
            }), (void*)(float*[]){a, b, c});
        BLACK_BOX_MEM(c, N * sizeof(float));
        total += get_precise_time() - start;
    }
    double pthread_time = total / ITERS;
    printf("pthread (%dt)     | %9.3f | %.2fx\n", 
           threads, pthread_time * 1000, scalar_time / pthread_time);
    
    free(a);
    free(b);
    free(c);
}

// ============================================
// 메인
// ============================================

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  SimpleOMP v3 Test Suite             ║\n");
    printf("║  Black Box + ARM Features            ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    test_feature_detection();
    test_black_box();
    test_thread_scaling_fixed();
    test_simd_unrolling();
    test_benchmark_macros();
    test_comprehensive();
    
    printf("\n=== All Tests Complete ===\n");
    
    return 0;
}
