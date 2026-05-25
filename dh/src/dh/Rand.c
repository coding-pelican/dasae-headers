#include "dh/Rand.h"
#include "dh/time/Instant.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((Rand__next(Rand* self))(u64));
$attr($inline_always)
$static fn_((Rand__splitMix64(u64* seed))(u64));

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
    var split_seed = seed;
    self.state = Rand__splitMix64(&split_seed);
    self.stream = Rand__splitMix64(&split_seed);
    return self;
};

fn_((Rand_setSeed(Rand* self, u64 seed))(void)) {
    *self = Rand_withSeed(*self, seed);
};

fn_((Rand_fillBytes(Rand* self, S$u8 buf))(S$u8)) {
    for_(($s(buf))(byte)) { *byte = Rand_next$u8(self); } $end(for);
    return buf;
};

fn_((Rand_boolean(Rand* self))(bool)) {
    return (Rand_next$u8(self) & 1u) != 0;
};

fn_((Rand_nextUInt(Rand* self))(u64)) {
    return Rand_next$u64(self);
};

fn_((Rand_next$usize(Rand* self))(usize)) {
    return as$(usize)(Rand_nextUInt(self) >> (64u - sizeof(usize) * 8u));
};

fn_((Rand_next$u64(Rand* self))(u64)) {
    return Rand__next(self);
};

fn_((Rand_next$ulong(Rand* self))(ulong)) {
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
    return as$(f64)(Rand_nextUInt(self) >> 11u) / as$(f64)((1ull << 53) - 1ull);
};

fn_((Rand_next$f32(Rand* self))(f32)) {
    return as$(f32)(Rand_next$u32(self) >> 8u) / as$(f32)((1u << 24) - 1u);
};

fn_((Rand_lessThanUInt(Rand* self, u64 less_than))(u64)) {
    return Rand_lessThan$u64(self, less_than);
};

fn_((Rand_lessThan$usize(Rand* self, usize less_than))(usize)) {
    claim_assert(0 < less_than);
    return Rand_range$usize(self, Range_inclExcl$(usize, 0, less_than));
};

fn_((Rand_lessThan$u64(Rand* self, u64 less_than))(u64)) {
    claim_assert(0 < less_than);
    return Rand_range$u64(self, Range_inclExcl$(u64, 0, less_than));
};

fn_((Rand_lessThan$ulong(Rand* self, ulong less_than))(ulong)) {
    claim_assert(0 < less_than);
    return Rand_range$ulong(self, Range_inclExcl$(ulong, 0, less_than));
};

fn_((Rand_lessThan$u32(Rand* self, u32 less_than))(u32)) {
    claim_assert(0 < less_than);
    return Rand_range$u32(self, Range_inclExcl$(u32, 0, less_than));
};

fn_((Rand_lessThan$u16(Rand* self, u16 less_than))(u16)) {
    claim_assert(0 < less_than);
    return Rand_range$u16(self, Range_inclExcl$(u16, 0, less_than));
};

fn_((Rand_lessThan$u8(Rand* self, u8 less_than))(u8)) {
    claim_assert(0 < less_than);
    return Rand_range$u8(self, Range_inclExcl$(u8, 0, less_than));
};

fn_((Rand_atMostUInt(Rand* self, u64 at_most))(u64)) {
    return Rand_atMost$u64(self, at_most);
};

fn_((Rand_atMost$usize(Rand* self, usize at_most))(usize)) {
    return Rand_range$usize(self, Range_inclIncl$(usize, 0, at_most));
};

fn_((Rand_atMost$u64(Rand* self, u64 at_most))(u64)) {
    return Rand_range$u64(self, Range_inclIncl$(u64, 0, at_most));
};

fn_((Rand_atMost$ulong(Rand* self, ulong at_most))(ulong)) {
    return Rand_range$ulong(self, Range_inclIncl$(ulong, 0, at_most));
};

