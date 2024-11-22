/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Random.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2024-11-03 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  Random
 *
 * @brief   Header of some software
 * @details Some detailed explanation
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
 * force_inline u64    Random_xorshiro256(u64 x, u32 k);
 * static       u64    Random_next();
 */

/*========== Extern Constant and Variable Declarations ======================*/

extern Random* const Random_rng;

/*========== Extern Function Prototypes =====================================*/

// Core random functions
extern void Random_init();
extern void Random_withSeed(u64 seed);
extern void Random_setSeed(u64 seed);

// Integer ranges
extern u8    Random_u8();
extern u16   Random_u16();
extern u32   Random_u32();
extern u64   Random_u64();
extern usize Random_usize();

extern i8    Random_i8();
extern i16   Random_i16();
extern i32   Random_i32();
extern i64   Random_i64();
extern isize Random_isize();

// Float ranges [0,1)
extern f32 Random_f32();
extern f64 Random_f64();

// Bounded ranges
extern u64 Random_range_u64(u64 min, u64 max);
extern i64 Random_range_i64(i64 min, i64 max);
extern f64 Random_range_f64(f64 min, f64 max);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RANDOM_INCLUDED */
