/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    err_res.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.2
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
#include "scope.h"
#include "Err.h"

/*========== Definitions ====================================================*/

/* Error result */
#define Err$(TOk)                                TYPE_UNNAMED__Err$(TOk)
#define use_Err$(TOk)                            GEN__use_Err$(TOk)
#define decl_Err$(TOk)                           GEN__decl_Err$(TOk)
#define impl_Err$(TOk)                           GEN__impl_Err$(TOk)
#define Err_asNamed$(TNamedErr, var_unnamed_err) OP__Err_asNamed$(TNamedErr, var_unnamed_err)

/* Determines error result */
#define err(val_err...) OP__err(val_err)
#define ok(val_ok...)   OP__ok(val_ok)

#define err$(TErr, val_err...) OP__err$(TErr, val_err)
#define ok$(TOk, val_ok...)    OP__ok$(TOk, val_ok)

#define assignErr(var_err_res, val_err...) OP__assignErr(var_err_res, val_err)
#define assignOk(var_err_res, val_ok...)   OP__assignOk(var_err_res, val_ok)

/* Checks error result */
#define isErr(val_err_res) OP__isErr(val_err_res)
#define isOk(val_err_res)  OP__isOk(val_err_res)

/* Returns error result */
#define return_err(val_err...) SYN__return_err(val_err)
#define return_ok(val_ok...)   SYN__return_ok(val_ok)

/* Propagates error (similar to Zig's try) */
#define try(expr) OP__try(expr)

/* Handles error (similar to Zig's catch) */
#define catch_default(expr, val_default...)   OP__catch_default(expr, val_default)
#define catch(expr, var_capture_err, body...) OP__catch(expr, var_capture_err, body)

/* Defers when error */
#define errdefer(_Statements...) SYN__errdefer(_Statements)

/* Error result payload captures */
#define if_ok(expr, var_capture)  SYN__if_ok(expr, var_capture)
#define else_err(var_capture)     SYN__else_err(var_capture)
#define if_err(expr, var_capture) SYN__if_err(expr, var_capture)
#define else_ok(var_capture)      SYN__else_ok(var_capture)

/* Error void result (special case) */
typedef struct Err$Void Err$Void;
struct Err$Void {
    bool is_err;
    union {
        Err  err;
        Void ok;
    };
};
typedef Err$Void Err$void;
#define return_void() SYN__return_void()

/* Error result specific */
#define use_ErrSet$(TErr, TOk)  GEN__use_ErrSet$(TErr, TOk)
#define decl_ErrSet$(TErr, TOk) GEN__decl_ErrSet$(TErr, TOk)
#define impl_ErrSet$(TErr, TOk) GEN__impl_ErrSet$(TErr, TOk)

/*========== Implementations ================================================*/

#define TYPE_UNNAMED__Err$(TOk) \
    struct {                    \
        bool is_err;            \
        union {                 \
            Err err;            \
            TOk ok;             \
        };                      \
    }
#define GEN__use_Err$(TOk) \
    decl_Err$(TOk);        \
    impl_Err$(TOk)
#define GEN__decl_Err$(TOk)                                                     \
    typedef struct pp_join($, Err$PtrConst, TOk) pp_join($, Err$PtrConst, TOk); \
    typedef struct pp_join($, Err$Ptr, TOk) pp_join($, Err$Ptr, TOk);           \
    typedef struct pp_join($, Err, TOk) pp_join($, Err, TOk)
#define GEN__impl_Err$(TOk)                \
    struct pp_join($, Err$PtrConst, TOk) { \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            rawptr_const$(TOk) ok;         \
        };                                 \
    };                                     \
    struct pp_join($, Err$Ptr, TOk) {      \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            rawptr$(TOk) ok;               \
        };                                 \
    };                                     \
    struct pp_join($, Err, TOk) {          \
        bool is_err;                       \
        union {                            \
            Err err;                       \
            TOk ok;                        \
        };                                 \
    }
#define OP__Err_asNamed$(TNamedErr, var_unnamed_err) eval({                                             \
    let _unnamed_err = var_unnamed_err;                                                                 \
    claim_assert_static(sizeOf(TypeOf(_unnamed_err)) == sizeOf(TNamedErr));                             \
    claim_assert_static(alignOf(TypeOf(_unnamed_err)) == alignOf(TNamedErr));                           \
    claim_assert_static(hasField(TypeOf(_unnamed_err), is_err));                                        \
    claim_assert_static(validateField(TypeOf(_unnamed_err), is_err, FieldTypeOf(TNamedErr, is_err)));   \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), is_err) == fieldPadding(TNamedErr, is_err)); \
    claim_assert_static(hasField(TypeOf(_unnamed_err), err));                                           \
    claim_assert_static(validateField(TypeOf(_unnamed_err), err, FieldTypeOf(TNamedErr, err)));         \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), err) == fieldPadding(TNamedErr, err));       \
    claim_assert_static(hasField(TypeOf(_unnamed_err), ok));                                            \
    claim_assert_static(validateField(TypeOf(_unnamed_err), ok, FieldTypeOf(TNamedErr, ok)));           \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), ok) == fieldPadding(TNamedErr, ok));         \
    eval_return(*(TNamedErr*)&_unnamed_err);                                                            \
})

