#ifndef prl_types_Opt__included
#define prl_types_Opt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"

/*========== Macros and Declarations ========================================*/

/* Optional Anonymous */
#define O$$(_T...) \
    TypeOf(union { \
        struct { \
            var_(is_some, bool); \
            union { \
                var_(none, Void); \
                var_(some, _T); \
            } payload; \
        }; \
        var_(as_raw $like_ref, O$raw); \
    })
/* Optional Alias */
#define O$(_T...) pp_join($, O, _T)
/* Optional Template */
#define T_decl_O$(_T...) \
    $maybe_unused typedef union O$(_T) O$(_T)
#define T_impl_O$(_T...) \
    union O$(_T) { \
        struct { \
            var_(is_some, bool); \
            union { \
                var_(none, Void); \
                var_(some, _T); \
            } payload; \
        }; \
        var_(as_raw $like_ref, O$raw); \
    }
#define T_use_O$(_T...) \
    T_decl_O$(_T); \
    T_impl_O$(_T)

#define O_InnerT$(_T...) TypeOf((as$(_T*)(null))->payload.some)

/* Optional void value (special case) */
typedef union O$Void {
    struct {
        var_(is_some, bool);
        union {
            var_(none, Void);
            var_(some, Void);
        } payload;
    };
    var_(as_raw $like_ref, O$raw);
} O$Void, O$void;

/* Optional Operations */
// #define init$O()
// #define init$O$()
// #define asg$O()
// #define asg$O$()

// #define asg$O(/*(_p_o: O$(_T))(_v_o: _T|void)*/... /*(_p_o: O$(_T))*/) \
//     __asg$O__step(pp_defer(__asg$O__emit)(__asg$O__parseLhs __VA_ARGS__))
// #define __asg$O__step(...)                  __VA_ARGS__
// #define __asg$O__parseLhs(_p_o...)          pp_uniqTok(p_o), _p_o, __asg$O__parseRhs
// #define __asg$O__parseRhs(_v_o...)          _v_o
// #define __asg$O__emit(__p_o, _p_o, _v_o...) local_({ \
//     let_(__p_o, TypeOf(_p_o)) = _p_o; \
//     claim_assert_nonnull(__p_o); \
//     *__p_o = l$((TypeOf(*__p_o))_v_o); \
//     local__return __p_o; \
// })

#define asgO(_p_o, _v_o...) asgO1(_p_o, _v_o)
#define asgO1(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some))
#define asgO2(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some))
#define asgO3(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some.payload.some))
#define asgO4(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some.payload.some.payload.some))

/* Determines optional value */
#define some(_val...) comp_inline__some(_val)
#define comp_inline__some(_val...) { \
    .is_some = true, \
    .payload = { .some = _val }, \
}
#define some$(/*(_T)_val: _T)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__some$)(__param_expand__some$ __VA_ARGS__))

#define none() comp_inline__none()
#define comp_inline__none() { \
    .is_some = false, \
    .payload = { .none = {} }, \
}
#define none$(/*(_T)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__none$)(__param_expand__none$ __VA_ARGS__))

/* Checks optional value */
#define isSome(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)((_o).is_some)
#define isNone(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)(!(_o).is_some)

#define O_asP$(/*(_OPT: O(P(T)))(_p_o: P(O(T)))*/... /*(_OPT)*/) \
    __step__O_asP$(__VA_ARGS__)
#define O_asP(_p_o /*: P(O(T))*/... /*(O(P(T)))*/) \
    __step__O_asP(_p_o)
#define O_ref$(/*(_OPT: O(P(T)))(_p_o: P(O(T)))*/... /*(_OPT)*/) \
    __step__O_asP$(__VA_ARGS__)
#define O_ref(_p_o /*: P(O(T))*/... /*(O(P(T)))*/) \
    __step__O_asP(_p_o)
#define O_deref$(/*(_OT: O(T))(_o: O(P(T)))*/... /*(_OT)*/) \
    __step__O_deref$(__step__O_deref$__parseOT __VA_ARGS__)
#define O_deref(_o /*: O(P(T))*/... /*(O(T))*/) \
    __step__O_deref(_o)

/* Returns optional value */
#define return_some(_val...) __return_some(_val)
#define return_some_void(_Expr...) pp_overload(__return_some_void, _Expr)(_Expr)
#define return_none(_Expr...) pp_overload(__return_none, _Expr)(_Expr)

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_(/*(_Expr: O(_T))(_DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__orelse_)(__param_expand__orelse_ __VA_ARGS__))
#define orelse_ok$(/*(_E_T)(_Expr: O(_T))(_ok)*/... /*(_E_T)*/) \
    __step__orelse_ok$(__VA_ARGS__)
