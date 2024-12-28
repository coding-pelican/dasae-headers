/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    err_res.h
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

#ifndef ERR_RES_INCLUDED
#define ERR_RES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Err.h"

/*========== Macros and Definitions =========================================*/

/* Error result */
#define ErrRes(TOk)  \
    struct {         \
        bool is_err; \
        union {      \
            Err err; \
            TOk ok;  \
        };           \
    }

#define using_ErrRes(TOk) \
    decl_ErrRes(TOk);     \
    impl_ErrRes(TOk)

#define decl_ErrRes(TOk) \
    typedef struct pp_join($, Err, TOk) pp_join($, Err, TOk)

#define impl_ErrRes(TOk)          \
    struct pp_join($, Err, TOk) { \
        bool is_err;              \
        union {                   \
            Err err;              \
            TOk ok;               \
        };                        \
    }

#define err(val_err...)   \
    {                     \
        .is_err = true,   \
        .err    = val_err \
    }

#define ok(val_ok...)    \
    {                    \
        .is_err = false, \
        .ok     = val_ok \
    }

/* Return macros */
#define return_ErrRes \
    return (TypeOf(getReservedReturn()))

#define return_err(val_err...)             \
    return (TypeOf(getReservedReturn())) { \
        .is_err = true,                    \
        .err    = val_err,                 \
    }

#define return_ok(val_ok...)               \
    return (TypeOf(getReservedReturn())) { \
        .is_err = false,                   \
        .ok     = val_ok,                  \
    }

/* Syntax sugar for Error result */
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

#define decl_Err$(TOk) \
    typedef struct pp_join($, Err, TOk) pp_join($, Err, TOk)

#define impl_Err$(TOk)            \
    struct pp_join($, Err, TOk) { \
        bool is_err;              \
        union {                   \
            Err err;              \
            TOk ok;               \
        };                        \
    }

#define return_Err$ \
    return (TypeOf(getReservedReturn()))

/* Error void result (special case) */
using_Err$(Void);
#define return_Err$Void \
    return (Err$Void)

typedef Err$Void Err$void;
#define return_Err$void \
    return (Err$void)

#define return_void()                      \
    return (TypeOf(getReservedReturn())) { \
        .is_err = false,                   \
        .ok     = (Void){},                \
    }

/* Error handling */
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

/* Error main */
#if ERR_RES_INCLUDED
#if !defined(ERR_RES_NO_HIJACK_MAIN) && !defined(ERR_RES_MAIN_INCLUDED)
#define ERR_RES_MAIN_INCLUDED (1)

extern must_check Err$void dh_main(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {
    const Err$void result = dh_main(argc, argv);
    if (!result.is_err) { return 0; }
    ignore fprintf(
        stderr,
        "Program failed: %s (type: %d)\n",
        Err_message(result.err),
        Err_type(result.err)
    );
    return 1;
}

#endif /* !defined(ERR_RES_NO_HIJACK_MAIN) && !defined(ERR_RES_MAIN_INCLUDED) */
#endif /* ERR_RES_INCLUDED */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
