#include "dh/Ptr.h"
#include "dh/Sli.h"
#include "dh/Str.h"

#include <stdio.h>

void fmt_printCh(u8 value) {
    putchar(value);
}

void fmt_printlnCh(u8 value) {
    putchar(value);
    putchar('\n');
}

void fmt_printStr(SliConst_u8 str) {
    for (usize i = 0; i < str.len; ++i) {
        putchar(SliConst_u8_at(str, i)->value);
    }
}

void fmt_printlnStr(SliConst_u8 str) {
    for (usize i = 0; i < str.len; ++i) {
        putchar(SliConst_u8_at(str, i)->value);
    }
    putchar('\n');
}

void fmt_print_f64(f64 value) {
    printf("%lg", value);
}

void fmt_println_f64(f64 value) {
    printf("%lg\n", value);
}

void test(void) {
    PtrSConst_u8 s = PtrSConst_u8_from(as(u8*, "hello"), '\0');
    debug_assert(PtrSConst_u8_at(s, 0)->value == 'h');
    debug_assert(PtrSConst_u8_at(s, 1)->value == 'e');
    debug_assert(PtrSConst_u8_at(s, 2)->value == 'l');
    debug_assert(PtrSConst_u8_at(s, 3)->value == 'l');
    debug_assert(PtrSConst_u8_at(s, 4)->value == 'o');
    debug_assert(PtrSConst_u8_at(s, 5)->value == '\0');
    debug_assert(PtrSConst_u8_at(s, 5)->value == PtrSConst_u8_sentinel(s)->value);
    debug_assert(PtrSConst_u8_isSentinel(s, 5));
}

int main(void) {
    test();

    const PtrSConst_u8 ptr = PtrSConst_u8_from((u8*)"Hello, World!\n", '\0');
    const SliConst_u8  str = { .base = mem_span(typeInfo(const u8), ptr.base) };
    fmt_printStr(str);

    const Ptr_f64 dbl_num = Ptr_f64_from(create(f64, 3.141592653589793));
    fmt_println_f64(dbl_num.addr->value);

    // clang-format off
    const f64 dbl_nums[] = {
        0.0, 1.0, 2.0,
        3.0, 4.0, 5.0,
        6.0, 7.0, 8.0,
        9.0
    };
    // clang-format on
    const PtrSConst_f64 dbl_num_ptr  = PtrSConst_f64_from(dbl_nums, 9.0);
    const SliConst_f64  dbl_nums_sli = { .base = mem_span(typeInfo(f64), dbl_num_ptr.base) };
    for (usize i = 0; i < dbl_nums_sli.len; ++i) {
        fmt_print_f64(PtrSConst_f64_at(dbl_num_ptr, i)->value);
    }

    const Sli_u8 new_line = {
        .base = mem_span(typeInfo(u8), PtrS_u8_from((u8[]){ "\r\n" }, 0).base)
    };
    fmt_printStr((SliConst_u8){ .base = new_line.base });

    // TODO: Add str literal init macro
    // TODO: Support for utf-8 (make default)
    // TODO: Support wchar str
    fmt_printlnStr(
        (SliConst_u8){
            .base = mem_span(
                typeInfo(u8),
                PtrS_u8_from((u8[]){ "아 배고파 햄버거 먹으러 가야지~ 후후" }, 0).base
            ) }
    );

    return 0;
}
