#define main_no_args        (1)
#define main_no_returns_err (1)
#include "dh/main.h"
#include "dh/Str.h"

static fn_(TEST_string_literals(void), void) {
    /* Test different string literal types */
    let str_readonly = Str_l("Readonly byte slice"); /* [*]const u8 */
    let str_writable = Str_m("Writable byte slice"); /* []u8 */

    /* Print each string */
    Str_println(Str_l("Readonly string:"));
    Str_println(str_readonly);

    Str_println(Str_l("Writable string:"));
    Str_println(str_writable.as_const);

    debug_assert(isNone(Str_constCast(str_readonly)));
    debug_assert(isSome(Str_constCast(str_writable.as_const)));

    /* UTF-8 test */
    Str_const utf8_str = Str_l("Hello, 世界! 안녕! 나는 왈도! 강하고 굳센 아침?");
    Str_println(Str_l("\nUTF-8 string:"));
    Str_println(utf8_str);

    printf("Byte length: %zu\n", Str_len(utf8_str));
    printf("Character count: %zu\n", StrUtf8_len(utf8_str));

    /* UTF-8 iteration test */
    Str_println(Str_l("\nIterating UTF-8 characters:"));
    {
        var iter      = StrUtf8_iter(utf8_str);
        var codepoint = make$(Opt$u32);
        while (StrUtf8Iter_next(&iter, &codepoint)) {
            if_some(codepoint, point) {
                // Print the actual Unicode character
                printf("%C", point);
            }
            Str_print(Str_l(" "));
        }
        $ignore = putchar('\n');
    }

    Str_println(Str_l("할아버지 보고싶어요"));
}

fn_(dh_main(void), void) {
    TEST_string_literals();
}
