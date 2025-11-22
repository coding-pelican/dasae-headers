#ifndef types_ErrRes__included
#define types_ErrRes__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "raw.h"

/*========== Macros and Declarations ========================================*/

#define $set $_set,
/* Result Anonymous */
#define E$$(...) pp_overload(__E$$, __VA_ARGS__)(__VA_ARGS__)
#define __E$$_1(_T...) TypeOf(union { \
    struct { \
        var_(is_ok, bool); \
        union { \
            var_(err, Err); \
            var_(ok, _T); \
        } payload; \
    }; \
    var_(as_raw $like_ref, E$raw); \
})
#define __E$$_2(_tag, _ESet_T...) pp_join(_, __E$$, _tag)(__E$$__expandESet _ESet_T)
#define __E$$__expandESet(_ESet...) _ESet, __E$$__expandT
#define __E$$__expandT(_T...) _T
#define __E$$_$_set(...) __E$$__withESet(__VA_ARGS__)
#define __E$$__withESet(_E, _T...) TypeOf(union { \
    struct { \
        var_(is_ok, bool); \
        union { \
            var_(err, _E); \
            var_(ok, _T); \
        } payload; \
    }; \
    var_(as_raw $like_ref, E$raw); \
})
/* Result Alias */
#define E$(...) pp_overload(__E$, __VA_ARGS__)(__VA_ARGS__)
#define __E$_1(_T...) pp_join($, E, _T)
#define __E$_2(_tag, _ESet_T...) pp_join(_, __E$, _tag)(__E$__expandESet _ESet_T)
#define __E$__expandESet(_ESet...) _ESet, __E$__expandT
#define __E$__expandT(_T...) _T
#define __E$_$_set(...) __E$__withESet(__VA_ARGS__)
#define __E$__withESet(_E, _T...) pp_join($, _E, _T)
/* Result Template */
#define T_decl_E$(_T...) \
    $maybe_unused typedef union E$(_T) E$(_T)
#define T_impl_E$(...) pp_overload(__T_impl_E$, __VA_ARGS__)(__VA_ARGS__)
#define __T_impl_E$_1(_T...) \
    union E$(_T) { \
        struct { \
            var_(is_ok, bool); \
            union { \
                var_(err, Err); \
                var_(ok, _T); \
            } payload; \
        }; \
        var_(as_raw $like_ref, E$raw); \
    }
#define __T_impl_E$_2(_tag, _ESet_T...) \
    pp_join(_, __T_impl_E$, _tag)(__T_impl_E$__expandESet _ESet_T)
#define __T_impl_E$__expandESet(_ESet...) _ESet, __T_impl_E$__expandT
#define __T_impl_E$__expandT(_T...) _T
#define __T_impl_E$_$_set(...) __T_impl_E$__withESet(__VA_ARGS__)
#define __T_impl_E$__withESet(_E, _T...) \
    union E$($set(_E)(_T)) { \
        struct { \
            var_(is_ok, bool); \
            union { \
                var_(err, _E); \
                var_(ok, _T); \
            } payload; \
        }; \
        var_(as_raw $like_ref, E$raw); \
    }
#define T_use_E$(_T...) \
    T_decl_E$(_T); \
    T_impl_E$(_T)

#define E_InnerE$(_T...) TypeOf((as$(_T*)(null))->payload.err)
#define E_InnerT$(_T...) TypeOf((as$(_T*)(null))->payload.ok)

/* Error void result (special case) */
typedef union E$Void {
    struct {
        var_(is_ok, bool);
        union {
            var_(err, Err);
            var_(ok, Void);
        } payload;
    };
    var_(as_raw $like_ref, E$raw);
} E$Void, E$void;

/* Error Operations */
// #define init$E()
// #define init$E$()
// #define asg$E()
// #define asg$E$()

#define asgE(_p_e, _v_e...) asgE1(_p_e, _v_e)
#define asgE1(_p_e, _v_e...) asg(_p_e, _v_e, (payload.ok))
#define asgE2(_p_e, _v_e...) asg(_p_e, _v_e, (payload.ok.payload.ok))
#define asgE3(_p_e, _v_e...) asg(_p_e, _v_e, (payload.ok.payload.ok.payload.ok))
#define asgE4(_p_e, _v_e...) asg(_p_e, _v_e, (payload.ok.payload.ok.payload.ok.payload.ok))

