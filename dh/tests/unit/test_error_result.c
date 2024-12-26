#include "dh/core.h"

/* Error Result */
#define ErrRes(TErr, TOk) \
    struct {              \
        bool is_err;      \
        union {           \
            Err err;      \
            TOk ok;       \
        };                \
    }

#define err(val_err...)     \
    {                       \
        .is_err = true,     \
        .err    = (val_err) \
    }

#define ok(val_ok...)      \
    {                      \
        .is_err = false,   \
        .ok     = (val_ok) \
    }

/* Return macros */
#define return_ErrRes \
    return (TypeOf(getReservedReturn()))

#define return_err(val_err...)             \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = true,            \
                .err    = (val_err),       \
            }                              \
        );                                 \
    })

#define return_ok(val_ok...)               \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = false,           \
                .ok     = (val_ok),        \
            }                              \
        );                                 \
    })

/* Error void result (special case) */
typedef ErrRes(Err, Void) ErrVoid;

#define return_ErrVoid \
    return (ErrVoid)

#define return_void()                      \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = false,           \
                .ok     = (Void){},        \
            }                              \
        );                                 \
    })

/* Error handling macros */
#define try(expr)                    \
    ({                               \
        let _result = (expr);        \
        if (_result.is_err) {        \
            return_err(_result.err); \
        }                            \
        _result.ok;                  \
    })

#define catch(expr, var_err, body...)  \
    ({                                 \
        let _result = (expr);          \
        if (_result.is_err) {          \
            let var_err = _result.err; \
            body                       \
        }                              \
        _result.ok;                    \
    })

// Example usage:
typedef ErrRes(MathErr, i32) MathErr$i32;

static MathErr$i32 safe_divide(i32 lhs, i32 rhs) {
    reserveReturn(MathErr$i32);

    if (rhs == 0) {
        return_err(MathErr_err(MathErrType_DivisionByZero));
    }
    return_ok(lhs / rhs);
}

ErrVoid example(void) {
    reserveReturn(ErrVoid);

    // Using catch
    i32 result = catch (safe_divide(10, 0), err, {
        printf("Error: %s\n", Err_message(err));
        return_err(err);
    });

    // Using try
    result = try(safe_divide(10, 2));
    printf("Result: %d\n", result);

    return_void();
}
