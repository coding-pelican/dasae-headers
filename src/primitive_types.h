/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    primitive_types.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIMITIVE_TYPES_INCLUDED
#define PRIMITIVE_TYPES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include <stdbool.h>
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#  include <corecrt.h>
#endif // defined(_WIN32) || defined(_WIN64)


#define unused(_value) ((void)(_value))
#define ignore_return  (void)


#define as(_T, ...) ((_T)__VA_ARGS__)

#define make(_T)          ((_T){ 0 })
#define makeCleared(_T)   ((_T){ 0 })
#define makeWith(_T, ...) ((_T){ __VA_ARGS__ })


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

typedef wchar_t wchar;

typedef float  f32;
typedef double f64;
// typedef long double f80;
// typedef __float128 f128;

#ifndef bool
#  define bool  u8
#  define true  (1)
#  define false (0)
#endif

#define divisible(_primitive_n, ...) ((__VA_ARGS__ == 0) ? 0 : ((_primitive_n) % (__VA_ARGS__) == 0))


typedef void* type;

// Dynamically-typed declarations
#if defined(__GNUC__) || defined(__GNUG__)
#  define var   __auto_type
#  define local __auto_type
#elif defined(__cplusplus) || (201710L < __STDC_VERSION__)
#  define var   auto
#  define local auto
#else
#  define var   no_var
#  define local no_local
#endif

#define Ptr(_T) _T*
#define Ref(_T) _T* const

#ifndef NULL
#  ifdef __cplusplus
#    ifndef _WIN64 /* _WIN32 */
#      define NULL (0)
#    else /* _WIN64 */
#      define NULL (0ll)
#    endif
#  else
#    define NULL ((void*)0)
#  endif
#endif

#ifndef nullptr
#  ifndef __cplusplus
#    define nullptr NULL
#  endif
#endif

#ifndef null
#  define null nullptr
#endif

#define ensure(_x, ...) ((_x) ? (_x) : (__VA_ARGS__))


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* PRIMITIVE_TYPES_INCLUDED */
