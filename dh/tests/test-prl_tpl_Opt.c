/* TODO: Make this to test cases */
#if UNUSED_CODE
#define main_no_args 1
#define main_no_returns_err 1
#include "dh-main.h"
#include "dh/io/stream.h"

$static fn_((getValueIfPositive(i32 val))(O$i32) $scope) {
    if (val <= 0) return_none();
    return_some(val);
} $unscoped(fn);

$static fn_((getCharIfNumeric(u8 val))(O$u8) $scope) {
    if ('0' <= val && val <= '9') return_some(val);
    return_none();
} $unscoped(fn);

fn_((main(void))(void)) {
    if_some((getValueIfPositive(10))(value)) {
        io_stream_println(u8_l("Value is {:d}"), value);
    } else_none {
        io_stream_println(u8_l("Value is not positive"));
    };

    if_some((getCharIfNumeric('a'))(value)) {
        io_stream_println(u8_l("Value is {:c}"), value);
    } else_none {
        io_stream_println(u8_l("Value is not numeric"));
    };

    if_none((getValueIfPositive(-10))) {
        io_stream_println(u8_l("Value is not positive"));
    } else_some(value) {
        io_stream_println(u8_l("Value is {:d}"), value);
    };

    if_none((getCharIfNumeric('9'))) {
        io_stream_println(u8_l("Value is not numeric"));
    } else_some(value) {
        io_stream_println(u8_l("Value is {:c}"), value);
    };
};

#endif /* UNUSED_CODE */
