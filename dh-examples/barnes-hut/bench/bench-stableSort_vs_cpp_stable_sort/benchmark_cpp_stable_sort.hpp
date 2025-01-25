#ifndef BENCHMARK_CPP_STABLE_SORT_INCLUDED
#define BENCHMARK_CPP_STABLE_SORT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if defined(__cplusplus)

#include <cstdint>

/* unsigned integer */
typedef uint8_t u8;
#define u8_limit     u8_limit_max
#define u8_limit_min (0)
#define u8_limit_max (UINT8_MAX)

typedef uint16_t u16;
#define u16_limit     u16_limit_max
#define u16_limit_min (0)
#define u16_limit_max (UINT16_MAX)

typedef uint32_t u32;
#define u32_limit     u32_limit_max
#define u32_limit_min (0)
#define u32_limit_max (UINT32_MAX)

typedef uint64_t u64;
#define u64_limit     u64_limit_max
#define u64_limit_min (0)
#define u64_limit_max (UINT64_MAX)

// typedef __uint128_t u128;
// #define u128_limit u128_limit_max
// #define u128_limit_min (0)
// #define u128_limit_max (UINT128_MAX)

typedef uintptr_t usize;
#define usize_limit     usize_limit_max
#define usize_limit_min (0)
#define usize_limit_max (UINTPTR_MAX)

/* signed integer */
typedef int8_t i8;
#define i8_limit_min (INT8_MIN)
#define i8_limit_max (INT8_MAX)

typedef int16_t i16;
#define i16_limit_min (INT16_MIN)
#define i16_limit_max (INT16_MAX)

typedef int32_t i32;
#define i32_limit_min (INT32_MIN)
#define i32_limit_max (INT32_MAX)

typedef int64_t i64;
#define i64_limit_min (INT64_MIN)
#define i64_limit_max (INT64_MAX)

// typedef __int128_t i128;
// #define i128_limit_min (INT128_MIN)
// #define i128_limit_max (INT128_MAX)

typedef intptr_t isize;
#define isize_limit_min (INTPTR_MIN)
#define isize_limit_max (INTPTR_MAX)

typedef float f32;
#define f32_limit_min (FLT_MIN)
#define f32_limit_max (FLT_MAX)
#define f32_eps       (FLT_EPSILON)
#define f32_nan       (NAN)
#define f32_inf       (INFINITY)

typedef double f64;
#define f64_limit_min (FLT_MIN)
#define f64_limit_max (DBL_MAX)
#define f64_eps       (DBL_EPSILON)
#define f64_nan       (NAN)
#define f64_inf       (INFINITY)

#else

#include "dh/core/prim/int.h"
#include "dh/core/prim/flt.h"

#endif

typedef struct TestElem {
    int value;
    int seq;
#if defined(__cplusplus)
    bool operator<(const TestElem& other) const {
        return value < other.value;
    }
#endif
} TestElem;
typedef struct BenchParams {
    struct {
        TestElem* ptr;
        usize     len;
    };
    usize iterations;
} BenchParams;
typedef struct BenchResult {
    const char* name;
    f64         total_ms;
    f64         avg_ms;
    bool        stable;
} BenchResult;
extern BenchResult benchmark_cpp_stable_sort(BenchParams params);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BENCHMARK_CPP_STABLE_SORT_INCLUDED */
