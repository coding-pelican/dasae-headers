/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    variant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-02 (date of creation)
 * @updated 2025-01-02 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief
 * @details
 */

#ifndef VARIANT_INCLUDED
#define VARIANT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "scope.h"
#include "opt.h"
#include "err_res.h"

/*========== Macros and Definitions =========================================*/

#define using_variant$(T) \
    decl_variant$(T);     \
    impl_variant$(T)

#define decl_variant$(T)                                              \
    typedef struct pp_join3($, Err, Opt, T) pp_join3($, Err, Opt, T); \
    typedef struct pp_join3($, Opt, Err, T) pp_join3($, Opt, Err, T)

#define impl_variant$(T)              \
    struct pp_join3($, Err, Opt, T) { \
        bool is_err;                  \
        union {                       \
            Err err;                  \
            pp_join($, Opt, T) ok;    \
        };                            \
    };                                \
    struct pp_join3($, Opt, Err, T) { \
        bool has_value;               \
        pp_join($, Err, T) value;     \
    }

/* Determines variant value */
#define ok_some(val_variant...)          \
    {                                    \
        .is_err = false,                 \
        .ok     = {                      \
                .has_value = true,       \
                .value     = val_variant \
        }                                \
    }

#define ok_none()                  \
    {                              \
        .is_err = false,           \
        .ok     = {                \
                .has_value = false \
        }                          \
    }

#define some_ok(val_variant...)       \
    {                                 \
        .has_value = true,            \
        .value     = {                \
                .is_err = false,      \
                .ok     = val_variant \
        }                             \
    }

#define some_err(val_variant...)      \
    {                                 \
        .has_value = true,            \
        .value     = {                \
                .is_err = true,       \
                .err    = val_variant \
        }                             \
    }

/* Returns variant value */
#define return_Err$Opt$ \
    return (TypeOf(getReservedReturn()))

#define return_Opt$Err$ \
    return (TypeOf(getReservedReturn()))

#define return_ok_some(val_variant...)     \
    return (TypeOf(getReservedReturn())) { \
        .is_err = false,                   \
        .ok     = {                        \
                .has_value = true,         \
                .value     = val_variant   \
        }                                  \
    }

#define return_ok_none()                   \
    return (TypeOf(getReservedReturn())) { \
        .is_err = false,                   \
        .ok     = {                        \
                .has_value = false         \
        }                                  \
    }

#define return_some_ok(val_variant...)     \
    return (TypeOf(getReservedReturn())) { \
        .has_value = true,                 \
        .value     = {                     \
                .is_err = false,           \
                .ok     = val_variant      \
        }                                  \
    }

#define return_some_err(val_variant...)    \
    return (TypeOf(getReservedReturn())) { \
        .has_value = true,                 \
        .value     = {                     \
                .is_err = true,            \
                .err    = val_variant      \
        }                                  \
    }

/* Combined payload captures */
#define if_ok_some(expr, var_capture)                                       \
    scope_if(let _result = (expr), !_result.is_err && _result.ok.has_value) \
        scope_with(let var_capture = _result.ok.value)

#define if_ok_some_mut(expr, var_capture)                                   \
    scope_if(var _result = (expr), !_result.is_err && _result.ok.has_value) \
        scope_with(var var_capture = _result.ok.value)

#define if_ok_none(expr) \
    scope_if(let _result = (expr), !_result.is_err && !_result.ok.has_value)

#define if_some_ok(expr, var_capture)                                          \
    scope_if(let _result = (expr), _result.has_value && !_result.value.is_err) \
        scope_with(let var_capture = _result.value.ok)

#define if_some_ok_mut(expr, var_capture)                                      \
    scope_if(var _result = (expr), _result.has_value && !_result.value.is_err) \
        scope_with(var var_capture = _result.value.ok)

#define if_some_err(expr, var_capture)                                        \
    scope_if(let _result = (expr), _result.has_value && _result.value.is_err) \
        scope_with(let var_capture = _result.value.err)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* VARIANT_INCLUDED */
