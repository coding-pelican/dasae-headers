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

/*
 * $static var_(Rand__s_rng, Rand);
 * $static fn_((Rand__xorshiro256(u64 x, u32 k))(u64));
 * $static fn_((Rand__next(void))(u64));
 */

/*========== Extern Constant and Variable Declarations ======================*/

extern Rand* const Rand_rng;

/*========== Extern Function Prototypes =====================================*/

// Core random functions
extern void Rand_init(void);
extern void Rand_initWithSeed(u64 seed);
extern void Rand_setSeed(u64 seed);

// Integer ranges
extern u64   Rand_nextUInt(void);
extern usize Rand_next$usize(void);
extern u64   Rand_next$u64(void);
extern u32   Rand_next$u32(void);
extern u16   Rand_next$u16(void);
extern u8    Rand_next$u8(void);

extern i64   Rand_nextInt(void);
extern isize Rand_next$isize(void);
extern i64   Rand_next$i64(void);
extern i32   Rand_next$i32(void);
extern i16   Rand_next$i16(void);
extern i8    Rand_next$i8(void);

// Float ranges [0,1)
extern f64 Rand_nextFlt(void);
extern f64 Rand_next$f64(void);
extern f32 Rand_next$f32(void);

// Bounded ranges
extern u64 Rand_rangeUInt(u64 min, u64 max);
extern i64 Rand_rangeInt(i64 min, i64 max);
extern f64 Rand_rangeFlt(f64 min, f64 max);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Rand__included */
