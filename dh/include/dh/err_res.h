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
#include "ErrTrace.h"
#include "debug/common.h"

/*========== Definitions ====================================================*/

/* Error result */
#define Err$(T_Ok)                                comp_type_unnamed__Err$(T_Ok)
#define use_Err$(T_Ok)                            comp_gen__use_Err$(T_Ok)
#define decl_Err$(T_Ok)                           comp_gen__decl_Err$(T_Ok)
#define impl_Err$(T_Ok)                           comp_gen__impl_Err$(T_Ok)
#define Err_asNamed$(T_NamedErr, var_unnamed_err) comp_op__Err_asNamed$(T_NamedErr, var_unnamed_err)

/* Determines error result */
#define err(val_err...) comp_op__err(val_err)
#define ok(val_ok...)   comp_op__ok(val_ok)

#define err$(T_Err, val_err...) comp_op__err$(T_Err, val_err)
#define ok$(T_Ok, val_ok...)    comp_op__ok$(T_Ok, val_ok)

#define errAsg(var_addr_result, val_err...) comp_op__errAsg(pp_uniqTok(addr_result), var_addr_result, val_err)
#define okAsg(var_addr_result, val_ok...)   comp_op__okAsg(pp_uniqTok(addr_result), var_addr_result, val_ok)

/* Checks error result */
#define isErr(val_result) comp_op__isErr(val_result)
#define isOk(val_result)  comp_op__isOk(val_result)

/* Returns error result */
#define return_err(val_err...) comp_syn__return_err(val_err)
#define return_ok(val_ok...)   comp_syn__return_ok(val_ok)

/* Propagates error (similar to Zig's try_) */
#define try_(_Expr) comp_op__try(_Expr)

/* Handles error (similar to Zig's catch_from) */
#define catch_(_Expr, val_ok_default...)                    comp_op__catch_(_Expr, val_ok_default)
#define catch_from(_Expr, _Payload_Capture, _Stmt_Block...) comp_op__catch_from(_Expr, _Payload_Capture, _Stmt_Block)

/* Defers when error */
#define errdefer_(_Stmt...) comp_syn__errdefer(_Stmt)

/* Error result payload captures */
#define if_ok(val_result, _Payload_Capture)  comp_syn__if_ok(val_result, _Payload_Capture)
#define else_err(_Payload_Capture)           comp_syn__else_err(_Payload_Capture)
#define if_err(val_result, _Payload_Capture) comp_syn__if_err(val_result, _Payload_Capture)
#define else_ok(_Payload_Capture)            comp_syn__else_ok(_Payload_Capture)

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
#define return_void() comp_syn__return_void()

/* Error result specific */
#define use_ErrSet$(T_Err, T_Ok)  comp_gen__use_ErrSet$(T_Err, T_Ok)
#define decl_ErrSet$(T_Err, T_Ok) comp_gen__decl_ErrSet$(T_Err, T_Ok)
#define impl_ErrSet$(T_Err, T_Ok) comp_gen__impl_ErrSet$(T_Err, T_Ok)

/*========== Implementations ================================================*/

#define comp_type_unnamed__Err$(T_Ok) \
    struct {                          \
        bool is_err;                  \
        union {                       \
            Err  err;                 \
            T_Ok ok;                  \
        };                            \
    }
#define comp_gen__use_Err$(T_Ok) \
    decl_Err$(T_Ok);             \
    impl_Err$(T_Ok)
#define comp_gen__decl_Err$(T_Ok)                                                   \
    typedef struct pp_join($, Err$Ptr_const, T_Ok) pp_join($, Err$Ptr_const, T_Ok); \
    typedef struct pp_join($, Err$Ptr, T_Ok) pp_join($, Err$Ptr, T_Ok);             \
    typedef struct pp_join($, Err, T_Ok) pp_join($, Err, T_Ok)
#define comp_gen__impl_Err$(T_Ok)            \
    struct pp_join($, Err$Ptr_const, T_Ok) { \
        bool is_err;                         \
        union {                              \
            Err err;                         \
            rawptr_const$(T_Ok) ok;          \
        };                                   \
    };                                       \
    struct pp_join($, Err$Ptr, T_Ok) {       \
        bool is_err;                         \
        union {                              \
            Err err;                         \
            rawptr$(T_Ok) ok;                \
        };                                   \
    };                                       \
    struct pp_join($, Err, T_Ok) {           \
        bool is_err;                         \
        union {                              \
            Err  err;                        \
            T_Ok ok;                         \
        };                                   \
    }
#define comp_op__Err_asNamed$(T_NamedErr, var_unnamed_err) eval({                                        \
    let _unnamed_err = var_unnamed_err;                                                                  \
    claim_assert_static(sizeOf(TypeOf(_unnamed_err)) == sizeOf(T_NamedErr));                             \
    claim_assert_static(alignOf(TypeOf(_unnamed_err)) == alignOf(T_NamedErr));                           \
    claim_assert_static(hasField(TypeOf(_unnamed_err), is_err));                                         \
    claim_assert_static(validateField(TypeOf(_unnamed_err), is_err, FieldTypeOf(T_NamedErr, is_err)));   \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), is_err) == fieldPadding(T_NamedErr, is_err)); \
    claim_assert_static(hasField(TypeOf(_unnamed_err), err));                                            \
    claim_assert_static(validateField(TypeOf(_unnamed_err), err, FieldTypeOf(T_NamedErr, err)));         \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), err) == fieldPadding(T_NamedErr, err));       \
    claim_assert_static(hasField(TypeOf(_unnamed_err), ok));                                             \
    claim_assert_static(validateField(TypeOf(_unnamed_err), ok, FieldTypeOf(T_NamedErr, ok)));           \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_err), ok) == fieldPadding(T_NamedErr, ok));         \
    eval_return(*(T_NamedErr*)&_unnamed_err);                                                            \
})

