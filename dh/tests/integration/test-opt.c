#define main_no_args        (1)
#define main_no_returns_err (1)
#include "dh/main.h"

Opt$i32 getValueIfPositive(i32 value) {
    reserveReturn(Opt$i32);
    if (value <= 0) { return_none(); }
    return_some(value);
}

Opt$u8 getCharIfNumeric(u8 value) {
    reserveReturn(Opt$u8);
    if ('0' <= value && value <= '9') { return_some(value); }
    return_none();
}

void dh_main(void) {
    if_some(getValueIfPositive(10), value) {
        printf("Value is %d\n", value);
    }
    else {
        printf("Value is not positive\n");
    }

    if_some(getCharIfNumeric('a'), value) {
        printf("Value is %c\n", value);
    }
    else {
        printf("Value is not numeric\n");
    }

    if_some(getValueIfPositive(-10), value) {
        printf("Value is %d\n", value);
    }
    else {
        printf("Value is not positive\n");
    }

    if_some(getCharIfNumeric('9'), value) {
        printf("Value is %c\n", value);
    }
    else {
        printf("Value is not numeric\n");
    }
}
