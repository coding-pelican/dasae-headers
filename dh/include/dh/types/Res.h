#ifndef types_Res__included
#define types_Res__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "dh/debug/common.h"
#include "raw.h"

/*========== Macros and Declarations ========================================*/

/* Result Anonymous */
#define E$$(_T...) \
    union { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, _T); \
                var_(raw, TypeOf(E_Payload$raw $like_ptr)); \
            } $like_ptr) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ptr)); \
    }
/* Result Alias */
#define $set                       $_set,
#define E$(...)                    pp_overload(__E$, __VA_ARGS__)(__VA_ARGS__)
#define __E$_1(_T...)              pp_join($, E, _T)
#define __E$_2(_tag, _ESet_T...)   pp_join(_, __E$, _tag)(__E$__expandESet _ESet_T)
#define __E$__expandESet(_ESet...) _ESet, __E$__expandT
#define __E$__expandT(_T...)       _T
#define __E$_$_set(...)            __E$__withESet(__VA_ARGS__)
#define __E$__withESet(_E, _T...)  pp_join($, _E, _T)
/* Result Template */
#define T_decl_E$(_T...) \
    /* $maybe_unused typedef union E$(P_const$(_T)) E$(P_const$(_T)); \
    $maybe_unused typedef union E$(P$(_T)) E$(P$(_T)); */ \
    $maybe_unused typedef union E$(_T) E$(_T)
#define T_impl_E$(...) pp_overload(__T_impl_E$, __VA_ARGS__)(__VA_ARGS__)
#define __T_impl_E$_1(_T...) \
    /* union E$(P_const$(_T)) { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, P_const$(_T)); \
                var_(raw, TypeOf(E_Payload$raw $like_ptr)); \
            } $like_ptr) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ptr)); \
    }; \
    union E$(P$(_T)) { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, P$(_T)); \
                var_(raw, TypeOf(E_Payload$raw $like_ptr)); \
            } $like_ptr) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ptr)); \
    }; */ \
    union E$(_T) { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, _T); \
                var_(raw, TypeOf(E_Payload$raw $like_ptr)); \
            } $like_ptr) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ptr)); \
    }
#define __T_impl_E$_2(_tag, _ESet_T...) \
    pp_join(_, __T_impl_E$, _tag)(__T_impl_E$__expandESet _ESet_T)
#define __T_impl_E$__expandESet(_ESet...) _ESet, __T_impl_E$__expandT
#define __T_impl_E$__expandT(_T...)       _T
#define __T_impl_E$_$_set(...)            __T_impl_E$__withESet(__VA_ARGS__)
#define __T_impl_E$__withESet(_E, _T...) \
    union E$($set(_E)(_T)) { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, _E); \
                var_(ok, _T); \
                var_(raw, TypeOf(E_Payload$raw $like_ptr)); \
            } $like_ptr) payload; \
        }; \
    }
#define T_use_E$(_T...) \
    T_decl_E$(_T); \
    T_impl_E$(_T)

/* Error void result (special case) */
typedef union E$Void {
    struct {
        var_(is_ok, bool);
        TypeOf(
            union {
                var_(err, Err);
                var_(ok, Void);
                var_(raw, TypeOf(E_Payload$raw $like_ptr));
            } $like_ptr) payload;
    };
    var_(as_raw, E$raw);
    var_(ref_raw, TypeOf(E$raw $like_ptr));
} E$Void, E$void;

/* Error Operations */
// #define init$E()
// #define init$E$()
// #define asg$E()
// #define asg$E$()

/* Determines error result */
#define ok(_val...) { \
    .is_ok = true, \
    .payload[0] = { .ok = _val }, \
}
#define ok$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__ok$)(__param_expand__ok$ __VA_ARGS__))
#define ok$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__ok$$)(__param_expand__ok$$ __VA_ARGS__))

#define err(_val...) { \
    .is_ok = false, \
    .payload[0] = { .err = _val }, \
}
#define err$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__err$)(__param_expand__err$ __VA_ARGS__))
#define err$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__err$$)(__param_expand__err$$ __VA_ARGS__))

