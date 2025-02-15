#include "dh/Str.h"

void Str_print(Str_const str) {
    if (!str.ptr) { return; }
    if (str.len <= 0) { return; }
    printf("%.*s", (i32)str.len, str.ptr);
}

void Str_println(Str_const str) {
    Str_print(str);
    ignore putchar('\n');
}

void TEST_string_literals(void) {
    /* Test different string literal types */
    let str_readonly = Str_l("Readonly byte slice"); /* [*]const u8 */
    let str_writable = Str_m("Writable byte slice"); /* []u8 */

    /* Print each string */
    Str_println(Str_l("Readonly string:"));
    Str_println(str_readonly);

    Str_println(Str_l("Writable string:"));
    Str_println(str_writable.as_const);

    debug_assert(!Str_constCast(str_readonly).has_value);
    debug_assert(Str_constCast(str_writable.as_const).has_value);

    /* UTF-8 test */
    Str_const utf8_str = Str_l("Hello, 世界! 안녕! 나는 왈도! 강하고 굳센 아침?");
    Str_println(Str_l("\nUTF-8 string:"));
    Str_println(utf8_str);

    printf("Byte length: %zu\n", Str_len(utf8_str));
    printf("Character count: %zu\n", StrUtf8_len(utf8_str));

    /* UTF-8 iteration test */
    Str_println(Str_l("\nIterating UTF-8 characters:"));
    {
        StrUtf8Iter iter      = StrUtf8_iter(utf8_str);
        Opt$u32     codepoint = cleared();
        while (StrUtf8Iter_next(&iter, &codepoint)) {
            if (codepoint.has_value) {
                // Print the actual Unicode character
                printf("%C", codepoint.value);
            }
            Str_print(Str_l(" "));
        }
        ignore putchar('\n');
    }

    Str_println(Str_l("할아버지 보고싶어요"));
}

int main(void) {
    TEST_string_literals();
    return 0;
}
