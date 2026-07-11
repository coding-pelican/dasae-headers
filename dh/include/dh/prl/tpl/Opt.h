#pragma once
#ifndef prl_tpl_Opt__included
#define prl_tpl_Opt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Optional Anonymous */
#define O$$(_$T...) \
    TypeOf(union { \
        T_embed$(struct { \
            var_(is_some, bool); \
            union { \
                var_(none, Void); \
                var_(some, _$T); \
            } payload; \
        }); \
        var_(as_raw $like_ref, O$raw); \
    })
/* Optional Alias */
#define O$(_$T...) tpl$(O, _$T)
/* Optional Template */
#define T_decl_O$(_$T...) \
    $maybe_unused typedef union O$(_$T) O$(_$T)
#define T_impl_O$(_$T...) \
    union O$(_$T) { \
        T_embed$(struct { \
            var_(is_some, bool); \
            union { \
                var_(none, Void); \
                var_(some, _$T); \
            } payload; \
        }); \
        var_(as_raw $like_ref, O$raw); \
    }
#define T_use_O$(_$T...) \
    T_decl_O$(_$T); \
    T_impl_O$(_$T)

#define O_T$(_$T...) TypeOf((as$(_$T*)(null))->payload.some)

/* Optional void value (special case) */
typedef union O$Void O$Void, O$void;
// typedef union O$Void {
//     T_embed$(struct {
//         var_(is_some, bool);
//         union {
//             var_(none, Void);
//             var_(some, Void);
//         } payload;
//     });
//     var_(as_raw $like_ref, O$raw);
// } O$Void, O$void;

/* Optional Operations */
// #define init$O()
// #define init$O$()
// #define asg$O()
// #define asg$O$()

// #define asg$O(/*(_$p_o: O$(_$T))(_$v_o: _$T|void)*/... /*(_$p_o: O$(_$T))*/) \
//     __asg$O__step(pp_defer(__asg$O__emit)(__asg$O__parseLhs __VA_ARGS__))
// #define __asg$O__step(...)                  __VA_ARGS__
// #define __asg$O__parseLhs(_$p_o...)          pp_uniqTok(p_o), _$p_o, __asg$O__parseRhs
// #define __asg$O__parseRhs(_$v_o...)          _$v_o
// #define __asg$O__emit(__p_o, _$p_o, _$v_o...) local_({ \
//     let_(__p_o, TypeOf(_$p_o)) = _$p_o; \
//     claim_assert_nonnull(__p_o); \
//     *__p_o = l$((TypeOf(*__p_o))_$v_o); \
//     local__return __p_o; \
// })

#define asgO(_$p_o, _$v_o...) asgO1(_$p_o, _$v_o)
#define asgO1(_$p_o, _$v_o...) asg(_$p_o, _$v_o, (payload.some))
#define asgO2(_$p_o, _$v_o...) asg(_$p_o, _$v_o, (payload.some.payload.some))
#define asgO3(_$p_o, _$v_o...) asg(_$p_o, _$v_o, (payload.some.payload.some.payload.some))
#define asgO4(_$p_o, _$v_o...) asg(_$p_o, _$v_o, (payload.some.payload.some.payload.some.payload.some))

/* Determines optional value */
#define some(_$val...) comp_inline__some(_$val)
#define comp_inline__some(_$val...) { \
    .is_some = true, \
    .payload = { .some = _$val }, \
}
#define some$(/*(_$T)_$val: _$T)*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__some$)(__param_expand__some$ __VA_ARGS__))

#define none() comp_inline__none()
#define comp_inline__none() { \
    .is_some = false, \
    .payload = { .none = {} }, \
}
#define none$(/*(_$T)*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__none$)(__param_expand__none$ __VA_ARGS__))

/* Checks optional value */
#define isSome(_$o /*: O$$(_$T)*/... /*(bool)*/) bool_((_$o).is_some)
#define isNone(_$o /*: O$$(_$T)*/... /*(bool)*/) bool_(!(_$o).is_some)

#define O_asP$(/*(_$OPT: O(P(T)))(_$p_o: P(O(T)))*/... /*(_$OPT)*/) \
    __step__O_asP$(__VA_ARGS__)
#define O_asP(_$p_o /*: P(O(T))*/... /*(O(P(T)))*/) \
    __step__O_asP(_$p_o)
#define O_ref$(/*(_$OPT: O(P(T)))(_$p_o: P(O(T)))*/... /*(_$OPT)*/) \
    __step__O_asP$(__VA_ARGS__)
#define O_ref(_$p_o /*: P(O(T))*/... /*(O(P(T)))*/) \
    __step__O_asP(_$p_o)
#define O_deref$(/*(_$OT: O(T))(_$o: O(P(T)))*/... /*(_$OT)*/) \
    __step__O_deref$(__step__O_deref$__parseOT __VA_ARGS__)
#define O_deref(_$o /*: O(P(T))*/... /*(O(T))*/) \
    __step__O_deref(_$o)

