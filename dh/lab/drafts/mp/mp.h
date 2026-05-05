/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-31 (date of creation)
 * @updated 2025-10-31 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  mp
 */

#ifndef mp__included
#define mp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Thrd.h"

/*========== Macros and Declarations ========================================*/

#define mp_arch_family_type         __comp_enum__mp_arch_family_type
#define mp_arch_family_type_unknown __comp_enum__mp_arch_family_type_unknown
#define mp_arch_family_type_x86     __comp_enum__mp_arch_family_type_x86
#define mp_arch_family_type_arm     __comp_enum__mp_arch_family_type_arm

#define mp_arch_has_avx2 __comp_bool__mp_arch_has_avx2
#define mp_arch_has_sse2 __comp_bool__mp_arch_has_sse2
#define mp_arch_has_neon __comp_bool__mp_arch_has_neon

#define __comp_enum__mp_arch_family_type         mp_arch_family_type_unknown
#define __comp_enum__mp_arch_family_type_unknown 0
#define __comp_enum__mp_arch_family_type_x86     1
#define __comp_enum__mp_arch_family_type_arm     2

#define __comp_bool__mp_arch_has_avx2 0
#define __comp_bool__mp_arch_has_sse2 0
#define __comp_bool__mp_arch_has_neon 0

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#undef __comp_enum__mp_arch_family_type
#define __comp_enum__mp_arch_family_type mp_arch_family_type_x86
#if defined(__AVX2__)
#undef __comp_bool__mp_arch_has_avx2
#define __comp_bool__mp_arch_has_avx2 1
#include <immintrin.h>
#elif defined(__SSE2__)
#undef __comp_bool__mp_arch_has_sse2
#define __comp_bool__mp_arch_has_sse2 1
#include <emmintrin.h>
#endif
#elif defined(__ARM_NEON) || defined(__aarch64__)
#undef __comp_enum__mp_arch_family_type
#define __comp_enum__mp_arch_family_type mp_arch_family_type_arm
#undef __comp_bool__mp_arch_has_neon
#define __comp_bool__mp_arch_has_neon 1
#include <arm_neon.h>
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mp__included */





// ============================================
// SimpleOMP: OpenMP 스타일의 병렬화 라이브러리
// pthread + atomic + SIMD 기반 구현
// ============================================

#ifndef SIMPLE_OMP_H
#define SIMPLE_OMP_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// ============================================
// 1. 기본 설정
// ============================================

#ifndef SOMP_MAX_THREADS
#define SOMP_MAX_THREADS 16
#endif

typedef struct {
    int num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    atomic_int next_iteration;
    int total_iterations;
    bool initialized;
} somp_context_t;

static somp_context_t global_ctx = {
    .num_threads = 4,
    .initialized = false
};

// ============================================
// 2. 초기화 및 설정 함수
// ============================================

void somp_set_num_threads(int n) {
    if (n > 0 && n <= SOMP_MAX_THREADS) {
        global_ctx.num_threads = n;
    }
}

int somp_get_num_threads(void) {
    return global_ctx.num_threads;
}

// ============================================
// 3. Parallel For 구현
// ============================================

typedef void (*somp_loop_func_t)(int, void*);

typedef struct {
    int thread_id;
    int start;
    int end;
    int stride;
    somp_loop_func_t func;
    void* user_data;
    atomic_int* next_iter; // 동적 스케줄링용
    int total_iters;
} somp_thread_data_t;

// 정적 스케줄링 워커
static void* somp_static_worker(void* arg) {
    somp_thread_data_t* data = (somp_thread_data_t*)arg;

    for (int i = data->start; i < data->end; i += data->stride) {
        data->func(i, data->user_data);
    }

    return NULL;
}

// 동적 스케줄링 워커
static void* somp_dynamic_worker(void* arg) {
    somp_thread_data_t* data = (somp_thread_data_t*)arg;

    while (true) {
        int i = atomic_fetch_add(data->next_iter, 1);
        if (i >= data->total_iters) {
            break;
        }
        data->func(i, data->user_data);
    }

    return NULL;
}

// ============================================
// 4. 매크로 기반 API (OpenMP 스타일)
// ============================================

