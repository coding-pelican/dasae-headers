#include "dh-main.h"
#include "dh/io/Limited.h"
#include "dh/io/Fixed.h"
#include "dh/mem/common.h"

TEST_fn_("io/Limited/Reader: limit bounds reads without consuming the inner tail" $scope) {
    var inner = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abcdef")));
    var limited = io_Limited_Reader_from(io_Fixed_reader(&inner), 4);
    let reader = io_Limited_reader(&limited);
    var_(out, A$$(3, u8)) $undefined;

    try_(TEST_expect(try_(io_Reader_read(reader, A_ref$((S$u8)(out)))) == 3));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(out)), u8_l("abc"))));
    try_(TEST_expect(try_(io_Reader_read(reader, A_ref$((S$u8)(out)))) == 1));
    try_(TEST_expect(*A_at((out)[0]) == u8_c('d')));
    try_(TEST_expect(io_Limited_Reader_remaining(limited) == 0));
    try_(TEST_expect(try_(io_Reader_read(reader, A_ref$((S$u8)(out)))) == 0));
    try_(TEST_expect(inner.stream.pos == 4));
    try_(TEST_expect(try_(io_Reader_readByte(io_Fixed_reader(&inner))) == u8_c('e')));
} $unscoped(TEST_fn);

TEST_fn_("io/Limited/Reader: source EOF preserves unused allowance" $scope) {
    var inner = io_Fixed_Reader_from(io_Fixed_reading(u8_l("ab")));
    var limited = io_Limited_Reader_from(io_Fixed_reader(&inner), 5);
    var_(out, A$$(4, u8)) $undefined;

    try_(TEST_expect(try_(io_Reader_read(
        io_Limited_reader(&limited), A_ref$((S$u8)(out))
    )) == 2));
    try_(TEST_expect(io_Limited_Reader_remaining(limited) == 3));
    try_(TEST_expect(try_(io_Reader_read(
        io_Limited_reader(&limited), A_ref$((S$u8)(out))
    )) == 0));
    try_(TEST_expect(io_Limited_Reader_remaining(limited) == 3));
} $unscoped(TEST_fn);

TEST_fn_("io/Limited/Reader: zero limit does not touch the inner reader" $scope) {
    var inner = io_Fixed_Reader_from(io_Fixed_reading(u8_l("x")));
    var limited = io_Limited_Reader_from(io_Fixed_reader(&inner), 0);
    var_(out, A$$(1, u8)) $undefined;

    try_(TEST_expect(try_(io_Reader_read(
        io_Limited_reader(&limited), A_ref$((S$u8)(out))
    )) == 0));
    try_(TEST_expect(inner.stream.pos == 0));
} $unscoped(TEST_fn);
