#include "dh/Str.h"

void TEST_string_literals(void) {
    /* Test different string literal types */
    CStrConst c_style        = cstrL("Null-terminated");           /* [*:0]const u8 */
    StrConst  slice_style    = strL("Just a slice");               /* []const u8 */
    StrSConst sentinel_slice = strSL("Sentinel-terminated slice"); /* [:0]const u8 */

    /* Print each string */
    Str_println(strL("C-style string:"));
    Str_println((StrConst){ .base = mem_span(typeInfo(const u8), c_style.base) });

    Str_println(strL("Slice string:"));
    Str_println(slice_style);

    Str_println(strL("Sentinel slice string:"));
    Str_println((StrConst){ .base = sentinel_slice.data.base });

    /* UTF-8 test */
    StrConst utf8_str = strL("Hello, 世界! 안녕하세요!");
    Str_println(strL("\nUTF-8 string:"));
    Str_println(utf8_str);

    printf("Byte length: %zu\n", Str_len(utf8_str));
    printf("Character count: %zu\n", Str_utf8Len(utf8_str));

    /* UTF-8 iteration test */
    Str_println(strL("\nIterating UTF-8 characters:"));
    {
        StrIter  iter = Str_iter(utf8_str);
        StrConst ch   = cleared();
        while (Str_next(&iter, &ch)) {
            Str_print(ch);
            Str_print(strL(" "));
        }
        putchar('\n');
    }
}


int main(void) {
    TEST_string_literals();
    return 0;
}
