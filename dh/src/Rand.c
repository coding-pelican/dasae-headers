/*========== Includes =======================================================*/

#include "dh/Rand.h"
#include "dh/time/Instant.h"

/*========== Static Function Prototypes =====================================*/

// Using xorshiro256** algorithm
$inline_always
$static fn_((Rand__xorshiro256(u64 x, u32 k))(u64));
// Generate next random number
$inline_always
$static fn_((Rand__next(Rand* self))(u64));

/*========== Extern Function Implementations ================================*/

fn_((Rand_init(void))(Rand)) {
    return Rand_initSeed(time_Instant_toUnixEpoch(time_Instant_now()));
}

fn_((Rand_initSeed(u64 seed))(Rand)) {
    var rng = Rand_default;
    rng.state = seed;
    return rng;
}

fn_((Rand_withSeed(Rand self, u64 seed))(Rand)) {
    self.state = seed;
    return self;
}

fn_((Rand_setSeed(Rand* self, u64 seed))(void)) {
    *self = Rand_withSeed(*self, seed);
}

fn_((Rand_nextUInt(Rand* self))(u64)) {
    return Rand_next$u64(self);
}

fn_((Rand_next$usize(Rand* self))(usize)) {
    return as$(usize)(Rand_nextUInt(self)); /* TODO: Precise conversion */
}

fn_((Rand_next$u64(Rand* self))(u64)) {
    return Rand__next(self);
}

fn_((Rand_next$u32(Rand* self))(u32)) {
    return as$(u32)(Rand_nextUInt(self) & 0xFFFFFFFFu);
}

fn_((Rand_next$u16(Rand* self))(u16)) {
    return as$(u16)(Rand_nextUInt(self) & 0xFFFFu);
}

fn_((Rand_next$u8(Rand* self))(u8)) {
    return as$(u8)(Rand_nextUInt(self) & 0xFFu);
}

fn_((Rand_nextIInt(Rand* self))(i64)) {
    return Rand_next$i64(self);
}

fn_((Rand_next$isize(Rand* self))(isize)) {
    return as$(isize)(Rand_nextIInt(self)); /* TODO: Precise conversion */
}

fn_((Rand_next$i64(Rand* self))(i64)) {
    return as$(i64)(Rand_next$u64(self));
}

fn_((Rand_next$i32(Rand* self))(i32)) {
    return as$(i32)(Rand_nextIInt(self));
}

fn_((Rand_next$i16(Rand* self))(i16)) {
    return as$(i16)(Rand_nextIInt(self));
}

fn_((Rand_next$i8(Rand* self))(i8)) {
    return as$(i8)(Rand_nextIInt(self));
}

fn_((Rand_nextFlt(Rand* self))(f64)) {
    return Rand_next$f64(self);
}

fn_((Rand_next$f64(Rand* self))(f64)) {
    return as$(f64)(Rand_next$u64(self) >> 11ull) / as$(f64)(1ull << 53ull);
}

fn_((Rand_next$f32(Rand* self))(f32)) {
    return as$(f32)(Rand_next$u32(self) >> 8u) / as$(f32)(1u << 24u);
}

fn_((Rand_rangeUInt(Rand* self, u64 min, u64 max))(u64)) {
    return min + (Rand_nextUInt(self) % (max - min + 1));
}

fn_((Rand_rangeIInt(Rand* self, i64 min, i64 max))(i64)) {
    return min + (Rand_nextIInt(self) % (max - min + 1));
}

fn_((Rand_rangeFlt(Rand* self, f64 min, f64 max))(f64)) {
    return min + (Rand_nextFlt(self) * (max - min));
}

/*========== Static Function Implementations ================================*/

$inline_always
$static fn_((Rand__xorshiro256(u64 x, u32 k))(u64)) {
    return (x << k) | (x >> (64ull - k));
}

$inline_always
$static fn_((Rand__next(Rand* self))(u64)) {
    const u64 next = Rand__xorshiro256(self->state * 5, 7) * 9;
    const u64 t = self->stream << 17ull;

    self->stream ^= self->state;
    self->state ^= self->stream;
    self->stream ^= t;
    self->state = Rand__xorshiro256(self->state, 45);

    return next;
}
