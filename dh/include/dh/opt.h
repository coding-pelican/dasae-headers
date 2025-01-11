/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2024-12-31 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief
 * @details
 */

#ifndef OPT_INCLUDED
#define OPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "scope.h"
#include "defer.h"

/*========== Macros and Definitions =========================================*/

/* Optional value */
#define Opt$(T)         \
    struct {            \
        bool has_value; \
        T    value;     \
    }

#define using_Opt$(T) \
    decl_Opt$(T);     \
    impl_Opt$(T)

#define decl_Opt$(T)                                                        \
    typedef struct pp_join($, Opt$PtrConst, T) pp_join($, Opt$PtrConst, T); \
    typedef struct pp_join($, Opt$Ptr, T) pp_join($, Opt$Ptr, T);           \
    typedef struct pp_join($, Opt, T) pp_join($, Opt, T)

#define impl_Opt$(T)                     \
    struct pp_join($, Opt$PtrConst, T) { \
        bool has_value;                  \
        const rawptr(T) value;           \
    };                                   \
    struct pp_join($, Opt$Ptr, T) {      \
        bool has_value;                  \
        rawptr(T) value;                 \
    };                                   \
    struct pp_join($, Opt, T) {          \
        bool has_value;                  \
        T    value;                      \
    }

/* Determines optional value */
#define some(val_opt...) \
    { .has_value = true, .value = val_opt }

#define none() \
    { .has_value = false }

/* Checks optional value */
#define isSome(opt) \
    ((opt).has_value)

#define isNone(opt) \
    (!isSome(opt))

/* Returns optional value */
#define return_Opt$ \
    return (TypeOf(getReservedReturn()[0]))

#define return_some(val_opt...)                                \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                                     \
        .value     = val_opt,                                  \
    })

#define return_none()                                          \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                                    \
    })

#define defer_return_some(val_opt...)              \
    defer_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                         \
        .value     = val_opt,                      \
    })

#define defer_return_none()                        \
    defer_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                        \
    })

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse(expr, body...) ({ \
    var _result = (expr);        \
    if (!_result.has_value) {    \
        body;                    \
    }                            \
    _result.value;               \
})

#define orelse_default(expr, val_default...) ({        \
    var _result = (expr);                              \
    _result.has_value ? _result.value : (val_default); \
})

#define unwrap(expr)                                                 \
    ({                                                               \
        var _result = (expr);                                        \
        debug_assert_fmt(_result.has_value, "Unwrap of null value"); \
        _result.value;                                               \
    })

/* Optional payload capture (similar to Zig's if/while captures) */
#define if_some(expr, var_capture)                    \
    scope_if(let _result = (expr), _result.has_value) \
        scope_with(let var_capture = _result.value)

#define if_none(expr) \
    scope_if(let _result = (expr), !_result.has_value)

#define else_some(var_capture) \
    scope_else(let var_capture = _result.value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
