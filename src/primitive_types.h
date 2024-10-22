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


#define PtrT(_Type) _Type*
#define RefT(_Type) _Type* const

#ifndef NULL
#  define NULL ((void*)0)
#endif // NULL

#ifndef nullptr
#  define nullptr NULL
#endif // nullptr


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   usize;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ssize_t isize;

typedef float  f32;
typedef double f64;

#ifndef bool
#  define bool  u8
#  define true  (1)
#  define false (0)
#endif // bool

typedef wchar_t wchar;


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* PRIMITIVE_TYPES_INCLUDED */
