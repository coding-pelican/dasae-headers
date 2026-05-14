/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Rand.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2024-11-03 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  Rand
 *
 * @brief   Rand number generator utilities
 * @details Provides a random number generator that can be used to generate random numbers.
 */
#ifndef Rand__included
#define Rand__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

typedef struct Rand {
    u64 state;
    u64 stream;
} Rand;
$static let_(Rand_default, Rand) = {
    .state = 0xDEADBEEFCAFEBABE,
    .stream = 0xCAFEF00DD15EA5E5,
};
/*
 * $static fn_((Rand__rotl(u64 x, u32 k))(u64));
 * $static fn_((Rand__next(Rand* self, void))(u64));
 */

/*========== Extern Function Prototypes =====================================*/

// Core random functions
$extern fn_((Rand_init(void))(Rand));
$extern fn_((Rand_initSeed(u64 seed))(Rand));
$extern fn_((Rand_withSeed(Rand self, u64 seed))(Rand));
$extern fn_((Rand_setSeed(Rand* self, u64 seed))(void));

// Integer ranges [int_limit_min, int_limit_max)
$extern fn_((Rand_nextUInt(Rand* self))(u64));
$extern fn_((Rand_next$usize(Rand* self))(usize));
$extern fn_((Rand_next$u64(Rand* self))(u64));
$extern fn_((Rand_next$ulong(Rand* self))(ulong));
$extern fn_((Rand_next$u32(Rand* self))(u32));
$extern fn_((Rand_next$u16(Rand* self))(u16));
$extern fn_((Rand_next$u8(Rand* self))(u8));

$extern fn_((Rand_nextIInt(Rand* self))(i64));
$extern fn_((Rand_next$isize(Rand* self))(isize));
$extern fn_((Rand_next$i64(Rand* self))(i64));
$extern fn_((Rand_next$ilong(Rand* self))(ilong));
$extern fn_((Rand_next$i32(Rand* self))(i32));
$extern fn_((Rand_next$i16(Rand* self))(i16));
$extern fn_((Rand_next$i8(Rand* self))(i8));

// Float ranges [0, 1]
$extern fn_((Rand_nextFlt(Rand* self))(f64));
$extern fn_((Rand_next$f64(Rand* self))(f64));
$extern fn_((Rand_next$f32(Rand* self))(f32));

// Integer bounded ranges [min, max)
$extern fn_((Rand_rangeUInt(Rand* self, u64 min, u64 max))(u64));
$extern fn_((Rand_range$usize(Rand* self, usize min, usize max))(usize));
$extern fn_((Rand_range$u64(Rand* self, u64 min, u64 max))(u64));
$extern fn_((Rand_range$ulong(Rand* self, ulong min, ulong max))(ulong));
$extern fn_((Rand_range$u32(Rand* self, u32 min, u32 max))(u32));
$extern fn_((Rand_range$u16(Rand* self, u16 min, u16 max))(u16));
$extern fn_((Rand_range$u8(Rand* self, u8 min, u8 max))(u8));

$extern fn_((Rand_rangeIInt(Rand* self, i64 min, i64 max))(i64));
$extern fn_((Rand_range$isize(Rand* self, isize min, isize max))(isize));
$extern fn_((Rand_range$i64(Rand* self, i64 min, i64 max))(i64));
$extern fn_((Rand_range$ilong(Rand* self, ilong min, ilong max))(ilong));
$extern fn_((Rand_range$i32(Rand* self, i32 min, i32 max))(i32));
$extern fn_((Rand_range$i16(Rand* self, i16 min, i16 max))(i16));
$extern fn_((Rand_range$i8(Rand* self, i8 min, i8 max))(i8));

// Float bounded ranges [min, max]
$extern fn_((Rand_rangeFlt(Rand* self, f64 min, f64 max))(f64));
$extern fn_((Rand_range$f64(Rand* self, f64 min, f64 max))(f64));
$extern fn_((Rand_range$f32(Rand* self, f32 min, f32 max))(f32));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Rand__included */
