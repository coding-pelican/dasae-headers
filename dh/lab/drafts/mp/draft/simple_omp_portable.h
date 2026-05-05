// ============================================
// 크로스 플랫폼 SimpleOMP (x86 + ARM 지원)
// x86: AVX2/SSE, ARM: NEON, Fallback: 스칼라
// ============================================

#ifndef SIMPLE_OMP_PORTABLE_H
#define SIMPLE_OMP_PORTABLE_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// ============================================
// 플랫폼 감지
// ============================================

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define SOMP_ARCH_X86
    #if defined(__AVX2__)
        #define SOMP_SIMD_AVX2
        #include <immintrin.h>
    #elif defined(__SSE2__)
        #define SOMP_SIMD_SSE2
        #include <emmintrin.h>
    #endif
#elif defined(__ARM_NEON) || defined(__aarch64__)
    #define SOMP_ARCH_ARM
    #define SOMP_SIMD_NEON
    #include <arm_neon.h>
#else
    #define SOMP_ARCH_GENERIC
    #define SOMP_SIMD_NONE
#endif

// ============================================
// 기본 설정
// ============================================

#ifndef SOMP_MAX_THREADS
#define SOMP_MAX_THREADS 16
#endif

static int global_num_threads = 4;

void somp_set_num_threads(int n) {
    if (n > 0 && n <= SOMP_MAX_THREADS) {
        global_num_threads = n;
    }
}

int somp_get_num_threads(void) {
    return global_num_threads;
}

// ============================================
// Parallel For 구현
// ============================================

typedef void (*somp_loop_func_t)(int, void*);

typedef struct {
    int start;
    int end;
    somp_loop_func_t func;
    void* user_data;
} somp_thread_data_t;

static void* somp_worker(void* arg) {
    somp_thread_data_t* data = (somp_thread_data_t*)arg;
    for (int i = data->start; i < data->end; i++) {
        data->func(i, data->user_data);
    }
    return NULL;
}

void somp_parallel_for(int start, int end, 
                       somp_loop_func_t func, 
                       void* user_data) {
    int num_threads = global_num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    
    int chunk = (end - start + num_threads - 1) / num_threads;
    
    for (int t = 0; t < num_threads; t++) {
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > end) ? 
                      end : data[t].start + chunk;
        data[t].func = func;
        data[t].user_data = user_data;
        
        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, somp_worker, &data[t]);
        }
    }
    
    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < end) {
            pthread_join(threads[t], NULL);
        }
    }
}

// ============================================
// SIMD 벡터 연산 (플랫폼별)
// ============================================

// 벡터 덧셈: c = a + b
void somp_vector_add(float* a, float* b, float* c, int n) {
    #if defined(SOMP_SIMD_AVX2)
        // x86 AVX2 (8 floats)
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256 va = _mm256_loadu_ps(&a[i]);
            __m256 vb = _mm256_loadu_ps(&b[i]);
            __m256 vc = _mm256_add_ps(va, vb);
            _mm256_storeu_ps(&c[i], vc);
        }
        // 나머지
        for (; i < n; i++) {
            c[i] = a[i] + b[i];
        }
        
    #elif defined(SOMP_SIMD_NEON)
        // ARM NEON (4 floats)
        int i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(&a[i]);
            float32x4_t vb = vld1q_f32(&b[i]);
            float32x4_t vc = vaddq_f32(va, vb);
            vst1q_f32(&c[i], vc);
        }
        // 나머지
        for (; i < n; i++) {
            c[i] = a[i] + b[i];
        }
        
    #else
        // 스칼라 fallback
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
        }
    #endif
}

// 벡터 곱셈: c = a * b
void somp_vector_mul(float* a, float* b, float* c, int n) {
    #if defined(SOMP_SIMD_AVX2)
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256 va = _mm256_loadu_ps(&a[i]);
            __m256 vb = _mm256_loadu_ps(&b[i]);
            __m256 vc = _mm256_mul_ps(va, vb);
            _mm256_storeu_ps(&c[i], vc);
        }
        for (; i < n; i++) {
            c[i] = a[i] * b[i];
        }
        
    #elif defined(SOMP_SIMD_NEON)
        int i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(&a[i]);
            float32x4_t vb = vld1q_f32(&b[i]);
            float32x4_t vc = vmulq_f32(va, vb);
            vst1q_f32(&c[i], vc);
        }
        for (; i < n; i++) {
            c[i] = a[i] * b[i];
        }
        
    #else
        for (int i = 0; i < n; i++) {
            c[i] = a[i] * b[i];
        }
    #endif
}

