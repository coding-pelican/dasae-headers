#define main_no_args 1
#define main_no_returns_err 1
#include "dh/main.h"
#include "dh/io/stream.h"
#include "dh/Str.h"

$static fn_((TEST_string_literals(void))(void)) {
    /* Test different string literal types */
    let str_readonly = u8_l("Readonly byte slice"); /* [*]const u8 */
    let str_writable = u8_a("Writable byte slice"); /* []u8 */

    /* Print each string */
    io_stream_println(u8_l("Readonly string:"));
    io_stream_println(str_readonly);

    io_stream_println(u8_l("Writable string:"));
    io_stream_println(A_ref$((S_const$u8)(str_writable)));

#if UNUSED_CODE
    debug_assert(isNone(S_const$u8Cast(str_readonly)));
    debug_assert(isSome(S_const$u8Cast(str_writable.as_const)));
#endif /* UNUSED_CODE */

    /* UTF-8 test */
    S_const$u8 utf8_str = u8_l("Hello, 世界! 안녕! 나는 왈도! 강하고 굳센 아침?");
    io_stream_println(u8_l("\nUTF-8 string:"));
    io_stream_println(utf8_str);

    io_stream_print(u8_l("Byte length: {:uz}\n"), utf8_str.len);
    io_stream_print(u8_l("Character count: {:uz}\n"), StrUtf8_len(utf8_str));

    /* UTF-8 iteration test */
    io_stream_println(u8_l("\nIterating UTF-8 characters:"));
    {
        var iter = StrUtf8_iter(utf8_str);
        var codepoint = none$((O$u32));
        while (StrUtf8Iter_next(&iter, &codepoint)) {
            if_some((codepoint)(point)) {
                // Print the actual Unicode character
                io_stream_print(u8_l("{:C}"), point);
            }
            io_stream_print(u8_l(" "));
        }
        io_stream_nl();
    }

    io_stream_println(u8_l("할아버지 보고싶어요"));
};

fn_((dh_main(void))(void)) {
    TEST_string_literals();
};
