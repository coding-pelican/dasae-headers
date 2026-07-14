#pragma once
#ifndef prl_tpl_E_ErrRes__included
#define prl_tpl_E_ErrRes__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define $set $_set,
/* Result Anonymous */
#define E$$(...) pp_overload(__E$$, __VA_ARGS__)(__VA_ARGS__)
#define __E$$_1(_$T...) TypeOf(union { \
    T_embed$(struct { \
        var_(is_ok, bool); \
        var_(payload, union { \
            var_(err, EAny); \
            var_(ok, _$T); \
        }); \
    }); \
    var_(as_raw $like_ref, E$raw); \
})
#define __E$$_2(_$tag, _$ESet_T...) pp_join(_, __E$$, _$tag)(__E$$__expandESet _$ESet_T)
#define __E$$__expandESet(_$ESet...) _$ESet, __E$$__expandT
#define __E$$__expandT(_$T...) _$T
#define __E$$_$_set(...) __E$$__withESet(__VA_ARGS__)
#define __E$$__withESet(_$E, _$T...) TypeOf(union { \
    T_embed$(struct { \
        var_(is_ok, bool); \
        var_(payload, union { \
            var_(err, _$E); \
            var_(ok, _$T); \
        }); \
    }); \
    var_(as_raw $like_ref, E$raw); \
})
/* Result Alias */
#define E$(...) pp_overload(__E$, __VA_ARGS__)(__VA_ARGS__)
#define __E$_1(_$T...) pp_join($, E, _$T)
#define __E$_2(_$tag, _$ESet_T...) pp_join(_, __E$, _$tag)(__E$__expandESet _$ESet_T)
#define __E$__expandESet(_$ESet...) _$ESet, __E$__expandT
#define __E$__expandT(_$T...) _$T
#define __E$_$_set(...) __E$__withESet(__VA_ARGS__)
#define __E$__withESet(_$E, _$T...) pp_join($, _$E, _$T)
/* Result Template */
#define T_decl_E$(_$T...) \
    $maybe_unused typedef union E$(_$T) E$(_$T)
#define T_impl_E$(...) pp_overload(__T_impl_E$, __VA_ARGS__)(__VA_ARGS__)
#define __T_impl_E$_1(_$T...) \
    union E$(_$T) { \
        T_embed$(struct { \
            var_(is_ok, bool); \
            var_(payload, union { \
                var_(err, EAny); \
                var_(ok, _$T); \
            }); \
        }); \
        var_(as_raw $like_ref, E$raw); \
    }
#define __T_impl_E$_2(_$tag, _$ESet_T...) \
    pp_join(_, __T_impl_E$, _$tag)(__T_impl_E$__expandESet _$ESet_T)
#define __T_impl_E$__expandESet(_$ESet...) _$ESet, __T_impl_E$__expandT
#define __T_impl_E$__expandT(_$T...) _$T
#define __T_impl_E$_$_set(...) __T_impl_E$__withESet(__VA_ARGS__)
#define __T_impl_E$__withESet(_$E, _$T...) \
    union E$($set(_$E)(_$T)) { \
        T_embed$(struct { \
            var_(is_ok, bool); \
            var_(payload, union { \
                var_(err, _$E); \
                var_(ok, _$T); \
            }); \
        }); \
        var_(as_raw $like_ref, E$raw); \
    }
#define T_use_E$(_$T...) \
    T_decl_E$(_$T); \
    T_impl_E$(_$T)

#define E_T$(_$T...) TypeOf((as$(_$T*)(null))->payload.ok)
#define E_E$(_$T...) TypeOf((as$(_$T*)(null))->payload.err)

/* Error void result (special case) */
typedef union E$Void E$Void, E$void;
// typedef union E$Void {
//     T_embed$(struct {
//         var_(is_ok, bool);
//         var_(payload, union {
//             var_(err, EAny);
//             var_(ok, Void);
//         });
//     });
//     var_(as_raw $like_ref, E$raw);
// } E$Void, E$void;

/* Error Operations */
// #define init$E()
// #define init$E$()
// #define asg$E()
// #define asg$E$()

