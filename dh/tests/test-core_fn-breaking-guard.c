#include "dh-main.h"
#include "dh/io/stream.h"

#define test_core_fn_breaking_guard__enabled_outstream pp_false
$static fn_((ignorePrintln(S_const$u8 fmt, ...))(void)) {
    let_ignore = fmt;
}
$static let io_println = pp_if_(test_core_fn_breaking_guard__enabled_outstream)(
    pp_then_(io_stream_println),
    pp_else_(ignorePrintln));

$static fn_((checkFlag(void))(bool)) { return false; }
$static fn_((append(S$i32* list, i32 val))(void)) { *S_at((*list)[list->len++]) = val; }
TEST_fn_("core/fn: breaking guard" $guard) {
    T_use_A$(10, i32);
    var_(deferred_list, S$i32) = { .ptr = l0$((A$10$i32)).val, .len = 0 };

    using_() blk_defer {
        defer_({
            io_println(u8_l("defer1"));
            append(&deferred_list, 1);
        });

        io_println(u8_l("checkFlag: {:s}"), checkFlag() ? u8_l("true") : u8_l("false"));
        io_println(u8_l("before blk_defer"));

        if (checkFlag()) blk_defer {
            defer_({
                io_println(u8_l("defer2"));
                append(&deferred_list, 2);
            });
            io_println(u8_l("checkFlag is true"));
        } blk_deferral else blk_defer {
            defer_({
                io_println(u8_l("defer3"));
                append(&deferred_list, 3);
            });
            io_println(u8_l("checkFlag is false"));
        } blk_deferral;
        io_println(u8_l("after blk_defer"));

        defer_({
            io_println(u8_l("defer4"));
            append(&deferred_list, 4);
        });
        let something = expr_(i32 $guard)({
            defer_({
                io_println(u8_l("defer5"));
                append(&deferred_list, 5);
            });
            if (true) $break_(123);
            return_ok({});
        }) $unguarded(expr);
        io_println(u8_l("something: {:d}"), something);
    } blk_deferral;

    let expected_list = A_from$((i32){ 3, 5, 4, 1 });
    io_println(u8_l("{:z}: {:d}"), nameOf(S_len(deferred_list)), S_len(deferred_list));
    io_println(u8_l("{:z}: {:d}"), nameOf(A_len(expected_list)), A_len(expected_list));
    try_(TEST_expect(deferred_list.len == A_len(expected_list)));
    for_(($rf(0), $s(deferred_list), $a(expected_list))(i, deferred, expected)) {
        io_println(u8_l("deferred_list[{:d}]: {:d}"), i, *deferred);
        io_println(u8_l("expected_list[{:d}]: {:d}"), i, *expected);
        try_(TEST_expect(*deferred == *expected));
    } $end(for);
} $unguarded(TEST_fn);
