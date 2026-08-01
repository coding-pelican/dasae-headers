#include "dh-main.h"
#include "dh/io/std.h"

TEST_fn_("io/std: direct provider returns endpoint-bound recursive leases" $guard) {
    let std = catch_((io_std_direct())(
        $ignore, return_ok(try_(TEST_skipMsg(u8_l("direct standard I/O is unavailable"))))
    ));
    var outer = io_std_lockOut(std);
    defer_(io_Locked_Writer_unlock(outer));
    try_(TEST_expect(io_Writer_isValid(io_Locked_writer(outer))));

    let nested_maybe = io_std_tryLockOut(std);
    try_(TEST_expect(isSome(nested_maybe)));
    var nested = unwrap_(nested_maybe);
    defer_(io_Locked_Writer_unlock(nested));
    try_(TEST_expect(io_Writer_isValid(io_Locked_writer(nested))));
} $unguarded(TEST_fn);

TEST_fn_("io/std: noop provider supplies all coordinated endpoints" $guard) {
    try_(TEST_expect(io_std_isValid(io_std_noop)));
    try_(TEST_expect(io_Reader_isValid(io_std_in(io_std_noop))));
    try_(TEST_expect(io_Writer_isValid(io_std_out(io_std_noop))));
    try_(TEST_expect(io_Writer_isValid(io_std_err(io_std_noop))));

    var in = io_std_lockIn(io_std_noop);
    defer_(io_Locked_Reader_unlock(in));
    var out = io_std_lockOut(io_std_noop);
    defer_(io_Locked_Writer_unlock(out));
    var err = io_std_lockErr(io_std_noop);
    defer_(io_Locked_Writer_unlock(err));

    var try_in = unwrap_(io_std_tryLockIn(io_std_noop));
    defer_(io_Locked_Reader_unlock(try_in));
    var try_out = unwrap_(io_std_tryLockOut(io_std_noop));
    defer_(io_Locked_Writer_unlock(try_out));
    var try_err = unwrap_(io_std_tryLockErr(io_std_noop));
    defer_(io_Locked_Writer_unlock(try_err));
} $unguarded(TEST_fn);

TEST_fn_("io/std: failing provider keeps locking valid and fails I/O" $guard) {
    try_(TEST_expect(io_std_isValid(io_std_failing)));
    var out = io_std_lockOut(io_std_failing);
    defer_(io_Locked_Writer_unlock(out));
    let failed = eval_(bool $scope)(catch_((
        io_Writer_writeByte(io_Locked_writer(out), u8_c('x'))
    )($ignore, $break_(true)))) eval_(else)($break_(false)) $unscoped(eval);
    try_(TEST_expect(failed));
} $unguarded(TEST_fn);
