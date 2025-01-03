/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    err_res.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2024-12-31 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief
 * @details
 */

#ifndef ERR_RES_INCLUDED
#define ERR_RES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Err.h"
#include "scope.h"

/*========== Macros and Definitions =========================================*/

/* Error result */
#define Err$(TOk)    \
    struct {         \
        bool is_err; \
        union {      \
            Err err; \
            TOk ok;  \
        };           \
    }

#define using_Err$(TOk) \
    decl_Err$(TOk);     \
    impl_Err$(TOk)

#define decl_Err$(TOk)                                                          \
    typedef struct pp_join($, Err$PtrConst, TOk) pp_join($, Err$PtrConst, TOk); \
    typedef struct pp_join($, Err$Ptr, TOk) pp_join($, Err$Ptr, TOk);           \
    typedef struct pp_join($, Err, TOk) pp_join($, Err, TOk)

#define impl_Err$(TOk)                     \
    struct pp_join($, Err$PtrConst, TOk) { \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            const rawptr(TOk) ok;          \
        };                                 \
    };                                     \
    struct pp_join($, Err$Ptr, TOk) {      \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            rawptr(TOk) ok;                \
        };                                 \
    };                                     \
    struct pp_join($, Err, TOk) {          \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            TOk ok;                        \
        };                                 \
    }

/* Determines error result */
#define err(val_err...) \
    { .is_err = true, .err = val_err }

#define ok(val_ok...) \
    { .is_err = false, .ok = val_ok }

/* Checks error result */
#define isErr(val_err_res) \
    ((val_err_res).is_err)

#define isOk(val_err_res) \
    (!isErr(val_err_res))

/* Returns error result */
#define return_Err$ \
    return (TypeOf(getReservedReturn()[0]))

#define return_err(val_err...)                                 \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                                        \
        .err    = val_err,                                     \
    })

#define return_ok(val_ok...)                                   \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = val_ok,                                      \
    })

#define defer_return_err(val_err...)               \
    defer_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                            \
        .err    = val_err,                         \
    })

#define defer_return_ok(val_ok...)                 \
    defer_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = val_ok,                          \
    })

/* Propagates error (similar to Zig's try) */
#define try(expr) ({             \
    let _result = (expr);        \
    if (_result.is_err) {        \
        return_err(_result.err); \
    }                            \
    _result.ok;                  \
})

#define try_defer(expr) ({             \
    let _result = (expr);              \
    if (_result.is_err) {              \
        defer_return_err(_result.err); \
    }                                  \
    _result.ok;                        \
})

/* Handles error (similar to Zig's catch) */
#define catch_default(expr, val_default...) ({   \
    var _result = (expr);                        \
    _result.is_err ? (val_default) : _result.ok; \
})

#define catch(expr, var_capture_err, body...) ({ \
    var _result = (expr);                        \
    if (_result.is_err) {                        \
        let var_capture_err = _result.err;       \
        body;                                    \
    }                                            \
    _result.ok;                                  \
})

#define errdefer(_Statements...)                                    \
    {                                                               \
        if (getReservedReturn() && getReservedReturn()[0].is_err) { \
            defer(_Statements);                                     \
        }                                                           \
    }

/* Error result payload captures */
#define if_ok(expr, var_capture)                    \
    scope_if(let _result = (expr), !_result.is_err) \
        scope_with(let var_capture = _result.ok)

#define else_err(var_capture) \
    scope_else(let var_capture = _result.err)

#define if_err(expr, var_capture)                  \
    scope_if(let _result = (expr), _result.is_err) \
        scope_with(let var_capture = _result.err)

#define else_ok(var_capture) \
    scope_else(let var_capture = _result.ok)

/* Error void result (special case) */
typedef struct Err$Void Err$Void;
struct Err$Void {
    bool is_err;
    union {
        Err  err;
        Void ok;
    };
};
#define return_Err$Void \
    return (Err$Void)

typedef Err$Void Err$void;
#define return_Err$void \
    return (Err$void)

#define return_void()                                          \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = (Void){},                                    \
    })

#define defer_return_void()                        \
    defer_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = (Void){},                        \
    })

// #define ErrRes(TOk)  \
//     struct {         \
//         bool is_err; \
//         union {      \
//             Err err; \
//             TOk ok;  \
//         };           \
//     }

// #define using_ErrRes(TOk) \
//     decl_ErrRes(TOk);     \
//     impl_ErrRes(TOk)

// #define decl_ErrRes(TOk) \
//     typedef struct pp_join($, Err, TOk) pp_join($, Err, TOk)

// #define impl_ErrRes(TOk)          \
//     struct pp_join($, Err, TOk) { \
//         bool is_err;              \
//         union {                   \
//             Err err;              \
//             TOk ok;               \
//         };                        \
//     }

// #define return_ErrRes \
//     return (TypeOf(getReservedReturn()))

// /* Enhanced catch macro that supports both statement blocks and default values */
// #define catch(expr, val_default_or_var_err_with_body...)        \
//     CATCH_MACRO_CHOOSER(expr, val_default_or_var_err_with_body) \
//     (expr, val_default_or_var_err_with_body)

// /* Helper macros to count and validate arguments */
// #define CATCH_GET_ARG_COUNT(...) \
//     CATCH_GET_ARG_COUNT_(__VA_ARGS__, 3, 2, 1, 0)

// #define CATCH_GET_ARG_COUNT_(_1, _2, _3, count, ...) count

// #define CATCH_CHECK_ARGS(...) \
//     CATCH_CHECK_ARGS_(CATCH_GET_ARG_COUNT(__VA_ARGS__))

// #define CATCH_CHECK_ARGS_(count) \
//     CATCH_CHECK_ARGS__(count)

// #define CATCH_MACRO_CHOOSER(...) \
//     CATCH_MACRO_CHOOSER_(__VA_ARGS__, CATCH_WITH_STATEMENTS, CATCH_WITH_DEFAULT, )

// #define CATCH_MACRO_CHOOSER_(_1, _2, _3, FUNC, ...) FUNC

// /* Catch with default value (2 arguments version) */
// #define CATCH_WITH_DEFAULT(expr, default_value...) ({ \
//     var _result = (expr);                             \
//     _result.is_err ? (default_value) : _result.ok;    \
// })

// /* Catch with statements (3 arguments version) */
// #define CATCH_WITH_STATEMENTS(expr, var_err, body...) ({ \
//     var _result = (expr);                                \
//     if (_result.is_err) {                                \
//         let var_err = _result.err;                       \
//         body                                             \
//     }                                                    \
//     _result.ok;                                          \
// })

/* // Example usage:
impl_Err(
    math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

using_Err$(i32);
must_check static Err$i32 safeDivide(i32 lhs, i32 rhs) {
    reserveReturn(Err$i32);
    if (rhs == 0) {
        return_err(math_Err_err(math_ErrType_DivisionByZero));
    }
    return_ok(lhs / rhs);
}

void test() {
    let result_invalid  = catch (safeDivide(10, 0));
    let result_default  = catch (safeDivide(10, 0), 1);
    let result_handling = catch (safeDivide(10, 0), err, {
        printf("Error: %s\n", Err_message(err));
        return;
    });
} */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
