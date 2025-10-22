/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    opt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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
#include "fn.h"
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
#define some(val_some...)         comp_op__some(val_some)
#define some$(T_Opt, val_some...) comp_op__some$(T_Opt, val_some)
#define none()                    comp_op__none()
#define none$(T_Opt)              comp_op__none$(T_Opt)

/* Assigns optional value */
#define Opt_asg$(T_Opt, var_addr_opt, val_opt...) comp_op__Opt_asg$(pp_uniqTok(addr_opt), T_Opt, var_addr_opt, val_opt)
#define Opt_asg(var_addr_opt, val_opt...)         comp_op__Opt_asg(var_addr_opt, val_opt)
#define toSome$(T_Opt, var_addr_opt, val_some...) comp_op__toSome$(T_Opt, var_addr_opt, val_some)
#define toSome(var_addr_opt, val_some...)         comp_op__toSome(var_addr_opt, val_some)
#define toNone$(T_Opt, var_addr_opt...)           comp_op__toNone$(T_Opt, var_addr_opt)
#define toNone(var_addr_opt...)                   comp_op__toNone(var_addr_opt)

/* Checks optional value */
#define isSome(val_opt...) comp_op__isSome(val_opt)
#define isNone(val_opt...) comp_op__isNone(val_opt)

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_(_Expr...) /* orelse_((_Expr...)(_DefaultExpr_OR_Body...)) */ \
    pp_expand(pp_defer(orelse)(pp_Tuple_unwrapSufCommaExpand _Expr))
#define unwrap_(_Expr...)                      comp_op__unwrap(_Expr)
#define orelse(_Expr, _DefaultExpr_OR_Body...) comp_op__orelse(pp_uniqTok(result), _Expr, ({ _DefaultExpr_OR_Body; }))
#define unwrap(_Expr...)                       comp_op__unwrap(_Expr)

/* Converts optional value to pointer */
#define Opt_asPtr$(T_OptPtr, var_addr_opt...) comp_op__Opt_asPtr$(pp_uniqTok(addr_opt), T_OptPtr, var_addr_opt)
#define Opt_asPtr(var_addr_opt...)            comp_op__Opt_asPtr(var_addr_opt)

/* Returns optional value */
#define return_some(val_some...) comp_syn__return_some(val_some)
#define return_none()            comp_syn__return_none()

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(val_opt, _Payload_Capture...)    comp_syn__if_some(val_opt, _Payload_Capture)
#define else_none                                comp_syn__else_none
#define if_none(val_opt...)                      comp_syn__if_none(val_opt)
#define else_some(_Payload_Capture...)           comp_syn__else_some(_Payload_Capture)
#define while_some(val_opt, _Payload_Capture...) comp_syn__while_some(val_opt, _Payload_Capture)
#define while_none(val_opt...)                   comp_syn__while_none(val_opt)

/*========== Implementations ================================================*/

#define comp_gen__use_Opt$(T) \
    use_Ptr$(T); \
    decl_Opt$(T); \
    impl_Opt$(T)
#define comp_gen__decl_Opt$(T) \
    $maybe_unused typedef struct Opt$(Ptr_const$(T)) Opt$(Ptr_const$(T)); \
    $maybe_unused typedef struct Opt$(Ptr$(T)) Opt$(Ptr$(T)); \
    $maybe_unused typedef struct Opt$(T) Opt$(T)
#define comp_gen__impl_Opt$(T) \
    struct Opt$(Ptr_const$(T)) { \
        rawptr_const$(T) value; \
        bool has_value; \
    }; \
    struct Opt$(Ptr$(T)) { \
        rawptr$(T) value; \
        bool has_value; \
    }; \
    struct Opt$(T) { \
        T    value; \
        bool has_value; \
    }

#define comp_type_alias__Opt$(T) \
    pp_join($, Opt, T)
#define comp_type_anon__Opt$$(T) \
    struct { \
        T    value; \
        bool has_value; \
    }
#define comp_op__Opt_anonCast$(T_Opt, var_anon...) eval({ \
    let __anon = var_anon; \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_Opt)); \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_Opt)); \
    claim_assert_static(hasField(TypeOf(__anon), has_value)); \
    claim_assert_static(validateField(TypeOf(__anon), has_value, FieldTypeOf(T_Opt, has_value))); \
    claim_assert_static(fieldPadding(TypeOf(__anon), has_value) == fieldPadding(T_Opt, has_value)); \
    claim_assert_static(hasField(TypeOf(__anon), value)); \
    claim_assert_static(validateField(TypeOf(__anon), value, FieldTypeOf(T_Opt, value))); \
    claim_assert_static(fieldPadding(TypeOf(__anon), value) == fieldPadding(T_Opt, value)); \
    eval_return(*(T_Opt*)&__anon); \
})