#define orelse_ok(/*(_Expr: O(_T))(_ok)*/... /*(E(_T))*/) \
    __step__orelse_ok(__VA_ARGS__)
#define orelse_err$(/*(_E_T)(_Expr: O(_T))(_err)*/... /*(_E_T)*/) \
    __step__orelse_err$(__VA_ARGS__)
#define orelse_err(/*(_Expr: O(_T))(_err)*/... /*(E(_T))*/) \
    __step__orelse_err(__VA_ARGS__)
#define unwrap_(/*(_Expr: O(_T))*/... /*(_T)*/) \
    ____unwrap(__VA_ARGS__)

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(/*(_Expr)(_capture)*/...) __if_some__step(pp_defer(__if_some__emit)(__if_some__parseExpr __VA_ARGS__))
#define if_some_void(/*(_Expr)*/...) __if_some_void__step(pp_defer(__if_some_void__emit)(__if_some_void__parseExpr __VA_ARGS__))
#define else_none comp_syn__else_none
#define if_none(val_opt...) comp_syn__if_none(val_opt)
#define else_some(_Payload_Capture...) comp_syn__else_some(_Payload_Capture)
#define else_some_void comp_syn__else_some_void
#define while_some(val_opt, _Payload_Capture...) comp_syn__while_some(val_opt, _Payload_Capture)
#define while_none(val_opt...) comp_syn__while_none(val_opt)

/*========== Macros and Definitions =========================================*/

#define __param_expand__some$(...) __VA_ARGS__,
#define __block_inline__some$(...) __block_inline1__some$(__VA_ARGS__)
#define __block_inline1__some$(_T, _val...) l$((_T)some(_val))

#define __param_expand__none$(...) __VA_ARGS__
#define __block_inline__none$(...) __block_inline1__none$(__VA_ARGS__)
#define __block_inline1__none$(_T...) l$((_T)none())

#define __step__O_asP$(...) \
    __step__O_asP$__emit(__step__O_asP$__parseOPT __VA_ARGS__)
#define __step__O_asP$__parseOPT(_OPT...) \
    _OPT, __step__O_asP$__parsePO
#define __step__O_asP$__parsePO(_p_o...) \
    pp_uniqTok(p_o), _p_o
#define __step__O_asP$__emit(...) __O_asP$(__VA_ARGS__)
#define __O_asP$(_OPT, __p_o, _p_o...) ({ \
    typedef _OPT O$Ret$O_asP; \
    let_(__p_o, TypeOf(_p_o)) = _p_o; \
    claim_assert_nonnull(__p_o); \
    __p_o->is_some \
        ? l$((O$Ret$O_asP)some(&__p_o->payload.some)) \
        : l$((O$Ret$O_asP)none()); \
})
#define __step__O_asP(_p_o...) \
    O_asP$((O$$(FieldType$(TypeOf(*_p_o), payload.some)*))(_p_o))

#define __step__O_deref$(...) __step__O_deref$__emit(__VA_ARGS__)
#define __step__O_deref$__parseOT(_OT...) \
    _OT, __step__O_deref$__parseO()
#define __step__O_deref$__parseO() \
    pp_uniqTok(o),
#define __step__O_deref$__emit(_OT, __o, _o...) ({ \
    typedef _OT O$Ret$O_deref; \
    let_(__o, TypeOf(_o)) = _o; \
    __o.is_some \
        ? l$((O$Ret$O_deref)some(*unwrap_(__o))) \
        : l$((O$Ret$O_deref)none()); \
})
#define __step__O_deref(_o...) \
    O_deref$((O$$(P_DerefTUnqual$(O_InnerT$(TypeOf(_o)))))(_o))

#define __return_some(_val...) return_(some(_val))
#define __return_some_void_0() (return_(some({})))
#define __return_some_void_1(_Expr...) (_Expr, return_(some({})))
#define __return_none_0() (return_(none()))
#define __return_none_1(_Expr...) (_Expr, return_(none()))

