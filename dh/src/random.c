/*========== Includes =======================================================*/

#include "dh/Random.h"
#include "dh/time/SysTime.h"

/*========== Static Constant and Variable Definitions =======================*/

static Random Random__s_rng[1] = create(Random);

/*========== Extern Constant and Variable Definitions =======================*/

Random* const Random_rng = Random__s_rng;

/*========== Static Function Prototypes =====================================*/

static u64 Random__next();

/*========== Static Function Implementations ================================*/

// Using xorshiro256** algorithm
force_inline u64 Random__xorshiro256(u64 x, u32 k) {
    return (x << k) | (x >> (64ull - k));
}

// Generate next random number
static u64 Random__next() {
    const u64 result = Random__xorshiro256(Random_rng->state_ * 5, 7) * 9;
    const u64 t      = Random_rng->stream_ << 17ull;

    Random_rng->stream_ ^= Random_rng->state_;
    Random_rng->state_ ^= Random_rng->stream_;
    Random_rng->stream_ ^= t;
    Random_rng->state_ = Random__xorshiro256(Random_rng->state_, 45);

    return result;
}

/*========== Extern Function Implementations ================================*/

void Random_init() {
    // return Random_withSeed(time_SysTime_toNanos(time_Instant_now()));
}

void Random_withSeed(u64 seed) {
    Random_rng->state_  = seed;
    Random_rng->stream_ = 0xCAFEF00DD15EA5E5;
}

void Random_setSeed(u64 seed) {
    Random_rng->state_ = seed;
}

u8 Random_u8() {
    return (u8)(Random__next() & 0xFFu);
}

u16 Random_u16() {
    return (u16)(Random__next() & 0xFFFFu);
}

u32 Random_u32() {
    return (u32)(Random__next() & 0xFFFFFFFFu);
}

u64 Random_u64() {
    return Random__next();
}

usize Random_usize() {
    return (usize)Random__next();
}

i8 Random_i8() {
    return (i8)Random_u8();
}

i16 Random_i16() {
    return (i16)Random_u16();
}

i32 Random_i32() {
    return (i32)Random_u32();
}

i64 Random_i64() {
    return (i64)Random_u64();
}

isize Random_isize() {
    return (isize)Random_usize();
}

f32 Random_f32() {
    return (f32)(Random_u32() >> 8u) / (f32)(1U << 24u);
}

f64 Random_f64() {
    return (f64)(Random_u64() >> 11ull) / (f64)(1ull << 53ull);
}

u64 Random_range_u64(u64 min, u64 max) {
    return min + (Random__next() % (max - min + 1));
}

i64 Random_range_i64(i64 min, i64 max) {
    return min + (i64)(Random__next() % (u64)(max - min + 1));
}

f64 Random_range_f64(f64 min, f64 max) {
    return min + Random_f64() * (max - min);
}
