/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    opt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-26 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.1
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

/*========== Definitions ====================================================*/

/* Optional value */
#define Opt$(T)                                   comp_type_unnamed__Opt$(T)
#define use_Opt$(T)                               comp_gen__use_Opt$(T)
#define decl_Opt$(T)                              comp_gen__decl_Opt$(T)
#define impl_Opt$(T)                              comp_gen__impl_Opt$(T)
#define Opt_asNamed$(T_NamedOpt, var_unnamed_opt) comp_op__Opt_asNamed$(T_NamedOpt, var_unnamed_opt)

/* Determines optional value */
#define some(val_some...) comp_op__some(val_some)
#define none()            comp_op__none()

#define some$(T_Opt, val_some...) comp_op__some$(T_Opt, val_some)
#define none$(T_Opt)              comp_op__none$(T_Opt)

#define someAsg(var_addr_opt, val_some...) comp_op__someAsg(pp_uniqTok(addr_opt), var_addr_opt, val_some)
#define noneAsg(var_addr_opt...)           comp_op__noneAsg(pp_uniqTok(addr_opt), var_addr_opt)

/* Checks optional value */
#define isSome(val_opt) comp_op__isSome(val_opt)
#define isNone(val_opt) comp_op__isNone(val_opt)

/* Returns optional value */
#define return_some(val_some...) comp_syn__return_some(val_some)
#define return_none()            comp_syn__return_none()

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define unwrap(_Expr)                              comp_op__unwrap(_Expr)
#define orelse_default(_Expr, val_some_default...) comp_op__orelse_default(_Expr, val_some_default)
#define orelse(_Expr, _Stmt_Block...)              comp_op__orelse(_Expr, _Stmt_Block)

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(val_opt, _Payload_Capture)        comp_syn__if_some(val_opt, _Payload_Capture)
#define if_some_mut(var_opt, _Payload_Capture)    comp_syn__if_some_mut(var_opt, _Payload_Capture)
#define if_none(val_opt)                          comp_syn__if_none(val_opt)
#define else_some(_Payload_Capture)               comp_syn__else_some(_Payload_Capture)
#define else_some_mut(_Payload_Capture)           comp_syn__else_some_mut(_Payload_Capture)
#define while_some(val_opt, _Payload_Capture)     comp_syn__while_some(val_opt, _Payload_Capture)
#define while_some_mut(var_opt, _Payload_Capture) comp_syn__while_some_mut(var_opt, _Payload_Capture)
#define while_none(val_opt)                       comp_syn__while_none(val_opt)

/* Optional pointer */
#define Optptr_const$(T)                                   comp_type_unnamed__Optptr_const$(T)
#define Optptr$(T)                                         comp_type_unnamed__Optptr$(T)
#define use_Optptr$(T)                                     comp_gen__use_Optptr$(T)
#define decl_Optptr$(T)                                    comp_gen__decl_Optptr$(T)
#define impl_Optptr$(T)                                    comp_gen__impl_Optptr$(T)
#define Optptr_asNamed$(T_NamedOptptr, var_unnamed_optptr) comp_op__Optptr_asNamed$(T_NamedOptptr, var_unnamed_optptr)

/* Determines optional pointer */
#undef nullptr
#define someptr(val_someptr...) comp_op__someptr(val_someptr)
#define nullptr()               comp_op__nullptr()

#define someptr$(T_Optptr, val_someptr...) comp_op__someptr$(T_Optptr, val_someptr)
#define nullptr$(T_Optptr)                 comp_op__nullptr$(T_Optptr)

#define someptrAsg(var_addr_optptr, val_someptr...) comp_op__someptrAsg(pp_uniqTok(addr_optptr), var_addr_optptr, val_someptr)
#define nullptrAsg(var_addr_optptr...)              comp_op__nullptrAsg(pp_uniqTok(addr_optptr), var_addr_optptr)

/* Checks optional pointer */
#define isSomeptr(val_optptr) comp_op__isSomeptr(val_optptr)
#define isNullptr(val_optptr) comp_op__isNullptr(val_optptr)

/* Returns optional pointer */
#define return_someptr(val_someptr...) comp_syn__return_someptr(val_someptr)
#define return_nullptr()               comp_syn__return_nullptr()