// 스칼라 곱셈: b = a * scalar
void somp_vector_scale(float* a, float scalar, float* b, int n) {
    #if defined(SOMP_SIMD_AVX2)
        __m256 vs = _mm256_set1_ps(scalar);
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256 va = _mm256_loadu_ps(&a[i]);
            __m256 vr = _mm256_mul_ps(va, vs);
            _mm256_storeu_ps(&b[i], vr);
        }
        for (; i < n; i++) {
            b[i] = a[i] * scalar;
        }
        
    #elif defined(SOMP_SIMD_NEON)
        float32x4_t vs = vdupq_n_f32(scalar);
        int i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(&a[i]);
            float32x4_t vr = vmulq_f32(va, vs);
            vst1q_f32(&b[i], vr);
        }
        for (; i < n; i++) {
            b[i] = a[i] * scalar;
        }
        
    #else
        for (int i = 0; i < n; i++) {
            b[i] = a[i] * scalar;
        }
    #endif
}

// FMA: d = a * b + c
void somp_vector_fma(float* a, float* b, float* c, float* d, int n) {
    #if defined(SOMP_SIMD_AVX2) && defined(__FMA__)
        int i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256 va = _mm256_loadu_ps(&a[i]);
            __m256 vb = _mm256_loadu_ps(&b[i]);
            __m256 vc = _mm256_loadu_ps(&c[i]);
            __m256 vr = _mm256_fmadd_ps(va, vb, vc);
            _mm256_storeu_ps(&d[i], vr);
        }
        for (; i < n; i++) {
            d[i] = a[i] * b[i] + c[i];
        }
        
    #elif defined(SOMP_SIMD_NEON)
        int i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(&a[i]);
            float32x4_t vb = vld1q_f32(&b[i]);
            float32x4_t vc = vld1q_f32(&c[i]);
            float32x4_t vr = vmlaq_f32(vc, va, vb);  // c + a * b
            vst1q_f32(&d[i], vr);
        }
        for (; i < n; i++) {
            d[i] = a[i] * b[i] + c[i];
        }
        
    #else
        for (int i = 0; i < n; i++) {
            d[i] = a[i] * b[i] + c[i];
        }
    #endif
}

// ============================================
// 플랫폼 정보 출력
// ============================================

void somp_print_info(void) {
    printf("SimpleOMP Portable Configuration:\n");
    
    #if defined(SOMP_ARCH_X86)
        printf("  Architecture: x86_64\n");
        #if defined(SOMP_SIMD_AVX2)
            printf("  SIMD: AVX2 (8 floats)\n");
        #elif defined(SOMP_SIMD_SSE2)
            printf("  SIMD: SSE2 (4 floats)\n");
        #else
            printf("  SIMD: None (scalar)\n");
        #endif
    #elif defined(SOMP_ARCH_ARM)
        printf("  Architecture: ARM\n");
        #if defined(SOMP_SIMD_NEON)
            printf("  SIMD: NEON (4 floats)\n");
        #else
            printf("  SIMD: None (scalar)\n");
        #endif
    #else
        printf("  Architecture: Generic\n");
        printf("  SIMD: None (scalar)\n");
    #endif
    
    printf("  Threads: %d\n", global_num_threads);
}

// ============================================
// Atomic 연산 (플랫폼 독립적)
// ============================================

#define SOMP_ATOMIC_ADD(var, val) atomic_fetch_add(&(var), (val))
#define SOMP_ATOMIC_INC(var) atomic_fetch_add(&(var), 1)
#define SOMP_ATOMIC_READ(var) atomic_load(&(var))
#define SOMP_ATOMIC_WRITE(var, val) atomic_store(&(var), (val))

#endif // SIMPLE_OMP_PORTABLE_H
