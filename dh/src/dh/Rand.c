#include "dh/Rand.h"
#include "dh/time/Instant.h"

/*========== Internal Declarations ==========================================*/

// xoroshiro128** (Blackman & Vigna, 2019): advance state, return scrambled output
$attr($inline_always)
$static fn_((Rand__next(Rand* self))(u64));

// Lemire's debiased bounded random in [0, range) — no modulo bias
$attr($inline_always)
$static fn_((Rand__bounded64(Rand* self, u64 range))(u64));
$attr($inline_always)
$static fn_((Rand__bounded32(Rand* self, u32 range))(u32));
$attr($inline_always)
$static fn_((Rand__bounded16(Rand* self, u16 range))(u16));
$attr($inline_always)
$static fn_((Rand__bounded8(Rand* self, u8 range))(u8));

/*========== External Definitions ===========================================*/

fn_((Rand_init(void))(Rand)) {
    return Rand_initSeed(time_Instant_ticks(time_Instant_now()));
};

fn_((Rand_initSeed(u64 seed))(Rand)) {
    return Rand_withSeed(Rand_default, seed);
};

fn_((Rand_withSeed(Rand self, u64 seed))(Rand)) {
    return with_((self)((.state)(seed)));
};

fn_((Rand_setSeed(Rand* self, u64 seed))(void)) {
    *self = Rand_withSeed(*self, seed);
};

fn_((Rand_nextUInt(Rand* self))(u64)) {
    return Rand_next$u64(self);
};

fn_((Rand_next$usize(Rand* self))(usize)) {
    // extract high sizeof(usize)*8 bits from 64-bit output
    return as$(usize)(Rand_nextUInt(self) >> (64u - sizeof(usize) * 8u));
};

fn_((Rand_next$u64(Rand* self))(u64)) {
    return Rand__next(self);
};

fn_((Rand_next$ulong(Rand* self))(ulong)) {
    // extract high sizeof(ulong)*8 bits from 64-bit output
    return as$(ulong)(Rand_nextUInt(self) >> (64u - sizeof(ulong) * 8u));
};

fn_((Rand_next$u32(Rand* self))(u32)) {
    return as$(u32)(Rand_nextUInt(self) >> 32u);
};

fn_((Rand_next$u16(Rand* self))(u16)) {
    return as$(u16)(Rand_nextUInt(self) >> 48u);
};

fn_((Rand_next$u8(Rand* self))(u8)) {
    return as$(u8)(Rand_nextUInt(self) >> 56u);
};

fn_((Rand_nextIInt(Rand* self))(i64)) {
    return Rand_next$i64(self);
};

fn_((Rand_next$isize(Rand* self))(isize)) {
    return bitCast$((isize)(Rand_next$usize(self)));
};

fn_((Rand_next$i64(Rand* self))(i64)) {
    return bitCast$((i64)(Rand_next$u64(self)));
};

fn_((Rand_next$ilong(Rand* self))(ilong)) {
    return bitCast$((ilong)(Rand_next$ulong(self)));
};

fn_((Rand_next$i32(Rand* self))(i32)) {
    return bitCast$((i32)(Rand_next$u32(self)));
};

fn_((Rand_next$i16(Rand* self))(i16)) {
    return bitCast$((i16)(Rand_next$u16(self)));
};

fn_((Rand_next$i8(Rand* self))(i8)) {
    return bitCast$((i8)(Rand_next$u8(self)));
};

fn_((Rand_nextFlt(Rand* self))(f64)) {
    return Rand_next$f64(self);
};

fn_((Rand_next$f64(Rand* self))(f64)) {
    // 53 mantissa bits in [0, 2^53-1], divide by 2^53-1 => [0.0, 1.0]
    return as$(f64)(Rand_nextUInt(self) >> 11u) / as$(f64)((1ull << 53) - 1ull);
};

fn_((Rand_next$f32(Rand* self))(f32)) {
    // 24 mantissa bits in [0, 2^24-1], divide by 2^24-1 => [0.0f, 1.0f]
    return as$(f32)(Rand_next$u32(self) >> 8u) / as$(f32)((1u << 24) - 1u);
};

// Unsigned integer [min, max)
fn_((Rand_rangeUInt(Rand* self, u64 min, u64 max))(u64)) {
    return min + Rand__bounded64(self, max - min);
};

fn_((Rand_range$usize(Rand* self, usize min, usize max))(usize)) {
    return min + intCast$((usize)(Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))));
};

fn_((Rand_range$u64(Rand* self, u64 min, u64 max))(u64)) {
    return min + Rand__bounded64(self, max - min);
};

fn_((Rand_range$ulong(Rand* self, ulong min, ulong max))(ulong)) {
    return min + intCast$((ulong)(Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))));
};

fn_((Rand_range$u32(Rand* self, u32 min, u32 max))(u32)) {
    return min + Rand__bounded32(self, max - min);
};

fn_((Rand_range$u16(Rand* self, u16 min, u16 max))(u16)) {
    return min + Rand__bounded16(self, as$(u16)(as$(u32)(max) - as$(u32)(min)));
};

