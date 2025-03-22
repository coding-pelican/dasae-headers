/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    err_res.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2025-03-18 (date of last update)
 * @version v0.1-alpha.3
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
#define Err$(T_Ok)                           comp_type_anon__Err$(T_Ok)
#define Err_anonCast$(T_ErrRes, var_anon...) comp_op__ErrRes_anonCast$(T_ErrRes, var_anon)

#define use_Err$(T_Ok)  comp_gen__use_Err$(T_Ok)
#define decl_Err$(T_Ok) comp_gen__decl_Err$(T_Ok)
#define impl_Err$(T_Ok) comp_gen__impl_Err$(T_Ok)

/* Determines error result */
#define err(val_err...) comp_op__err(val_err)
#define ok(val_ok...)   comp_op__ok(val_ok)

#define err$(T_ErrRes, val_err...) comp_op__err$(T_ErrRes, val_err)
#define ok$(T_ErrRes, val_ok...)   comp_op__ok$(T_ErrRes, val_ok)

#define errAsg(var_addr_result, val_err...) comp_op__errAsg(pp_uniqTok(addr_result), var_addr_result, val_err)
#define okAsg(var_addr_result, val_ok...)   comp_op__okAsg(pp_uniqTok(addr_result), var_addr_result, val_ok)

/* Checks error result */
#define isErr(val_result...) comp_op__isErr(val_result)
#define isOk(val_result...)  comp_op__isOk(val_result)

/* Returns error result */
#define return_err(val_err...) comp_syn__return_err(val_err)
#define return_ok(val_ok...)   comp_syn__return_ok(val_ok)

/* Propagates error (similar to Zig's try_) */
#define try_(_Expr...) comp_op__try_(pp_uniqTok(result), _Expr)

/* Handles error (similar to Zig's catch_from) */
#define catch_(_Expr, _Default_Or_Eval...)                       comp_op__catch_(pp_uniqTok(result), _Expr, _Default_Or_Eval)
#define catch_from(_Expr, _Payload_Capture, _Default_Or_Eval...) comp_op__catch_from(pp_uniqTok(result), _Expr, _Payload_Capture, _Default_Or_Eval)

/* Defers when error */
#define errdefer_(_Stmt...)                       comp_syn__errdefer_(_Stmt)
#define errdefer_from(_Payload_Capture, _Stmt...) comp_syn__errdefer_from(_Payload_Capture, _Stmt)

/* Error result payload captures */
#define if_ok(val_result, _Payload_Capture)  comp_syn__if_ok(val_result, _Payload_Capture)
#define else_err(_Payload_Capture)           comp_syn__else_err(_Payload_Capture)
#define if_err(val_result, _Payload_Capture) comp_syn__if_err(val_result, _Payload_Capture)
#define else_ok(_Payload_Capture)            comp_syn__else_ok(_Payload_Capture)

/* Error void result (special case) */
typedef struct Err$Void {
    bool is_err;
    union {
        Err  err;
        Void ok;
    } data;
} Err$Void, Err$void;
#define return_void() comp_syn__return_void()

/* Error result specific */
#define use_ErrSet$(T_Err, T_Ok)  comp_gen__use_ErrSet$(T_Err, T_Ok)
#define decl_ErrSet$(T_Err, T_Ok) comp_gen__decl_ErrSet$(T_Err, T_Ok)
#define impl_ErrSet$(T_Err, T_Ok) comp_gen__impl_ErrSet$(T_Err, T_Ok)

/*========== Implementations ================================================*/

#define comp_type_anon__Err$(T_Ok) \
    struct {                       \
        bool is_err;               \
        union {                    \
            Err  err;              \
            T_Ok ok;               \
        } data;                    \
    }
#define comp_op__Err_anonCast$(T_ErrRes, var_anon...) eval({                                         \
    let __anon = var_anon;                                                                           \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_ErrRes));                                 \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_ErrRes));                               \
    claim_assert_static(hasField(TypeOf(__anon), is_err));                                           \
    claim_assert_static(validateField(TypeOf(__anon), is_err, FieldTypeOf(T_ErrRes, is_err)));       \
    claim_assert_static(fieldPadding(TypeOf(__anon), is_err) == fieldPadding(T_ErrRes, is_err));     \
    claim_assert_static(hasField(TypeOf(__anon), data.err));                                         \
    claim_assert_static(validateField(TypeOf(__anon), data.err, FieldTypeOf(T_ErrRes, data.err)));   \
    claim_assert_static(fieldPadding(TypeOf(__anon), data.err) == fieldPadding(T_ErrRes, data.err)); \
    claim_assert_static(hasField(TypeOf(__anon), data.ok));                                          \
    claim_assert_static(validateField(TypeOf(__anondata.), ok, FieldTypeOf(T_ErrRes, data.ok)));     \
    claim_assert_static(fieldPadding(TypeOf(__anon), data.ok) == fieldPadding(T_ErrRes, data.ok));   \
    eval_return(*(T_ErrRes*)&__anon);                                                                \
})

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
        } data;                              \
    };                                       \
    struct pp_join($, Err$Ptr, T_Ok) {       \
        bool is_err;                         \
        union {                              \
            Err err;                         \
            rawptr$(T_Ok) ok;                \
        } data;                              \
    };                                       \
    struct pp_join($, Err, T_Ok) {           \
        bool is_err;                         \
        union {                              \
            Err  err;                        \
            T_Ok ok;                         \
        } data;                              \
    }