fn_((Rand_atMost$u32(Rand* self, u32 at_most))(u32)) {
    return Rand_range$u32(self, Range_inclIncl$(u32, 0, at_most));
};

fn_((Rand_atMost$u16(Rand* self, u16 at_most))(u16)) {
    return Rand_range$u16(self, Range_inclIncl$(u16, 0, at_most));
};

fn_((Rand_atMost$u8(Rand* self, u8 at_most))(u8)) {
    return Rand_range$u8(self, Range_inclIncl$(u8, 0, at_most));
};

fn_((Rand_rangeUInt(Rand* self, Range$u64 range))(u64)) {
    return Rand_range$u64(self, range);
};

fn_((Rand_range$usize(Rand* self, Range$usize range))(usize)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(usize));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi -= 1;
    }

    claim_assert(lo <= hi);
    let width = hi - lo + 1;
    if (width == 0) return Rand_next$usize(self);
    return lo + intCast$((usize)(Rand__bounded64(self, as$(u64)(width))));
};

fn_((Rand_range$u64(Rand* self, Range$u64 range))(u64)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(u64));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi -= 1;
    }

    claim_assert(lo <= hi);
    let width = hi - lo + 1;
    if (width == 0) return Rand_next$u64(self);
    return lo + Rand__bounded64(self, width);
};

fn_((Rand_range$ulong(Rand* self, Range$ulong range))(ulong)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(ulong));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi -= 1;
    }

    claim_assert(lo <= hi);
    let width = hi - lo + 1;
    if (width == 0) return Rand_next$ulong(self);
    return lo + intCast$((ulong)(Rand__bounded64(self, as$(u64)(width))));
};

fn_((Rand_range$u32(Rand* self, Range$u32 range))(u32)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(u32));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi -= 1;
    }

    claim_assert(lo <= hi);
    let width = hi - lo + 1;
    if (width == 0) return Rand_next$u32(self);
    return lo + Rand__bounded32(self, width);
};

fn_((Rand_range$u16(Rand* self, Range$u16 range))(u16)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(u16));
        lo = as$(u16)(lo + 1);
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi = as$(u16)(hi - 1);
    }

    claim_assert(lo <= hi);
    let width = as$(u32)(hi) - as$(u32)(lo) + 1;
    if (width == as$(u32)(uint_limit$(u16)) + 1) return Rand_next$u16(self);
    return as$(u16)(lo + Rand__bounded16(self, as$(u16)(width)));
};

fn_((Rand_range$u8(Rand* self, Range$u8 range))(u8)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != uint_limit$(u8));
        lo = as$(u8)(lo + 1);
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != 0);
        hi = as$(u8)(hi - 1);
    }

    claim_assert(lo <= hi);
    let width = as$(u16)(hi) - as$(u16)(lo) + 1;
    if (width == as$(u16)(uint_limit$(u8)) + 1) return Rand_next$u8(self);
    return as$(u8)(lo + Rand__bounded8(self, as$(u8)(width)));
};

fn_((Rand_rangeIInt(Rand* self, Range$i64 range))(i64)) {
    return Rand_range$i64(self, range);
};

fn_((Rand_range$isize(Rand* self, Range$isize range))(isize)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(isize));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(isize));
        hi -= 1;
    }

    claim_assert(lo <= hi);
    if (lo == int_limit_min$(isize) && hi == int_limit_max$(isize)) return Rand_next$isize(self);
    let lo_raw = bitCast$((usize)(lo));
    let hi_raw = bitCast$((usize)(hi));
    let width = hi_raw - lo_raw + 1;
    if (width == 0) return Rand_next$isize(self);
    let val = lo_raw + intCast$((usize)(Rand__bounded64(self, as$(u64)(width))));
    return bitCast$((isize)(val));
};