#define asgE(_$p_e, _$v_e...) asgE1(_$p_e, _$v_e)
#define asgE1(_$p_e, _$v_e...) asg(_$p_e, _$v_e, (payload.ok))
#define asgE2(_$p_e, _$v_e...) asg(_$p_e, _$v_e, (payload.ok.payload.ok))
#define asgE3(_$p_e, _$v_e...) asg(_$p_e, _$v_e, (payload.ok.payload.ok.payload.ok))
#define asgE4(_$p_e, _$v_e...) asg(_$p_e, _$v_e, (payload.ok.payload.ok.payload.ok.payload.ok))

/* Determines error result */
#define ok(_$val...) { \
    .is_ok = true, \
    .payload = { .ok = _$val }, \
}
#define ok$(/*(_$T)(_$val: _$T))*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__ok$)(__param_expand__ok$ __VA_ARGS__))

#define err(_$val...) { \
    .is_ok = false, \
    .payload = { .err = _$val }, \
}
#define err$(/*(_$T)(_$val: _$T))*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__err$)(__param_expand__err$ __VA_ARGS__))

/* Checks error result */
#define isOk(_$e /*: E$$(_$T)*/... /*(bool)*/) bool_((_$e).is_ok)
#define isErr(_$e /*: E$$(_$T)*/... /*(bool)*/) bool_(!(_$e).is_ok)

#define E_asP$(/*(_$E_P_T: E(P(T)))(_$p_e: P(E(T)))*/... /*(_$E_P_T)*/) \
    __step__E_asP$(__VA_ARGS__)
#define E_asP(_$p_e /*: P(E(T))*/... /*(E(P(T)))*/) \
    __step__E_asP(_$p_e)
#define E_ref$(/*(_$E_P_T: E(P(T)))(_$p_e: P(E(T)))*/... /*(_$E_P_T)*/)
#define E_ref(_$p_e /*: P(E(T))*/... /*(E(P(T)))*/)
#define E_deref$(/*(_$Ok: E(T))(_$e: E(P(T)))*/... /*(_$Ok)*/)
#define E_deref(_$e /*: E(P(T))*/... /*(E(T))*/)

/* Returns error result */
#define return_ok(_$val...) \
    (return_(ok(_$val)))
#define return_ok_void(_$Expr...) pp_overload(__return_ok_void, _$Expr)(_$Expr)
#define __expr__E__asAny(_$p_err...) \
    (*((const EAny*)(_$p_err)))
#define __expr__E__asGeneral(_$p_err...) \
    (*((const General_E*)(_$p_err)))
#define __expr__E__reinterpretAs(_$TargetErrExpr, _$p_err...) \
    (*((const TypeOf(_$TargetErrExpr)*)(_$p_err)))
#define __expr__E__castForReturn(_$TargetErrExpr, _$val...) ({ \
    let __err = (_$val); \
    _Generic((_$TargetErrExpr), EAny: __expr__E__asAny(&__err), General_E: __expr__E__asGeneral(&__err), default: __expr__E__reinterpretAs(_$TargetErrExpr, &__err)); \
})
#define return_err(_$val...) ( \
    $debug_point ETrace_captureFrame(), \
    return_(err(__expr__E__castForReturn( \
        ((TypeOf(reservedReturn())){}).payload.err, \
        _$val \
    ))) \
)

/* Propagates error (similar to Zig's try_) */
#define try_(/*(_$Expr: E(_$T))*/... /*(_$T)*/) \
    __step__try_(__VA_ARGS__)
/* Handles error (similar to Zig's catch_from) */
#define catch_(/*(_$Expr: E(_$T))(_$Capture|$ignore, _$DefaultExpr_OR_Body...: _$T|void)*/... /*(_$T)*/) \
    pp_expand(pp_defer(__block_inline__catch_)(__param_expand__catch_ __VA_ARGS__))
#define catch_some$(/*(_$O_T)(_$Expr: E(_$T))(_$some)*/... /*(_$O_T)*/) \
    __step__catch_some$(__VA_ARGS__)
#define catch_some(/*(_$Expr: E(_$T))(_$some)*/... /*(O(_$T))*/) \
    __step__catch_some(__VA_ARGS__)
#define catch_none$(/*(_$O_T)(_$Expr: E(_$T))*/... /*(_$O_T)*/) \
    __step__catch_none$(__VA_ARGS__)
#define catch_none(/*(_$Expr: E(_$T))*/... /*(O(_$T))*/) \
    __step__catch_none(__VA_ARGS__)
/* Defers when error */
#define errdefer_(/*(_$O_Capture|$ignore, _$Expr_OR_Body...:void)*/... /*(void)*/) \
    ____errdefer_(__VA_ARGS__)

