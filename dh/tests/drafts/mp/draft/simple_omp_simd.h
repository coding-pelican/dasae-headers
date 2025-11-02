// ============================================
// SimpleOMP + SIMD: SIMD 가속 지원 추가
// ============================================

#ifndef SIMPLE_OMP_SIMD_H
#define SIMPLE_OMP_SIMD_H

#include "simple_omp.h"
#include <immintrin.h>  // AVX/AVX2

// ============================================
// SIMD 병렬 루프 (AVX2 - 8 floats)
// ============================================

typedef void (*somp_simd_func_t)(int, __m256*, void*);

typedef struct {
    int thread_id;
    int start;
    int end;
    float* data;
    somp_simd_func_t func;
    void* user_data;
} somp_simd_thread_data_t;

static void* somp_simd_worker(void* arg) {
    somp_simd_thread_data_t* data = (somp_simd_thread_data_t*)arg;
    
    // 8개씩 처리 (AVX2)
    for (int i = data->start; i < data->end; i += 8) {
        __m256 vec = _mm256_load_ps(&data->data[i]);
        data->func(i, &vec, data->user_data);
        _mm256_store_ps(&data->data[i], vec);
    }
    
    return NULL;
}

void somp_parallel_for_simd(int start, int end,
                            float* array,
                            somp_simd_func_t func,
                            void* user_data) {
    // 끝을 8의 배수로 정렬
    int aligned_end = (end / 8) * 8;
    
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_simd_thread_data_t data[SOMP_MAX_THREADS];
    
    int chunk = ((aligned_end - start) / 8 + num_threads - 1) / num_threads * 8;
    
    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > aligned_end) ? 
                      aligned_end : data[t].start + chunk;
        data[t].data = array;
        data[t].func = func;
        data[t].user_data = user_data;
        
        if (data[t].start < aligned_end) {
            pthread_create(&threads[t], NULL, 
                          somp_simd_worker, &data[t]);
        }
    }
    
    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < aligned_end) {
            pthread_join(threads[t], NULL);
        }
    }
    
    // 나머지 처리 (정렬되지 않은 부분)
    for (int i = aligned_end; i < end; i++) {
        // 스칼라 처리
        __m256 vec = _mm256_set1_ps(array[i]);
        func(i, &vec, user_data);
        array[i] = vec[0];
    }
}

// ============================================
// 일반적인 SIMD 연산 헬퍼
// ============================================

// 벡터 덧셈: c = a + b
void somp_simd_add(float* a, float* b, float* c, int n) {
    somp_parallel_for_simd(0, n, a,
        (somp_simd_func_t)({
            void _func(int i, __m256* vec, void* data) {
                float** arrays = (float**)data;
                __m256 va = *vec;
                __m256 vb = _mm256_load_ps(&arrays[0][i]);
                *vec = _mm256_add_ps(va, vb);
            }
            _func;
        }), (void*)(float*[]){b, c});
}

// 벡터 곱셈: c = a * b
void somp_simd_mul(float* a, float* b, float* c, int n) {
    somp_parallel_for_simd(0, n, a,
        (somp_simd_func_t)({
            void _func(int i, __m256* vec, void* data) {
                float** arrays = (float**)data;
                __m256 va = *vec;
                __m256 vb = _mm256_load_ps(&arrays[0][i]);
                *vec = _mm256_mul_ps(va, vb);
            }
            _func;
        }), (void*)(float*[]){b, c});
}

// 스칼라 곱셈: b = a * scalar
void somp_simd_scale(float* a, float scalar, float* b, int n) {
    __m256 vscalar = _mm256_set1_ps(scalar);
    
    somp_parallel_for_simd(0, n, a,
        (somp_simd_func_t)({
            void _func(int i, __m256* vec, void* data) {
                __m256 vs = *((__m256*)data);
                *vec = _mm256_mul_ps(*vec, vs);
            }
            _func;
        }), &vscalar);
}

// FMA (Fused Multiply-Add): d = a * b + c
void somp_simd_fma(float* a, float* b, float* c, float* d, int n) {
    somp_parallel_for_simd(0, n, a,
        (somp_simd_func_t)({
            void _func(int i, __m256* vec, void* data) {
                float** arrays = (float**)data;
                __m256 va = *vec;
                __m256 vb = _mm256_load_ps(&arrays[0][i]);
                __m256 vc = _mm256_load_ps(&arrays[1][i]);
                *vec = _mm256_fmadd_ps(va, vb, vc);
            }
            _func;
        }), (void*)(float*[]){b, c, d});
}

// ============================================
// 리덕션 (SIMD 최적화)
// ============================================

float somp_simd_sum(float* array, int n) {
    int num_threads = global_ctx.num_threads;
    float partial_sums[SOMP_MAX_THREADS] = {0};
    
    pthread_t threads[SOMP_MAX_THREADS];
    somp_simd_thread_data_t data[SOMP_MAX_THREADS];
    
    int aligned_n = (n / 8) * 8;
    int chunk = ((aligned_n / 8) + num_threads - 1) / num_threads * 8;
    
    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = t * chunk;
        data[t].end = (data[t].start + chunk > aligned_n) ? 
                      aligned_n : data[t].start + chunk;
        data[t].data = array;
        data[t].user_data = &partial_sums[t];
        
        if (data[t].start < aligned_n) {
            pthread_create(&threads[t], NULL,
                (void*(*)(void*))({
                    void* _worker(void* arg) {
                        somp_simd_thread_data_t* d = arg;
                        __m256 vsum = _mm256_setzero_ps();
                        
                        for (int i = d->start; i < d->end; i += 8) {
                            __m256 vec = _mm256_load_ps(&d->data[i]);
                            vsum = _mm256_add_ps(vsum, vec);
                        }
                        
                        // 수평 합계
                        float temp[8];
                        _mm256_store_ps(temp, vsum);
                        float sum = 0.0f;
                        for (int i = 0; i < 8; i++) {
                            sum += temp[i];
                        }
                        
                        *((float*)d->user_data) = sum;
                        return NULL;
                    }
                    _worker;
                }), &data[t]);
        }
    }
    
    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < aligned_n) {
            pthread_join(threads[t], NULL);
        }
    }
    
    // 나머지 처리
    float remainder_sum = 0.0f;
    for (int i = aligned_n; i < n; i++) {
        remainder_sum += array[i];
    }
    
    // 최종 합계
    float total_sum = remainder_sum;
    for (int t = 0; t < num_threads; t++) {
        total_sum += partial_sums[t];
    }
    
    return total_sum;
}

#endif // SIMPLE_OMP_SIMD_H
