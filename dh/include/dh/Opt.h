/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2024-12-26 (date of last update)
 * @version v0.1
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

/*========== Macros and Definitions =========================================*/

/* Optional */
#define Opt$(T)         \
    struct {            \
        bool has_value; \
        T    value;     \
    }

#define using_Opt$(T) \
    decl_Opt$(T);     \
    impl_Opt$(T)

#define decl_Opt$(T)                                      \
    typedef struct pp_join($, Opt, T) pp_join($, Opt, T); \
    typedef struct pp_join($, Optptr, T) pp_join($, Optptr, T)

#define impl_Opt$(T)               \
    struct pp_join($, Opt, T) {    \
        bool has_value;            \
        T    value;                \
    };                             \
    struct pp_join($, Optptr, T) { \
        bool has_value;            \
        rawptr(T) value;           \
    }

#define some(val_opt...)     \
    {                        \
        .has_value = true,   \
        .value     = val_opt \
    }

#define none()              \
    {                       \
        .has_value = false, \
    }

#define isSome(opt) ((opt).has_value)
#define isNone(opt) (!isSome(opt))

/* Return macros */
#define return_Opt$ \
    return (TypeOf(getReservedReturn()))

#define return_some(val_opt...)            \
    return ({                              \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .has_value = true,         \
                .value     = val_opt,      \
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
#define if_some(expr, var_name)                       \
    scope_if(let _result = (expr), _result.has_value) \
        scope_with(let var_name = _result.value)

#define if_some_mut(expr, var_name)                   \
    scope_if(var _result = (expr), _result.has_value) \
        scope_with(var var_name = _result.value)

#define if_none(expr) \
    scope_if(let _result = (expr), !_result.has_value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
