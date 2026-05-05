#include "dh-main.h"
#include "dh/time.h"
#include "dh/io/stream.h"
#include "draft-tup.h"

typedef V$raw FnCtx$raw;
typedef fn_(((*Fn$raw)(FnCtx$raw* ctx))(FnCtx$raw*));
typedef struct Closure$raw {
    Fn$raw entry;
    FnCtx$raw ctx $like_ref;
} Closure$raw;

#define FnArgs$(_fn...) __alias__FnArgs$(_fn)
#define __alias__FnArgs$(_fn...) pp_join($, FnArgs, _fn)
#define FnRet$(_fn...) __alias__FnRet$(_fn)
#define __alias__FnRet$(_fn...) pp_join($, FnRet, _fn)
#define FnCtx$(_fn...) __alias__FnCtx$(_fn)
#define __alias__FnCtx$(_fn...) pp_join($, FnCtx, _fn)
#define fn$(_fn...) __alias__fn$(_fn)
#define __alias__fn$(_fn...) pp_join($, fn, _fn)
#define Closure$(_fn...) __alias__Closure$(_fn)
#define __alias__Closure$(_fn...) pp_join($, Closure, _fn)
#define closure$(_fn...) __alias__closure$(_fn)
#define __alias__closure$(_fn...) pp_join($, closure, _fn)
#define fn_use_Closure$(/*(_fn)(_Arg_Type...)(_Ret_Type)*/...) __stmt__fn_use_Closure$(__VA_ARGS__)
#define __stmt__fn_use_Closure$(...) __step__fn_use_Closure$(__step__fn_use_Closure$__parse0 __VA_ARGS__)
#define __step__fn_use_Closure$__parse0(_fn...) \
    _fn, __step__fn_use_Closure$__parse1
#define __step__fn_use_Closure$__parse1(_Arg_Type...) \
    pp_countArg(_Arg_Type), (_Arg_Type), __step__fn_use_Closure$__parse2
#define __step__fn_use_Closure$__parse2(_Ret_Type...) \
    _Ret_Type
#define __step__fn_use_Closure$(...) __inline__fn_use_Closure$(__VA_ARGS__)
#define __inline__fn_use_Closure$(_fn, _N_Arg_Type, _Arg_Type, _Ret_Type...) \
    T_alias$((FnArgs$(_fn))(Tup$$ _Arg_Type)); \
    T_alias$((FnRet$(_fn))(_Ret_Type)); \
    T_alias$((FnCtx$(_fn))(union FnCtx$(_fn) { \
        T_embed$(struct { \
            var_(args, FnArgs$(_fn)); \
            var_(ret, FnRet$(_fn)); \
        }); \
        var_(as_raw, FnCtx$raw) $like_ref; \
    })); \
    $attr($inline_always $static) \
    fn_((fn$(_fn)(P$$(FnCtx$raw) ctx))(P$$(FnCtx$raw))) { \
        let fn = _fn; \
        let fn_ctx = ptrAlignCast$((FnCtx$(_fn)*)(ctx)); \
        fn_ctx->ret = fn(__step__fn_use_Closure$__passTupFields(_N_Arg_Type, fn_ctx->args.)); \
        return fn_ctx->as_raw; \
    }; \
    T_alias$((Closure$(_fn))(union Closure$(_fn) { \
        T_embed$(struct { \
            var_(entry, Fn$raw); \
            var_(ctx, FnCtx$(_fn)) $like_ref; \
        }); \
        var_(as_raw, Closure$raw) $like_ref; \
    })); \
    $attr($inline_always $static) \
    fn_((closure$(_fn)(__step__fn_use_Closure$__tupFieldsToParams(_N_Arg_Type, _Arg_Type)))(Closure$(_fn))) { \
        return (Closure$(_fn)){ \
            .entry = fn$(_fn), \
            .ctx $like_deref = { .args = { __step__fn_use_Closure$__passTupFields(_N_Arg_Type) } } \
        }; \
    };

#define __step__fn_use_Closure$__passTupFields(_N_Arg_Type, _field_path...) \
    pp_cat(__step__fn_use_Closure$__passTupFields, _N_Arg_Type)(_field_path)
#define __step__fn_use_Closure$__passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Closure$__passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Closure$__passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Closure$__passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3

#define __step__fn_use_Closure$__tupFieldsToParams(_N_Arg_Type, _Arg_Type...) \
    pp_cat(__step__fn_use_Closure$__tupFieldsToParams, _N_Arg_Type) _Arg_Type
#define __step__fn_use_Closure$__tupFieldsToParams1(_Arg_Type1...) \
    _Arg_Type1 $0
#define __step__fn_use_Closure$__tupFieldsToParams2(_Arg_Type1, _Arg_Type2...) \
    _Arg_Type1 $0, _Arg_Type2 $1
