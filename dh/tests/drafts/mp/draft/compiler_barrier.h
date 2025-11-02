// ============================================
// 컴파일러 최적화 방지 유틸리티
// Rust의 black_box와 유사한 기능
// ============================================

#ifndef COMPILER_BARRIER_H
#define COMPILER_BARRIER_H

#include <stdint.h>
#include <string.h>

// ============================================
// 1. Black Box - 값이 사용된다고 강제
// ============================================

// GCC/Clang용 black_box
#if defined(__GNUC__) || defined(__clang__)

// 포인터 버전 (메모리)
static inline void black_box_ptr(void* ptr, size_t size) {
    // 메모리 clobber - 컴파일러가 메모리 내용을 모른다고 가정
    __asm__ __volatile__("" : : "r"(ptr), "r"(size) : "memory");
}

// 정수 버전
static inline void black_box_int(int* val) {
    __asm__ __volatile__("" : "+r"(*val) : : "memory");
}

// float 버전
static inline void black_box_float(float* val) {
    __asm__ __volatile__("" : "+m"(*val) : : "memory");
}

// double 버전
static inline void black_box_double(double* val) {
    __asm__ __volatile__("" : "+m"(*val) : : "memory");
}

// 배열 버전
static inline void black_box_array(void* arr, size_t elem_size, size_t count) {
    __asm__ __volatile__("" : : "r"(arr), "r"(elem_size * count) : "memory");
}

// 범용 매크로
#define BLACK_BOX(x) do { \
    __asm__ __volatile__("" : "+r"(x) : : "memory"); \
} while(0)

#define BLACK_BOX_MEM(ptr, size) do { \
    __asm__ __volatile__("" : : "r"(ptr), "r"(size) : "memory"); \
} while(0)

#else
// 다른 컴파일러용 fallback (volatile 사용)

static inline void black_box_ptr(void* ptr, size_t size) {
    volatile char* p = (volatile char*)ptr;
    (void)p[0];  // 최소한 첫 바이트 접근
}

static inline void black_box_int(int* val) {
    volatile int* p = val;
    (void)*p;
}

static inline void black_box_float(float* val) {
    volatile float* p = val;
    (void)*p;
}

static inline void black_box_double(double* val) {
    volatile double* p = val;
    (void)*p;
}

static inline void black_box_array(void* arr, size_t elem_size, size_t count) {
    volatile char* p = (volatile char*)arr;
    (void)p[0];
}

#define BLACK_BOX(x) do { \
    volatile typeof(x) _tmp = (x); \
    (void)_tmp; \
} while(0)

#define BLACK_BOX_MEM(ptr, size) do { \
    volatile char* _p = (volatile char*)(ptr); \
    (void)_p[0]; \
} while(0)

#endif

// ============================================
// 2. Do Not Optimize - 함수 최적화 방지
// ============================================

#if defined(__GNUC__) || defined(__clang__)
    #define DO_NOT_OPTIMIZE __attribute__((optimize("O0")))
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
    #define NEVER_INLINE __attribute__((noinline))
    #define USED __attribute__((used))
#elif defined(_MSC_VER)
    #define DO_NOT_OPTIMIZE __pragma(optimize("", off))
    #define ALWAYS_INLINE __forceinline
    #define NEVER_INLINE __declspec(noinline)
    #define USED
#else
    #define DO_NOT_OPTIMIZE
    #define ALWAYS_INLINE inline
    #define NEVER_INLINE
    #define USED
#endif

// ============================================
// 3. 메모리 배리어
// ============================================

static inline void memory_barrier(void) {
    #if defined(__GNUC__) || defined(__clang__)
        __asm__ __volatile__("" : : : "memory");
    #elif defined(_MSC_VER)
        _ReadWriteBarrier();
    #else
        volatile int dummy = 0;
        (void)dummy;
    #endif
}

static inline void compiler_barrier(void) {
    memory_barrier();
}

// ============================================
// 4. 벤치마크 헬퍼
// ============================================

// 결과를 사용한다고 컴파일러에게 알림
#define BENCHMARK_USE(x) black_box_ptr(&(x), sizeof(x))

