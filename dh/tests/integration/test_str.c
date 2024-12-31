#include "dh/Str.h"

void Str_print(StrConst str) {
    if (!str.ptr) { return; }
    if (str.len <= 0) { return; }
    printf("%.*s", (i32)str.len, str.ptr);
}

void Str_println(StrConst str) {
    Str_print(str);
    putchar('\n');
}

void TEST_string_literals(void) {
    /* Test different string literal types */
    StrConst str_readonly = strL("Readonly byte slice"); /* [*]const u8 */
    Str      str_writable = strM("Writable byte slice"); /* []u8 */

    /* Print each string */

    Str_println(strL("Readonly string:"));
    Str_println(str_readonly);

    Str_println(strL("Writable string:"));
    Str_println(str_writable.as_const);

    /* UTF-8 test */
    StrConst utf8_str = strL("Hello, 世界! 안녕! 나는 왈도! 강하고 굳센 아침?");
    Str_println(strL("\nUTF-8 string:"));
    Str_println(utf8_str);

    printf("Byte length: %zu\n", Str_len(utf8_str));
    printf("Character count: %zu\n", StrUtf8_len(utf8_str));

    /* UTF-8 iteration test */
    Str_println(strL("\nIterating UTF-8 characters:"));
    {
        StrUtf8Iter iter      = StrUtf8_iter(utf8_str);
        Opt$u32     codepoint = cleared();
        while (StrUtf8Iter_next(&iter, &codepoint)) {
            if (codepoint.has_value) {
                // Print the actual Unicode character
                printf("%C", codepoint.value);
            }
            Str_print(strL(" "));
        }
        putchar('\n');
    }

    Str_println(strL("할아버지 보고싶어요"));
}

int main(void) {
    TEST_string_literals();
    return 0;
}