#define __step__fn_use_Closure$__tupFieldsToParams3(_Arg_Type1, _Arg_Type2, _Arg_Type3...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2
#define __step__fn_use_Closure$__tupFieldsToParams4(_Arg_Type1, _Arg_Type2, _Arg_Type3, _Arg_Type4...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2, _Arg_Type4 $3

#define invoke(_p_closure...) __expr__callClosure$(_p_closure)
#define __expr__callClosure$(_p_closure...) __inline__callClosure$(pp_uniqTok(p_fn_handle), _p_closure)
#define __inline__callClosure$(__p_closure, _p_closure...) local_({ \
    let __p_closure = _p_closure; \
    ptrAlignCast$((FieldType$(TypeOf(*__p_closure), ctx $like_deref)*)(__p_closure->entry(as$(FnCtx$raw*)(__p_closure->ctx)))); \
})

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[{:s}] "), label);
    using_(var args = l0$((va_list))) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
};

$static fn_((countFn(usize n, time_Duration interval, S_const$u8 label))(f64)) {
    let instant = time_Instant_now();
    report(label, u8_l("before loop {:fl}"), interval);

    for_(($rt(n))(i)) {
        time_sleep(interval);
        report(label, u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), interval, i, n);
    } $end(for);

    let elapsed = pipe_((instant)(
        (t)(time_Instant_elapsed(t)),
        (t)(time_Duration_asSecs$f64(t))
    ));
    report(label, u8_l("after loop {:fl}"), elapsed);
    return elapsed;
};
fn_use_Closure$((countFn)(usize, time_Duration, S_const$u8)(f64));
#pragma region expand_fn_use_Closure
// T_alias$((FnArgs$countFn)(Tup$$(usize, time_Duration, S_const$u8)));
// T_alias$((FnRet$countFn)(f64));
// typedef union FnCtx$countFn {
//     T_embed$(union {
//         var_(args, FnArgs$countFn);
//         var_(ret, FnRet$countFn);
//     });
//     var_(as_raw, FnCtx$raw) $like_ref;
// } FnCtx$countFn;
// $attr($inline_always)
// $static fn_((fn$countFn(FnCtx$raw* ctx))(FnCtx$raw*)) {
//     let fn = countFn;
//     let fn_ctx = ptrAlignCast$((FnCtx$countFn*)(ctx));
//     fn_ctx->ret = fn(
//         fn_ctx->args.$0,
//         fn_ctx->args.$1,
//         fn_ctx->args.$2
//     );
//     return fn_ctx->as_raw;
// };
// typedef union Closure$countFn {
//     T_embed$(struct {
//         var_(fn, Fn$raw);
//         var_(ctx, FnCtx$countFn) $like_ref;
//     });
//     var_(as_raw, Closure$raw) $like_ref;
// } Closure$countFn;
// $attr($inline_always)
// $static fn_((closure$countFn(usize $0, time_Duration $1, S_const$u8 $2))(Closure$countFn)) {
//     return (Closure$countFn){
//         .fn = fn$countFn,
//         .ctx $like_deref = { .args = { $0, $1, $2 } }
//     };
// };
#pragma endregion expand_fn_use_Closure

