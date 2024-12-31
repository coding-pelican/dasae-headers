#include "common.h"
int main(void) {
    let raw_str    = (u8*)"hello world";
    let str_prefix = Sli_range$(SliConst$u8, raw_str, 0, 5);
    let str_suffix = Sli_slice(str_prefix, 6, 11);
    printf("prefix: %.*s\n", (int)str_prefix.len, str_prefix.ptr);
    printf("suffix: %.*s\n", (int)str_suffix.len, str_suffix.ptr);
}