#define __param_expand__orelse_(...) __VA_ARGS__, pp_expand
#define __block_inline__orelse_(_Expr, _DefaultExpr_OR_Body...) __block_inline1__orelse_(pp_uniqTok(result), _Expr, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__orelse_(__result, _Expr, _DefaultExpr_OR_Body...) $supress_compound_token_split_by_macro( \
    ({ \
        var __result = _Expr; \
        if (isNone(__result)) { \
            __result.payload.some = T_switch$((TypeOf(_DefaultExpr_OR_Body))( \
                T_case$((void)({ \
                    $ignore_void _DefaultExpr_OR_Body; \
                    l0$((TypeOf(__result.payload.some))); \
                })), \
                T_default_(_DefaultExpr_OR_Body) \
            )); \
        } \
        __result.payload.some; \
    }) \
)
#define __step__orelse_ok$(...) __step__orelse_ok$__emit(__step__orelse_ok$__parseET __VA_ARGS__)
#define __step__orelse_ok$__parseET(_E_T...) _E_T, __step__orelse_ok$__parseExpr
#define __step__orelse_ok$__parseExpr(_expr...) pp_uniqTok(expr), (_expr),
#define __step__orelse_ok$__emit(...) ____orelse_ok$(__VA_ARGS__)
#define ____orelse_ok$(_E_T, __expr, _expr, _ok...) ({ \
    typedef _E_T E$Ret$orelse_ok$; \
    let __expr = _expr; \
    __expr.is_some \
        ? l$((E$Ret$orelse_ok$)ok(unwrap_(__expr))) \
        : l$((E$Ret$orelse_ok$)ok(_ok)); \
})
#define __step__orelse_ok(...) __step__orelse_ok__emit(__step__orelse_ok__parseExpr __VA_ARGS__)
#define __step__orelse_ok__parseExpr(_expr...) _expr,
#define __step__orelse_ok__emit(...) ____orelse_ok(__VA_ARGS__)
#define ____orelse_ok(_expr, _ok...) \
    orelse_ok$((E$$(O_InnerT$(TypeOfUnqual(_expr))))(_expr)_ok)
#define __step__orelse_err$(...) __step__orelse_err$__emit(__step__orelse_err$__parseET __VA_ARGS__)
#define __step__orelse_err$__parseET(_E_T...) _E_T, __step__orelse_err$__parseExpr
#define __step__orelse_err$__parseExpr(_expr...) pp_uniqTok(expr), (_expr),
#define __step__orelse_err$__emit(...) ____orelse_err$(__VA_ARGS__)
#define ____orelse_err$(_E_T, __expr, _expr, _err...) ({ \
    typedef _E_T E$Ret$orelse_err$; \
    let __expr = _expr; \
    __expr.is_some \
        ? l$((E$Ret$orelse_err$)ok(unwrap_(__expr))) \
        : l$((E$Ret$orelse_err$)err(_err)); \
})
#define __step__orelse_err(...) __step__orelse_err__emit(__step__orelse_err__parseExpr __VA_ARGS__)
#define __step__orelse_err__parseExpr(_expr...) _expr,
#define __step__orelse_err__emit(...) ____orelse_err(__VA_ARGS__)
#define ____orelse_err(_expr, _err...) \
    orelse_err$((E$$(O_InnerT$(TypeOfUnqual(_expr))))(_expr)_err)
#define ____unwrap(_Expr...) orelse_((_Expr)(claim_unreachable))

#define __if_some__step(...) __VA_ARGS__
#define __if_some__parseExpr(_Expr...) (_Expr), __if_some__parseCapture
#define __if_some__parseCapture(_capture...) _capture
#define __if_some__emit(_Expr, _capture...) \
    if_(let _result = _Expr, _result.is_some) \
        using_(let _capture = _result.payload.some)
#define __if_some_void__step(...) __VA_ARGS__
#define __if_some_void__parseExpr(_Expr...) (_Expr), __if_some_void__parseCapture
#define __if_some_void__parseCapture(_capture...) _capture
#define __if_some_void__emit(_Expr, _capture...) \
    if_(let _result = _Expr, _result.is_some)
#define comp_syn__else_none \
    else
#define comp_syn__if_none(val_opt...) \
    if_(let _result = (val_opt), !_result.is_some)
#define comp_syn__else_some(_Payload_Capture...) \
    else_(let _Payload_Capture = _result.payload.some)
#define comp_syn__else_some_void \
    else
#define comp_syn__while_some(val_opt, _Payload_Capture...) \
    for (var _result = (val_opt); _result.is_some; _result = (val_opt)) \
    using_(let _Payload_Capture = _result.payload.some)
#define comp_syn__while_none(val_opt...) \
    while_(var _result = (val_opt), !_result.is_some)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_Opt__included */