/* Coroutines enforce the use of closures. (co_use_Closure) is mandatory. */
#pragma region coroutine_goals_syntax
#if UNUSED_CODE
$static co_fn_(((countCo)(usize n, time_Duration interval, S_const$u8 label))(f64)$scope(
    $co_locals({
        var_(instant, time_Instant);
        var_(i, usize);
        var_(elapsed, f64);
    }),
    $co_locals_mut({})
)) {
    co_let_((instant)(time_Instant_now()));
    report($co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    co_for_(($rt($co_arg(n)))(i)) {
        time_sleep($co_arg(interval));
        report($co_arg(label), u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), $co_arg(interval), $co(i), $co_arg(n));
    } $end(co_for);

    co_let_((elapsed)(pipe_(($co(instant))(
        (t)(time_Instant_elapsed(t)),
        (t)(time_Duration_asSecs$f64(t))
    ))));
    report($co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
co_use_Closure$((countCo)(usize, time_Duration, S_const$u8)(f64));
#endif /* UNUSED_CODE */
#pragma endregion coroutine_goals_syntax

#define $co_arg(_ident...) (__args->_ident)
#define co_let_(_ident, _expr...) __ctx->locals._ident = _expr
#define co_var_(_ident, _expr...) __locals_mut->_ident = _expr
#define $co(_ident...) (__locals->_ident)
#define $co_mut(_ident...) (__locals_mut->_ident)
#define co_return_(_expr...) return_((((*__ret) = _expr), __ctx))

#pragma region coroutine_poc_syntax
/* clang-format off */
#if UNUSED_CODE
$static co_fn_(countCo, (usize n, time_Duration interval, S_const$u8 label), f64, $scope(
    $co_locals({
        var_(instant, time_Instant);
        var_(i, usize);
        var_(elapsed, f64);
    }),
    $co_locals_mut({})
)) {
    co_let_(instant, time_Instant_now());
    report($co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    co_for_(($rt($co_arg(n)))(i)) {
        time_sleep($co_arg(interval));
        report($co_arg(label), u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), $co_arg(interval), $co(i), $co_arg(n));
    } $end(co_for);

    co_let_(elapsed, pipe_(($co(instant))(
        (t)(time_Instant_elapsed(t)),
        (t)(time_Duration_asSecs$f64(t))
    )));
    report($co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));
    co_return_($co(elapsed));
} $unscoped(co_fn);
#endif /* UNUSED_CODE */
/* clang-format on */
#pragma endregion coroutine_poc_syntax

#pragma region expand_co_poc_syntax
typedef Fn$raw Co$raw;
typedef FnCtx$raw CoCtx$raw;
typedef struct Co$countCo {
    struct {
        usize n;
        time_Duration interval;
        S_const$u8 label;
    } args; // <- memcpy CoArgs$countCo
    f64 ret;
    struct {
        var_(instant, time_Instant);
        var_(i, usize);
        var_(elapsed, f64);
    } locals;
    struct {
    } locals_mut;
} Co$countCo;
#define co_called_(_ctx...) \
    let __ctx = _ctx; \
    $attr($maybe_unused) let __args = &__ctx->args; \
    $attr($maybe_unused) let __ret = &__ctx->ret; \
    $attr($maybe_unused) let_const __locals = &__ctx->locals; \
    $attr($maybe_unused) let __locals_mut = &__ctx->locals_mut;
$static fn_((countCo(Co$countCo* ctx))(Co$countCo*) $scope) {
    co_called_(ctx);

    co_let_(instant, time_Instant_now());
    report($co_arg(label), u8_l("before loop {:fl}"), $co_arg(interval));

    for_(($rt($co_arg(n)))(i)) {
        time_sleep($co_arg(interval));
        report($co_arg(label), u8_l("slept {:fl} | i: {:uz} < n: {:uz}"), $co_arg(interval), i, $co_arg(n));
    } $end(for);

    co_let_(
        elapsed,
        pipe_(($co(instant))(
            (t)(time_Instant_elapsed(t)),
            (t)(time_Duration_asSecs$f64(t))
        ))
    );
    report($co_arg(label), u8_l("after loop {:fl}"), $co(elapsed));

    co_return_($co(elapsed));
} $unscoped(fn);
typedef struct {
    usize $0;
    time_Duration $1;
    S_const$u8 $2;
} CoArgs$countCo;
typedef f64 CoRet$countCo;
typedef union CoCtx$countCo {
    Co$countCo frame[1];
    CoCtx$raw as_raw[1];
} CoCtx$countCo;
$attr($inline_always)
$static fn_((co$countCo(CoCtx$raw* ctx))(CoCtx$raw*)) {
    let co = countCo;
    let co_ctx = ptrAlignCast$((CoCtx$countCo*)(ctx));
    co_ctx->frame->ret = co(co_ctx->frame)->ret;
    return co_ctx->as_raw;
};
T_alias$((Closure$countCo)(union Closure$countCo {
    T_embed$(struct {
        var_(entry, Co$raw);
        var_(ctx, Co$countCo) $like_ref;
    });
    var_(as_raw, Closure$raw) $like_ref;
}));
$attr($inline_always $static)
fn_((closure$countCo(usize $0, time_Duration $1, S_const$u8 $2))(Closure$countCo)) {
    return (Closure$countCo){
        .entry = co$countCo,
        .ctx $like_deref = { .args = { $0, $1, $2 } }
    };
};
/* place holder for co_use_Closure$((countCo)(usize, time_Duration, S_const$u8)(f64)); */
#pragma endregion expand_co_poc_syntax

/*
fn_use_Closure와 co_use_Closure를 분리하므로서 동일한 API패턴에 다른 상세 구현을 지원하게되어
단일 closure$ syntax 만으로 정규함수와 코루틴함수를 동시에 지원할 수 있다.
*/
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    {
        let count = closure$(countFn);
        var closure = count(10, time_Duration_fromMillis(100), u8_l("count"));
        io_stream_print(u8_l("fn_ret: {:fl}\n"), invoke(&closure)->ret);
    }
    {
        let count = closure$(countCo);
        var closure = count(10, time_Duration_fromMillis(100), u8_l("count"));
        io_stream_print(u8_l("co_ret: {:fl}\n"), invoke(&closure)->ret);
    }
    return_ok({});
} $unscoped(fn);