fn_((Rand_range$i64(Rand* self, Range$i64 range))(i64)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(i64));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(i64));
        hi -= 1;
    }

    claim_assert(lo <= hi);
    if (lo == int_limit_min$(i64) && hi == int_limit_max$(i64)) return Rand_next$i64(self);
    let lo_raw = bitCast$((u64)(lo));
    let hi_raw = bitCast$((u64)(hi));
    let width = hi_raw - lo_raw + 1;
    if (width == 0) return Rand_next$i64(self);
    let val = lo_raw + Rand__bounded64(self, width);
    return bitCast$((i64)(val));
};

fn_((Rand_range$ilong(Rand* self, Range$ilong range))(ilong)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(ilong));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(ilong));
        hi -= 1;
    }

    claim_assert(lo <= hi);
    if (lo == int_limit_min$(ilong) && hi == int_limit_max$(ilong)) return Rand_next$ilong(self);
    let lo_raw = bitCast$((ulong)(lo));
    let hi_raw = bitCast$((ulong)(hi));
    let width = hi_raw - lo_raw + 1;
    if (width == 0) return Rand_next$ilong(self);
    let val = lo_raw + intCast$((ulong)(Rand__bounded64(self, as$(u64)(width))));
    return bitCast$((ilong)(val));
};

fn_((Rand_range$i32(Rand* self, Range$i32 range))(i32)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(i32));
        lo += 1;
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(i32));
        hi -= 1;
    }

    claim_assert(lo <= hi);
    if (lo == int_limit_min$(i32) && hi == int_limit_max$(i32)) return Rand_next$i32(self);
    let lo_raw = bitCast$((u32)(lo));
    let hi_raw = bitCast$((u32)(hi));
    let width = hi_raw - lo_raw + 1;
    if (width == 0) return Rand_next$i32(self);
    let val = lo_raw + Rand__bounded32(self, width);
    return bitCast$((i32)(val));
};

fn_((Rand_range$i16(Rand* self, Range$i16 range))(i16)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(i16));
        lo = as$(i16)(lo + 1);
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(i16));
        hi = as$(i16)(hi - 1);
    }

    claim_assert(lo <= hi);
    let lo_raw = bitCast$((u16)(lo));
    let hi_raw = bitCast$((u16)(hi));
    if (lo_raw == u16_(0x8000u) && hi_raw == u16_(0x7fffu)) return Rand_next$i16(self);
    let width = as$(u16)(hi_raw - lo_raw + 1);
    if (width == 0) return Rand_next$i16(self);
    let val = as$(u16)(lo_raw + Rand__bounded16(self, width));
    return bitCast$((i16)(val));
};

fn_((Rand_range$i8(Rand* self, Range$i8 range))(i8)) {
    var lo = range.begin.point;
    if (range.begin.bound == Limit_Bound_excl) {
        claim_assert(lo != int_limit_max$(i8));
        lo = as$(i8)(lo + 1);
    }

    var hi = range.end.point;
    if (range.end.bound == Limit_Bound_excl) {
        claim_assert(hi != int_limit_min$(i8));
        hi = as$(i8)(hi - 1);
    }

    claim_assert(lo <= hi);
    let lo_raw = bitCast$((u8)(lo));
    let hi_raw = bitCast$((u8)(hi));
    if (lo_raw == u8_(0x80u) && hi_raw == u8_(0x7fu)) return Rand_next$i8(self);
    let width = as$(u8)(hi_raw - lo_raw + 1);
    if (width == 0) return Rand_next$i8(self);
    let val = as$(u8)(lo_raw + Rand__bounded8(self, width));
    return bitCast$((i8)(val));
};

fn_((Rand_rangeFlt(Rand* self, Range$f64 range))(f64)) {
    return Rand_range$f64(self, range);
};

fn_((Rand_range$f64(Rand* self, Range$f64 range))(f64)) {
    var lo = range.begin.point;
    var hi = range.end.point;

    if (range.begin.bound == Limit_Bound_excl) lo = flt_nextAfter(lo, flt_inf_pstv$(f64));
    if (range.end.bound == Limit_Bound_excl) hi = flt_nextAfter(hi, flt_inf_ngtv$(f64));

    claim_assert(lo <= hi);
    if (lo == hi) return lo;
    return lo + Rand_next$f64(self) * (hi - lo);
};

