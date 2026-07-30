#include "dh/hash.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

#define hash__wyhash_secret0 0xa0761d6478bd642full
#define hash__wyhash_secret1 0xe7037ed1a0b428dbull
#define hash__wyhash_secret2 0x8ebc6af09c88c6e3ull
#define hash__wyhash_secret3 0x589965cc75374cc3ull

$attr($inline_always)
$static fn_((hash__mum(u64* a, u64* b))(void));
$attr($inline_always)
$static fn_((hash__mix(u64 a, u64 b))(u64));
$attr($inline_always)
$static fn_((hash__readLE32(S_const$u8 bytes))(u32));
$attr($inline_always)
$static fn_((hash__readLE64(S_const$u8 bytes))(u64));

/*========== External Definitions ===========================================*/

fn_((hash_int$usize(usize input))(usize)) {
    return pp_if_(abi_size_is_64bit)(
        pp_then_(as$(usize)(hash_int$u64(as$(u64)(input)))),
        pp_else_(as$(usize)(hash_int$u32(as$(u32)(input)))));
};

fn_((hash_int$u64(u64 input))(u64)) {
    var x = input;
    let c = u64_(0xbea225f9eb34556dull);
    x = (x ^ (x >> 32)) * c;
    x = (x ^ (x >> 29)) * c;
    x = (x ^ (x >> 32)) * c;
    return x ^ (x >> 29);
};

fn_((hash_int$u32(u32 input))(u32)) {
    var x = input;
    x = (x ^ (x >> 17)) * u32_(0xed5ad4bbu);
    x = (x ^ (x >> 11)) * u32_(0xac4c1b51u);
    x = (x ^ (x >> 15)) * u32_(0x31848babu);
    return x ^ (x >> 14);
};

fn_((hash_int$u16(u16 input))(u16)) {
    var x = input;
    x = as$(u16)((x ^ (x >> 7)) * u16_(0x2993u));
    x = as$(u16)((x ^ (x >> 5)) * u16_(0xe877u));
    x = as$(u16)((x ^ (x >> 9)) * u16_(0x0235u));
    return as$(u16)(x ^ (x >> 10));
};

fn_((hash_int$u8(u8 input))(u8)) {
    var x = input;
    for_(($r(0, 4))(_)) {
        let_ignore = _;
        x = as$(u8)((x ^ (x >> 4)) * u8_(0x5bu));
    } $end(for);
    return x;
};

fn_((hash_fnv1a64(S_const$u8 input))(u64)) {
    debug_assert_nonnull(input.ptr);
    var hash = u64_(0xcbf29ce484222325ull);
    for_(($s(input))(byte)) {
        hash ^= *byte;
        hash *= u64_(0x100000001b3ull);
    } $end(for);
    return hash;
};

fn_((hash_fnv1a32(S_const$u8 input))(u32)) {
    debug_assert_nonnull(input.ptr);
    var hash = u32_(0x811c9dc5u);
    for_(($s(input))(byte)) {
        hash ^= *byte;
        hash *= u32_(0x01000193u);
    } $end(for);
    return hash;
};

fn_((hash_adler32(S_const$u8 input))(u32)) {
    debug_assert_nonnull(input.ptr);
    let base = u32_(65521u);
    var s1 = u32_(1u);
    var s2 = u32_(0u);
    for_(($s(input))(byte)) {
        s1 = (s1 + *byte) % base;
        s2 = (s2 + s1) % base;
    } $end(for);
    return s1 | (s2 << 16);
};

