#define main_no_args        (1)
#define main_no_returns_err (1)
#include "dh/main.h"

static fn_ext_scope(getValueIfPositive(i32 value), Opt$i32) {
    if (value <= 0) { return_none(); }
    return_some(value);
} ext_unscoped;

static fn_ext_scope(getCharIfNumeric(u8 value), Opt$u8) {
    if ('0' <= value && value <= '9') { return_some(value); }
    return_none();
} ext_unscoped;

fn_(dh_main(void), void){
    if_some(getValueIfPositive(10), value) {
        printf("Value is %d\n", value);
    } else_none {
        printf("Value is not positive\n");
    }

    if_some(getCharIfNumeric('a'), value) {
        printf("Value is %c\n", value);
    } else_none {
        printf("Value is not numeric\n");
    }

    if_none(getValueIfPositive(-10)) {
        printf("Value is not positive\n");
    } else_some(value) {
        printf("Value is %d\n", value);
    }

    if_none(getCharIfNumeric('9')) {
        printf("Value is not numeric\n");
    } else_some(value) {
        printf("Value is %c\n", value);
    }
}
