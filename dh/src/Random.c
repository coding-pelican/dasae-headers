/*========== Includes =======================================================*/

#include "dh/Random.h"
#include "dh/time/Instant.h"

/*========== Static Constant and Variable Definitions =======================*/

static Random Random_s_rng[1] = { { .state_ = 0xDEADBEEFCAFEBABE, .stream_ = 0xCAFEF00DD15EA5E5 } };

/*========== Extern Constant and Variable Definitions =======================*/

Random* const Random_rng = Random_s_rng;

/*========== Static Function Prototypes =====================================*/

// Using xorshiro256** algorithm
$inline_always u64 Random_xorshiro256(u64 x, u32 k);
// Generate next random number
static u64         Random_next(void);

/*========== Static Function Implementations ================================*/

$inline_always u64 Random_xorshiro256(u64 x, u32 k) {
    return (x << k) | (x >> (64ull - k));
}

static u64 Random_next() {
    const u64 result = Random_xorshiro256(Random_rng->state_ * 5, 7) * 9;
    const u64 t      = Random_rng->stream_ << 17ull;

    Random_rng->stream_ ^= Random_rng->state_;
    Random_rng->state_ ^= Random_rng->stream_;
    Random_rng->stream_ ^= t;
    Random_rng->state_ = Random_xorshiro256(Random_rng->state_, 45);

    return result;
}

/*========== Extern Function Implementations ================================*/

void Random_init(void) {
    return Random_initWithSeed(time_Instant_toUnixEpoch(time_Instant_now()));
}

void Random_initWithSeed(u64 seed) {
    Random_rng->state_  = seed;
    Random_rng->stream_ = 0xCAFEF00DD15EA5E5;
}

void Random_setSeed(u64 seed) {
    Random_rng->state_ = seed;
}

u8 Random_u8(void) {
    return as$((u8)(Random_next() & 0xFFu));
}

u16 Random_u16(void) {
    return as$((u16)(Random_next() & 0xFFFFu));
}

u32 Random_u32(void) {
    return as$((u32)(Random_next() & 0xFFFFFFFFu));
}

u64 Random_u64(void) {
    return Random_next();
}

usize Random_usize(void) {
    return as$((usize)(Random_next()));
}

i8 Random_i8(void) {
    return as$((i8)(Random_u8()));
}

i16 Random_i16(void) {
    return as$((i16)(Random_u16()));
}

i32 Random_i32(void) {
    return as$((i32)(Random_u32()));
}

i64 Random_i64(void) {
    return as$((i64)(Random_u64()));
}

isize Random_isize(void) {
    return as$((isize)(Random_usize()));
}

f32 Random_f32(void) {
    return as$((f32)(Random_u32() >> 8u)) / as$((f32)(1u << 24u));
}

f64 Random_f64(void) {
    return as$((f64)(Random_u64() >> 11ull)) / as$((f64)(1ull << 53ull));
}

u64 Random_range_u64(u64 min, u64 max) {
    return min + (Random_next() % (max - min + 1));
}

i64 Random_range_i64(i64 min, i64 max) {
    return min + as$((i64)(Random_next() % as$((u64)(max - min + 1))));
}

f64 Random_range_f64(f64 min, f64 max) {
    return min + Random_f64() * (max - min);
}