#define OP__err(val_err...) { .is_err = true, .err = val_err }
#define OP__ok(val_ok...)   { .is_err = false, .ok = val_ok }

#define OP__err$(TErr, val_err...) ((TErr)err(val_err))
#define OP__ok$(TOk, val_ok)       ((TOk)ok(val_ok))

#define OP__assignErr(var_err_res, val_err...) eval({        \
    let _ptr_err_res = &var_err_res;                         \
    *_ptr_err_res    = err$(TypeOf(*_ptr_err_res), val_err); \
})
#define OP__assignOk(var_err_res, val_ok...) eval({        \
    let _ptr_err_res = &var_err_res;                       \
    *_ptr_err_res    = ok$(TypeOf(*_ptr_err_res), val_ok); \
})

#define OP__isErr(val_err_res) ((val_err_res).is_err)
#define OP__isOk(val_err_res)  (!isErr(val_err_res))

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define SYN__return_err(val_err...)                            \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                                        \
        .err    = val_err,                                     \
    })
#define SYN__return_ok(val_ok...)                              \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = val_ok,                                      \
    })

#define OP__try(expr) ({         \
    let _result = (expr);        \
    if (_result.is_err) {        \
        return_err(_result.err); \
    }                            \
    _result.ok;                  \
})

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define SYN__return_err(val_err...)                \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                            \
        .err    = val_err,                         \
    })
#define SYN__return_ok(val_ok...)                  \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = val_ok,                          \
    })

#define OP__try(expr) ({         \
    let _result = (expr);        \
    if (_result.is_err) {        \
        return_err(_result.err); \
    }                            \
    _result.ok;                  \
})

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define OP__catch_default(expr, val_default...) eval({       \
    var _result = (expr);                                    \
    eval_return _result.is_err ? (val_default) : _result.ok; \
})
#define OP__catch(expr, var_capture_err, body...) eval({ \
    var _result = (expr);                                \
    if (_result.is_err) {                                \
        let var_capture_err = _result.err;               \
        body;                                            \
    };                                                   \
    eval_return _result.ok;                              \
})

#define SYN__errdefer(_Statements...) defer(                    \
    if (getReservedReturn() && getReservedReturn()[0].is_err) { \
        _Statements;                                            \
    }                                                           \
)

#define SYN__if_ok(expr, var_capture)               \
    scope_if(let _result = (expr), !_result.is_err) \
        scope_with(let var_capture = _result.ok)
#define SYN__else_err(var_capture) \
    scope_else(let var_capture = _result.err)
#define SYN__if_err(expr, var_capture)             \
    scope_if(let _result = (expr), _result.is_err) \
        scope_with(let var_capture = _result.err)
#define SYN__else_ok(var_capture) \
    scope_else(let var_capture = _result.ok)

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define SYN__return_void()                                     \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = (Void){},                                    \
    })

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define SYN__return_void()                         \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = (Void){},                        \
    })

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define GEN__use_ErrSet$(TErr, TOk) \
    decl_ErrSet$(TErr, TOk);        \
    impl_ErrSet$(TErr, TOk)
#define GEN__decl_ErrSet$(TErr, TOk)                                                  \
    typedef struct pp_join3($, TErr, PtrConst, TOk) pp_join3($, TErr, PtrConst, TOk); \
    typedef struct pp_join3($, TErr, Ptr, TOk) pp_join3($, TErr, Ptr, TOk);           \
    typedef struct pp_join($, TErr, TOk) pp_join($, TErr, TOk)
#define GEN__impl_ErrSet$(TErr, TOk)          \
    struct pp_join3($, TErr, PtrConst, TOk) { \
        bool is_err;                          \
        union {                               \
            TErr err;                         \
            rawptr_const$(TOk) ok;            \
        };                                    \
    };                                        \
    struct pp_join3($, TErr, Ptr, TOk) {      \
        bool is_err;                          \
        union {                               \
            TErr err;                         \
            rawptr$(TOk) ok;                  \
        };                                    \
    };                                        \
    struct pp_join($, TErr, TOk) {            \
        bool is_err;                          \
        union {                               \
            TErr err;                         \
            TOk  ok;                          \
        };                                    \
    }

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
 */

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
config_ErrSet(
    math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_Err$(i32);
must_check static Err$i32 safeDivide(i32 lhs, i32 rhs) {
    reserveReturn(Err$i32);
    if (rhs == 0) {
        return_err(math_Err_err(math_ErrCode_DivisionByZero));
    }
    return_ok(lhs / rhs);
}

Err$void test(void) {
    reserveReturn(Err$void);

    let result_invalid  = try(safeDivide(10, 0));
    let result_default  = catch_default(safeDivide(10, 0), 1);
    let result_handling = catch (safeDivide(10, 0), err, {
        ignore fprintf(stderr, "Error: %s\n", Err_codeToCStr(err));
        return_err(err);
    });

    return_ok({});
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
