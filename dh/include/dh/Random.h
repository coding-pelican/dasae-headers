/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Random.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2024-11-03 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  Random
 *
 * @brief   Random number generator utilities
 * @details Provides a random number generator that can be used to generate random numbers.
 */

#ifndef RANDOM_INCLUDED
#define RANDOM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

typedef struct Random {
    u64 state_;
    u64 stream_;
} Random;

/*
 * static       Random Random_s_rng[1];
 * $inline_always u64    Random_xorshiro256(u64 x, u32 k);
 * static       u64    Random_next();
 */

/*========== Extern Constant and Variable Declarations ======================*/

extern Random* const Random_rng;

/*========== Extern Function Prototypes =====================================*/

// Core random functions
extern void Random_init(void);
extern void Random_initWithSeed(u64 seed);
extern void Random_setSeed(u64 seed);

// Integer ranges
extern u8    Random_u8(void);
extern u16   Random_u16(void);
extern u32   Random_u32(void);
extern u64   Random_u64(void);
extern usize Random_usize(void);

extern i8    Random_i8(void);
extern i16   Random_i16(void);
extern i32   Random_i32(void);
extern i64   Random_i64(void);
extern isize Random_isize(void);

// Float ranges [0,1)
extern f32 Random_f32(void);
extern f64 Random_f64(void);

// Bounded ranges
extern u64 Random_range_u64(u64 min, u64 max);
extern i64 Random_range_i64(i64 min, i64 max);
extern f64 Random_range_f64(f64 min, f64 max);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RANDOM_INCLUDED */
