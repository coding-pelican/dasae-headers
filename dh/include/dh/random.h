#ifndef RANDOM_INCLUDED
#define RANDOM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*========== Includes =======================================================*/

#include "core/core.h"

/*========== Forward Declarations ===========================================*/

typedef struct Random Random;

/*========== Macros and Definitions =========================================*/

struct Random {
    u64 state_;
    u64 stream_;
};

/*========== Extern Constant and Variable Declarations ======================*/

extern Random random_rng[1];

/*========== Extern Function Prototypes =====================================*/

// Core random functions
Random* random_init(Random* rng);
Random* random_withSeed(Random* rng, u64 seed);
void    random_setSeed(Random* rng, u64 seed);

// Integer ranges
u8    random_u8(Random* rng);
u16   random_u16(Random* rng);
u32   random_u32(Random* rng);
u64   random_u64(Random* rng);
usize random_usize(Random* rng);

i8    random_i8(Random* rng);
i16   random_i16(Random* rng);
i32   random_i32(Random* rng);
i64   random_i64(Random* rng);
isize random_isize(Random* rng);

// Float ranges [0,1)
f32 random_f32(Random* rng);
f64 random_f64(Random* rng);

// Bounded ranges
u64 random_range_u64(Random* rng, u64 min, u64 max);
i64 random_range_i64(Random* rng, i64 min, i64 max);
f64 random_range_f64(Random* rng, f64 min, f64 max);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RANDOM_INCLUDED */