// 정적 스케줄링
#define SOMP_PARALLEL_FOR_STATIC(start, end, body) \
    do { \
        int _start = (start); \
        int _end = (end); \
        int _num_threads = global_ctx.num_threads; \
        pthread_t _threads[SOMP_MAX_THREADS]; \
        somp_thread_data_t _data[SOMP_MAX_THREADS]; \
        int _chunk = (_end - _start + _num_threads - 1) / _num_threads; \
\
        for (int _t = 0; _t < _num_threads; _t++) { \
            _data[_t].thread_id = _t; \
            _data[_t].start = _start + _t * _chunk; \
            _data[_t].end = (_data[_t].start + _chunk > _end) ? _end : _data[_t].start + _chunk; \
            _data[_t].stride = 1; \
            _data[_t].func = NULL; \
            _data[_t].user_data = NULL; \
\
            if (_data[_t].start < _end) { \
                pthread_create(&_threads[_t], NULL, (void* (*)(void*))({ \
                    void* _worker(void* _arg) { \
                        somp_thread_data_t* _d = _arg; \
                        for (int i = _d->start; i < _d->end; i++) { \
                            body; \
                        } \
                        return NULL; \
                    } \
                    _worker; \
                }), \
                               &_data[_t]); \
            } \
        } \
\
        for (int _t = 0; _t < _num_threads; _t++) { \
            if (_data[_t].start < _end) { \
                pthread_join(_threads[_t], NULL); \
            } \
        } \
    } while (0)

// ============================================
// 5. 함수 기반 API (더 유연함)
// ============================================

void somp_parallel_for_static(int start, int end, somp_loop_func_t func, void* user_data) {
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];

    int chunk = (end - start + num_threads - 1) / num_threads;

    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > end) ? end : data[t].start + chunk;
        data[t].stride = 1;
        data[t].func = func;
        data[t].user_data = user_data;

        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, somp_static_worker, &data[t]);
        }
    }

    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < end) {
            pthread_join(threads[t], NULL);
        }
    }
}

void somp_parallel_for_dynamic(int start, int end, somp_loop_func_t func, void* user_data) {
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    atomic_int next_iter = start;

    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].func = func;
        data[t].user_data = user_data;
        data[t].next_iter = &next_iter;
        data[t].total_iters = end;

        pthread_create(&threads[t], NULL, somp_dynamic_worker, &data[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
}

// ============================================
// 6. Reduction 지원
// ============================================

typedef enum {
    SOMP_REDUCE_SUM,
    SOMP_REDUCE_MIN,
    SOMP_REDUCE_MAX,
    SOMP_REDUCE_PROD
} somp_reduce_op_t;

typedef struct {
    somp_loop_func_t func;
    void* user_data;
    double* partial_results;
    somp_reduce_op_t op;
} somp_reduce_data_t;

static void* somp_reduce_worker(void* arg) {
    somp_thread_data_t* data = (somp_thread_data_t*)arg;
    somp_reduce_data_t* rdata = (somp_reduce_data_t*)data->user_data;

    double local_result = 0.0;

    // 각 스레드의 로컬 결과 계산
    for (int i = data->start; i < data->end; i++) {
        // 사용자 함수는 인덱스와 결과 포인터를 받음
        double val = 0.0;
        rdata->func(i, &val);

        switch (rdata->op) {
        case SOMP_REDUCE_SUM:
            local_result += val;
            break;
        case SOMP_REDUCE_MIN:
            if (i == data->start || val < local_result) {
                local_result = val;
            }
            break;
        case SOMP_REDUCE_MAX:
            if (i == data->start || val > local_result) {
                local_result = val;
            }
            break;
        case SOMP_REDUCE_PROD:
            if (i == data->start) {
                local_result = val;
            } else {
                local_result *= val;
            }
            break;
        }
    }

    rdata->partial_results[data->thread_id] = local_result;
    return NULL;
}

double somp_parallel_reduce(int start, int end, somp_loop_func_t func, void* user_data, somp_reduce_op_t op) {
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    double partial_results[SOMP_MAX_THREADS] = { 0 };

    somp_reduce_data_t rdata = {
        .func = func,
        .user_data = user_data,
        .partial_results = partial_results,
        .op = op
    };

    int chunk = (end - start + num_threads - 1) / num_threads;

    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > end) ? end : data[t].start + chunk;
        data[t].user_data = &rdata;

        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, somp_reduce_worker, &data[t]);
        }
    }

    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < end) {
            pthread_join(threads[t], NULL);
        }
    }

    // 최종 결과 합치기
    double final_result = partial_results[0];
    for (int t = 1; t < num_threads; t++) {
        switch (op) {
        case SOMP_REDUCE_SUM:
            final_result += partial_results[t];
            break;
        case SOMP_REDUCE_MIN:
            if (partial_results[t] < final_result) {
                final_result = partial_results[t];
            }
            break;
        case SOMP_REDUCE_MAX:
            if (partial_results[t] > final_result) {
                final_result = partial_results[t];
            }
            break;
        case SOMP_REDUCE_PROD:
            final_result *= partial_results[t];
            break;
        }
    }

    return final_result;
}

// ============================================
// 7. Critical Section 지원
// ============================================

