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

/* Error Result */
#define ErrRes(TErr, TOk) \
    struct {              \
        bool is_err;      \
        union {           \
            Err err;      \
            TOk ok;       \
        };                \
    }

#define using_ErrRes(TErr, TOk) \
    typedef ErrRes(TErr, TOk) pp_join($, TErr, TOk)

#define decl_ErrRes(TErr, TOk) \
    typedef struct pp_join($, TErr, TOk) pp_join($, TErr, TOk)

#define impl_ErrRes(TErr, TOk)     \
    struct pp_join($, TErr, TOk) { \
        bool is_err;               \
        union {                    \
            Err err;               \
            TOk ok;                \
        };                         \
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

/* Error main */
#if ERR_RES_INCLUDED
#if !defined(ERR_RES_NO_HIJACK_MAIN) && !defined(ERR_RES_MAIN_INCLUDED)
#define ERR_RES_MAIN_INCLUDED (1)

extern ErrVoid dh_main(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {
    const ErrVoid result = dh_main(argc, argv);
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