fn_((Rand_range$f32(Rand* self, Range$f32 range))(f32)) {
    var lo = range.begin.point;
    var hi = range.end.point;

    if (range.begin.bound == Limit_Bound_excl) lo = flt_nextAfter(lo, flt_inf_pstv$(f32));
    if (range.end.bound == Limit_Bound_excl) hi = flt_nextAfter(hi, flt_inf_ngtv$(f32));

    claim_assert(lo <= hi);
    if (lo == hi) return lo;
    return lo + Rand_next$f32(self) * (hi - lo);
};

/*========== Internal Definitions ===========================================*/

fn_((Rand__next(Rand* self))(u64)) {
    let s0 = self->state;
    let s1 = self->stream;
    let result = int_rotateLeft(s0 * 5, as$(u64)(7)) * 9;
    let s1_xor = s1 ^ s0;

    self->state = int_rotateLeft(s0, as$(u64)(24)) ^ s1_xor ^ (s1_xor << 16);
    self->stream = int_rotateLeft(s1_xor, as$(u64)(37));
    return result;
};

fn_((Rand__splitMix64(u64* seed))(u64)) {
    *seed += u64_(0x9e3779b97f4a7c15ull);
    var z = *seed;
    z = (z ^ (z >> 30)) * u64_(0xbf58476d1ce4e5b9ull);
    z = (z ^ (z >> 27)) * u64_(0x94d049bb133111ebull);
    return z ^ (z >> 31);
};

fn_((Rand__bounded64(Rand* self, u64 range))(u64)) {
    claim_assert(0 < range);

#if defined(__SIZEOF_INT128__)
    typedef __uint128_t u128;
    var m = as$(u128)(Rand_next$u64(self)) * as$(u128)(range);
    var l = as$(u64)(m);
    if (l < range) {
        let threshold = as$(u64)(-range) % range;
        while (l < threshold) {
            m = as$(u128)(Rand_next$u64(self)) * as$(u128)(range);
            l = as$(u64)(m);
        }
    }
    return as$(u64)(m >> 64u);
#else
    let threshold = as$(u64)(-range) % range;
    var r = Rand_next$u64(self);
    while (r < threshold) {
        r = Rand_next$u64(self);
    }
    return r % range;
#endif
};

fn_((Rand__bounded32(Rand* self, u32 range))(u32)) {
    claim_assert(0 < range);

    var m = as$(u64)(Rand_next$u32(self)) * as$(u64)(range);
    var l = as$(u32)(m);
    if (l < range) {
        let threshold = as$(u32)(-range) % range;
        while (l < threshold) {
            m = as$(u64)(Rand_next$u32(self)) * as$(u64)(range);
            l = as$(u32)(m);
        }
    }
    return as$(u32)(m >> 32u);
};

fn_((Rand__bounded16(Rand* self, u16 range))(u16)) {
    claim_assert(0 < range);

    var m = as$(u32)(Rand_next$u16(self)) * as$(u32)(range);
    var l = as$(u16)(m);
    if (l < range) {
        let threshold = as$(u16)(-range) % range;
        while (l < threshold) {
            m = as$(u32)(Rand_next$u16(self)) * as$(u32)(range);
            l = as$(u16)(m);
        }
    }
    return as$(u16)(m >> 16u);
};

fn_((Rand__bounded8(Rand* self, u8 range))(u8)) {
    claim_assert(0 < range);

    var m = as$(u16)(as$(u16)(Rand_next$u8(self)) * as$(u16)(range));
    var l = as$(u8)(m);
    if (l < range) {
        let threshold = as$(u8)(-range) % range;
        while (l < threshold) {
            m = as$(u16)(as$(u16)(Rand_next$u8(self)) * as$(u16)(range));
            l = as$(u8)(m);
        }
    }
    return as$(u8)(m >> 8u);
};