/* Determines error result */
#define ok(_val...) { \
    .is_ok = true, \
    .payload = {.ok = _val}, \
}
#define ok$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__ok$)(__param_expand__ok$ __VA_ARGS__))
#define ok$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__ok$$)(__param_expand__ok$$ __VA_ARGS__))

#define err(_val...) { \
    .is_ok = false, \
    .payload = {.err = _val}, \
}
#define err$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__err$)(__param_expand__err$ __VA_ARGS__))
#define err$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__err$$)(__param_expand__err$$ __VA_ARGS__))

/* Checks error result */
#define isOk(_e /*: E$$(_T)*/... /*(bool)*/) as$(bool)((_e).is_ok)
#define isErr(_e /*: E$$(_T)*/... /*(bool)*/) as$(bool)(!(_e).is_ok)

#define E_asP$(/*(_E: E(P(T)))(_p_e: P(E(T)))*/... /*(_E)*/) \
    __step__E_asP$(__step__E_asP$__parseE __VA_ARGS__)
#define E_asP(_p_e /*: P(E(T))*/... /*(E(P(T)))*/) \
    __step__E_asP(_p_e)

/* Returns error result */
#define return_ok(_val...) \
    (return_(ok(_val)))
#define return_ok_void(_Expr...) pp_overload(__return_ok_void, _Expr)(_Expr)
#define return_err(_val...) \
    ($debug_point ErrTrace_captureFrame(), return_(err(_val)))

/* Propagates error (similar to Zig's try_) */
#define try_(/*(_Expr: E$$(_T))*/... /*(_T)*/) \
    __try_(pp_uniqTok(result), __VA_ARGS__)
/* Handles error (similar to Zig's catch_from) */
#define catch_(/*(_Expr: E$$(_T))(_Capture|$ignore, _DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__catch_)(__param_expand__catch_ __VA_ARGS__))
/* Defers when error */
#define errdefer_(/*(_O_Capture|$ignore, _Expr_OR_Body...:void)*/... /*(void)*/) \
    __errdefer_(__VA_ARGS__)

/* Error result payload captures */
#define if_ok(/*(_Expr)(_capture)*/...) __if_ok__step(pp_defer(__if_ok__emit)(__if_ok__parseExpr __VA_ARGS__))
#define if_ok_void(val_result) comp_syn__if_ok_void(val_result)
#define else_err(_Payload_Capture) comp_syn__else_err(_Payload_Capture)
#define if_err(/*(_Expr)(_capture)*/...) __if_err__step(pp_defer(__if_err__emit)(__if_err__parseExpr __VA_ARGS__))
#define else_ok(_Payload_Capture) comp_syn__else_ok(_Payload_Capture)
#define else_ok_void comp_syn__else_ok_void

/*========== Macros and Definitions =========================================*/

#define __param_expand__ok$(...) __VA_ARGS__,
#define __block_inline__ok$(...) __block_inline1__ok$(__VA_ARGS__)
#define __block_inline1__ok$(_T, _val...) lit$((E$(_T))ok(_val))

#define __param_expand__ok$$(...) __VA_ARGS__,
#define __block_inline__ok$$(...) __block_inline1__ok$$$(__VA_ARGS__)
#define __block_inline1__ok$$$(_T, _val...) lit$((E$$(_T))ok(_val))

#define __param_expand__err$(...) __VA_ARGS__,
#define __block_inline__err$(...) __block_inline1__err$(__VA_ARGS__)
#define __block_inline1__err$(_T, _val...) lit$((E$(_T))err(_val))

#define __param_expand__err$$(...) __VA_ARGS__,
#define __block_inline__err$$(...) __block_inline1__err$$$(__VA_ARGS__)
#define __block_inline1__err$$$(_T, _val...) lit$((E$$(_T))err(_val))

#define __step__E_asP$(...) \
    __step__E_asP$__emit(__VA_ARGS__)