/* Error result payload captures */
#define if_ok(/*(_$Expr)(_$capture)*/...) __if_ok__step(pp_defer(__if_ok__emit)(__if_ok__parseExpr __VA_ARGS__))
#define if_ok_void(_$val_result) comp_syn__if_ok_void(_$val_result)
#define else_err(_$Payload_Capture) comp_syn__else_err(_$Payload_Capture)
#define if_err(/*(_$Expr)(_$capture)*/...) __if_err__step(pp_defer(__if_err__emit)(__if_err__parseExpr __VA_ARGS__))
#define else_ok(_$Payload_Capture) comp_syn__else_ok(_$Payload_Capture)
#define else_ok_void comp_syn__else_ok_void

/*========== Macros and Definitions =========================================*/

#define __param_expand__ok$(...) __VA_ARGS__,
#define __block_inline__ok$(...) __block_inline1__ok$(__VA_ARGS__)
#define __block_inline1__ok$(_$T, _$val...) l$((_$T)ok(_$val))

#define __param_expand__err$(...) __VA_ARGS__,
#define __block_inline__err$(...) __block_inline1__err$(__VA_ARGS__)
#define __block_inline1__err$(_$T, _$val...) l$((_$T)err((E_InnerE$(_$T))_$val))

#define __step__E_asP$(...) \
    __step__E_asP$__emit(__step__E_asP$__parseEPT __VA_ARGS__)
#define __step__E_asP$__parseEPT(_$EPT...) \
    _$EPT, __step__E_asP$__parsePE
#define __step__E_asP$__parsePE(_$p_e...) \
    pp_uniqTok(p_e), _$p_e
#define __step__E_asP$__emit(...) __E_asP$(__VA_ARGS__)
#define __E_asP$(_$EPT, __p_e, _$p_e...) ({ \
    typedef _$EPT E$Ret$E_asP; \
    let_(__p_e, TypeOf(_$p_e)) = _$p_e; \
    claim_assert_nonnull(__p_e); \
    __p_e->is_ok \
        ? ok$((E$Ret$E_asP)(&__p_e->payload.ok)) \
        : err$((E$Ret$E_asP)(__p_e->payload.err)); \
})
#define __step__E_asP(_$p_e...) \
    E_asP$((E$$(FieldType$(TypeOf(*_$p_e), payload.ok)*))(_$p_e))

#define __return_ok_void_0() (return_(ok({})))
#define __return_ok_void_1(_$Expr...) (_$Expr, return_(ok({})))

#define __step__try_(...) ____try_(pp_uniqTok(result), __VA_ARGS__)
#define ____try_(__result, _$Expr...) ({ \
    let __result = _$Expr; \
    if (isErr(__result)) { \
        return_err(__result.payload.err); \
    } \
    __result.payload.ok; \
})
#define __param_expand__catch_(...) __VA_ARGS__, pp_expand
#define __block_inline__catch_(_$Expr, _$Payload_Capture, _$DefaultExpr_OR_Body...) __block_inline1__catch_(pp_uniqTok(result), _$Expr, _$Payload_Capture, ({ _$DefaultExpr_OR_Body; }))
#define __block_inline1__catch_(__result, _$Expr, _$Payload_Capture, _$DefaultExpr_OR_Body...) $suppress_compound_token_split_by_macro( \
    ({ \
        var __result = _$Expr; \
        if (isErr(__result)) { \
            /* claim_assert(__result.payload.err.ctx != 0); */ \
            /* claim_assert_nonnull(__result.payload.err.vt); */ \
            let _$Payload_Capture = __result.payload.err; \
            __result.payload.ok = _Generic( \
                TypeOfUnqual(_$DefaultExpr_OR_Body), \
                void: ({ \
                    _$DefaultExpr_OR_Body; \
                    l0$((TypeOf(__result.payload.ok))); \
                }), \
                default: _$DefaultExpr_OR_Body \
            ); \
            ETrace_reset(); \
        } \
        __result.payload.ok; \
    }) \
)
#define __step__catch_some$(...) __step__catch_some$__emit(__step__catch_some$__parseOT __VA_ARGS__)
#define __step__catch_some$__parseOT(_$O_T...) _$O_T, __step__catch_some$__parseExpr
#define __step__catch_some$__parseExpr(_$expr...) pp_uniqTok(expr), (_$expr),
#define __step__catch_some$__emit(...) ____catch_some$(__VA_ARGS__)
#define ____catch_some$(_$O_T, __expr, _$expr, _$some...) ({ \
    typedef _$O_T O$Ret$catch_some$; \
    let __expr = _$expr; \
    __expr.is_ok \
        ? l$((O$Ret$catch_some$)some(catch_((__expr)($ignore, claim_unreachable)))) \
        : (ETrace_reset(), l$((O$Ret$catch_some$)some(_$some))); \
})
#define __step__catch_some(...) __step__catch_some__emit(__step__catch_some__parseExpr __VA_ARGS__)
#define __step__catch_some__parseExpr(_$expr...) _$expr,
#define __step__catch_some__emit(...) ____catch_some$(__VA_ARGS__)
#define ____catch_some(_$expr, _$some...) \
    catch_some$((O$$(E_T$(TypeOf(_$expr))))(_$expr)_$some)
