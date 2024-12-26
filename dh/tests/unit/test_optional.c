#include "dh/core.h"

/* Optional */
#define Opt(T)          \
    struct {            \
        bool has_value; \
        T    value;     \
    }

#define some(val_opt...)       \
    {                          \
        .has_value = true,     \
        .value     = (val_opt) \
    }

#define none()              \
    {                       \
        .has_value = false, \
    }

/* Return macros */
#define return_Opt \
    return (TypeOf(getReservedReturn()))

#define return_some(val_opt...)            \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .has_value = true,         \
                .value     = (val_opt),    \
            }                              \
        );                                 \
    })

#define return_none()                      \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .has_value = false,        \
            }                              \
        );                                 \
    })

/* Unwrapping macros (similar to Zig's orelse and .?) */
#define orelse(expr, val_default)                          \
    ({                                                     \
        let _result = (expr);                              \
        _result.has_value ? _result.value : (val_default); \
    })
#define unwrap(expr)                                                 \
    ({                                                               \
        let _result = (expr);                                        \
        debug_assert_fmt(_result.has_value, "Unwrap of null value"); \
        _result.value;                                               \
    })

/* Optional payload capture (similar to Zig's if/while captures) */
#define if_some(expr, var_name, body...)  \
    ({                                    \
        let _result = (expr);             \
        if (_result.has_value) {          \
            let var_name = _result.value; \
            body                          \
        }                                 \
    })

#define if_some_mut(expr, var_name, body...) \
    ({                                       \
        var _result = (expr);                \
        if (_result.has_value) {             \
            var var_name = &_result.value;   \
            body                             \
        }                                    \
    })

/* Example usage: */
typedef Opt(i32) Opt$i32;
static Opt$i32 find_value(const i32* arr, usize len, i32 target) {
    reserveReturn(Opt$i32);

    for (usize i = 0; i < len; ++i) {
        if (arr[i] != target) { continue; }
        return_some(i);
    }
    return_none();
}

void example(void) {
    i32 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 12 };

    // Using orelse
    let found_index = find_value(data, sizeof(data) / sizeof(data[0]), 10);
    let safe_index  = orelse(found_index, -1);
    unused(safe_index);

    // Using if_some
    if_some(found_index, index, {
        printf("Found at index: %d\n", index);
    });

    // Using if_some_mut for modifiable captures
    let value = (Opt$i32)some(5);
    if_some_mut(value, ptr, {
        (*ptr)++;
    });

    // Using unwrap (will assert if none)
    i32 unwrapped = unwrap((Opt$i32)some(42));
    unused(unwrapped);
}

/* Special case for pointers (null pointer optimization) */
#define Optptr(T)     T*
#define ptr_some(val) (val)
#define ptr_none()    null
#define ptr_orelse(expr, val_default) \
    ({                                \
        let _ptr = (expr);            \
        _ptr ? _ptr : (val_default);  \
    })
#define ptr_unwrap(expr)                                          \
    ({                                                            \
        let _ptr = (expr);                                        \
        debug_assert_nonnull_fmt(_ptr, "Unwrap of null pointer"); \
        _ptr;                                                     \
    })

int main(void) {
    example();
    return 0;
}