#define comp_op__some(val_some...)         { .has_value = true, .value = val_some }
#define comp_op__some$(T_Opt, val_some...) ((T_Opt)some(val_some))
#define comp_op__none()                    { .has_value = false }
#define comp_op__none$(T_Opt)              ((T_Opt)none())

#define comp_op__Opt_asg$(__addr_opt, T_Opt, var_addr_opt, val_opt...) eval({ \
    let __addr_opt = var_addr_opt; \
    debug_assert_nonnull(__addr_opt); \
    *__addr_opt = *(T_Opt[1]){ [0] = val_opt }; \
    eval_return __addr_opt; \
})
#define comp_op__Opt_asg(var_addr_opt, val_opt...)         Opt_asg$(TypeOf(*var_addr_opt), var_addr_opt, val_opt)
#define comp_op__toSome$(T_Opt, var_addr_opt, val_some...) Opt_asg$(T_Opt, var_addr_opt, some(val_some))
#define comp_op__toSome(var_addr_opt, val_some...)         toSome$(TypeOf(*var_addr_opt), var_addr_opt, val_some)
#define comp_op__toNone$(T_Opt, var_addr_opt...)           Opt_asg$(T_Opt, var_addr_opt, none())
#define comp_op__toNone(var_addr_opt...)                   toNone$(TypeOf(*var_addr_opt), var_addr_opt)

#define comp_op__isSome(val_opt...) as$((bool)((val_opt).has_value))
#define comp_op__isNone(val_opt...) as$((bool)((!(val_opt).has_value)))

#define comp_syn__return_some(val_some...) return_(some(val_some))
#define comp_syn__return_none()            return_(none())


#define genericIf$(_Expr, _T, _True, _False...) _Generic(TypeOfUnqual(_Expr), _T: _True, default: _False)
#define genericExprOrDefaultEvaledIfVoid$(_Expr, _T...) \
    genericIf$(_Expr, void, ({ $ignore_void(_Expr); make$(TypeOf(_T)); }), (_Expr))
#define genericExprOrDefaultIfNotT$(_Expr, _T...) \
    genericIf$(_Expr, _T, (_Expr), make$(_T))

// #define defaultEvaledIfVoid$(_T, _Expr...) bti_Generic_match$( \
//     TypeOfUnqual(_Expr), \
//     bti_Generic_pattern$(void)({ \
//         $ignore_void(_Expr); \
//         make$(TypeOf(_T)); \
//     }), \
//     bti_Generic_fallback_(_Expr) \
// )

#define comp_op__orelse(__result, _Expr, _DefaultExpr_OR_Body...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    eval({ \
        var __result = _Expr; \
        if (isNone(__result)) { \
            __result.value = bti_Generic_match$( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                bti_Generic_pattern$(void) eval({ \
                    $ignore_void _DefaultExpr_OR_Body; \
                    eval_return make$(TypeOf(__result.value)); \
                }), \
                bti_Generic_fallback_ _DefaultExpr_OR_Body \
            ); \
        } \
        eval_return __result.value; \
    }) \
)
#define comp_op__unwrap(_Expr...) orelse_((_Expr)(claim_unreachable))

#define comp_op__Opt_asPtr$(__addr_opt, T_OptPtr, var_addr_opt...) eval({ \
    let __addr_opt = var_addr_opt; \
    debug_assert_nonnull(__addr_opt); \
    (T_OptPtr){ \
        .value     = &__addr_opt->value, \
        .has_value = __addr_opt->has_value \
    }; \
})
#define comp_op__Opt_asPtr(var_addr_opt...) \
    Opt_asPtr$(Opt$$(FieldTypeOf(TypeOf(*var_addr_opt), value)*), var_addr_opt)

#define comp_syn__if_some(val_opt, _Payload_Capture...) \
    if_(let _result = (val_opt), _result.has_value) \
        with_(let _Payload_Capture = _result.value)
#define comp_syn__else_none \
    else
#define comp_syn__if_none(val_opt...) \
    if_(let _result = (val_opt), !_result.has_value)
#define comp_syn__else_some(_Payload_Capture...) \
    else_(let _Payload_Capture = _result.value)
#define comp_syn__while_some(val_opt, _Payload_Capture...) \
    for (var _result = (val_opt); _result.has_value; _result = (val_opt)) \
    with_(let _Payload_Capture = _result.value)
#define comp_syn__while_none(val_opt...) \
    while_(var _result = (val_opt), !_result.has_value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
