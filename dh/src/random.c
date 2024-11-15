/*========== Includes =======================================================*/

#include "dh/random.h"
#include "dh/time.h"

/*========== Static Constant and Variable Definitions =======================*/

static Random Random__rng[1] = create(Random);

/*========== Extern Constant and Variable Definitions =======================*/

Random* const Random_rng = Random__rng;

/*========== Static Function Prototypes =====================================*/

static u64 Random__next(Random* rng);

/*========== Static Function Implementations ================================*/

// Using xorshiro256** algorithm
force_inline u64 Random__xorshiro256(u64 x, u32 k) {
    return (x << k) | (x >> (64ull - k));
}

// Generate next random number
static u64 Random__next(Random* rng) {
    const u64 result = Random__xorshiro256(rng->state_ * 5, 7) * 9;
    const u64 t      = rng->stream_ << 17ull;

    rng->stream_ ^= rng->state_;
    rng->state_ ^= rng->stream_;
    rng->stream_ ^= t;
    rng->state_ = Random__xorshiro256(rng->state_, 45);

    return result;
}

/*========== Extern Function Implementations ================================*/

Random* Random_init(Random* rng) {
    return Random_withSeed(rng, time_Instant_toNanos(time_Instant_now()));
}

Random* Random_withSeed(Random* rng, u64 seed) {
    rng->state_  = seed;
    rng->stream_ = 0xCAFEF00DD15EA5E5;
    return rng;
}

void Random_setSeed(Random* rng, u64 seed) {
    rng->state_ = seed;
}

u8 Random_u8(Random* rng) {
    return (u8)(Random__next(rng) & 0xFFu);
}

u16 Random_u16(Random* rng) {
    return (u16)(Random__next(rng) & 0xFFFFu);
}

u32 Random_u32(Random* rng) {
    return (u32)(Random__next(rng) & 0xFFFFFFFFu);
}

u64 Random_u64(Random* rng) {
    return Random__next(rng);
}

usize Random_usize(Random* rng) {
    return (usize)Random__next(rng);
}

i8 Random_i8(Random* rng) {
    return (i8)Random_u8(rng);
}

i16 Random_i16(Random* rng) {
    return (i16)Random_u16(rng);
}

i32 Random_i32(Random* rng) {
    return (i32)Random_u32(rng);
}

i64 Random_i64(Random* rng) {
    return (i64)Random_u64(rng);
}

isize Random_isize(Random* rng) {
    return (isize)Random_usize(rng);
}

f32 Random_f32(Random* rng) {
    return (f32)(Random_u32(rng) >> 8u) / (f32)(1U << 24u);
}

f64 Random_f64(Random* rng) {
    return (f64)(Random_u64(rng) >> 11ull) / (f64)(1ull << 53ull);
}

u64 Random_range_u64(Random* rng, u64 min, u64 max) {
    return min + (Random__next(rng) % (max - min + 1));
}

i64 Random_range_i64(Random* rng, i64 min, i64 max) {
    return min + (i64)(Random__next(rng) % (u64)(max - min + 1));
}

f64 Random_range_f64(Random* rng, f64 min, f64 max) {
    return min + Random_f64(rng) * (max - min);
}
