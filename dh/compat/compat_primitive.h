/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    compat_primitive.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/compat
 * @prefix  prim
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef COMPAT_PRIMITIVE_INCLUDED
#define COMPAT_PRIMITIVE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include <stdint.h>


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
// typedef __uint128_t u128;
typedef uintptr_t usize;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
// typedef __int128_t i128;
typedef intptr_t isize;

// typedef _Float16    f16;
typedef float  f32;
typedef double f64;
// typedef long double f80;
// typedef _Float128   f128;

typedef void* anyopaque;

#define prim_as(_T, ...)             ((_T)__VA_ARGS__)
#define prim_divisible(_prim_n, ...) ((__VA_ARGS__ == 0) ? 0 : ((_prim_n) % (__VA_ARGS__) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* COMPAT_PRIMITIVE_INCLUDED */
