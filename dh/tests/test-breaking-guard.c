#include "dh/main.h"
#include "dh/io/stream.h"
#include "dh/Arr.h"
#include "dh/sli.h"

static bool checkFlag(void) { return false; }
use_Arr$(10, i32);
TEST_fn_("test breaking guard" $guard) {
    // use_Arr$(10, i32) arr = Arr_zero();
    // var deferred_list = Sli_from$(Sli$i32, arr.buf, 0);
    // Sli$i32 deferred_list = { .ptr = ({ Arr$$(10, i32) a = {}; a; }).buf, .len = 0 };
    // Sli$i32 deferred_list = { .ptr = (&copy(({ Arr$(10, i32) a = {}; a; })))->buf, .len = 0 };
    Sli$i32 deferred_list = { .ptr = ((Arr$(10, i32)){}).buf, .len = 0 };
    let append = la_((Sli$i32* list, i32 val)(void)) { list->ptr[list->len++] = val; };

    block_defer {
        defer_({
            io_stream_println(u8_l("defer1"));
            append(&deferred_list, 1);
        });

        io_stream_println(u8_l("checkFlag: {:s}"), checkFlag() ? u8_l("true") : u8_l("false"));
        io_stream_println(u8_l("before block_defer"));

        if (checkFlag()) block_defer {
            defer_({
                io_stream_println(u8_l("defer2"));
                append(&deferred_list, 2);
            });
            io_stream_println(u8_l("checkFlag is true"));
        } block_deferral else block_defer {
            defer_({
                io_stream_println(u8_l("defer3"));
                append(&deferred_list, 3);
            });
            io_stream_println(u8_l("checkFlag is false"));
        } block_deferral;
        io_stream_println(u8_l("after block_defer"));

        defer_({
            io_stream_println(u8_l("defer4"));
            append(&deferred_list, 4);
        });
        let something = eval_(i32 $guard) {
            defer_({
                io_stream_println(u8_l("defer5"));
                append(&deferred_list, 5);
            });
            if (true) { $break_(123); }
            return_ok({});
        } $unguarded_($eval);
        io_stream_println(u8_l("something: {:d}"), something);
    } block_deferral;

    let expected_list = Arr_from$(i32, { 3, 5, 4, 1 });
    io_stream_println(u8_l("{:z}: {:d}"), nameOf(deferred_list.len), deferred_list.len);
    io_stream_println(u8_l("{:z}: {:d}"), nameOf(Arr_len(expected_list)), Arr_len(expected_list));
    try_(TEST_expect(deferred_list.len == Arr_len(expected_list)));
    for (usize i = 0; i < deferred_list.len; ++i) {
        io_stream_println(u8_l("deferred_list[{:d}]: {:d}"), i, *Sli_at(deferred_list, i));
        io_stream_println(u8_l("expected_list[{:d}]: {:d}"), i, *Arr_at(expected_list, i));
        try_(TEST_expect(*Sli_at(deferred_list, i) == *Arr_at(expected_list, i)));
    }
} $unguarded_(TEST_fn);