fn_((hash_wyhash(S_const$u8 input, u64 seed))(u64)) {
    debug_assert_nonnull(input.ptr);
    var a = u64_(0);
    var b = u64_(0);
    var state0 = seed ^ hash__mix(seed ^ hash__wyhash_secret0, hash__wyhash_secret1);
    var state1 = state0;
    var state2 = state0;

    if (input.len <= 16) {
        if (input.len >= 4) {
            let end = input.len - 4;
            let quarter = (input.len >> 3) << 2;
            a = (as$(u64)(hash__readLE32(S_slice((input)$r(0, 4)))) << 32)
              | as$(u64)(hash__readLE32(S_slice((input)$r(quarter, quarter + 4))));
            b = (as$(u64)(hash__readLE32(S_slice((input)$r(end, end + 4)))) << 32)
              | as$(u64)(hash__readLE32(S_slice((input)$r(end - quarter, end - quarter + 4))));
        } else if (input.len > 0) {
            a = (as$(u64)(*S_at((input)[0])) << 16)
              | (as$(u64)(*S_at((input)[input.len >> 1])) << 8)
              | as$(u64)(*S_at((input)[input.len - 1]));
        }
    } else {
        var i = usize_(0);
        if (input.len >= 48) {
            while (i + 48 < input.len) {
                state0 = hash__mix(
                    hash__readLE64(S_slice((input)$r(i + 0, i + 8))) ^ hash__wyhash_secret1,
                    hash__readLE64(S_slice((input)$r(i + 8, i + 16))) ^ state0
                );
                state1 = hash__mix(
                    hash__readLE64(S_slice((input)$r(i + 16, i + 24))) ^ hash__wyhash_secret2,
                    hash__readLE64(S_slice((input)$r(i + 24, i + 32))) ^ state1
                );
                state2 = hash__mix(
                    hash__readLE64(S_slice((input)$r(i + 32, i + 40))) ^ hash__wyhash_secret3,
                    hash__readLE64(S_slice((input)$r(i + 40, i + 48))) ^ state2
                );
                i += 48;
            }
            state0 ^= state1 ^ state2;
        }
        while (i + 16 < input.len) {
            state0 = hash__mix(
                hash__readLE64(S_slice((input)$r(i, i + 8))) ^ hash__wyhash_secret1,
                hash__readLE64(S_slice((input)$r(i + 8, i + 16))) ^ state0
            );
            i += 16;
        }
        a = hash__readLE64(S_slice((input)$r(input.len - 16, input.len - 8)));
        b = hash__readLE64(S_slice((input)$r(input.len - 8, input.len)));
    }

    a ^= hash__wyhash_secret1;
    b ^= state0;
    hash__mum(&a, &b);
    return hash__mix(a ^ hash__wyhash_secret0 ^ as$(u64)(input.len), b ^ hash__wyhash_secret1);
};

fn_((hash_bytes64(S_const$u8 input))(u64)) {
    return hash_wyhash(input, 0);
};

/*========== Internal Definitions ===========================================*/

fn_((hash__mum(u64* a, u64* b))(void)) {
#if defined(__SIZEOF_INT128__)
    let r = (unsigned __int128)(*a) * (unsigned __int128)(*b);
    *a = as$(u64)(r);
    *b = as$(u64)(r >> 64);
#else
    let al = as$(u64)(as$(u32)(*a));
    let ah = *a >> 32;
    let bl = as$(u64)(as$(u32)(*b));
    let bh = *b >> 32;
    let ll = al * bl;
    let lh = al * bh;
    let hl = ah * bl;
    let hh = ah * bh;
    let mid = (ll >> 32) + as$(u32)(lh) + as$(u32)(hl);
    *a = (mid << 32) | as$(u32)(ll);
    *b = hh + (lh >> 32) + (hl >> 32);
#endif
};

fn_((hash__mix(u64 a, u64 b))(u64)) {
    hash__mum(&a, &b);
    return a ^ b;
};

fn_((hash__readLE32(S_const$u8 bytes))(u32)) {
    let fixed = S_deref$((const mem_ReadLE32Buf)(bytes));
    return mem_readLE32(fixed);
};

fn_((hash__readLE64(S_const$u8 bytes))(u64)) {
    let fixed = S_deref$((const mem_ReadLE64Buf)(bytes));
    return mem_readLE64(fixed);
};