// 배열 전체를 사용한다고 알림
#define BENCHMARK_USE_ARRAY(arr, count) \
    black_box_array(arr, sizeof((arr)[0]), count)

// 함수 포인터를 통한 간접 호출 (최적화 방지)
typedef void (*benchmark_func_t)(void*);

static NEVER_INLINE void call_indirectly(benchmark_func_t func, void* data) {
    func(data);
    memory_barrier();
}

// ============================================
// 5. 실제 사용 예제
// ============================================

// 예제 1: 단일 값
static inline void example_single_value(void) {
    float result = 0.0f;
    
    // 계산
    for (int i = 0; i < 1000; i++) {
        result += i * 0.5f;
    }
    
    // 컴파일러가 제거하지 못하도록
    BLACK_BOX(result);
}

// 예제 2: 배열
static inline void example_array(void) {
    float data[1000];
    
    // 계산
    for (int i = 0; i < 1000; i++) {
        data[i] = i * 2.0f;
    }
    
    // 컴파일러가 제거하지 못하도록
    BENCHMARK_USE_ARRAY(data, 1000);
}

// 예제 3: 벤치마크 함수
typedef struct {
    float* data;
    int size;
} benchmark_data_t;

static NEVER_INLINE void benchmark_work(void* arg) {
    benchmark_data_t* bd = (benchmark_data_t*)arg;
    
    for (int i = 0; i < bd->size; i++) {
        bd->data[i] = bd->data[i] * 2.0f;
    }
    
    // 결과 사용
    BLACK_BOX_MEM(bd->data, bd->size * sizeof(float));
}

// ============================================
// 6. 고급: volatile 래퍼
// ============================================

// volatile을 통한 강제 메모리 접근
static inline void force_memory_access(void* ptr, size_t size) {
    volatile char* p = (volatile char*)ptr;
    volatile char dummy = 0;
    
    // 첫 바이트와 마지막 바이트 접근
    dummy += p[0];
    if (size > 1) {
        dummy += p[size - 1];
    }
    
    (void)dummy;
}

// 배열의 체크섬을 계산하여 사용
static inline uint64_t force_array_use(const void* arr, size_t size) {
    const unsigned char* p = (const unsigned char*)arr;
    uint64_t sum = 0;
    
    // 간단한 체크섬
    for (size_t i = 0; i < size; i += 64) {
        sum += p[i];
    }
    
    // 마지막 바이트
    if (size > 0) {
        sum += p[size - 1];
    }
    
    return sum;
}

// ============================================
// 7. 타이밍 측정용
// ============================================

#include <time.h>
#include <sys/time.h>

typedef struct {
    double start_time;
    void* data;
    size_t data_size;
} benchmark_timer_t;

static inline double get_precise_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

static inline void benchmark_start(benchmark_timer_t* timer, 
                                   void* data, size_t size) {
    // 데이터를 사용한다고 표시
    BLACK_BOX_MEM(data, size);
    
    timer->data = data;
    timer->data_size = size;
    timer->start_time = get_precise_time();
}

static inline double benchmark_end(benchmark_timer_t* timer) {
    double end_time = get_precise_time();
    
    // 결과를 사용한다고 표시
    BLACK_BOX_MEM(timer->data, timer->data_size);
    
    return end_time - timer->start_time;
}

// ============================================
// 8. 매크로 헬퍼
// ============================================

// 벤치마크 블록
#define BENCHMARK_BLOCK_BEGIN(name, data, size) \
    do { \
        benchmark_timer_t _timer_##name; \
        benchmark_start(&_timer_##name, data, size);

#define BENCHMARK_BLOCK_END(name) \
        double _time_##name = benchmark_end(&_timer_##name); \
        printf("%s: %.6f ms\n", #name, _time_##name * 1000.0); \
    } while(0)

// 워밍업 실행
#define WARMUP_ITERATIONS 3

#define BENCHMARK_WARMUP(code) \
    do { \
        for (int _i = 0; _i < WARMUP_ITERATIONS; _i++) { \
            code; \
        } \
        memory_barrier(); \
    } while(0)

#endif // COMPILER_BARRIER_H
