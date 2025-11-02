/*========== Includes =======================================================*/

#include "dh/Rand.h"
#include "dh/time/Instant.h"

/*========== Static Constant and Variable Definitions =======================*/

static Rand Rand__s_rng = { .state = 0xDEADBEEFCAFEBABE, .stream = 0xCAFEF00DD15EA5E5 };

/*========== Extern Constant and Variable Definitions =======================*/

Rand* const Rand_rng = &Rand__s_rng;

/*========== Static Function Prototypes =====================================*/

// Using xorshiro256** algorithm
static $inline_always u64 Rand__xorshiro256(u64 x, u32 k);
// Generate next random number
static u64                Rand__next(void);

/*========== Static Function Implementations ================================*/

static $inline_always u64 Rand__xorshiro256(u64 x, u32 k) {
    return (x << k) | (x >> (64ull - k));
}

static u64 Rand__next(void) {
    const u64 result = Rand__xorshiro256(Rand_rng->state * 5, 7) * 9;
    const u64 t      = Rand_rng->stream << 17ull;

    Rand_rng->stream ^= Rand_rng->state;
    Rand_rng->state ^= Rand_rng->stream;
    Rand_rng->stream ^= t;
    Rand_rng->state = Rand__xorshiro256(Rand_rng->state, 45);

    return result;
}

/*========== Extern Function Implementations ================================*/

void Rand_init(void) {
    return Rand_initWithSeed(time_Instant_toUnixEpoch(time_Instant_now()));
}

void Rand_initWithSeed(u64 seed) {
    Rand_rng->state  = seed;
    Rand_rng->stream = 0xCAFEF00DD15EA5E5;
}

void Rand_setSeed(u64 seed) {
    Rand_rng->state = seed;
}

u64 Rand_nextUInt(void) {
    return Rand_next$u64();
}

usize Rand_next$usize(void) {
    return as$((usize)(Rand_nextUInt())); /* TODO: Precise conversion */
}

u64 Rand_next$u64(void) {
    return Rand__next();
}

u32 Rand_next$u32(void) {
    return as$((u32)(Rand_nextUInt() & 0xFFFFFFFFu));
}

u16 Rand_next$u16(void) {
    return as$((u16)(Rand_nextUInt() & 0xFFFFu));
}

u8 Rand_next$u8(void) {
    return as$((u8)(Rand_nextUInt() & 0xFFu));
}

isize Rand_nextInt(void) {
    return Rand_next$i64();
}

isize Rand_next$isize(void) {
    return as$((isize)(Rand_nextInt())); /* TODO: Precise conversion */
}

i64 Rand_next$i64(void) {
    return as$((i64)(Rand_next$u64()));
}

i32 Rand_next$i32(void) {
    return as$((i32)(Rand_nextInt()));
}

i16 Rand_next$i16(void) {
    return as$((i16)(Rand_nextInt()));
}

i8 Rand_next$i8(void) {
    return as$((i8)(Rand_nextInt()));
}

f64 Rand_nextFlt(void) {
    return Rand_next$f64();
}

f64 Rand_next$f64(void) {
    return as$((f64)(Rand_next$u64() >> 11ull)) / as$((f64)(1ull << 53ull));
}

f32 Rand_next$f32(void) {
    return as$((f32)(Rand_next$u32() >> 8u)) / as$((f32)(1u << 24u));
}

u64 Rand_rangeUInt(u64 min, u64 max) {
    return min + (Rand_nextUInt() % (max - min + 1));
}

i64 Rand_rangeInt(i64 min, i64 max) {
    return min + (Rand_nextInt() % (max - min + 1));
}

f64 Rand_rangeFlt(f64 min, f64 max) {
    return min + (Rand_nextFlt() * (max - min));
}
