/*========== Includes =======================================================*/

#include "../random.h"

/*========== Extern Constant and Variable Definitions =======================*/

Random random_rng[1] = create(Random);

/*========== Static Function Prototypes =====================================*/

static u64 random__next(Random* rng);

/*========== Static Function Implementations ================================*/

// Using xoshiro256** algorithm
static inline u64 random__xorshiro256(u64 x, u32 k) {
    return (x << k) | (x >> (64ull - k));
}

// Generate next random number
static u64 random__next(Random* rng) {
    const u64 result = random__xorshiro256(rng->state_ * 5, 7) * 9;
    const u64 t      = rng->stream_ << 17ull;

    rng->stream_ ^= rng->state_;
    rng->state_ ^= rng->stream_;
    rng->stream_ ^= t;
    rng->state_ = random__xorshiro256(rng->state_, 45);

    return result;
}

/*========== Extern Function Implementations ================================*/

Random* random_init(Random* rng) {
    return random_withSeed(rng, SystemTime_toNanos(SystemTime_now()));
}

Random* random_withSeed(Random* rng, u64 seed) {
    rng->state_  = seed;
    rng->stream_ = 0xCAFEF00DD15EA5E5;
    return rng;
}

void random_setSeed(Random* rng, u64 seed) {
    rng->state_ = seed;
}

u8 random_u8(Random* rng) {
    return (u8)(random__next(rng) & 0xFFu);
}

u16 random_u16(Random* rng) {
    return (u16)(random__next(rng) & 0xFFFFu);
}

u32 random_u32(Random* rng) {
    return (u32)(random__next(rng) & 0xFFFFFFFFu);
}

u64 random_u64(Random* rng) {
    return random__next(rng);
}

usize random_usize(Random* rng) {
    return (usize)random__next(rng);
}

i8 random_i8(Random* rng) {
    return (i8)random_u8(rng);
}

i16 random_i16(Random* rng) {
    return (i16)random_u16(rng);
}

i32 random_i32(Random* rng) {
    return (i32)random_u32(rng);
}

i64 random_i64(Random* rng) {
    return (i64)random_u64(rng);
}

isize random_isize(Random* rng) {
    return (isize)random_usize(rng);
}

f32 random_f32(Random* rng) {
    return (f32)(random_u32(rng) >> 8u) / (f32)(1U << 24u);
}

f64 random_f64(Random* rng) {
    return (f64)(random_u64(rng) >> 11ull) / (f64)(1ull << 53ull);
}

u64 random_range_u64(Random* rng, u64 min, u64 max) {
    return min + (random__next(rng) % (max - min + 1));
}

i64 random_range_i64(Random* rng, i64 min, i64 max) {
    return min + (i64)(random__next(rng) % (u64)(max - min + 1));
}

f64 random_range_f64(Random* rng, f64 min, f64 max) {
    return min + random_f64(rng) * (max - min);
}