/* Unwraps optional pointer (similar to Zig's orelse and .?) */
#define unwrapptr(_Expr)                                 comp_op__unwrapptr(_Expr)
#define orelseptr_default(_Expr, val_someptr_default...) comp_op__orelseptr_default(_Expr, val_someptr_default)
#define orelseptr(_Expr, _Stmt_Block...)                 comp_op__orelseptr(_Expr, _Stmt_Block)

/* Optional pointer payload capture (similar to Zig's if/while captures) */
#define if_someptr(val_optptr, _Payload_Capture)    comp_syn__if_someptr(val_optptr, _Payload_Capture)
#define if_nullptr(val_optptr)                      comp_syn__if_nullptr(val_optptr)
#define else_someptr(_Payload_Capture)              comp_syn__else_someptr(_Payload_Capture)
#define while_someptr(val_optptr, _Payload_Capture) comp_syn__while_someptr(val_optptr, _Payload_Capture)
#define while_nullptr(val_optptr)                   comp_syn__while_nullptr(val_optptr)

/*========== Implementations ================================================*/

#define comp_type_unnamed__Opt$(T) \
    struct {                       \
        bool has_value;            \
        T    value;                \
    }
#define comp_gen__use_Opt$(T) \
    decl_Opt$(T);             \
    impl_Opt$(T)
#define comp_gen__decl_Opt$(T)                                                \
    typedef struct pp_join($, Opt$Ptr_const, T) pp_join($, Opt$Ptr_const, T); \
    typedef struct pp_join($, Opt$Ptr, T) pp_join($, Opt$Ptr, T);             \
    typedef struct pp_join($, Opt, T) pp_join($, Opt, T)
#define comp_gen__impl_Opt$(T)            \
    struct pp_join($, Opt$Ptr_const, T) { \
        bool has_value;                   \
        rawptr_const$(T) value;           \
    };                                    \
    struct pp_join($, Opt$Ptr, T) {       \
        bool has_value;                   \
        rawptr$(T) value;                 \
    };                                    \
    struct pp_join($, Opt, T) {           \
        bool has_value;                   \
        T    value;                       \
    }
#define comp_op__Opt_asNamed$(T_NamedOpt, var_unnamed_opt) eval({                                              \
    let _unnamed_opt = var_unnamed_opt;                                                                        \
    claim_assert_static(sizeOf(TypeOf(_unnamed_opt)) == sizeOf(T_NamedOpt));                                   \
    claim_assert_static(alignOf(TypeOf(_unnamed_opt)) == alignOf(T_NamedOpt));                                 \
    claim_assert_static(hasField(TypeOf(_unnamed_opt), has_value));                                            \
    claim_assert_static(validateField(TypeOf(_unnamed_opt), has_value, FieldTypeOf(T_NamedOpt, has_value)));   \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_opt), has_value) == fieldPadding(T_NamedOpt, has_value)); \
    claim_assert_static(hasField(TypeOf(_unnamed_opt), value));                                                \
    claim_assert_static(validateField(TypeOf(_unnamed_opt), value, FieldTypeOf(T_NamedOpt, value)));           \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_opt), value) == fieldPadding(T_NamedOpt, value));         \
    eval_return(*(T_NamedOpt*)&_unnamed_opt);                                                                  \
})

#define comp_op__some(val_some...) { .has_value = true, .value = val_some }
#define comp_op__none()            { .has_value = false }

#define comp_op__some$(T_Opt, val_some...) ((T_Opt)some(val_some))
#define comp_op__none$(T_Opt)              ((T_Opt)none())

#define comp_op__someAsg(__addr_opt, var_addr_opt, val_some...) eval({ \
    let __addr_opt = var_addr_opt;                                     \
    debug_assert_nonnull(__addr_opt);                                  \
    *__addr_opt = some$(TypeOf(*__addr_opt), val_some);                \
    eval_return __addr_opt;                                            \
})
#define comp_op__noneAsg(__addr_opt, var_addr_opt...) eval({ \
    let __addr_opt = var_addr_opt;                           \
    debug_assert_nonnull(__addr_opt);                        \
    *__addr_opt = none$(TypeOf(*__addr_opt));                \
    eval_return __addr_opt;                                  \
})

#define comp_op__isSome(val_opt) ((val_opt).has_value)
#define comp_op__isNone(val_opt) (!isSome(val_opt))

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define comp_syn__return_some(val_some...)                     \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                                     \
        .value     = val_some,                                 \
    })