#define comp_op__err(val_err...) { .is_err = true, .err = val_err }
#define comp_op__ok(val_ok...)   { .is_err = false, .ok = val_ok }

#define comp_op__err$(T_Err, val_err...) ((T_Err)err(val_err))
#define comp_op__ok$(T_Ok, val_ok)       ((T_Ok)ok(val_ok))

#define comp_op__errAsg(__addr_result, var_addr_result, val_err...) eval({ \
    let __addr_result = var_addr_result;                                   \
    *__addr_result    = err$(TypeOf(*__addr_result), val_err);             \
    eval_return __addr_result;                                             \
})
#define comp_op__okAsg(__addr_result, var_result, val_ok...) eval({ \
    let __addr_result = var_result;                                 \
    *__addr_result    = ok$(TypeOf(*__addr_result), val_ok);        \
    eval_return __addr_result;                                      \
})

#define comp_op__isErr(val_result) ((val_result).is_err)
#define comp_op__isOk(val_result)  (!isErr(val_result))

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define comp_syn__return_err(val_err...)                       \
    debug_point ErrTrace_captureFrame();                       \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                                        \
        .err    = val_err,                                     \
    })
#define comp_syn__return_ok(val_ok...)                         \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = val_ok,                                      \
    })

#define comp_op__try(_Expr) ({   \
    let _result = (_Expr);       \
    if (_result.is_err) {        \
        return_err(_result.err); \
    }                            \
    _result.ok;                  \
})

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_syn__return_err(val_err...)           \
    debug_point ErrTrace_captureFrame();           \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = true,                            \
        .err    = val_err,                         \
    })
#define comp_syn__return_ok(val_ok...)             \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = val_ok,                          \
    })

#define comp_op__try(_Expr) ({   \
    let _result = (_Expr);       \
    if (_result.is_err) {        \
        return_err(_result.err); \
    }                            \
    _result.ok;                  \
})

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_op__catch_(_Expr, val_ok_default...) eval({        \
    var _result = (_Expr);                                      \
    ErrTrace_reset();                                           \
    eval_return _result.is_err ? (val_ok_default) : _result.ok; \
})
#define comp_op__catch_from(_Expr, _Payload_Capture, _Stmt_Block...) eval({ \
    var _result = (_Expr);                                                  \
    if (isErr(_result)) {                                                   \
        let _Payload_Capture = _result.err;                                 \
        _Stmt_Block;                                                        \
        ErrTrace_reset();                                                   \
    };                                                                      \
    eval_return _result.ok;                                                 \
})

#define comp_syn__errdefer(_Stmt...) defer_(                    \
    if (getReservedReturn() && getReservedReturn()[0].is_err) { \
        _Stmt;                                                  \
    }                                                           \
)

#define comp_syn__if_ok(val_result, _Payload_Capture)     \
    scope_if(let _result = (val_result), !_result.is_err) \
        scope_with(let _Payload_Capture = _result.ok)
#define comp_syn__else_err(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.err)
#define comp_syn__if_err(val_result, _Payload_Capture)   \
    scope_if(let _result = (val_result), _result.is_err) \
        scope_with(let _Payload_Capture = _result.err)
#define comp_syn__else_ok(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.ok)

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define comp_syn__return_void()                                \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                                       \
        .ok     = (Void){},                                    \
    })

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_syn__return_void()                    \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err = false,                           \
        .ok     = (Void){},                        \
    })

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_gen__use_ErrSet$(T_Err, T_Ok) \
    decl_ErrSet$(T_Err, T_Ok);             \
    impl_ErrSet$(T_Err, T_Ok)
#define comp_gen__decl_ErrSet$(T_Err, T_Ok)                                                 \
    typedef struct pp_join3($, T_Err, Ptr_const, T_Ok) pp_join3($, T_Err, Ptr_const, T_Ok); \
    typedef struct pp_join3($, T_Err, Ptr, T_Ok) pp_join3($, T_Err, Ptr, T_Ok);             \
    typedef struct pp_join($, T_Err, T_Ok) pp_join($, T_Err, T_Ok)
#define comp_gen__impl_ErrSet$(T_Err, T_Ok)      \
    struct pp_join3($, T_Err, Ptr_const, T_Ok) { \
        bool is_err;                             \
        union {                                  \
            T_Err err;                           \
            rawptr_const$(T_Ok) ok;              \
        };                                       \
    };                                           \
    struct pp_join3($, T_Err, Ptr, T_Ok) {       \
        bool is_err;                             \
        union {                                  \
            T_Err err;                           \
            rawptr$(T_Ok) ok;                    \
        };                                       \
    };                                           \
    struct pp_join($, T_Err, T_Ok) {             \
        bool is_err;                             \
        union {                                  \
            T_Err err;                           \
            T_Ok  ok;                            \
        };                                       \
    }

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
config_ErrSet(
    math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_ErrSet$(math_Err, i32); // or Generally `use_Err$(i32)`
static must_check math_Err$i32 safeDivide(i32 lhs, i32 rhs) {
    reserveReturn(math_Err$i32);
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(lhs / rhs);
}

static must_check Err$void test(void) {
    reserveReturn(Err$void);

    let result_invalid  = try_(safeDivide(10, 0));
    let result_default  = catch_(safeDivide(10, 0), 1);
    let result_handling = catch_from(safeDivide(10, 0), err, {
        Err_print(err);
        ErrTrace_print();
        return_err(err);
    });

    return_void();
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
