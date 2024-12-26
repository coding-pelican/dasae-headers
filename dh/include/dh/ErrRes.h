/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ErrRes.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2024-12-26 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ErrRes
 *
 * @brief   Header for Error Result type
 * @details Common Error Result type for error handling
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

/* Error Result type */
#define using_ErrRes(TErr, TOk)             \
    typedef_struct(pp_join($, TErr, TOk)) { \
        bool is_err;                        \
        union {                             \
            Err err;                        \
            TOk ok;                         \
        };                                  \
    }

#define decl_ErrRes(TErr, TOk) \
    typedef struct pp_join($, TErr, TOk) pp_join($, TErr, TOk);

#define impl_ErrRes(TErr, TOk)     \
    struct pp_join($, TErr, TOk) { \
        bool is_err;               \
        union {                    \
            Err err;               \
            TOk ok;                \
        };                         \
    }

#define usingAs_ErrRes(TErr, TOk, name) \
    typedef_struct(name) {              \
        bool is_err;                    \
        union {                         \
            Err err;                    \
            TOk ok;                     \
        };                              \
    }

#define declAs_ErrRes(TErr, TOk, name) \
    typedef struct name name;

#define implAs_ErrRes(TErr, TOk, name) \
    struct name {                      \
        bool is_err;                   \
        union {                        \
            Err err;                   \
            TOk ok;                    \
        };                             \
    }

usingAs_ErrRes(Err, Void, ErrVoid);

/* Error propagation */

#define ErrRes_reserveReturn(TErrRes) \
    reserveReturn(TErrRes)

#define ErrRes_err(val_err...)             \
    ({                                     \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = true,            \
                .err    = val_err,         \
            }                              \
        );                                 \
        getReservedReturn();               \
    })

#define ErrRes_ok(val_ok...)               \
    ({                                     \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = false,           \
                .ok     = val_ok,          \
            }                              \
        );                                 \
        getReservedReturn();               \
    })

#define ErrVoid_reserveReturn() \
    reserveReturn(ErrVoid)

#define ErrVoid_err(val_err...) \
    ErrRes_err(val_err)

#define ErrVoid_ok() \
    ErrRes_ok({})

/* Error handling */

#define try(_Expr)                          \
    ({                                      \
        let _result = (_Expr);              \
        if (_result.is_err) {               \
            return ErrRes_err(_result.err); \
        }                                   \
        _result.ok;                         \
    })

#define catch(_Expr, var_err, _Expr_Handling...) \
    ({                                           \
        let _result = (_Expr);                   \
        if (_result.is_err) {                    \
            let var_err = _result.err;           \
            _Expr_Handling                       \
        }                                        \
        _result.ok;                              \
    })

/* Error main */

#if ERR_RES_INCLUDED
#if !defined(ERR_NO_HIJACK_MAIN) && !defined(ERR_MAIN_INCLUDED)
#define ERR_MAIN_INCLUDED (1)

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

#endif /* !defined(ERR_NO_HIJACK_MAIN) && !defined(ERR_MAIN_INCLUDED) */
#endif /* ERR_RES_INCLUDED */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
