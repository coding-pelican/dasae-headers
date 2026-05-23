/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Rand.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2026-05-22 (date of last update)
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

#include "Range.h"

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
$extern fn_((Rand_fillBytes(Rand* self, S$u8 buf))(S$u8));
$extern fn_((Rand_boolean(Rand* self))(bool));

// Integer ranges [int_limit_min, int_limit_max]
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

// Integer upper bounds
$extern fn_((Rand_lessThanUInt(Rand* self, u64 less_than))(u64));
$extern fn_((Rand_lessThan$usize(Rand* self, usize less_than))(usize));
$extern fn_((Rand_lessThan$u64(Rand* self, u64 less_than))(u64));
$extern fn_((Rand_lessThan$ulong(Rand* self, ulong less_than))(ulong));
$extern fn_((Rand_lessThan$u32(Rand* self, u32 less_than))(u32));
$extern fn_((Rand_lessThan$u16(Rand* self, u16 less_than))(u16));
$extern fn_((Rand_lessThan$u8(Rand* self, u8 less_than))(u8));

$extern fn_((Rand_atMostUInt(Rand* self, u64 at_most))(u64));
$extern fn_((Rand_atMost$usize(Rand* self, usize at_most))(usize));
$extern fn_((Rand_atMost$u64(Rand* self, u64 at_most))(u64));
$extern fn_((Rand_atMost$ulong(Rand* self, ulong at_most))(ulong));
$extern fn_((Rand_atMost$u32(Rand* self, u32 at_most))(u32));
$extern fn_((Rand_atMost$u16(Rand* self, u16 at_most))(u16));
$extern fn_((Rand_atMost$u8(Rand* self, u8 at_most))(u8));

// Integer ranges
$extern fn_((Rand_rangeUInt(Rand* self, Range$u64 range))(u64));
$extern fn_((Rand_range$usize(Rand* self, Range$usize range))(usize));
$extern fn_((Rand_range$u64(Rand* self, Range$u64 range))(u64));
$extern fn_((Rand_range$ulong(Rand* self, Range$ulong range))(ulong));
$extern fn_((Rand_range$u32(Rand* self, Range$u32 range))(u32));
$extern fn_((Rand_range$u16(Rand* self, Range$u16 range))(u16));
$extern fn_((Rand_range$u8(Rand* self, Range$u8 range))(u8));

$extern fn_((Rand_rangeIInt(Rand* self, Range$i64 range))(i64));
$extern fn_((Rand_range$isize(Rand* self, Range$isize range))(isize));
$extern fn_((Rand_range$i64(Rand* self, Range$i64 range))(i64));
$extern fn_((Rand_range$ilong(Rand* self, Range$ilong range))(ilong));
$extern fn_((Rand_range$i32(Rand* self, Range$i32 range))(i32));
$extern fn_((Rand_range$i16(Rand* self, Range$i16 range))(i16));
$extern fn_((Rand_range$i8(Rand* self, Range$i8 range))(i8));

// Float ranges
$extern fn_((Rand_rangeFlt(Rand* self, Range$f64 range))(f64));
$extern fn_((Rand_range$f64(Rand* self, Range$f64 range))(f64));
$extern fn_((Rand_range$f32(Rand* self, Range$f32 range))(f32));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Rand__included */