/* Returns optional value */
#define return_some(_$val...) __return_some(_$val)
#define return_some_void(_$Expr...) pp_overload(__return_some_void, _$Expr)(_$Expr)
#define return_none(_$Expr...) pp_overload(__return_none, _$Expr)(_$Expr)

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_(/*(_$Expr: O(_$T))(_$DefaultExpr_OR_Body...: _$T|void)*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__orelse_)(__param_expand__orelse_ __VA_ARGS__))
#define orelse_ok$(/*(_$E_T)(_$Expr: O(_$T))(_$ok)*/... /*(_$E_T)*/) \
    __step__orelse_ok$(__VA_ARGS__)
#define orelse_ok(/*(_$Expr: O(_$T))(_$ok)*/... /*(E(_$T))*/) \
    __step__orelse_ok(__VA_ARGS__)
#define orelse_err$(/*(_$E_T)(_$Expr: O(_$T))(_$err)*/... /*(_$E_T)*/) \
    __step__orelse_err$(__VA_ARGS__)
#define orelse_err(/*(_$Expr: O(_$T))(_$err)*/... /*(E(_$T))*/) \
    __step__orelse_err(__VA_ARGS__)
#define unwrap_(/*(_$Expr: O(_$T))*/... /*(_$T)*/) \
    ____unwrap(__VA_ARGS__)

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(/*(_$Expr)(_$capture)*/...) __if_some__step(pp_defer(__if_some__emit)(__if_some__parseExpr __VA_ARGS__))
#define if_some_void(/*(_$Expr)*/...) __if_some_void__step(pp_defer(__if_some_void__emit)(__if_some_void__parseExpr __VA_ARGS__))
#define else_none comp_syn__else_none
#define if_none(_$val_opt...) comp_syn__if_none(_$val_opt)
#define else_some(_$Payload_Capture...) comp_syn__else_some(_$Payload_Capture)
#define else_some_void comp_syn__else_some_void
#define while_some(_$val_opt, _$Payload_Capture...) comp_syn__while_some(_$val_opt, _$Payload_Capture)
#define while_none(_$val_opt...) comp_syn__while_none(_$val_opt)

/*========== Macros and Definitions =========================================*/

#define __param_expand__some$(...) __VA_ARGS__,
#define __block_inline__some$(...) __block_inline1__some$(__VA_ARGS__)
#define __block_inline1__some$(_$T, _$val...) l$((_$T)some(_$val))

#define __param_expand__none$(...) __VA_ARGS__
#define __block_inline__none$(...) __block_inline1__none$(__VA_ARGS__)
#define __block_inline1__none$(_$T...) l$((_$T)none())

#define __step__O_asP$(...) \
    __step__O_asP$__emit(__step__O_asP$__parseOPT __VA_ARGS__)
#define __step__O_asP$__parseOPT(_$OPT...) \
    _$OPT, __step__O_asP$__parsePO
#define __step__O_asP$__parsePO(_$p_o...) \
    pp_uniqTok(p_o), _$p_o
#define __step__O_asP$__emit(...) __O_asP$(__VA_ARGS__)
#define __O_asP$(_$OPT, __p_o, _$p_o...) ({ \
    typedef _$OPT O$Ret$O_asP; \
    let_(__p_o, TypeOf(_$p_o)) = _$p_o; \
    claim_assert_nonnull(__p_o); \
    __p_o->is_some \
        ? some$((O$Ret$O_asP)(&__p_o->payload.some)) \
        : none$((O$Ret$O_asP)); \
})
#define __step__O_asP(_$p_o...) \
    O_asP$((O$$(FieldType$(TypeOf(*_$p_o), payload.some)*))(_$p_o))

#define __step__O_deref$(...) __step__O_deref$__emit(__VA_ARGS__)
#define __step__O_deref$__parseOT(_$OT...) \
    _$OT, __step__O_deref$__parseO()
#define __step__O_deref$__parseO() \
    pp_uniqTok(o),
#define __step__O_deref$__emit(_$OT, __o, _$o...) ({ \
    typedef _$OT O$Ret$O_deref; \
    let_(__o, TypeOf(_$o)) = _$o; \
    __o.is_some \
        ? l$((O$Ret$O_deref)some(*unwrap_(__o))) \
        : l$((O$Ret$O_deref)none()); \
})
#define __step__O_deref(_$o...) \
    O_deref$((O$$(P_DerefTUnqual$(O_InnerT$(TypeOf(_$o)))))(_$o))

#define __return_some(_$val...) return_(some(_$val))
#define __return_some_void_0() (return_(some({})))
#define __return_some_void_1(_$Expr...) (_$Expr, return_(some({})))
#define __return_none_0() (return_(none()))
#define __return_none_1(_$Expr...) (_$Expr, return_(none()))

