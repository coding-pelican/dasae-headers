#ifndef types_Opt__included
#define types_Opt__included 1
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
// #define __asg$O__emit(__p_o, _p_o, _v_o...) blk({ \
//     let_(__p_o, TypeOf(_p_o)) = _p_o; \
//     claim_assert_nonnull(__p_o); \
//     *__p_o = lit$((TypeOf(*__p_o))_v_o); \
//     blk_return __p_o; \
// })

#define asgO(_p_o, _v_o...)  asgO1(_p_o, _v_o)
#define asgO1(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some))
#define asgO2(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some))
#define asgO3(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some.payload.some))
#define asgO4(_p_o, _v_o...) asg(_p_o, _v_o, (payload.some.payload.some.payload.some.payload.some))

/* Determines optional value */
#define some(_val...)              comp_inline__some(_val)
#define comp_inline__some(_val...) { \
    .is_some = true, \
    .payload = { .some = _val }, \
}
#define some$(/*(_T)(_val: _T))*/... /*(O$(_T))*/) \
    pp_expand(pp_defer(__block_inline__some$)(__param_expand__some$ __VA_ARGS__))
#define some$$(/*(_T)(_val: _T))*/... /*(O$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__some$$)(__param_expand__some$$ __VA_ARGS__))

#define none()              comp_inline__none()
#define comp_inline__none() { \
    .is_some = false, \
    .payload = { .none = {} }, \
}
#define none$(/*(_T)(none())*/... /*(O$(_T))*/)   pp_expand(pp_defer(__block_inline__none$)(__param_expand__none$ __VA_ARGS__))
#define none$$(/*(_T)(none())*/... /*(O$$(_T))*/) pp_expand(pp_defer(__block_inline__none$$)(__param_expand__none$$ __VA_ARGS__))

/* Checks optional value */
#define isSome(_o /*: O$$(_T)*/... /*(bool)*/) as$((bool)((_o).is_some))
#define isNone(_o /*: O$$(_T)*/... /*(bool)*/) as$((bool)(!(_o).is_some))

#define O_asP$(/*(_OPT: O(P(T)))(_p_o: P(O(T)))*/... /*(_OPT)*/) \
    __step__O_asP$(__step__O_asP$__parseOPT __VA_ARGS__)
#define O_asP(_p_o /*: P(O(T))*/... /*(O(P(T)))*/) \
    __step__O_asP(_p_o)

/* Returns optional value */
#define return_some(_val...)       __return_some(_val)
#define return_some_void(_Expr...) pp_overload(__return_some_void, _Expr)(_Expr)
#define return_none(_Expr...)      pp_overload(__return_none, _Expr)(_Expr)

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_(/*(_Expr: O$$(_T))(_DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__orelse_)(__param_expand__orelse_ __VA_ARGS__))
#define unwrap_(/*(_Expr: O$$(_T))*/... /*(_T)*/) \
    __unwrap(__VA_ARGS__)

/* Optional value payload capture (similar to Zig's if/while captures) */
#define if_some(/*(_Expr)(_capture)*/...)        __if_some__step(pp_defer(__if_some__emit)(__if_some__parseExpr __VA_ARGS__))
#define if_some_void(/*(_Expr)*/...)             __if_some_void__step(pp_defer(__if_some_void__emit)(__if_some_void__parseExpr __VA_ARGS__))
#define else_none                                comp_syn__else_none
#define if_none(val_opt...)                      comp_syn__if_none(val_opt)
#define else_some(_Payload_Capture...)           comp_syn__else_some(_Payload_Capture)
#define else_some_void                           comp_syn__else_some_void
#define while_some(val_opt, _Payload_Capture...) comp_syn__while_some(val_opt, _Payload_Capture)
#define while_none(val_opt...)                   comp_syn__while_none(val_opt)

/*========== Macros and Definitions =========================================*/

#define __param_expand__some$(...)          __VA_ARGS__, pp_expand
#define __block_inline__some$(...)          __block_inline1__some$(__VA_ARGS__)
#define __block_inline1__some$(_T, _val...) lit$((O$(_T))some(_val))

#define __param_expand__some$$(...)           __VA_ARGS__, pp_expand
#define __block_inline__some$$(...)           __block_inline1__some$$$(__VA_ARGS__)
#define __block_inline1__some$$$(_T, _val...) lit$((O$$(_T))some(_val))

#define __param_expand__none$(...)          __VA_ARGS__, pp_expand
#define __block_inline__none$(...)          __block_inline1__none$(__VA_ARGS__)
#define __block_inline1__none$(_T, _val...) lit$((O$(_T))none())

#define __param_expand__none$$(...)           __VA_ARGS__, pp_expand
#define __block_inline__none$$(...)           __block_inline1__none$$$(__VA_ARGS__)
#define __block_inline1__none$$$(_T, _val...) lit$((O$$(_T))none())

#define __step__O_asP$(...) \
    __step__O_asP$__emit(__VA_ARGS__)
#define __step__O_asP$__parseOPT(_OPT...) \
    _OPT, __step__O_asP$__parsePO
#define __step__O_asP$__parsePO(_p_o...) \
    pp_uniqTok(p_o), _p_o
#define __step__O_asP$__emit(_OPT, __p_o, _p_o...) ({ \
    typedef _OPT O$Ret$O_asP; \
    let_(__p_o, TypeOf(_p_o)) = _p_o; \
    claim_assert_nonnull(__p_o); \
    __p_o->is_some \
        ? lit$((O$Ret$O_asP)some(&__p_o->payload.some)) \
        : lit$((O$Ret$O_asP)none()); \
})
#define __step__O_asP(_p_o...) \
    O_asP$((O$$(FieldType$(TypeOf(*_p_o), payload.some)*))(_p_o))

#define __return_some(_val...)         return_(some(_val))
#define __return_some_void_0()         (return_(some({})))
#define __return_some_void_1(_Expr...) (_Expr, return_(some({})))
#define __return_none_0()              (return_(none()))
#define __return_none_1(_Expr...)      (_Expr, return_(none()))

#define __param_expand__orelse_(...)                                       __VA_ARGS__, pp_expand
#define __block_inline__orelse_(_Expr, _DefaultExpr_OR_Body...)            __block_inline1__orelse_(pp_uniqTok(result), _Expr, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__orelse_(__result, _Expr, _DefaultExpr_OR_Body...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isNone(__result)) { \
            __result.payload.some = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    $ignore_void _DefaultExpr_OR_Body; \
                    lit$((TypeOf(__result.payload.some)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
        } \
        __result.payload.some; \
    }) \
)
#define __unwrap(_Expr...) orelse_((_Expr)(claim_unreachable))

#define __if_some__step(...)                 __VA_ARGS__
#define __if_some__parseExpr(_Expr...)       (_Expr), __if_some__parseCapture
#define __if_some__parseCapture(_capture...) _capture
#define __if_some__emit(_Expr, _capture...) \
    if_(let _result = _Expr, _result.is_some) \
        with_(let _capture = _result.payload.some)
#define __if_some_void__step(...)                 __VA_ARGS__
#define __if_some_void__parseExpr(_Expr...)       (_Expr), __if_some_void__parseCapture
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
    with_(let _Payload_Capture = _result.payload.some)
#define comp_syn__while_none(val_opt...) \
    while_(var _result = (val_opt), !_result.is_some)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* types_Opt__included */