fn_((Rand_range$u8(Rand* self, u8 min, u8 max))(u8)) {
    return min + Rand__bounded8(self, as$(u8)(as$(u32)(max) - as$(u32)(min)));
};

// Signed integer [min, max)
// Range computed as unsigned to handle all edge cases (including INT_MIN to INT_MAX).
// Result built by unsigned addition then bitcast — avoids signed overflow UB.
fn_((Rand_rangeIInt(Rand* self, i64 min, i64 max))(i64)) {
    return bitCast$((i64)(as$(u64)(min) + Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))));
};

fn_((Rand_range$isize(Rand* self, isize min, isize max))(isize)) {
    return bitCast$((isize)(intCast$((usize)(as$(u64)(as$(usize)(min))
                                             + Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))))));
};

fn_((Rand_range$i64(Rand* self, i64 min, i64 max))(i64)) {
    return bitCast$((i64)(as$(u64)(min) + Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))));
};

fn_((Rand_range$ilong(Rand* self, ilong min, ilong max))(ilong)) {
    return bitCast$((ilong)(intCast$((ulong)(as$(u64)(as$(ulong)(min))
                                             + Rand__bounded64(self, as$(u64)(max) - as$(u64)(min))))));
};

fn_((Rand_range$i32(Rand* self, i32 min, i32 max))(i32)) {
    return bitCast$((i32)(as$(u32)(min) + Rand__bounded32(self, as$(u32)(max) - as$(u32)(min))));
};

fn_((Rand_range$i16(Rand* self, i16 min, i16 max))(i16)) {
    return bitCast$((i16)(as$(u16)(as$(u16)(min) + Rand__bounded16(self, as$(u16)(as$(u32)(max) - as$(u32)(min))))));
};

fn_((Rand_range$i8(Rand* self, i8 min, i8 max))(i8)) {
    return bitCast$((i8)(as$(u8)(as$(u8)(min) + Rand__bounded8(self, as$(u8)(as$(u32)(max) - as$(u32)(min))))));
};

// Float [min, max]
fn_((Rand_rangeFlt(Rand* self, f64 min, f64 max))(f64)) {
    return min + Rand_nextFlt(self) * (max - min);
};

fn_((Rand_range$f64(Rand* self, f64 min, f64 max))(f64)) {
    return min + Rand_next$f64(self) * (max - min);
};

fn_((Rand_range$f32(Rand* self, f32 min, f32 max))(f32)) {
    return min + Rand_next$f32(self) * (max - min);
};

/*========== Internal Definitions ===========================================*/

// xoroshiro128** (Blackman & Vigna, 2019)
fn_((Rand__next(Rand* self))(u64)) {
    const u64 s0 = self->state;
    const u64 s1 = self->stream;
    const u64 result = int_rotateLeft(s0 * 5, as$(u64)(7)) * 9;
    const u64 s1_xor = s1 ^ s0;
    self->state = int_rotateLeft(s0, as$(u64)(24)) ^ s1_xor ^ (s1_xor << 16);
    self->stream = int_rotateLeft(s1_xor, as$(u64)(37));
    return result;
};

// Lemire (2019): multiply-then-reject — expected < 2 iterations for large ranges
fn_((Rand__bounded64(Rand* self, u64 range))(u64)) {
    typedef __uint128_t u128;
    u128 m = (u128)Rand_nextUInt(self) * (u128)range;
    u64 l = as$(u64)(m);
    if (l < range) {
        const u64 threshold = as$(u64)(-range) % range;
        while (l < threshold) {
            m = (u128)Rand_nextUInt(self) * (u128)range;
            l = as$(u64)(m);
        }
    }
    return as$(u64)(m >> 64u);
};

fn_((Rand__bounded32(Rand* self, u32 range))(u32)) {
    u64 m = as$(u64)(Rand_next$u32(self)) * as$(u64)(range);
    u32 l = as$(u32)(m);
    if (l < range) {
        const u32 threshold = as$(u32)(-range) % range;
        while (l < threshold) {
            m = as$(u64)(Rand_next$u32(self)) * as$(u64)(range);
            l = as$(u32)(m);
        }
    }
    return as$(u32)(m >> 32u);
};

fn_((Rand__bounded16(Rand* self, u16 range))(u16)) {
    u32 m = as$(u32)(Rand_next$u16(self)) * as$(u32)(range);
    u16 l = as$(u16)(m);
    if (l < range) {
        const u16 threshold = as$(u16)(-range) % range;
        while (l < threshold) {
            m = as$(u32)(Rand_next$u16(self)) * as$(u32)(range);
            l = as$(u16)(m);
        }
    }
    return as$(u16)(m >> 16u);
};

fn_((Rand__bounded8(Rand* self, u8 range))(u8)) {
    u16 m = as$(u16)(as$(u16)(Rand_next$u8(self)) * as$(u16)(range));
    u8 l = as$(u8)(m);
    if (l < range) {
        const u8 threshold = as$(u8)(-range) % range;
        while (l < threshold) {
            m = as$(u16)(as$(u16)(Rand_next$u8(self)) * as$(u16)(range));
            l = as$(u8)(m);
        }
    }
    return as$(u8)(m >> 8u);
};