#define __param_expand__orelse_(...) __VA_ARGS__, pp_expand
#define __block_inline__orelse_(_$Expr, _$DefaultExpr_OR_Body...) __block_inline1__orelse_(pp_uniqTok(result), _$Expr, ({ _$DefaultExpr_OR_Body; }))
#define __block_inline1__orelse_(__result, _$Expr, _$DefaultExpr_OR_Body...) $suppress_compound_token_split_by_macro( \
    ({ \
        var __result = _$Expr; \
        if (isNone(__result)) { \
            __result.payload.some = T_switch$((TypeOf(_$DefaultExpr_OR_Body))( \
                T_case$((void)({ \
                    $ignore_void _$DefaultExpr_OR_Body; \
                    l0$((TypeOf(__result.payload.some))); \
                })), \
                T_default_(_$DefaultExpr_OR_Body) \
            )); \
        } \
        __result.payload.some; \
    }) \
)
#define __step__orelse_ok$(...) __step__orelse_ok$__emit(__step__orelse_ok$__parseET __VA_ARGS__)
#define __step__orelse_ok$__parseET(_$E_T...) _$E_T, __step__orelse_ok$__parseExpr
#define __step__orelse_ok$__parseExpr(_$expr...) pp_uniqTok(expr), (_$expr),
#define __step__orelse_ok$__emit(...) ____orelse_ok$(__VA_ARGS__)
#define ____orelse_ok$(_$E_T, __expr, _$expr, _$ok...) ({ \
    typedef _$E_T E$Ret$orelse_ok$; \
    let __expr = _$expr; \
    __expr.is_some \
        ? l$((E$Ret$orelse_ok$)ok(unwrap_(__expr))) \
        : l$((E$Ret$orelse_ok$)ok(_$ok)); \
})
#define __step__orelse_ok(...) __step__orelse_ok__emit(__step__orelse_ok__parseExpr __VA_ARGS__)
#define __step__orelse_ok__parseExpr(_$expr...) _$expr,
#define __step__orelse_ok__emit(...) ____orelse_ok(__VA_ARGS__)
#define ____orelse_ok(_$expr, _$ok...) \
    orelse_ok$((E$$(O_InnerT$(TypeOfUnqual(_$expr))))(_$expr)_$ok)
#define __step__orelse_err$(...) __step__orelse_err$__emit(__step__orelse_err$__parseET __VA_ARGS__)
#define __step__orelse_err$__parseET(_$E_T...) _$E_T, __step__orelse_err$__parseExpr
#define __step__orelse_err$__parseExpr(_$expr...) pp_uniqTok(expr), (_$expr),
#define __step__orelse_err$__emit(...) ____orelse_err$(__VA_ARGS__)
#define ____orelse_err$(_$E_T, __expr, _$expr, _$err...) ({ \
    typedef _$E_T E$Ret$orelse_err$; \
    let __expr = _$expr; \
    __expr.is_some \
        ? l$((E$Ret$orelse_err$)ok(unwrap_(__expr))) \
        : l$((E$Ret$orelse_err$)err(_$err)); \
})
#define __step__orelse_err(...) __step__orelse_err__emit(__step__orelse_err__parseExpr __VA_ARGS__)
#define __step__orelse_err__parseExpr(_$expr...) _$expr,
#define __step__orelse_err__emit(...) ____orelse_err(__VA_ARGS__)
#define ____orelse_err(_$expr, _$err...) \
    orelse_err$((E$$(O_InnerT$(TypeOfUnqual(_$expr))))(_$expr)_$err)
#define ____unwrap(_$Expr...) orelse_((_$Expr)(claim_unreachable))

#define __if_some__step(...) __VA_ARGS__
#define __if_some__parseExpr(_$Expr...) (_$Expr), __if_some__parseCapture
#define __if_some__parseCapture(_$capture...) _$capture
#define __if_some__emit(_$Expr, _$capture...) \
    if_(let _result = _$Expr, _result.is_some) \
        using_(let _$capture = _result.payload.some)
#define __if_some_void__step(...) __VA_ARGS__
#define __if_some_void__parseExpr(_$Expr...) (_$Expr), __if_some_void__parseCapture
#define __if_some_void__parseCapture(_$capture...) _$capture
#define __if_some_void__emit(_$Expr, _$capture...) \
    if_(let _result = _$Expr, _result.is_some)
#define comp_syn__else_none \
    else
#define comp_syn__if_none(_$val_opt...) \
    if_(let _result = (_$val_opt), !_result.is_some)
#define comp_syn__else_some(_$Payload_Capture...) \
    else_(let _$Payload_Capture = _result.payload.some)
#define comp_syn__else_some_void \
    else
#define comp_syn__while_some(_$val_opt, _$Payload_Capture...) \
    while_(var _result = (_$val_opt), _result.is_some, _result = (_$val_opt)) \
        using_(let _$Payload_Capture = _result.payload.some)
#define comp_syn__while_none(_$val_opt...) \
    while_(var _result = (_$val_opt), !_result.is_some, _result = (_$val_opt))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Opt__included */
