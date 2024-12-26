#include "dh/ext_types.h"

Opt_i32 getValueIfPositive(i32 value) {
    if (value <= 0) {
        return Opt_i32_none();
    }
    return Opt_i32_some(value);
}

Opt_u8 getCharIfNumeric(u8 value) {
    if ('0' <= value && value <= '9') {
        return Opt_u8_some(value);
    }
    return Opt_u8_none();
}

int main() {
    {
        let opt = getValueIfPositive(10);
        if (Opt_i32_isSome(opt)) {
            let value = Opt_i32_unwrap(opt);
            printf("Value is %d\n", value);
        } else {
            printf("Value is not positive\n");
        }
    }

    {
        let opt = getCharIfNumeric('a');
        if (Opt_u8_isSome(opt)) {
            const u8 value = Opt_u8_unwrap(opt);
            printf("Value is %c\n", value);
        } else {
            printf("Value is not numeric\n");
        }
    }

    {
        let opt = getValueIfPositive(-10);
        if (Opt_i32_isSome(opt)) {
            let value = Opt_i32_unwrap(opt);
            printf("Value is %d\n", value);
        } else {
            printf("Value is not positive\n");
        }
    }

    {
        let opt = getCharIfNumeric('9');
        if (Opt_u8_isSome(opt)) {
            let value = Opt_u8_unwrap(opt);
            printf("Value is %c\n", value);
        } else {
            printf("Value is not numeric\n");
        }
    }

    /* Result example */
    let result = ResErr_i32_err(Err_AccessDenied); // Value stored directly
    if (ResErr_i32_isOk(result)) {
        let value      = ResErr_i32_unwrap(result);       // Safe access
        let or_default = ResErr_i32_unwrapOr(result, -1); // With default
        printf("%d %d\n", value, or_default);
    } else {
        let error = ResErr_i32_unwrapErr(result); // Safe error access
        Err_unreachable(error);
        // or ResErr_i32_unwrap(result);
    }

    return 0;
}