#define comp_syn__return_none()                                \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                                    \
    })

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_syn__return_some(val_some...)         \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                         \
        .value     = val_some,                     \
    })

#define comp_syn__return_none()                    \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                        \
    })

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define comp_op__orelse_default(_Expr, val_some_default...) eval({      \
    var _result = (_Expr);                                              \
    eval_return _result.has_value ? _result.value : (val_some_default); \
})
#define comp_op__orelse(_Expr, _Stmt_Block...) eval({ \
    var _result = (_Expr);                            \
    if (isNone(_result)) {                            \
        _Stmt_Block;                                  \
    }                                                 \
    eval_return _result.value;                        \
})
#define comp_op__unwrap(_Expr) orelse(_Expr, claim_unreachable)

#define comp_syn__if_some(val_opt, _Payload_Capture) \
    if_(let _result = (val_opt), _result.has_value)  \
        with_(let _Payload_Capture = _result.value)
#define comp_syn__if_some_mut(var_opt, _Payload_Capture) \
    if_(let _result = &(var_opt), _result->has_value)    \
        with_(let _Payload_Capture = &_result->value)
#define comp_syn__if_none(val_opt) \
    if_(let _result = (val_opt), !_result.has_value)
#define comp_syn__else_some(_Payload_Capture) \
    else_(let _Payload_Capture = _result.value)
#define comp_syn__else_some_mut(var_opt, _Payload_Capture) \
    else_(let _Payload_Capture = &_result->value)
#define comp_syn__while_some(val_opt, _Payload_Capture)                   \
    for (var _result = (val_opt); _result.has_value; _result = (val_opt)) \
    with_(let _Payload_Capture = _result.value)
#define comp_syn__while_some_mut(var_opt, _Payload_Capture)                  \
    for (var _result = &(var_opt); _result->has_value; _result = &(var_opt)) \
    with_(let _Payload_Capture = &_result->value)
#define comp_syn__while_none(val_opt) \
    while_(var _result = (val_opt), !_result.has_value)

#define comp_type_unnamed__Optptr_const$(T) \
    struct {                                \
        rawptr_const$(T) ptr;               \
    }
#define comp_type_unnamed__Optptr$(T) \
    union {                           \
        Optptr_const$(T) as_const;    \
        struct {                      \
            rawptr$(T) ptr;           \
        };                            \
    }
#define comp_gen__use_Optptr$(T) \
    decl_Optptr$(T);             \
    impl_Optptr$(T)
#define comp_gen__decl_Optptr$(T)                                           \
    typedef struct pp_join($, Optptr_const, T) pp_join($, Optptr_const, T); \
    typedef union pp_join($, Optptr, T) pp_join($, Optptr, T)
#define comp_gen__impl_Optptr$(T)             \
    struct pp_join($, Optptr_const, T) {      \
        rawptr_const$(T) ptr;                 \
    };                                        \
    union pp_join($, Optptr, T) {             \
        pp_join($, Optptr_const, T) as_const; \
        struct {                              \
            rawptr$(T) ptr;                   \
        };                                    \
    }
#define comp_op__Optptr_asNamed$(T_NamedOptptr, var_unnamed_optptr)

#define comp_op__someptr(val_someptr...) { .ptr = val_someptr }
#define comp_op__nullptr()               { .ptr = null }

#define comp_op__someptr$(T_Optptr, val_someptr...) ((T_Optptr)someptr(val_someptr))
#define comp_op__nullptr$(T_Optptr)                 ((T_Optptr) nullptr())

#define comp_op__someptrAsg(__addr_optptr, var_addr_optptr, val_someptr) eval({ \
    let __addr_optptr = var_addr_optptr;                                        \
    debug_assert_nonnull(__addr_optptr);                                        \
    *(__addr_optptr) = someptr$(TypeOf(*(__addr_optptr)), val_someptr);         \
    eval_return __addr_optptr;                                                  \
})

#define comp_op__nullptrAsg(__addr_optptr, var_addr_optptr) eval({ \
    let __addr_optptr = var_addr_optptr;                           \
    debug_assert_nonnull(__addr_optptr);                           \
    *(__addr_optptr) = nullptr$(TypeOf(*(__addr_optptr)));         \
    eval_return __addr_optptr;                                     \
})




#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