#define __step__E_asP$__parseOPT(_EPT...) \
    _EPT, __step__E_asP$__parsePE
#define __step__E_asP$__parsePE(_p_e...) \
    pp_uniqTok(p_e), _p_e
#define __step__E_asP$__emit(_EPT, __p_e, _p_e...) ({ \
    typedef _EPT E$Ret$E_asP; \
    let_(__p_e, TypeOf(_p_e)) = _p_e; \
    claim_assert_nonnull(__p_e); \
    __p_e->is_ok \
        ? lit$((E$Ret$E_asP)ok(&__p_e->payload.ok)) \
        : lit$((E$Ret$E_asP)err(__p_e->payload.err)); \
})
#define __step__E_asP(_p_e...) \
    E_asP$((E$$(FieldType$(TypeOf(*_p_e), payload.some)*))(_p_e))

#define __return_ok_void_0() (return_(ok({})))
#define __return_ok_void_1(_Expr...) (_Expr, return_(ok({})))

#define __try_(__result, _Expr...) ({ \
    let __result = _Expr; \
    if (isErr(__result)) { \
        return_err(__result.payload.err); \
    } \
    __result.payload.ok; \
})
#define __param_expand__catch_(...) __VA_ARGS__, pp_expand
#define __block_inline__catch_(_Expr, _Payload_Capture, _DefaultExpr_OR_Body...) __block_inline1__catch_(pp_uniqTok(result), _Expr, _Payload_Capture, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__catch_(__result, _Expr, _Payload_Capture, _DefaultExpr_OR_Body...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isErr(__result)) { \
            claim_assert(__result.payload.err.ctx != 0); \
            claim_assert_nonnull(__result.payload.err.vt); \
            let _Payload_Capture = __result.payload.err; \
            __result.payload.ok = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    _DefaultExpr_OR_Body; \
                    lit$((TypeOf(__result.payload.ok)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
            ErrTrace_reset(); \
        } \
        __result.payload.ok; \
    }) \
)
#define __errdefer_(_Payload_Capture, _Expr...) defer_(if (!__reserved_return->is_ok) { \
    claim_assert(__reserved_return->payload.err.ctx != 0); \
    claim_assert_nonnull(__reserved_return->payload.err.vt); \
    let _Payload_Capture = __reserved_return->payload.err; \
    _Expr; \
})

#define __if_ok__step(...) __VA_ARGS__
#define __if_ok__parseExpr(_Expr...) (_Expr), __if_ok__parseCapture
#define __if_ok__parseCapture(_capture...) _capture
#define __if_ok__emit(_Expr, _capture...) \
    if_(let _result = _Expr, _result.is_ok) \
        with_(let _capture = _result.payload.ok)
#define comp_syn__if_ok_void(val_result) \
    scope_if(let _result = (val_result), _result.is_ok)
#define comp_syn__else_err(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.payload.err)
#define __if_err__step(...) __VA_ARGS__
#define __if_err__parseExpr(_Expr...) (_Expr), __if_err__parseCapture
#define __if_err__parseCapture(_capture...) _capture
#define __if_err__emit(_Expr, _capture...) \
    if_(let _result = _Expr, !_result.is_ok) \
        with_(let _capture = _result.payload.err)
#define comp_syn__else_ok(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.payload.ok)
#define comp_syn__else_ok_void \
    else

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));
T_use_E$($set(math_Err)(i32)); // or Generally `T_use_E$(i32)`
$static fn_((safeDivide(i32 lhs, i32 rhs))(math_E$i32)) $must_check;
$static fn_((test(void))(E$void)) $must_check;

$static fn_((safeDivide(i32 lhs, i32 rhs))(math_E$i32) $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(lhs / rhs);
} $unscoped_(fn);
$static fn_((test(void))(E$void) $scope) {
    let result_invalid  = try_(safeDivide(10, 0));
    let result_default  = catch_((safeDivide(10, 0))($ignore, 1));
    let result_handling = catch_((safeDivide(10, 0))(err, {
        Err_print(err);
        ErrTrace_print();
        return_err(err);
    }));
    return_ok({});
} $unscoped_(fn);
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* types_ErrRes__included */