static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

#define SOMP_CRITICAL_BEGIN pthread_mutex_lock(&global_lock)
#define SOMP_CRITICAL_END   pthread_mutex_unlock(&global_lock)

// ============================================
// 8. Atomic 연산 래퍼
// ============================================

#define SOMP_ATOMIC_ADD(var, val)   atomic_fetch_add(&(var), (val))
#define SOMP_ATOMIC_INC(var)        atomic_fetch_add(&(var), 1)
#define SOMP_ATOMIC_READ(var)       atomic_load(&(var))
#define SOMP_ATOMIC_WRITE(var, val) atomic_store(&(var), (val))

#endif // SIMPLE_OMP_H





// ============================================
// SimpleOMP + SIMD: SIMD 가속 지원 추가
// ============================================

#ifndef SIMPLE_OMP_SIMD_H
#define SIMPLE_OMP_SIMD_H

#include "simple_omp.h"
#include <immintrin.h> // AVX/AVX2

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

void somp_parallel_for_simd(int start, int end, float* array, somp_simd_func_t func, void* user_data) {
    // 끝을 8의 배수로 정렬
    int aligned_end = (end / 8) * 8;

    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_simd_thread_data_t data[SOMP_MAX_THREADS];

    int chunk = ((aligned_end - start) / 8 + num_threads - 1) / num_threads * 8;

    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > aligned_end) ? aligned_end : data[t].start + chunk;
        data[t].data = array;
        data[t].func = func;
        data[t].user_data = user_data;

        if (data[t].start < aligned_end) {
            pthread_create(&threads[t], NULL, somp_simd_worker, &data[t]);
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
    somp_parallel_for_simd(0, n, a, (somp_simd_func_t)({
                               void _func(int i, __m256* vec, void* data) {
                                   float** arrays = (float**)data;
                                   __m256 va = *vec;
                                   __m256 vb = _mm256_load_ps(&arrays[0][i]);
                                   *vec = _mm256_add_ps(va, vb);
                               }
                               _func;
                           }),
                           (void*)(float*[]){ b, c });
}

// 벡터 곱셈: c = a * b
void somp_simd_mul(float* a, float* b, float* c, int n) {
    somp_parallel_for_simd(0, n, a, (somp_simd_func_t)({
                               void _func(int i, __m256* vec, void* data) {
                                   float** arrays = (float**)data;
                                   __m256 va = *vec;
                                   __m256 vb = _mm256_load_ps(&arrays[0][i]);
                                   *vec = _mm256_mul_ps(va, vb);
                               }
                               _func;
                           }),
                           (void*)(float*[]){ b, c });
}

// 스칼라 곱셈: b = a * scalar
void somp_simd_scale(float* a, float scalar, float* b, int n) {
    __m256 vscalar = _mm256_set1_ps(scalar);

    somp_parallel_for_simd(0, n, a, (somp_simd_func_t)({
                               void _func(int i, __m256* vec, void* data) {
                                   __m256 vs = *((__m256*)data);
                                   *vec = _mm256_mul_ps(*vec, vs);
                               }
                               _func;
                           }),
                           &vscalar);
}

// FMA (Fused Multiply-Add): d = a * b + c
void somp_simd_fma(float* a, float* b, float* c, float* d, int n) {
    somp_parallel_for_simd(0, n, a, (somp_simd_func_t)({
                               void _func(int i, __m256* vec, void* data) {
                                   float** arrays = (float**)data;
                                   __m256 va = *vec;
                                   __m256 vb = _mm256_load_ps(&arrays[0][i]);
                                   __m256 vc = _mm256_load_ps(&arrays[1][i]);
                                   *vec = _mm256_fmadd_ps(va, vb, vc);
                               }
                               _func;
                           }),
                           (void*)(float*[]){ b, c, d });
}

// ============================================
// 리덕션 (SIMD 최적화)
// ============================================

float somp_simd_sum(float* array, int n) {
    int num_threads = global_ctx.num_threads;
    float partial_sums[SOMP_MAX_THREADS] = { 0 };

    pthread_t threads[SOMP_MAX_THREADS];
    somp_simd_thread_data_t data[SOMP_MAX_THREADS];

    int aligned_n = (n / 8) * 8;
    int chunk = ((aligned_n / 8) + num_threads - 1) / num_threads * 8;

    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = t * chunk;
        data[t].end = (data[t].start + chunk > aligned_n) ? aligned_n : data[t].start + chunk;
        data[t].data = array;
        data[t].user_data = &partial_sums[t];

        if (data[t].start < aligned_n) {
            pthread_create(&threads[t], NULL, (void* (*)(void*))({
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
            }),
                           &data[t]);
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
