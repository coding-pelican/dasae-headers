#include "dh-main.h"
#include "dh/hash.h"

TEST_fn_("hash: integer mixers match Zig vectors" $scope) {
    try_(TEST_expect(hash_int$u8(1) == u8_(0xd6u)));
    try_(TEST_expect(hash_int$u16(1) == u16_(0x2880u)));
    try_(TEST_expect(hash_int$u32(1) == u32_(0x042741d6u)));
    try_(TEST_expect(hash_int$u64(1) == u64_(0x0071894de00d9981full)));
} $unscoped(TEST_fn)

TEST_fn_("hash: fnv1a vectors" $scope) {
    try_(TEST_expect(hash_fnv1a32(u8_l("")) == u32_(0x811c9dc5u)));
    try_(TEST_expect(hash_fnv1a32(u8_l("a")) == u32_(0xe40c292cu)));
    try_(TEST_expect(hash_fnv1a32(u8_l("foobar")) == u32_(0xbf9cf968u)));

    try_(TEST_expect(hash_fnv1a64(u8_l("")) == u64_(0xcbf29ce484222325ull)));
    try_(TEST_expect(hash_fnv1a64(u8_l("a")) == u64_(0xaf63dc4c8601ec8cull)));
    try_(TEST_expect(hash_fnv1a64(u8_l("foobar")) == u64_(0x85944171f73967e8ull)));
} $unscoped(TEST_fn)

TEST_fn_("hash: adler32 vectors" $scope) {
    try_(TEST_expect(hash_adler32(u8_l("a")) == u32_(0x00620062u)));
    try_(TEST_expect(hash_adler32(u8_l("example")) == u32_(0x0bc002edu)));
} $unscoped(TEST_fn)

TEST_fn_("hash: wyhash vectors" $scope) {
    try_(TEST_expect(hash_wyhash(u8_l(""), 0) == u64_(0x0409638ee2bde459ull)));
    try_(TEST_expect(hash_wyhash(u8_l("a"), 1) == u64_(0xa8412d091b5fe0a9ull)));
    try_(TEST_expect(hash_wyhash(u8_l("abc"), 2) == u64_(0x32dd92e4b2915153ull)));
    try_(TEST_expect(hash_wyhash(u8_l("message digest"), 3) == u64_(0x8619124089a3a16bull)));
    try_(TEST_expect(hash_wyhash(u8_l("abcdefghijklmnopqrstuvwxyz"), 4) == u64_(0x7a43afb61d7f5f40ull)));
    try_(TEST_expect(hash_wyhash(u8_l("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), 5) == u64_(0xff42329b90e50d58ull)));
    try_(TEST_expect(hash_wyhash(u8_l("12345678901234567890123456789012345678901234567890123456789012345678901234567890"), 6) == u64_(0xc39cab13b115aad3ull)));
} $unscoped(TEST_fn)