#define __step__catch_none$(...) __step__catch_none$__emit(__step__catch_none$__parseOT __VA_ARGS__)
#define __step__catch_none$__parseOT(_$O_T...) _$O_T, pp_uniqTok(expr),
#define __step__catch_none$__emit(...) ____catch_none$(__VA_ARGS__)
#define ____catch_none$(_$O_T, __expr, _$expr...) ({ \
    typedef _$O_T O$Ret$catch_none$; \
    let __expr = _$expr; \
    __expr.is_ok \
        ? l$((O$Ret$catch_none$)some(catch_((__expr)($ignore, claim_unreachable)))) \
        : (ETrace_reset(), l$((O$Ret$catch_none$)none())); \
})
#define __step__catch_none(...) ____catch_none(__VA_ARGS__)
#define ____catch_none(_$expr...) \
    catch_none$((O$$(E_T$(TypeOf(_$expr))))(_$expr))
#define ____errdefer_(_$Payload_Capture, _$Expr...) defer_(if (!__reserved_return->is_ok) { \
    /* claim_assert(__reserved_return->payload.err.ctx != 0); */ \
    /* claim_assert_nonnull(__reserved_return->payload.err.vt); */ \
    let _$Payload_Capture = __reserved_return->payload.err; \
    _$Expr; \
})

#define __if_ok__step(...) __VA_ARGS__
#define __if_ok__parseExpr(_$Expr...) (_$Expr), __if_ok__parseCapture
#define __if_ok__parseCapture(_$capture...) _$capture
#define __if_ok__emit(_$Expr, _$capture...) \
    if_(let _result = _$Expr, _result.is_ok) \
        using_(let _$capture = _result.payload.ok)
#define comp_syn__if_ok_void(_$val_result) \
    if_(let _result = (_$val_result), _result.is_ok)
#define comp_syn__else_err(_$Payload_Capture) \
    else_(let _$Payload_Capture = _result.payload.err)
#define __if_err__step(...) __VA_ARGS__
#define __if_err__parseExpr(_$Expr...) (_$Expr), __if_err__parseCapture
#define __if_err__parseCapture(_$capture...) _$capture
#define __if_err__emit(_$Expr, _$capture...) \
    if_(let _result = _$Expr, !_result.is_ok) \
        using_(let _$capture = _result.payload.err)
#define comp_syn__else_ok(_$Payload_Capture) \
    else_(let _$Payload_Capture = _result.payload.ok)
#define comp_syn__else_ok_void \
    else

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
errset_((math_E)(
    DivisionByZero,
    Overflow,
    Underflow
));
T_use_E$($set(math_E)(i32)); // or Generally `T_use_E$(i32)`
$static fn_((safeDivide(i32 lhs, i32 rhs))(math_E$i32)) $must_check;
$static fn_((test(void))(E$void)) $must_check;

$static fn_((safeDivide(i32 lhs, i32 rhs))(math_E$i32) $scope) {
    if (rhs == 0) {
        return_err(math_E_DivisionByZero());
    }
    return_ok(lhs / rhs);
} $unscoped(fn);
$static fn_((test(void))(E$void) $scope) {
    let result_invalid = try_(safeDivide(10, 0));
    let result_default = catch_((safeDivide(10, 0))($ignore, 1));
    let result_handling = catch_((safeDivide(10, 0))(err, {
        E_print(err);
        ETrace_print();
        return_err(err);
    }));
    return_ok({});
} $unscoped(fn);
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_E_ErrRes__included */