/* Checks error result */
#define isOk(_e /*: E$$(_T)*/... /*(bool)*/)  as$((bool)((_e).is_ok))
#define isErr(_e /*: E$$(_T)*/... /*(bool)*/) as$((bool)(!(_e).is_ok))

/* Returns error result */
#define return_ok(_val...) \
    return_(ok(_val))
#define return_err(_val...) \
    ($debug_point ErrTrace_captureFrame()); \
    return_(err(_val))

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
#define if_err(val_result, _Payload_Capture) comp_syn__if_err(val_result, _Payload_Capture)
#define else_ok(_Payload_Capture)            comp_syn__else_ok(_Payload_Capture)
#define else_void                            comp_syn__else_void
#define if_ok(val_result, _Payload_Capture)  comp_syn__if_ok(val_result, _Payload_Capture)
#define if_void(val_result)                  comp_syn__if_void(val_result)
#define else_err(_Payload_Capture)           comp_syn__else_err(_Payload_Capture)

/*========== Macros and Definitions =========================================*/

#define __param_expand__ok$(...)          __VA_ARGS__, pp_expand
#define __block_inline__ok$(...)          __block_inline1__ok$(__VA_ARGS__)
#define __block_inline1__ok$(_T, _val...) lit$((E$(_T))ok(_val))

#define __param_expand__ok$$(...)           __VA_ARGS__, pp_expand
#define __block_inline__ok$$(...)           __block_inline1__ok$$$(__VA_ARGS__)
#define __block_inline1__ok$$$(_T, _val...) lit$((E$$(_T))ok(_val))

#define __param_expand__err$(...)          __VA_ARGS__, pp_expand
#define __block_inline__err$(...)          __block_inline1__err$(__VA_ARGS__)
#define __block_inline1__err$(_T, _val...) lit$((E$(_T))err(_val))

#define __param_expand__err$$(...)           __VA_ARGS__, pp_expand
#define __block_inline__err$$(...)           __block_inline1__err$$$(__VA_ARGS__)
#define __block_inline1__err$$$(_T, _val...) lit$((E$$(_T))err(_val))

#define __try_(__result, _Expr...) ({ \
    let __result = _Expr; \
    if (isErr(__result)) { \
        return_err(__result.payload->err); \
    } \
    __result.payload->ok; \
})
#define __param_expand__catch_(...)                                                         __VA_ARGS__, pp_expand
#define __block_inline__catch_(_Expr, _Payload_Capture, _DefaultExpr_OR_Body...)            __block_inline1__catch_(pp_uniqTok(result), _Expr, _Payload_Capture, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__catch_(__result, _Expr, _Payload_Capture, _DefaultExpr_OR_Body...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isErr(__result)) { \
            let _Payload_Capture = __result.payload->err; \
            __result.payload->ok = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    _DefaultExpr_OR_Body; \
                    lit$((TypeOf(__result.payload->ok)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
            ErrTrace_reset(); \
        } \
        __result.payload->ok; \
    }) \
)
#define __errdefer_(_Payload_Capture, _Expr...) defer_(if (!__reserved_return->is_ok) { \
    let _Payload_Capture = __reserved_return->payload->err; \
    _Expr; \
})

#define comp_syn__if_err(val_result, _Payload_Capture) \
    scope_if(let _result = (val_result), !_result.is_ok) \
        scope_with(let _Payload_Capture = _result.payload->err)
#define comp_syn__else_ok(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.payload->ok)
#define comp_syn__else_void \
    else
#define comp_syn__if_ok(val_result, _Payload_Capture) \
    scope_if(let _result = (val_result), _result.is_ok) \
        scope_with(let _Payload_Capture = _result.payload->ok)
#define comp_syn__if_void(val_result) \
    scope_if(let _result = (val_result), _result.is_ok)
#define comp_syn__else_err(_Payload_Capture) \
    scope_else(let _Payload_Capture = _result.payload->err)

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
#endif /* types_Res__included */
