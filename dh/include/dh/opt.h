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
#define Opt$(T)                                  TYPE_UNNAMED__Opt$(T)
#define use_Opt$(T)                              GEN__use_Opt$(T)
#define decl_Opt$(T)                             GEN__decl_Opt$(T)
#define impl_Opt$(T)                             GEN__impl_Opt$(T)
#define Opt_asNamed$(TNamedOpt, var_unnamed_opt) OP__Opt_asNamed$(TNamedOpt, var_unnamed_opt)

/* Determines optional value */
#define some(val_opt...) OP__some(val_opt)
#define none()           OP__none()

#define some$(TOpt, val_opt...) OP__some$(TOpt, val_opt)
#define none$(TOpt)             OP__none$(TOpt)

#define assignSome(var_opt, val_opt...) OP__assignSome(var_opt, val_opt)
#define assignNone(var_opt...)          OP__assignNone(var_opt)

/* Checks optional value */
#define isSome(opt) OP__isSome(opt)
#define isNone(opt) OP__isNone(opt)

/* Returns optional value */
#define return_some(val_opt...) SYN__return_some(val_opt)
#define return_none()           SYN__return_none()

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_default(expr, val_default...) OP__orelse_default(expr, val_default)
#define orelse(expr, body...)                OP__orelse(expr, body)
#define unwrap(expr)                         OP__unwrap(expr)

/* Optional payload capture (similar to Zig's if/while captures) */
#define if_some(expr, var_capture)     SYN__if_some(expr, var_capture)
#define if_some_mut(expr, var_capture) SYN__if_some_mut(expr, var_capture)
#define if_none(expr)                  SYN__if_none(expr)
#define else_some(var_capture)         SYN__else_some(var_capture)
#define else_some_mut(var_capture)     SYN__else_some_mut(var_capture)

/*========== Implementations ================================================*/

#define TYPE_UNNAMED__Opt$(T) \
    struct {                  \
        bool has_value;       \
        T    value;           \
    }
#define GEN__use_Opt$(T) \
    decl_Opt$(T);        \
    impl_Opt$(T)
#define GEN__decl_Opt$(T)                                                   \
    typedef struct pp_join($, Opt$PtrConst, T) pp_join($, Opt$PtrConst, T); \
    typedef struct pp_join($, Opt$Ptr, T) pp_join($, Opt$Ptr, T);           \
    typedef struct pp_join($, Opt, T) pp_join($, Opt, T)
#define GEN__impl_Opt$(T)                \
    struct pp_join($, Opt$PtrConst, T) { \
        bool has_value;                  \
        rawptr_const$(T) value;          \
    };                                   \
    struct pp_join($, Opt$Ptr, T) {      \
        bool has_value;                  \
        rawptr$(T) value;                \
    };                                   \
    struct pp_join($, Opt, T) {          \
        bool has_value;                  \
        T    value;                      \
    }
#define OP__Opt_asNamed$(TNamedOpt, var_unnamed_opt) eval({                                                   \
    let _unnamed_opt = var_unnamed_opt;                                                                       \
    claim_assert_static(sizeOf(TypeOf(_unnamed_opt)) == sizeOf(TNamedOpt));                                   \
    claim_assert_static(alignOf(TypeOf(_unnamed_opt)) == alignOf(TNamedOpt));                                 \
    claim_assert_static(hasField(TypeOf(_unnamed_opt), has_value));                                           \
    claim_assert_static(validateField(TypeOf(_unnamed_opt), has_value, FieldTypeOf(TNamedOpt, has_value)));   \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_opt), has_value) == fieldPadding(TNamedOpt, has_value)); \
    claim_assert_static(hasField(TypeOf(_unnamed_opt), value));                                               \
    claim_assert_static(validateField(TypeOf(_unnamed_opt), value, FieldTypeOf(TNamedOpt, value)));           \
    claim_assert_static(fieldPadding(TypeOf(_unnamed_opt), value) == fieldPadding(TNamedOpt, value));         \
    eval_return(*(TNamedOpt*)&_unnamed_opt);                                                                  \
})

#define OP__some(val_opt...) { .has_value = true, .value = val_opt }
#define OP__none()           { .has_value = false }

#define OP__some$(TOpt, val_opt...) ((TOpt)some(val_opt))
#define OP__none$(TOpt)             ((TOpt)none())

#define OP__assignSome(var_opt, val_opt...) eval({    \
    let _ptr_opt = &var_opt;                          \
    *_ptr_opt    = some$(TypeOf(*_ptr_opt), val_opt); \
})
#define OP__assignNone(var_opt...) eval({    \
    let _ptr_opt = &var_opt;                 \
    *_ptr_opt    = none$(TypeOf(*_ptr_opt)); \
})

#define OP__isSome(opt) ((opt).has_value)
#define OP__isNone(opt) (!isSome(opt))

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define SYN__return_some(val_opt...)                           \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                                     \
        .value     = val_opt,                                  \
    })

#define SYN__return_none()                                     \
    return setReservedReturn((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                                    \
    })

#else /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define SYN__return_some(val_opt...)               \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = true,                         \
        .value     = val_opt,                      \
    })

#define SYN__return_none()                         \
    scope_return((TypeOf(getReservedReturn()[0])){ \
        .has_value = false,                        \
    })

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define OP__orelse_default(expr, val_default...) eval({            \
    var _result = (expr);                                          \
    eval_return _result.has_value ? _result.value : (val_default); \
})
#define OP__orelse(expr, body...) eval({ \
    var _result = (expr);                \
    if (!_result.has_value) {            \
        body;                            \
    }                                    \
    eval_return _result.value;           \
})
#define OP__unwrap(expr) orelse(expr, claim_unreachable)

#define SYN__if_some(expr, var_capture)               \
    scope_if(let _result = (expr), _result.has_value) \
        scope_with(let var_capture = _result.value)
#define SYN__if_some_mut(expr, var_capture)             \
    scope_if(let _result = &(expr), _result->has_value) \
        scope_with(let var_capture = &_result->value)
#define SYN__if_none(expr) \
    scope_if(let _result = (expr), !_result.has_value)
#define SYN__else_some(var_capture) \
    scope_else(let var_capture = _result.value)
#define SYN__else_some_mut(expr, var_capture) \
    scope_else(let var_capture = &_result->value)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
