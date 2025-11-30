#include "dh/main.h"
#include "dh/io/stream.h"

$static fn_((checkFlag(void))(bool)) { return false; }
TEST_fn_("test breaking guard" $guard) {
    T_use_A$(10, i32);
    S$i32 deferred_list = { .ptr = lit0$((A$(10, i32))).val, .len = 0 };
    let append = la_((S$i32 * list, i32 val)(void)) { list->ptr[list->len++] = val; };

    blk_defer_({
        defer_({
            io_stream_println(u8_l("defer1"));
            append(&deferred_list, 1);
        });

        io_stream_println(u8_l("checkFlag: {:s}"), checkFlag() ? u8_l("true") : u8_l("false"));
        io_stream_println(u8_l("before blk_defer"));

        if (checkFlag()) blk_defer_({
            defer_({
                io_stream_println(u8_l("defer2"));
                append(&deferred_list, 2);
            });
            io_stream_println(u8_l("checkFlag is true"));
        }) blk_deferral else blk_defer_({
            defer_({
                io_stream_println(u8_l("defer3"));
                append(&deferred_list, 3);
            });
            io_stream_println(u8_l("checkFlag is false"));
        }) blk_deferral;
        io_stream_println(u8_l("after blk_defer"));

        defer_({
            io_stream_println(u8_l("defer4"));
            append(&deferred_list, 4);
        });
        let something = expr_(i32 $guard)({
            defer_({
                io_stream_println(u8_l("defer5"));
                append(&deferred_list, 5);
            });
            if (true) { $break_(123); }
            return_ok({});
        }) $unguarded_(expr);
        io_stream_println(u8_l("something: {:d}"), something);
    }) blk_deferral;

    let expected_list = A_from$((i32){ 3, 5, 4, 1 });
    io_stream_println(u8_l("{:z}: {:d}"), nameOf(S_len(deferred_list)), deferred_list.len);
    io_stream_println(u8_l("{:z}: {:d}"), nameOf(A_len(expected_list)), A_len(expected_list));
    try_(TEST_expect(deferred_list.len == A_len(expected_list)));
    for_(($rf(0), $s(deferred_list), $a(expected_list))(i, deferred, expected) {
        io_stream_println(u8_l("deferred_list[{:d}]: {:d}"), i, *deferred);
        io_stream_println(u8_l("expected_list[{:d}]: {:d}"), i, *expected);
        try_(TEST_expect(*deferred == *expected));
    });
} $unguarded_(TEST_fn);
