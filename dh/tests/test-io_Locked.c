#include "dh-main.h"
#include "dh/io/Locked.h"
#include "dh/io/Fixed.h"

$static fn_((test_io_Locked__unlock(P$raw ctx))(void)) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    *P_at((calls)[0]) += 1;
};

TEST_fn_("io/Locked: noop and failing leases remain valid capabilities" $scope) {
    try_(TEST_expect(io_Locked_isValid(io_Locked_noop)));
    try_(TEST_expect(io_Locked_isValid(io_Locked_failing)));
    var lock = io_Locked_noop;
    io_Locked_unlock(&lock);
    try_(TEST_expect(!io_Locked_isValid(lock)));
} $unscoped(TEST_fn);

TEST_fn_("io/Locked: wrappers preserve streams and release associated lock" $scope) {
    var_(unlock_calls, usize) = 0;
    var lock = io_Locked_ensureValid((io_Locked){
        .ctx = &unlock_calls,
        .unlockFn = test_io_Locked__unlock,
    });
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("x")));
    var reader = io_Locked_Reader_adopt(
        io_Fixed_reader(&reader_impl), &lock
    );
    try_(TEST_expect(!io_Locked_isValid(lock)));
    try_(TEST_expect(io_Reader_isValid(io_Locked_reader(&reader))));
    try_(TEST_expect(try_(io_Reader_readByte(io_Locked_reader(&reader))) == u8_c('x')));
    io_Locked_Reader_unlock(&reader);
    try_(TEST_expect(!io_Locked_isValid(reader.lock)));
    try_(TEST_expect(!io_Reader_isValid(reader.inner)));
    try_(TEST_expect(unlock_calls == 1));

    var_(mem, A$$(1, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(
        io_Fixed_writing(A_ref$((S$u8)(mem)))
    );
    lock = io_Locked_ensureValid((io_Locked){
        .ctx = &unlock_calls,
        .unlockFn = test_io_Locked__unlock,
    });
    var writer = io_Locked_Writer_adopt(
        io_Fixed_writer(&writer_impl), &lock
    );
    try_(TEST_expect(!io_Locked_isValid(lock)));
    try_(TEST_expect(io_Writer_isValid(io_Locked_writer(&writer))));
    try_(io_Writer_writeByte(io_Locked_writer(&writer), u8_c('y')));
    try_(TEST_expect(*A_at((mem)[0]) == u8_c('y')));
    io_Locked_Writer_unlock(&writer);
    try_(TEST_expect(!io_Locked_isValid(writer.lock)));
    try_(TEST_expect(!io_Writer_isValid(writer.inner)));
    try_(TEST_expect(unlock_calls == 2));
} $unscoped(TEST_fn);