#define comp_op__err(val_err...) { .is_err = true, .data.err = val_err }
#define comp_op__ok(val_ok...)   { .is_err = false, .data.ok = val_ok }

#define comp_op__err$(T_ErrRes, val_err...) ((T_ErrRes)err(val_err))
#define comp_op__ok$(T_ErrRes, val_ok...)   ((T_ErrRes)ok(val_ok))

#define comp_op__errAsg(__addr_result, var_addr_result, val_err...) eval({ \
    const TypeOf(var_addr_result) __addr_result                            \
        = var_addr_result;                                                 \
    *__addr_result = err$(TypeOf(*__addr_result), val_err);                \
    eval_return __addr_result;                                             \
})
#define comp_op__okAsg(__addr_result, var_result, val_ok...) eval({ \
    const TypeOf(var_result) __addr_result                          \
        = var_result;                                               \
    *__addr_result = ok$(TypeOf(*__addr_result), val_ok);           \
    eval_return __addr_result;                                      \
})

#define comp_op__isErr(val_result...) ((val_result).is_err)
#define comp_op__isOk(val_result...)  (!(val_result).is_err)

#define comp_syn__return_err(val_err...)           \
    debug_point ErrTrace_captureFrame();           \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err   = true,                          \
        .data.err = val_err,                       \
    })
#define comp_syn__return_ok(val_ok...)             \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err  = false,                          \
        .data.ok = val_ok,                         \
    })

#define comp_op__try_(__result, _Expr...) eval({ \
    let __result = _Expr;                        \
    if (isErr(__result)) {                       \
        return_err(__result.data.err);           \
    }                                            \
    eval_return __result.data.ok;                \
})

#define comp_op__catch_(__result, _Expr, _Default_Or_Eval...) eval({ \
    var __result = _Expr;                                            \
    if (isErr(__result)) {                                           \
        __result.data.ok = bti_Generic_match$(                       \
            TypeOf(_Default_Or_Eval),                                \
            bti_Generic_pattern$(void) eval({                        \
                _Default_Or_Eval;                                    \
                eval_return make$(TypeOf(__result.data.ok));         \
            }),                                                      \
            bti_Generic_fallback_ _Default_Or_Eval                   \
        );                                                           \
    }                                                                \
    eval_return ErrTrace_reset(), __result.data.ok;                  \
})
#define comp_op__catch_from(__result, _Expr, _Payload_Capture, _Default_Or_Eval...) eval({ \
    var __result = _Expr;                                                                  \
    if (isErr(__result)) {                                                                 \
        let _Payload_Capture = __result.data.err;                                          \
        __result.data.ok     = bti_Generic_match$(                                         \
            TypeOf(_Default_Or_Eval),                                                  \
            bti_Generic_pattern$(void) eval({                                          \
                _Default_Or_Eval;                                                      \
                eval_return make$(TypeOf(__result.data.ok));                           \
            }),                                                                        \
            bti_Generic_fallback_ _Default_Or_Eval                                     \
        );                                                                             \
    }                                                                                      \
    eval_return ErrTrace_reset(), __result.data.ok;                                        \
})

#define comp_syn__errdefer_(_Stmt...) defer_(                   \
    if (getReservedReturn() && getReservedReturn()[0].is_err) { \
        _Stmt;                                                  \
    }                                                           \
)
#define comp_syn__errdefer_from(_Payload_Capture, _Stmt...) defer_( \
    if (getReservedReturn() && getReservedReturn()[0].is_err) {     \
        let _Payload_Capture = getReservedReturn()[0].data.err;     \
        _Stmt;                                                      \
    }                                                               \
)

#define comp_syn__if_ok(val_result, _Payload_Capture)     \
    scope_if(let _result = (val_result), !_result.is_err) \
        scope_with(let _Payload_Capture = _result.data.ok)
#define comp_syn__else_err(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.data.err)
#define comp_syn__if_err(val_result, _Payload_Capture)   \
    scope_if(let _result = (val_result), _result.is_err) \
        scope_with(let _Payload_Capture = _result.data.err)
#define comp_syn__else_ok(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.data.ok)

#define comp_syn__return_void()                    \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .is_err  = false,                          \
        .data.ok = (Void){},                       \
    })

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
        } data;                                  \
    };                                           \
    struct pp_join3($, T_Err, Ptr, T_Ok) {       \
        bool is_err;                             \
        union {                                  \
            T_Err err;                           \
            rawptr$(T_Ok) ok;                    \
        } data;                                  \
    };                                           \
    struct pp_join($, T_Err, T_Ok) {             \
        bool is_err;                             \
        union {                                  \
            T_Err err;                           \
            T_Ok  ok;                            \
        } data;                                  \
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
static fn_(safeDivide(i32 lhs, i32 rhs), must_check math_Err$i32);
static fn_(test(void), must_check Err$void);

static fn_ext_scope(safeDivide(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(lhs / rhs);
} ext_unscoped;
static fn_ext_scope(test(void), Err$void) {
    let result_invalid  = try_(safeDivide(10, 0));
    let result_default  = catch_(safeDivide(10, 0), 1);
    let result_handling = catch_from(safeDivide(10, 0), err, eval({
        Err_print(err);
        ErrTrace_print();
        return_err(err);
    }));
    return_void();
} ext_unscoped;
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_RES_INCLUDED */
