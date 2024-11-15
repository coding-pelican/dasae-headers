/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    random.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2024-11-03 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
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
// #define Random(TYPE)       pp_concat(Random_, TYPE)
// #define Random_range(TYPE, ...) pp_concat(Random_range_, TYPE)(__VA_ARGS__)

/*========== Extern Constant and Variable Declarations ======================*/

extern Random* const Random_rng;

/*========== Extern Function Prototypes =====================================*/

// Core random functions
extern Random* Random_init(Random* rng);
extern Random* Random_withSeed(Random* rng, u64 seed);
extern void    Random_setSeed(Random* rng, u64 seed);

// Integer ranges
extern u8    Random_u8(Random* rng);
extern u16   Random_u16(Random* rng);
extern u32   Random_u32(Random* rng);
extern u64   Random_u64(Random* rng);
extern usize Random_usize(Random* rng);

extern i8    Random_i8(Random* rng);
extern i16   Random_i16(Random* rng);
extern i32   Random_i32(Random* rng);
extern i64   Random_i64(Random* rng);
extern isize Random_isize(Random* rng);

// Float ranges [0,1)
extern f32 Random_f32(Random* rng);
extern f64 Random_f64(Random* rng);

// Bounded ranges
extern u64 Random_range_u64(Random* rng, u64 min, u64 max);
extern i64 Random_range_i64(Random* rng, i64 min, i64 max);
extern f64 Random_range_f64(Random* rng, f64 min, f64 max);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RANDOM_INCLUDED */
