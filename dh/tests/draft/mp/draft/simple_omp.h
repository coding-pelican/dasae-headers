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
    atomic_int* next_iter;  // 동적 스케줄링용
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
        if (i >= data->total_iters) break;
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
            _data[_t].end = (_data[_t].start + _chunk > _end) ? \
                            _end : _data[_t].start + _chunk; \
            _data[_t].stride = 1; \
            _data[_t].func = NULL; \
            _data[_t].user_data = NULL; \
            \
            if (_data[_t].start < _end) { \
                pthread_create(&_threads[_t], NULL, \
                    (void*(*)(void*))({ \
                        void* _worker(void* _arg) { \
                            somp_thread_data_t* _d = _arg; \
                            for (int i = _d->start; i < _d->end; i++) { \
                                body; \
                            } \
                            return NULL; \
                        } \
                        _worker; \
                    }), &_data[_t]); \
            } \
        } \
        \
        for (int _t = 0; _t < _num_threads; _t++) { \
            if (_data[_t].start < _end) { \
                pthread_join(_threads[_t], NULL); \
            } \
        } \
    } while(0)

// ============================================
// 5. 함수 기반 API (더 유연함)
// ============================================

void somp_parallel_for_static(int start, int end, 
                               somp_loop_func_t func, 
                               void* user_data) {
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    
    int chunk = (end - start + num_threads - 1) / num_threads;
    
    for (int t = 0; t < num_threads; t++) {
        data[t].thread_id = t;
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > end) ? 
                      end : data[t].start + chunk;
        data[t].stride = 1;
        data[t].func = func;
        data[t].user_data = user_data;
        
        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, 
                          somp_static_worker, &data[t]);
        }
    }
    
    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < end) {
            pthread_join(threads[t], NULL);
        }
    }
}

void somp_parallel_for_dynamic(int start, int end,
                                somp_loop_func_t func,
                                void* user_data) {
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
        
        pthread_create(&threads[t], NULL, 
                      somp_dynamic_worker, &data[t]);
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
                if (i == data->start || val < local_result)
                    local_result = val;
                break;
            case SOMP_REDUCE_MAX:
                if (i == data->start || val > local_result)
                    local_result = val;
                break;
            case SOMP_REDUCE_PROD:
                if (i == data->start)
                    local_result = val;
                else
                    local_result *= val;
                break;
        }
    }
    
    rdata->partial_results[data->thread_id] = local_result;
    return NULL;
}

double somp_parallel_reduce(int start, int end,
                            somp_loop_func_t func,
                            void* user_data,
                            somp_reduce_op_t op) {
    int num_threads = global_ctx.num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    double partial_results[SOMP_MAX_THREADS] = {0};
    
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
        data[t].end = (data[t].start + chunk > end) ? 
                      end : data[t].start + chunk;
        data[t].user_data = &rdata;
        
        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, 
                          somp_reduce_worker, &data[t]);
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
                if (partial_results[t] < final_result)
                    final_result = partial_results[t];
                break;
            case SOMP_REDUCE_MAX:
                if (partial_results[t] > final_result)
                    final_result = partial_results[t];
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
#define SOMP_CRITICAL_END pthread_mutex_unlock(&global_lock)

// ============================================
// 8. Atomic 연산 래퍼
// ============================================

#define SOMP_ATOMIC_ADD(var, val) atomic_fetch_add(&(var), (val))
#define SOMP_ATOMIC_INC(var) atomic_fetch_add(&(var), 1)
#define SOMP_ATOMIC_READ(var) atomic_load(&(var))
#define SOMP_ATOMIC_WRITE(var, val) atomic_store(&(var), (val))

#endif // SIMPLE_OMP_H
