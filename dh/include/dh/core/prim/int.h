/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    int.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_INT_INCLUDED
#define PRIM_INT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

#include <stdint.h>

/*========== Macros and Definitions =========================================*/

/* unsigned integer */
typedef uint8_t u8;
#define u8_limit UINT8_MAX

typedef uint16_t u16;
#define u16_limit UINT16_MAX

typedef uint32_t u32;
#define u32_limit UINT32_MAX

typedef uint64_t u64;
#define u64_limit UINT64_MAX

// typedef __uint128_t u128;
// #define u128_limit UINT128_MAX

typedef uintptr_t usize;
#define usize_limit UINTPTR_MAX

/* signed integer */
typedef int8_t i8;
#define i8_limit_min INT8_MIN
#define i8_limit_max INT8_MAX

typedef int16_t i16;
#define i16_limit_min INT16_MIN
#define i16_limit_max INT16_MAX

typedef int32_t i32;
#define i32_limit_min INT32_MIN
#define i32_limit_max INT32_MAX

typedef int64_t i64;
#define i64_limit_min INT64_MIN
#define i64_limit_max INT64_MAX

// typedef __int128_t i128;
// #define i128_limit_min INT128_MIN
// #define i128_limit_max INT128_MAX

typedef intptr_t isize;
#define isize_limit_min INTPTR_MIN
#define isize_limit_max INTPTR_MAX


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_INT_INCLUDED */
