/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2025-03-18 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Optional value implementation
 * @details Provides a type for representing optional values, which may or may not contain a value.
 */

#ifndef OPT_INCLUDED
#define OPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "scope.h"
#include "ptr.h"

/*========== Definitions ====================================================*/

/* Optional value */
#define use_Opt$(T)  comp_gen__use_Opt$(T)
#define decl_Opt$(T) comp_gen__decl_Opt$(T)
#define impl_Opt$(T) comp_gen__impl_Opt$(T)

#define Opt$(T)                           comp_type_alias__Opt$(T)
#define Opt$$(T)                          comp_type_anon__Opt$$(T)
#define Opt_anonCast$(T_Opt, var_anon...) comp_op__Opt_anonCast$(T_Opt, var_anon)

/* Determines optional value */
#define some(val_some...) comp_op__some(val_some)
#define none()            comp_op__none()

#define some$(T_Opt, val_some...) comp_op__some$(T_Opt, val_some)
#define none$(T_Opt)              comp_op__none$(T_Opt)

#define toSome(var_addr_opt, val_some...) comp_op__toSome(pp_uniqTok(addr_opt), var_addr_opt, val_some)
#define toNone(var_addr_opt...)           comp_op__toNone(pp_uniqTok(addr_opt), var_addr_opt)

/* Checks optional value */
#define isSome(val_opt...) comp_op__isSome(val_opt)
#define isNone(val_opt...) comp_op__isNone(val_opt)

/* Returns optional value */
#define return_some(val_some...) comp_syn__return_some(val_some)
#define return_none()            comp_syn__return_none()

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define unwrap(_Expr...)                   comp_op__unwrap(_Expr)
#define orelse(_Expr, _Default_Or_Eval...) comp_op__orelse(pp_uniqTok(result), _Expr, _Default_Or_Eval)

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(val_opt, _Payload_Capture)    comp_syn__if_some(val_opt, _Payload_Capture)
#define else_none                             comp_syn__else_none
#define if_none(val_opt)                      comp_syn__if_none(val_opt)
#define else_some(_Payload_Capture)           comp_syn__else_some(_Payload_Capture)
#define while_some(val_opt, _Payload_Capture) comp_syn__while_some(val_opt, _Payload_Capture)
#define while_none(val_opt)                   comp_syn__while_none(val_opt)

/*========== Implementations ================================================*/

#define comp_gen__use_Opt$(T) \
    use_Ptr$(T);              \
    decl_Opt$(T);             \
    impl_Opt$(T)
#define comp_gen__decl_Opt$(T)                              \
    typedef struct Opt$(Ptr_const$(T)) Opt$(Ptr_const$(T)); \
    typedef struct Opt$(Ptr$(T)) Opt$(Ptr$(T));             \
    typedef struct Opt$(T) Opt$(T)
#define comp_gen__impl_Opt$(T)   \
    struct Opt$(Ptr_const$(T)) { \
        rawptr_const$(T) value;  \
        bool has_value;          \
    };                           \
    struct Opt$(Ptr$(T)) {       \
        rawptr$(T) value;        \
        bool has_value;          \
    };                           \
    struct Opt$(T) {             \
        T    value;              \
        bool has_value;          \
    }

#define comp_type_alias__Opt$(T) \
    pp_join($, Opt, T)
#define comp_type_anon__Opt$$(T) \
    struct {                     \
        T    value;              \
        bool has_value;          \
    }
#define comp_op__Opt_anonCast$(T_Opt, var_anon...) eval({                                           \
    let __anon = var_anon;                                                                          \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_Opt));                                   \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_Opt));                                 \
    claim_assert_static(hasField(TypeOf(__anon), has_value));                                       \
    claim_assert_static(validateField(TypeOf(__anon), has_value, FieldTypeOf(T_Opt, has_value)));   \
    claim_assert_static(fieldPadding(TypeOf(__anon), has_value) == fieldPadding(T_Opt, has_value)); \
    claim_assert_static(hasField(TypeOf(__anon), value));                                           \
    claim_assert_static(validateField(TypeOf(__anon), value, FieldTypeOf(T_Opt, value)));           \
    claim_assert_static(fieldPadding(TypeOf(__anon), value) == fieldPadding(T_Opt, value));         \
    eval_return(*(T_Opt*)&__anon);                                                                  \
})

#define comp_op__some(val_some...) { .has_value = true, .value = val_some }
#define comp_op__none()            { .has_value = false }

#define comp_op__some$(T_Opt, val_some...) ((T_Opt)some(val_some))
#define comp_op__none$(T_Opt)              ((T_Opt)none())

#define comp_op__toSome(__addr_opt, var_addr_opt, val_some...) eval({ \
    let __addr_opt = var_addr_opt;                                    \
    debug_assert_nonnull(__addr_opt);                                 \
    *__addr_opt = some$(TypeOf(*__addr_opt), val_some);               \
    eval_return __addr_opt;                                           \
})
#define comp_op__toNone(__addr_opt, var_addr_opt...) eval({ \
    let __addr_opt = var_addr_opt;                          \
    debug_assert_nonnull(__addr_opt);                       \
    *__addr_opt = none$(TypeOf(*__addr_opt));               \
    eval_return __addr_opt;                                 \
})

#define comp_op__isSome(val_opt...) ((val_opt).has_value)
#define comp_op__isNone(val_opt...) (!(val_opt).has_value)

#define comp_syn__return_some(val_some...)         \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                         \
        .value     = val_some,                     \
    })
#define comp_syn__return_none()                    \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                        \
    })

#define comp_op__unwrap(_Expr...)                             orelse(_Expr, claim_unreachable)
#define comp_op__orelse(__result, _Expr, _Default_Or_Eval...) eval({ \
    var __result = _Expr;                                            \
    if (isNone(__result)) {                                          \
        __result.value = bti_Generic_match$(                         \
            TypeOf(_Default_Or_Eval),                                \
            bti_Generic_pattern$(void) eval({                        \
                $ignore _Default_Or_Eval;                            \
                eval_return make$(TypeOf(__result.value));           \
            }),                                                      \
            bti_Generic_fallback_ _Default_Or_Eval                   \
        );                                                           \
    }                                                                \
    eval_return __result.value;                                      \
})

#define comp_syn__if_some(val_opt, _Payload_Capture) \
    if_(let _result = (val_opt), _result.has_value)  \
        with_(let _Payload_Capture = _result.value)
#define comp_syn__else_none \
    else
#define comp_syn__if_none(val_opt) \
    if_(let _result = (val_opt), !_result.has_value)
#define comp_syn__else_some(_Payload_Capture) \
    else_(let _Payload_Capture = _result.value)
#define comp_syn__while_some(val_opt, _Payload_Capture)                   \
    for (var _result = (val_opt); _result.has_value; _result = (val_opt)) \
    with_(let _Payload_Capture = _result.value)
#define comp_syn__while_none(val_opt) \
    while_(var _result = (val_opt), !_result.has_value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
