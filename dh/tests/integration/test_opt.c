#include "dh/core/prim/struct.h"
#include "dh/opt.h"

using_Opt(i32);
Opt$i32 getValueIfPositive(i32 value) {
    reserveReturn(Opt$i32);
    if (value <= 0) {
        return_none();
    }
    return_some(value);
}

using_Opt(u8);
Opt$u8 getCharIfNumeric(u8 value) {
    reserveReturn(Opt$u8);
    if ('0' <= value && value <= '9') {
        return_some(value);
    }
    return_none();
}

int main() {
    {
        let opt = getValueIfPositive(10);
        if (opt.has_value) {
            let value = unwrap(opt);
            printf("Value is %d\n", value);
        } else {
            printf("Value is not positive\n");
        }
    }

    {
        let opt = getCharIfNumeric('a');
        if (opt.has_value) {
            const u8 value = unwrap(opt);
            printf("Value is %c\n", value);
        } else {
            printf("Value is not numeric\n");
        }
    }

    {
        let opt = getValueIfPositive(-10);
        if (opt.has_value) {
            let value = unwrap(opt);
            printf("Value is %d\n", value);
        } else {
            printf("Value is not positive\n");
        }
    }

    {
        let opt = getCharIfNumeric('9');
        if (opt.has_value) {
            let value = unwrap(opt);
            printf("Value is %c\n", value);
        } else {
            printf("Value is not numeric\n");
        }
    }

    /* Result example */
    // let result = ResErr_i32_err(Err_AccessDenied); // Value stored directly
    // if (ResErr_i32_isOk(result)) {
    //     let value      = ResErr_i32_unwrap(result);       // Safe access
    //     let or_default = ResErr_i32_unwrapOr(result, -1); // With default
    //     printf("%d %d\n", value, or_default);
    // } else {
    //     let error = ResErr_i32_unwrapErr(result); // Safe error access
    //     Err_unreachable(error);
    //     // or ResErr_i32_unwrap(result);
    // }

    return 0;
}
